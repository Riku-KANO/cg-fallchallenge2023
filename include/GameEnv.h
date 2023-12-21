#pragma once

#include <set>
#include <cstring>
#include "global.h"
#include "Drone.h"
#include "Creature.h"
#include "GameLoopInput.h"

class GameEnv
{
public:
  CreatureGroup cg;
  std::set<ID> my_drone_ids;
  std::set<ID> foe_drone_ids;
  DroneGroup my_dg;
  DroneGroup foe_dg;

  int my_score;
  int foe_score;
  HashMap<ID, bool> my_scanned;
  HashMap<ID, bool> foe_scanned;
  HashMap<ID, bool> my_saved;
  HashMap<ID, bool> foe_saved;
  bool my_found[MAX_COLOR][MAX_TYPE];
  bool foe_found[MAX_COLOR][MAX_TYPE];
  Vec<ID> my_new_found;
  Vec<ID> foe_new_found;

  Vec<VisibleCreature> visibles;
  Vec<VisibleCreature> monsters;
  HashMap<ID, bool> observable;

  bool specie_first[MAX_COLOR][MAX_TYPE];
  bool type0_all;
  bool type1_all;
  bool type2_all;
  bool color0_all;
  bool color1_all;
  bool color2_all;
  bool color3_all;

  bool type0_all_first;
  bool type1_all_first;
  bool type2_all_first;
  bool color0_all_first;
  bool color1_all_first;
  bool color2_all_first;
  bool color3_all_first;

  inline void init();
  inline void update(const GameLoopInput &input);
  inline int calc_scanned_fish_estimated_point();

private:
  inline void update_score(const GameLoopInput &input);
  inline void update_found_data(const GameLoopInput &input);
  inline void update_creatures(const GameLoopInput &input);
  inline void update_drones(const GameLoopInput &input);
  inline void update_observable(const GameLoopInput &input);
  inline void update_scanned_data(const GameLoopInput &input);
  inline void update_first_point();
};