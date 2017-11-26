CXX = g++

debug_or_optimize = -O1
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 $(debug_or_optimize)

main.exe: search_functions.cpp Tree_Node.cpp Tree.cpp AStarProblem.cpp AStarState.cpp preprocessing.cpp main.cpp
    $(CXX) $(CXXFLAGS) $^ -o $@

.SUFFIXES:

.PHONY: clean
clean:
    rm -vf *.out *.exe

