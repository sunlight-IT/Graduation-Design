#include "core.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"

void core_init(void)
{
}

void core_loop(void)
{
    debug_light();
}

void core_main(void)
{
    // setup
    while (1)
    {
        // loop
        core_loop();
    }
}