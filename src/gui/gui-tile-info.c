#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"

#include <string.h>

#define TILE_INFO_LINES 4
#define RESOURCE_ICON_WIDTH 16
#define LINE_HEIGHT 12

struct _UITileInfo {
    World *w;
    UITextPanel *tp[TILE_INFO_LINES];
    UIRect text_coords[TILE_INFO_LINES];
};


UITileInfo *ui_tile_info_new(World *w, UIRect dim)
{
    if (!w) {
        HE("invalid arguments", "ui_tile_info_new");
        return NULL;
    }

    UITileInfo *ti = oopsalloc(1, sizeof(UITileInfo), "ui_tile_info_new");

    ti->w = w;
    for(int i = 0; i < TILE_INFO_LINES; ++i){
      ti->text_coords[i] = (UIRect) {
          .x = dim.x + RESOURCE_ICON_WIDTH,
          .y = dim.y + i * LINE_HEIGHT,
          .w = dim.w - RESOURCE_ICON_WIDTH,
          .h = ui_font_get_char_height(ui_get_font()) + 1
      };

      ti->tp[i] = ui_text_panel_new(ti->text_coords[i], ui_get_font());
    }

    return ti;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index)
{
    if (!ti || tile_index < 0) {
        HE("invalid arguments", "ui_tile_info_draw");
        return;
    }
    for (int i = 0; i < TILE_INFO_LINES; ++i){
      ui_tile_info_print_single_line(ti,tile_index, i);
    }
}

void ui_tile_info_print_single_line (UITileInfo *ti, int tile_index, int line_index){
    if (!ti || line_index < 0 || line_index >= TILE_INFO_LINES) {
        HE("invalid arguments", "ui_tile_info_print_single_line");
        return;
    }

  Tile *tile = world_tile_at_index(ti->w, tile_index);
  if (!tile) {
      HE("could not retrieve tile", "ui_tile_info_draw");
  }

  int x = 263;
  int y = 130;
  y += line_index*LINE_HEIGHT;
  Sprite *icon;

  bool visible = tile_get_visible(tile);
  ui_text_panel_clear(ti->tp[line_index]);
  if(!visible){
    switch (line_index) {
      case 0:
        ui_text_panel_print(ti->tp[line_index], "Not visible");
        icon = dict_get(ui_get_sprite_dict(), "not_visible");
        break;
      case 1:;
        char *enem_hp = oopsalloc(strlen("0000000") + 1, sizeof(char), "ui_tile_info_print_single_line");
        sprintf(enem_hp, "%07d", tile_get_enemies(tile));
        ui_text_panel_print(ti->tp[line_index], enem_hp);
        free(enem_hp);
        icon = dict_get(ui_get_sprite_dict(), "enemies");
        break;
      default:
        icon = dict_get(ui_get_sprite_dict(), "void_icon");
        break;
    }
  }else{
    char res_name[20];
    switch (line_index) {
      case 0:
        ui_text_panel_print(ti->tp[line_index], "Visible");
        icon = dict_get(ui_get_sprite_dict(), "visible");
        break;
      case 1:;
        char *enem_hp = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_tile_info_print_single_line");
        Building *b;
        b = tile_get_building(tile);
        if(!b){
            // if no building is present, there may be enemies
            if (tile_get_enemies(tile) >= 0) {
                sprintf(enem_hp, "%07d", tile_get_enemies(tile));
                icon = dict_get(ui_get_sprite_dict(), "enemies");
            } else {
              sprintf(enem_hp, "NONE");
              icon = dict_get(ui_get_sprite_dict(), "void_icon");
            }
        }else{
          sprintf(enem_hp, "%07d", building_get_health(b));
          icon = dict_get(ui_get_sprite_dict(), "hp");
        }
        ui_text_panel_print(ti->tp[line_index], enem_hp);
        free(enem_hp);
        break;
      case 2:;
        char *res_turn = oopsalloc(strlen("0000/turn")+1, sizeof(char), "ui_tile_info_print_single_line");
        int res_id;
        if( (res_id = tile_find_resource_index(tile)) ==-1){
          sprintf(res_turn, "NONE/turn");
          icon = dict_get(ui_get_sprite_dict(), "void_icon");
        }else{
          sprintf(res_turn, "%04d/turn", tile_get_resource_per_turn(tile, tile_find_resource_index(tile)));
          sprintf(res_name, "resource_%d", res_id+1);
          icon = dict_get(ui_get_sprite_dict(), res_name);
        }
        ui_text_panel_print(ti->tp[line_index], res_turn);
        free(res_turn);
        break;
      case 3:;
        char *rem_res = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_tile_info_print_single_line");
        if( (res_id = tile_find_resource_index(tile)) ==-1){
          sprintf(rem_res, "NONE");
          icon = dict_get(ui_get_sprite_dict(), "void_icon");
        }else{
          sprintf(rem_res, "%07d", tile_get_remaining_resources(tile, tile_find_resource_index(tile)));
          /*sprintf(res_name, "resource_%d_rem", res_id+1); NOTE: if we want to get different remaining*/
          sprintf(res_name, "resource_%d", res_id+1);
          icon = dict_get(ui_get_sprite_dict(), res_name);
        }
        ui_text_panel_print(ti->tp[line_index], rem_res);
        break;
      default:
        // TODO: this line silences a warning, but this is bad practice
        icon = NULL;
        break;
    }
  }
  sprite_draw(stdout, icon, x, y);
}

void ui_tile_info_destroy(UITileInfo *ti)
{
    if (!ti)
        return;
    for(int i = 0; i < TILE_INFO_LINES; ++i){
      ui_text_panel_destroy(ti->tp[i]);
    }
    free(ti);
}
