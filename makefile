CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O0
TARGET := matrix
SOURCE := matrix.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)