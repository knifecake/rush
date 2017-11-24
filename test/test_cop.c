#include <stdlib.h>

#include "minitest.h"

#include "../src/lib/cop.h"
#include "../src/lib/error_handling.h"

#define EXT_CMD_FILENAME "test/assets/stub_cmd.txt"

// This is just a test functions to associate commands to
int cop_f_stub(void *w, char *cmd, char **msg, int num_msg)
{
    return !UINT_ERROR;
}
int main(void)
{
    FILE *fp = fopen(EXT_CMD_FILENAME, "r");
    if (!fp) {
        assert("could not open file for testing cop", false);
        return failed_tests();
    }

    // a stub world object
    void *w = malloc(sizeof(char));

    CoP *c;
    assert("can create a cop from a command definition file",
            c = cop_new(fp));

    fclose(fp);
    assert("running an existing but not associated cmd yields an error",
            UINT_ERROR == cop_exec(c, "t", w));

    assert("can associate a function to an internal command",
            cop_assoc(c, "test", cop_f_stub));

    assert("running a cmd with an associated function does not yield an error",
            UINT_ERROR != cop_exec(c, "t", w));

    assert("running a non existing cmd yields an error if there is no error cmd defined",
            UINT_ERROR == cop_exec(c, "non existing cmd", w));

    assert("can set an error cmd which is an ext cmd",
            UINT_ERROR != cop_set_error_cmd(c, "404_not_found"));

    assert("running a non existing cmd with a set error cmd yields an error id error cmd is not associated to a function",
            UINT_ERROR == cop_exec(c, "non existing cmd", w));

    assert("can associate another function (to the error cmd)",
            UINT_ERROR != cop_assoc(c, "error_cmd", cop_f_stub));

    assert("running a non existing cmd with a set and associated error cmd does not yield an error",
            UINT_ERROR != cop_exec(c, "non exixting cmd", w));

    return failed_tests();
}
