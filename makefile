output: main.o prototype.o num_threads.o
	g++ main.o prototype.o num_threads.o -std=c++20 -fopenmp -o output
	rm -f *.o *.gch
	mkdir output
	./output.exe
	

main.o: main.cpp
	g++ -c main.cpp -std=c++20 -fopenmp

run_experements.o: run_experements.hpp
	g++ -c run_experements.hpp -std=c++20

num_threads.o: num_threads.cpp num_threads.h
	g++ -c num_threads.cpp num_threads.h -std=c++20 -fopenmp

prototype.o: prototype.cpp prototype.h
	g++ -c prototype.cpp prototype.h -std=c++20 -fopenmp