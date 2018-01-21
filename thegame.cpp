#include <iostream>
#include <cstdlib>
using namespace std;

char insults[3][256] = {
	"First step beating you. Next step World Domination.",
	"Rematch? It won't change the outcome.",
	"What came first the chicken or the egg? Never mind, it's me.",
};


/* The tic-tac-toe board. */
class Board {
	public:
		Board();
		bool complete();
		int winner();
		bool add(unsigned int, unsigned int, char);
		void clear(unsigned int, unsigned int);
		void draw();
	private:
		char board[3][3];
};

Board::Board() {
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j)
			board[i][j] = ' ';
}

/* Check if board is complete. */
int Board::winner() {
	for (int ch=1; ch <= 2; ++ch) {
		char chk = ((ch == 1) ? 'X' : 'O');
		for (int i=0; i<3; ++i) {
			if (board[i][0] == chk && board[i][1] == chk && board[i][2] == chk) return chk;
			if (board[0][i] == chk && board[1][i] == chk && board[2][i] == chk) return chk;
		}
		if (board[0][0] == chk && board[1][1] == chk && board[2][2] == chk) return chk;
		if (board[2][0] == chk && board[1][1] == chk && board[0][2] == chk) return chk;
	}
	return 0;
}

/* Check if board is complete. */
bool Board::complete() {
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j)
			if (board[i][j] == ' ') return false;
	return true;
}

/* Add a character to the board. */
bool Board::add(unsigned int x, unsigned int y, char value) {
	if (complete() || board[x][y] != ' ') return false;
	if (value != 'X' && value != 'O') return false;
	board[x][y] = value;
	return true;
}

/* Clear a character from the board. */
void Board::clear(unsigned int x, unsigned int y) {
	board[x][y] = ' ';
}

/* Draw the board on screen. */
void Board::draw() {
	cout <<  endl;
	cout << "\t   " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
	cout << "\t  ---|---|---" << endl;
	cout << "\t   " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
	cout << "\t  ---|---|---" << endl;
	cout << "\t   " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
	cout << endl << endl;
}


/* Construct tree. */
int construct_game_tree(Board const& root, char val, bool min_) {
	int minimax = (min_) ? 1 : -1;
	int current;
	char winner;
	Board board = root;
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j) {
			bool status;
			if (!min_) status = board.add(i, j, val);						// If minimizing, add val.
			else status = board.add(i, j, (val == 'X') ? 'O' : 'X');		// If maximizing, add opposite of val.
			if (!status) continue;											// Check if insertion successful.

			winner = board.winner();
			if (!winner) {
				if (!board.complete()) {
					// Only continue construction if the board is incomplete.
					current = construct_game_tree(board, val, !min_);
				} else current = 0;											// Draw.
			} else {
				if (winner == val) current = 1;								// Computer wins. (yay!)
				else current = -1;											// Player wins. (nay.)
			}

			if (min_ && current < minimax) minimax = current;				// Minimizing stage.
			if (!min_ && current > minimax) minimax = current;				// Maximizing stage.
			board.clear(i, j);												// Clear the current addition.
		}
	return minimax;
}

/* Decide next move based on minimax criterion. */
void decide_next_move(Board& root, char val) {
	int minimax[9];
	int choices[9], len=0;
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j) {
			if (!root.add(i, j, val)) {
				minimax[i*3 + j] = -1;
				continue;
			}
			minimax[i*3 + j] = construct_game_tree(root, val, true);
			root.clear(i, j);
		}

	int max_ = -1;

	// Find the maximum minimax value and pick your choices.
	for (int i=0; i<9; ++i) if (max_ < minimax[i]) max_ = minimax[i];
	for (int i=0; i<9; ++i) if (max_ == minimax[i]) choices[len++] = i;
	
	// Pick a random element from your choices.
	int pos = choices[rand() % len];
	root.add(pos/3, pos%3, val);
}


/* Main function. */
int main() {
	srand(time(NULL));
	char user, computer;
	do {
		cout << "Pick X or O: ";
		cin >> user;
	} while (user != 'X' && user != 'O');
	computer = (user == 'X') ? 'O' : 'X'; 

	bool user_turn = (user == 'X') ? true : false;
	Board board;

	system("clear");
	board.draw();

	while (!board.complete() && !board.winner()) {
		if (user_turn) {
			unsigned int pos;
			do {
				cout << "\tEnter position: ";
				cin >> pos;
				if (pos < 1 || pos > 9) continue; 
			} while (!board.add((pos-1)/3, (pos-1)%3, user));
		} else {
			decide_next_move(board, computer);
		}
		system("clear");
		board.draw();
		user_turn = !user_turn;
	}

	char winner = board.winner();
	if (winner == 0) cout << "\tThat's the best you can do. It's a draw!" << endl;
	else if (winner == user) cout << "\tYou got lucky!" << endl;
	else cout << "\t" << insults[rand() % 3] << "\n\n\n\n\n";

	return 0;
}

// vim:ts=4:sw=4:foldlevel=999
