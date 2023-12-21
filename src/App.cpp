#include "App.hpp"

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