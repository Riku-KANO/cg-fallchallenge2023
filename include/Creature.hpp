#pragma once

#include <iostream>
#include <unordered_map>
#include "global.hpp"
#include "geometry.hpp"

struct Creature {
  ID id;
  int color;
  int type;
};

struct VisibleCreature {
  ID id;
  Point pos;
  int vx, vy;
};

class CreatureGroup {
public:
  Vec<ID> ids;
  HashMap<ID, Creature> creatures;

  inline void read_input() {
    int n_creature;
    std::cin >> n_creature;
    ids.resize(n_creature);
    for (ID &creature_id : ids) {
      Creature creature;
      std::cin >> creature_id;
      std::cin >> creature.color >> creature.type;
      creatures[creature_id] = creature;
      if (DEBUG) {
        LOG_INFO("Creature %2d.. color: %d, type: %d", creature_id,
                 creatures[creature_id].color, creatures[creature_id].type);
      }
    }
  }

  inline bool is_monster(ID cid) { 
    return creatures[cid].color == -1;
  }

  inline bool is_away(ID cid) {
    return contain_value(&ids, cid);
  }


private:
};
