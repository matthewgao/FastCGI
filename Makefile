all : fcgirecord.o main.o
	g++ -o main fcgirecord.o main.o

fcgirecord.o : fcgirecord.cpp
	g++ -c fcgirecord.cpp
main.o : main.cpp
	g++ -c main.cpp

