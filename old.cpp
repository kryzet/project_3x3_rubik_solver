// project_3x3_rubik_solver.cpp : Defines the entry point for the application.
//

#include "project_3x3_rubik_solver.h"

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
    std::array<unsigned int, N_CORNERS> corner_permutation,
        corner_orientations;

    /* Only half of the corners change state in a physical cube when a rotation
    is performed */
    void rotate(const std::array<size_t, N_CORNERS / 2> CORNERS_N, bool clockwise) {
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
    std::array<unsigned int, N_EDGES> edge_permutation, edge_orientations;

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
std::array<std::array<std::array<char, N_COLS>, N_ROWS>, N_FACES> cube,
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
`color_coords` */
bool operator==(const color_coords &lhs, const color_coords &rhs) {
    return lhs.face == rhs.face
           && lhs.row == rhs.row
           && lhs.col == rhs.col;
}

// functions

// Define valid moves
enum Move { U, U_, D, D_, L, L_, R, R_, F, F_, B, B_ };

/* TODO (kryzet, 22003): Remove unnecessary functions and reorganize
prototypes, then change the order of function definitions to match the order
of prototypes */
void rotateFaceClockwise(std::array<std::array<char, N_COLS>, N_ROWS>& face);
void rotateFaceCounterClockwise(std::array<std::array<char, N_COLS>, N_ROWS>& face);
bool isValidMove(const std::string& move);
void applyMove(const std::string& move);
void displayCube(void);
std::string exterior_face(bool);
void resetCube(void);
bool isCubeSolved(void);
// TODO (kryzet): Implement these functions
void solveWhiteCross(void);
void solveWhiteCorners(void);
void solveMiddleLayer(void);
// TODO (22003): Implement these functions
void solveLastLayer(void);
void solveYellowCross(void);
void solveYellowCorners(void);
void solveYellowEdges(void);
void positionYellowCorners(void);
void orientYellowCorners(void);
bool isYellowEdgesOriented(void);
bool isYellowCornersPositioned(void);
bool isYellowCrossShape(void);
bool isYellowLineShape(void);
bool isYellowLShape(void);


// Face moves
void moveU(void);
void moveD(void);
void moveF(void);
void moveB(void);
void moveR(void);
void moveL(void);

void scramble(void);

// Move sequences
void rightySequence(void);
void reverseRightySequence(void);
void sledgehammerSequence(void);

static void clear_console(void) {
    /* The definition of `clear_screen()` below is courtesy of
    https://stackoverflow.com/a/42500322. Please note that the answer has been
    licensed under CC BY-SA 3.0, and the code has been originally adapted from
    https://cplusplus.com/articles/4z18T05o/#OSSpecificWays and modified such
    that it can be cross-compiled and have consistent behavior on all
    mainstream operating systems. The original licensing from
    https://cplusplus.com is unknown. */
#ifdef _WIN32
    HANDLE                     h_std_out;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cell_count;
    COORD                      home_coords = { 0, 0 };

    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_std_out == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(h_std_out, &csbi)) return;
    cell_count = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        h_std_out,
        (TCHAR)' ',
        cell_count,
        home_coords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        h_std_out,
        csbi.wAttributes,
        cell_count,
        home_coords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(h_std_out, home_coords);
#else
/* Kryzet: Not _WIN32. This assumes that it indicates a POSIX-compatible
build environment */
    if (!cur_term)
    {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0) return;
    }
    
    char action[] = "clear";  /* This was added in order to comply with ISO
                               * C++11
                               */
    putp(tigetstr(action));
#endif
}

int main()
{
    resetCube();

    while (true) {  // Loop until the user chooses to exit
        // Display the menu
        unsigned int choice = 0;
        std::cout << "=== Welcome to Rubik's Cube Solver ===\n"
            << "1. Display cube\n"
            << "2. Scramble cube\n"
            << "3. Solve white cross\n"
            << "4. Solve white corners\n"
            << "5. Solve middle layer\n"
            << "6. Solve last layer\n"
            << "7. Check if cube is solved\n"
            << "8. Reset cube\n"
            << "9. Exit\n"
            << "======================================\n"
            << "Enter your choice (1-8): ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();  // Clear the error flag
            // Ignore the rest of the invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input! Please enter a number between 1 and 8"
                << ".\n";
            continue;  // Will reshow the menu
        }
        void (*func)() = nullptr;
        // Handle user input
        switch (choice) {
        case 1:
            func = displayCube;
            break;
        case 2:
            func = scramble;
            break;
        case 3:
            func = solveWhiteCross;
            break;
        case 4:
            func = solveWhiteCorners;
            break;
        case 5:
            func = solveMiddleLayer;
            break;
        case 6:
            func = solveLastLayer;
            break;
        case 7:
            clear_console();
            std::cout << "The Rubik's Cube is ";
            if (not isCubeSolved()) std::cout << "not ";
            std::cout << ".\n";
            break;
        case 8:
            func = resetCube;
            std::cout << "The cube has been reset.\n";
            break;
        case 9:
            std::cout << "Exiting the program. Goodbye!\n";
            exit(0);
        default:
            std::cout << "Invalid choice! Please enter a number between 1 and 8.\n";
        }
        if (func) func();
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // ^ Clear any leftover newline
    std::cout << "Enter the scramble: ";
    std::string move = "", moves; getline(std::cin, moves);

    clear_console();
    // Parse and apply the moves
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

void applyMove(const std::string& move) {
    if (move.empty() || !isValidMove(move)) {
        std::cout << "Invalid move: " << move << '\n';
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
    void (*func)(){};
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

bool isValidMove(const std::string& MOVE) {
    // Basic moves: F, B, R, L, U, D
    // Prime moves: F', B', R', L', U', D'
    // Double moves: F2, B2, R2, L2, U2, D2

    if (MOVE.length() > 2) return false;

    const char FACE = MOVE[0];
    constexpr char VALID_FACES[] = { 'U', 'L', 'F', 'R', 'B', 'D' };
    const char* FACES_BEGINNING = std::begin(VALID_FACES),
        * FACES_END = std::end(VALID_FACES);
    if (std::find(FACES_BEGINNING, FACES_END, FACE) == FACES_END)
        return false;

    if (MOVE.length() == 2) {
        const char DIRECTIVE = MOVE[1];
        constexpr char VALID_DIRECTIVES[] = { '\'', '2' };
        const char* DIRECTIVES_BEGINNING = std::begin(VALID_DIRECTIVES),
            * DIRECTIVES_END = std::end(VALID_DIRECTIVES);
        if (std::find(DIRECTIVES_BEGINNING, DIRECTIVES_END, DIRECTIVE)
            == DIRECTIVES_END) {
            return false;
        }
    }

    return true;
}


void rotateFaceClockwise(std::array<std::array<char, N_COLS>, N_ROWS>& face) {
    std::array<std::array<char, N_COLS>, N_ROWS> temp;

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


void rotateFaceCounterClockwise(std::array<std::array<char, N_COLS>, N_ROWS>
                                &face) {
    std::array<std::array<char, N_COLS>, N_ROWS> temp;

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
    // Useful definitions
    typedef struct w_edge {
        color_coords coords;
        char other_color = '\0';
    } w_edge;
    constexpr size_t N_WHITE_EDGES = 4;
    std::array<w_edge, N_WHITE_EDGES> white_edges;

    // Populate `white_edges`
    // A counter is defined outside of the loops because
    size_t white_edge_n = 0;
    for (size_t face_n = 0; face_n < N_FACES; ++face_n) {
        // White edge search should be terminated once all edges are found
        if (white_edge_n >= N_WHITE_EDGES) break;

        /* On every face, edges are located at {0, 1}, {2, 1}, {1, 0},
        and {1, 2}. These are definitions for searching those specific
        coordinates in a loop and to avoid magic numbers */
        constexpr std::array<size_t, 2> EDGE_RCS_N = { 0, 2 };
        constexpr size_t MIDDLE = 1;

        // Check the edges of the current face for 'W'
        std::array<std::array<char, N_COLS>, N_ROWS> &face = cube[face_n];
        for (const size_t RC_N : EDGE_RCS_N) {
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
    for (w_edge white_edge : white_edges) {
        // Ask the user for the other color of the white edge
        clear_console();
        displayCube();

        // Keep prompting the user until receiving valid input
        constexpr char WHITE_EDGE_COLORS[] = { 'O', 'R', 'G', 'B' };
        std::string input = "\0";
        const char* COLORS_END = std::end(WHITE_EDGE_COLORS);
        while (std::find(std::begin(WHITE_EDGE_COLORS), COLORS_END, input[0])
            == COLORS_END) {
            // Ignore any remnant input from other prompts
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "What is the other color of the white edge on face "
                << white_edge.coords.face << ", row "
                << white_edge.coords.row << ", and column "
                << white_edge.coords.col << "? ";
            getline(std::cin, input);
        }
        white_edge.other_color = input[0];


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
        size_t case_n = std::numeric_limits<size_t>::max();
        // White edge correctly positioned and oriented
        if (white_edge.coords == correct_coords)
            continue;
        //else if (cube[][1][1] == white_edge.other_color)
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
    std::cout << "White corners\n";
}
void solveMiddleLayer() {
    std::cout << "Middle layer\n";
}

void solveOll() {
    std::cout << "Oll\n";
}

void solvePll() {
    std::cout << "Pll\n";
}

void displayCube() {
    clear_console();
    std::cout << "The first face is UP, then follow LEFT, FRONT, RIGHT, and BA"
        "CK, then the last\nface is DOWN.\n";

    // Print the top face
    std::cout << exterior_face(true);

    // Print the middle faces
    constexpr size_t N_MIDDLE_FACES = N_FACES - 1;
    for (size_t row = 0; row < N_ROWS; row++) {
        for (size_t face = 1; face < N_MIDDLE_FACES; ++face)
            for (size_t col = 0; col < N_COLS; ++col)
                std::cout << cube[face][row][col] << ' ';
        std::cout << '\n';
    }

    // Print the bottom face
    std::cout << exterior_face(false);
}

std::string exterior_face(bool top_face) {
    std::string out = "";
    std::array<std::array<char, N_COLS>, N_ROWS>
        &face = cube[(top_face ? 0 : 5)];
    for (std::array<char, N_COLS> &row : face) {
        out += "      ";
        for (char &col : row) {
            out += col + ' ';
        }
        out += '\n';
    }
    return out;
}


// Cube moves
void move_x(const bool PRIME) {
    // X rotation rotates the entire cube around the x-axis
    std::array<std::array<std::array<char, N_COLS>, N_ROWS>, N_FACES>
        temp_cube = cube;

    if (!PRIME) {
        // Clockwise rotation
        cube[UP] = temp_cube[FRONT];
        cube[FRONT] = temp_cube[DOWN];

        // Create temporary copy to rotate
        auto tempBack = temp_cube[BACK];
        rotateFaceClockwise(tempBack);
        rotateFaceClockwise(tempBack);
        cube[DOWN] = tempBack;

        // Create temporary copy to rotate
        auto tempBackUp = temp_cube[UP];
        rotateFaceClockwise(tempBackUp);
        rotateFaceClockwise(tempBackUp);
        cube[BACK] = tempBackUp;

        rotateFaceClockwise(cube[RIGHT]);
        rotateFaceCounterClockwise(cube[LEFT]);
    } else {
        // Counterclockwise rotation (3 clockwise rotations)
        move_x(false);
        move_x(false);
        move_x(false);
    }
}

void move_y(const bool PRIME) {
    // Y rotation rotates entire cube around y-axis
    std::array<std::array<std::array<char, N_COLS>, N_ROWS>, N_FACES> cube;

    if (!PRIME) {
        // Clockwise rotation of side faces
        for (size_t i = 0; i < N_COLS; i++) {
            std::array<char, N_COLS> temp;

            temp[i] = cube[FRONT][0][i];
            cube[FRONT][0][i] = cube[RIGHT][0][i];
            cube[RIGHT][0][i] = cube[BACK][0][i];
            cube[BACK][0][i] = cube[LEFT][0][i];
            cube[LEFT][0][i] = temp[i];
        }

        // Rotate top and bottom faces
        rotateFaceClockwise(cube[UP]);
        rotateFaceCounterClockwise(cube[DOWN]);
    } else {
        // Counterclockwise rotation (3 clockwise rotations)
        move_y(false);
        move_y(false);
        move_y(false);
    }
}

void move_z(const bool PRIME) {
    // Z rotation rotates entire cube around z-axis
    std::array<std::array<std::array<char, N_COLS>, N_ROWS>, N_FACES>
        temp_cube = cube;

    if (!PRIME) {
        // Clockwise rotation
        for (size_t row = 0; row < N_ROWS; row++) {
            for (size_t col = 0; col < N_COLS; col++) {
                cube[UP][row][col] = temp_cube[LEFT][2 - col][row];
                cube[RIGHT][row][col] = temp_cube[UP][row][col];
                cube[DOWN][row][col] = temp_cube[RIGHT][row][col];
                cube[LEFT][row][col] = temp_cube[DOWN][2 - col][row];
            }
        }

        rotateFaceClockwise(cube[FRONT]);
        rotateFaceCounterClockwise(cube[BACK]);
    } else {
        // Counterclockwise rotation (3 clockwise rotations)
        move_z(false);
        move_z(false);
        move_z(false);
    }
}


// Face moves
void moveU() {
    std::array<char, N_COLS> temp;
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
    std::array<char, N_COLS> temp;
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
    std::array<char, N_COLS> temp;
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
    std::array<char, N_COLS> temp;
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
    std::array<char, N_COLS> temp;
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
    std::array<char, N_COLS> temp;
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

// Move sequences
void rightySequence() {
    moveR(); moveU(); moveRPrime(); moveUPrime();
}

void leftySequence() {
    moveL(); moveUPrime(); moveLPrime(); moveU();
}

void reverseRightySequence() {
    moveUPrime(); moveRPrime(); moveU(); moveR();
}

void sledgehammerSequence() {
    moveRPrime(); moveFPrime(); moveR(); moveF();
}

void solveYellowCross() {
    const int MAX_ATTEMPTS = 20; // Increased attempts
    int attempts = 0;

    std::cout << "Solving Yellow Cross...\n";
    while (!isYellowCrossShape() && attempts < MAX_ATTEMPTS) {
        // Diagnostic print
        std::cout << "Current Yellow Cross Attempt: " << attempts + 1 << '\n';

        // Check for different starting configurations
        if (isYellowLShape()) {
            // Rotate to get line or dot configuration
            moveU();
            std::cout << "Detected L-Shape, rotating U\n";
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
    std::cout << "Yellow Cross Shape: ";
    if (not isYellowCrossShape()) std::cout << "Not ";
    std::cout << "Solved\n";
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

    std::cout << "Positioning Yellow Corners...\n";
    while (!isYellowCornersPositioned() && attempts < MAX_ATTEMPTS) {
        std::cout << "Current Corner Positioning Attempt: " << attempts + 1 << '\n';

        // Diagnostic print of current corner positions
        std::cout << "Current Corner Positions:\n";
        std::cout << "Front Corners: "
            << cube[FRONT][0][0] << " " << cube[FRONT][0][2]
            << " (Center: " << cube[FRONT][1][1] << ")\n";
        std::cout << "Right Corners: "
            << cube[RIGHT][0][0] << " " << cube[RIGHT][0][2]
            << " (Center: " << cube[RIGHT][1][1] << ")\n";
        std::cout << "Back Corners: "
            << cube[BACK][0][0] << " " << cube[BACK][0][2]
            << " (Center: " << cube[BACK][1][1] << ")\n";
        std::cout << "Left Corners: "
            << cube[LEFT][0][0] << " " << cube[LEFT][0][2]
            << " (Center: " << cube[LEFT][1][1] << ")\n";

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

    std::cout << "Yellow Corners Positioning: ";
    if (not isYellowCornersPositioned()) std::cout << "Not ";
    std::cout << "Solved\n";
}


void alignYellowCorners() {
    // Orient yellow corners
    for (int corner = 0; corner < 4; corner++) {
        // Repeat until corner is oriented correctly
        while (cube[UP][2][2] != 'Y') {
            // R U R' U' (sexy move)
            rightySequence();
        }
        moveD(); // Move to next corner
    }
}

void orientYellowEdges() {
    const int MAX_ATTEMPTS = 20; // Increased attempts
    int attempts = 0;

    std::cout << "Orienting Yellow Edges...\n";
    while (!isYellowEdgesOriented() && attempts < MAX_ATTEMPTS) {
        std::cout << "Current Edge Orientation Attempt: " << attempts + 1 << '\n';

        // Diagnostic print of current edge orientations
        std::cout << "Current Edge Orientations:\n";
        std::cout << "Front Edge: " << cube[FRONT][0][1]
            << " (Center: " << cube[FRONT][1][1] << ")\n";
        std::cout << "Right Edge: " << cube[RIGHT][0][1]
            << " (Center: " << cube[RIGHT][1][1] << ")\n";
        std::cout << "Back Edge: " << cube[BACK][0][1]
            << " (Center: " << cube[BACK][1][1] << ")\n";
        std::cout << "Left Edge: " << cube[LEFT][0][1]
            << " (Center: " << cube[LEFT][1][1] << ")\n";

        // Find a correctly oriented edge
        for (int i = 0; i < 4; i++) {
            if (cube[FRONT][0][1] == cube[FRONT][1][1])
                break;
            moveU();
            std::cout << "Rotating U to find oriented edge\n";
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

    std::cout << "Yellow Edges Orientation: ";
    if (isYellowEdgesOriented()) std::cout << "Not ";
    std::cout << "Solved\n";
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

    std::cout << "Orienting Yellow Corners...\n";
    while (!isAllCornersOriented() && attempts < MAX_ATTEMPTS) {
        std::cout << "Current Corner Orientation Attempt: " << attempts + 1 << '\n';

        // Diagnostic print of current corner orientations
        std::cout << "Current Corner Orientations:\n";
        std::cout << "Top Face Corners: "
            << cube[UP][0][0] << " "
            << cube[UP][0][2] << " "
            << cube[UP][2][0] << " "
            << cube[UP][2][2] << '\n';

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

// Attempt to solve the last layer using other functions
void solveLastLayer() {
    const int MAX_GLOBAL_ATTEMPTS = 50;
    int globalAttempts = 0;
    bool stagesSolved[4] = { false };

    std::cout << "Starting Last Layer Solution...\n";

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
        std::cout << "Cube successfully solved in " << globalAttempts << " global attempts!\n";
    }
    else {
        std::cout << "Cube solving failed after " << MAX_GLOBAL_ATTEMPTS << " attempts.\n";

        // Print which stages were not solved
        std::cout << "Unsolved Stages:\n";
        if (!stagesSolved[0]) std::cout << "- Yellow Cross\n";
        if (!stagesSolved[1]) std::cout << "- Yellow Edges Orientation\n";
        if (!stagesSolved[2]) std::cout << "- Yellow Corners Positioning\n";
        if (!stagesSolved[3]) std::cout << "- Yellow Corners Orientation\n";

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
