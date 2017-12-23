#include "minitest.h"

#include "../src/ui.h"

char buff[100];

char *int_2_hex(int *i)
{
    if (!i) return NULL;
    sprintf(buff, "%d", *i);
    return buff;
}

int main(void) {

    int a = 1, b = 2, c = 3;
    int *ints[3] = {&a, &b, &c};
    int *selected;

    UIList *l;
    assert("can create a ui list", l = ui_list_new((void **)ints, 3, int_2_hex));
    assert("can display a list", selected = ui_list_present(l));

    ui_list_destroy(l);
    return failed_tests();
}
