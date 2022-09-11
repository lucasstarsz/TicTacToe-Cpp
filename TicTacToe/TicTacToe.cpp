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
#include <iterator>

namespace TicTacToe
{

	namespace Utilities
	{
		/// Gets input from std::cin, with a given prompt.
		/// If input is empty, or blank (all US-ASCII spaces), then it will be considered invalid.
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

		std::string* join(std::vector<std::string>* strs, const std::string* delimiter, std::string* dest)
		{
			for (size_t i = 0; i < strs->size(); i++)
			{
				dest->append(strs->at(i));

				if (i + 1 < strs->size())
				{
					dest->append(*delimiter);
				}
			}

			return dest;
		}

		std::string* join(std::vector<std::string>* strs, std::string* dest)
		{
			std::string delimiter = " ";
			return join(strs, &delimiter, dest);
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

	enum GameState
	{
		Playing,
		Player1Wins,
		Player2Wins,
		Tie,
		Ended,
		Error = -1
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

		const char PLAYER_1 = 'X';
		const char PLAYER_2 = 'O';
		const char EMPTY = ' ';

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
			return board[(3 * row) + col] == EMPTY;
		}

		void play(int row, int col, int playerNumber)
		{
			board[(3 * row) + col] = playerNumber == 1 ? PLAYER_1 : PLAYER_2;
		}

		bool boardIsFilled()
		{
			for (int i = 0; i < 9; i++)
			{
				if (board[i] == EMPTY)
				{
					return false;
				}
			}

			return true;
		}

		GameState getGameState()
		{
			for (const int* winCondition : WIN_CONDITIONS)
			{
				if (board[winCondition[0]] == EMPTY || board[winCondition[1]] == EMPTY || board[winCondition[2]] == EMPTY)
				{
					continue;
				}

				if (board[winCondition[0]] == board[winCondition[1]] && board[winCondition[0]] == board[winCondition[2]])
				{
					return PLAYER_1 == board[winCondition[0]] ? Player1Wins : Player2Wins;
				}
			}

			if (boardIsFilled())
			{
				return Tie;
			}

			return Playing;
		}

	private:
		void initializeBoard()
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					board[(3 * i) + j] = EMPTY;
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
			if (command->size() < 2)
			{
				std::cout << "Press the enter key to exit.";
				std::cin.get();
			}
			else
			{
				Utilities::stringToLower(&command->at(1));

				if (command->at(1) != "quiet" && (command->at(1).length() != 1 && command->at(1)[0] != 'q'))
				{
					std::cout << "Press the enter key to exit.";
					std::cin.get();
				}
			}

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
				std::cout << "Nothing to show for '" << command->at(1) << "'.\n";
				std::cout << "Usage: show [name/boardname, board/contents, all]\n";

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
				std::cout << "Usage: play <row 1, 2, or 3> <column 1, 2, or 3>\n";
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
				std::cout << "Usage: play <row 1, 2, or 3> <column 1, 2, or 3>\n";

				return false;
			}

			if (!Utilities::withinRange(1, 3, column, row))
			{
				std::cout << "Invalid row/column.\n";
				std::cout << "Usage: play <row 1, 2, or 3> <column 1, 2, or 3>\n";

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

		bool renamePlayerCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			if (command->size() < 2)
			{
				std::cout << "No target specified.\n";
				std::cout << "Usage: rename <board, or self> <new name>\n";

				return false;
			}

			if (command->size() < 3)
			{
				std::cout << "No new name specified.\n";
				std::cout << "Usage: rename <board, or self> <new name>\n";

				return false;
			}

			Utilities::stringToLower(&command->at(1));

			std::string* nameToSet;

			if (command->at(1) == "board")
			{
				nameToSet = &board->boardName;
			}
			else if (command->at(1) == "self")
			{
				nameToSet = player->name;
			}
			else
			{
				std::cout << "Couldn't find '" << command->at(1) << "' to rename.\n";
				std::cout << "Usage: rename <board, or self> <new name>\n";

				return false;
			}

			std::vector<std::string> unjoinedName(command->begin() + 2, command->end());
			std::string newName;
			Utilities::join(&unjoinedName, &newName);

			*nameToSet = newName;

			return false;
		}

		bool helpCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			std::cout << "This is Tic-Tac-Toe! Your objective is to get 3 in a row, column, or diagonally, in order to win!\n";
			std::cout << "You, " << *player->name << ", and your opponent, compete to win this turn-based game.\n\n";
			std::cout << "Game Commands:\n";
			std::cout << "exit -- exits the game immediately.\n";
			std::cout << "show [name/boardname, board/contents, all] -- shows information about the board.\n";
			std::cout << "rename <board, or self> <new name> -- renames the given target with a new name.\n";
			std::cout << "play <row> <column> -- shows information about the board.\n";
			std::cout << "help -- brings up this message.\n\n";

			return false;
		}

		const std::unordered_map<std::string, bool (*)(std::vector<std::string>*, Board*, Player*)> commands = {
			{"exit", &exitCommand},
			{"show", &showCommand},
			{"play", &playCommand},
			{"p", &playCommand},
			{"help", &helpCommand},
			{"rename", &renamePlayerCommand}
		};

		/// Returns whether a command was found, or completed an action warranting a player swap (like playing a move, or skipping).
		/// Note: does not support anything other than US-ASCII.
		bool processCommand(std::vector<std::string>* command, Board* board, Player* player)
		{
			Utilities::stringToLower(&command->at(0));

			if (commands.count(command->at(0)) < 1)
			{
				std::cout << "No command '" << command->at(0) << "' was found. ('help' lists all the commands)\n";
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
				winner = board.getGameState();
			}
		}
		catch (std::exception& exception)
		{
			winner = -1;
			std::cerr << "Exception: " << exception.what() << '\n';
		}

		switch (winner)
		{
		case Player1Wins:
			std::cout << *player1->name << " wins!\n";
			break;
		case Player2Wins:
			std::cout << *player2->name << " wins!\n";
			break;
		case Tie:
			std::cout << "It's a tie!\n";
			break;
		case Error:
			std::cout << "Game had an error while running, and had to cancel the current board.\n";
			break;
		case Ended:
			std::cout << "Game ended.\n";
			break;
		default:
			std::cout << "Game ended, case '" << winner << "'.\n";
			break;
		}
	}
}

bool shouldPlayAgain()
{
	std::string response;

	TicTacToe::Utilities::getInput("Play another match? (Y)es to play, (N)o or (C)ancel to exit. ", &response);
	TicTacToe::Utilities::stringToLower(&response);

	return response == "yes" || (response.length() == 1 && response[0] == 'y');
}

int main()
{
	std::cout << "This, is a Tic-Tac-Toe project by lucasstarsz.\nGitHub: https://github.com/lucasstarsz/Cpp-TicTacToe\n\n";

	std::string player1Name;
	std::string player2Name;

	TicTacToe::Utilities::getInput("Player 1's name: ", &player1Name);
	TicTacToe::Utilities::getInput("Player 2's name: ", &player2Name);

	TicTacToe::Player player1 = { 1, &player1Name };
	TicTacToe::Player player2 = { 2, &player2Name };

	do
	{
		TicTacToe::runGame(&player1, &player2);
	} while (shouldPlayAgain());

	std::cout << "Press the enter key to exit.";
	std::cin.get();
}
