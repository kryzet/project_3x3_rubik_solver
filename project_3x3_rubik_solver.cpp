﻿// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

using namespace std;

/* Declare the face arrays with a compile-time constant for the
number of pieces per face. */
constexpr size_t N_FACES = 6, N_ROWS = 3, N_COLS = 3;
char cube[N_FACES][N_ROWS][N_COLS] = {
	{{'W', 'W', 'W'}, {'W', 'W', 'W'}, {'W', 'W', 'W'}},  // Top
	{{'O', 'O', 'O'}, {'O', 'O', 'O'}, {'O', 'O', 'O'}},  // Left
	{{'G', 'G', 'G'}, {'G', 'G', 'G'}, {'G', 'G', 'G'}},  // Front
	{{'R', 'R', 'R'}, {'R', 'R', 'R'}, {'R', 'R', 'R'}},  // Right
	{{'B', 'B', 'B'}, {'B', 'B', 'B'}, {'B', 'B', 'B'}},  // Back
	{{'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}}   // Bottom
};

// functions
void printFace(char face[N_ROWS][N_COLS]);
void rotateTopFaceEdges();

// TODO: Write the functions for rotating the cube's faces
void rotateFaceClockwise(char face[N_ROWS][N_COLS]);
void rotateFaceCounterClockwise(char face[N_ROWS][N_COLS]);
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


// Regular moves
void moveU();
void moveD();
void moveF();
void moveB();
void moveR();
void moveL();

// Prime moves
void moveUPrime();
void moveDPrime();
void moveFPrime();
void moveBPrime();
void moveRPrime();
void moveLPrime();

// Double moves
void moveU2();
void moveD2();
void moveF2();
void moveB2();
void moveL2();
void moveR2();

// Algorithms
void sexyMove();
void ReverseSexyMove();
void sledgehammerMove();


void scramble();
void exterior_face(bool top_face);

int main()
{
	/* Ask for the scramble and use it to simulate a scramble of the virtual
	Rubik's cube */
	cout << "Welcome to Rubik Solver!" << endl;
	displayCube();

	cout << "This program solves a Rubik's cube given a scramble of moves and "
		<< "provides the" << endl << "steps to solve the cube. Please enter "
		<< "the scramble below." << endl;

	scramble();
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

void scramble() {
	// Get the scramble moves from the user
	cout << "Enter the scramble: ";
	string moves; getline(cin, moves);

	// Parse and apply the moves
	string move = "";
	for (char c : moves) {
		if (c == ' ') {
			if (!move.empty() && isValidMove(move)) {
				applyMove(move);
			}
			move = ""; // Reset the move
		}
		else {
			move += c; // Build the move character by character
		}
	}

	// Apply the last move if valid
	if (!move.empty() && isValidMove(move)) {
		applyMove(move);
	}
}

void parseAndApplyMoves(const string& moves) {
	string move = "";
	for (char c : moves) {
		if (c == ' ') {
			if (!move.empty() && isValidMove(move)) {
				applyMove(move);
			}
			move = ""; // Reset the move
		}
		else {
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

	char temp[N_ROWS];

	// TODO (22003): Account for F2' and similar
	/* TODO (22003): Try to find patterns and modularize the code to reduce
	copy-pasting and slight modifications */
	/* TODO (22003): Refactor the code to work with one 3D array instead of six
	2D arrays */
	char face = move[0];
	const bool IS_PRIME = (move.length() == 2 && move[1] == '\''),
		IS_DOUBLE = (move.length() == 2 && move[1] == '2');

	const size_t TIMES = [&IS_DOUBLE, &IS_PRIME]() {
		size_t times = 1;
		if (IS_DOUBLE) times *= 2;
		if (IS_PRIME) times *= 3;
		while (times >= 4) times -= 4;
		return times;
		}();
	switch (face) {
	case 'U':
		if (IS_PRIME) moveUPrime();
		else if (IS_DOUBLE) moveU2();
		else moveU();
		break;
	case 'D':
		if (IS_PRIME) moveDPrime();
		else if (IS_DOUBLE) moveD2();
		else moveD();
		break;
	case 'F':
		if (IS_PRIME) moveFPrime();
		else if (IS_DOUBLE) moveF2();
		else moveF();
		break;
	case 'B':
		if (IS_PRIME) moveBPrime();
		else if (IS_DOUBLE) moveB2();
		else moveB();
		break;
	case 'L':
		if (IS_PRIME) moveLPrime();
		else if (IS_DOUBLE) moveL2();
		else moveL();
		break;
	case 'R':
		if (IS_PRIME) moveRPrime();
		else if (IS_DOUBLE) moveR2();
		else moveR();
		break;
	}
}

bool isValidMove(const string& MOVE) {
	// Basic moves: F, B, R, L, U, D
	// Prime moves: F', B', R', L', U', D'
	// Double moves: F2, B2, R2, L2, U2, D2

	if (MOVE.length() > 2) return false;

	const char FACE = MOVE[0];
	constexpr char VALID_FACES[] = { 'U', 'L', 'F', 'R', 'B', 'D' };
	const char* FACES_BEGINNING = begin(VALID_FACES),
		* FACES_END = end(VALID_FACES);
	if (find(FACES_BEGINNING, FACES_END, FACE) == FACES_END)
		return false;

	if (MOVE.length() == 2) {
		const char DIRECTIVE = MOVE[1];
		constexpr char VALID_DIRECTIVES[] = { '\'', '2' };
		const char* DIRECTIVES_BEGINNING = begin(VALID_DIRECTIVES),
			* DIRECTIVES_END = end(VALID_DIRECTIVES);
		if (find(DIRECTIVES_BEGINNING, DIRECTIVES_END, DIRECTIVE) == DIRECTIVES_END) {
			return false;
		}
	}

	return true;
}


void rotateFaceClockwise(char face[N_ROWS][N_COLS]) {
	char temp[N_ROWS][N_COLS];

	// Copy the original face
	for (int i = 0; i < N_ROWS; i++) {
		for (int j = 0; j < N_COLS; j++) {
			temp[i][j] = face[i][j];
		}
	}

	// Rotate 90 degrees clockwise
	for (int i = 0; i < N_ROWS; i++) {
		for (int j = 0; j < N_COLS; j++) {
			face[j][N_COLS - 1 - i] = temp[i][j];
		}
	}
}


void rotateFaceCounterClockwise(char face[N_ROWS][N_COLS]) {
	char temp[N_ROWS][N_COLS];

	// Copy the original face
	for (int i = 0; i < N_ROWS; i++) {
		for (int j = 0; j < N_COLS; j++) {
			temp[i][j] = face[i][j];
		}
	}

	// Rotate 90 degrees counter-clockwise
	for (int i = 0; i < N_ROWS; i++) {
		for (int j = 0; j < N_COLS; j++) {
			face[N_ROWS - 1 - j][i] = temp[i][j];
		}
	}
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
	// Print the top face
	exterior_face(true);

	// Print the middle faces
	constexpr size_t N_MIDDLE_FACES = N_FACES - 1;
	for (int row = 0; row < N_ROWS; row++) {
		for (size_t face = 1; face < N_MIDDLE_FACES; ++face)
			for (int col = 0; col < N_COLS; ++col)
				cout << cube[face][row][col] << ' ';
		cout << endl;
	}

	// Print the bottom face
	exterior_face(false);
}

void exterior_face(bool top_face) {
	size_t face = 0;
	if (!top_face) face = 5;
	for (size_t row = 0; row < N_ROWS; ++row) {
		cout << "      ";
		for (size_t col = 0; col < N_COLS; ++col) {
			cout << cube[face][row][col] << ' ';
		}
		cout << endl;
	}
}

//moves
void moveU() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[2][0][i];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[2][0][i] = cube[3][0][i];
		cube[3][0][i] = cube[4][0][i];
		cube[4][0][i] = cube[1][0][i];
		cube[1][0][i] = temp[i];
	}
	rotateFaceClockwise(cube[0]);
}

void moveD() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[2][2][i];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[2][2][i] = cube[1][2][i];
		cube[1][2][i] = cube[4][2][i];
		cube[4][2][i] = cube[3][2][i];
		cube[3][2][i] = temp[i];
	}
	rotateFaceClockwise(cube[5]);
}

void moveF() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[0][2][i];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[0][2][i] = cube[1][2 - i][2];
		cube[1][2 - i][2] = cube[5][0][2 - i];
		cube[5][0][2 - i] = cube[3][i][0];
		cube[3][i][0] = temp[i];
	}
	rotateFaceClockwise(cube[2]);
}

