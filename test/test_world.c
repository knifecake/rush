#include "minitest.h"

#include "../src/entities/world.h"

/*
 * An example test specification for world.
 *
 * Apart from being an example/tutorial, this file actually tests the world entity.
 *
 * Some general considerations:
 * You should test every function you write for the expected input and also for
 * edge cases. A good test will only test one thing about a function or about
 * an entity. For testing you can use the assert function which takes a
 * description of the test as its first argument and a boolean value as the
 * second. For more information see the documentation in test/mintest.h
 *
 * Keep in mind that these test are always run on the same order, which may
 * result in not being able to detect some edge cases that might cause the
 * program to break. We still do not have a solution other than writing your
 * tests in a thorough way that takes this order into account. If we were
 * really clever we would randomize the order in which these asserts are
 * performed ;).
 */

int main(void) {
    /*
     * Note how you can check for a non-NULL pointer without any comparison or equality operator. If the pointer returned is not NULL then it won't be 0. In C, anything that's not zero will be evaluated to true, so this test will pass when world_new returns a new pointer.
     *
     * Also, keep in mind that you may not nest the call to world new inside
     * the assert call. This is because world new is expected to return a
     * pointer that is now your responsibility to free. You should either nest
     * the assignment inside the function call or check for the value of the
     * variable. In any case you have to free the allocated entity after you
     * finish testing it.
     */
    Tile **map=NULL;
    Building **b=NULL;
    float flist[]={0,1.0,2.3};
    int ilist[]={0,1,2};


    map=(Tile **)malloc(4*sizeof(Tile *));
    map[0]=tile_new(0, "tile0", flist, ilist, 0);
    map[1]=tile_new(1, "tile1", flist, ilist, 0);
    map[2]=tile_new(2, "tile2", flist, ilist, 0);

    b=(Building **)malloc(4*sizeof(Building *));
    b[0]=building_new(0);
    b[1]=building_new(1);
    b[2]=building_new(2);

    World *w = world_new(map, b);
    assert("can create world with valid parameters", w);

    /*
     * Tests for destruction are hard with our current entity_destroy
     * prototypes. However, bad code in these methods will be picked up by
     * Valgrind.
     */
    world_destroy(w);
    assert("can destroy world", true);

    /*
     * You may directly nest a function call into an assert call if the
     * function does not return something that needs to be freed later.
     */
    assert("cannot create a world with invalid parameters", !world_new(NULL, NULL));

    assert("cannot create a world with invalid parameters", !world_new(NULL, b));

    assert("cannot create a world with invalid parameters", !world_new(map, NULL));

    /*
     * Always, always return the value of failed_tests(). This value will be
     * zero on success of all tests or the number of tests failed otherwise. In
     * this way, our continuous integration platform will be able to tell if
     * any tests have failed.
     */
     free(map);
     free(b);

    return failed_tests();
}
