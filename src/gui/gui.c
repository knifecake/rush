/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


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
    UIBuildPanel *bp;
    UIExchangePanel *ep;
    UICodePanel *cp;
    UITextPanel *tp;
    Dict *sprite_dict;

    UIRect top_sidebar_dim;
    UIRect bottom_sidebar_dim;
    UIRect text_panel_dim;
};
void ui_presetup(){
      ui = oopsalloc(1, sizeof(UI), "ui_setup");

      ui->top_sidebar_dim         = (UIRect) { .x = 260,  .y = 12,     .w = 60,    .h = 95 };
      ui->bottom_sidebar_dim      = (UIRect) { .x = 260,  .y = 132,   .w = 60,    .h = 70 };
      ui->text_panel_dim          = (UIRect) { .x = 1,    .y = 153,   .w = 250,   .h = 27 };

      ui->font = ui_font_new(config_get("general.font_path"));
      ui->tp = ui_text_panel_new(ui->text_panel_dim, ui->font);
      ui->sprite_dict = load_sprite_dict_from_file(config_get("asset_dbs.sprites"));
}
int ui_setup(World *w)
{
    if (!w) {
        HE("invalid paramenters, no world given", "ui_setup");
        return UINT_ERROR;
    }

    ui->w = w;
    ui->map = ui_map_new(ui->w);
    ui->wi = ui_world_info_new(ui->w, ui->top_sidebar_dim);
    ui->ti = ui_tile_info_new(ui->w, ui->bottom_sidebar_dim);
    ui->bp = ui_build_panel_new(world_get_buildings(ui->w));
    ui->ep = ui_exchange_panel_new(ui->w);
    ui->cp = ui_code_panel_new(ui->w);

    if (!ui->font || !ui->map || !ui->tp || !ui->ti || !ui->wi || !ui->sprite_dict) {
        HE("Could not initialize UI", "ui_setup");
        return UINT_ERROR;
    }

    return !UINT_ERROR;
}

void ui_draw_all()
{
    printf("\033[2J");
    ui_map_draw(ui->map);
    ui_draw_interface();
    ui_text_panel_draw(ui->tp);
    ui_update_tile_info();
    ui_update_world_info();
}

void ui_teardown()
{
    ui_tile_info_destroy(ui->ti);
    ui_world_info_destroy(ui->wi);
    ui_map_destroy(ui->map);
    ui_text_panel_destroy(ui->tp);
    ui_build_panel_destroy(ui->bp);
    ui_exchange_panel_destroy(ui->ep);
    ui_code_panel_destroy(ui->cp);
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

int ui_display_main_screen(){
  int cursor = 0, old_cursor = 0;
  int x = 94, y = 85;
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "rush"), 0, 0);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "rush_cursor_on"), x, y);
  int key = HERE_ARROW;
  while( key != '\n' && key != 'q'){
    key = term_read_key(stdin);
    old_cursor = cursor;
    if(term_is_arrow_key(key)){
      switch (key) {
      case UP_ARROW:
        cursor--;
        cursor = (cursor < 0) ? 0 : cursor;
        break;
      case DOWN_ARROW:
        cursor++;
        cursor = (cursor > 2) ? 2 : cursor;
        break;
      default:;
      }
    }
    if(cursor != old_cursor){
      sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "rush_cursor_off"), x, y + old_cursor*29);
      sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "rush_cursor_on"), x, y + cursor*29);
    }
  }
  if(key == 'q'){
    return UINT_ERROR;
  }
  return cursor;
}

int ui_get_cursor()
{
    return ui_map_get_cursor(ui->map);
}

World *ui_get_world()
{
    if (ui)
        return ui->w;
    return NULL;
}

int ui_redraw_tile(int tile_index){
  ui_map_redraw_tile(ui->map, tile_index);
  return !UINT_ERROR;
}

int ui_redraw_layers(){
  ui_map_layers_draw(ui->map);
  return !UINT_ERROR;
}

int ui_redraw_neighbours(int tile_index){
  ui_map_redraw_neighbours(ui->map, tile_index);
  return !UINT_ERROR;
}

int ui_update_world_info(){
  ui_world_info_draw(ui->wi);
  return !UINT_ERROR;
}

// No need to call this after calling update cursor, it gets called automatically.
int ui_update_tile_info() {
  ui_tile_info_draw(ui->ti, ui_get_cursor());
  return !UINT_ERROR; // TODO: this doesn't mean anything
}

void ui_redraw_sidebar() {
    ui_update_tile_info();
    ui_update_world_info();
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

bool ui_show_dialogue(char *question)
{
    if (!ui)
        return false;

    return ui_text_panel_dialogue(ui->tp, question);
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

void ui_draw_interface()
{
    if (!ui)
        return;

    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "interface"), 0, 0);
}
Building *ui_control_build_panel(){
  if(!ui){
    HE("Null user interface - REALLY WEIRD ERROR", "ui_control_build_panel")
    return NULL;
  }
  return ui_build_panel_control(ui->bp);
}
int ui_control_exchange_panel(int *res_id, int building_level){
  if(!ui){
    HE("Null user interface - REALLY WEIRD ERROR", "ui_control_build_panel")
    return UINT_ERROR;
  }
  return ui_exchange_panel_control(ui->ep, res_id, building_level);
}
int ui_control_code_panel(int *res_from, int *res_id, int building_level){
  if(!ui){
    HE("Null user interface - REALLY WEIRD ERROR", "ui_control_build_panel")
    return UINT_ERROR;
  }
  return ui_code_panel_control(ui->cp, res_from, res_id, building_level);
}
