CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

all: main
	./main

plot: main
	./main
	python3 plot.py

img: main
	./main
	open out.bmp

main: Fractal.cpp Fractal.h Image.cpp Image.h Color.cpp Color.h main.cpp
	$(CXX) $(CXXFLAGS) Fractal.cpp Image.cpp Color.cpp main.cpp -o main

clean:
	rm -f main boundary.txt points.txt
	rm -rf main.dSYM