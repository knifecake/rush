CC			= gcc
CFLAGS	= -Wall -std=c99

BUILD_DIR = build
EXE				= game

src	= $(wildcard src/*.c) \
			$(wildcard src/**/*.c)
lib = $(wildcard lib/*.c)
obj = $(src:.c=.o) \
			$(lib:.c=.o)

test_sources = $(wildcard test/test_*.c)
test_objs = $(test_sources:.c=.o)
test_exes = $(test_sources:.c=)

game: $(obj)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(EXE) $^ -lm -lpng
	@make clear

# compiles all tests
.PHONY: test
test: $(test_exes)
	$(foreach test,$(test_exes), $(test);)

# compiles whichever test you tell it to
test_%: $(test_objs) test/minitest.o
	$(CC) $(CFLAGS) -o $(@:.c=) $^ -lm -lpng

run: game
	@echo "\n🤡  Running THE GAME 🤡\n"
	./$(BUILD_DIR)/$(EXE)

.PHONY: clear
clear:
	@rm -rf **/**/*.o **/*.o

.PHONY: clean
clean: clear
	@rm -rf $(BUILD_DIR)/*
	@rm $(test_exes)
