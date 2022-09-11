// TicTacToe.cpp : This file contains the 'main' function for CLI Tic-Tac-Toe.
// 
// Author: Andrew Dey

#include <string>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <type_traits>

namespace TicTacToe
{

	namespace Utilities
	{
		/// Gets input from std::cin, with a given prompt.
		/// If input is empty, or blank (all ASCII spaces), then it will be considered invalid.
		/// If input is invalid, the user is prompted again.
		std::string* getInput(std::string prompt, std::string* dest)
		{
			do
			{
				std::cout << prompt;
				std::getline(std::cin, *dest);

				if (dest->empty())
				{
					continue;
				}

				bool isValid = false;

				for (int i = 0; i < dest->length(); i++)
				{
					if ((*dest)[i] != ' ')
					{
						isValid = true;
						break;
					}
				}

				if (isValid)
				{
					break;
				}

			} while (true);

			return dest;
		}

		std::vector<std::string>* split(std::string* str, const std::string* delimiter, std::vector<std::string>* dest)
		{
			size_t last(0);
			size_t next(0);

			if ((next = str->find(*delimiter, last)) != std::string::npos)
			{
				dest->push_back(str->substr(last, next - last));
				last = next + 1;
			}
			else
			{
				dest->push_back(*str);
				return dest;
			}

			while (last < str->length())
			{
				next = str->find(*delimiter, last);
				dest->push_back(str->substr(last, next - last));

				if (next == std::string::npos)
				{
					break;
				}

				last = next + 1;
			}

			return dest;
		}

		std::vector<std::string>* split(std::string* str, std::vector<std::string>* dest)
		{
			std::string delimiter = " ";
			return split(str, &delimiter, dest);
		}

		/// Converts US-ASCII characters to their lowercase counterparts, using std::tolower.
		/// Note: does not support anything other than ascii.
		void stringToLower(std::string* str)
		{
			std::transform(str->begin(), str->end(), str->begin(), [](unsigned char c) { return std::tolower(c); });
		}

		/// Checks whether the given values in 'nums' are within the given 'min' and 'max' values.
		/// Note that types 'T' and 'TVA' need to be the same.
		template<
			typename T,
			typename... TVA,
			typename = std::enable_if_t<std::conjunction<std::is_same<T, TVA>...>::value, bool>
		>
		bool withinRange(T min, T max, TVA... nums)
		{
			for (T n : {nums...})
			{
				if (n < min || n > max)
				{
					return false;
				}
			}

			return true;
		}
	}

	struct Player
	{
		int number;
		std::string* name;
	};

	enum BoardPrintType
	{
		Name,
		Contents,
		All,
	};

	class Board
	{
	public:
		const std::string DEFAULT_BOARD_NAME = "Unnamed Board";

		const int WIN_CONDITIONS[8][3] = {
			{0, 1, 2},
			{3, 4, 5},
			{6, 7, 8},
			{0, 3, 6},
			{1, 4, 7},
			{2, 5, 8},
			{0, 4, 8},
			{2, 4, 6}
		};

		char board[9];
		std::string boardName;

		Board() : board()
		{
			boardName = DEFAULT_BOARD_NAME;
			initializeBoard();
		}

		Board(std::string* name) : board(), boardName(*name)
		{
			initializeBoard();
		}

		void print()
		{
			print(All);
		}

		void print(BoardPrintType printType)
		{
			switch (printType)
			{
			case Name:
				printBoardName();
				break;
			case Contents:
				printBoardContents();
				break;
			case All:
				printBoardName();
				printBoardContents();
				break;
			}
		}

		bool isSpaceAvailable(int row, int col)
		{
			return board[(3 * row) + col] == ' ';
		}

		void play(int row, int col, int playerNumber)
		{
			board[(3 * row) + col] = playerNumber == 1 ? 'X' : 'O';
		}

		bool boardIsFilled()
		{
			for (int i = 0; i < 9; i++)
			{
				if (board[i] == ' ')
				{
					return false;
				}
			}

			return true;
		}

		int hasWinner()
		{
			const char player1 = 'X';
			const char player2 = 'O';

			for (const int* winCondition : WIN_CONDITIONS)
			{
				if (board[winCondition[0]] == ' ' || board[winCondition[1]] == ' ' || board[winCondition[2]] == ' ')
				{
					continue;
				}

				if (board[winCondition[0]] == board[winCondition[1]] && board[winCondition[0]] == board[winCondition[2]])
				{
					return player1 == board[winCondition[0]] ? 1 : 2;
				}
			}

			if (boardIsFilled())
			{
				return 3;
			}

			return 0;
		}

