/*
 * PPROG Game – Morzilla Firefox 2018
 *
 * Authors: Miguel Baquedano, Sergio Cordero, Elias Hernandis
 *          and Rafael Sánchez.
 *
 * Lead author: <replace me>
 */


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

#define COFFEE_SKILL_ID 1
#define COFFEE_SKILL_PROP 0.5

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

int action_quit(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return CTRL_ERROR;
    }

    if (show_dialogue(msg[0])) {
        show_msg(msg[1]);
        return CTRL_OK;
    }

    show_msg(msg[2]);
    return CTRL_ERROR;
}

int action_build(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_build");
        return UINT_ERROR;
    }
    // get the current tile index
    int tile_index = ui_get_cursor();

    if (!tile_get_visible(world_tile_at_index(w, tile_index))) {
        show_msg(msg[8]);
        return CTRL_OK;
    }

    if (0 < tile_get_enemies(world_tile_at_index(w, tile_index))) {
        show_msg(msg[10]);
        return CTRL_OK;
    }

    show_msg(msg[0]);

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
        case WORLD_BUILD_TOOMANY_TOWNHALLS:
            show_msg(msg[11]);
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

    if (!tile_get_building(world_tile_at_index(w, ui_get_cursor()))) {
        show_msg(msg[0]);
        return CTRL_OK;
    }

    int result = world_upgrade_building(w, ui_get_cursor());
    switch (result) {
      case WORLD_UPGRADE_MAX_LEVEL:
          show_msg(msg[1]);
          break;
      case WORLD_BUILD_SUCCESS_LEVEL_UP:
          show_msg(msg[2]);
          break;
      case WORLD_BUILD_NO_LEVEL:
          show_msg(msg[3]);
          break;
      case WORLD_BUILD_NO_MONEY:
          show_msg(msg[4]);
          break;
      case WORLD_BUILD_SUCCESS:
          show_msg(msg[5]);
          break;
      default:
          show_msg(msg[6]);
          return CTRL_ERROR;
    }
    ui_redraw_tile(ui_get_cursor());
    ui_redraw_sidebar();
    return CTRL_NEXT_TURN;
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

  int building_level = building_get_level(tile_get_building(world_tile_at_index(w, tile_index)));
  price = ui_control_exchange_panel(&res_id, building_level);
  if(price == UINT_ERROR){
    show_msg(msg[5]);
    ui_redraw_sidebar();
    return CTRL_OK;
  }
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
int action_hack(void *w, char *cmd, char **msg, int num_msg){
  if (!w || !cmd || !msg || num_msg < 1) {
      HE("invalid parameters", "action_build");
      return UINT_ERROR;
  }
  int tile_index = ui_get_cursor();
  int price, res_id;
  show_msg(msg[0]);
  if(WORLD_CODING_NOT_POSSIBLE == world_hack(w, tile_index, 0, 0, 0)){
    show_msg(msg[1]);
    ui_redraw_sidebar();
    return CTRL_OK;
  }
  int building_level = building_get_level(tile_get_building(world_tile_at_index(w, tile_index)));
  int res_from;
  price = ui_control_code_panel(&res_from, &res_id, building_level);
  if(price == UINT_ERROR){
    show_msg(msg[5]);
    ui_redraw_sidebar();
    return CTRL_OK;
  }
  int result = world_hack(w, tile_index, price, res_from, res_id);
  switch (result) {
    case WORLD_CODING_NO_ECTS:
      show_msg(msg[2]);
      break;
    case WORLD_CODING_DONE:
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

int action_main_screen(void *w, char *cmd, char **msg, int num_msg){
  if(!cmd || !msg || num_msg < 0) return CTRL_ERROR;
  int option = 2;
  while(option == 2){
    option = ui_display_main_screen();
    switch (option) {
      case 0: return CTRL_OK;
      case 1:
      config_set("general.saved_game", "saved_game.txt");
      return CTRL_OK;
      case 2:
      sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "morcilla_firefox"), 0, 0);
      term_read_key(stdin);
      break;
      default: return CTRL_ERROR;
    }

  }
  return CTRL_ERROR;
}

int action_next_turn(void *world, char *cmd, char **msg, int num_msg)
{
    int ntiles = world_get_num_tiles(world);
    int *tiles_to_update = oopsalloc(ntiles, sizeof(int), "action_next_turn");
    if (!world_next_turn(world, tiles_to_update))
        return CTRL_ERROR;
    ui_redraw_layers();

    int score;
    if(0 < (score = world_game_finished(world))) {
        show_msg("The game ended. Press any key to continue.");
        term_read_key(stdin);
        return CTRL_GAME_ENDED;
    }
    ui_redraw_sidebar();
    return CTRL_OK;
}

