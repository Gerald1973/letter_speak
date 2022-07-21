CC=gcc
CFLAGS=-Wall
LDFLAGS=-lespeak-ng -lncurses
EXEC=letterspeak

all: $(EXEC)

letterspeak: letterspeak.o graphism.o characters_c64.o
	$(CC) -o $@ $^ $(LDFLAGS)

letterspeak.o: letterspeak.c
	$(CC) -o $@ -c letterspeak.c $(CFLAGS)

graphism.o: graphism.c
	$(CC) -o $@ -c graphism.c $(CFLAGS)

characters_c64.o: characters_c64.c
	$(CC) -o $@ -c characters_c64.c $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
