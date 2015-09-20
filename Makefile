CC= g++ -g
OBJS=fcgirecord.o \
	 main.o

TARGET=main

all : $(TARGET)
	echo "All Done"

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^

%.o : %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f *.o
	echo "Remove Done"
