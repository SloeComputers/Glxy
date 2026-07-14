#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(Glxy
        LANGUAGES CXX
        VERSION   0.0
        HOMEPAGE_URL "https://github.com/SloeComputers/Chain")

set(PDK_PROJ_OWNER "Sloe Computers")
set(PDK_PROJ_EMAIL "sloecomputer@gmail.com")

add_subdirectory(PDK)

add_executable(glxy Source/glxy.cpp Source/KeplerianElements.cpp)
target_link_libraries(glxy PRIVATE GUI PLT)
