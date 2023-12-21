#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "global.h"
#include "geometry.h"

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

  inline void read_input() {
    cin >> pos.x >> pos.y >> emergency >> battery;
    if (emergency && message == "") {
      int r = mt() % 5;
      if (r == 0)
        message = "😇😇😇";
      else if (r == 1)
        message = "🥹🥹🥹";
      else if (r == 2)
        message = "😱😱😱";
      else if (r == 3)
        message = "🤬🤬🤬";
      else if (r == 4)
        message = "😵‍💫😵‍💫😵‍💫";
    } else if (emergency == 0) {
      message = "";
    }
  }
};

class DroneGroup {
public:
  Vec<ID> ids;
  HashMap<ID, Drone> drones;

private:
};