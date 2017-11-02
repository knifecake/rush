#include <stdio.h>
#include <stdlib.h>

#include "minitest.h"
#include "../src/entities/resource.h"
#include "../src/entities/building.h"
#include "../src/asset_loaders/building_loader.h"
#include "../src/lib/error_handling.h"


int main(){

  int i = 0;
  FILE *f=NULL;
  Building **buildings;

  f=fopen("example_buiilding_loader.txt", "r");
  if(!f){
    HE("No puedo abrir el archivo.", "test_building_loader");
    return -1;
  }

  buildings = load_buildings_from_file(f, 3);
  assert("can load buildings correctly", buildings);

  while(buildings[i]){
    building_destroy(buildings[i]);
    i++;
  }
  fclose(f);
  return failed_tests();
}
