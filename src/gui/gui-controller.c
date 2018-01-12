#include "../controller.h"

#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/messages.h"

#include "../entities/world.h"
#include "../entities/tile.h"

// TODO: this is a temporary function until building sprites are done
Sprite *get_placeholder_sprite(Building *b)
{
    FILE *f = fopen("assets/img/building-placeholder.png", "r");
    Sprite *placeholder_sprite = sprite_new(f);
    fclose(f);
    return placeholder_sprite;
}

//TODO:modify the sprintf so that it includes all the resources
char *building_get_desc(Building *b)
{
    if (!b) {
        HE("invalid arguments", "building_get_desc");
        return NULL;
    }

    char *buff = oopsalloc(100, sizeof(char), "building_get_desc");

    sprintf(buff, "%s, level %d ($%d)", building_get_name(b), building_get_level(b), building_get_cost(b, 0));
    return buff;
}

int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }
    // get the current tile index
    int tile_index = ui_get_cursor();

    // get a list of available buildings for that tile
    Building **bs = world_get_buildings(w);

    // assemble a list that is compatible with the UI-library
    UIList *ui_l = ui_list_new((void *)bs,
            world_get_num_buildings(w),
            ui_get_top_sidebar_dim(),
            (ui_get_li_string_fun)building_get_desc,
            (ui_get_li_sprite_fun)get_placeholder_sprite);

    show_msg("What do you want to build this tile?\nUse the arrow keys to select a building on the top right.");

    // display the list: passes control to the uilist, will return a pointer to the list item that was chosen
    Building *b = ui_list_present(ui_l);

    if (!b) {
        show_msg("Okay, nothing will be built\n\n");
        ui_redraw_sidebar();
        return CTRL_OK;
    }

    // update the model (entity) to reflect the changes that took place
    int result = world_build_on_tile(w, tile_index, b);

    // communicate the result to the user and update neighbours
    switch(result) {
        case WORLD_BUILD_SUCCESS_LEVEL_UP:
            show_msg("Building was constructed and you leveled up!");
            world_update_neighbours(w, tile_index);
            break;
        case WORLD_BUILD_SUCCESS:
            show_msg("Building was constructed!");
            world_update_neighbours(w, tile_index);
            break;
        case WORLD_BUILD_NO_LEVEL:
            show_msg("You need to level up before building this. Hint: upgrade townhall.");
            break;
        case WORLD_BUILD_NO_MONEY:
            show_msg("You need to save more to build this");
            break;
        case WORLD_BUILD_OCCUPIED:
            show_msg("There already is a building on that tile. You may only upgrade it to a higher level.");
            break;
        case WORLD_BUILD_SUCCESS_UPGRADE:
            show_msg("The building on this tile was upgraded to the next level.");
            break;
        case WORLD_BUILD_NO_LIGHT:
            show_msg("The tile is not visible.");
            break;
        default:
            show_msg("An error ocurred and nothing was built");
            ui_redraw_sidebar();
            return CTRL_ERROR;
    }

    // redraw the current tile. TODO: think about who should do this
    ui_redraw_tile(tile_index);
    switch (result) {
      case WORLD_BUILD_SUCCESS_LEVEL_UP:
      case WORLD_BUILD_SUCCESS:
        ui_redraw_neighbours(tile_index);
        break;
      default:
        break;
    }

    // TODO: think about if signaling a common UI redraw from the return value is a good idea
    ui_redraw_sidebar();
    return CTRL_NEXT_TURN;
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

    show_msg("You executed %s.\n%s\n", cmd, msg[0]);

    return !UINT_ERROR;
}

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg(msg[0], cmd);

    return !UINT_ERROR;
}
