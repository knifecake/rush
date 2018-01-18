#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../lib/sprite.h"
#include "../lib/dict.h"
#include "../lib/terminal.h"

#include <string.h>
#include <math.h>

UIRect code_prices[]={
   {270,37,39,7}, //Payment (in ECTS)
   {276,61,39,7} //Code income
 };

 struct _UICodePanel{
   int cursor;
   int price;
   UITextPanel *tp[2];
   World *w;
   int ects_x;
   int ects_y;
   int first_x;
   int first_y;
   int delta_x;
   int delta_y;
 };

 /* Private functions */
 void _code_panel_draw(UICodePanel *cp, int x0, int y0);
 void _code_redraw_cursor(UICodePanel *cp, int old_cursor);
 void _code_rewrite_prices(UICodePanel *cp, int building_level);
 void _code_redraw_big_resource(UICodePanel *cp);
/* --------------- */
UICodePanel *ui_code_panel_new(World *w){
 if(!w) return NULL;
 UICodePanel *cp = oopsalloc(1, sizeof(UICodePanel), "ui_code_panel_new");
 for (int i = 0; i < 2; ++i) {
   cp->tp[i] = ui_text_panel_new(code_prices[i], ui_get_font());
 }
 cp->cursor = 0;
 cp->price = 0;
 cp->ects_x = 279;
 cp->ects_y = 13;
 cp->first_x = 263;
 cp->first_y = 59;
 cp->delta_x = 14;
 cp->delta_y = 24;
 cp->w = w;
 return cp;
}

void ui_code_panel_destroy(UICodePanel *cp){
 if(!cp) return;
 for (int i = 0; i < 2; ++i){
   ui_text_panel_destroy(cp->tp[i]);
 }
 free(cp);
}

int ui_code_panel_control(UICodePanel *cp,int *resource_from, int *resource_id, int building_level){
 if(!cp || !resource_id){
   HE("Invalid arguments", "ui_code_panel_control")
   return UINT_ERROR;
 }
 _code_panel_draw(cp, 259, 6);
 _code_redraw_cursor(cp, cp->cursor);
 _code_rewrite_prices(cp, building_level);
 int key = HERE_ARROW;
 while(key != '\n' && key != 'q'){
   int old_cursor = cp->cursor;
   key = term_read_key(stdin);
   if(term_is_arrow_key(key)){
     switch (key) {
       case UP_ARROW:
         cp->cursor--;
         cp->cursor = (cp->cursor < 0) ? 0 : cp->cursor;
         _code_redraw_cursor(cp, old_cursor);
         _code_rewrite_prices(cp, building_level);
         if(cp->cursor == 0){
           _code_redraw_big_resource(cp);
         }
         break;
       case DOWN_ARROW:
         cp->cursor++;
         cp->cursor = (cp->cursor > 4) ? 4 : cp->cursor;
         _code_redraw_cursor(cp, old_cursor);
         _code_rewrite_prices(cp, building_level);
         if(old_cursor == 0){
           _code_redraw_big_resource(cp);
         }
         break;
       case LEFT_ARROW:
         if(!old_cursor){
             cp->price-=10;
             cp->price = (cp->price < 0) ? 0 : cp->price;
             _code_rewrite_prices(cp, building_level);
         }else{
           cp->cursor--;
           cp->cursor = (cp->cursor < 0) ? 0 : cp->cursor;
           _code_redraw_cursor(cp, old_cursor);
           _code_rewrite_prices(cp, building_level);
           if(cp->cursor == 0){
             _code_redraw_big_resource(cp);
           }
         }
         break;
       case RIGHT_ARROW:
         if(!old_cursor){
           cp->price+=10;
           int ects_resource = world_get_resource_quantity(cp->w, 5);
           cp->price = (cp->price > ects_resource) ? ects_resource : cp->price;
           _code_rewrite_prices(cp, building_level);
         }else{
           cp->cursor++;
           cp->cursor = (cp->cursor > 4) ? 4 : cp->cursor;
           _code_redraw_cursor(cp, old_cursor);
           _code_rewrite_prices(cp, building_level);
           if(old_cursor == 0){
             _code_redraw_big_resource(cp);
           }
         }
         break;
       default:
         HE("Cannot move cursor", "ui_code_panel_new")
         ui_draw_interface();
         ui_redraw_sidebar();
         return UINT_ERROR;
     }
   }
 }
 if(key == 'q'){
   ui_draw_interface();
   ui_redraw_sidebar();
   cp->price = 0;
   cp->cursor = 0;
   return UINT_ERROR;
 }
 ui_draw_interface();
 ui_redraw_sidebar();
 switch (cp->cursor) {
   case 0:
    *resource_id =  4;
    *resource_from = 5;
    break;
   case 1:
   case 2:
   case 3:
   case 4:
      *resource_id = cp->cursor - 1;
      *resource_from = 4;
 }
 int price_ret = cp->price;
 cp->price = 0;
 cp->cursor = 0;
 return price_ret;
}


void _code_panel_draw(UICodePanel *cp, int x0, int y0){
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "coding_panel_back") , x0, y0);
  if(!cp->cursor){
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "big_ects") , cp->ects_x, cp->ects_y);
  }else{
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "big_code") , cp->ects_x, cp->ects_y);
  }
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_on") , cp->first_x-1, cp->first_y-1);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "resource_5"), cp->first_x, cp->first_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "skill_0"), cp->first_x, cp->first_y+cp->delta_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "skill_1"), cp->first_x + cp->delta_x, cp->first_y + cp->delta_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "skill_2"), cp->first_x + 2*cp->delta_x, cp->first_y + cp->delta_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "skill_3"), cp->first_x + 3*cp->delta_x, cp->first_y + cp->delta_y);
}
void _code_redraw_cursor(UICodePanel *cp, int old_cursor){
    int old_col = (old_cursor == 0) ? 0 : old_cursor - 1;
    int new_col = (cp->cursor == 0) ? 0 : cp->cursor - 1;
    int old_row = (old_cursor == 0) ? 0 : 1;
    int new_row = (cp->cursor == 0) ? 0 : 1;
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_off") , cp->first_x - 1 + cp->delta_x*old_col, cp->first_y + cp->delta_y*old_row - 1);
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_on") , cp->first_x - 1 + cp->delta_x * new_col, cp->first_y + cp->delta_y*new_row - 1);
}
void _code_rewrite_prices(UICodePanel *cp, int building_level){
  char prices[9];
  if(!cp->cursor){
    sprintf(prices, "%08d", cp->price);
    ui_text_panel_print(cp->tp[0], prices);
    sprintf(prices, "%08d", world_get_price_exchange(cp->price, 5, 4, building_level));
    ui_text_panel_print(cp->tp[1], prices);
    return;
  }
  sprintf(prices, "%08d", world_get_skill_price(cp->w, cp->cursor - 1));
  ui_text_panel_print(cp->tp[0], prices);
}

void _code_redraw_big_resource(UICodePanel *cp){
  if(!cp->cursor){
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "big_ects") , cp->ects_x, cp->ects_y);
    return;
  }
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "big_code") , cp->ects_x, cp->ects_y);
}
