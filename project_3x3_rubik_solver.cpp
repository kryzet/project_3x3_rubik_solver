// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

using namespace std;

// BEGIN DEPRECATED CODE

// Define the cube's state structure

// Define the movable pieces
/* We represent the orientation of a piece using all the possible modulo values
for N_<PIECE>_ORIENTS. We also represent the number of all possible pieces of
the same kind using the group of modulo values for N_<PIECE>S */

/* A corner has 3 possible orientations, and 8 corners exist on a 3x3 Rubik's
cube */
constexpr unsigned int N_CORNERS = 8, N_CORNER_ORIENTS = 3;
struct Corners {
    array<unsigned int, N_CORNERS> corner_permutation, corner_orientations;

    /* Only half of the corners change state in a physical cube when a rotation
    is performed */
    void rotate(const array<size_t, N_CORNERS / 2> CORNERS_N, bool clockwise) {
        unsigned int temp;
        if (clockwise) {
            temp = corner_permutation[CORNERS_N[3]];
        }
    }
};
/* An edge has 2 possible orientations, and 12 edges exist on a 3x3 Rubik's
cube */
constexpr unsigned int N_EDGES = 12, N_EDGE_ORIENTS = 2;
struct Edges {
    array<unsigned int, N_EDGES> edge_permutation, edge_orientations;

    void flip(size_t edge_n) {
        edge_orientations[edge_n] = 1 - edge_orientations[edge_n];
    }
};

/* Permutations should be sorted when the cube is solved, and the orientations
should be all 0 */
struct CubeState {
    Corners corners;
    Edges edges;
};

//struct CubeState {
//    std::array<uint8_t, N_EDGES> edge_permutation,
//        edge_orientations;    // Values: 0 or 1 (mod N_EDGE_ORIENTS)
//    std::array<uint8_t, N_CORNERS> corner_permutation,  
//        corner_orientations;  // Values: 0, 1, or 2 (mod N_CORNER_ORIENTS)
//};

// END DEPRECATED CODE

/* Declare the face arrays with a compile-time constant for the
number of pieces per face. */
constexpr size_t N_FACES = 6, N_ROWS = 3, N_COLS = 3,
UP = 0, LEFT = 1, FRONT = 2, RIGHT = 3, BACK = 4, DOWN = 5;
array<array<array<char, N_COLS>, N_ROWS>, N_FACES> cube,
defaultCube = { {
        {{{{'W', 'W', 'W'}},
          {{'W', 'W', 'W'}},
          {{'W', 'W', 'W'}}}},

        {{{{'O', 'O', 'O'}},
          {{'O', 'O', 'O'}},
          {{'O', 'O', 'O'}}}},

        {{{{'G', 'G', 'G'}},
          {{'G', 'G', 'G'}},
          {{'G', 'G', 'G'}}}},

        {{{{'R', 'R', 'R'}},
          {{'R', 'R', 'R'}},
          {{'R', 'R', 'R'}}}},

        {{{{'B', 'B', 'B'}},
          {{'B', 'B', 'B'}},
          {{'B', 'B', 'B'}}}},

        {{{{'Y', 'Y', 'Y'}},
          {{'Y', 'Y', 'Y'}},
          {{'Y', 'Y', 'Y'}}}}
    } };

// A data structure for storing the coordinates of a color tag
typedef struct color_coords {
    size_t face, row, col;
} color_coords;
/* Operator overload for allowing the comparison of two instances of the new
datatype */
bool operator==(const color_coords& lhs, const color_coords& rhs) {
    return lhs.face == rhs.face &&
        lhs.row == rhs.row &&
        lhs.col == rhs.col;
}

// functions
void printFace(array<array<char, N_COLS>, N_ROWS> face);
void rotateTopFaceEdges();

// Define valid moves
enum Move { U, U_, D, D_, L, L_, R, R_, F, F_, B, B_};

