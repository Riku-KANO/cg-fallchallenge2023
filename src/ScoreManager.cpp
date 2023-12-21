#include "ScoreManager.hpp"

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