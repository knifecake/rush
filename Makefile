CC			= gcc
CFLAGS	= -Wall -g3 -std=c99
DISTCFLAGS = -Wnone -O3
LDFLAGS	= -lm -lpng

BUILD_DIR = build
EXE				= game

src	= $(wildcard src/**/*.c) \
			$(wildcard src/*.c)
lib = $(wildcard lib/*.c)
obj = $(src:.c=.o) \
			$(lib:.c=.o)

test_sources = $(wildcard test/test_*.c)
test_exes = $(test_sources:.c=.test)

#
# This one should be the first target. It will compile the game, but will take
# care of not compiling anything that was already compiled, thus saving time.
#
.PHONY: game
game: $(BUILD_DIR)/$(EXE)

# actually build the main game executable
$(BUILD_DIR)/$(EXE): main.o $(obj)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(EXE) $^ $(LDFLAGS)

# compiles all tests
.PHONY: test
test: $(test_exes)
	@echo "💁  💁‍♂️  Running the whole test suite... 🙏  🙏\n"
	@$(foreach test,$(test_exes), echo "Running $(test)..."; $(test); echo "";)

# compiles whichever test you tell it to
# TODO: fix this rule to only compile the test_*.o that have changed
test_%.test: $(obj) test/minitest.o test/*.c
	@echo "Compiling $(@)"
	$(CC) $(CFLAGS) -c -o $(@:.test=.o) $(@:.test=.c)
	$(CC) $(CFLAGS) -o $@ $(@:.test=.o) $(obj) test/minitest.o $(LDFLAGS)

# compiles the test library
test/minitest.o:
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

.PHONY: docker
docker:
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --rm -ti -v /Users/elias/u2/pprog/game:/var/projects cubuntu

.PHONY: clear
clear:
	@rm -rf **/**/*.o **/*.o *.o

.PHONY: clean
clean: clear
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(test_exes)
