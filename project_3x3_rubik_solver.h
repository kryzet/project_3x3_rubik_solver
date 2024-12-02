// project_3x3_rubik_solver.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include <array>
#include <string>
#include <cstdbool>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <curses.h>  // Wasn't included in original code
                     // https://linux.die.net/man/3/setupterm
#include <term.h>
#include <unistd.h>  // Included before `term.h` in original code
#endif