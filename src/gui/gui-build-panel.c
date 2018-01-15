#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../lib/sprite.h"
#include "../lib/dict.h"
#include "../lib/terminal.h"

#include <string.h>
#include <math.h>

struct _UIBuildPanel {
  int cursor;
  Building **list;
  Sprite *cursor_on;
  Sprite *cursor_off;
  Sprite *buildings[9];
  int town_x;
  int town_y;
  int first_x;
  int first_y;
  int delta_x;
  int delta_y;
};

/* Private functions */
int _get_lvl_one_building_by_cursor(int cursor);
char *_get_message_by_cursor(int cursor);

UIBuildPanel *ui_build_panel_new(Building ** list){
  UIBuildPanel *bp;
  bp = oopsalloc(1, sizeof(UIBuildPanel), "ui_build_panel_new");
  bp->cursor = 0;
  bp->cursor_off = dict_get(ui_get_sprite_dict(), "build_panel_cursor_off");
  if(!bp->cursor_off){
    HE("Cannot load cursor_off image", "ui_build_panel_new")
    return NULL;
  }
  bp->cursor_on = dict_get(ui_get_sprite_dict(), "build_panel_cursor_on");
  if(!bp->cursor_on){
    HE("Cannot load cursor_on image", "ui_build_panel_new")
    return NULL;
  }
  for (int i = 0; i < 9; i++) {
    char name[16];
    sprintf(name, "building_icon_%d", i);
    bp->buildings[i] = dict_get(ui_get_sprite_dict(), name);
    if(!bp->buildings[i]){
      HE("Cannot load building image", "ui_build_panel_new")
      return NULL;}
  }
  bp->town_x = 281;
  bp->town_y = 8;
  bp->first_x = 269;
  bp->first_y = 31;
  bp->delta_x = 25;
  bp->delta_y = 20;
  bp->list = list;
  return bp;
}

int ui_build_panel_move_cursor(UIBuildPanel *bp, UIBuildDirection dir){
  int old_cursor = bp->cursor;
  switch (dir) {
    case DOWN_BDIR:
    bp->cursor += 2;
    break;
    case UP_BDIR:
    bp->cursor -=2;
    break;
    case LEFT_BDIR:
    bp->cursor -= 1;
    break;
    case RIGHT_BDIR:
    bp->cursor +=1;
    break;
    default:;
  }
  bp->cursor = (bp->cursor > 8) ? 8 : bp->cursor;
  bp->cursor = (bp->cursor < 0) ? 0 : bp->cursor;
  ui_build_panel_update_cursor(bp, old_cursor);
  return !UINT_ERROR;
}

void ui_build_panel_update_cursor(UIBuildPanel *bp, int old_cursor){
  int line, column, x, y;
  if (old_cursor == 0){
    sprite_draw(stdout, bp->cursor_off, bp->town_x -1, bp->town_y -1);
  }else{
    line = ceil((float)old_cursor/2.0) - 1;
    column = (old_cursor + 1) % 2;
    x = -1 + bp->first_x + (bp->delta_x * column);
    y = bp->first_y + (line * bp->delta_y) - 1;
    sprite_draw(stdout, bp->cursor_off, x, y);
  }

  if (bp->cursor == 0){
    sprite_draw(stdout, bp->cursor_on, bp->town_x -1, bp->town_y -1);
  }else{
    line = ceil((float)bp->cursor/2.0) - 1;
    column = (bp->cursor + 1) % 2;
    x = -1 + bp->first_x + (bp->delta_x * column);
    y = bp->first_y + (line * bp->delta_y) - 1;
    sprite_draw(stdout, bp->cursor_on, x, y);
  }
  return;
}

int ui_build_panel_get_cursor(UIBuildPanel *bp){
  return bp->cursor;
}

void ui_build_panel_draw(UIBuildPanel *bp, int x, int y){
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "build_panel_back"), x, y);
  for(int i = 0; i < 9; ++i){
    if(!i){
      sprite_draw(stdout, bp->buildings[i], bp->town_x, bp->town_y);
    }else{
      int line = ceil((float)i/2.0) - 1;
      int column = (i + 1) % 2;
      x = bp->first_x + (bp->delta_x * column);
      y = bp->first_y + (line * bp->delta_y);
      sprite_draw(stdout, bp->buildings[i], x, y);
    }
  }
  return;
}

void ui_build_panel_destroy(UIBuildPanel *bp){
  if(bp) free(bp);
}

Building *ui_build_panel_control(UIBuildPanel *buildpanel){
  if(!buildpanel){
    HE("invalid parameters", "ui_build_panel_control")
    return NULL;
  }
  ui_build_panel_draw(buildpanel, 259, 6);
  int key = HERE_BDIR;
  while(key != '\n' && key != 'q'){
    int old_cursor = ui_build_panel_get_cursor(buildpanel);
    key = term_read_key(stdin);
    if(term_is_arrow_key(key)){
      ui_build_panel_move_cursor(buildpanel, key);
    }
    if(old_cursor != buildpanel->cursor){
      char message[128];
      strcpy(message,_get_message_by_cursor(buildpanel->cursor));
      ui_show_msg(message);
    }
  }
  if(key == 'q'){
    ui_draw_interface();
    ui_redraw_sidebar();
    return NULL;
  }
  ui_draw_interface();
  ui_redraw_sidebar();
  int i = 0;
  int building_id = _get_lvl_one_building_by_cursor(buildpanel->cursor);
  while(buildpanel->list[i]){
    if (building_id == building_get_id(buildpanel->list[i])){
      return buildpanel->list[i];
    }
    i++;
  }
  HE("Building not found", "ui_build_panel_control");
  return NULL;
}

int _get_lvl_one_building_by_cursor(int cursor){
  switch (cursor) {
    case 0: return 101; //Town Hall
    case 1: return 111; //EPS
    case 2: return 121; //A Var
    case 3: return 131; //Coffee maker
    case 4: return 141; //Clock
    case 5: return 181; //Bank
    case 6: return 171; //Church
    case 7: return 151; //Market
    case 8: return 161; //Computer
    default:
      HE("Cursor out of range", "_get_lvl_one_building_by_cursor");
  }
  return UINT_ERROR;
}

char *_get_message_by_cursor(int cursor){
  switch (cursor) {
    case 0:
      return "TOWNHALL. YOUR MAIN BUILDING. YOU LIVE HERE."; //Town Hall
    case 1:
      return "EPS. FARM \"MANOLOS\". YOUR CURRENCY. 20 CENTS EACH."; //EPS
    case 2:
      return "A VAR, OR BAL, OR BAR... I DO NOT KNOW. GET GIN TO BUILD. SURPRISED?"; //A Var
    case 3:
      return "COFFEE MAKER. SIMPLE AS THAT. YOU NEED THIS TO SURVIVE. WHAT ELSE?"; //Coffee maker
    case 4:
      return "TIC-TAC (TOE?). GET THOSE MINUTES IF YOU DONT WANNA DIE!"; //Clock
    case 5:
      return "DONT STUDY! BUY YOUR ECTS INSTEAD ;)"; //Bank
    case 6:
      return "PRODUCES SOLDIERS. THERE IS NOTHING BETTER THAN AN ARMY OF MARABINI WARRIORS"; //Church
    case 7:
      return "WANNA SPENT THOSE MANOLOS FOLK?. THROW THEM HERE."; //Market
    case 8:
      return "YOU HACKER... .SPEND ECTS. WIN SKILLS."; //Computer
    default:
      HE("Cursor out of range", "_get_lvl_one_building_by_cursor");
      return NULL;
  }
}
