#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

const int SIZE = 8;

class Piece {
private:
	char symbol; // 'r', 'b', 'R', 'B', or '.'
public:
	Piece(char s = '.') : symbol(s) {}
	char getSymbol() const { return symbol; }
	void setSymbol(char s) { symbol = s; }
	bool isKing() const { return (symbol == 'R' || symbol == 'B'); }
	bool isEmpty() const { return symbol == '.'; }
};

class Board {
private:
	vector<vector<Piece>> grid;
public:
	Board() {
		grid.resize(SIZE, vector<Piece>(SIZE));
		initialize();
	}

	void initialize() {
		// Place pieces on dark squares
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				if ((row + col) % 2 != 0) {
					if (row < 3) grid[row][col].setSymbol('b'); // Black pieces
					else if (row > 4) grid[row][col].setSymbol('r'); // Red pieces
				}
			}
		}
	}

	void display() const {
		cout << "   a b c d e f g h\n";
		for (int row = 0; row < SIZE; row++) {
			cout << row + 1 << "  ";
			for (int col = 0; col < SIZE; col++) {
				cout << grid[row][col].getSymbol() << " ";
			}
			cout << "\n";
		}
	}

	bool movePiece(int fromRow, int fromCol, int toRow, int toCol) {
		// Basic move logic (no capture yet)
		if (!isValidIndex(fromRow, fromCol) || !isValidIndex(toRow, toCol)) return false;
		if (!grid[toRow][toCol].isEmpty()) return false;

		grid[toRow][toCol].setSymbol(grid[fromRow][fromCol].getSymbol());
		grid[fromRow][fromCol].setSymbol('.');
		return true;
	}

	bool isValidIndex(int row, int col) const {
		return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
	}

	void saveToFile(const string& filename) const {
		ofstream out(filename);
		if (!out) {
			cerr << "Error saving file.\n";
			return;
		}
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				out << grid[row][col].getSymbol();
			}
			out << "\n";
		}
		out.close();
		cout << "Game saved to " << filename << "\n";
	}

	void loadFromFile(const string& filename) {
		ifstream in(filename);
		if (!in) {
			cerr << "Error loading file.\n";
			return;
		}
		string line;
		for (int row = 0; row < SIZE; row++) {
			getline(in, line);
			for (int col = 0; col < SIZE && col < (int)line.size(); col++) {
				grid[row][col].setSymbol(line[col]);
			}
		}
		in.close();
		cout << "Game loaded from " << filename << "\n";
	}
};

class Game {
private:
	Board board;
	bool isRedTurn;
public:
	Game() : isRedTurn(true) {}

	void play() {
		string command;
		while (true) {
			board.display();
			cout << (isRedTurn ? "Red" : "Black") << "'s turn.\n";
			cout << "Enter move (e.g., d3 e4), or 'save', 'load', 'quit': ";
			cin >> command;

			if (command == "quit") break;
			else if (command == "save") {
				board.saveToFile("checkers_save.txt");
			}
			else if (command == "load") {
				board.loadFromFile("checkers_save.txt");
			}
			else {
				string to;
				cin >> to;
				int fromRow = command[1] - '1';
				int fromCol = command[0] - 'a';
				int toRow = to[1] - '1';
				int toCol = to[0] - 'a';

				if (board.movePiece(fromRow, fromCol, toRow, toCol)) {
					isRedTurn = !isRedTurn;
				}
				else {
					cout << "Invalid move!\n";
				}
			}
		}
	}
};

int main() {
	Game game;
	game.play();
	return 0;
}