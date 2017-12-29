#ifndef __UI_LIST__
#define __UI_LIST__

#include "lib/sprite.h"
#include "ui.h"

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
 * UIList
 *
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
UIList *ui_list_new(void **s, int s_len, UIRect dim,
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
