// TicTacToe.cpp : This file contains the 'main' function for CLI Tic-Tac-Toe.
// 
// Author: Andrew Dey

#include <string>
#include <iostream>

namespace TicTacToe
{
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

		Board(std::string *name) : board(), boardName(*name)
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

	void runGame(Player* player1, Player* player2)
	{
		std::string boardName = (*player1->name + " vs " + *player2->name);
		Board board = (&boardName);

		board.print();

		while (!board.hasWinner())
		{
			// TODO: notify player turn
			// TODO: wait for command
			// TODO: process command
			// TODO: show board
		}
	}
}

std::string getInput(std::string prompt)
{
	std::string result;

	do
	{
		std::cout << prompt;
		std::getline(std::cin, result);

		if (result.empty())
		{
			continue;
		}

		bool isValid = false;

		for (int i = 0; i < result.length(); i++)
		{
			if (result[i] != ' ')
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

	return result;
}

int main()
{
	std::cout << "Hello World!\n";

	std::string player1Name;
	std::string player2Name;

	player1Name = getInput("Player 1's name: ");
	player2Name = getInput("Player 2's name: ");

	TicTacToe::Player player1 = { 1, &player1Name };
	TicTacToe::Player player2 = { 2, &player2Name };

	TicTacToe::runGame(&player1, &player2);
}
