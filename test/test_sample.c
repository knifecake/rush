#include "minitest.h"

int main(void)
{
    assert("this should pass", true);
    // assert("this should fail", false);

    return failed_tests();
}
