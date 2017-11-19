#include <stdio.h>

#include "../lib/lineread.h"

#include "lib/error_handling.h"
#include "lib/messages.h"
#include "lib/cop.h"
#include "lib/terminal.h"
#include "lib/config.h"

#include "entities/world.h"

#define CONFIG_FILE "assets/config.txt"
#define CMD_FILE "assets/cmd.txt"

int do_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg("You executed %s\n", cmd);
    show_msg("%s\n", msg[0]);

    return !UINT_ERROR;
}

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg(msg[0], cmd);
    show_msg("\n");

    return !UINT_ERROR;
}

int main(void) {
    // init terminal (saving previous state)
    gc_terminal_setup(stdin);

    // load configuration dictionary
    load_config_from_file(CONFIG_FILE);

    // load assets
    World *w = world_new();

    // load cop
    // TODO: decide what to do about error handling
    FILE *cf = fopen(CMD_FILE, "r");
    CoP *c = cop_new(cf);
    fclose(cf);

    // associate our game commands with it
    cop_assoc(c, "build", do_build);
    cop_assoc(c, "error_cmd", cop_error_cmd);
    cop_set_error_cmd(c, "404_not_found");

    // GAME LOOP
    char input;
    char cmd[2];
    while (1) {
        if ((input = gc_read_key(stdin))) {
            if (input == 'q') {
                show_msg("\nExiting...\n");
                break;
            } else if (gc_is_arrow_key(input)) {
                w = world_move_cursor(w, input);
            } else {
                // turn the given command into a string
                sprintf(cmd, "%c", input);

                cop_exec(c, cmd, w);
            }
        }
    }
    // END GAME LOOP

    // free
    world_destroy(w);
    config_destroy();

    // restore terminal configuration
    gc_terminal_teardown(stdin);

    // exit
    return 0;
}
