#include "../ui.h"
#include "../entities/building.h"
#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../lib/dict.h"
#include "../lib/sprite.h"
#include "../lib/terminal.h"
#include "../asset_loaders/sprite_repository.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*
 * The global UI structure.
 */
struct _UI {
    World *w;

    UIMap *map;
    UIWorldInfo *wi;
    UITileInfo *ti;
    UITextPanel *tp;
    Dict *sprite_dict;
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
    ui->wi = ui_world_info_new(ui->w);

    ui->sprite_dict = load_sprite_dict_from_file(config_get("sprite db"));
    if(!ui->sprite_dict){
      HE("Error creating the sprite dictionary", "ui_setup")
      return UINT_ERROR;
    }

    // ui tile info is created when the cursor is first moved
    ui_map_draw(ui->map);
    ui->tp = ui_text_panel_new(9, 149, 450, 75, config_get("font path"));
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

int ui_redraw_tile(int tile_index){
  /*TODO: Implement this*/
  return !UINT_ERROR;
}

int ui_update_world_info(){
  /*TODO: Implement this*/
  return !UINT_ERROR;
}

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info(){
  /*TODO: Implement this*/
  return !UINT_ERROR;
}

Dict *ui_get_sprite_dict() {
    if (ui)
        return ui->sprite_dict;

    return NULL;
}
