
#include "World.hpp"

/*
 * How TODOs are classed:
 *     Class 1: Needs to be solved first before development can continue
 *     Class 2: What to do next
 *     Class 3: Should be solved to make program better
 */

//TODO: class 3: move ec_system.hpp, event_system.hpp, physics_units.hpp, glm(...).hpp, std-headers to precompiled header (can i use c++20 modules?)

int main()
{
    World world;
    world.init();
    world.loop();

    return 0;
}
