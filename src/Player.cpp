#include "Player.h"
#include "Property.h"
#include "geometry.h"

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
    for (ID drone_id : ge.my_dg.ids)
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
    for (ID drone_id : ge.my_dg.ids)
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
        if (ge.my_dg.drones[drone_id].emergency == 1)
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
        for (ID did : ge.my_dg.ids)
        {
            std::string radar = drone_radar[did][cid];
            if (radar == "TL")
            {
                rect.max_y = std::min(rect.max_y, ge.my_dg.drones[did].pos.y);
                rect.max_x = std::min(rect.max_x, ge.my_dg.drones[did].pos.x);
            }
            else if (radar == "TR")
            {
                rect.max_y = std::min(rect.max_y, ge.my_dg.drones[did].pos.y);
                rect.min_x = std::max(rect.min_x, ge.my_dg.drones[did].pos.x);
            }
            else if (radar == "BL")
            {
                rect.min_y = std::max(rect.min_y, ge.my_dg.drones[did].pos.y);
                rect.max_x = std::min(rect.max_x, ge.my_dg.drones[did].pos.x);
            }
            else if (radar == "BR")
            {
                rect.min_y = std::max(rect.min_y, ge.my_dg.drones[did].pos.y);
                rect.min_x = std::max(rect.min_x, ge.my_dg.drones[did].pos.x);
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
    DroneGroup &my_dg = env.my_dg;
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

    for (const ID drone_id : my_dg.ids)
    {
        Drone &drone = my_dg.drones[drone_id];
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