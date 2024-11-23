// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

using namespace std;

constexpr size_t ROW = 3, COLUMN = 3;

char f_front[ROW][COLUMN],
f_back[ROW][COLUMN],
f_top[ROW][COLUMN],
f_bottom[ROW][COLUMN],
f_left[ROW][COLUMN],
f_right[ROW][COLUMN];

//functions
void printFace(char face[ROW][COLUMN]);
void rotateTopFaceEdges();
void rotateFaceClockwise(char face[ROW][COLUMN]);
void rotateFaceCounterClockwise(char face[ROW][COLUMN]);
bool isValidMove(const string& move);
void applyMove(const string& move);
void parseAndApplyMoves(const string& moves);
void displayCube();
void solveWhiteCross();
void solveF2l();
void solveOll();
void solvePll();
void resetCube();
bool isSolved();

void scramble(char f_front[ROW][COLUMN],
	char f_back[ROW][COLUMN],
	char f_top[ROW][COLUMN],
	char f_bottom[ROW][COLUMN],
	char f_left[ROW][COLUMN],
	char f_right[ROW][COLUMN]);


// TODO: Write the functions for rotating the cube's faces (gotta decide
// whether we will keep using six separate arrays :P)

int main()
{
	/* TODO (Hassan--still haven't decided on a username after quite a while on
	GitHub :D): Declare the face arrays with a compile-time constant for the
	number of pieces per face.
	*/


	// TODO (22003): Fill the faces with colors

	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COLUMN; ++j) {
			f_front[i][j] = 'G';
			f_back[i][j] = 'W';
			f_top[i][j] = 'Y';
			f_bottom[i][j] = 'R';
			f_left[i][j] = 'O';
			f_right[i][j] = 'B';
		}
	}

	/* Ask for the scramble and use it to simulate a scramble of the virtual
	Rubik's cube */
	displayCube();


	cout << "This program solves a Rubik's cube given a scramble of moves and "
		<< "provides the" << endl << "steps to solve the cube. Please enter "
		<< "the scramble below." << endl;

	scramble(f_front, f_back, f_top, f_bottom, f_left, f_right);
	displayCube();

	solveF2l();
	displayCube();

	solveOll();
	displayCube();

	solvePll();
	displayCube();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

void scramble(char f_front[ROW][COLUMN],
			  char f_back[ROW][COLUMN],
			  char f_top[ROW][COLUMN],
			  char f_bottom[ROW][COLUMN],
			  char f_left[ROW][COLUMN],
			  char f_right[ROW][COLUMN]) {
	string moves;

	// Get the scramble moves from the user
	cout << "Enter the scramble: ";
	getline(cin, moves);
	cout << "Scramble moves: " << moves << endl;

	// Parse and apply the moves
	parseAndApplyMoves(moves);
}

void parseAndApplyMoves(const string& moves) {
	string move = "";
	for (char c : moves) {
		if (c == ' ') {
			if (!move.empty() && isValidMove(move)) {
				applyMove(move);
			}
			move = ""; // Reset the move
		} else {
			move += c; // Build the move character by character
		}
	}

	// Apply the last move if valid
	if (!move.empty() && isValidMove(move)) {
		applyMove(move);
	}
}