/* TODO (kryzet, 22003): Remove unnecessary functions and reorganize
prototypes, then change the order of function definitions to match the order
of prototypes */
void rotateFaceClockwise(array<array<char, N_COLS>, N_ROWS>);
void rotateFaceCounterClockwise(array<array<char, N_COLS>, N_ROWS>);
bool isValidMove(const string& move);
void applyMove(const string& move);
void parseAndApplyMoves(const string& moves);
void displayCube();
void exterior_face(bool top_face);
void resetCube();
bool isCubeSolved();
// TODO (kryzet): Implement these functions
void solveWhiteCross();
void solveWhiteCorners();
void solveMiddleLayer();
// TODO (22003): Implement these functions
void solveLastLayer();// This will include all functions for solving last layer
void solveYellowCross();
void solveYellowCorners();
void solveYellowEdges();
void positionYellowCorners();
void orientYellowCorners();
bool isYellowEdgesOriented();
bool isYellowCornersPositioned();
bool isYellowCrossShape();
bool isYellowLineShape();
bool isYellowLShape();


// Cube moves
void move_x(const bool);
void move_y(const bool);
void move_z(const bool);

// Face moves
void moveU();
void moveD();
void moveF();
void moveB();
void moveR();
void moveL();

void scramble();

// Algorithms
void rightyAlg();
void ReverserightyAlg();
void sledgehammerMove();


int main()
{
    unsigned int choice;
    resetCube();

    do {
        // Display the menu
        cout << "=== Welcome to Rubik's Cube Solver ===" << endl
            << "1. Display cube" << endl
            << "2. Scramble cube" << endl
            << "3. Solve white cross" << endl
            << "4. Solve white corners" << endl
            << "5. Solve last layer" << endl
            << "6. Check if cube is solved" << endl
            << "7. Reset cube" << endl
            << "8. Exit" << endl
            << "======================================" << endl
            << "Enter your choice (1-8): ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();  // Clear the error flag
            // Ignore the rest of the invalid input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Please enter a number between 1 and 8.\n";
            continue;  // Will reshow the menu
        }

        // Handle user input
        switch (choice) {
        case 1:
            displayCube();
            break;
        case 2:
            scramble();
            break;
        case 3:
            solveWhiteCross();
            break;
        case 4:
            solveWhiteCorners();
            break;
        case 5:
            solveLastLayer();
            break;
        case 6:
            if (isCubeSolved()) {
                cout << "The Rubik's Cube is solved!" << endl;
            }
            else {
                cout << "The Rubik's Cube is not solved yet." << endl;
            }
            break;
        case 7:
            resetCube();
            cout << "The cube has been reset." << endl;
            break;
        case 8:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please enter a number between 1 and 8."
                << endl;
        }
    } while (choice != 7);  // Loop until the user chooses to exit

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


void rotateFaceClockwise(array<array<char, N_COLS>, N_ROWS> face) {
    array<array<char, N_COLS>, N_ROWS> temp;

    // Copy the original face
    for (size_t i = 0; i < N_ROWS; i++) {
        for (size_t j = 0; j < N_COLS; j++) {
            temp[i][j] = face[i][j];
        }
    }

    // Rotate 90 degrees clockwise
    for (size_t i = 0; i < N_ROWS; i++) {
        for (size_t j = 0; j < N_COLS; j++) {
            face[j][N_COLS - 1 - i] = temp[i][j];
        }
    }
}


void rotateFaceCounterClockwise(array<array<char, N_COLS>, N_ROWS> face) {
    array<array<char, N_COLS>, N_ROWS> temp;

    // Copy the original face
    for (size_t i = 0; i < N_ROWS; i++) {
        for (size_t j = 0; j < N_COLS; j++) {
            temp[i][j] = face[i][j];
        }
    }

    // Rotate 90 degrees counter-clockwise
    for (size_t i = 0; i < N_ROWS; i++) {
        for (size_t j = 0; j < N_COLS; j++) {
            face[N_ROWS - 1 - j][i] = temp[i][j];
        }
    }
}



