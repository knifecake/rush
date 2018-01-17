/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../lib/sprite.h"
#include "../lib/dict.h"
#include "../lib/terminal.h"

#include <string.h>
#include <math.h>

UIRect ex_prices[]={
   {270,37,39,7}, //Payment
   {276,61,39,7}, //Gin income
   {276,85,39,7} //Coffee income
 };

struct _UIExchangePanel{
  int cursor;
  int price;

  UITextPanel *tp[3];
  World *w;
  int man_x;
  int man_y;
  int first_x;
  int first_y;
  int delta_y;
};

/* Private functions */
void _panel_draw(UIExchangePanel *ep, int x0, int y0);
void _redraw_cursor(UIExchangePanel *ep, int old_cursor);
void _rewrite_prices(UIExchangePanel *ep);

UIExchangePanel *ui_exchange_panel_new(World *w){
  if(!w) return NULL;
  UIExchangePanel *ep = oopsalloc(1, sizeof(UIExchangePanel), "ui_exchange_panel_new");
  for (int i = 0; i < 3; ++i) {
    ep->tp[i] = ui_text_panel_new(ex_prices[i], ui_get_font());
  }
  ep->cursor = 0;
  ep->price = 0;
  ep->man_x = 279;
  ep->man_y = 13;
  ep->first_x = 263;
  ep->first_y = 59;
  ep->delta_y = 24;
  ep->w = w;
  return ep;
}
int ui_exchange_panel_control(UIExchangePanel *exchangepanel, int *resource_id){
  if(!exchangepanel || !resource_id){
    HE("Invalid arguments", "ui_exchange_panel_control")
    return UINT_ERROR;
  }
  _panel_draw(exchangepanel, 259, 6);
  _redraw_cursor(exchangepanel, exchangepanel->cursor);
  _rewrite_prices(exchangepanel);
  int key = HERE_ARROW;
  while(key != '\n' && key != 'q'){
    int old_cursor = exchangepanel->cursor;
    key = term_read_key(stdin);
    if(term_is_arrow_key(key)){
      switch (key) {
        case UP_ARROW:
          exchangepanel->cursor--;
          exchangepanel->cursor = (exchangepanel->cursor < 0) ? 0 : exchangepanel->cursor;
          _redraw_cursor(exchangepanel, old_cursor);
          break;
        case DOWN_ARROW:
          exchangepanel->cursor++;
          exchangepanel->cursor = (exchangepanel->cursor > 1) ? 1 : exchangepanel->cursor;
          _redraw_cursor(exchangepanel, old_cursor);
          break;
        case LEFT_ARROW:
          exchangepanel->price--;
          exchangepanel->price = (exchangepanel->price < 0) ? 0 : exchangepanel->price;
          _rewrite_prices(exchangepanel);
          break;
        case RIGHT_ARROW:
          exchangepanel->price++;
          int manolos_resource = world_get_resource_quantity(exchangepanel->w, 0);
          exchangepanel->price = (exchangepanel->price > manolos_resource) ? manolos_resource : exchangepanel->price;
          _rewrite_prices(exchangepanel);
          break;
        default:
          HE("Cannot move cursor", "ui_exchange_panel_new")
          ui_draw_interface();
          ui_redraw_sidebar();
          return UINT_ERROR;
      }
    }
  }
  if(key == 'q'){
    ui_draw_interface();
    ui_redraw_sidebar();
    exchangepanel->price = 0;
    exchangepanel->cursor = 0;
    return UINT_ERROR;
  }
  ui_draw_interface();
  ui_redraw_sidebar();
  *resource_id = exchangepanel->cursor + 1;
  exchangepanel->cursor = 0;
  int price_ret = exchangepanel->price;
  exchangepanel->price = 0;
  return price_ret;
}

void ui_exchange_panel_destroy(UIExchangePanel *ep){
  if(!ep) return;
  for (int i = 0; i < 3; ++i){
    ui_text_panel_destroy(ep->tp[i]);
  }
  free(ep);
}
void _panel_draw(UIExchangePanel *ep, int x0, int y0){
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_back") , x0, y0);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "big_manolos") , ep->man_x, ep->man_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_on") , ep->first_x-1, ep->first_y-1);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "resource_2"), ep->first_x, ep->first_y);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "resource_3"), ep->first_x, ep->first_y+ep->delta_y);
}

void _redraw_cursor(UIExchangePanel *ep, int old_cursor){
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_off") , ep->first_x - 1, ep->first_y + ep->delta_y*old_cursor - 1);
  sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "exchange_panel_cursor_on") , ep->first_x - 1, ep->first_y + ep->delta_y*ep->cursor - 1);
}

void _rewrite_prices(UIExchangePanel *ep){
  char prices[9];
  sprintf(prices, "%08d", ep->price);
  ui_text_panel_print(ep->tp[0], prices);
  sprintf(prices, "%08d", world_get_price_exchange(ep->price, 0, 1));
  ui_text_panel_print(ep->tp[1], prices);
  sprintf(prices, "%08d", world_get_price_exchange(ep->price, 0, 2));
  ui_text_panel_print(ep->tp[2], prices);
}
