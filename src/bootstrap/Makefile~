CC = gcc
INCLUDES = ${shell mysql_config --include}
SQLLIBS = ${shell mysql_config --libs}
CFGLIBS = -lconfig

all: bootstrap

bootstrap.o: bootstrap.c
			$(CC) -c $(INCLUDES) bootstrap.c

bootstrap: bootstrap.o
			$(CC) -o bootstrap bootstrap.o $(SQLLIBS) $(CFGLIBS)

clean:
			rm -f bootstrap bootstrap.o
