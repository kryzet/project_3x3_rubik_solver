// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

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
void resetCube();
bool isSolved();
// TODO (kryzet): Implement these functions
void solveWhiteCross();
void solveWhiteCorners();
void solveMiddleLayer();
// TODO (22003): Implement these functions
void solveLastLayer();// This will include all functions for solving last layer
void solveYellowCross();
void solveYellowCorners();
void positionYellowCorners();
void orientYellowCorners();
bool isYellowEdgesOriented();
bool areYellowCornersPositioned();


// Regular moves
void moveU();
void moveD();
void moveF();
void moveB();
void moveR();
void moveL();

// Algorithms
void rightyAlg();
void ReverserightyAlg();
void sledgehammerMove();


void scramble();
void exterior_face(bool top_face);

void displayMenu() {
    int choice = 0;


    do {
        // Display the menu
        cout << "============ Welcome to Rubik's Cube Solver ============" << endl
            << "1. Scramble the cube" << endl
            << "2. Solve up to F2L " << endl
            << "3. Solve Last Layers" << endl
            << "4. Display cube" << endl
            << "5. Check if cube is solved" << endl
            << "6. Reset the cube" << endl
            << "7. Exit" << endl
            << "=============================================" << endl << endl
            << "Enter your choice (1-8): ";
        cin >> choice;
        if (cin.fail()) {
            // Clear the error flag and ignore the rest of the line
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore the rest of the invalid input
            cout << "Invalid input! Please enter a number between 1 and 8.\n";
            continue;  // Skip to the next iteration of the loop
        }

        // Handle user input
        switch (choice) {
        case 1:
            scramble();
            break;
        case 2:
            solveWhiteCross(); solveWhiteCorners(); solveMiddleLayer();
            break;
        case 3:
            solveLastLayer();
            break;

            case 4:
            displayCube();
            break;
        case 5:
            if (isSolved()) {
                cout << "The Rubik's Cube is solved!\n";
            }
            else {
                cout << "The Rubik's Cube is not solved yet.\n";
            }
            break;
        case 6:
            resetCube();
            cout << "The cube has been reset.\n";
            break;
        case 7:
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please enter a number between 1 and 8.\n";
        }
    } while (choice != 7);  // Loop until the user chooses to exit
}

int main()
{
    /* Ask for the scramble and use it to simulate a scramble of the virtual
    Rubik's cube */
    displayMenu();
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear any leftover newline
    cout << "Enter the scramble: ";
    string moves;
    getline(cin, moves);

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
    if (move.empty() || !isValidMove(move)) {
        cout << "Invalid move: " << move << endl;
        return;
    }

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
    size_t white_edge[N_WHITE_EDGES][3], white_edge_n = 0;
    for (size_t face = 0; face < N_FACES; ++face)
        for (size_t rows = 0; white_edge_n < N_WHITE_EDGES
            && rows < N_EDGE_ROWS; ++rows) {
            if ('W' == cube[face][rows][1]) {
                white_edge[white_edge_n][0] = face;
                white_edge[white_edge_n][1] = rows;
                white_edge[white_edge_n][2] = 1;
                ++white_edge_n;
            }
        }
    
    // Form a daisy
    for (white_edge_n = 0; white_edge_n < N_WHITE_EDGES; ++white_edge_n) {
            string move;
        //if (white_edge[]);
        //switch (rows) {
        //case 2:
        //    switch (face) {
        //    case LEFT:
        //        move += "L";
        //        break;
        //    case FRONT:
        //        move += "F";
        //        break;
        //    case RIGHT:
        //        move += "R";
        //        break;
        //    case BACK:
        //        move += "B";
        //    }
        //    move += " ";
        //    break;
        //case 1:
        //    switch (face) {
        //    case LEFT:
        //        move += "F";
        //        break;
        //    case FRONT:
        //        move += "R";
        //        break;
        //    case RIGHT:
        //        move += "B";
        //        break;
        //    case BACK:
        //        move += "L";
        //        break;
        //    }
        //    move += " ";
        //    break;
        //case 0:
        //    break;
        //}
        }

    // Assemble the cross


}

void solveWhiteCorners() {
    cout << "White corners" << endl;
}
void solveMiddleLayer() {
    cout << "Middle layer" << endl;
}

void solveOll() {
    cout << "Oll" << endl;
}

void solvePll() {
    cout << "Pll" << endl;
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
void rightyAlg() {
    moveR(); moveU(); moveRPrime(); moveUPrime();
}

void leftyAlg() {
    moveL(); moveUPrime(); moveLPrime(); moveU();
}

void reverseSexyMove() {
    moveUPrime(); moveRPrime(); moveU(); moveR();
}

void sledgehammerMove() {
    moveRPrime(); moveFPrime(); moveR(); moveF();
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

void resetCube() {
    constexpr char defaultCube[N_FACES][N_ROWS][N_COLS] = {
        {{'W', 'W', 'W'}, {'W', 'W', 'W'}, {'W', 'W', 'W'}},
        {{'O', 'O', 'O'}, {'O', 'O', 'O'}, {'O', 'O', 'O'}},
        {{'G', 'G', 'G'}, {'G', 'G', 'G'}, {'G', 'G', 'G'}},
        {{'R', 'R', 'R'}, {'R', 'R', 'R'}, {'R', 'R', 'R'}},
        {{'B', 'B', 'B'}, {'B', 'B', 'B'}, {'B', 'B', 'B'}},
        {{'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}, {'Y', 'Y', 'Y'}}
    };

    // Copy the default state back into the cube
    for (size_t face = 0; face < N_FACES; ++face) {
        for (size_t row = 0; row < N_ROWS; ++row) {
            for (size_t col = 0; col < N_COLS; ++col) {
                cube[face][row][col] = defaultCube[face][row][col];
            }
        }
    }
    cout << "The cube has been reset to the solved state." << endl;
}