#include "minitest.h"

#include "../src/lib/messages.h"

int main(void)
{
    assert("can print hello", 5 == show_message("hello"));

    assert("can handle a format string with placeholders and arguments for substitution",
            4 == show_message("%d", 1729));

    assert("can handle multiple placeholders",
            3 == show_message("%s %d", "a", 1));

    return failed_tests();
}
