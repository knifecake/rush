CC			= gcc
CFLAGS	= -Wall -std=c99

BUILD_DIR = build
EXE				= game

src	= $(wildcard src/*.c) \
			$(wildcard src/**/*.c)
lib = $(wildcard lib/*.c)
obj = $(src:.c=.o) \
			$(lib:.c=.o)

game: $(obj)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(EXE) $^ -lm -lpng
	@make clear

run: game
	@echo "\n🤡  Running THE GAME 🤡\n"
	./$(BUILD_DIR)/$(EXE)

.PHONY: clear
clear:
	@rm -rf **/**/*.o **/*.o

.PHONY: clean
clean: clear
	@rm -rf $(BUILD_DIR)/*
