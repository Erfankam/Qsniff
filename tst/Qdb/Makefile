CC = gcc
INCLUDES = ${shell mysql_config --include}
LIBS = ${shell mysql_config --libs}

all: testqdb

testqdb: testqdb.c qdb.c 
			$(CC) -o testqdb testqdb.c qdb.c $(INCLUDES) $(LIBS)

clean:
			rm -f testqdb
