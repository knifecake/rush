#include <stdio.h>

typedef struct _CoP CoP;

typedef int (*CoP_fun)(void *, char *, char **, int n);

CoP *cop_new(FILE *cmdfile);

void cop_destroy(CoP *c);

int cop_assoc(CoP *c, const char *name, CoP_fun f);

int cop_set_error_cmd(CoP *c, const char *cmd);

int cop_exec(CoP *c, char *cmd, void *p);

void cop_print(FILE *s, CoP *c);
