#ifndef __UI_CODE_LAB_PANEL__
#define __UI_CODE_LAB_PANEL__

#include "entities/world.h"

/*
 * UICodePanel
 *
 * A Panel to code resources.
 */
typedef struct _UICodePanel UICodePanel;

/*
 * Creates a new panel to display different operations.
 */
UICodePanel *ui_code_panel_new(World *w);

/*
 * Controls the exchange panel, returns the amount of resource spent. Res id would be the resource won (4 - code, [-1,-4] skills).
 */
int ui_code_panel_control(UICodePanel *cp,int *res_from, int *res_id);

/*
 * Frees all memory allocated by ui_code_panel_new.
 */
void ui_code_panel_destroy(UICodePanel *cp);

#endif