void moveB() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[0][0][i];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[0][0][i] = cube[3][i][2];
		cube[3][i][2] = cube[5][2][2 - i];
		cube[5][2][2 - i] = cube[1][2 - i][0];
		cube[1][2 - i][0] = temp[i];
	}
	rotateFaceClockwise(cube[4]);
}

void moveL() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[0][i][0];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[0][i][0] = cube[4][2 - i][2];
		cube[4][2 - i][2] = cube[5][i][0];
		cube[5][i][0] = cube[2][i][0];
		cube[2][i][0] = temp[i];
	}
	rotateFaceClockwise(cube[1]);
}

void moveR() {
	char temp[N_COLS];
	for (int i = 0; i < N_COLS; i++) {
		temp[i] = cube[0][i][2];
	}
	for (int i = 0; i < N_COLS; i++) {
		cube[0][i][2] = cube[2][i][2];
		cube[2][i][2] = cube[5][i][2];
		cube[5][i][2] = cube[4][2 - i][0];
		cube[4][2 - i][0] = temp[i];
	}
	rotateFaceClockwise(cube[3]);
}

//Prime moves
void moveUPrime() { for (int i = 0; i < 3; i++) moveU(); } // instead of going back once it just rotates 3 times clockwise
void moveDPrime() { for (int i = 0; i < 3; i++) moveD(); }
void moveFPrime() { for (int i = 0; i < 3; i++) moveF(); }
void moveBPrime() { for (int i = 0; i < 3; i++) moveB(); }
void moveRPrime() { for (int i = 0; i < 3; i++) moveR(); }
void moveLPrime() { for (int i = 0; i < 3; i++) moveL(); }

//Double moves
void moveU2() { moveU(); moveU(); } //repeats move twice
void moveD2() { moveD(); moveD(); }
void moveF2() { moveF(); moveF(); }
void moveB2() { moveB(); moveB(); }
void moveL2() { moveL(); moveL(); }
void moveR2() { moveR(); moveR(); }

//Algorithms
// Algorithm sequences
void sexyMove() {
	moveR(); moveU(); moveRPrime(); moveUPrime();
}

void reverseSexyMove() {
	moveUPrime(); moveRPrime(); moveU(); moveR();
}

void sledgehammerMove() {
	moveRPrime(); moveFPrime(); moveR(); moveF();
}