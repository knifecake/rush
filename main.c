#include <stdio.h>

#include "lib/lineread.h"

#include "src/lib/error_handling.h"
#include "src/lib/messages.h"
#include "src/lib/cop.h"
#include "src/lib/terminal.h"
#include "src/lib/config.h"

#include "src/ui.h"
#include "src/controller.h"

#include "src/entities/world.h"

#define CONFIG_FILE "assets/config.txt"
#define CMD_FILE "assets/cmd.txt"


int main(void) {
    // init terminal (saving previous state)
    term_setup(stdin);

    // load configuration dictionary
    load_config_from_file(CONFIG_FILE);

    // load assets
    World *w = world_new();
    if (!w) {
        HE("FATAL: could not load some asset", "main");

        config_destroy();
        term_teardown(stdin);
        abort();
    }

    // setup UI
    ui_setup(w);

    // load cop
    // TODO: decide what to do about error handling
    FILE *cf = fopen(CMD_FILE, "r");
    CoP *c = cop_new(cf);
    fclose(cf);

    // associate our game commands with it
    cop_assoc(c, "build", action_generic);
    cop_assoc(c, "error_cmd", cop_error_cmd);
    cop_set_error_cmd(c, "404_not_found");

    // GAME LOOP
    char input;
    char cmd[2];
    while (1) {
        if ((input = term_read_key(stdin))) {
            if (input == 'q') {
                show_msg("\nExiting...\n");
                break;
            } else if (term_is_arrow_key(input)) {
                w = world_move_cursor(w, input);
            } else {
                // turn the given command into a string
                sprintf(cmd, "%c", input);

                cop_exec(c, cmd, w);
                show_msg("Executed command successfully\n");
                ui_update_world_info();
            }
        }
    }
    // END GAME LOOP

    // free
    ui_teardown();
    world_destroy(w);
    config_destroy();

    // restore terminal configuration
    term_teardown(stdin);

    // exit
    return 0;
}
