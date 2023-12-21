#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "global.hpp"
#include "geometry.hpp"

struct Radar {
  ID drone_id;
  ID creature_id;
  std::string radar;
};

struct Drone {
  ID id;
  Point pos;
  int emergency;
  int battery;
  std::string message;
};

class DroneGroup {
public:
  Vec<ID> my_ids;
  Vec<ID> foe_ids;
  HashMap<ID, Drone> drones;
private:
};