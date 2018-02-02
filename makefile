CC=g++
CF=-ggdb -Wall

all: server.out client.out

server.out: server.o utils.o svrskt.o skt.o netutils.o
	$(CC) $(CF) -o server.out server.o utils.o netutils.o svrskt.o skt.o

client.out: client.o utils.o netutils.o skt.o cltskt.o
	$(CC) $(CF) -o client.out client.o utils.o netutils.o cltskt.o skt.o

client.o: client.cpp utils.h cltskt.h skt.h netutils.h
	$(CC) $(CF) -c client.cpp

server.o: server.cpp utils.h svrskt.h skt.h netutils.h
	$(CC) $(CF) -c server.cpp

netutils.o: netutils.cpp netutils.h utils.h
	$(CC) $(CF) -c netutils.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CF) -c utils.cpp

svrskt.o: svrskt.cpp svrskt.h skt.h netutils.h utils.h
	$(CC) $(CF) -c svrskt.cpp

cltskt.o: cltskt.cpp cltskt.h skt.h netutils.h utils.h
	$(CC) $(CF) -c cltskt.cpp

skt.o: skt.cpp skt.h netutils.h utils.h
	$(CC) $(CF) -c skt.cpp

clean:
	rm -rf *.o *.out .DS_Store *.dSYM &> /dev/null