void applyMove(const string& move) {
	if (!isValidMove(move)) {
		cout << "Invalid move: " << move << endl;
		return;
	}

	char temp[ROW];

	if (move == "F") {
		rotateFaceClockwise(f_front);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) f_top[ROW - 1][i] = f_left[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_left[i][COLUMN - 1] = f_bottom[0][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) f_bottom[0][i] = f_right[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) f_right[i][0] = temp[i];
	} else if (move == "F'") {
		rotateFaceCounterClockwise(f_front);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) f_top[ROW - 1][i] = f_right[i][0];
		for (int i = 0; i < ROW; ++i) f_right[i][0] = f_bottom[0][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) f_bottom[0][i] = f_left[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_left[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "F2") {
		rotateFaceClockwise(f_front);
		rotateFaceClockwise(f_front);
	} else if (move == "B") {
		rotateFaceClockwise(f_back);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[0][i];
		for (int i = 0; i < ROW; ++i) f_top[0][i] = f_right[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_right[i][COLUMN - 1] = f_bottom[ROW - 1][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) f_bottom[ROW - 1][i] = f_left[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) f_left[i][0] = temp[ROW - 1 - i];
	} else if (move == "B'") {
		rotateFaceCounterClockwise(f_back);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[0][i];
		for (int i = 0; i < ROW; ++i) f_top[0][i] = f_left[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) f_left[i][0] = f_bottom[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) f_bottom[ROW - 1][i] = f_right[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_right[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "B2") {
		rotateFaceClockwise(f_back);
		rotateFaceClockwise(f_back);
	} else if (move == "R") {
		rotateFaceClockwise(f_right);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_top[i][COLUMN - 1] = f_front[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_front[i][COLUMN - 1] = f_bottom[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_bottom[i][COLUMN - 1] = f_back[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) f_back[i][0] = temp[ROW - 1 - i];
	} else if (move == "R'") {
		rotateFaceCounterClockwise(f_right);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_top[i][COLUMN - 1] = f_back[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) f_back[i][0] = f_bottom[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_bottom[i][COLUMN - 1] = f_front[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_front[i][COLUMN - 1] = temp[i];
	} else if (move == "R2") {
		rotateFaceClockwise(f_right);
		rotateFaceClockwise(f_right);
	} else if (move == "L") {
		rotateFaceClockwise(f_left);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[i][0];
		for (int i = 0; i < ROW; ++i) f_top[i][0] = f_back[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_back[i][COLUMN - 1] = f_bottom[i][0];
		for (int i = 0; i < ROW; ++i) f_bottom[i][0] = f_front[i][0];
		for (int i = 0; i < ROW; ++i) f_front[i][0] = temp[i];
	} else if (move == "L'") {
		rotateFaceCounterClockwise(f_left);
		for (int i = 0; i < ROW; ++i) temp[i] = f_top[i][0];
		for (int i = 0; i < ROW; ++i) f_top[i][0] = f_front[i][0];
		for (int i = 0; i < ROW; ++i) f_front[i][0] = f_bottom[i][0];
		for (int i = 0; i < ROW; ++i) f_bottom[i][0] = f_back[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) f_back[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "L2") {
		rotateFaceClockwise(f_left);
		rotateFaceClockwise(f_left);
	} else if (move == "U") {
		rotateFaceClockwise(f_top);
		for (int i = 0; i < COLUMN; ++i) temp[i] = f_front[0][i];
		for (int i = 0; i < COLUMN; ++i) f_front[0][i] = f_right[0][i];
		for (int i = 0; i < COLUMN; ++i) f_right[0][i] = f_back[0][i];
		for (int i = 0; i < COLUMN; ++i) f_back[0][i] = f_left[0][i];
		for (int i = 0; i < COLUMN; ++i) f_left[0][i] = temp[i];
	} else if (move == "U'") {
		rotateFaceCounterClockwise(f_top);
		for (int i = 0; i < COLUMN; ++i) temp[i] = f_front[0][i];
		for (int i = 0; i < COLUMN; ++i) f_front[0][i] = f_left[0][i];
		for (int i = 0; i < COLUMN; ++i) f_left[0][i] = f_back[0][i];
		for (int i = 0; i < COLUMN; ++i) f_back[0][i] = f_right[0][i];
		for (int i = 0; i < COLUMN; ++i) f_right[0][i] = temp[i];
	} else if (move == "U2") {
		rotateFaceClockwise(f_top);
		rotateFaceClockwise(f_top);
	} else if (move == "D") {
		rotateFaceClockwise(f_bottom);
		for (int i = 0; i < COLUMN; ++i) temp[i] = f_front[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_front[ROW - 1][i] = f_left[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_left[ROW - 1][i] = f_back[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_back[ROW - 1][i] = f_right[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_right[ROW - 1][i] = temp[i];
	} else if (move == "D'") {
		rotateFaceCounterClockwise(f_bottom);
		for (int i = 0; i < COLUMN; ++i) temp[i] = f_front[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_front[ROW - 1][i] = f_right[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_right[ROW - 1][i] = f_back[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_back[ROW - 1][i] = f_left[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) f_left[ROW - 1][i] = temp[i];
	} else if (move == "D2") {
		rotateFaceClockwise(f_bottom);
		rotateFaceClockwise(f_bottom);
	}
}




void rotateFaceClockwise(char face[ROW][COLUMN]) {
	char temp[ROW][COLUMN];

	// Copy the original face
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			temp[i][j] = face[i][j];
		}
	}

	// Rotate 90 degrees clockwise
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			face[j][COLUMN - 1 - i] = temp[i][j];
		}
	}
}

void rotateFaceCounterClockwise(char face[ROW][COLUMN]) {
	char temp[ROW][COLUMN];

	// Copy the original face
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			temp[i][j] = face[i][j];
		}
	}

	// Rotate 90 degrees counter-clockwise
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			face[ROW - 1 - j][i] = temp[i][j];
		}
	}
}

bool isValidMove(const string& move) {
	// Basic moves: F, B, R, L, U, D
	// Prime moves: F', B', R', L', U', D'
	// Double moves: F2, B2, R2, L2, U2, D2

	if (move.length() > 2) return false;

	char face = move[0];
	if (face != 'F' && face != 'B' && face != 'R' &&
		face != 'L' && face != 'U' && face != 'D') {
		return false;
	}

	if (move.length() == 2 && move[1] != '\'' && move[1] != '2') {
		return false;
	}

	return true;
}

void solveF2l() {
	// Add logic to solve the F2L step
	cout << "Solving F2L..." << endl;
}

void solveOll() {
	// Add logic to solve the OLL step
	cout << "Solving OLL..." << endl;
}

void solvePll() {
	// Add logic to solve the PLL step
	cout << "Solving PLL..." << endl;
}

void displayCube() {
	//add code to display cube
	cout << "Displaying Cube..." << endl;
}
