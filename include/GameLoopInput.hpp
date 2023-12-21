#pragma once

#include <iostream>
#include <vector>
#include "global.hpp"
#include "Drone.hpp"
#include "Creature.hpp"


struct GameLoopInput {
  int my_score, foe_score;

  int my_saved_count, foe_saved_count;
  Vec<ID> my_saved_creature_ids, foe_saved_creature_ids;

  int my_drone_count, foe_drone_count;
  Vec<Drone> my_drones, foe_drones;

  int my_scan_count;
  Vec<std::pair<ID, ID>> scanned_drone_creatures;

  int visible_count;
  Vec<VisibleCreature> visibles;

  int radar_blip_count;
  Vec<Radar> radars;

  inline void init() {
    my_saved_creature_ids.clear();
    foe_saved_creature_ids.clear();
    my_drones.clear();
    foe_drones.clear();
    scanned_drone_creatures.clear();
    visibles.clear();
    radars.clear();
  }

  inline void read_input() {
    // read score;
    read_score();

    // read saved data
    read_saved_data();

    // read drones
    read_drones();

    // read scanned data
    read_scanned_data();

    // read visible creatures
    read_visibles();

    // read data
    read_radars();

    if (DEBUG) {
      LOG_INFO("GAME LOOP INPUT DONE");
    }
  }

private:
  inline void read_score() { std::cin >> my_score >> foe_score; }

  inline void read_saved_data() {
    std::cin >> my_saved_count;
    my_saved_creature_ids.resize(my_saved_count);
    for (ID &creature_id : my_saved_creature_ids) {
      std::cin >> creature_id;
    }

    std::cin >> foe_saved_count;
    foe_saved_creature_ids.resize(foe_saved_count);
    for (ID &creature_id : foe_saved_creature_ids) {
      std::cin >> creature_id;
    }
  }

  inline void read_drones() {
    std::cin >> my_drone_count;
    my_drones.resize(my_drone_count);
    for (Drone &drone : my_drones) {
      std::cin >> drone.id >> drone.pos.x >> drone.pos.y >> drone.emergency >>
          drone.battery;
      if (DEBUG) {
        LOG_INFO(
            "Rick's drone %2d position: (%5d,%5d), battery: %2d, emergency: %d",
            drone.id, drone.pos.x, drone.pos.y, drone.battery, drone.emergency);
      }
    }
    std::cin >> foe_drone_count;
    foe_drones.resize(foe_drone_count);
    for (Drone &drone : foe_drones) {
      std::cin >> drone.id >> drone.pos.x >> drone.pos.y >> drone.emergency >>
          drone.battery;
      if (DEBUG) {
        LOG_INFO(
            "Foe's drone %2d position: (%5d,%5d), battery: %2d, emergency: %d",
            drone.id, drone.pos.x, drone.pos.y, drone.battery, drone.emergency);
      }
    }
  }

  inline void read_scanned_data() {
    std::cin >> my_scan_count;
    scanned_drone_creatures.resize(my_scan_count);
    for (std::pair<ID, ID> &id_pair : scanned_drone_creatures) {
      std::cin >> id_pair.first >> id_pair.second;
      if (DEBUG) {
        LOG_INFO("My Drone %2d scanned creature %d", id_pair.first,
                 id_pair.second);
      }
    }
  }

  inline void read_visibles() {
    std::cin >> visible_count;
    visibles.resize(visible_count);
    for (VisibleCreature &visible : visibles) {
      std::cin >> visible.id >> visible.pos.x >> visible.pos.y >> visible.vx >>
          visible.vy;
    }
  }

  inline void read_radars() {
    std::cin >> radar_blip_count;
    radars.resize(radar_blip_count);
    for (Radar &radar : radars) {
      std::cin >> radar.drone_id >> radar.creature_id >> radar.radar;
    }
  }
};