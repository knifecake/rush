#include "../../lib/lineread.h"
#include "../lib/error_handling.h"

#include "cop.h"

#include <stdlib.h>
#include <string.h>


/*
 * Structure for an external command.
 */
typedef struct {
    /*
     * Name of the external command (what the user must type).
     */
    char *cmd;

    /*
     * Name of the internal command associated with this external command.
     */
    char *internal;

    /*
     * Number of messages defined for this command.
     */
    int num_msg;

    /*
     * Actual messages defined for this command. These will get passed to the
     * function that is run when this command is called.
     */
    char **msg;
} ext_cmd;

/*
 * Structure for an internal command.
 */
typedef struct {
    /*
     * Name of the internal command. External commands point to this command by
     * storing this name on their structure.
     */
    char i_name[MAX_INT_CMD_NAME];

    /*
     * Pointer to the function that will be run when an external command linked
     * to this command is executed.
     */
    CoP_fun f;
} int_cmd;

struct _CoP {
    /*
     * A list of external commands (e_list) of length num_ext;
     */
    ext_cmd **e_list;
    int     num_ext;

    /*
     * The index of the error command in the ext cmd list.
     */
    int     error_cmd;

    /*
     * A list of internal commands of length num_int. i_list is allocated to be
     * as big as e_list even though not all possitions might be used.
     */
    int_cmd **i_list;
    int     num_int;
} _CoP;


/*
 * @private
 *
 * Reads a chunk of the fiven file and returns an external command, NULL on
 * error.
 */
ext_cmd *_cop_read_ext_cmd(FILE *cf)
{
    if (!cf) {
        HE("invalid params", "_cop_read_ext_cmd");
        return NULL;
    }

    ext_cmd *e = calloc(1, sizeof(ext_cmd));
    if (!e) {
        HE("could not allocate memory for external command", "_cop_read_ext_cmd");
        return NULL;
    }

    char *buff;

    // read the command name (the one you type on the terminal)
    e->cmd = fgetll(cf);

    // read the internal command name to which this cmd will be associated
    e->internal = fgetll(cf);

    if (!e->cmd || !e->internal) {
        HE("error reading external command", "_cop_read_ext_cmd");
        free(e->cmd); free(e->internal); free(e);
        return NULL;
    }

    // read the number of messages we'll associate with this command
    buff = fgetll(cf);
    e->num_msg = atoi(buff); free(buff);

    if (e->num_msg < 1) {
        HE("there must be at least one message associated with this command", "_cop_read_ext_cmd");
        free(e->cmd); free(e->internal); free(e);
        return NULL;
    }

    e->msg = calloc(e->num_msg, sizeof(char *));
    if (!e->msg) {
        HE("could not allocate memory for command messages", "_cop_read_ext_cmd");
        free(e->cmd); free(e->internal); free(e);
        return NULL;
    }

    // read messages
    for (int i = 0; i < e->num_msg; i++) {
        e->msg[i] = fgetll(cf);

        if (!e->msg[i]) {
            HE("could not read a command message", "_cop_read_ext_cmd");
            for (int j = 0; j < i; free(e->msg[j++]));
            free(e->msg); free(e->cmd); free(e->internal); free(e);
            return NULL;
        }
    }

    return e;
}

/*
 * @private
 *
 * Destroys an external command.
 */
void _cop_ext_cmd_destroy(ext_cmd *e)
{
    if (!e) return;

    free(e->cmd);
    free(e->internal);

    for (int i = 0; i < e->num_msg; free(e->msg[i++]));
    free(e->msg);
    free(e);
}

/*
 * @private
 *
 * Linearly walks through the list of internal commands looking for one with
 * i_name equal to the given name. Returns the index of that command inside the
 * given CoP, UINT_ERROR if is not found or on error.
 */
