#based on https://stackoverflow.com/a/30602701/6933102

OBJ_DIR := ../obj

EXE := test
SRC := $(wildcard *.c) $(wildcard)
OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)

CFLASGS := -g -W -Wall -Wextra -pedantic -I../include -I../src -MMD -MP
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer 

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) 
	$(CC) @< $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c 
	$(CC) $(CFLASGS) -c $< -o $@

clean:
	@$(RM) -rv  $(OBJ_DIR)

-include $(OBJ:.o=.d)
