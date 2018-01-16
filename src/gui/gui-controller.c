#include "../controller.h"

#include "../ui.h"

#include "../lib/error_handling.h"
#include "../lib/messages.h"
#include "../lib/terminal.h"
#include "../lib/config.h"

#include "../entities/world.h"
#include "../entities/tile.h"

#include "../../lib/qrnd.h"

#include <unistd.h>
#include <pthread.h>
#include <time.h>

// TODO: this is a temporary function until building sprites are done
/*Sprite *get_placeholder_sprite(Building *b)
{
    char temporary[256]={'\0'};
    sprintf(temporary,"%s",building_get_sprite(b));
    int i = 0;
    while (temporary[i] != '-') {
      ++i;
    }
    temporary[i]='I';++i;
    temporary[i]='C';++i;
    temporary[i]='O';++i;
    temporary[i]='N';++i;
    temporary[i]='\0';++i;
    return dict_get(ui_get_sprite_dict(), temporary);
}*/

//TODO:modify the sprintf so that it includes all the resources
char *building_get_desc(Building *b)
{
    if (!b) {
        HE("invalid arguments", "building_get_desc");
        return NULL;
    }

    char *buff = oopsalloc(100, sizeof(char), "building_get_desc");

    sprintf(buff, "%s, level %d ($%d)", building_get_name(b), building_get_level(b), building_get_cost(b, 0));
    return buff;
}

int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }
    // get the current tile index
    int tile_index = ui_get_cursor();

    // get a list of available buildings for that tile
    //Building **bs = world_get_buildings(w);

    /*// assemble a list that is compatible with the UI-library
    UIList *ui_l = ui_list_new((void *)bs,
            world_get_num_buildings(w),
            ui_get_top_sidebar_dim(),
            (ui_get_li_string_fun)building_get_desc,
            (ui_get_li_sprite_fun)get_placeholder_sprite);*/

    show_msg(msg[0]);

    // display the list: passes control to the uilist, will return a pointer to the list item that was chosen
    //Building *b = ui_list_present(ui_l);
    Building *b = ui_control_build_panel();

    if (!b) {
        show_msg(msg[1]);
        ui_redraw_sidebar();
        return CTRL_OK;
    }

    // update the model (entity) to reflect the changes that took place
    int result = world_build_on_tile(w, tile_index, b);

    // communicate the result to the user and update neighbours
    switch(result) {
        case WORLD_BUILD_SUCCESS_LEVEL_UP:
            show_msg(msg[2]);
            world_update_neighbours(w, tile_index);
            break;
        case WORLD_BUILD_SUCCESS:
            show_msg(msg[3]);
            world_update_neighbours(w, tile_index);
            break;
        case WORLD_BUILD_NO_LEVEL:
            show_msg(msg[4]);
            break;
        case WORLD_BUILD_NO_MONEY:
            show_msg(msg[5]);
            break;
        case WORLD_BUILD_OCCUPIED:
            show_msg(msg[6]);
            break;
        case WORLD_BUILD_SUCCESS_UPGRADE:
            show_msg(msg[7]);
            break;
        case WORLD_BUILD_NO_LIGHT:
            show_msg(msg[8]);
            break;
        case WORLD_BUILD_ENEMIES_PRESENT:
            show_msg(msg[10]);
            break;
        default:
            show_msg(msg[9]);
            ui_redraw_sidebar();
            return CTRL_ERROR;
    }

    // redraw the current tile. TODO: think about who should do this
    ui_redraw_tile(tile_index);
    switch (result) {
      case WORLD_BUILD_SUCCESS_LEVEL_UP:
      case WORLD_BUILD_SUCCESS:
        ui_redraw_neighbours(tile_index);
        break;
      default:
        break;
    }

    // TODO: think about if signaling a common UI redraw from the return value is a good idea
    ui_redraw_sidebar();
    return CTRL_NEXT_TURN;
}

