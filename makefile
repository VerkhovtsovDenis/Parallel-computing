output: main.o prototype.o
	g++ main.o prototype.o -std=c++20 -fopenmp -o output
	rm -f *.o *.gch
	./output.exe

main.o: main.cpp
	g++ -c main.cpp -std=c++20 -fopenmp
prototype.o: prototype.cpp prototype.h
	g++ -c prototype.cpp prototype.h -std=c++20 -fopenmp