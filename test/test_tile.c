/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


#include "minitest.h"

#include "../src/entities/tile.h"

#include "../src/entities/building.h"
#include "../src/entities/event.h"
#include "../src/lib/error_handling.h"

int main(void) {
    int rr[] = {1, 2, 3};
    float rm[] = {0.8, 10.9, 10};

    Tile *t = tile_new(1, "filename", rm, rr, 10);
    assert("can create a tile with valid params", t);

    assert("refuses to create a tile with invalid params",
            !tile_new(1, NULL, NULL, NULL, -1));

    assert("tiles are invisible by default",
            !tile_get_visible(t));

    int res[MAX_RESOURCES] = {1, 2, 3};
    Building *b = building_new(10, 1, 1, 10, res, res, "holi", "hola");

    assert("can link a tile and a building",
            UINT_ERROR != tile_build(t, b));

    assert("tile refuses to link a new building",
            UINT_ERROR == tile_build(t, NULL));

    Event *e = event_new("Earthquake", rm, 1, 1, -100);

    assert("can link a tile and an event",
            t = tile_set_event(t, e));
    event_destroy(e);

    assert("linked event is not NULL",
            tile_get_event(t));

    assert("tile refuses to link a NULL event",
            !tile_set_event(t, NULL));

    assert("can collect a resource",
            (int )(rr[0] * rm[0]) == tile_collect_resources(t, 0));

    assert("refuses to collect a resource with an invalid id",
            0 == tile_collect_resources(t, MAX_RESOURCES + 1));

            int resources[MAX_RESOURCES]= {0, 0, 0};
    assert("event finishes when number of turns reaches 0",
            tile_next_turn(t, resources) && !tile_get_event(t));
    /*
    assert("tile_next_turn return the correct number of resources",
            resources[0]==0 && resources[1]==228 && resources[2]==300);
    */
    building_destroy(b);
    tile_destroy(t);

    return failed_tests();
}
