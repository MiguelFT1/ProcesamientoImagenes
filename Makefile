CXX = g++
CXXFLAGS = -std=c++17 -Wall
OPENCV = $(shell pkg-config --cflags --libs opencv4 2>/dev/null || pkg-config --cflags --libs opencv 2>/dev/null)

TARGET = solucion

all: $(TARGET)

$(TARGET): solucion.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) solucion.cpp $(OPENCV)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
