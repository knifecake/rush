#include "minitest.h"

#include "../src/asset_loaders/resource_loader.h"
#include "../src/entities/resource.h"

#define RESOURCE_DB_STUB "test/assets/resources.txt"
#define RESOURCE_DB_STUB_NUM 2

#define RESOURCE_DB_STUB_2 "test/assets/resources-2.txt"
#define RESOURCE_DB_STUB_2_NUM 11

int main(void) {

    Resource *res[MAX_RESOURCES];
    FILE *db = fopen(RESOURCE_DB_STUB, "r");
    FILE *db2 = fopen(RESOURCE_DB_STUB_2, "r");
    if (!db || !db2) {
        assert("could not find resource db stub", false);
        return failed_tests();
    }

    assert("can load a resources db file",
            RESOURCE_DB_STUB_NUM == load_resources_from_file(db, res));

    if (MAX_RESOURCES < RESOURCE_DB_STUB_2_NUM) {
        assert("loads only MAX_RESOURCES even if file has more",
                MAX_RESOURCES == load_resources_from_file(db2, res));
    } else {
        assert("could not test failing edge cases in resources", false);
        printf(" -> ensure %s has more resources than MAX_RESOURCES = %d\n", RESOURCE_DB_STUB_2, MAX_RESOURCES);
        return failed_tests();
    }

    fclose(db);
    fclose(db2);
    return failed_tests();
}
