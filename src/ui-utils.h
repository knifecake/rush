#ifndef __UI_UTILS__
#define __UI_UTILS__

/*
 * A generic structure for expressing rectangular dimensions.
 */
typedef struct {
    int x, y;
    int width, height;
} UIRect;


/*
 * Writes spaces filling and "drawing" the borders of the rectangle starting at
 * (x, y) of the specified dimensions.
 */
void ui_clear_rect(UIRect r);

/*
 * Clears the screen section delimited by r and draws a border surrounding it
 * with the given character.
 */
void ui_draw_rect(UIRect r, char border);

#endif
