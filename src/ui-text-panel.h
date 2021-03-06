#ifndef __UI_TEXT_PANEL__
#define __UI_TEXT_PANEL__

#include "ui-font.h"
#include "lib/terminal.h"

/*
 * UITextPanel
 *
 * A panel to display text.
 */

typedef struct _UITextPanel UITextPanel;

/*
 * Instantiates a new UITextPanel of width x height pixels starting at (x, y).
 * font_path is the name of the directory where sprites of all printable ascii
 * characters can be found. The names of these sprites should be the ascii
 * codes for each character.
 */
UITextPanel *ui_text_panel_new(UIRect outer_dim, UIFont *font);

/*
 * Frees all resources associated with a given UITextPanel.
 */
void ui_text_panel_destroy(UITextPanel *tp);

/*
 * Draw the text panel. If some text was drawn and it wasn't cleared, it draws
 * it again.
 */
int ui_text_panel_draw(UITextPanel *tp);

/*
 * Prints a string of text in the UI text panel.
 *
 * Returns UINT_ERROR on error.
 */
int ui_text_panel_print(UITextPanel *tp, char *msg);

/*
 * Prints a dialoque and returns true or false depending on the yes/no answer
 * provided by the user.
 */
bool ui_text_panel_dialogue(UITextPanel *tp, char *question);

/*
 * Clears any text written in the given UIText panel.
 */
int ui_text_panel_clear(UITextPanel *tp);
#endif
