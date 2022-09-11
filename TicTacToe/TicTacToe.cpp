// TicTacToe.cpp : This file contains the 'main' function for CLI Tic-Tac-Toe.
// 
// Author: Andrew Dey

#include <string>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <vector>

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

		char board[3][3];
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

		bool hasWinner()
		{
			// TODO: check 3 in a row:
			// TODO: left-to-right
			// TODO: top-to-bottom
			// TODO: diagonally left-to-right
			// TODO: diagonally right-to-left

			return false;
		}

		~Board()
		{
			std::cout << "Board '" << boardName << "' deleted.";
		}

	private:
		void initializeBoard()
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					board[i][j] = ' ';
				}
			}
		}

		void printBoardName()
		{
			std::cout << "Board: " << boardName << ".\n";
		}

		void printBoardContents()
		{
			std::cout << "\n " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " \n";
			std::cout << "-----------\n";
			std::cout << ' ' << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " \n";
			std::cout << "-----------\n";
			std::cout << ' ' << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " \n\n";
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

		const std::unordered_map<std::string, bool (*)(std::vector<std::string>*, Board*, Player*)> commands = {
			{"exit", &exitCommand},
			{"show", &showCommand}
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

		// I mean... It's fine?
		// I should look farther into that C++11 api...
		const std::chrono::nanoseconds timeSeed = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::system_clock::now().time_since_epoch());

		std::srand(timeSeed.count());

		const int firstPlayerTurn = (std::rand() % 2);

		Player currentPlayer = firstPlayerTurn % 2 == 0 ? *player2 : *player1;
		std::string playerCommand;

		std::vector<std::string> parsedPlayerCommand = {};

		while (!board.hasWinner())
		{
			Utilities::getInput(*currentPlayer.name + "'s turn: ", &playerCommand);
			Utilities::split(&playerCommand, &parsedPlayerCommand);

			if (Commands::processCommand(&parsedPlayerCommand, &board, &currentPlayer))
			{
				currentPlayer = currentPlayer.number == 1 ? *player2 : *player1;
			}

			parsedPlayerCommand.clear();
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
