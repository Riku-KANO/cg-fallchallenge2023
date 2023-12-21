#pragma once

#include <iostream>
#include <unordered_map>
#include "global.h"
#include "geometry.h"

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
    cin >> n_creature;
    cin.ignore();
    ids.resize(n_creature);
    for (ID &creature_id : ids) {
      Creature creature;
      cin >> creature_id;
      cin >> creature.color >> creature.type;
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

private:
};
