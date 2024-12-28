// Copyright (C) 2024 Hassan "Kryzet" Khalloof
// 
// This file is part of Rubik Solver.
// 
// Rubik Solver is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
// 
// Rubik Solver is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
// 
// You should have received a copy of the GNU General Public License along with
// Rubik Solver. If not, see <https://www.gnu.org/licenses/>.

// project_3x3_rubik_solver.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <array>
#include <string>
#include <cstdbool>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
// clear_console()
// TODO: Add support for macOS and Linux
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif