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

    UIFont *font;
    UIMap *map;
    UIWorldInfo *wi;
    UITileInfo *ti;
    UITextPanel *tp;
    Dict *sprite_dict;

    UIRect top_sidebar_dim;
};

int ui_setup(World *w)
{
    if (!w) {
        HE("invalid paramenters, no world given", "ui_setup");
        return UINT_ERROR;
    }

    ui = oopsalloc(1, sizeof(UI), "ui_setup");

    ui->top_sidebar_dim.x = 260;
    ui->top_sidebar_dim.y = 1;
    ui->top_sidebar_dim.width = 45;
    ui->top_sidebar_dim.height = 100;

    ui->w = w;

    ui->font = ui_font_new(config_get("font path"));
    ui->map = ui_map_new(ui->w);
    ui->wi = ui_world_info_new(ui->w);
    ui->tp = ui_text_panel_new((UIRect ) { 1, 150, 250, 30 }, ui->font);
    ui->ti = ui_tile_info_new(ui->w, ui->top_sidebar_dim);
    ui->sprite_dict = load_sprite_dict_from_file(config_get("sprite db"));
    if(!ui->sprite_dict){
      HE("Error creating the sprite dictionary", "ui_setup")
      return UINT_ERROR;
    }

    ui_map_draw(ui->map);
    ui_update_tile_info();
    return !UINT_ERROR;
}

void ui_teardown()
{
    ui_tile_info_destroy(ui->ti);
    ui_world_info_destroy(ui->wi);
    ui_map_destroy(ui->map);
    ui_text_panel_destroy(ui->tp);
    // TODO: free the sprite dict
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
    ui_map_move_cursor(ui->map, _ui_keypress_to_vector(input));
    return !UINT_ERROR;
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
int ui_update_tile_info() {
  ui_tile_info_draw(ui->ti, ui_get_cursor());
  return !UINT_ERROR; // TODO: this doesn't mean anything
}

void ui_redraw_sidebar() {
    ui_tile_info_draw(ui->ti, ui_get_cursor());
    /* ui_world_info_draw() */
}

Dict *ui_get_sprite_dict() {
    if (ui)
        return ui->sprite_dict;

    return NULL;
}

int ui_show_msg(char *msg) {
    if (!ui)
        return UINT_ERROR;

    return ui_text_panel_print(ui->tp, msg);
}

UIFont *ui_get_font()
{
    if (!ui)
        return NULL;

    return ui->font;
}

UIRect ui_get_top_sidebar_dim()
{
    if (!ui)
        return (UIRect) { 0 };
    return ui->top_sidebar_dim;
}
