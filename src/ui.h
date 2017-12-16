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

#include "entities/world.h"
#include "entities/tile.h"

/*
 * These function types are designed to bind UI strings to information on
 * Entities. Instead of directly touching the entity, reusable UI components
 * can receive these as parameters and use them to get the values they need to
 * draw.
 */
typedef char *(*ui_get_li_string_fun)(void *);

typedef int (*ui_get_li_int_fun)(void *);

typedef Sprite *(*ui_get_li_sprite_fun)(void *);


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

int ui_update_cursor();

int ui_redraw_tile(int tile_index);

int ui_update_world_info();

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info();

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

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */
typedef struct _UIWorldInfo UIWorldInfo;

/*
 * Creates a new world info panel UI component.
 */
UIWorldInfo *ui_world_info_new(World *w);

/*
 * Draws a new world info panel starting from (x,y).
 *
 * Before drawing all the values displayed are updated by calling getters on
 * the World entity.
 */
void ui_world_info_draw(UIWorldInfo *wi, int x, int y);

/*
 * Frees all the memory associated with a UIWorldInfo component that was
 * allocated by ui_world_info_new.
 */
void ui_world_info_destroy(UIWorldInfo *wi);

// ----------------------------------------------------------------------------

/*
 * UITileInfo panel
 *
 * A tile info panel intended to be displayed on the right of the sidebar.
 *
 * Notice: this component's new/destroy functions need to be lightweight as
 * they will be called every time the cursor is moved. Alternative: design this
 * component to be reusable and add functionality to update tiles.
 */
typedef struct _UITileInfo UITileInfo;

/*
 * Creates a new tile info panel UI component.
 */
UITileInfo *ui_tile_info_new(World *w);

/*
 * Draws a new tile info panel starting from (x,y).
 *
 * Before drawing all the values displayed are updated by calling getters on
 * the World entity.
 */
void ui_tile_info_draw(UITileInfo *ti, int x, int y);

/*
 * Frees all the memory associated with a UITileInfo component that was
 * allocated by ui_tile_info_new.
 */
void ui_tile_info_destroy(UITileInfo *ti);

// ----------------------------------------------------------------------------

/*
 * UIMap
 *
 * A Map view displayed on the center of the screen.
 */
typedef struct _UIMap UIMap;

/*
 * Creates a new map component bound the the given world. Maybe additional
 * getters are needed on World to be able to access the internal world map
 * structure.
 */
UIMap *ui_map_new(World *w);

/*
 * Deletes the previous cursor and redraws the new one. Needs to keep track of
 * where the previous cursor was and read where the cursor is now by calling
 * World getters.
 */
void ui_map_update_cursor(UIMap *m);

/*
 * Redraws a spectific Tile (if visible) on the map. This function gets called,
 * for instance, when a building is constructed in one of the tiles.
 */
void ui_map_redraw_tile(UIMap *m, int tile_index);

/*
 * Frees all memory allocated by ui_map_new.
 */
void ui_map_destroy(UIMap *m);

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

/*
 * GENERIC UI COMPONENTS:
 *
 * These are not coupled with the model, instead they receive function pointers
 * to access the data they need to display. These are more reusable but cannot
 * be customized as much, as they need to be adaptable for many scenarios.
 * Also, these sometimes are interactive, meaning that when activated they can
 * block the main game loop and listen for player input.
 */

/*
 * A generic list, created from a list of objects. When presented to the user
 * takes controll of the input and allows the player to select a list item.
 * Should include special items for an empty selection / exiting.
 */
typedef struct _UIList UIList;

/*
 * Generates a new UIList from the generic list of objects s of length len_s.
 * Also receives a pointer to a function able to retrieve a string from each of
 * the objects making up s used for displaying in the list.
 */
UIList *ui_list_new(void **s, int s_len,
        ui_get_li_string_fun get_li_title,
        ui_get_li_sprite_fun get_li_sprite);

/*
 * Destroys a UIList.
 */
void ui_list_destroy(UIList *l);

/*
 * Displays a UI list and waits for user to select input.
 *
 * Returns a pointer to the selected list item on success, NULL on error.
 */
void *ui_list_present(UIList *l);

#endif