int _cop_find_int_cmd_by_name(CoP *c, const char *name)
{
    if (!name || !c) {
        HE("invalid parameters", "_cop_find_int_cmd_by_name");
        return UINT_ERROR;
    }

    for (int i = 0; i < c->num_int; i++) {
        if (c->i_list[i] && strcmp(c->i_list[i]->i_name, name) == 0)
            return i;
    }

    return UINT_ERROR;
}

/*
 * @private
 *
 * Linearly walks thorugh the list of external commands looking for one with
 * cmd equal to the givem cmd. Returns the index of that command in the given
 * cop or UINT_ERROR in case it was not found or if there was an error.
 */
int _cop_find_ext_cmd_by_name(CoP *c, const char *cmd)
{
    if (!cmd || !c) {
        HE("invalid parameters", "_cop_find_int_cmd_by_name");
        return UINT_ERROR;
    }

    for (int i = 0; i < c->num_ext; i++) {
        if (c->e_list[i] && strcmp(c->e_list[i]->cmd, cmd) == 0)
            return i;
    }

    return UINT_ERROR;
}

/*
 * @private
 *
 * Creates an internal command with the given name. Returns the command on
 * success, NULL on error.
 */
int_cmd *_cop_int_cmd_new(const char *name)
{
    if (!name) {
        HE("invalid parameters", "_cop_int_cmd_new");
        return NULL;
    }

    if (strlen(name) > MAX_INT_CMD_NAME - 1) {
        HE("internal command name too long", "_cop_int_cmd_new");
        return NULL;
    }

    int_cmd *ic = calloc(1, sizeof(int_cmd));
    if (!ic) {
        HE("could not create int cmd, out of memory", "_cop_int_cmd_new");
        return NULL;
    }

    strcpy(ic->i_name, name);
    return ic;
}


/*
 * @private
 *
 * Destroys an internal command.
 */
void _cop_int_cmd_destroy(int_cmd *ic)
{
    free(ic);
}

CoP *cop_new(FILE *cf)
{
    if (!cf) {
        HE("invalid parameters", "cop_new");
        return NULL;
    }

    char *buff;

    buff = fgetll(cf);
    if (!buff) {
        HE("invalid command file", "cop_new");
        return NULL;
    }

    CoP *c = oopsalloc(1, sizeof(CoP), "cop_new");

    c->num_ext = atoi(buff); free(buff);
    if (c->num_ext < 1) {
        HE("refusing to load 0 or invalid number of commands from command file", "cop_new");
        free(c); return NULL;
    }

    // allocate memory for as many external commands as we need to
    c->e_list = oopsalloc(c->num_ext, sizeof(ext_cmd *), "cop_new");
    c->i_list = oopsalloc(c->num_ext, sizeof(int_cmd *), "cop_new");

    for (int i = 0; i < c->num_ext; i++)
    {
        c->e_list[i] = _cop_read_ext_cmd(cf);
        if (!c->e_list[i]) {
            HE("could not read an external command", "cop_new");

            // free all previously loaded commands
            for (int j = 0; j < i; _cop_ext_cmd_destroy(c->e_list[j++]));

            free(c);
            return NULL;
        }
    }

    for (int i = 0; i < c->num_ext; i++)
    {
        if (UINT_ERROR == _cop_find_int_cmd_by_name(c, c->e_list[i]->internal))
        {
            // create an internal command for this (and possibly other) ext_cmd
            int_cmd *ic = _cop_int_cmd_new(c->e_list[i]->internal);
            if (!ic) {
                HE("could not create internal command", "cop_new");

                // free all previously loaded int commands
                for (int j = 0; j < c->num_int; _cop_int_cmd_destroy(c->i_list[j++]));

                // free all previously loaded ext commands
                for (int j = 0; j < i; _cop_ext_cmd_destroy(c->e_list[j++]));

                free(c);
                return NULL;
            }

            c->i_list[c->num_int++] = ic;
        }

    }

    c->error_cmd = UINT_ERROR;
    return c;
}

