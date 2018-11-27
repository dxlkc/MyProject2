
CC=gcc

../RELEASE/recvfile : *.o
	$(CC) -o ../RELEASE/recvfile *.o -lpthread 

*.o : 
	$(CC) -c *.c -lpthread

.PHONY : clean
clean :
	rm -f *.o