int action_upgrade(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }

    // TODO: move messages to the command file
    int result = world_upgrade_building(w, ui_get_cursor());
    switch (result) {
      case WORLD_UPGRADE_NO_BUILDING:
          show_msg("There is no building to upgrade");
          break;
      case WORLD_UPGRADE_MAX_LEVEL:
          show_msg("This is currently in its max level");
          break;
      case WORLD_BUILD_SUCCESS_LEVEL_UP:
          show_msg("Building was constructed and you leveled up!");
          break;
      case WORLD_BUILD_NO_LEVEL:
          show_msg("You need to level up before upgrading this. Hint: upgrade townhall.");
          break;
      case WORLD_BUILD_NO_MONEY:
          show_msg("You need to save more to upgrade this.");
          break;
      case WORLD_BUILD_SUCCESS_UPGRADE:
          show_msg("The building on this tile was upgraded to the next level.");
          break;
      case WORLD_BUILD_NO_LIGHT:
          show_msg("The tile is not visible.");
          break;
      default:
          show_msg("An error ocurred and nothing was upgraded.");
          return CTRL_ERROR;
    }
    ui_redraw_tile(ui_get_cursor());
    ui_redraw_sidebar();
    return CTRL_OK;
}
int action_exchange(void *w, char *cmd, char **msg, int num_msg){
  if (!w || !cmd || !msg || num_msg < 1) {
      HE("invalid parameters", "action_build");
      return UINT_ERROR;
  }
  int tile_index = ui_get_cursor(); //TODO: Improve this.
  show_msg(msg[0]);
  int price, res_id;
  if(WORLD_EXCHANGE_NOT_POSSIBLE == world_exchange(w, tile_index, 0, 0, 0)){
    show_msg(msg[1]);
    ui_redraw_sidebar();
    return CTRL_OK;
  }
  price = ui_control_exchange_panel(&res_id);
  int result = world_exchange(w, tile_index, price, 0, res_id);
  switch (result) {
    case WORLD_EXCHANGE_NO_MONEY:
      show_msg(msg[2]);
      break;
    case WORLD_EXCHANGE_DONE:
      show_msg(msg[3]);
      break;
    default:
      show_msg(msg[4]);
      ui_redraw_sidebar();
      return CTRL_ERROR;
  }
  ui_redraw_sidebar();
  return CTRL_OK;
}
int action_welcome(void *world, char *cmd, char **msg, int num_msg)
{
    if (!msg || num_msg < 1)
        return CTRL_ERROR;

    for (int k = 0; k < num_msg; k++) {
        show_msg(msg[k]);

        // if this is not the last message, listen for a keypress to continue
        if (k < num_msg - 1)
            term_read_key(stdin);
    }
    return CTRL_OK;
}

int action_next_turn(void *world, char *cmd, char **msg, int num_msg)
{
    if (!world_next_turn(world))
        return CTRL_ERROR;

    ui_redraw_sidebar();
    return CTRL_OK;
}

typedef enum { ERROR, NOT_ENDED, PLAYER_LOST, PLAYER_WON } _game_state_t;

static _game_state_t _game_state;

static SKMinion *_game_player;

static int _game_speed;

void _game_ended(SKGru *g, SKMinion *player)
{
    if (!g || !player) {
        HE("invalid arguments", "_game_ended");
        _game_state = ERROR;
    }

    UIRect *player_pos = sk_minion_get_dim(player);
    UIRect *gru_pos = sk_gru_get_dim(g);
    if (!player_pos || !gru_pos) {
        HE("could not get player position", "_game_ended");
        _game_state = ERROR;
    }

    if (player_pos->x > gru_pos->x + 2 * gru_pos->w)
        _game_state = PLAYER_WON;

    if (sk_gru_minion_collides_with_others(g, player))
        _game_state = PLAYER_LOST;
}

void *_move_enemies(void *s)
{
    SKGru *g = (SKGru *)s;
    if (!g || !_game_player) {
        HE("invalid arguments or NULL global player", "_move_enemies");
        return NULL;
    }

    while (1) {
        usleep(_game_speed);
        sk_gru_next_frame(g);
        _game_ended(g, _game_player);
        if (_game_state != NOT_ENDED)
            return NULL;
    }
    return NULL;
}