void cop_destroy(CoP *c)
{
    if (!c) return;

    for (int i = 0; i < c->num_ext; _cop_ext_cmd_destroy(c->e_list[i++]));
    for (int i = 0; i < c->num_int; _cop_int_cmd_destroy(c->i_list[i++]));
    free(c->e_list);
    free(c->i_list);
    free(c);
}

int cop_assoc(CoP *c, const char *name, CoP_fun f)
{
    if (!c || !name || !f) {
        HE("invalid parameters", "cop_assoc");
        return UINT_ERROR;
    }

    // get the command with int_cmd = name
    int ic_index = _cop_find_int_cmd_by_name(c, name);
    if (ic_index == UINT_ERROR) {
        HE("could not associate command. int cmd not found", "cop_assoc");
        return UINT_ERROR;
    }

    c->i_list[ic_index]->f = f;

    return c->num_int;
}

int cop_set_error_cmd(CoP *c, const char *cmd)
{
    if (!c || !cmd) {
        HE("invalid parameters", "cop_set_error_cmd");
        return UINT_ERROR;
    }

    c->error_cmd = _cop_find_ext_cmd_by_name(c, cmd);
    if (c->error_cmd == UINT_ERROR) {
        HE("could not set that command as an error command", "cop_set_error_cmd");
        return UINT_ERROR;
    }

    if (_cop_find_int_cmd_by_name(c, c->e_list[c->error_cmd]->internal) == UINT_ERROR) {
        HE("warning – the cmd you set as the error cmd does not have an internal cmd associated with it", "_cop_set_error_cmd");
    }

    return !UINT_ERROR;
}

int cop_exec(CoP *c, char *cmd, void *p)
{
    if (!c || !cmd || !p) {
        HE("invalid parameters", "cop_exec");
        return UINT_ERROR;
    }

    int ec = _cop_find_ext_cmd_by_name(c, cmd);
    if (UINT_ERROR == ec) {
        if (c->error_cmd != UINT_ERROR) {
            ec = c->error_cmd;
        } else {
            HE("don't know how to run this cmd, and no error cmd was set", "cop_exec");
            return UINT_ERROR;
        }
    }

    int ic = _cop_find_int_cmd_by_name(c, c->e_list[ec]->internal);
    if (UINT_ERROR == ic) {
        HE("could not find an internal cmd associated with this cmd", "cop_exec");
        if (ec == c->error_cmd) {
            handle_error(" -> you set an error cmd and it was triggered but did not cop_assoc with any int cmd\n");
        }
        return UINT_ERROR;
    }

    if (!c->i_list[ic]->f) {
        HE("the cmd is recognized but it was not associated with any function", "cop_exec");
            handle_error(" -> run cop_assoc to point this cmd to a function you defined\n");

        return UINT_ERROR;
    }

    return c->i_list[ic]->f(p, cmd,
            c->e_list[ec]->msg,
            c->e_list[ec]->num_msg);
}

void cop_print(FILE *s, CoP *c)
{
    if (!c) {
        HE("invalid parameters", "cop_print");
        return;
    }

    fprintf(s, "External commands:\n");
    for (int i = 0; i < c->num_ext; i++)
        fprintf(s, "%2d. %s -> %s\n", i + 1, c->e_list[i]->cmd, c->e_list[i]->internal);

    fprintf(s, "Internal commands:\n");
    for (int i = 0; i < c->num_int; i++)
        fprintf(s, "%2d. %s -> %p\n", i + 1, c->i_list[i]->i_name, c->i_list[i]->f);

    if (c->error_cmd != UINT_ERROR) {
        fprintf(s, "Error command: %2d. %s -> %s\n", c->error_cmd, c->e_list[c->error_cmd]->cmd, c->e_list[c->error_cmd]->internal);
    } else {
        fprintf(s, "Error command: no error command was defined.\n");
    }
}
