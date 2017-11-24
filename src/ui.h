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


typedef struct _UIListItem UIListItem;

typedef struct _UIList UIList;

/*
 * The type of a function that takes a particular item in a list of objects and
 * returns the string that should be displayed when drawing the list.
 */
typedef char *(*ui_get_li_title_fun)(void *);

/*
 * Generates a new UIList from the generic list of objects s of length len_s.
 * Also receives a pointer to a function able to retrieve a string from each of
 * the objects making up s used for displaying in the list.
 */
UIList *ui_list_new(void **s, int s_len, ui_get_li_title_fun get_li_title);

/*
 * Destroys a UIList.
 */
void ui_list_destroy(UIList *l);

/*
 * Displays a UI list and waits for user to select input.
 *
 * Returns a pointer to the selected list item on success, NULL on error.
 */
UIListItem *ui_list_present(UIList *l);

/*
 * Returns the object inside a list item, NULL on error.
 */
void *ui_li_get_info(UIListItem *li);

#endif