void solveWhiteCross() {
    typedef struct w_edge {
        color_coords coords;
        char other_color = '\0';
    } w_edge;
    constexpr size_t N_WHITE_EDGES = 4;

    // Find white edges
    // Will store search results in the below array, using a counter
    array<w_edge, N_WHITE_EDGES> white_edges;
    size_t white_edge_n = 0;
    for (size_t face_n = 0; face_n < N_FACES && N_WHITE_EDGES > white_edge_n;
        ++face_n) {
        constexpr size_t MIDDLE = 1;
        // Only two rows and two columns of every face can include a white edge
        constexpr array<size_t, 2> RCS_N = { 0, 2 };
        array<array<char, N_COLS>, N_ROWS>& face = cube[face_n];
        // Two conditions for breaking the below loop
        // 1. We have found 4 white edges
        // 2. We exhausted all four edge locations
        while (N_WHITE_EDGES > white_edge_n)
            for (const size_t RC_N : RCS_N) {
                bool white_edge_found = false;
                if ('W' == face[RC_N][MIDDLE]) {
                    white_edges[white_edge_n].coords = { face_n, RC_N, MIDDLE };
                    white_edge_found = true;
                }
                else if ('W' == face[MIDDLE][RC_N]) {
                    white_edges[white_edge_n].coords = { face_n, MIDDLE, RC_N };
                    white_edge_found = true;
                }
                if (white_edge_found) ++white_edge_n;
            }
    }

    // We have found all the white edges!
    displayCube();
    for (w_edge white_edge : white_edges) {
        //// Ask the user for the other color of the white edge

        //// Keep prompting the user until receiving valid input
        //constexpr char WHITE_EDGE_COLORS[] = { 'O', 'R', 'G', 'B' };
        //string input = "\0";
        //const char* COLORS_END = end(WHITE_EDGE_COLORS);
        //while (find(begin(WHITE_EDGE_COLORS), COLORS_END, input[0])
        //    == COLORS_END) {
        //    // Ignore any remnant input from other prompts
        //    cin.ignore(numeric_limits<streamsize>::max(), '\n');
        //    cout << "What is the other color of the white edge on face "
        //        << white_edge.coords.face << ", row "
        //        << white_edge.coords.row << " and column "
        //        << white_edge.coords.col << "? ";
        //    getline(cin, input);
        //}
        //white_edge.other_color = input[0];


        // Make a move based on the white edge's other color
        color_coords correct_coords = { .face = UP };
        switch (white_edge.other_color) {
        case 'O':
            correct_coords.row = 1;
            correct_coords.col = 0;
            break;
        case 'R':
            correct_coords.row = 1;
            correct_coords.col = 2;
            break;
        case 'G':
            correct_coords.row = 2;
            correct_coords.col = 1;
            break;
        case 'B':
            correct_coords.row = 0;
            correct_coords.col = 1;
            break;
        default:
            throw;
        }
        size_t case_n = numeric_limits<size_t>::max();
        // White edge correctly positioned and oriented
        if (white_edge.coords == correct_coords)
            continue;
        else if (cube[][1][1] == white_edge.other_color)
    }

    //bool done = false;
    //while (!done) {
    //    //for (size_t white_edge = 0; white_edge < N_WHITE_EDGES; ++white_edge)
    //    for (size_t face = 0, white_edge = 0;
    //        face < N_FACES && white_edge < N_WHITE_EDGES;
    //        ++face)
    //        for (size_t row = 0; row < N_ROWS; ++row)
    //            for (size_t col = 0; col < N_COLS; ++col)
    //                if ('W' == cube[face][row][col]) {
    //                    white_edges[white_edge] = {face, row, col};
    //                    ++white_edge;
    //                }
    //    for (array<size_t, 3> white_edge : white_edges) {
    //        if (UP != white_edge[0]) {
    //            done = false;
    //            break;
    //        }
    //        done = true;
    //    }
    //    if (done) continue;
    //    for (array<size_t, 3> white_edge : white_edges) {
    //        // TODO: Skip good edges
    //        size_t case_n = numeric_limits<size_t>::max();
    //        if (UP == white_edge[0]) case_n = 0;
    //        else if ([white_edge]() {
    //            constexpr array<size_t, 4> MIDDLE_FACES = {{LEFT, FRONT,
    //            RIGHT, BACK}};
    //            for (const size_t FACE : MIDDLE_FACES)
    //                if (FACE == white_edge[0]) {
    //                    return true;
    //                }
    //            return false;
    //            }()) {
    //            if ([white_edge]() {
    //                constexpr array<size_t, 2> CASE_1 = {{0, 2}};
    //                for (const size_t ROW_N : CASE_1)
    //                    if (ROW_N == white_edge[1]) return true;
    //                return false;
    //                }())
    //                case_n = 1;
    //            else if (1 == white_edge[1])
    //                case_n = 2;
    //            }
    //            switch (case_n) {
    //            case 0:
    //                
    //                break;
    //            default:
    //                throw;
    //        }
    //    }
    //}

    // Form a daisy
    //for (white_edge_n = 0; white_edge_n < N_WHITE_EDGES; ++white_edge_n) {
    //        string move;
    //    //if (white_edge[]);
    //    //switch (row) {
    //    //case 2:
    //    //    switch (face) {
    //    //    case LEFT:
    //    //        move += "L";
    //    //        break;
    //    //    case FRONT:
    //    //        move += "F";
    //    //        break;
    //    //    case RIGHT:
    //    //        move += "R";
    //    //        break;
    //    //    case BACK:
    //    //        move += "B";
    //    //    }
    //    //    move += " ";
    //    //    break;
    //    //case 1:
    //    //    switch (face) {
    //    //    case LEFT:
    //    //        move += "F";
    //    //        break;
    //    //    case FRONT:
    //    //        move += "R";
    //    //        break;
    //    //    case RIGHT:
    //    //        move += "B";
    //    //        break;
    //    //    case BACK:
    //    //        move += "L";
    //    //        break;
    //    //    }
    //    //    move += " ";
    //    //    break;
    //    //case 0:
    //    //    break;
    //    //}
    //    }

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
    cout << "The first face is UP, then follow LEFT, FRONT, RIGHT, and BACK, "
        << "then the last" << endl << "face is DOWN." << endl;

    // Print the top face
    exterior_face(true);

    // Print the middle faces
    constexpr size_t N_MIDDLE_FACES = N_FACES - 1;
    for (size_t row = 0; row < N_ROWS; row++) {
        for (size_t face = 1; face < N_MIDDLE_FACES; ++face)
            for (size_t col = 0; col < N_COLS; ++col)
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


// Cube moves
void move_x(const bool PRIME) {

}

void move_y(const bool PRIME) {

}

void move_z(const bool PRIME) {

}

// Face moves
void moveU() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[FRONT][0][i];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[FRONT][0][i] = cube[RIGHT][0][i];
        cube[RIGHT][0][i] = cube[BACK][0][i];
        cube[BACK][0][i] = cube[LEFT][0][i];
        cube[LEFT][0][i] = temp[i];
    }
    rotateFaceClockwise(cube[UP]);
}

