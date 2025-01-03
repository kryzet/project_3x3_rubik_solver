﻿# Copyright (C) 2024 Hassan "Kryzet" Khalloof
# 
# This file is part of Rubik Solver.
# 
# Rubik Solver is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
# 
# Rubik Solver is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
# 
# You should have received a copy of the GNU General Public License along with
# Rubik Solver. If not, see <https://www.gnu.org/licenses/>.

# CMakeList.txt : CMake project for project_3x3_rubik_solver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("project_3x3_rubik_solver")

# Add source to this project's executable.
add_executable (project_3x3_rubik_solver "project_3x3_rubik_solver.cpp" "project_3x3_rubik_solver.h")
#target_link_libraries(project_3x3_rubik_solver "curses.h" "term.h" "unistd.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET project_3x3_rubik_solver PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
