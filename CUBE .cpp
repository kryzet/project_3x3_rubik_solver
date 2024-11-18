// project_3x3_cube_solver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

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

    for (int i = 0; i < ROW ; ++i ){
        for (int j = 0; j < COLUMN; ++j){
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
              char rightSide[ROW][COLUMN]){
    
}

void rotateFaceClockwise(char face[ROW][COLUMN]) {
    char temp[ROW][COLUMN];
    
    // Copy the original face
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            temp[i][j] = face[i][j];
        }
    }
    
    // Rotate 90 degrees clockwise
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            face[j][COLUMN-1-i] = temp[i][j];
        }
    }
}

void rotateFaceCounterClockwise(char face[ROW][COLUMN]) {
    char temp[ROW][COLUMN];
    
    // Copy the original face
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            temp[i][j] = face[i][j];
        }
    }
    
    // Rotate 90 degrees counter-clockwise
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COLUMN; j++) {
            face[ROW-1-j][i] = temp[i][j];
        }
    }
}

bool isValidMove(const string& move) {
    // Basic moves: F, B, R, L, U, D
    // Prime moves: F', B', R', L', U', D'
    // Double moves: F2, B2, R2, L2, U2, D2
    
    if(move.length() > 2) return false;
    
    char face = move[0];
    if(face != 'F' && face != 'B' && face != 'R' &&
       face != 'L' && face != 'U' && face != 'D') {
        return false;
    }
    
    if(move.length() == 2 && move[1] != '\'' && move[1] != '2') {
        return false;
    }
    
    return true;
}
