#include "../ui-world-info.h"

#include "../lib/error_handling.h"

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */

struct _UIWorldInfo {
    World *w;
};

UIWorldInfo *ui_world_info_new(World *w, UIRect dim)
{
    if (!w) {
        HE("invalid parameters", "ui_world_info_new");
        return NULL;
    }

    UIWorldInfo *wi = oopsalloc(1, sizeof(UIWorldInfo), "ui_world_info_new");
    wi->w = w;
    return wi;
}

void ui_world_info_draw(UIWorldInfo *wi)
{
    if (!wi) {
        HE("invalid params", "ui_world_info_draw");
        return;
    }

    printf("You have the following resources:\n");
    Resource **res = world_get_resources(wi->w);
    if (!res) {
        HE("cannot print get resources from world", "ui_world_info_draw");
        return;
    }

    for (int i = 0; i < world_get_num_resources(wi->w); i++) {
        printf("%s: %d\n", resource_get_name(res[i]), world_get_resource_quantity(wi->w, i));
    }

    printf("\n");
}

void ui_world_info_destroy(UIWorldInfo *wi)
{
    free(wi);
    return;
}
