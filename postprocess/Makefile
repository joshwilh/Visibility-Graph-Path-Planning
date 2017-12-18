CXX = g++

debug_or_optimize = -O1
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 $(debug_or_optimize)

postprocess.exe: postprocess.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.SUFFIXES:

.PHONY: clean
clean:
	rm -vf *.out *.exe

