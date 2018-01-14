#include "../ui.h"
#include "../lib/error_handling.h"
#include "../lib/terminal.h"

typedef struct _UIListItem {
    void *info;
} UIListItem;

struct _UIList {
    UIListItem **list;
    int len;
    ui_get_li_string_fun get_li_title;
    ui_get_li_sprite_fun get_li_sprite;

    // component dimensions
    UIRect dim;
    int sprite_height, sprite_width;
    int items_per_screen;
    int min_displayed_index;

    int cursor;
    Sprite *cursor_on, *cursor_off;
};

UIListItem *_ui_li_new(void *info)
{
    if (!info) {
        HE("invalid params", "_ui_li_new");
        return NULL;
    }

    UIListItem *li = calloc(1, sizeof(UIListItem));
    if (!li) {
        HE("could not allocate memory for list item", "_ui_li_new");
        return NULL;
    }

    li->info = info;
    return li;
}

void _ui_li_destroy(UIListItem *li)
{
    if (!li) return;

    free(li);
}

UIList *ui_list_new(void **s, int s_len, UIRect dim,
        ui_get_li_string_fun get_li_title,
        ui_get_li_sprite_fun get_li_sprite)
{
    if (!s || s_len < 0 || !get_li_title) {
        HE("invalid parameters", "ui_list_new");
        return NULL;
    }

    UIList *l = oopsalloc(1, sizeof(UIList), "ui_list_new");

    l->list = oopsalloc(s_len, sizeof(UIListItem *), "ui_list_new");

    for (int i = 0; i < s_len; i++) {
        l->list[i] = _ui_li_new(s[i]);
        if (!l->list[i]) {
            HE("could not create list item", "ui_list_new");
            for (int j = 0; j < i; _ui_li_destroy(l->list[j++]));
            free(l->list); free(l);
            return NULL;
        }
    }

    l->len = s_len;

    l->get_li_title = get_li_title;
    l->get_li_sprite = get_li_sprite;

    l->dim = dim;

    l->items_per_screen = 8;

    l->sprite_height = l->sprite_width = 25;

    l->cursor = 0;


    // load cursor
    FILE *cf = fopen("assets/img/list-cursor.png", "r");
    FILE *df = fopen("assets/img/del-list-cursor.png", "r");
    if (!cf || !df) {
        HE("could not load cursor", "ui_list_new");
        return NULL;
    }

    l->cursor_on = sprite_new(cf);
    l->cursor_off = sprite_new(df);
    fclose(cf);
    fclose(df);

    return l;
}

void ui_list_destroy(UIList *l)
{
    if (!l) return;

    for (int i = 0; i < l->len; _ui_li_destroy(l->list[i++]));
    sprite_destroy(l->cursor_on);
    sprite_destroy(l->cursor_off);
    free(l->list);
    free(l);
}

void _ui_list_draw_cursor(UIList *l, int new_cursor)
{
    if (!l) {
        HE("invalid arguements", "_ui_list_draw_cursor");
        return;
    }

    int i = l->cursor % l->items_per_screen;
    int x = l->dim.x + (i % 2) * l->sprite_width;
    int y = l->dim.y + i / 2 * l->sprite_height;

    // delete previous cursor
    sprite_draw(stdout, l->cursor_off, x, y);

    i = new_cursor % l->items_per_screen;
    x = l->dim.x + (i % 2) * l->sprite_width;
    y = l->dim.y + i / 2 * l->sprite_height;

    // draw new cursor
    sprite_draw(stdout, l->cursor_on, x, y);

    // update cursor
    l->cursor = new_cursor;
}

void _ui_list_draw_sublist(UIList *l, int first_item)
{
    if (!l || first_item < 0 || first_item >= l->len) {
        HE("invalid arguments", "_ui_list_draw_sublist");
        return;
    }
    for (int i = first_item; i < l->len && i < first_item + l->items_per_screen; i++) {
        int j = i - first_item;
        int x = l->dim.x + (j % 2) * l->sprite_width;
        int y = l->dim.y + j / 2 * l->sprite_height;

        sprite_draw(stdout, l->get_li_sprite(l->list[i]->info), x, y);
    }

    l->min_displayed_index = first_item;
    _ui_list_draw_cursor(l, l->cursor);
}

void *ui_list_present(UIList *l)
{
    if (!l) {
        HE("invalid parameters", "ui_list_present");
        return NULL;
    }

    // clear the space we need
    ui_clear_rect(l->dim);

    l->cursor = 0;
    int key = HERE_ARROW;
    _ui_list_draw_sublist(l, l->cursor);
    while (key != '\n' && key != 'q')
    {
      fprintf(stderr, "%d in ui_list_present\n", l->cursor);
        // draw another fragment of the list if we overflow this one
        if (l->cursor < l->min_displayed_index || l->cursor >= l->min_displayed_index + l->items_per_screen){
              fprintf(stderr, "Entering \"if\" with a l->cursor value:%d\n",l->cursor);
              _ui_list_draw_sublist(l, l->cursor);
        }

        key = term_read_key(stdin);

        // move the cursor if necessary
        if (term_is_arrow_key(key)) {
            int new_cursor = l->cursor;
            switch (key) {
                case UP_ARROW:
                    if (l->cursor - 2 >= 0)
                        new_cursor = l->cursor - 2;
                    break;
                case RIGHT_ARROW:
                    if (l->cursor + 1 < l->len)
                        new_cursor = l->cursor + 1;
                    break;
                case DOWN_ARROW:
                    if (l->cursor + 2 < l->len)
                        new_cursor = l->cursor + 2;
                    break;
                case LEFT_ARROW:
                    if (l->cursor - 1 >= 0)
                        new_cursor = l->cursor - 1;
                    break;
            }


            // draw new cursor and updated description text as needed
            if (new_cursor != l->cursor) {
                _ui_list_draw_cursor(l, new_cursor);
                ui_show_msg(l->get_li_title(l->list[l->cursor]->info));
            }
        }
    }

    if (key == 'q') {
        // clear the screen space we occupied
        ui_clear_rect(l->dim);

        return NULL;
    }

    // clear the screen space we occupied
    ui_clear_rect(l->dim);

    return l->list[l->cursor]->info;
}
