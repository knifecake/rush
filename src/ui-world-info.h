#ifndef __UI_WORLD_INFO__
#define __UI_WORLD_INFO__

#include "entities/world.h"
#include "ui.h"

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */
typedef struct _UIWorldInfo UIWorldInfo;

/*
 * Creates a new world info panel UI component.
 */
UIWorldInfo *ui_world_info_new(World *w, UIRect dim);

/*
 * Draws the given world info panel.).
 *
 * Before drawing all the values displayed are updated by calling getters on
 * the World entity.
 */
void ui_world_info_draw(UIWorldInfo *wi);

/*
 * Refreshes just one line of the world info panel.
 * Lines indexes as follows:
 * Index Field
 * 0     Level
 * 1     1st Resource
 * 2     2st Resource
 * 3     3st Resource
 * 4     4st Resource
 * 5     5st Resource
 * 6     6st Resource
 * 7     7st Resource
 */
void ui_world_info_print_single_line (UIWorldInfo *wi, int index);

/*
 * Frees all the memory associated with a UIWorldInfo component that was
 * allocated by ui_world_info_new.
 */
void ui_world_info_destroy(UIWorldInfo *wi);

#endif
