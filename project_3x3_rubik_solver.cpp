// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

using namespace std;

constexpr size_t N_FACE = 6, N_ROW = 3, N_COL = 3;
constexpr size_t N_FACES = 6, N_ROWS = 3, N_COLS = 3;

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

void scramble();
void exterior_face(bool top_face);

int main()
{
	/* Ask for the scramble and use it to simulate a scramble of the virtual
	Rubik's cube */
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

	char temp[N_ROW];

	if (move == "F") {
		rotateFaceClockwise(f_front);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[N_ROW - 1][i];
		for (int i = 0; i < N_ROW; ++i) f_top[N_ROW - 1][i] = f_left[N_ROW - 1 - i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_left[i][N_COL - 1] = f_bottom[0][N_ROW - 1 - i];
		for (int i = 0; i < N_ROW; ++i) f_bottom[0][i] = f_right[N_ROW - 1 - i][0];
		for (int i = 0; i < N_ROW; ++i) f_right[i][0] = temp[i];
	}
	else if (move == "F'") {
		rotateFaceCounterClockwise(f_front);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[N_ROW - 1][i];
		for (int i = 0; i < N_ROW; ++i) f_top[N_ROW - 1][i] = f_right[i][0];
		for (int i = 0; i < N_ROW; ++i) f_right[i][0] = f_bottom[0][N_ROW - 1 - i];
		for (int i = 0; i < N_ROW; ++i) f_bottom[0][i] = f_left[N_ROW - 1 - i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_left[i][N_COL - 1] = temp[N_ROW - 1 - i];
	}
	else if (move == "F2") {
		rotateFaceClockwise(f_front);
		rotateFaceClockwise(f_front);
	}
	else if (move == "B") {
		rotateFaceClockwise(f_back);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[0][i];
		for (int i = 0; i < N_ROW; ++i) f_top[0][i] = f_right[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_right[i][N_COL - 1] = f_bottom[N_ROW - 1][N_ROW - 1 - i];
		for (int i = 0; i < N_ROW; ++i) f_bottom[N_ROW - 1][i] = f_left[N_ROW - 1 - i][0];
		for (int i = 0; i < N_ROW; ++i) f_left[i][0] = temp[N_ROW - 1 - i];
	}
	else if (move == "B'") {
		rotateFaceCounterClockwise(f_back);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[0][i];
		for (int i = 0; i < N_ROW; ++i) f_top[0][i] = f_left[N_ROW - 1 - i][0];
		for (int i = 0; i < N_ROW; ++i) f_left[i][0] = f_bottom[N_ROW - 1][i];
		for (int i = 0; i < N_ROW; ++i) f_bottom[N_ROW - 1][i] = f_right[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_right[i][N_COL - 1] = temp[N_ROW - 1 - i];
	}
	else if (move == "B2") {
		rotateFaceClockwise(f_back);
		rotateFaceClockwise(f_back);
	}
	else if (move == "R") {
		rotateFaceClockwise(f_right);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_top[i][N_COL - 1] = f_front[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_front[i][N_COL - 1] = f_bottom[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_bottom[i][N_COL - 1] = f_back[N_ROW - 1 - i][0];
		for (int i = 0; i < N_ROW; ++i) f_back[i][0] = temp[N_ROW - 1 - i];
	}
	else if (move == "R'") {
		rotateFaceCounterClockwise(f_right);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_top[i][N_COL - 1] = f_back[N_ROW - 1 - i][0];
		for (int i = 0; i < N_ROW; ++i) f_back[i][0] = f_bottom[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_bottom[i][N_COL - 1] = f_front[i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_front[i][N_COL - 1] = temp[i];
	}
	else if (move == "R2") {
		rotateFaceClockwise(f_right);
		rotateFaceClockwise(f_right);
	}
	else if (move == "L") {
		rotateFaceClockwise(f_left);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[i][0];
		for (int i = 0; i < N_ROW; ++i) f_top[i][0] = f_back[N_ROW - 1 - i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_back[i][N_COL - 1] = f_bottom[i][0];
		for (int i = 0; i < N_ROW; ++i) f_bottom[i][0] = f_front[i][0];
		for (int i = 0; i < N_ROW; ++i) f_front[i][0] = temp[i];
	}
	else if (move == "L'") {
		rotateFaceCounterClockwise(f_left);
		for (int i = 0; i < N_ROW; ++i) temp[i] = f_top[i][0];
		for (int i = 0; i < N_ROW; ++i) f_top[i][0] = f_front[i][0];
		for (int i = 0; i < N_ROW; ++i) f_front[i][0] = f_bottom[i][0];
		for (int i = 0; i < N_ROW; ++i) f_bottom[i][0] = f_back[N_ROW - 1 - i][N_COL - 1];
		for (int i = 0; i < N_ROW; ++i) f_back[i][N_COL - 1] = temp[N_ROW - 1 - i];
	}
	else if (move == "L2") {
		rotateFaceClockwise(f_left);
		rotateFaceClockwise(f_left);
	}
	else if (move == "U") {
		rotateFaceClockwise(f_top);
		for (int i = 0; i < N_COL; ++i) temp[i] = f_front[0][i];
		for (int i = 0; i < N_COL; ++i) f_front[0][i] = f_right[0][i];
		for (int i = 0; i < N_COL; ++i) f_right[0][i] = f_back[0][i];
		for (int i = 0; i < N_COL; ++i) f_back[0][i] = f_left[0][i];
		for (int i = 0; i < N_COL; ++i) f_left[0][i] = temp[i];
	}
	else if (move == "U'") {
		rotateFaceCounterClockwise(f_top);
		for (int i = 0; i < N_COL; ++i) temp[i] = f_front[0][i];
		for (int i = 0; i < N_COL; ++i) f_front[0][i] = f_left[0][i];
		for (int i = 0; i < N_COL; ++i) f_left[0][i] = f_back[0][i];
		for (int i = 0; i < N_COL; ++i) f_back[0][i] = f_right[0][i];
		for (int i = 0; i < N_COL; ++i) f_right[0][i] = temp[i];
	}
	else if (move == "U2") {
		rotateFaceClockwise(f_top);
		rotateFaceClockwise(f_top);
	}
	else if (move == "D") {
		rotateFaceClockwise(f_bottom);
		for (int i = 0; i < N_COL; ++i) temp[i] = f_front[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_front[N_ROW - 1][i] = f_left[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_left[N_ROW - 1][i] = f_back[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_back[N_ROW - 1][i] = f_right[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_right[N_ROW - 1][i] = temp[i];
	}
	else if (move == "D'") {
		rotateFaceCounterClockwise(f_bottom);
		for (int i = 0; i < N_COL; ++i) temp[i] = f_front[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_front[N_ROW - 1][i] = f_right[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_right[N_ROW - 1][i] = f_back[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_back[N_ROW - 1][i] = f_left[N_ROW - 1][i];
		for (int i = 0; i < N_COL; ++i) f_left[N_ROW - 1][i] = temp[i];
	}
	else if (move == "D2") {
		rotateFaceClockwise(f_bottom);
		rotateFaceClockwise(f_bottom);
	}
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

bool isValidMove(const string& MOVE) {
	// Basic moves: F, B, R, L, U, D
	// Prime moves: F', B', R', L', U', D'
	// Double moves: F2, B2, R2, L2, U2, D2

	if (MOVE.length() > 2) return false;

	const char FACE = MOVE[0];
	constexpr char VALID_FACES[] = { 'U', 'L', 'F', 'R', 'B', 'D' };
	const char *FACES_BEGINNING = begin(VALID_FACES),
		*FACES_END = end(VALID_FACES);
	if (find(FACES_BEGINNING, FACES_END, FACE) == FACES_END)
		return false;

	if (MOVE.length() == 2) {
		const char DIRECTIVE = MOVE[1];
		constexpr char VALID_DIRECTIVES[] = { '\'', '2' };
		const char* DIRECTIVES_BEGINNING = begin(VALID_DIRECTIVES),
			*DIRECTIVES_END = end(VALID_DIRECTIVES);
		if (find(DIRECTIVES_BEGINNING, DIRECTIVES_END, DIRECTIVE) == DIRECTIVES_END) {
		return false;
	}
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
