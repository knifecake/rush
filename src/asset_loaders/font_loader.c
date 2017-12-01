#include "./font_loader.h"
#include "../lib/error_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFLEN 35

Sprite **load_font(char init, char last){
 if(init < ' ' || last < ' '){
   HE("invalid_parameters", "load_font")
   return NULL;
 }
 char buffer[BUFFLEN];
 int range = last - init + 1;
 Sprite **list = oopsalloc(range, sizeof(Sprite *), "load_font");
 if(!list){
   HE("cannot alloc memory for list", "load_font")
   return NULL;
 }
 if(range < 0){
   HE("range is negative", "load_font")
   return NULL;
 }
 FILE *fp = NULL;
 for(int i = 0; i < range; i++) {
   sprintf(buffer, "./assets/img/font/%d.png",i+init);
   fp = fopen(buffer, "r");
   list[i] = sprite_new (fp);
   if(!list[i]){
     HE("cannot alloc memory for list[i]", "load_font")
     for(int j = 0; j < i; j++){
       sprite_destroy(list[j]);
     }
     free(list);
     return NULL;
   }
   fclose(fp);
 }
 return list;
}

void destroy_font(Sprite **list, char init, char last){
  if(!list){
    HE("invalid parameters", "free_font")
    return;
  }
  int range = last - init + 1;
  if(range < 0){
    HE("range is negative", "free_font")
    return;
  }
  for (int i = 0; i<range; i++){
    sprite_destroy(list[i]);
  }
  free(list);
}
