
CC=gcc
INCLUDE=-I./ -I/usr/local/mysql/include 
LIBRARY=-L/usr/local/mysql/lib
LIBFILE=-lmysqlclient -lpthread
LIB_PATH=-Wl,-rpath=/usr/local/mysql/lib

../RELEASE/server:*.o
	$(CC) -o ../RELEASE/server *o $(INCLUDE) $(LIBRARY) $(LIB_PATH) $(LIBFILE)

*.o: 
	$(CC) $(INCLUDE) -c *.c $(LIBFILE) 

.PHONY : clean
clean :
	rm -f *o
