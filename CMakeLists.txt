cmake_minimum_required(VERSION 3.15)
project(check3 C)

set(CMAKE_C_STANDARD 99)

add_executable(check3 utilities/RBUtilities.c Structs.c testsmaor.c)