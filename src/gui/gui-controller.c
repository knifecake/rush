#include "../controller.h"

#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/messages.h"

#include "../entities/world.h"
#include "../entities/tile.h"

// TODO: add proper error handling and finish implementation
int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }
    //TODO: IMPLEMENT THIS
    // get current tile pointer

    //NOTE THIS LINE BELOW SHOULDN'T BE COMMENTED
    /*Tile *current_tile = world_get_current_tile(w);*/

    // check if the player can build to that tile
    /* if (!tile_can_build(current_tile)) { */
    /*     show_msg("You cannot build on that tile!\n"); */
    /* } */

    // get a list of available buildings for that tile
    //Building **bs = world_get_buildings(w);

    return !UINT_ERROR;
}

int action_welcome(void *world, char *cmd, char **msg, int num_msg)
{
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
