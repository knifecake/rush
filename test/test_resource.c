#include "minitest.h"

#include "../src/entities/resource.h"

/*
 * An example test specification for Resource.
 *
 * Apart from being an example/tutorial, this file actually tests the Resource entity.
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
     * Note how you can check for a non-NULL pointer without any comparison or equality operator. If the pointer returned is not NULL then it won't be 0. In C, anything that's not zero will be evaluated to true, so this test will pass when resource_new returns a new pointer.
     *
     * Also, keep in mind that you may not nest the call to resource new inside
     * the assert call. This is because resource new is expected to return a
     * pointer that is now your responsibility to free. You should either nest
     * the assignment inside the function call or check for the value of the
     * variable. In any case you have to free the allocated entity after you
     * finish testing it.
     */
    Resource *r = resource_new(1, "sample");
    assert("can create resource with valid parameters", r);

    /*
     * Tests for destruction are hard with our current entity_destroy
     * prototypes. However, bad code in these methods will be picked up by
     * Valgrind.
     */
    resource_destroy(r);
    assert("can destroy resource", true);

    /*
     * You may directly nest a function call into an assert call if the
     * function does not return something that needs to be freed later.
     */
    assert("cannot create a resource with invalid parameters", !resource_new(0, NULL));

    /*
     * Always, always return the value of failed_tests(). This value will be
     * zero on success of all tests or the number of tests failed otherwise. In
     * this way, our continuous integration platform will be able to tell if
     * any tests have failed.
     */
    return failed_tests();
}