void moveD() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[FRONT][2][i];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[FRONT][2][i] = cube[LEFT][2][i];
        cube[LEFT][2][i] = cube[BACK][2][i];
        cube[BACK][2][i] = cube[RIGHT][2][i];
        cube[RIGHT][2][i] = temp[i];
    }
    rotateFaceClockwise(cube[DOWN]);
}

void moveF() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[UP][2][i];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[UP][2][i] = cube[LEFT][2 - i][2];
        cube[LEFT][2 - i][2] = cube[DOWN][0][2 - i];
        cube[DOWN][0][2 - i] = cube[RIGHT][i][0];
        cube[RIGHT][i][0] = temp[i];
    }
    rotateFaceClockwise(cube[FRONT]);
}

void moveB() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[UP][0][i];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[UP][0][i] = cube[RIGHT][i][2];
        cube[RIGHT][i][2] = cube[DOWN][2][2 - i];
        cube[DOWN][2][2 - i] = cube[LEFT][2 - i][0];
        cube[LEFT][2 - i][0] = temp[i];
    }
    rotateFaceClockwise(cube[BACK]);
}

void moveL() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[UP][i][0];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[UP][i][0] = cube[BACK][2 - i][2];
        cube[BACK][2 - i][2] = cube[DOWN][i][0];
        cube[DOWN][i][0] = cube[FRONT][i][0];
        cube[FRONT][i][0] = temp[i];
    }
    rotateFaceClockwise(cube[LEFT]);
}

