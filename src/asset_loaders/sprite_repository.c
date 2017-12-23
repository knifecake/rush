#include "sprite_repository.h"

#include "../lib/error_handling.h"
#include "../lib/config.h"
#include "../../lib/lineread.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 256

Dict *load_sprite_dict_from_file(char *filename){
  if(!filename){
    HE("Filename is null", "load_sprite_dict_from_file")
    return NULL;
  }
  FILE *fp = fopen(filename, "r");
  if(!fp){
    HE("File not found", "load_sprite_dict_from_file");
    return NULL;
  }
  char *sprite_dir = config_get("sprite dir");
  char *sprite_number = fgetll(fp);
  int size = atoi(sprite_number); free(sprite_number);
  Dict *d = dict_new( size, &cmp_string, &cmp_sprite, &free_sprite);
  for(int i = 0; i < size; i++){
    char full_name[MAX_NAME];
    char *buff = fgetll(fp);
    sprintf(full_name, "%s%s.png", sprite_dir, buff);
    FILE *img = fopen(full_name, "r");
    if(UINT_ERROR == dict_set(d, buff, sprite_new(img))){
      HE("Error setting a pair key-value", "load_sprite_dict_from_file")
      return NULL;
    }
    fclose(img);
  }
  return d;
}
