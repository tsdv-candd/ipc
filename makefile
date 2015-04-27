# makefile -- rules to build OSP workshop C
# to use simply type "make"
# this will build the server and client and launcher script
# note, this is a configuration file for the MAKE utility
# do not try to run it directly
# if typing up the file, the indented lines need to be indented
# with TABS not spaces.
CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lcunit

all: logserver logclient test_driver
	chmod +x launch_server.sh

clean:
	rm -f *.o logserver logclient test_driver

logclient: logclient.o logservice.o
	$(CC) $(CFLAGS) logclient.o logservice.o -o logclient
logservice.o: logservice.c logservice.h

logserver: logserver.o logservice.o
	$(CC) $(CFLAGS) logserver.o logservice.o -o logserver
logserver.o: logserver.c logservice.h

logclient.o: logclient.c logservice.h

test_driver: test_driver.o logservice.o
	$(CC) $(CFLAGS)  $(LDFLAGS) test_driver.o logservice.o -o test_driver