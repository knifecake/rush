#include "controller.h"

#include "ui.h"

#include "lib/error_handling.h"
#include "lib/messages.h"

#include "entities/world.h"
#include "entities/tile.h"

// TODO: add proper error handling and finish implementation
int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }

    // get current tile pointer
    Tile *current_tile = world_get_current_tile(w);

    // check if the player can build to that tile
    /* if (!tile_can_build(current_tile)) { */
    /*     show_msg("You cannot build on that tile!\n"); */
    /* } */

    // get a list of available buildings for that tile
    Building **bs = world_get_buildings(w);

    // assemble a list that is compatible with the UI-library
    UIList *ui_l = ui_list_new((void *)bs, world_get_num_buildings(w), (ui_get_li_string_fun)building_get_name, NULL);

    show_msg("\nWhat do you want to build on tile %d?\n", tile_get_id(current_tile));
    // display the list: passes control to the ui, will return a pointer to the list item that was chosen
    Building *b = ui_list_present(ui_l);

    if (!b) {
        show_msg("Okay, nothing will be built\n");
        return !UINT_ERROR;
    }

    // update the model (entity) to reflect the changes that took place
    if (UINT_ERROR == tile_build(current_tile, b)) {
        HE("could not build on this tile", "action_build");
        return UINT_ERROR;
    }

    show_msg("Your building was constructed!\n");

    // redraw the current tile. TODO: think about who should do this
    // ui_map_draw_tile(current_tile);

    printf("\n");

    // TODO: think about if signaling a common UI redraw from the return value is a good idea
    return !UINT_ERROR;
}

int action_generic(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg("You executed %s\n", cmd);
    show_msg("%s\n", msg[0]);

    return !UINT_ERROR;
}

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg(msg[0], cmd);
    show_msg("\n\n");

    return !UINT_ERROR;
}
