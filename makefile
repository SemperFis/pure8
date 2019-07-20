CFLAGS= -Wall -Wextra -Ofast

pure8: pure8.c
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

debug: pure8.c
	$(CC) $(CFLAGS) -DDEBUG -g $? $(LDFLAGS) -o $@

clean:
	rm -f *.o pure8 debug
