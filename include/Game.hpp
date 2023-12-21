#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "GameEnv.hpp"
#include "ScoreManager.hpp"
#include "Property.hpp"
#include "Player.hpp"
#include "global.hpp"

class Game {
public:
  inline void init(Property props);
  inline void run();

private:
  std::shared_ptr<GameEnv> env;
  GameLoopInput game_loop_input;
  ScoreManager score_manager;
  Player my;
  inline void read_game_loop_input();
};
