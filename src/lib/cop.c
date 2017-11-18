#include "../../lib/lineread.h"
#include "../lib/error_handling.h"

#include "cop.h"

#include <stdlib.h>
#include <string.h>

#define MAX_INT_CMD_NAME 25

typedef struct {
    char *cmd;
    char *internal;
    int num_msg;
    char **msg;
} ext_cmd;

typedef struct {
    char i_name[MAX_INT_CMD_NAME];
    CoP_fun f;
} int_cmd;

struct _CoP {
    ext_cmd **e_list;
    int     num_ext;
    int     error_cmd;

    int_cmd **i_list;
    int     num_int;
    int     int_max;
} _CoP;


ext_cmd *_cop_read_ext_cmd(CoP *c, FILE *cf)
{
    if (!c || !cf) {
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

void _cop_ext_cmd_destroy(ext_cmd *e)
{
    if (!e) return;

    free(e->cmd);
    free(e->internal);

    for (int i = 0; i < e->num_msg; free(e->msg[i++]));
    free(e->msg);
    free(e);
}

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

int_cmd *_cop_int_cmd_new(const char *name)
{
    if (!name) {
        HE("invalid parameters", "_cop_int_cmd_new");
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

    CoP *c = calloc(1, sizeof(CoP));
    if (!c) {
        HE("could not allocate CoP, out of memory", "cop_new");
        free(buff); return NULL;
    }

    c->num_ext = atoi(buff);
    if (c->num_ext < 1) {
        HE("refusing to load 0 or invalid number of commands from command file", "cop_new");
        free(c); return NULL;
    }

    // allocate memory for as many external commands as we need to
    c->e_list = calloc(c->num_ext, sizeof(ext_cmd *));
    if (!c->e_list) {
        HE("could not allocate memory for external commands", "cop_new");
        free(c); return NULL;
    }

    // TODO: use dynamic allocation here
    c->i_list = calloc(c->num_ext, sizeof(int_cmd *));
    if (!c->i_list) {
        HE("could not allocate memory for internal commands", "cop_new");
        free(c); return NULL;
    }

    for (int i = 0; i < c->num_ext; i++)
    {
        c->e_list[i] = _cop_read_ext_cmd(c, cf);
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
}
