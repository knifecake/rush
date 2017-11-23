#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

/*
 * Game Controller
 *
 * This file contains the functions that bind user commands to entity
 * functions. These functions are associated to the CoP. When the user types
 * the correct command one of these functions will get called. These are
 * responsible for updating the UI, asking for further input if necessary and
 * finally updating the entity to reflect the new changes.
 *
 * For instance, take the action of constructing a new building in a tile. The action would follow like this:
 *  1. The user moves to a tile using the arrow keys.
 *  2. The user presses 'b' for build. The cop identifies that this is a known
 *  command and calls the corresponging function defined in this file, which
 *  was associated to the cop beforehand.
 *  3. The function receives a pointer to the world object from where this it
 *  should get all information needed to carry out the action. In addition it
 *  receives the command that called it as well as a list of messages defined
 *  in the cmd.txt file. This additional infor is probably useles when working
 *  in GUI-mode buy may come in handfull when using text mode. In particular
 *  this function would probably do as
 *  follows:
 *      3.1. Check if it's ok to build on that tile: by calling Tile-functions
 *      like tile_is_conquered or sth like that – never by directly touching
 *      the Tile in question (not that it can in any case).
 *      3.2. Get a list of available buildings from World by calling
 *      world_get_building_list(w) for example. This function should probably
 *      return a pointer only.
 *      3.3. Display that list to the user to choose from by using a UI-library
 *      function like ui_list(...). This function now has control over input
 *      and keeps it until the user chooses an option or decides to exit. The
 *      function will return the selected option in some way.
 *      3.4. Control is given back to the function defined in this file. Now
 *      the tile_build() function is called to reflect whatever whatever
 *      changes the user has requested. Also, player resources may need to be
 *      updated to reflect the cost of building (decrementing money, etc...).
 *      3.5. Changes are reflected by updating the UI: this function is
 *      responsible for deciding which part of the ui is to be redrawn and to
 *      call the corresponding UI-library function to do it. In this example,
 *      the tile might need to be redrawn to reflect the fact that it now has a
 *      new building. If player resources or other non-action especific
 *      information has changed and it needs to be reflected on the UI this
 *      function should either do it or signal it to the main game loop in a
 *      way which we have not decided.
 *      3.6. OPTIONAL: the result is announced by calling show_msg("A new
 *      building was constructed").
 *
 *  4. Control is now given back to the main game loop. If no more actions can
 *  be carried out during this turn the turn is finished by calling
 *  world_next_turn.
 *
 *  To be compatible with the CoP all function on this file must share the same
 *  type: CoP_fun defined in lib/cop.h, that is, int
 *  action_name(void *world, char *cmd, char
 *  **msg_list, int num_msg);
 */
int action_build(void *world, char *cmd, char **msg, int num_msg);


int action_generic(void *w, char *cmd, char **msg, int num_msg);

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg);
#endif
