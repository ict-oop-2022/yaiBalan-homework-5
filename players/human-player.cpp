#include "players/human-player.h"

std::pair<int, int> human_player_t::make_move(const field_t &my_field, const field_t &enemy_field) {
  output_stream << "Your field:" << std::endl;
  output_stream << my_field << std::endl;
  output_stream << "Enemy's field" << std::endl;
  output_stream << enemy_field << std::endl;
  output_stream << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;

  std::string s;
  while (true) {
    std::getline(input_stream, s);
    int x = -1;
    int y = -1;

    if (s.size() == 2 || s.size() == 3) {
      std::string left_part = s.substr(0, s.size() - 1);
      std::string right_part = s.substr(s.size() - 1);

      if (left_part.size() == 2 && left_part == "10") {
        x = 9;
      } else if (left_part.size() == 1 && isdigit(left_part[0])) {
        x = std::atoi(&left_part[0]) - 1;
      }
      if (right_part.size() == 1 && right_part[0] >= 'A' && right_part[0] <= 'J') {
        y = right_part[0] - 'A';
      }
    }
    if (x == -1 || y == -1) {
      output_stream << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;
    } else {
      std::pair<int, int> pair(x, y);
      return pair;
    }
  }

}

void human_player_t::on_incorrect_move(int x, int y) {
  assert(false);
}
void human_player_t::on_duplicate_move(int x, int y) {
  char column = y +'A';
  output_stream << x+1 << column << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int x, int y) {
  char column = y +'A';
  output_stream << x+1 << column << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int x, int y) {
  char column = y +'A';
  output_stream << x+1 << column << " HIT! Your next move!" << std::endl;
}
void human_player_t::on_kill(int x, int y) {
  char column = y +'A';
  output_stream << x+1 << column << " KILL! Your next move!" << std::endl;
}

void human_player_t::on_win() {
  output_stream << "You WIN!" << std::endl;
}
void human_player_t::on_lose() {
  output_stream << "You LOSE!" << std::endl;
}