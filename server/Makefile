all:
	gcc -D_REENTRANT -I/usr/include/nptl -o server server.c -L/usr/lib/nptl -lpthread
	#gcc -D_REENTRANT -lpthread -o server server.c
clean:
	rm server
