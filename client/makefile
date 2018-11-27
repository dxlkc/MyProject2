
CC=gcc

../RELEASE/client : *.o
	$(CC) -o ../RELEASE/client *.o -lpthread 

*.o : 
	$(CC) -c *.c -lpthread

.PHONY : clean
clean :
	rm -f *o
