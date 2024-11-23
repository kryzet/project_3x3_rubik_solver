// project_3x3_rubik_solver1.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

using namespace std;

constexpr size_t ROW = 3, COLUMN = 3;

char front[ROW][COLUMN],
back[ROW][COLUMN],
top[ROW][COLUMN],
bottom[ROW][COLUMN],
leftSide[ROW][COLUMN],
rightSide[ROW][COLUMN];

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

void scramble(char front[ROW][COLUMN],
	char back[ROW][COLUMN],
	char top[ROW][COLUMN],
	char bottom[ROW][COLUMN],
	char leftSide[ROW][COLUMN],
	char rightSide[ROW][COLUMN]);


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
			front[i][j] = 'G';
			back[i][j] = 'W';
			top[i][j] = 'Y';
			bottom[i][j] = 'R';
			leftSide[i][j] = 'O';
			rightSide[i][j] = 'B';
		}
	}

	/* Ask for the scramble and use it to simulate a scramble of the virtual
	Rubik's cube */

	cout << "This program solves a Rubik's cube given a scramble of moves and "
		<< "provides the" << endl << "steps to solve the cube. Please enter "
		<< "the scramble below." << endl;

	scramble(front, back, top, bottom, leftSide, rightSide);
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

void scramble(char front[ROW][COLUMN],
			  char back[ROW][COLUMN],
			  char top[ROW][COLUMN],
			  char bottom[ROW][COLUMN],
			  char leftSide[ROW][COLUMN],
			  char rightSide[ROW][COLUMN]) {
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
		rotateFaceClockwise(front);
		for (int i = 0; i < ROW; ++i) temp[i] = top[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) top[ROW - 1][i] = leftSide[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) leftSide[i][COLUMN - 1] = bottom[0][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) bottom[0][i] = rightSide[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) rightSide[i][0] = temp[i];
	} else if (move == "F'") {
		rotateFaceCounterClockwise(front);
		for (int i = 0; i < ROW; ++i) temp[i] = top[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) top[ROW - 1][i] = rightSide[i][0];
		for (int i = 0; i < ROW; ++i) rightSide[i][0] = bottom[0][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) bottom[0][i] = leftSide[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) leftSide[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "F2") {
		rotateFaceClockwise(front);
		rotateFaceClockwise(front);
	} else if (move == "B") {
		rotateFaceClockwise(back);
		for (int i = 0; i < ROW; ++i) temp[i] = top[0][i];
		for (int i = 0; i < ROW; ++i) top[0][i] = rightSide[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) rightSide[i][COLUMN - 1] = bottom[ROW - 1][ROW - 1 - i];
		for (int i = 0; i < ROW; ++i) bottom[ROW - 1][i] = leftSide[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) leftSide[i][0] = temp[ROW - 1 - i];
	} else if (move == "B'") {
		rotateFaceCounterClockwise(back);
		for (int i = 0; i < ROW; ++i) temp[i] = top[0][i];
		for (int i = 0; i < ROW; ++i) top[0][i] = leftSide[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) leftSide[i][0] = bottom[ROW - 1][i];
		for (int i = 0; i < ROW; ++i) bottom[ROW - 1][i] = rightSide[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) rightSide[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "B2") {
		rotateFaceClockwise(back);
		rotateFaceClockwise(back);
	} else if (move == "R") {
		rotateFaceClockwise(rightSide);
		for (int i = 0; i < ROW; ++i) temp[i] = top[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) top[i][COLUMN - 1] = front[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) front[i][COLUMN - 1] = bottom[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) bottom[i][COLUMN - 1] = back[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) back[i][0] = temp[ROW - 1 - i];
	} else if (move == "R'") {
		rotateFaceCounterClockwise(rightSide);
		for (int i = 0; i < ROW; ++i) temp[i] = top[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) top[i][COLUMN - 1] = back[ROW - 1 - i][0];
		for (int i = 0; i < ROW; ++i) back[i][0] = bottom[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) bottom[i][COLUMN - 1] = front[i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) front[i][COLUMN - 1] = temp[i];
	} else if (move == "R2") {
		rotateFaceClockwise(rightSide);
		rotateFaceClockwise(rightSide);
	} else if (move == "L") {
		rotateFaceClockwise(leftSide);
		for (int i = 0; i < ROW; ++i) temp[i] = top[i][0];
		for (int i = 0; i < ROW; ++i) top[i][0] = back[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) back[i][COLUMN - 1] = bottom[i][0];
		for (int i = 0; i < ROW; ++i) bottom[i][0] = front[i][0];
		for (int i = 0; i < ROW; ++i) front[i][0] = temp[i];
	} else if (move == "L'") {
		rotateFaceCounterClockwise(leftSide);
		for (int i = 0; i < ROW; ++i) temp[i] = top[i][0];
		for (int i = 0; i < ROW; ++i) top[i][0] = front[i][0];
		for (int i = 0; i < ROW; ++i) front[i][0] = bottom[i][0];
		for (int i = 0; i < ROW; ++i) bottom[i][0] = back[ROW - 1 - i][COLUMN - 1];
		for (int i = 0; i < ROW; ++i) back[i][COLUMN - 1] = temp[ROW - 1 - i];
	} else if (move == "L2") {
		rotateFaceClockwise(leftSide);
		rotateFaceClockwise(leftSide);
	} else if (move == "U") {
		rotateFaceClockwise(top);
		for (int i = 0; i < COLUMN; ++i) temp[i] = front[0][i];
		for (int i = 0; i < COLUMN; ++i) front[0][i] = rightSide[0][i];
		for (int i = 0; i < COLUMN; ++i) rightSide[0][i] = back[0][i];
		for (int i = 0; i < COLUMN; ++i) back[0][i] = leftSide[0][i];
		for (int i = 0; i < COLUMN; ++i) leftSide[0][i] = temp[i];
	} else if (move == "U'") {
		rotateFaceCounterClockwise(top);
		for (int i = 0; i < COLUMN; ++i) temp[i] = front[0][i];
		for (int i = 0; i < COLUMN; ++i) front[0][i] = leftSide[0][i];
		for (int i = 0; i < COLUMN; ++i) leftSide[0][i] = back[0][i];
		for (int i = 0; i < COLUMN; ++i) back[0][i] = rightSide[0][i];
		for (int i = 0; i < COLUMN; ++i) rightSide[0][i] = temp[i];
	} else if (move == "U2") {
		rotateFaceClockwise(top);
		rotateFaceClockwise(top);
	} else if (move == "D") {
		rotateFaceClockwise(bottom);
		for (int i = 0; i < COLUMN; ++i) temp[i] = front[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) front[ROW - 1][i] = leftSide[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) leftSide[ROW - 1][i] = back[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) back[ROW - 1][i] = rightSide[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) rightSide[ROW - 1][i] = temp[i];
	} else if (move == "D'") {
		rotateFaceCounterClockwise(bottom);
		for (int i = 0; i < COLUMN; ++i) temp[i] = front[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) front[ROW - 1][i] = rightSide[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) rightSide[ROW - 1][i] = back[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) back[ROW - 1][i] = leftSide[ROW - 1][i];
		for (int i = 0; i < COLUMN; ++i) leftSide[ROW - 1][i] = temp[i];
	} else if (move == "D2") {
		rotateFaceClockwise(bottom);
		rotateFaceClockwise(bottom);
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
