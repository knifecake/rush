#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"

#include <string.h>

#define TILE_INFO_LINES 4

//TODO: Load this array below from files
UIRect text_coords[]={
  {276, 132, 39, 7}, //Visible or not, line 0
  {276, 144, 39, 7}, //Enemies or hp, line 1
  {276, 156, 43, 7}, //Resources per turn, line 2
  {276, 168, 39, 7}  //Remaining resources, line 3
};

struct _UITileInfo {
    World *w;
    UITextPanel *tp[TILE_INFO_LINES];
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
      ti->tp[i] = ui_text_panel_new(text_coords[i], ui_get_font());
    }
    return ti;
}

void ui_tile_info_draw(UITileInfo *ti, int tile_index)
{
    if (!ti || tile_index < 0) {
        HE("invalid arguments", "ui_tile_info_draw");
        return;
    }

    /*Tile *tile = world_tile_at_index(ti->w, tile_index);
    if (!tile) {
        HE("could not retrieve tile", "ui_tile_info_draw");
    }

    char *info = oopsalloc(MAX_RESOURCES * 10 + 100, sizeof(char), "ui_tile_info_draw");

    // communicate the presence of a building // TODO: remove this when building sprites are done
    Building *b = tile_get_building(tile);
    if (!b)
        sprintf(info, "Tile %d\nPress b to build.\n", tile_index);
    else
        sprintf(info, "Tile %d\n%s (%d)\n", tile_index, building_get_sprite(b), building_get_level(b));

    for (int i = 0; i < MAX_RESOURCES; i++)
        sprintf(info, "%s%d: %d\n", info, i, tile_get_remaining_resources(tile, i));

    ui_text_panel_print(ti->tp, info);*/
    for(int i = 0; i < TILE_INFO_LINES; ++i){
      ui_tile_info_print_single_line(ti,tile_index, i);
    }
}

void ui_tile_info_print_single_line (UITileInfo *ti, int tile_index, int line_index){
  Tile *tile = world_tile_at_index(ti->w, tile_index);
  if (!tile) {
      HE("could not retrieve tile", "ui_tile_info_draw");
  }
  bool visible = tile_get_visible(tile);
  ui_text_panel_clear(ti->tp[line_index]);
  if(!visible){
    switch (line_index) {
      case 0:
        ui_text_panel_print(ti->tp[line_index], "Not visible");
        break;
      case 1:;
        char *enem_hp = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_tile_info_print_single_line");
        sprintf(enem_hp, "%7d", tile_get_enemies(tile));
        ui_text_panel_print(ti->tp[line_index], enem_hp);
        free(enem_hp);
        break;
      default:
        break;
    }
  }else{
    switch (line_index) {
      case 0:
        ui_text_panel_print(ti->tp[line_index], "Visible");
        break;
      case 1:;
        char *enem_hp = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_tile_info_print_single_line");
        Building *b;
        b = tile_get_building(tile);
        if(!b){
          sprintf(enem_hp, "NONE   ");
        }else{
          sprintf(enem_hp, "%7d", building_get_health(b));
        }
        ui_text_panel_print(ti->tp[line_index], enem_hp);
        free(enem_hp);
        break;
      case 2:;
        char *res_turn = oopsalloc(strlen("0000/turn")+1, sizeof(char), "ui_tile_info_print_single_line");
        if(tile_find_resource_index(tile)==-1){
          sprintf(res_turn, "NONE/turn");
        }else{
          sprintf(res_turn, "%4d/turn", tile_get_resource_per_turn(tile, tile_find_resource_index(tile)));
        }
        ui_text_panel_print(ti->tp[line_index], res_turn);
        free(res_turn);
        break;
      case 3:;
        char *rem_res = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_tile_info_print_single_line");
        if(tile_find_resource_index(tile)==-1){
          sprintf(rem_res, "NONE   ");
        }else{
        sprintf(rem_res, "%7d", tile_get_remaining_resources(tile, tile_find_resource_index(tile)));
        }
        ui_text_panel_print(ti->tp[line_index], rem_res);
        break;
      default:
        break;
    }
  }
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
