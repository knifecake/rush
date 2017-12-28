#include "../controller.h"

#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/messages.h"

#include "../entities/world.h"
#include "../entities/tile.h"

char *building_get_desc(Building *b)
{
    if (!b) {
        HE("invalid arguments", "building_get_desc");
        return NULL;
    }

    char *buff = oopsalloc(100, sizeof(char), "building_get_desc");

    sprintf(buff, "%s, level %d ($%d)", building_get_name(b), building_get_level(b), building_get_cost(b));
    return buff;
}

int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return CTRL_ERROR;
    }

    // get the current tile index
    int tile_index = ui_get_cursor();

    // get a list of available buildings for that tile
    Building **bs = world_get_buildings(w);

    // assemble a list that is compatible with the UI-library
    UIList *ui_l = ui_list_new((void *)bs, world_get_num_buildings(w), (ui_get_li_string_fun)building_get_desc, NULL);

    show_msg("\nWhat do you want to build this tile?\n");
    // display the list: passes control to the ui, will return a pointer to the list item that was chosen
    Building *b = ui_list_present(ui_l);

    if (!b) {
        show_msg("Okay, nothing will be built\n\nn");
        return CTRL_OK;
    }

    // update the model (entity) to reflect the changes that took place
    int result = world_build_on_tile(w, tile_index, b);

    // communicate the result to the user
    switch(result) {
        case WORLD_BUILD_SUCCESS_LEVEL_UP:
            show_msg("Building was constructed and you leveled up!");
            break;
        case WORLD_BUILD_SUCCESS:
            show_msg("Building was constructed!");
            break;
        case WORLD_BUILD_NO_LEVEL:
            show_msg("You need to level up before building this. Hint: upgrade townhall.");
            break;
        case WORLD_BUILD_NO_MONEY:
            show_msg("You need to save more to build this");
            break;
        default:
            show_msg("An error ocurred and nothing was built");
            return CTRL_ERROR;
    }

    // redraw the current tile. TODO: think about who should do this
    // ui_map_draw_tile(current_tile);

    printf("\n");
    // TODO: think about if signaling a common UI redraw from the return value is a good idea
    return CTRL_OK;
}

int action_welcome(void *world, char *cmd, char **msg, int num_msg)
{
    show_msg("      __        __   _ _       \n");
    show_msg("      \\ \\      / /__| | |      \n");
    show_msg("       \\ \\ /\\ / / _ \\ | |      \n");
    show_msg("        \\ V  V /  __/ | |_ _ _ \n");
    show_msg("         \\_/\\_/ \\___|_|_(_|_|_)\n");

    show_msg("A game about... well... maybe tomorrow...\n\n");

    show_msg("Instructions:\n");
    show_msg("\tUse the arrow keys to move trought the map.\n");
    show_msg("\tPress 'n' to move onto the next turn\n");
    show_msg("\n");
    return CTRL_OK;
}

int action_next_turn(void *world, char *cmd, char **msg, int num_msg)
{
    return CTRL_NEXT_TURN;
}

int action_generic(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return CTRL_ERROR;
    }

    show_msg("You executed %s\n", cmd);
    show_msg("%s\n", msg[0]);

    return CTRL_OK;
}

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return CTRL_ERROR;
    }

    show_msg(msg[0], cmd);
    show_msg("\n\n");

    return CTRL_OK;
}
