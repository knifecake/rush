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


int main(int argc, char **argv) {
    // init terminal (saving previous state)
    term_setup(stdin, stdout);

    // load configuration dictionary
    load_config_from_file(CONFIG_FILE);

    // load assets
    World *w = world_new();
    if (!w) {
        term_teardown(stdin, stdout);
        HE("FATAL: could not load some asset", "main");

        config_destroy();
        abort();
    }

    // TODO: this is a small hack, do this properly with getopts
    if (argc != 2) {
        // instruct user to resize the screen
        term_resize_hint(stdin, stdout, config_get_int("screen_height"), config_get_int("screen_width"));
    }



    // setup UI
    ui_setup(w);

    // load cop
    // TODO: decide what to do about error handling
    FILE *cf = fopen(CMD_FILE, "r");
    CoP *c = cop_new(cf);
    fclose(cf);

    // associate our game commands with it
    cop_assoc(c, "build", action_build);
    cop_assoc(c, "upgrade", action_upgrade);
    cop_assoc(c, "exchange", action_exchange);
    cop_assoc(c, "attack", action_attack);
    cop_assoc(c, "redraw_ui", action_redraw_ui);
    cop_assoc(c, "next_turn", action_next_turn);
    cop_assoc(c, "welcome", action_welcome);
    cop_assoc(c, "error_cmd", cop_error_cmd);
    cop_set_error_cmd(c, "404_not_found");

    if (argc != 2) {
        // say hello to the user
        cop_exec(c, "welcome", w);
    }

    // GAME LOOP
    char input;
    char cmd[2];
    while (1) {
        if ((input = term_read_key(stdin))) {
            if (input == 'q') {
                ui_show_msg("\nExiting...\n");
                break;
            }
            // check if we're moving the cursor
            else if (term_is_arrow_key(input)) {
                if (UINT_ERROR == ui_move_cursor(input)) {
                    ui_show_msg("cannot move further in that direction\n");
                }
                ui_update_tile_info();
            }
            else if (' ' <= input && input <= '~'){
                // turn the given command into a string
                sprintf(cmd, "%c", input);

                int ret = cop_exec(c, cmd, w);
                switch (ret) {
                    case CTRL_NEXT_TURN:
                        action_next_turn(w, NULL, NULL, 0);
                        break;
                    case CTRL_REDRAW_ALL_UI:
                        ui_draw_all();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    // END GAME LOOP

    // free
    ui_teardown();
    world_destroy(w);
    cop_destroy(c);
    config_destroy();

    // restore terminal configuration
    term_teardown(stdin, stdout);

    // exit
    return 0;
}
