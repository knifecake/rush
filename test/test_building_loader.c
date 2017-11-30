#include <stdio.h>
#include <stdlib.h>

#include "minitest.h"
#include "../src/entities/resource.h"
#include "../src/entities/building.h"
#include "../src/asset_loaders/building_loader.h"
#include "../src/lib/error_handling.h"


int main(){

  FILE *f=NULL;
  Building **buildings;

  f=fopen("test/assets/example_building_loader.txt", "r");
  if(!f){
    assert("cannot load building test file", false);
    return -1;
  }

  buildings = load_buildings_from_file(f, 2);
  assert("can load buildings correctly", buildings);

  building_list_destroy(buildings);
  fclose(f);

  return failed_tests();
}
