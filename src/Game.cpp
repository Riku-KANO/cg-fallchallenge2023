#include "Game.hpp"

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