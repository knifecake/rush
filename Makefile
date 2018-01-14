CC			= gcc
CFLAGS	= -Wall -g3 -std=c99
LDFLAGS	= -lm -lpng -lpthread

BUILD_DIR = build

src	= $(wildcard src/entities/*.c) \
			$(wildcard src/lib/*.c) \
			$(wildcard src/asset_loaders/*.c)
lib = $(wildcard lib/*.c)
obj = $(src:.c=.o) \
			$(lib:.c=.o)

gui_src = $(wildcard src/gui/*.c)
gui_obj = $(gui_src:.c=.o)
tui_src = $(wildcard src/tui/*.c)
tui_obj = $(tui_src:.c=.o)

test_sources = $(wildcard test/test_*.c)
test_exes = $(test_sources:.c=.test)

#
# This one should be the first target. It will compile the game, but will take
# care of not compiling anything that was already compiled, thus saving time.
#
.PHONY: game
game: $(BUILD_DIR)/tui $(BUILD_DIR)/gui $(BUILD_DIR)/minigame

$(BUILD_DIR)/tui: tui.o $(tui_obj) $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/gui: gui.o $(gui_obj) $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/minigame: minigame.o $(gui_obj) $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# compiles all tests
.PHONY: test
test: $(test_exes)
	@echo "💁  💁‍♂️  Running the whole test suite... 🙏  🙏\n"
	@$(foreach test,$(test_exes), echo "Running $(test)..."; $(test); echo "";)

# compiles whichever test you tell it to
# TODO: fix this rule to only compile the test_*.o that have changed
test_%.test: $(obj) $(tui_obj) test/minitest.o test/*.c
	@echo "Compiling $(@)"
	$(CC) $(CFLAGS) -c -o $(@:.test=.o) $(@:.test=.c)
	$(CC) $(CFLAGS) -o $@ $(@:.test=.o) $(obj) $(tui_obj) test/minitest.o $(LDFLAGS)

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
