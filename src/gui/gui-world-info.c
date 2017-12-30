#include "../ui-world-info.h"
#include "../lib/error_handling.h"

#include <string.h>

/*
 * UIWorldInfo panel
 *
 * A world info panel intended to be displayed on the top of the sidebar.
 */

#define WORLD_INFO_LINES (MAX_RESOURCES + 1)
#define LINE_HEIGHT 12
#define RESOURCE_ICON_WIDTH 16

 /*
  * TODO: Implement this array below to be loaded from file
  */
  UIRect text_dim[]={
    {276,12,37,7}, //Level, line 0
    {276,25,39,7}, //1st Resource, line 1
    {276,37,39,7}, //2nd Resource, line 2
    {276,49,39,7}, //3rd Resource, line 3
    {276,61,39,7}, //4th Resource, line 4
    {276,73,39,7}, //5th Resource, line 5
    {276,85,39,7}, //6th Resource, line 6
    {276,97,39,7}  //7th Resource, line 7
  };


struct _UIWorldInfo {
    World *w;

    UITextPanel *tp[MAX_RESOURCES+1];
    UIRect text_dim[WORLD_INFO_LINES];
};

UIWorldInfo *ui_world_info_new(World *w, UIRect dim)
{
    if (!w) {
        HE("invalid arguments", "ui_world_info_new");
        return NULL;
    }

    UIWorldInfo *wi = oopsalloc(1, sizeof(UIWorldInfo), "ui_world_info_new");

    wi->w = w;

    for(int i = 0; i < WORLD_INFO_LINES; ++i){
      wi->text_dim[i] = (UIRect) {
          .x = dim.x + RESOURCE_ICON_WIDTH,
          .y = dim.y + i * LINE_HEIGHT,
          .width = dim.width  - RESOURCE_ICON_WIDTH,
          .height = 7
      };

      wi->tp[i] = ui_text_panel_new(wi->text_dim[i], ui_get_font());
    }

    return wi;
}

void ui_world_info_draw(UIWorldInfo *wi)
{
    if (!wi) {
        HE("invalid params", "ui_world_info_draw");
        return;
    }

    Resource **res = world_get_resources(wi->w);
    if (!res) {
        HE("cannot print get resources from world", "ui_world_info_draw");
        return;
    }

    for(int i = 0; i < MAX_RESOURCES+1; ++i){
      ui_world_info_print_single_line(wi, i);
    }
    //TODO: Print resources icons.
}

void ui_world_info_destroy(UIWorldInfo *wi)
{
  for(int i = 0; i < MAX_RESOURCES+1; ++i){
    ui_text_panel_destroy(wi->tp[i]);
  }
  free(wi);
  return;
}

void ui_world_info_print_single_line(UIWorldInfo* wi, int i){
  if(!wi || i<0){
    HE("Input error", "ui_world_info_print_single_line")
    return;
  }
  ui_text_panel_clear(wi->tp[i]);
  //NOTE: Shall this format be loaded from file?
  if (!i){
    char *lvl = oopsalloc(strlen("LVL. 000")+1, sizeof(char), "ui_world_info_draw");
    sprintf(lvl, "LVL. %03d", world_get_player_level(wi->w));
    ui_text_panel_print(wi->tp[0], lvl);
    free(lvl);
    return;
  }
  char *info = oopsalloc(strlen("0000000")+1, sizeof(char), "ui_world_info_draw");
  sprintf(info, "%07d", world_get_resource_quantity(wi->w, i-1));
  ui_text_panel_print(wi->tp[i], info);
  free(info);
  //TODO: Decide wether to use %03d and %07d or %3d and %7d.
}
