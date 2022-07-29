# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library(Prj_Lib "ternary_seach_tree.cpp" "hash_table.cpp" "dictionary.cpp" "database.cpp" "util.cpp")

# Add source to this project's executable.
add_executable (Project_CS163 "main.cpp")

target_link_libraries(Project_CS163 Prj_Lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Project_CS163 Prj_Lib PROPERTY CXX_STANDARD 20)
else()
  set_property(TARGET Project_CS163 Prj_Lib PROPERTY CXX_STANDARD 17)	
endif()

# TODO: Add tests and install targets if needed.
