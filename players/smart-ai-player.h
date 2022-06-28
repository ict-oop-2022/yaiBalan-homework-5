#pragma once

#include "players/player-interface.h"

class smart_ai_player_t : public player_interface_t {
public:
  smart_ai_player_t() : _enemy_field(0) {
    for (int i = 0; i < field_t::FIELD_SIZE; i++) {
      for (int j= 0;j < field_t::FIELD_SIZE; j++) {
        _enemy_field[i][j] = field_t::EMPTY_CELL;
      }
    }
  }

  std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field);
  void on_incorrect_move(int x, int y);
  void on_duplicate_move(int x, int y);
  void on_miss(int x, int y);
  void on_hit(int x, int y);
  void on_kill(int x, int y);

  void on_win();
  void on_lose();

private:
  field_t _enemy_field;
  std::vector<std::pair<int, int>> next_move;
};