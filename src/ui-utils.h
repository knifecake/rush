#ifndef __UI_UTILS__
#define __UI_UTILS__

/*
 * Writes spaces filling and "drawing" the borders of the rectangle starting at
 * (x, y) of the specified dimensions.
 */
void ui_clear_rect(int x, int y, int width, int height);

/*
 * Clears the screen under the rectangle and draws an outline of plus signs
 * ('+') around the rectangle starting at (x, y) of dimensions width * height.
 */
void ui_draw_rect(int x, int y, int width, int height);

#endif
