#pragma once
#include <cmath>
#include <random>
#include <chrono>

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