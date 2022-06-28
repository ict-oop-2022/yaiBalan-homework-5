#pragma once

#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>

#include "players/player-interface.h"

class human_player_t : public player_interface_t {
public:
  human_player_t(std::istream &input_stream, std::ostream &output_stream)
      : input_stream(input_stream), output_stream(output_stream) {}

  std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field);

  void on_incorrect_move(int x, int y);
  void on_duplicate_move(int x, int y);
  void on_miss(int x, int y);
  void on_hit(int x, int y);
  void on_kill(int x, int y);

  void on_win();
  void on_lose();

private:
  std::istream &input_stream;
  std::ostream &output_stream;
};