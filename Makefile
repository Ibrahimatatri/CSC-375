CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

SRC = src/main.cpp src/HashTable.cpp src/Feedback.cpp src/WordList.cpp
OUT = wordle

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

test: tests/TestRunner.cpp src/HashTable.cpp src/Feedback.cpp src/WordList.cpp
	$(CXX) $(CXXFLAGS) tests/TestRunner.cpp src/HashTable.cpp src/Feedback.cpp src/WordList.cpp -o test_runner
	./test_runner
clean:
	rm -f $(OUT) test_runner
