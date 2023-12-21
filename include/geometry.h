#pragma once

#include "global.h"

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
