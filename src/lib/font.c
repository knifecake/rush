#include "font.h"
#include "error_handling.h"
#include <string.h>

void font_write(FILE *fp, Sprite **letters_assets, char *string, int x0, int y0, int maxsize){
  if(!fp || !letters_assets || !string || x0<1 || y0<0 || maxsize < 1){
    HE("null parameters", "sprite_write")
    return;
  }
  int x, y;
  x = x0;
  y = y0;
  if(maxsize > strlen(string)){
    maxsize = strlen(string);
  }
  for (int i = 0; i < maxsize; i++){
    if(string[i] == '\n'){
      x = x0;
      y += sprite_get_h(letters_assets['A' - ' ']);
      continue;
    }
    int index = string[i] - ' ';
    sprite_draw(fp, letters_assets[index], x, y);
    x += sprite_get_w(letters_assets[index]);
  }
}
