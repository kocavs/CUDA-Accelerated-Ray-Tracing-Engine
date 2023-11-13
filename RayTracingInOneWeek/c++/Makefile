# Specify the compiler
CXX = g++

# Specify the source file
SRC = main.cc

# Specify the output file
OUT = main

# Specify the flags for the compiler
CXXFLAGS = -Wall -std=c++11

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)