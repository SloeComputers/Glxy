
cmake_minimum_required(VERSION 3.11.4)

project(Glxy
        VERSION 0.0.0
        LANGUAGES C CXX ASM)

set(source  Source/glxy.cpp Source/KeplerianElements.cpp)
set(binary  glxy)

#-------------------------------------------------------------------------------

include(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${PLT_libs})
