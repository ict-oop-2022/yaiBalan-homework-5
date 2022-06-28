#include "game/sea-battle.h"

sea_battle_t::sea_battle_t(std::shared_ptr<player_interface_t> player1, field_t field1, std::shared_ptr<player_interface_t> player2,
                           field_t field2) : _field1(field1), _field2(field2) {
  _player1 = player1;
  _player2 = player2;
}

sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}

void sea_battle_t::play() {
  turn_t current_turn = FIRST_PLAYER;

  std::pair<int, int> move;
  std::shared_ptr<player_interface_t> current_player = _player1, enemy_player = _player2;
  field_t copy_enemy(0), current_field(_field1), enemy_field(_field2);

  // скопируем поле врага и закрасим корабли
  for (int i = 0; i < field_t::FIELD_SIZE; i++) {
    for (int j = 0; j < field_t::FIELD_SIZE; j++) {
      if (enemy_field[i][j] == field_t::SHIP_CELL) {
        copy_enemy[i][j] = field_t::EMPTY_CELL;
      } else {
        copy_enemy[i][j] = enemy_field[i][j];
      }
    }
  }

  while (true) {
    move = current_player->make_move(current_field, copy_enemy);
    int x = move.first, y = move.second;

    if (!field_t::is_cell_valid(x, y)) {
      current_player->on_incorrect_move(x, y);
    } else if (enemy_field[x][y] == field_t::HIT_CELL) {
      current_player->on_duplicate_move(x, y);
    } else if (enemy_field[x][y] == field_t::EMPTY_CELL) {
      enemy_field[x][y] = field_t::MISS_CELL;
      current_player->on_miss(x, y);
      current_turn = change_turn(current_turn);
    }
    else if (enemy_field[x][y] == field_t::SHIP_CELL) {
      enemy_field[x][y] = field_t::HIT_CELL;
      // проверим, последняя ли это живая клетка
      bool isLast = false;

      // вверх
      for(int i = 1; x-i >=0; i++) {
        if (enemy_field[x-i][y] == field_t::SHIP_CELL) {
          isLast = true;
        }
        if (enemy_field[x-i][y] == field_t::EMPTY_CELL || enemy_field[x-i][y] == field_t::MISS_CELL) {
          break;
        }
      }
      // вниз
      if (!isLast) {
        for (int i = 1; x + i < field_t::FIELD_SIZE; i++) {
          if (enemy_field[x + i][y] == field_t::SHIP_CELL) {
            isLast = true;
          }
          if (enemy_field[x + i][y] == field_t::EMPTY_CELL || enemy_field[x + i][y] == field_t::MISS_CELL) {
            break;
          }
        }
      }

      // направо
      if (!isLast) {
        for (int i = 1;; i++) {
          if (!(y + i < field_t::FIELD_SIZE)) {
            break;
          }
          if (y + i < field_t::FIELD_SIZE) {
            if (enemy_field[x][y + i] == field_t::SHIP_CELL) {
              isLast = true;
            }
            if (enemy_field[x][y + i] == field_t::EMPTY_CELL || enemy_field[x][y + i] == field_t::MISS_CELL) {
              break;
            }
          }
        }
      }
      // налево
      if (!isLast) {
        for (int i = 1;; i++) {
          if (!(y - i >= 0)) {
            break;
          }
          if (y - i >= 0) {
            if (enemy_field[x][y - i] == field_t::SHIP_CELL) {
              isLast = true;
            }
            if (enemy_field[x][y - i] == field_t::EMPTY_CELL || enemy_field[x][y - i] == field_t::MISS_CELL) {
              break;
            }
          }
        }
      }

      if (isLast) {
        current_player->on_hit(x, y);
      } else {
        current_player->on_kill(x, y);

        // проверим на победу
        bool won = true;
        for (int i = 0; i < field_t::FIELD_SIZE; i++) {
          for (int j = 0; j < field_t::FIELD_SIZE; j++) {
            if (enemy_field[i][j] == field_t::SHIP_CELL) {
              won = false;
            }
          }
        }
        if (won == true) {
          current_player->on_win();
          enemy_player->on_lose();
          return;
        }
      }
    }

    if (current_turn == FIRST_PLAYER) {
      if (current_player == _player2) {
        field_t tmp = current_field;
        current_field = enemy_field;
        enemy_field = tmp;
      }

      current_player = _player1;
      enemy_player = _player2;
      copy_enemy = enemy_field;
    } else {
      if (current_player == _player1) {
        field_t tmp = current_field;
        current_field = enemy_field;
        enemy_field = tmp;
      }

      current_player = _player2;
      enemy_player = _player1;
      copy_enemy = enemy_field;
    }

    for (int i = 0; i < field_t::FIELD_SIZE; i++) {
      for (int j = 0; j < field_t::FIELD_SIZE; j++) {
        if (enemy_field[i][j] == field_t::SHIP_CELL) {
          copy_enemy[i][j] = field_t::EMPTY_CELL;
        } else {
          copy_enemy[i][j] = enemy_field[i][j];
        }
      }
    }
  }
}