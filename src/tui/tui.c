#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/terminal.h"
#include "../lib/config.h"

#include <stdlib.h>
#include <stdio.h>


/*
 * The global UI structure.
 */
struct _UI {
    World *w;

    UIMap *map;
    UIWorldInfo *wi;
    UITileInfo *ti;
};

int ui_setup(World *w)
{
    if (!w) {
        HE("invalid paramenters, no world given", "ui_setup");
        return UINT_ERROR;
    }

    ui = oopsalloc(1, sizeof(UI), "ui_setup");

    ui->w = w;

    ui->map = ui_map_new(ui->w);
    if (!ui->map) {
        HE("could not load map", "ui_setup");
        return UINT_ERROR;
    }

    ui->wi = ui_world_info_new(ui->w, (UIRect) { 0 });
    ui->ti = ui_tile_info_new(ui->w, (UIRect) { 0 });

    // ui tile info is created when the cursor is first moved

    return !UINT_ERROR;
}

void ui_teardown()
{
    // this function does nothing at the moment as the UI is
    // statically allocated
    return;
}

UIMapVector _ui_keypress_to_vector(int input)
{
    switch (input) {
        case UP_ARROW:
            return UP;
        case LEFT_ARROW:
            return LEFT;
        case DOWN_ARROW:
            return DOWN;
        case RIGHT_ARROW:
            return RIGHT;
        default:
            return HERE;
    }
}

int ui_move_cursor(int input)
{
    return ui_map_move_cursor(ui->map, _ui_keypress_to_vector(input));
}

int ui_get_cursor()
{
    return ui_map_get_cursor(ui->map);
}

int ui_redraw_tile(int tile_index)
{
  return UINT_ERROR;
}

int ui_update_world_info()
{
  ui_world_info_draw(ui->wi);
  return !UINT_ERROR;
}

int ui_update_tile_info()
{
    ui_tile_info_draw(ui->ti, ui_map_get_cursor(ui->map));
    return !UINT_ERROR;
}

int ui_show_msg(char *msg)
{
    return printf("%s", msg);
}
