#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include "GameEnv.hpp"
#include "Property.hpp"
#include "global.hpp"
#include "geometry.hpp"

class Player
{
public:
  std::string name;

  Player() {}
  Player(std::string _name) : name(_name) {}
  inline void init(const Property &props);
  inline void update(GameEnv &ge, const GameLoopInput &input);
  inline Point define_move_by_target_rectangle(ID drone_id);
  inline Vec<std::string> suggest_actions(int turn, GameEnv &ge);

private:
  HashMap<ID, Rectangle> creature_area;
  HashMap<ID, std::set<ID>> drone_scanned;
  HashMap<ID, HashMap<ID, std::string>> drone_radar;
  HashMap<ID, int> drone_current_target;
  Vec<Vector2d<int>> dxys;
  bool judge_collide(Point drone_pos, Vector2d<int> drone_v, Point ugly_pos,
                     Vector2d<int> ugly_v) const;
};