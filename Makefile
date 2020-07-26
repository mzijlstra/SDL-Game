src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -lSDL2 -lSDL2_image

game: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) game