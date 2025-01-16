output: main.o prototype.o
	g++ -std=c++20 main.o prototype.o -fopenmp -openmp -o output
main.o: main.cpp
	g++ -std=c++20 -c main.cpp -fopenmp -openmp
prototype.o: prototype.cpp prototype.h
	g++ -std=c++20 -c prototype.cpp prototype.h -openmp