CC			= gcc
CFLAGS	= -Wall -Wextra -std=c99

BUILD_DIR = build

src	= $(wildcard src/*.c) \
			$(wildcard src/**/*.c)
obj = $(src:.c=.o)

game: $(obj)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^
	@make clear

.PHONY: clear
clear:
	@rm -rf **/**/*.o **/*.o

.PHONY: clean
clean: clear
	@rm -rf $(BUILD_DIR)/*
