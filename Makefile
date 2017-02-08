all:
	g++ -Wall -fexceptions -g -std=c++11 -c main.cpp -o main.o
	g++ -Wall -fexceptions -g -std=c++11 -c PTO.cpp -o PTO.o
	g++ -Wall -fexceptions -g -std=c++11 -c Date.cpp -o Date.o
	g++ -Wall -fexceptions -g -std=c++11 -c PTODatabase.cpp -o PTODatabase.o
	g++ -o PTO *.o

clean:
	rm *.o PTO
