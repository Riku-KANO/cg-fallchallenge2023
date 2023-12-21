#include "App.hpp"

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