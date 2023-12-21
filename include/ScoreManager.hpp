#pragma once 
#include "GameEnv.hpp"

class ScoreManager
{
public:
    std::shared_ptr<GameEnv> env;
    inline void init(std::shared_ptr<GameEnv> _env);
    inline int calc_scanned_fish_estimated_point();
private:
};