#include "../ui-world-info.h"
#include "../lib/error_handling.h"

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */

struct _UIWorldInfo {
    World *w;

    UIRect dim;

    UITextPanel *tp;
};

UIWorldInfo *ui_world_info_new(World *w, UIRect dim)
{
    if (!w) {
        HE("invalid arguments", "ui_world_info_new");
        return NULL;
    }

    UIWorldInfo *wi = oopsalloc(1, sizeof(UIWorldInfo), "ui_world_info_new");

    wi->dim = dim;
    wi->w = w;

    wi->tp = ui_text_panel_new(wi->dim, ui_get_font());

    return wi;
}

void ui_world_info_draw(UIWorldInfo *wi)
{
    if (!wi) {
        HE("invalid params", "ui_world_info_draw");
        return;
    }

    Resource **res = world_get_resources(wi->w);
    if (!res) {
        HE("cannot print get resources from world", "ui_world_info_draw");
        return;
    }

    char *info = oopsalloc(MAX_RESOURCES * 10 + 100, sizeof(char), "ui_world_info_draw");
    sprintf(info, "level %d\n", world_get_player_level(wi->w));
    for (int i = 0; i < world_get_num_resources(wi->w); i++) {
        sprintf(info, "%s%d: %d\n", info, i, world_get_resource_quantity(wi->w, i));
    }

    ui_text_panel_print(wi->tp, info);
    free(info);
}

void ui_world_info_destroy(UIWorldInfo *wi)
{
    // TODO
    return;
}