void moveR() {
    array<char, N_COLS> temp;
    for (size_t i = 0; i < N_COLS; i++) {
        temp[i] = cube[UP][i][2];
    }
    for (size_t i = 0; i < N_COLS; i++) {
        cube[UP][i][2] = cube[FRONT][i][2];
        cube[FRONT][i][2] = cube[DOWN][i][2];
        cube[DOWN][i][2] = cube[BACK][2 - i][0];
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

void reverserightyAlg() {
    moveUPrime(); moveRPrime(); moveU(); moveR();
}

void sledgehammerMove() {
    moveRPrime(); moveFPrime(); moveR(); moveF();
}

void solveYellowCross() {
    const int MAX_ATTEMPTS = 20; // Increased attempts
    int attempts = 0;

    cout << "Solving Yellow Cross..." << endl;
    while (!isYellowCrossShape() && attempts < MAX_ATTEMPTS) {
        // Diagnostic print
        cout << "Current Yellow Cross Attempt: " << attempts + 1 << endl;

        // Check for different starting configurations
        if (isYellowLShape()) {
            // Rotate to get line or dot configuration
            moveU();
            cout << "Detected L-Shape, rotating U" << endl;
        }

        // Primary cross-forming algorithm
        moveF();
        moveR();
        moveU();
        moveRPrime();
        moveUPrime();
        moveFPrime();

        // If still not solved, try alternate algorithm
        if (!isYellowCrossShape()) {
            moveF();
            moveR();
            moveU();
            moveRPrime();
            moveUPrime();
            moveFPrime();
        }

        attempts++;
    }

    // Debug print for cross shape
    cout << "Yellow Cross Shape: " << (isYellowCrossShape() ? "Solved" : "Not Solved") << endl;
}
void solveYellowEdges() {
    // Keep applying algorithm until yellow edges are oriented correctly
    while (!isYellowEdgesOriented()) {
        // R U R' U R U2 R'
        moveR();
        moveU();
        moveRPrime();
        moveU();
        moveR();
        moveU2();
        moveRPrime();
    }
}

void positionYellowCorners() {
    const int MAX_ATTEMPTS = 20;
    int attempts = 0;

    cout << "Positioning Yellow Corners..." << endl;
    while (!isYellowCornersPositioned() && attempts < MAX_ATTEMPTS) {
        cout << "Current Corner Positioning Attempt: " << attempts + 1 << endl;

        // Diagnostic print of current corner positions
        cout << "Current Corner Positions:" << endl;
        cout << "Front Corners: "
            << cube[FRONT][0][0] << " " << cube[FRONT][0][2]
            << " (Center: " << cube[FRONT][1][1] << ")" << endl;
        cout << "Right Corners: "
            << cube[RIGHT][0][0] << " " << cube[RIGHT][0][2]
            << " (Center: " << cube[RIGHT][1][1] << ")" << endl;
        cout << "Back Corners: "
            << cube[BACK][0][0] << " " << cube[BACK][0][2]
            << " (Center: " << cube[BACK][1][1] << ")" << endl;
        cout << "Left Corners: "
            << cube[LEFT][0][0] << " " << cube[LEFT][0][2]
            << " (Center: " << cube[LEFT][1][1] << ")" << endl;

        // Positioning algorithm
        moveU();
        moveR();
        moveUPrime();
        moveLPrime();
        moveU();
        moveRPrime();
        moveUPrime();
        moveL();

        attempts++;
    }

    cout << "Yellow Corners Positioning: "
        << (isYellowCornersPositioned() ? "Solved" : "Not Solved") << endl;
}


void alignYellowCorners() {
    // Orient yellow corners
    for (int corner = 0; corner < 4; corner++) {
        // Repeat until corner is oriented correctly
        while (cube[UP][2][2] != 'Y') {
            // R U R' U' (sexy move)
            rightyAlg();
        }
        moveD(); // Move to next corner
    }
}

void orientYellowEdges() {
    const int MAX_ATTEMPTS = 20; // Increased attempts
    int attempts = 0;

    cout << "Orienting Yellow Edges..." << endl;
    while (!isYellowEdgesOriented() && attempts < MAX_ATTEMPTS) {
        cout << "Current Edge Orientation Attempt: " << attempts + 1 << endl;

        // Diagnostic print of current edge orientations
        cout << "Current Edge Orientations:" << endl;
        cout << "Front Edge: " << cube[FRONT][0][1]
            << " (Center: " << cube[FRONT][1][1] << ")" << endl;
        cout << "Right Edge: " << cube[RIGHT][0][1]
            << " (Center: " << cube[RIGHT][1][1] << ")" << endl;
        cout << "Back Edge: " << cube[BACK][0][1]
            << " (Center: " << cube[BACK][1][1] << ")" << endl;
        cout << "Left Edge: " << cube[LEFT][0][1]
            << " (Center: " << cube[LEFT][1][1] << ")" << endl;

        // Find a correctly oriented edge
        for (int i = 0; i < 4; i++) {
            if (cube[FRONT][0][1] == cube[FRONT][1][1])
                break;
            moveU();
            cout << "Rotating U to find oriented edge" << endl;
        }

        // Edge orientation algorithm
        moveR();
        moveU();
        moveRPrime();
        moveU();
        moveR();
        moveU2();
        moveRPrime();

        attempts++;
    }

    cout << "Yellow Edges Orientation: "
        << (isYellowEdgesOriented() ? "Solved" : "Not Solved") << endl;
}

bool isYellowEdgesOriented() {
    return (cube[FRONT][0][1] == cube[FRONT][1][1] &&
        cube[RIGHT][0][1] == cube[RIGHT][1][1] &&
        cube[BACK][0][1] == cube[BACK][1][1] &&
        cube[LEFT][0][1] == cube[LEFT][1][1]);
}
bool isAllCornersOriented() {
    return (cube[UP][0][0] == 'Y' &&
        cube[UP][0][2] == 'Y' &&
        cube[UP][2][0] == 'Y' &&
        cube[UP][2][2] == 'Y');
}

void orientYellowCorners() {
    const int MAX_ATTEMPTS = 20;
    int attempts = 0;

    cout << "Orienting Yellow Corners..." << endl;
    while (!isAllCornersOriented() && attempts < MAX_ATTEMPTS) {
        cout << "Current Corner Orientation Attempt: " << attempts + 1 << endl;

        // Diagnostic print of current corner orientations
        cout << "Current Corner Orientations:" << endl;
        cout << "Top Face Corners: "
            << cube[UP][0][0] << " "
            << cube[UP][0][2] << " "
            << cube[UP][2][0] << " "
            << cube[UP][2][2] << endl;

        // Iterate through each corner
        for (int corner = 0; corner < 4; corner++) {
            // Orientation algorithm
            moveRPrime();
            moveDPrime();
            moveR();
            moveD();

            // Check if current corner is oriented
            if (cube[UP][2][2] == 'Y') {
                break;
            }

            // Rotate to next corner
            moveU();
        }

        attempts++;
    }
}
bool isYellowCornersPositioned() {
    int rotations = 0;
    for (int i = 0; i < 4; i++) {
        if (cube[FRONT][0][0] != cube[FRONT][0][2] ||
            cube[RIGHT][0][0] != cube[RIGHT][0][2] ||
            cube[BACK][0][0] != cube[BACK][0][2] ||
            cube[LEFT][0][0] != cube[LEFT][0][2]) {
            return false;
        }
        moveU();
        rotations++;
    }
    while (rotations < 4) {
        moveU();
        rotations++;
    }
    return true;
}

// Additional helper functions for last layer
bool isYellowCrossShape() {
    return (cube[UP][0][1] == 'Y' && cube[UP][1][0] == 'Y' &&
        cube[UP][1][2] == 'Y' && cube[UP][2][1] == 'Y');
}

bool isYellowLineShape() {
    return ((cube[UP][0][1] == 'Y' && cube[UP][2][1] == 'Y') ||
        (cube[UP][1][0] == 'Y' && cube[UP][1][2] == 'Y'));
}

bool isYellowLShape() {
    return ((cube[UP][0][1] == 'Y' && cube[UP][1][0] == 'Y') ||
        (cube[UP][1][0] == 'Y' && cube[UP][2][1] == 'Y') ||
        (cube[UP][2][1] == 'Y' && cube[UP][1][2] == 'Y') ||
        (cube[UP][1][2] == 'Y' && cube[UP][0][1] == 'Y'));
}

// Final Layer function which includes all functions to be used in final layer
void solveLastLayer() {
    const int MAX_GLOBAL_ATTEMPTS = 50;
    int globalAttempts = 0;
    bool stagesSolved[4] = { false };

    cout << "Starting Last Layer Solution..." << endl;

    while (!isCubeSolved() && globalAttempts < MAX_GLOBAL_ATTEMPTS) {
        // Yellow Cross Stage
        if (!stagesSolved[0]) {
            solveYellowCross();
            stagesSolved[0] = isYellowCrossShape();
        }

        // Yellow Edges Orientation Stage
        if (stagesSolved[0] && !stagesSolved[1]) {
            solveYellowEdges();
            stagesSolved[1] = isYellowEdgesOriented();
        }

        // Yellow Corners Positioning Stage
        if (stagesSolved[0] && stagesSolved[1] && !stagesSolved[2]) {
            positionYellowCorners();
            stagesSolved[2] = isYellowCornersPositioned();
        }

        // Yellow Corners Orientation Stage
        if (stagesSolved[0] && stagesSolved[1] && stagesSolved[2] && !stagesSolved[3]) {
            orientYellowCorners();
            stagesSolved[3] = isAllCornersOriented();
        }

        globalAttempts++;
    }

    // Final verification
    if (isCubeSolved()) {
        cout << "Cube successfully solved in " << globalAttempts << " global attempts!" << endl;
    }
    else {
        cout << "Cube solving failed after " << MAX_GLOBAL_ATTEMPTS << " attempts." << endl;

        // Print which stages were not solved
        cout << "Unsolved Stages:" << endl;
        if (!stagesSolved[0]) cout << "- Yellow Cross" << endl;
        if (!stagesSolved[1]) cout << "- Yellow Edges Orientation" << endl;
        if (!stagesSolved[2]) cout << "- Yellow Corners Positioning" << endl;
        if (!stagesSolved[3]) cout << "- Yellow Corners Orientation" << endl;

        displayCube();  // Show final state for debugging
    }
}


bool isCubeSolved() {
    for (size_t face = 0; face < N_FACES; face++) {
        char centerColor = cube[face][1][1];
        for (size_t row = 0; row < N_ROWS; row++) {
            for (size_t col = 0; col < N_COLS; col++) {
                if (cube[face][row][col] != centerColor) {
                    return false;
                }
            }
        }
    }
    return true;
}

void resetCube() {
    // Copy the default state to the cube
    cube = defaultCube;
}
