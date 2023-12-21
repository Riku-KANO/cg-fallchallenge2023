#include "GameEnv.hpp"

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