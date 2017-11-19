#ifndef __COP_H__
#define __COP_H__

#include <stdio.h>

/*
 * Maximum length of internal command names.
 */
#define MAX_INT_CMD_NAME 25


/*
 * The Command Processor.
 */
typedef struct _CoP CoP;


/*
 * Type that a function must have to be linked to an internal command in the
 * cop. These kinds of functions should be defined in the controller module of
 * the game.
 */
typedef int (*CoP_fun)(void *, char *, char **, int n);

/*
 * Creates a new CoP from the command file provided. The CoP is in charge of
 * associating user input to the appropriate functionality in the game. It does
 * this by defining two types of commands:
 *
 *  - External Commands: contain the string that the user would type to invoke
 *  the desired functionality. They are always linked to an internal command.
 *  Different external commands can point to the same internal command. In
 *  addition, external commands can have messages associated with them. These
 *  will be passed to the function that is called when the user types the name
 *  of the external cmd. This function should now what to do with them and it
 *  can assume they will be passed on the order defined in the file. For
 *  example, an external command may contain two messages, one for success
 *  (messages[0]) and one for failure (message[1]).  The function that is
 *  called when the user executes this ext cmd will have access to those
 *  messages and will be able to respond accordingly.
 *
 *  - Internal Commands: are used to group ext cmds with similar functionality.
 *  Similar functionality means that the same function can be called on two
 *  different user inputs. It may be the case that the game does not benefit
 *  much from this abstraction, that is, sometimes there will be one internal
 *  cmd per external cmd. The function that gets called when an external
 *  command that points to an internal command is defined at runtime by using
 *  cop_assoc. Internal commands do not need to be defined separately from
 *  external commands, a list of unique internal commands will be derived from
 *  the "pointers" to int cmds that you declare in the cmd file.
 *
 * A note on messages. Messages are intentionally included in the cmd file for two reasons:
 *
 *  - It allows to group cmds which only differ on the user interface (the text
 *  displayed to the user) but share the same underlying logic into internal
 *  commands. As it is internal commands the ones which are linked to the
 *  actual functions that run the game, different invocations of the same
 *  internal command (calling different external commands) will result in the
 *  function associated with the underlying internal command being given
 *  different messages. So, the logic can be the same but the user will see
 *  different outputs for different external commands.
 *
 *  - It allows for game internationalization. By avoiding to store strings
 *  directly in code, the game may be translated at any point.
 *
 *
 * Files must conform to this standard (for an example see the assets/cmd.txt):
 *
 * 2            # number of external commands that follow.
 *
 * b            # the string the user must type to invoke this command
 * build        # the name of the internal command that you'll use to link a function
 * 2            # number of messages supported by this command, the order will be kept
 * Message 1    # the first message
 * Message 2    # the second message
 *
 * # a new external command would follow
 */
CoP *cop_new(FILE *cmdfile);

/*
 * Frees all resources allocated by cop_new.
 */
void cop_destroy(CoP *c);

/*
 * Links an internal comand to a C function. The function will get called
 * whenever a user types the name of an external command which references the
 * internal function you are linking. The function must be of type CoP_fun
 * defined above and will receive the external command that was executed in
 * addition to the messages on that external command.
 *
 * Returns UINT_ERROR on error, something else on success.
 */
int cop_assoc(CoP *c, const char *name, CoP_fun f);

/*
 * Defines an internal command to be the error catcher. This internal command
 * will get called whenever the cop recieves an input which is not defined on
 * the command file.
 */
int cop_set_error_cmd(CoP *c, const char *cmd);


/*
 * Finds the correct command to execute given the name (cmd) of an external
 * command. Will execute the error command in case there is no external command
 * with name <cmd>. If no error command was set it will return UINT_ERROR.
 * Otherwise it will return the value of the function associated to the
 * internal command that was called.
 */
int cop_exec(CoP *c, char *cmd, void *p);


/*
 * Prints a list of all external commands and the internal commands associated
 * with them. Intended for debugging purposes. Receives a stream to write
 * output to.
 */
void cop_print(FILE *s, CoP *c);
#endif
