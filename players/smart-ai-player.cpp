#include "players/smart-ai-player.h"

std::pair<int, int> smart_ai_player_t::make_move(const field_t &my_field, const field_t &_enemy_field_field) {
  if (next_move.size() == 0) {
    for (int i = 0; i < field_t::FIELD_SIZE; i++) {
      for (int j = 0; j < field_t::FIELD_SIZE; j++) {
        if (_enemy_field[i][j] == field_t::EMPTY_CELL) {
          std::pair<int, int> pair(i, j);
          return pair;
        }
      }
    }
  } else {
    std::pair<int, int> next = next_move[next_move.size()-1];
    next_move.pop_back();
    return next;
  }
}

void smart_ai_player_t::on_incorrect_move(int x, int y) {}

void smart_ai_player_t::on_duplicate_move(int x, int y) {}

void smart_ai_player_t::on_miss(int x, int y) {
  _enemy_field[x][y] = field_t::MISS_CELL;
}

void smart_ai_player_t::on_hit(int x, int y) {
  _enemy_field[x][y] = field_t::HIT_CELL;

  // добавим в next_move все подходящие направления
  for (int i = 0; i < field_t::DIRECTIONS.size(); i++) {
    int shift_x = field_t::DIRECTIONS[i].first;
    int shift_y = field_t::DIRECTIONS[i].second;
    if (x + shift_x >= 0 && x + shift_x < field_t::FIELD_SIZE) {
      if (y + shift_y >= 0 && y + shift_y < field_t::FIELD_SIZE) {
        next_move.push_back({x + shift_x, y + shift_y});
      }
    }
  }
}

void smart_ai_player_t::on_kill(int x, int y) {
  _enemy_field[x][y] = field_t::HIT_CELL;
  next_move.clear();

  // пометим все вокруг корабля так, чтобы не стрелять туда
  // вверху
  for(int i = 1; x-i >=0; i++) {
    if (_enemy_field[x-i][y] == field_t::MISS_CELL || _enemy_field[x-i][y] == field_t::EMPTY_CELL) {
      _enemy_field[x-i][y] = field_t::MISS_CELL;
      break;
    }
  }
  // внизу
  for(int i = 1; x+i < field_t::FIELD_SIZE; i++) {
    if (_enemy_field[x+i][y] == field_t::MISS_CELL || _enemy_field[x+i][y] == field_t::EMPTY_CELL) {
      _enemy_field[x+i][y] = field_t::MISS_CELL;
      break;
    }
  }
  // слева
  for(int i = 1; y-i >= 0; i++) {
    if (_enemy_field[x][y-i] == field_t::MISS_CELL || _enemy_field[x][y-i] == field_t::EMPTY_CELL) {
      _enemy_field[x][y-i] = field_t::MISS_CELL;
      break;
    }
  }
  // справа
  for(int i = 1; y+i < field_t::FIELD_SIZE; i++) {
    if (_enemy_field[x][y+i] == field_t::MISS_CELL || _enemy_field[x][y+i] == field_t::EMPTY_CELL) {
      _enemy_field[x][y+i] = field_t::MISS_CELL;
      break;
    }
  }
};

void smart_ai_player_t::on_win() {}

void smart_ai_player_t::on_lose() {}