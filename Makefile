#based on https://stackoverflow.com/a/30602701/6933102

SRC_DIR := src
OBJ_DIR := obj

EXE := game
SRC := $(wildcard $(SRC_DIR)/*.c) 
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLASGS := -g -W -Wall -Wextra -pedantic -Iinclude -MMD -MP
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) 
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLASGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
