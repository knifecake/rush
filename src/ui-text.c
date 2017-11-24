#include "ui.h"

#include "lib/error_handling.h"

#include <stdlib.h>
#include <stdio.h>

struct _UIListItem {
    void *info;
};

struct _UIList {
    UIListItem **list;
    int len;
    ui_get_li_title_fun get_li_title;
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

UIList *ui_list_new(void **s, int s_len, ui_get_li_title_fun get_li_title)
{
    if (!s || s_len < 0 || !get_li_title) {
        HE("invalid parameters", "ui_list_new");
        return NULL;
    }

    UIList *l = calloc(1, sizeof(UIList));
    if (!l) {
        HE("could not allocate memory", "ui_list_new");
        return NULL;
    }

    l->list = calloc(s_len, sizeof(UIListItem *));
    if (!l->list) {
        HE("could not allocate memory for list", "ui_list_new");
        free(l);
        return NULL;
    }

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

// TODO: finish implementation
UIListItem *ui_list_present(UIList *l)
{
    if (!l) {
        HE("invalid parameters", "ui_list_present");
        return NULL;
    }

    printf("Should present a list");
    return l->list[0];
}

void *ui_li_get_info(UIListItem *li)
{
    if (!li) {
        HE("invalid parameters", "ui_li_get_info");
        return NULL;
    }

    return li->info;
}