int action_end_screen(void *w, char *cmd, char **msg, int num_msg)
{
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "game_over"), 0, 0);
    int score = world_game_finished(w);
    if (score < config_get_int("general.winning_threshold")) {
        show_msg("You lost! Score: %d", score);
    }
    else {
        show_msg("You won! Score: %d", score);
    }

    term_read_key(stdin);

    return !UINT_ERROR;
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
        if (_game_state != NOT_ENDED) {
            return NULL;
        }
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

    show_msg(msg[7], soldiers);
    term_read_key(stdin);

    printf("\033[2J");
    FILE *enemy_sprite_file = fopen(config_get("attack_minigame.enemy_sprite"), "r");
    Sprite *enemy_sprite = sprite_new(enemy_sprite_file);
    fclose(enemy_sprite_file);

    FILE *player_sprite_file = fopen(config_get("attack_minigame.player_sprite"), "r");
    Sprite *player_sprite = sprite_new(player_sprite_file);
    fclose(player_sprite_file);

    // TODO: tie this to game config
    sprite_draw(stdout, dict_get(ui_get_sprite_dict(), "ui-attack"), 0, 0);
    UIRect gru_dim = (UIRect) {0, 19, 320, 145};
    SKGru *g = sk_gru_new(gru_dim, ' ');

    // create the player
    UIRect player_box = (UIRect) {1, 17, 340, 148 };

    // if the player has a special skill, enable jumping over the box
    if (world_get_skill(world, config_get_int("attack_minigame.skill_num")))
        player_box.h += 40;

    _game_player = sk_minion_new(
            (UIRect) { 1, 19, sprite_get_w(player_sprite), sprite_get_h(player_sprite) },
            'P', 'P', (SKVector) { 0 }, player_sprite, &player_box);
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
                enemy_sprite, NULL);

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

        int ncode = world_get_resource_quantity(world, config_get_int("attack_minigame.speed_resource"));
        int speed = ((int)(2 + ncode/5) > 12) ? 12 : (int)(2 + ncode/5);
        switch (key) {
            case 't':
                goto win;
            case 's':
            case 'q':
                goto surrender;
            case UP_ARROW:
                sk_minion_update_position(_game_player, 0, -speed);
                break;
            case DOWN_ARROW:
                sk_minion_update_position(_game_player, 0, speed);
                break;
            case LEFT_ARROW:
                sk_minion_update_position(_game_player, -2*speed, 0);
                break;
            case RIGHT_ARROW:
                sk_minion_update_position(_game_player, 2*speed, 0);
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
    world_wallet_delta(world, config_get_int("attack_minigame.soldier_resource"), -1);
    goto finish;

surrender:
    world_wallet_delta(world, config_get_int("attack_minigame.soldier_resource"), -1);
    output = msg[6];

finish:
    pthread_cancel(thr);
    sk_gru_destroy(g);
    ui_draw_all();
    show_msg(output);
    while (term_is_arrow_key(term_read_key(stdin)));
    show_msg("");
    return CTRL_OK;
}
int action_repair(void *w, char *cmd, char **msg, int num_msg){
  if (!w || !cmd || !msg || num_msg < 1) {
      HE("invalid parameters", "action_repair");
      return UINT_ERROR;
  }
  int tile_index=ui_get_cursor();
  Tile *t=world_tile_at_index(w,tile_index);
  Building *b=tile_get_building(t);

  if(!b){
    show_msg(msg[0]);
    return CTRL_OK;
  }

  int b_initial_health=building_get_health(b);

  int cost=world_repair_building(w,b);
  int *wallet=world_get_wallet(w);

  if(wallet[2]<cost){
    show_msg(msg[1]);
    building_set_health(b,b_initial_health);

    return CTRL_OK;
  }
  if(world_get_skill(w, COFFEE_SKILL_ID)){
    world_wallet_delta(w,2,-cost*COFFEE_SKILL_PROP);
  }else{
    world_wallet_delta(w, 2, -cost);
  }

  show_msg(msg[2]);

  ui_redraw_sidebar();

  return CTRL_OK;
}

int action_save_game(void *w, char *cmd, char **msg, int num_msg)
{
    if (UINT_ERROR == world_save_game(w, "saved_game.txt"))
        show_msg(msg[1]);
    else
        show_msg(msg[0]);

    return CTRL_OK;
}
int action_redraw_ui(void *w, char *cmd, char **msg, int num_msg)
{
    return CTRL_REDRAW_ALL_UI;
}

int action_generic(void *w, char *cmd, char **msg, int num_msg)
{
    if (!w || !cmd || !msg || num_msg < 1) {
        HE("invalid parameters", "action_generic");
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
