#include "../ui-list.h"

#include "../lib/error_handling.h"
#include "../lib/terminal.h"

typedef struct _UIListItem {
    void *info;
} UIListItem;

struct _UIList {
    UIListItem **list;
    int len;
    ui_get_li_string_fun get_li_title;
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

    return l;
}

void ui_list_destroy(UIList *l)
{
    if (!l) return;

    for (int i = 0; i < l->len; _ui_li_destroy(l->list[i++]));
    free(l->list);
    free(l);
}

void *ui_list_present(UIList *l)
{
    if (!l) {
        HE("invalid parameters", "ui_list_present");
        return NULL;
    }

    for (int i = 0; i < l->len; i++) {
        printf("[%2d]: %s\n", i + 1, l->get_li_title(l->list[i]->info));
    }


    int index = -1;
    do {
        printf("Choose from the list by typing a number or type 'q' to exit without choosing: ");
        char *buff = term_read_string(stdin, stdout);

        if (!buff) {
            HE("could not read from stdin", "ui_list_present");
            return NULL;
        }

        if (buff[0] == 'q' || buff[0] == 'Q') {
            printf("Selected nothing.\n");
            return NULL;
        }

        index = atoi(buff);
    } while (index <= 0 || index > l->len);

    return l->list[index - 1]->info;
}
