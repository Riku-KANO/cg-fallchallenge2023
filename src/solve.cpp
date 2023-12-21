#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

// includes
#include <array>
#include <bitset>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <utility>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define Vec std::vector
#define HashMap std::unordered_map
#define ID int
#define FOR(i, n) for (int i = 0; i < (int)(n); i++)
#define DEBUG 1
#define LOG_INFO(message, ...)                                                 \
  fprintf(stderr, "[INFO] " message "\n", ##__VA_ARGS__)
#define LOG_ERROR(message, ...)                                                \
  fprintf(stderr, "[ERROR] " message "\n", ##__VA_ARGS__)

using std::cin, std::cout;
using ll = long long;

using i8 = char;

using u16 = uint16_t;
using u32 = uint32_t;
using i32 = int32_t;
using i64 = int64_t;
using byte = unsigned int;

constexpr double TL_FIRST = 1.0;
constexpr double TL_TURN = 0.050;
constexpr int MAX_TURN = 200;
constexpr int MAX_WIDTH = 10'000;
constexpr int MAX_HEIGHT = 10'000;
constexpr int MAX_COLOR = 4;
constexpr int MAX_TYPE = 3;

constexpr int INF = 1 << 30;
constexpr long long LINF = 1LL << 60;
const double PI = std::acos(-1);

constexpr int MOVE_UNIT = 600;
constexpr int SINK_UNIT = 300;
constexpr int SCAN0_UNIT = 800;
constexpr int SCAN1_UNIT = 2'000;
constexpr int FRIGHTEN_UNIT = 1'400;
constexpr int AGGRESSIVE_UNIT = 1000;
constexpr int BATTERY_USE_UNIT = 5;
constexpr int RECHARGE_UNIT = 1;
constexpr int BATTERY_CAP = 30;

constexpr int POINT_T0 = 1;
constexpr int POINT_T1 = 2;
constexpr int POINT_T2 = 3;
constexpr int POINT_ALL_COLOR = 3;
constexpr int POINT_ALL_TYPE = 4;

std::random_device seed_gen;
std::mt19937 mt(seed_gen());

double get_time(clock_t start_time) {
  return (double)(clock() - start_time) / CLOCKS_PER_SEC;
}

struct Property {
  std::string todo;
};

struct Point {
  int x, y;

  static ll calc_dist2(const Point &p1, const Point &p2) {
    return (ll)(p1.x - p2.x) * (p1.x - p2.x) +
           (ll)(p1.y - p2.y) * (p1.y - p2.y);
  }
};

template<typename T>
struct Vector2d {
  T dx, dy;
};



struct Rectangle {
  int min_x, max_x;
  int min_y, max_y;
  Rectangle() : min_x(0), max_x(MAX_WIDTH), min_y(0), max_y(MAX_HEIGHT) {}
  int calc_area() { return (max_x - min_x) * (max_y - min_y); }
  Point get_center() { return Point{(min_x + max_x) / 2, (min_y + max_y) / 2};}
};

struct Creature {
  ID id;
  int color;
  int type;
  bool away = false;
};

struct VisibleCreature : public Creature {
  Point pos;
  int vx, vy;

  VisibleCreature() {}
  VisibleCreature(const Creature &creature) {
    this->id = creature.id;
    this->color = creature.color;
    this->type = creature.type;
    this->away = creature.away;
  }

  inline void read_input() {
    this->read_position();
    this->read_velocity();
  }
  inline void read_position() { cin >> pos.x >> pos.y; }
  inline void read_velocity() { cin >> vx >> vy; }
};

class CreatureGroup {
public:
  Vec<ID> ids;
  HashMap<ID, Creature> creatures;

  inline void init() {
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

  inline bool is_away(ID cid) {
    return creatures[cid].away;
  }

private:
};

struct Drone {
  ID id;
  Point pos;
  int emergency;
  int battery;
  HashMap<ID, std::string> rador;
  HashMap<ID, bool> scanned;
  std::string message;

  ID current_target_id = -1;

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

  inline Point define_move_by_current_target() {
    std::string _rador = rador[current_target_id];
    const int MAX_RADOR_SIZE = 2;
    Point ret;

    if (_rador.find('T') < MAX_RADOR_SIZE) {
      ret.y = pos.y - 5000;
    } else {
      ret.y = pos.y + 5000;
    }
    if (_rador.find('L') < MAX_RADOR_SIZE) {
      ret.x = pos.x - 5000;
    } else {
      ret.x = pos.x + 5000;
    }
    return ret;
  }
};

class DroneGroup {
public:
  Vec<ID> ids;
  HashMap<ID, Drone> drones;

private:
};

class Player {
public:
  std::string name;
  std::shared_ptr<CreatureGroup> cg;
  DroneGroup dg;
  Vec<VisibleCreature> visible_creatures;
  Vec<VisibleCreature> monsters;

  HashMap<ID, bool> observable;
  HashMap<ID, Rectangle> creature_area;

  int score;
  HashMap<ID, bool> total_scanned;
  HashMap<ID, bool> saved;
  bool found[MAX_COLOR][MAX_TYPE];

  Player() {}
  Player(std::string _name, std::shared_ptr<CreatureGroup> _cg)
      : name(_name), cg(_cg) {}
  inline void read_score();
  inline void read_saved_data();
  inline void read_drones();
  inline void read_scanned_data();
  inline void read_visible_creatures();
  inline void read_radors();
  inline void init();
  inline void update();
  inline Point define_move_by_target_rectangle(const Drone &drone);
  inline Vec<std::string> suggest_actions(int turn);

private:
  Vec<Vector2d<int>> dxys;
};

void Player::init() {
  memset(found, false, sizeof(found));
  for (ID cid : cg->ids) {
    total_scanned[cid] = false;
    saved[cid] = false;
  }

  const int NUM_SPLIT = 12;
  double radius = (double)MOVE_UNIT;
  for(int i = 0; i < NUM_SPLIT; i++) {
    double theta = 2.0 * PI * i / NUM_SPLIT;
    double dx = radius * std::cos(theta);
    double dy = radius * std::sin(theta);
    dxys.push_back(Vector2d<int>{(int)dx, (int)dy});
  }
}

void Player::update() {
  total_scanned.clear();
  for (ID cid : cg->ids) {
    total_scanned[cid] = false;
  }
  for (ID drone_id : dg.ids) {
    for (auto [cid, flag] : dg.drones[drone_id].scanned) {
      total_scanned[cid] |= flag;
    }
  }

  for (ID drone_id : dg.ids) {
    int target = dg.drones[drone_id].current_target_id;
    if (total_scanned[target] || cg->is_away(target) || saved[target]) {
      dg.drones[drone_id].current_target_id = -1;
      if(DEBUG) {
        LOG_INFO("Drone %2d lost its target %2d...", drone_id, target);
      }
    }
  }

  // calculate creature area
  for (ID cid : cg->ids) {
    if (!observable[cid]) {
      continue;
    }
    Rectangle rect;
    for (ID did : dg.ids) {
      std::string rador = dg.drones[did].rador[cid];
      if (rador == "TL") {
        rect.max_y = std::min(rect.max_y, dg.drones[did].pos.y);
        rect.max_x = std::min(rect.max_x, dg.drones[did].pos.x);
      } else if (rador == "TR") {
        rect.max_y = std::min(rect.max_y, dg.drones[did].pos.y);
        rect.min_x = std::max(rect.min_x, dg.drones[did].pos.x);
      } else if (rador == "BL") {
        rect.min_y = std::max(rect.min_y, dg.drones[did].pos.y);
        rect.max_x = std::min(rect.max_x, dg.drones[did].pos.x);
      } else if (rador == "BR") {
        rect.min_y = std::max(rect.min_y, dg.drones[did].pos.y);
        rect.min_x = std::max(rect.min_x, dg.drones[did].pos.x);
      } else {
        LOG_ERROR("INVALID TYPE at line %d", __LINE__);
      }
    }
    creature_area[cid] = rect;
    if(DEBUG) {
      LOG_INFO("Creature %2d situated in DY:(%d,%d), DX(%d,%d)", cid, rect.min_y, rect.max_y, rect.min_x, rect.max_x);
    }
  }
}

inline Point Player::define_move_by_target_rectangle(const Drone& drone) {
  int target = drone.current_target_id;
    Point ret;
    int center_x =
        (creature_area[target].max_x + creature_area[target].min_x) / 2;
    int center_y =
        (creature_area[target].max_y + creature_area[target].min_y) / 2;
    ret = Point{center_x, center_y};
    return ret;
}

inline Vec<std::string> Player::suggest_actions(int turn) {
  Vec<std::string> commands;
  Vec<Point> monsters_next_pos;
  bool lightOn;

  for(const VisibleCreature& monster: monsters) {
    int x = monster.pos.x;
    int y = monster.pos.y;
    int vx = monster.vx;
    int vy = monster.vy;
    int nx = x + vx;
    int ny = y + vy;
    monsters_next_pos.push_back(Point{nx, ny});
  }

  for (const ID drone_id : dg.ids) {
    Drone &drone = dg.drones[drone_id];
    if(drone.current_target_id == -1) {
      LOG_INFO("find nearest by drone %d", drone_id);
      // fine nearest creature
      int nearest_creature_id = -1;
      ll min_dist2 = LINF;
      for(ID cid: cg->ids) {
        if(!observable[cid] || cg->is_monster(cid) || total_scanned[cid] || saved[cid] || cg->is_away(cid)) {
          continue;
        }
        Point creature_pos = creature_area[cid].get_center();
        ll dist2 = Point::calc_dist2(creature_pos, drone.pos);
        if(dist2 < min_dist2) {
          min_dist2 = dist2;
          nearest_creature_id = cid;
        }
      }

      drone.current_target_id = nearest_creature_id;
    }

    Point target;
    ll min_dist2 = LINF;
    for(Vector2d<int> dxy: dxys) {
      Point next;
      bool ok = true;
      next.x = drone.pos.x + dxy.dx;
      next.y = drone.pos.y + dxy.dy;

      for(const Point& monster_pos: monsters_next_pos) {
        if(Point::calc_dist2(next, monster_pos) <  AGGRESSIVE_UNIT * AGGRESSIVE_UNIT) {
          ok = false;
          break;
        }
      }
      if(!ok) {
        continue;
      }

      Point destination;
      if(drone.current_target_id == -1) {
        destination = Point{drone.pos.x, 0};
      } else {
        Rectangle rect = creature_area[drone.current_target_id];
        destination = rect.get_center();
      }

      ll dist2 = Point::calc_dist2(next, destination);
      if(dist2 < min_dist2) {
        min_dist2 = dist2;
        target = next;
      }
    }

    lightOn = turn % 5 == 0;

    LOG_INFO("drone id: %2d, target creature %d, xy: (%d,%d)", drone.id,
              drone.current_target_id, target.x, target.y);

    char commands_str[60];

    sprintf(commands_str, "MOVE %d %d %d %s", target.x, target.y, lightOn,
            drone.message.c_str());
    commands.emplace_back(std::string(commands_str));
  }
  return commands;
}

inline void Player::read_score() { cin >> score; }

inline void Player::read_saved_data() {
  int saved_count;
  cin >> saved_count;
  cin.ignore();
  for (int i = 0; i < saved_count; i++) {
    ID creature_id;
    cin >> creature_id;
    cin.ignore();
    saved[creature_id] = true;
    int type = cg->creatures[creature_id].type;
    int color = cg->creatures[creature_id].color;
    found[color][type] = true;

    if (DEBUG) {
      LOG_INFO("%2d found by %s", creature_id, name.c_str());
    }
  }
}

inline void Player::read_drones() {
  dg.ids.clear();
  int drone_count;
  cin >> drone_count;
  for (int i = 0; i < drone_count; i++) {
    ID drone_id;
    cin >> drone_id;
    dg.ids.emplace_back(drone_id);
    dg.drones[drone_id].read_input();
    dg.drones[drone_id].id = drone_id;

    if (DEBUG) {
      LOG_INFO("%s's drone %2d position: (%d,%d), battery: %d, emergency: %d",
               name.c_str(), drone_id, dg.drones[drone_id].pos.x,
               dg.drones[drone_id].pos.y, dg.drones[drone_id].battery,
               dg.drones[drone_id].emergency);
    }
  }
}

inline void Player::read_scanned_data() {
  int drone_scan_count;
  cin >> drone_scan_count;
  cin.ignore();

  for (ID cid : cg->ids) {
    for (ID did : dg.ids) {
      dg.drones[did].scanned[cid] = false;
    }
  }

  for (int i = 0; i < drone_scan_count; i++) {
    int drone_id;
    int creature_id;
    cin >> drone_id >> creature_id;
    cin.ignore();

    dg.drones[drone_id].scanned[creature_id] = true;
    int type = cg->creatures[creature_id].type;
    int color = cg->creatures[creature_id].color;

    if (DEBUG) {
      LOG_INFO("Drone %2d scanned creature %d", drone_id, creature_id);
    }
  }
}

inline void Player::read_visible_creatures() {
  visible_creatures.clear();
  monsters.clear();
  int visible_creature_count;
  cin >> visible_creature_count;
  cin.ignore();
  for (int i = 0; i < visible_creature_count; i++) {
    ID creature_id;
    cin >> creature_id;
    VisibleCreature visible_creature(cg->creatures[creature_id]);
    visible_creature.read_input();
    visible_creatures.push_back(visible_creature);
    if(visible_creature.color == -1) {
      monsters.push_back(visible_creature);
    }
    if (DEBUG) {
      LOG_INFO("creature %2d position: (%d,%d), speed: (%d,%d)", creature_id,
                visible_creature.pos.x, visible_creature.pos.y,
                visible_creature.vx, visible_creature.vy);

      if (visible_creature.color == -1) {
        LOG_INFO("**MONSTER NEABY** position: (%d,%d)", visible_creature.pos.x,
                 visible_creature.pos.y);
      } 
      
    }
  }
}

inline void Player::read_radors() {
  observable.clear();
  for (ID cid : cg->ids) {
    observable[cid] = false;
  }

  int radar_blip_count;
  cin >> radar_blip_count;
  cin.ignore();
  for (int i = 0; i < radar_blip_count; i++) {
    int drone_id;
    int creature_id;
    std::string radar;
    cin >> drone_id >> creature_id >> radar;
    cin.ignore();
    dg.drones[drone_id].rador[creature_id] = radar;
    observable[creature_id] = true;
    if (DEBUG) {
      LOG_INFO("Drone id %2d creature id %2d, %s", drone_id, creature_id,
               radar.c_str());
    }
  }
}

enum FirstPoint {
  T0 = 0,
  T1 = 1,
  T2 = 2,
  C0_ALL = 3,
  C1_ALL = 4,
  C2_ALL = 5,
  T0_ALL = 6,
  T1_ALL = 7,
  T2_ALL = 8,
};

class Game {
public:
  inline void init() {
    cg = std::make_shared<CreatureGroup>();
    cg->init();

    my = Player("Rick", cg);
    foe = Player("Foe", cg);
    my.init();
    foe.init();

    if (DEBUG) {
      LOG_INFO("GAME INITIALIZATION DONE!!");
    }
  }

  void run() {
    // game loop
    for (int turn = 0; turn < MAX_TURN; turn++) {
      clock_t start_time = clock();
      read_input();
      update_state();

      Vec<std::string> commands = my.suggest_actions(turn);
      for (const std::string &commands : commands) {
        std::cout << commands << "\n";
      }
      std::cout.flush();
      LOG_INFO("TURN: %03d, ELAPSED TIME: %6.4f[s]", turn + 1,
               get_time(start_time));
    }
  }

  static bool is_collide(const ) {

  }

private:
  std::shared_ptr<CreatureGroup> cg;
  Player my, foe;
  HashMap<FirstPoint, bool> points_available;


  inline void read_input() {
    // read score
    my.read_score();
    foe.read_score();

    // read saved data
    my.read_saved_data();
    foe.read_saved_data();

    // read drones data
    my.read_drones();
    foe.read_drones();

    // input for scan count
    my.read_scanned_data();

    // input for near creatures
    my.read_visible_creatures();

    // read radars data
    my.read_radors();

    if (DEBUG) {
      LOG_INFO("GAME INPUT DONE");
    }
  } // read_input

  inline void update_state() {
    update_creature_group();
    my.update();
    update_first_point();

    if (DEBUG) {
      LOG_INFO("GAME UPDATE DONE");
    }
  } // update state

  inline void update_creature_group() {
    // update whether the fish is away or not
    for (ID cid : cg->ids) {
      if (!my.observable[cid]) {
        cg->creatures[cid].away = true;
      }
    }
  }

  inline void update_first_point() {
    // update first point available
    for (int type = 0; type < MAX_TYPE; type++) {
      FirstPoint fp;
      switch (type) {
      case 0:
        fp = FirstPoint::T0;
        break;
      case 1:
        fp = FirstPoint::T1;
        break;
      case 2:
        fp = FirstPoint::T2;
        break;
      default:
        break;
      }

      if (!points_available[fp]) {
        for (int color = 0; color < MAX_COLOR; color++) {
          if (my.found[color][type] || foe.found[color][type]) {
            points_available[fp] = true;
            break;
          }
        }
      }
    }

    // all type first
    for (int type = 0; type < MAX_TYPE; type++) {
      FirstPoint fp;
      switch (type) {
      case 0:
        fp = FirstPoint::T0_ALL;
        break;
      case 1:
        fp = FirstPoint::T1_ALL;
        break;
      case 2:
        fp = FirstPoint::T2_ALL;
        break;
      default:
        break;
      }
      if (!points_available[fp]) {
        bool my_done = true;
        bool foe_done = true;
        for (int color = 0; color < MAX_COLOR; color++) {
          my_done &= my.found[color][type];
          foe_done &= foe.found[color][type];
        }
        if (my_done || foe_done) {
          points_available[fp] = true;
        }
      }
    }

    // all color first
    for (int color = 0; color < MAX_COLOR; color++) {
      FirstPoint fp;
      switch (color) {
      case 0:
        fp = FirstPoint::C0_ALL;
        break;
      case 1:
        fp = FirstPoint::C1_ALL;
        break;
      case 2:
        fp = FirstPoint::C2_ALL;
        break;
      default:
        break;
      }
      if (!points_available[fp]) {
        bool my_done = true;
        bool foe_done = true;
        for (int type = 0; type < MAX_COLOR; type++) {
          my_done &= my.found[color][type];
          foe_done &= foe.found[color][type];
        }
        if (my_done || foe_done) {
          points_available[fp] = true;
        }
      }
    } // all color first
  }
};

class App {
public:
  App() {}
  App(const Property &_props) : props(_props) {}
  void init() {
    start_time = clock();
    game.init();
    if (DEBUG) {
      LOG_INFO("Application Initialization done at %6.4f[s]",
               get_time(start_time));
      LOG_INFO("Application is Ready");
    }
  }

  void run() {
    game.run();
    if (DEBUG) {
      LOG_INFO("Application Successfuly Ended at %6.4f[s]",
               get_time(start_time));
    }
  }

private:
  Property props;
  Game game;
  clock_t start_time;
};

int main() {
  if (DEBUG) {
    LOG_INFO("PROGRAM STARTED");
  }
  Property props;
  App app(props);
  app.init();
  app.run();
  return 0;
}