	private:
		void initializeBoard()
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					board[(3 * i) + j] = ' ';
				}
			}
		}

		void printBoardName()
		{
			std::cout << "Board: " << boardName << ".\n";
		}

		void printBoardContents()
		{
			std::cout << "     1   2   3  \n   -------------\n";
			std::cout << " 1 | " << board[0] << " | " << board[1] << " | " << board[2] << " |\n";
			std::cout << "   |-----------|\n";
			std::cout << " 2 | " << board[3] << " | " << board[4] << " | " << board[5] << " |\n";
			std::cout << "   |-----------|\n";
			std::cout << " 3 | " << board[6] << " | " << board[7] << " | " << board[8] << " |\n";
			std::cout << "   -------------\n\n";
		}
	};

	namespace Commands
	{

		bool exitCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			std::cout << "Exiting game...\n";
			exit(0);

			return true;
		}

		const std::unordered_map<std::string, BoardPrintType> showOptions = {
			{"name", Name},
			{"boardname", Name},
			{"contents", Contents},
			{"board", Contents},
			{"all", All}
		};

		bool showCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			if (command->size() < 2)
			{
				// default to showing the board
				board->print();
				return false;
			}

			Utilities::stringToLower(&command->at(1));

			if (showOptions.count(command->at(1)) < 1)
			{
				std::cout << "Nothing to show for '" << command->at(1) << "'. Displaying default.\n";
				board->print();
				return false;
			}

			BoardPrintType printType = showOptions.at(command->at(1));
			board->print(printType);

			return false;
		}

		bool playCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			if (command->size() < 3)
			{
				std::cout << "Usage: play <column 1, 2, or 3> <row 1, 2, or 3>\n";
				return false;
			}

			int row;
			int column;

			try
			{
				row = std::stoi(command->at(1));
				column = std::stoi(command->at(2));
			}
			catch (std::logic_error const& exception)
			{
				std::cout << "Invalid row/column.\n";
				std::cout << "Usage: play <column 1, 2, or 3> <row 1, 2, or 3>\n";

				return false;
			}

			if (!Utilities::withinRange(1, 3, column, row))
			{
				std::cout << "Invalid row/column.\n";
				std::cout << "Usage: play <column 1, 2, or 3> <row 1, 2, or 3>\n";

				return false;
			}

			if (!board->isSpaceAvailable(row - 1, column - 1))
			{
				std::cout << "Space (" << row << ", " << column << ") has already been played!\n";

				return false;
			}

			board->play(row - 1, column - 1, player->number);

			// players will want to know what the board looks like after a successful play
			board->print(Contents);

			return true;
		}

		const std::unordered_map<std::string, bool (*)(std::vector<std::string>*, Board*, Player*)> commands = {
			{"exit", &exitCommand},
			{"show", &showCommand},
			{"play", &playCommand}
		};

		/// Returns whether a command was found.
		/// Note: does not support anything other than ascii.
		bool processCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			Utilities::stringToLower(&command->at(0));

			if (commands.count(command->at(0)) < 1)
			{
				std::cout << "No command '" << command->at(0) << "' was found.\n";
				return false;
			}

			auto nextCommand = commands.at(command->at(0));
			return nextCommand(command, board, player);
		}
	}

	void runGame(Player* player1, Player* player2)
	{
		std::string boardName = (*player1->name + " vs " + *player2->name);
		Board board = (&boardName);

		board.print();

		// I mean... It's fine, I guess?
		// I should look farther into that C++11 api...
		namespace chrono = std::chrono;
		const chrono::nanoseconds timeSeed = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch());

		std::srand((unsigned int)timeSeed.count());

		const int firstPlayerTurn = (std::rand() % 2);
		Player currentPlayer = firstPlayerTurn == 0 ? *player2 : *player1;

		std::string playerCommand;
		std::vector<std::string> parsedPlayerCommand = {};

		int winner = 0;

		try
		{
			while (!winner)
			{
				Utilities::getInput(*currentPlayer.name + "'s turn: ", &playerCommand);
				Utilities::split(&playerCommand, &parsedPlayerCommand);

				if (Commands::processCommand(&parsedPlayerCommand, &board, &currentPlayer))
				{
					currentPlayer = currentPlayer.number == 1 ? *player2 : *player1;
				}

				parsedPlayerCommand.clear();
				winner = board.hasWinner();
			}

		}
		catch (std::exception& exception)
		{
			winner = -1;
			std::cerr << exception.what();
		}

		switch (winner)
		{
		case 1:
			std::cout << player1->name << " wins!\n";
			break;
		case 2:
			std::cout << player2->name << " wins!\n";
			break;
		case 3:
			std::cout << "It's a tie!\n";
			break;
		case -1:
			std::cout << "Game error.\n";
			break;
		case 0:
			std::cout << "Game ended.\n";
			break;
		default:
			std::cout << "Game ended, winner was case '" << winner << "'.\n";
			break;
		}
	}
}

int main()
{
	std::cout << "This, ia a very WIP Tic-Tac-Toe project by lucasstarsz.\nGitHub: https://github.com/lucasstarsz/Cpp-TicTacToe\n\n";

	std::string player1Name;
	std::string player2Name;

	TicTacToe::Utilities::getInput("Player 1's name: ", &player1Name);
	TicTacToe::Utilities::getInput("Player 2's name: ", &player2Name);

	TicTacToe::Player player1 = { 1, &player1Name };
	TicTacToe::Player player2 = { 2, &player2Name };

	TicTacToe::runGame(&player1, &player2);
}
