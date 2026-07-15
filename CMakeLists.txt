#-------------------------------------------------------------------------------
# Copyright (c) 2018 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

Project(Glxy
        LANGUAGES    CXX
        VERSION      0.1
        HOMEPAGE_URL "https://github.com/SloeComputers/Glxy")

set(PDK_PROJ_OWNER "Sloe Computers")
set(PDK_PROJ_EMAIL "sloecomputer@gmail.com")

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

add_subdirectory(PDK)
add_subdirectory(Source)