int action_attack(void *world, char *cmd, char **msg, int num_msg)
{

    int tile_index = ui_get_cursor();
    Tile *current_tile = world_tile_at_index(world, tile_index);
    if (!current_tile) {
        HE("error retrieving tile", "action_attack");
        return CTRL_ERROR;
    }

    if (!tile_get_visible(current_tile)) {
        show_msg(msg[1]);
        return CTRL_OK;
    }

    int num_enemies = tile_get_enemies(current_tile);
    if (num_enemies == 0) {
        show_msg(msg[4]);
        return CTRL_OK;
    } else if (num_enemies == -1) {
        show_msg(msg[5]);
        return CTRL_OK;
    }

    int soldiers = world_get_resource_quantity(world, config_get_int("attack_minigame.soldier_resource"));
    if (soldiers < 1) {
        show_msg(msg[0]);
        return CTRL_OK;
    }

    _game_speed = config_get_int("attack_minigame.enemy_speed");
    if (_game_speed == 0) {
        HE("invalid speed set in config file. hitn: 5e4 works right", "move_enemies");
        _game_speed = 5e4;
    }

    printf("\033[2J");
    FILE *enemy_sprite_file = fopen(config_get("attack_minigame.enemy_sprite"), "r");
    Sprite *enemy_sprite = sprite_new(enemy_sprite_file);
    fclose(enemy_sprite_file);

    FILE *player_sprite_file = fopen(config_get("attack_minigame.player_sprite"), "r");
    Sprite *player_sprite = sprite_new(player_sprite_file);
    fclose(player_sprite_file);

    // TODO: tie this to game config
    UIRect gru_dim = (UIRect) {40, 5, 300, 150};
    SKGru *g = sk_gru_new(gru_dim, 'M');

    // create the player
    _game_player = sk_minion_new(
            (UIRect) { 11, 11, sprite_get_w(player_sprite), sprite_get_h(player_sprite) },
            'P', 'P', (SKVector) { 0 }, player_sprite);
    sk_gru_add_minion(g, _game_player);

    // create the enemies
    rnd_state *rs = r_init(time(NULL));
    for (int i = 0; i < num_enemies; i++) {
        // TODO: maybe we can improve here
        int rand_x = 2 + gru_dim.x + f_rnd(rs) * 2 * (gru_dim.w - sprite_get_w(enemy_sprite));
        int rand_y = 2 + gru_dim.y + f_rnd(rs) * (gru_dim.h - sprite_get_h(enemy_sprite));
        if (rand_x % 2 == 0)
            rand_x -= 1;

        int rand_dx = 2 * (f_rnd(rs) * 8 - 4);
        int rand_dy = f_rnd(rs) * 8 - 4;

        SKMinion *e = sk_minion_new(
                (UIRect) {rand_x, rand_y, sprite_get_w(enemy_sprite), sprite_get_h(enemy_sprite)},
                '1', '@',
                (SKVector) { rand_dx, rand_dy},
                enemy_sprite);

        sk_gru_add_minion(g, e);
    }
    r_end(rs);

    sk_gru_draw(g);

    pthread_t thr;
    pthread_create(&thr, NULL, _move_enemies, g);

    _game_state = NOT_ENDED;
    int key;
    while ((key = term_read_key(stdin))) {
        switch (_game_state) {
            case PLAYER_WON:
                goto win;
            case PLAYER_LOST:
                goto loose;
            default:
                break;
        }

        switch (key) {
            case 't':
                goto win;
            case 's':
            case 'q':
                goto surrender;
            case UP_ARROW:
                sk_minion_update_position(_game_player, 0, -2);
                break;
            case DOWN_ARROW:
                sk_minion_update_position(_game_player, 0, 2);
                break;
            case LEFT_ARROW:
                sk_minion_update_position(_game_player, -4, 0);
                break;
            case RIGHT_ARROW:
                sk_minion_update_position(_game_player, 4, 0);
                break;
            default:
                break;
        }
    }

    char *output = NULL;
win:
    // mark the tile as conquered
    tile_conquer(current_tile);
    output = msg[2];
    goto finish;

loose:
    output = msg[3];
    goto finish;

surrender:
    output = msg[6];

finish:
    pthread_cancel(thr);
    sk_gru_destroy(g);
    ui_draw_all();
    show_msg(output);
    return CTRL_OK;
}

int action_redraw_ui(void *w, char *cmd, char **msg, int num_msg)
{
    return CTRL_REDRAW_ALL_UI;
}

int action_generic(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg("You executed %s.\n%s\n", cmd, msg[0]);

    return !UINT_ERROR;
}

int cop_error_cmd(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "cop_demo");
        return UINT_ERROR;
    }

    show_msg(msg[0], cmd);

    return !UINT_ERROR;
}
