CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

SRC = src/main.cpp src/HashTable.cpp src/Feedback.cpp
OUT = wordle

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
