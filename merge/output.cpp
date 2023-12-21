#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using ll = long long;

using i8 = char;

using u16 = uint16_t;
using u32 = uint32_t;
using i32 = int32_t;
using i64 = int64_t;
using byte = unsigned int;

#define Vec std::vector
#define HashMap std::unordered_map
#define ID int
#define FOR(i, n) for (int i = 0; i < (int)(n); i++)
#define DEBUG 1
#define LOG_INFO(message, ...) \
    fprintf(stderr, "[INFO] " message "\n", ##__VA_ARGS__)
#define LOG_ERROR(message, ...) \
    fprintf(stderr, "[ERROR] " message "\n", ##__VA_ARGS__)

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

double get_time(clock_t start_time)
{
    return (double)(clock() - start_time) / CLOCKS_PER_SEC;
}

int get_time_ms(clock_t start_time)
{
    return clock() - start_time;
}

// for clang compiler
bool contain_value(const std::vector<int>* vec, int value) {
    for(int element: *vec) {
        if(element == value) {
            return true;
        }
    }
    return false;
}

struct Property {
  int NUM_SPLIT = 30;
};



struct Point
{
    int x, y;

    static long long calc_dist2(const Point &p1, const Point &p2)
    {
        return (long long)(p1.x - p2.x) * (p1.x - p2.x) +
               (long long)(p1.y - p2.y) * (p1.y - p2.y);
    }
};

template <typename T>
struct Vector2d
{
    T dx, dy;
};

struct Rectangle
{
    int min_x, max_x;
    int min_y, max_y;
    Rectangle() : min_x(0), max_x(MAX_WIDTH), min_y(0), max_y(MAX_HEIGHT) {}
    int calc_area()
    {
        return (max_x - min_x) * (max_y - min_y);
    }
    Point get_center()
    {
        return Point{(min_x + max_x) / 2, (min_y + max_y) / 2};
    }
};



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


class ScoreManager
{
public:
    std::shared_ptr<GameEnv> env;
    inline void init(std::shared_ptr<GameEnv> _env);
    inline int calc_scanned_fish_estimated_point();
private:
};



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


class App {
public:
  App() {}
  App(const Property &_props) : props(_props) {}
  void init();
  void run();

private:
  Property props;
  Game game;
  clock_t start_time;
};

/**
 * @brief main function 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

int main(int argc, char *argv[])
{
    if (DEBUG)
    {
        LOG_INFO("PROGRAM STARTED");
    }

    Property props;
    App app(props);
    app.init();
    app.run();
    return 0;
}


void App::init()
{
    start_time = clock();
    game.init(props);
    if (DEBUG)
    {
        LOG_INFO("Application Initialization done at %6.4f[s]",
                 get_time(start_time));
        LOG_INFO("Application is Ready");
    }
}

void App::run()
{
    game.run();
    if (DEBUG) {
      LOG_INFO("Application Successfuly Ended at %6.4f[s]",
               get_time(start_time));
    }
}


inline void Game::init(Property props)
{
    env = std::make_shared<GameEnv>();
    env->init();
    score_manager.init(env);

    my = Player("Rick");
    my.init(props);

    if (DEBUG)
    {
        LOG_INFO("GAME INITIALIZATION DONE!!");
    }
}

inline void Game::run()
{
    for (int turn = 0; turn < MAX_TURN; turn++)
    {
        clock_t start_time = clock();
        read_game_loop_input();
        env->update(game_loop_input);
        my.update(*env, game_loop_input);

        Vec<std::string> commands = my.suggest_actions(turn, *env);
        for (const std::string &commands : commands)
        {
            std::cout << commands << "\n";
        }
        std::cout.flush();
        LOG_INFO("TURN: %03d, ELAPSED TIME: %6.4f[s]", turn + 1,
                 get_time(start_time));
    }
}

inline void Game::read_game_loop_input()
{
    game_loop_input.init();
    game_loop_input.read_input();
}


inline void GameEnv::init()
{
    memset(specie_first, false, sizeof(specie_first));
    type0_all = false;
    type1_all = false;
    type2_all = false;
    color0_all = false;
    color1_all = false;
    color2_all = false;
    color3_all = false;
    type0_all_first = false;
    type1_all_first = false;
    type2_all_first = false;
    color0_all_first = false;
    color1_all_first = false;
    color2_all_first = false;
    color3_all_first = false;

    cg.read_input();

    memset(my_found, false, sizeof(my_found));
    memset(foe_found, false, sizeof(foe_found));
    for (ID cid : cg.ids)
    {
        my_scanned[cid] = false;
        foe_scanned[cid] = false;
        my_saved[cid] = false;
        foe_saved[cid] = false;
    }
}

inline void GameEnv::update(const GameLoopInput &input)
{
    // score update
    update_score(input);

    // update found data
    update_found_data(input);

    // update creature states
    update_creatures(input);

    // update drones
    update_drones(input);

    // updata observable creatures
    update_observable(input);

    //
    update_scanned_data(input);

    // update first point
    update_first_point();

    if (DEBUG)
    {
        LOG_INFO("GAME ENV UPDATE DONE");
    }
}

// inline int GameEnv::calc_scanned_fish_estimated_point() {
//   int estimated_score = 0;
//   bool next_found[MAX_COLOR][MAX_TYPE];
//   FOR(type, MAX_TYPE) {
//     FOR(color, MAX_COLOR) { next_found[color][type] = my_found[color][type];
//     }
//   }

//   for (ID drone_id : my_dg.ids) {
//     for (ID creature_id : cg.ids) {
//       if (mmy_dg.drones[drone_id].scanned[creature_id]) {
//         int basic_point;
//         int next_type = cg.creatures[creature_id].type;
//         int next_color = cg.creatures[creature_id].color;
//         next_found[next_color][next_type] = true;

//         if (next_type == 0)
//           basic_point = 1;
//         else if (next_type == 1)
//           basic_point = 2;
//         else if (next_type == 2)
//           basic_point = 3;

//         if (!specie_first[next_color][next_type])
//           basic_point *= 2;

//         estimated_score += basic_point;
//       }
//     }
//   }

//   FOR(color, MAX_COLOR) {}
// }

inline void GameEnv::update_score(const GameLoopInput &input)
{
    my_score = input.my_score;
    foe_score = input.foe_score;
}

inline void GameEnv::update_found_data(const GameLoopInput &input)
{
    my_new_found.clear();
    for (ID creature_id : input.my_saved_creature_ids)
    {
        int type = cg.creatures[creature_id].type;
        int color = cg.creatures[creature_id].color;
        if (!my_saved[creature_id])
        {
            my_new_found.emplace_back(creature_id);
        }
        my_saved[creature_id] = true;
        my_found[color][type] = true;
    }
    foe_new_found.clear();
    for (ID creature_id : input.foe_saved_creature_ids)
    {
        int type = cg.creatures[creature_id].type;
        int color = cg.creatures[creature_id].color;
        if (!foe_saved[creature_id])
        {
            foe_new_found.emplace_back(creature_id);
        }
        foe_saved[creature_id] = true;
        foe_found[color][type] = true;
    }
}

inline void GameEnv::update_creatures(const GameLoopInput &input)
{
    visibles.clear();
    monsters.clear();
    visibles = input.visibles;

    for (VisibleCreature visible_creature : input.visibles)
    {
        ID creature_id = visible_creature.id;
        int creature_type = cg.creatures[creature_id].type;
        if (creature_type == -1)
        {
            monsters.push_back(visible_creature);
        }

        if (DEBUG)
        {
            LOG_INFO("creature %2d position: (%d,%d), speed: (%d,%d)", creature_id,
                     visible_creature.pos.x, visible_creature.pos.y,
                     visible_creature.vx, visible_creature.vy);

            if (creature_type == -1)
            {
                LOG_INFO("**MONSTER NEABY** position: (%d,%d)", visible_creature.pos.x,
                         visible_creature.pos.y);
            }
        }
    }
}

inline void GameEnv::update_drones(const GameLoopInput &input)
{
    dg.my_ids.clear();
    for (const Drone &drone : input.my_drones)
    {
        dg.my_ids.emplace_back(drone.id);
        dg.drones[drone.id] = drone;
    }

    dg.foe_ids.clear();
    for (const Drone &drone : input.foe_drones)
    {
        dg.foe_ids.emplace_back(drone.id);
        dg.drones[drone.id] = drone;
    }

    // update message
    for (ID drone_id : dg.my_ids)
    {
        Drone &drone = dg.drones[drone_id];
        if (drone.emergency && drone.message == "")
        {
            int r = mt() % 5;
            if (r == 0)
                drone.message = "😇😇😇";
            else if (r == 1)
                drone.message = "🥹🥹🥹";
            else if (r == 2)
                drone.message = "😱😱😱";
            else if (r == 3)
                drone.message = "🤬🤬🤬";
            else if (r == 4)
                drone.message = "😵‍💫😵‍💫😵‍💫";
        }
        else if (drone.emergency == 0)
        {
            drone.message = "";
        }
    }
}

inline void GameEnv::update_observable(const GameLoopInput &input)
{
    observable.clear();
    for (ID cid : cg.ids)
    {
        observable[cid] = false;
    }
    for (const Radar &radar : input.radars)
    {
        observable[radar.creature_id] = true;
    }
}

inline void GameEnv::update_scanned_data(const GameLoopInput &input)
{
    my_scanned.clear();
    foe_scanned.clear();

    for (ID cid : cg.ids)
    {
        my_scanned[cid] = false;
        foe_scanned[cid] = false;
    }

    for (std::pair<ID, ID> pair : input.scanned_drone_creatures)
    {
        auto [drone_id, creature_id] = pair;
        if (contain_value(&dg.my_ids, drone_id))
        {
            my_scanned[creature_id] = true;
        }
        else
        {
            foe_scanned[creature_id] = true;
        }
    }
}

inline void GameEnv::update_first_point()
{

    FOR(type, MAX_TYPE)
    {
        FOR(color, MAX_COLOR)
        {
            if (!specie_first[type][color])
            {
                if (my_found[type][color] || foe_found[type][color])
                {
                    specie_first[type][color] = true;
                }
            }
        }
    }

    // type 0 all judge
    if (!type0_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(color, MAX_COLOR)
        {
            if (!my_found[color][0])
            {
                my_f = false;
            }
            if (!foe_found[color][0])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            type0_all_first = true;
        }
    }

    // type 1 0 all judge
    if (!type1_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(color, MAX_COLOR)
        {
            if (!my_found[color][1])
            {
                my_f = false;
            }
            if (!foe_found[color][1])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            type1_all_first = true;
        }
    }

    // type 2 all judge
    if (!type2_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(color, MAX_COLOR)
        {
            if (!my_found[color][2])
            {
                my_f = false;
            }
            if (!foe_found[color][2])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            type2_all_first = true;
        }
    }

    // color 0 all judge
    if (!color0_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(type, MAX_TYPE)
        {
            if (!my_found[0][type])
            {
                my_f = false;
            }
            if (!foe_found[0][type])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            color0_all_first = true;
        }
    }

    // color 1 all judge
    if (!color1_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(type, MAX_TYPE)
        {
            if (!my_found[1][type])
            {
                my_f = false;
            }
            if (!foe_found[1][type])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            color1_all_first = true;
        }
    }

    // color 2 all judge
    if (!color2_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(type, MAX_TYPE)
        {
            if (!my_found[2][type])
            {
                my_f = false;
            }
            if (!foe_found[2][type])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            color2_all_first = true;
        }
    }

    // color 3 all judge
    if (!color3_all_first)
    {
        bool my_f = true;
        bool foe_f = true;
        FOR(type, MAX_TYPE)
        {
            if (!my_found[3][type])
            {
                my_f = false;
            }
            if (!foe_found[3][type])
            {
                foe_f = false;
            }
        }

        if (my_f || foe_f)
        {
            color3_all_first = true;
        }
    }
}

inline bool GameEnv::is_specie_found_first(int color, int type) const {
    return this->specie_first[color][type];
}


void ScoreManager::init(std::shared_ptr<GameEnv> _env) {
    env = _env;
}

inline int ScoreManager::calc_scanned_fish_estimated_point() {
    int estimated_score = 0;

    // update next new found species
    bool new_found[MAX_COLOR][MAX_TYPE];
    memset(new_found, false, sizeof(new_found));
    for(ID creature_id: env->cg.ids) {
        const Creature& creature = env->cg.creatures[creature_id];
        if(env->my_scanned[creature_id]) {
            int type = creature.type;
            int color = creature.color;
            new_found[color][type] = true;
        }
    }

    // for single species
    FOR(color, MAX_COLOR) {
        FOR(type, MAX_TYPE) {
            if(new_found[color][type]) {
                int point = 0;
                if(type == 0) {
                    point = POINT_T0;
                } else if(type == 1) {
                    point = POINT_T1;   
                } else if(type == 2) {
                    point = POINT_T2;
                }

                if(!env->is_specie_found_first(color, type)) {
                    point *= 2;
                }

                estimated_score += point;
            }
        }
    }

    // all color point check
    auto all_color_point_process = [&](int color, bool color_all_first, int base_point) -> int {
        bool all_found = true;
        int ret = 0;
        FOR(type, MAX_TYPE) {
            all_found &= (env->my_found[color][type] | new_found[color][type]);
        }
        if(all_found) {
            int point = base_point;

            if(!color_all_first) {
                point *= 2;
            }
            ret += point;
        }
        return ret;
    };

    if(!env->color0_all) {
        estimated_score += all_color_point_process(0, env->color0_all_first, POINT_ALL_COLOR);
    }
    if(!env->color1_all) {
        estimated_score += all_color_point_process(1, env->color1_all_first, POINT_ALL_COLOR);
    }
    if(!env->color2_all) {
        estimated_score += all_color_point_process(2, env->color2_all_first, POINT_ALL_COLOR);
    }
    if(!env->color3_all) {
        estimated_score += all_color_point_process(3, env->color3_all_first, POINT_ALL_COLOR);
    }

    // all type point check
    auto all_type_point_process = [&](int type, bool type_all_first, int base_point) -> int {
        bool all_found = true;
        int ret = 0;
        FOR(color, MAX_COLOR) {
            all_found &= (env->my_found[color][type] | new_found[color][type]);
        }
        if(all_found) {
            int point = base_point;

            if(!type_all_first) {
                point *= 2;
            }
            ret += point;
        }
        return ret;
    };

    if(!env->type0_all) {
        estimated_score += all_type_point_process(0, env->type0_all_first, POINT_ALL_TYPE);
    }
    if(!env->type1_all) {
        estimated_score += all_type_point_process(1, env->type1_all_first, POINT_ALL_TYPE);
    }
    if(!env->type2_all) {
        estimated_score += all_type_point_process(2, env->type0_all_first, POINT_ALL_TYPE);
    }

    return estimated_score;
}


void Player::init(const Property &props)
{

    int num_split = props.NUM_SPLIT;
    double radius = (double)MOVE_UNIT;
    for (int i = 0; i < num_split; i++)
    {
        double theta = 2.0 * PI * i / num_split;
        double dx = radius * std::cos(theta);
        double dy = radius * std::sin(theta);
        dxys.push_back(Vector2d<int>{(int)dx, (int)dy});
    }
}

void Player::update(GameEnv &ge, const GameLoopInput &input)
{
    // drone scan update
    for (ID drone_id : ge.dg.my_ids)
    {
        drone_scanned[drone_id].clear();
    }
    for (std::pair<ID, ID> p : input.scanned_drone_creatures)
    {
        auto [drone_id, creature_id] = p;
        drone_scanned[drone_id].insert(creature_id);
    }

    // update drone radar
    for (const Radar &radar : input.radars)
    {
        auto [drone_id, creature_id, radar_type] = radar;
        drone_radar[drone_id][creature_id] = radar_type;
    }

    // drone target update
    for (ID drone_id : ge.dg.my_ids)
    {
        int target = drone_current_target[drone_id];
        if (ge.my_scanned[target] || !ge.observable[target] ||
            ge.my_saved[target])
        {
            drone_current_target[drone_id] = -1;
            if (DEBUG)
            {
                LOG_INFO("%d %d %d", ge.my_scanned[target], ge.observable[target],
                         ge.my_saved[target]);
                LOG_INFO("Drone %2d lost its target %2d...", drone_id, target);
            }
        }
        if (ge.dg.drones[drone_id].emergency == 1)
        {
            drone_current_target[drone_id] = -1;
            drone_scanned[drone_id].clear();
            if (DEBUG)
            {
                LOG_INFO("Drone %2d lost its target %2d due to emergency", drone_id,
                         target);
            }
        }
    }

    // calculate creature area
    for (ID cid : ge.cg.ids)
    {
        if (!ge.observable[cid])
        {
            continue;
        }
        Rectangle rect;
        for (ID did : ge.dg.my_ids)
        {
            std::string radar = drone_radar[did][cid];
            if (radar == "TL")
            {
                rect.max_y = std::min(rect.max_y, ge.dg.drones[did].pos.y);
                rect.max_x = std::min(rect.max_x, ge.dg.drones[did].pos.x);
            }
            else if (radar == "TR")
            {
                rect.max_y = std::min(rect.max_y, ge.dg.drones[did].pos.y);
                rect.min_x = std::max(rect.min_x, ge.dg.drones[did].pos.x);
            }
            else if (radar == "BL")
            {
                rect.min_y = std::max(rect.min_y, ge.dg.drones[did].pos.y);
                rect.max_x = std::min(rect.max_x, ge.dg.drones[did].pos.x);
            }
            else if (radar == "BR")
            {
                rect.min_y = std::max(rect.min_y, ge.dg.drones[did].pos.y);
                rect.min_x = std::max(rect.min_x, ge.dg.drones[did].pos.x);
            }
            else
            {
                LOG_ERROR("INVALID TYPE at line %d", __LINE__);
            }
        }
        creature_area[cid] = rect;
        if (DEBUG)
        {
            LOG_INFO("Creature %2d situated in DY:(%d,%d), DX(%d,%d)", cid,
                     rect.min_y, rect.max_y, rect.min_x, rect.max_x);
        }
    }

    if (DEBUG)
    {
        LOG_INFO("PLAYER UPDATE DONE");
    }
}

inline Point Player::define_move_by_target_rectangle(ID drone_id)
{
    int target = drone_current_target[drone_id];
    Point ret;
    int center_x =
        (creature_area[target].max_x + creature_area[target].min_x) / 2;
    int center_y =
        (creature_area[target].max_y + creature_area[target].min_y) / 2;
    ret = Point{center_x, center_y};
    return ret;
}

inline Vec<std::string> Player::suggest_actions(int turn, GameEnv &env)
{
    CreatureGroup &cg = env.cg;
    DroneGroup &dg = env.dg;
    Vec<std::string> commands;
    Vec<std::pair<Point, Vector2d<int>>> monsters_physics;
    bool lightOn;
    HashMap<ID, bool> assigned;
    for (ID cid : cg.ids)
    {
        assigned[cid] = false;
    }

    for (const VisibleCreature &monster : env.monsters)
    {
        int x = monster.pos.x;
        int y = monster.pos.y;
        int vx = monster.vx;
        int vy = monster.vy;
        monsters_physics.push_back({Point{x, y}, Vector2d<int>{vx, vy}});
    }

    for (const ID drone_id : dg.my_ids)
    {
        Drone &drone = dg.drones[drone_id];
        if (drone_current_target[drone_id] == -1)
        {
            LOG_INFO("find nearest by drone %d", drone_id);
            // fine nearest creature
            int nearest_creature_id = -1;
            ll min_dist2 = LINF;
            for (ID cid : cg.ids)
            {
                if (!env.observable[cid] || cg.is_monster(cid) || env.my_scanned[cid] ||
                    env.my_saved[cid] || !env.observable[cid] || assigned[cid])
                {
                    continue;
                }
                Point creature_pos = creature_area[cid].get_center();
                ll dist2 = Point::calc_dist2(creature_pos, drone.pos);
                if (dist2 < min_dist2)
                {
                    min_dist2 = dist2;
                    nearest_creature_id = cid;
                }
            }

            drone_current_target[drone_id] = nearest_creature_id;
        }

        Point target;
        ll min_dist2 = LINF;
        for (Vector2d<int> dxy : dxys)
        {
            Point next;
            bool ok = true;
            next.x = drone.pos.x + dxy.dx;
            next.y = drone.pos.y + dxy.dy;

            // check monster threats by center
            for (ID cid : cg.ids)
            {
                if (cg.is_monster(cid))
                {
                    Point monster_center = creature_area[cid].get_center();
                    if (Point::calc_dist2(next, monster_center) < 700 * 700)
                    {
                        ok = false;
                        break;
                    }
                }
            }

            for (auto [monster_pos, monster_v] : monsters_physics)
            {
                if (judge_collide(drone.pos, dxy, monster_pos, monster_v))
                {
                    ok = false;
                    break;
                }

                Point monster_next_pos = Point{
                    monster_pos.x + monster_v.dx,
                    monster_pos.y + monster_v.dy,
                };

                if (Point::calc_dist2(next, monster_next_pos) <
                    AGGRESSIVE_UNIT * AGGRESSIVE_UNIT)
                {
                    ok = false;
                    break;
                }
            }
            if (!ok)
            {
                continue;
            }

            Point destination;
            int drone_target = drone_current_target[drone_id];
            if (drone_target == -1)
            {
                destination = Point{drone.pos.x, 0};
            }
            else
            {
                Rectangle rect = creature_area[drone_target];
                destination = rect.get_center();
            }

            ll dist2 = Point::calc_dist2(next, destination);
            if (dist2 < min_dist2)
            {
                min_dist2 = dist2;
                target = next;
            }
        }

        int drone_target_id = drone_current_target[drone_id];

        LOG_INFO("min dist %10lld", min_dist2);

        lightOn = turn % 5 == 0;

        LOG_INFO("drone id: %2d, target creature %d, xy: (%d,%d)", drone.id,
                 drone_target_id, target.x, target.y);

        char commands_str[60];

        sprintf(commands_str, "MOVE %d %d %d %s", target.x, target.y, lightOn,
                drone.message.c_str());
        commands.emplace_back(std::string(commands_str));

        if (drone_target_id != -1)
        {
            assigned[drone_target_id] = true;
        }
    }
    return commands;
}

inline bool Player::judge_collide(Point drone_pos, Vector2d<int> drone_v,
                                  Point ugly_pos, Vector2d<int> ugly_v) const
{
    const int DRONE_HIT_RANGE = 200;
    const int UGLY_EAT_RANGE = 300;
    if (Point::calc_dist2(drone_pos, ugly_pos) <=
        (DRONE_HIT_RANGE + UGLY_EAT_RANGE) * (DRONE_HIT_RANGE + UGLY_EAT_RANGE))
    {
        return true;
    }

    double x = ugly_pos.x;
    double y = ugly_pos.y;
    double ux = drone_pos.x;
    double uy = drone_pos.y;

    double x2 = x - ux;
    double y2 = y - uy;
    double r2 = UGLY_EAT_RANGE + DRONE_HIT_RANGE;
    double vx2 = ugly_v.dx - drone_v.dx;
    double vy2 = ugly_v.dy - drone_v.dy;

    // Resolving: sqrt((x + t*vx)^2 + (y + t*vy)^2) = radius <=> t^2*(vx^2 + vy^2)
    // + t*2*(x*vx + y*vy) + x^2 + y^2 - radius^2 = 0 at^2 + bt + c = 0; a = vx^2
    // + vy^2 b = 2*(x*vx + y*vy) c = x^2 + y^2 - radius^2

    double a = vx2 * vx2 + vy2 * vy2;

    if (a <= 0.0)
    {
        return false;
    }

    double b = 2.0 * (x2 * vx2 + y2 * vy2);
    double c = x2 * x2 + y2 * y2 - r2 * r2;
    double delta = b * b - 4.0 * a * c;

    if (delta < 0.0)
    {
        return false;
    }

    double t = (-b - std::sqrt(delta)) / (2.0 * a);

    if (t <= 0.0)
    {
        return false;
    }
    if (t > 1.0)
    {
        return false;
    }

    return true;
}