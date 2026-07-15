#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_executable(glxy
               glxy.cpp
               KeplerianElements.cpp)

target_link_libraries(glxy PRIVATE GUI TGT)

set_target_properties(glxy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
