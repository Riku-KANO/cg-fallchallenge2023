#pragma once

#include <set>
#include <cstring>
#include "global.hpp"
#include "Drone.hpp"
#include "Creature.hpp"
#include "GameLoopInput.hpp"

class GameEnv
{
public:
  CreatureGroup cg;
  DroneGroup dg;

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
  inline bool is_specie_found_first(int color, int type) const ;

private:
  inline void update_score(const GameLoopInput &input);
  inline void update_found_data(const GameLoopInput &input);
  inline void update_creatures(const GameLoopInput &input);
  inline void update_drones(const GameLoopInput &input);
  inline void update_observable(const GameLoopInput &input);
  inline void update_scanned_data(const GameLoopInput &input);
  inline void update_first_point();
};