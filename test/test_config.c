#include "minitest.h"
#include "../src/lib/error_handling.h"

#include "../src/lib/config.h"

#include <string.h>

#define TEST_CONFIG_FILE "test/assets/config.txt"

int main(void) {
    assert("can set a configration pair",
            UINT_ERROR != config_set("one", "uno"));

    assert("can retrieve a set configuration pair",
            strcmp("uno", config_get("one")) == 0);

    assert("cannot retrieve an unset configuration pair",
            !config_get("non existing key"));

    assert("can load config from a file",
            load_config_from_file(TEST_CONFIG_FILE) > 0);

    assert("can retrieve a pair loaded from a file",
            strcmp("test/assets/example_building_loader.txt", config_get("buildings db")) == 0);

    config_destroy();
    return failed_tests();
}
