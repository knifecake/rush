#include <stdio.h>
#include <termios.h>

#include "terminal.h"
#include "error_handling.h"

#define MAX_READ_STRING 10

/*
 * Stores the initial terminal configuration.
 */
static struct termios term_initial;

int term_read_key(FILE *s)
{
    if (!s) {
        HE("invalid arguments", "term_read_key");
        return 0;
    }

    char buff = fgetc(s);

    // handle arrow keys
    if (buff == 27 && fgetc(stdin) == '[') {
        buff = fgetc(stdin);

        switch(buff) {
            case('A'):
                return UP_ARROW;
            case('B'):
                return DOWN_ARROW;
            case('C'):
                return RIGHT_ARROW;
            case('D'):
                return LEFT_ARROW;
            default:
                return HERE;
        }
    }


    return buff;
}

char *term_read_string(FILE *s)
{
    if (!s) {
        HE("invalid arguments", "term_read_string");
        return NULL;
    }

    char *buff = oopsalloc(MAX_READ_STRING + 1, sizeof(char), "term_read_string");
    char key;
    int i = 0;

    // TODO: find a cleaner way to write this loop
    key = fgetc(s);
    while (key != '\0' && key != '\n') {
        // if it is a backspace and this is not the first character,
        // erase the last character and wait for the next one
        if (i > 0 && (key == 8 || key == 127)) {
            printf("\b \b");
            i--;
            key = fgetc(s);
            continue;
        }

        // only take into account printable characters
        if (key > '~' || key < ' ') {
            key = fgetc(s);
            continue;
        }


        // print the character for visual feedback
        printf("%c", key);

        buff[i++] = key;
        if (i >= MAX_READ_STRING) {
            handle_error("\nSorry, cannot read more letters.\n");
            return buff;
        }

        key = fgetc(s);
    }

    printf("\n");
    return buff;
}

bool term_is_arrow_key(int k)
{
    return (k == UP_ARROW || k == DOWN_ARROW || k == LEFT_ARROW || k == RIGHT_ARROW || k == HERE);
}

void term_setup(FILE *s)
{
    if (!s) {
        HE("invalid arguments", "term_terminal_teardown");
        return;
    }

    tcgetattr(fileno(stdin), &term_initial);

    // init terminal
    struct termios new;
    new = term_initial;

    // do not wait for enter to send key presses
    new.c_lflag &= ~ICANON;

    // do not automatically show typed characters
    new.c_lflag &= ~ECHO;

    // send characters one at a time, just when they are received
    new.c_cc[VMIN] = 1;

    // TODO: find out what this does
    new.c_cc[VTIME] = 0;

    // dicard signals (do not quit on Ctrl-c)
    new.c_lflag &= ~ISIG;

    // apply settings to the terminal
    tcsetattr(fileno(stdin), TCSANOW, &new);


}

void term_teardown(FILE *s)
{
    if (!s) {
        HE("invalid arguments", "term_terminal_teardown");
        return;
    }

    tcsetattr(fileno(stdin), TCSANOW, &term_initial);
}
