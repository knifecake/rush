#include <stdio.h>
#include <termios.h>

#include "terminal.h"
#include "error_handling.h"
#include "sprite.h"

#define MAX_READ_STRING 10
#define _POSIX_C_SOURCE 1

/*
 * Stores the initial terminal configuration.
 */
static struct termios term_initial;

int term_read_key(FILE *in)
{
    if (!in) {
        HE("invalid arguments", "term_read_key");
        return 0;
    }

    char buff = fgetc(in);

    // handle arrow keys
    if (buff == 27 && fgetc(in) == '[') {
        buff = fgetc(in);

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
                return HERE_ARROW;
        }
    }


    return buff;
}

char *term_read_string(FILE *in, FILE *out)
{
    if (!in) {
        HE("invalid arguments", "term_read_string");
        return NULL;
    }

    char *buff = oopsalloc(MAX_READ_STRING + 1, sizeof(char), "term_read_string");
    char key;
    int i = 0;

    // TODO: find a cleaner way to write this loop
    key = fgetc(in);
    while (key != '\0' && key != '\n') {
        // if it is a backspace and this is not the first character,
        // erase the last character and wait for the next one
        if (i > 0 && (key == 8 || key == 127)) {
            fprintf(out, "\b \b");
            i--;
            key = fgetc(in);
            continue;
        }

        // only take into account printable characters
        if (key > '~' || key < ' ') {
            key = fgetc(in);
            continue;
        }


        // print the character for visual feedback
        fprintf(out, "%c", key);

        buff[i++] = key;
        if (i >= MAX_READ_STRING) {
            handle_error("\nSorry, cannot read more letters.\n");
            return buff;
        }

        key = fgetc(in);
    }

    fprintf(out, "\n");
    return buff;
}

bool term_is_arrow_key(int k)
{
    return (k == UP_ARROW || k == DOWN_ARROW || k == LEFT_ARROW || k == RIGHT_ARROW || k == HERE_ARROW);
}

void term_resize_hint(FILE *in, FILE *out, int height, int width)
{
    if (!in || !out)
    {
        HE("invalid arguments", "term_resize_hint");
        return;
    }

    // print instructions
    fprintf(out, "This game needs the screen to be of a certain size.\n"
            "Please, do the following:\n"
            "\t1. Enter fullscreen if your terminal allows it\n"
            "\t2. Reduce the font size until you can see a square on the screen\n"
            "\t3. Press any key when you are able to see the square to begin playing\n"
            "Ready? (press any key to continue)");

    // wait for user confirmation
    term_read_key(in);

    // erase screen and move to the top left corner
    /*fprintf(out, "\033[2J\033[1;1H");*/
/*
    // draw a square
    for (int i = 0; i < width; i++)
        fprintf(out, "+");

    for (int i = 0; i < height - 1; i++) {
        fprintf(out, "\n+");
        for (int j = 0; j < width - 1; j++)
            fprintf(out, " ");
        fprintf(out, "+");
    }

    fprintf(out, "\n");

    for (int i = 0; i < width; i++)
        fprintf(out, "+");*/
    FILE* img = fopen("assets/img/welcome.png", "r");
    Sprite *s = sprite_new(img);
    sprite_draw(out, s, 0, 70);
    sprite_destroy(s);
    fclose(img);
    // wait for user confirmation
    term_read_key(in);

    // clear and move to the top left
    fprintf(out, "\033[2J\033[1;1H");
}

void term_setup(FILE *in, FILE *out)
{
    if (!in || !out) {
        HE("invalid arguments", "term_terminal_teardown");
        return;
    }

    tcgetattr(fileno(in), &term_initial);

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
    tcsetattr(fileno(in), TCSANOW, &new);


}

void term_teardown(FILE *in, FILE *out)
{
    if (!in || !out) {
        HE("invalid arguments", "term_terminal_teardown");
        return;
    }

    // clear the screen
    fprintf(out, "\033[2J\033[0;0H");

    tcsetattr(fileno(in), TCSANOW, &term_initial);
}
