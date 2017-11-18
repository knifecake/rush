#include <stdio.h>
#include <termios.h>

#include "../lib/lineread.h"

#include "lib/error_handling.h"
#include "lib/messages.h"
#include "lib/cop.h"

#include "entities/world.h"

#define CMD_FILE "assets/cmd.txt"

int do_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg("You executed %s\n", cmd);
    show_msg(msg[0]);

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
    // save term state
    struct termios initial;
    tcgetattr(fileno(stdin), &initial);


    // init terminal
    struct termios new;
    new = initial;

    // do not wait for enter to send key presses
    new.c_lflag &= ~ICANON;

    // do not automatically show typed characters
    new.c_lflag &= ~ECHO;

    // send characters one at a time, just when they are received
    new.c_cc[VMIN] = 1;

    // TODO: find out what this does
    new.c_cc[VTIME] = 0;

    // dicard signals (do not quit on Ctrl-c)
    /* new.c_lflag &= ~ISIG; */

    // apply settings to the terminal
    tcsetattr(fileno(stdin), TCSANOW, &new);

    // load assets
    World *w = world_new();

    // load cop
    // TODO: decide what to do about error handling
    FILE *cf = fopen(CMD_FILE, "r");
    CoP *c = cop_new(cf);
    fclose(cf);

    cop_assoc(c, "int_test", do_build);
    cop_assoc(c, "error_cmd", cop_error_cmd);
    cop_set_error_cmd(c, "404_not_found");

    // GAME LOOP
    char input;
    char cmd[2];
    while (1) {
        if ((input = fgetc(stdin))) {
            show_msg("You pressed %c\n", input);

            if (input == 'q') {
                show_msg("\nExiting...\n");
                break;
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

    // restore terminal configuration
    tcsetattr(fileno(stdin), TCSANOW, &initial);

    // exit
    return 0;
}
