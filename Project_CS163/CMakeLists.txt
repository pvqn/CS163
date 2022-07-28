# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library(LIB_CS163 "database.cpp"
					  "dictionary.cpp"
					  "hash_table.cpp"
					  "ternary_seach_tree.cpp"
					  "util.h")

# Add source to this project's executable.
add_executable (Project_CS163 "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Project_CS163 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
