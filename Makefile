CC= g++ -g
all : fcgirecord.o main.o
	$(CC) -o main fcgirecord.o main.o

%.o : %.cpp
	$(CC) -c $< -o $@
clean:
	rm -f *.o
