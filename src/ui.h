#ifndef __UI_H__
#define __UI_H__

/*
 * UI Module
 *
 * This module contains helper functions to draw different things on screen, if
 * on GUI-mode, or represent them through text, if on text mode.
 *
 * Functions on this module should be game-engine agnostic. Preferrably, they
 * should only depend on the sprite, terminal and font libraries.
 *
 * Each of the functions defined here should take care of drawing just a tiny
 * part of the UI, and possible receive some user input in case the UI
 * component is interactive, such as with a list.
 */
#include "lib/sprite.h"
#include "lib/dict.h"

#include "entities/world.h"
#include "entities/tile.h"

/*
 * The global UI structure.
 */
typedef struct _UI UI;

UI *ui;

/*
 * GENERAL UI FUNCTIONS
 *
 * The following functions are called once to prepare the UI for drawing.
 */

/*
 * Creates the global UI structure (hidden in ui-something.c) and configures it
 * to have the common UI components defined below.
 */
int ui_setup(World *w);

/*
 * Frees all memory allocated by ui_setup.
 */
void ui_teardown();

/*
 * The following functions are wrappers arround more specific functions defined
 * below for general UI compontents. They are the same but do not receive UI
 * components as arguments. Instead, they use the ones stored on the global UI
 * structure and pass them on the the component-specific update functions
 * below.
 */


int ui_move_cursor(int input);

int ui_get_cursor();

int ui_redraw_tile(int tile_index);

int ui_update_world_info();

int ui_update_tile_info();

// TODO: esto cada vez me da más asco
Dict *ui_get_sprite_dict();

/*
 * Displays a message where it's appropriate.
 */
int ui_show_msg(char *msg);
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*
 * COMMON AND UNCHANGING UI COMPONENTS:
 *
 * These can be thightly coupled to the Entities. On UI set these are
 * instantiated and integrated into the UI. Then the controller can triger
 * updates on specific parts of the UI or the whole. These are never
 * interactive (cursor movement in UIMap is handled separately) and will not
 * block the main game loop to listen for input.
 */

#include "ui-world-info.h"

#include "ui-tile-info.h"

#include "ui-map.h"

#include "ui-text-panel.h"

/*
 * GENERIC UI COMPONENTS:
 *
 * These are not coupled with the model, instead they receive function pointers
 * to access the data they need to display. These are more reusable but cannot
 * be customized as much, as they need to be adaptable for many scenarios.
 * Also, these sometimes are interactive, meaning that when activated they can
 * block the main game loop and listen for player input.
 */

#include "ui-list.h"

#include "ui-utils.h"

#endif
