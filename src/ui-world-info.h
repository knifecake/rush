#ifndef __UI_WORLD_INFO__
#define __UI_WORLD_INFO__

#include "entities/world.h"

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

#endif
