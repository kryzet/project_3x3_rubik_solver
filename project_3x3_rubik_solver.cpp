// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"
#include <algorithm>

using namespace std;

/* Declare the face arrays with a compile-time constant for the
number of pieces per face. */
constexpr size_t N_FACES = 6, N_ROWS = 3, N_COLS = 3,
TOP = 0, LEFT = 1, FRONT = 2, RIGHT = 3, BACK = 4, BOTTOM = 5;
char cube[N_FACES][N_ROWS][N_COLS] = {
    {{'W', 'W', 'W'}, {'W', 'W', 'W'}, {'W', 'W', 'W'}},
    {{'O', 'O', 'O'}, {'O', 'O', 'O'}, {'O', 'O', 'O'}},
    {{'G', 'G', 'G'}, {'G', 'G', 'G'}, {'G', 'G', 'G'}},
    {{'R', 'R', 'R'}, {'R', 'R', 'R'}, {'R', 'R', 'R'}},
    {{'B', 'B', 'B'}, {'B', 'B', 'B'}, {'B', 'B', 'B'}},
    {{'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}}
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
// TODO (kryzet): Implement these functions
void solveWhiteCross();
void solveOll();
// TODO (22003): Implement these functions
void solveF2l();
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

// Algorithms
void sexyMove();
void ReverseSexyMove();
void sledgehammerMove();
void cornerPermutation();
void edgePermutation();
void rightCornerAlgorithm();
void leftCornerAlgorithm();
void adjacentEdgesAlgorithm();
void oppositeEdgesAlgorithm();
void solveF2lPair(int pairIndex);
bool isPairInTopLayer(int edgeFace, int cornerFace);
bool isPairAligned(int edgeFace, int cornerFace);
void orientPair(int edgeFace, int cornerFace);
void insertF2lPair(int edgeFace, int cornerFace);
void performF2LAlgorithm1();
void performF2LAlgorithm2();
void performF2LAlgorithm3();


void scramble();
void exterior_face(bool top_face);

int main()
{
    /* Ask for the scramble and use it to simulate a scramble of the virtual
    Rubik's cube */
    displayCube();

    cout << "Welcome to Rubik Solver!" << endl
        << "This program solves a Rubik's cube given a scramble of moves and "
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

    if (isSolved()) {
        cout << "The Rubik's Cube is solved!" << endl;
    } else {
        cout << "The Rubik's Cube is not solved yet." << endl;
    }

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
        applyMove(move);
    }

void parseAndApplyMoves(const string& moves) {
    string move = "";
    for (char c : moves) {
        if (c == ' ') {
                applyMove(move);
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
    if (!isValidMove(move) || move.empty()) {
        cout << "Invalid move: " << move << endl;
        return;
    }

    char temp[N_ROWS];

    char face = move[0];
    bool is_prime = false, is_double = false;
    if (2 == move.length()) {
        is_prime = move[1] == '\'';
        is_double = move[1] == '2';
    }

    /* Calculates the number of moves to be made based on whether a prime move
    and a double move is requested */
    const size_t TIMES = [&is_double, &is_prime]() {
        size_t times = 1;
        if (is_double) times *= 2;
        if (is_prime) times *= 3;  /* 3 regular moves are equivalent to a prime
                                   move */
        while (times >= 4) times -= 4;
        return times;
        }();
    // Function pointer. Will point to the function that needs to be run.
    void (*func)() = nullptr;
    switch (face) {
    case 'U':
        func = moveU;
        break;
    case 'D':
        func = moveD;
        break;
    case 'F':
        func = moveF;
        break;
    case 'B':
        func = moveB;
        break;
    case 'L':
        func = moveL;
        break;
    case 'R':
        func = moveR;
        break;
    }
    for (size_t i = 0; i < TIMES; ++i)
        func();
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
        if (find(DIRECTIVES_BEGINNING, DIRECTIVES_END, DIRECTIVE)
            == DIRECTIVES_END) {
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


void solveWhiteCross() {
    // Find white edges
    constexpr size_t N_EDGE_ROWS = 2, N_WHITE_EDGES = 4;
    size_t white_edge_positions[N_WHITE_EDGES][3], white_edge_n = 0;
    for (size_t face = 0; face < N_FACES; ++face)
        for (size_t rows = 0; rows < N_EDGE_ROWS; ++rows) {
            if (N_WHITE_EDGES == white_edge_n) continue;
            if ('W' == cube[face][rows][1]) {
                white_edge_positions[white_edge_n][0] = face;
                white_edge_positions[white_edge_n][1] = rows;
                white_edge_positions[white_edge_n][2] = 1;
                ++white_edge_n;
            }
        }

    // Position the edges... somehow...
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
        temp[i] = cube[FRONT][0][i];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[FRONT][0][i] = cube[RIGHT][0][i];
        cube[RIGHT][0][i] = cube[BACK][0][i];
        cube[BACK][0][i] = cube[LEFT][0][i];
        cube[LEFT][0][i] = temp[i];
    }
    rotateFaceClockwise(cube[TOP]);
}

void moveD() {
    char temp[N_COLS];
    for (int i = 0; i < N_COLS; i++) {
        temp[i] = cube[FRONT][2][i];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[FRONT][2][i] = cube[LEFT][2][i];
        cube[LEFT][2][i] = cube[BACK][2][i];
        cube[BACK][2][i] = cube[RIGHT][2][i];
        cube[RIGHT][2][i] = temp[i];
    }
    rotateFaceClockwise(cube[BOTTOM]);
}

void moveF() {
    char temp[N_COLS];
    for (int i = 0; i < N_COLS; i++) {
        temp[i] = cube[TOP][2][i];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[TOP][2][i] = cube[LEFT][2 - i][2];
        cube[LEFT][2 - i][2] = cube[BOTTOM][0][2 - i];
        cube[BOTTOM][0][2 - i] = cube[RIGHT][i][0];
        cube[RIGHT][i][0] = temp[i];
    }
    rotateFaceClockwise(cube[FRONT]);
}

void moveB() {
    char temp[N_COLS];
    for (int i = 0; i < N_COLS; i++) {
        temp[i] = cube[TOP][0][i];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[TOP][0][i] = cube[RIGHT][i][2];
        cube[RIGHT][i][2] = cube[BOTTOM][2][2 - i];
        cube[BOTTOM][2][2 - i] = cube[LEFT][2 - i][0];
        cube[LEFT][2 - i][0] = temp[i];
    }
    rotateFaceClockwise(cube[BACK]);
}

void moveL() {
    char temp[N_COLS];
    for (int i = 0; i < N_COLS; i++) {
        temp[i] = cube[TOP][i][0];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[TOP][i][0] = cube[BACK][2 - i][2];
        cube[BACK][2 - i][2] = cube[BOTTOM][i][0];
        cube[BOTTOM][i][0] = cube[FRONT][i][0];
        cube[FRONT][i][0] = temp[i];
    }
    rotateFaceClockwise(cube[LEFT]);
}

void moveR() {
    char temp[N_COLS];
    for (int i = 0; i < N_COLS; i++) {
        temp[i] = cube[TOP][i][2];
    }
    for (int i = 0; i < N_COLS; i++) {
        cube[TOP][i][2] = cube[FRONT][i][2];
        cube[FRONT][i][2] = cube[BOTTOM][i][2];
        cube[BOTTOM][i][2] = cube[BACK][2 - i][0];
        cube[BACK][2 - i][0] = temp[i];
    }
    rotateFaceClockwise(cube[RIGHT]);
}

// Prime moves
void moveUPrime() { moveU(); moveU(); moveU(); } // instead of going back once
                                                 // it just rotates 3 times
                                                 // clockwise
void moveDPrime() { moveD(); moveD(); moveD(); }
void moveFPrime() { moveF(); moveF(); moveF(); }
void moveBPrime() { moveB(); moveB(); moveB(); }
void moveRPrime() { moveR(); moveR(); moveR(); }
void moveLPrime() { moveL(); moveL(); moveL(); }

// Double moves
void moveU2() { moveU(); moveU(); } //repeats move twice
void moveD2() { moveD(); moveD(); }
void moveF2() { moveF(); moveF(); }
void moveB2() { moveB(); moveB(); }
void moveL2() { moveL(); moveL(); }
void moveR2() { moveR(); moveR(); }

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

void solveF2l() {
    cout << "Solving F2L..." << endl;

    // We need to loop through the 4 slots of the top layer (where the edge-corner pairs are)
    for (int i = 0; i < 4; ++i) {
        // Solve the edge-corner pair at the specified position
        solveF2lPair(i);
    }
}

void solveF2lPair(int pairIndex) {
    // Normalize pairIndex to be within 0-3
    pairIndex = pairIndex % 4;

    bool pairFound = false;
    int attempts = 0;
    const int MAX_ATTEMPTS = 16; // Prevent infinite loops

    while (!pairFound && attempts < MAX_ATTEMPTS) {
        if (isPairInTopLayer(pairIndex, (pairIndex + 1) % 4)) {
            if (isPairAligned(pairIndex, (pairIndex + 1) % 4)) {
                insertF2lPair(pairIndex, (pairIndex + 1) % 4);
                pairFound = true;
            } else {
                orientPair(pairIndex, (pairIndex + 1) % 4);
            }
        } else {
            moveU();
        }
        attempts++;
    }

    if (!pairFound) {
        cout << "Warning: Could not solve F2L pair " << pairIndex << " within reasonable attempts" << endl;
    }
}

bool isPairInTopLayer(int edgeFace, int cornerFace) {
    // Check if both pieces are in the top layer and belong together
    char edgeColor = cube[TOP][1][edgeFace];
    char cornerColor = cube[TOP][cornerFace][cornerFace];

    // Verify if the colors are consistent with the F2L pair
    return (edgeColor == cube[edgeFace][1][1] &&
            cornerColor == cube[cornerFace][1][1]);
}

bool isPairAligned(int edgeFace, int cornerFace) {
    char edgeTarget = cube[edgeFace][1][1];
    char cornerTarget = cube[cornerFace][1][1];

    return (cube[TOP][edgeFace][1] == edgeTarget &&
            cube[TOP][cornerFace][2] == cornerTarget);
}


void orientPair(int edgeFace, int cornerFace) {
    // This function is for orienting the pair into the correct position using the following algorithms

    // Step 1: If the edge is misaligned, bring the edge to the correct position
    // Algorithm 1 (used to orient the pair when the edge is at the top layer but needs to be rotated):
    if (cube[edgeFace][2][1] == 'W' && cube[cornerFace][2][1] == 'W') {
        performF2LAlgorithm1();
    }
    // Step 2: If the edge-corner pair is misaligned (in a different orientation), apply another algorithm
    else if (cube[edgeFace][0][2] == 'W' && cube[cornerFace][2][0] == 'W') {
        performF2LAlgorithm2();
    }
    // Step 3: Apply an algorithm to handle all other cases
    else {
        performF2LAlgorithm3();
    }
}

void insertF2lPair(int edgeFace, int cornerFace) {
    // Once the edge-corner pair is oriented, we can insert it into the first two layers
    // Use a basic F2L algorithm for insertion
    moveU();
    moveR();
    moveRPrime();
    moveUPrime();
}

void performF2LAlgorithm1() {
    // Algorithm 1: Insert edge-corner pair into F2L (simple orientation correction)
    moveR();
    moveU();
    moveRPrime();
    moveUPrime();
}

void performF2LAlgorithm2() {
    // Algorithm 2: Another way to orient and insert the edge-corner pair
    moveU();
    moveR();
    moveUPrime();
    moveRPrime();
    moveUPrime();
}

void performF2LAlgorithm3() {
    // Algorithm 3: A different algorithm to orient and insert the pair
    moveRPrime();
    moveUPrime();
    moveR();
    moveU();
    moveRPrime();
    moveUPrime();
    moveR();
}

void solvePll() {

}

bool isSolved() {
    // Check each face
    for (int face = 0; face < N_FACES; face++) {
        // Get the center color of the current face
        char centerColor = cube[face][1][1];

        // Check if all pieces on this face match the center color
        for (int row = 0; row < N_ROWS; row++) {
            for (int col = 0; col < N_COLS; col++) {
                if (cube[face][row][col] != centerColor) {
                    return false;
                }
            }
        }
    }
    return true;
}

