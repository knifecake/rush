/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


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
  assert("can load buildings", buildings);

  assert("correctly loads the id", building_get_id(buildings[0]) == 524);

  assert("correctly loads the level", building_get_level(buildings[0]) == 3);

  assert("correctly loads the unlocking level", building_get_unlocking_level(buildings[0]) == 14);

  assert("correctly loads the health", building_get_health(buildings[0]) == 1599);

  bool truth = true;
  for(int i = 0; i < 2; i++){
    truth *= (building_get_cost(buildings[0], i) == 550);
  }
  assert("correctly loads the costs", truth == true);

  building_list_destroy(buildings);
  fclose(f);

  return failed_tests();
}
