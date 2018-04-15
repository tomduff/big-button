#include <ArduinoUnitTests.h>
#include "../Utilities.h"

unittest(no_cycle)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 1;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(1, value);
}

unittest(cycle_to_min)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 16;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(0, value);
}

unittest(cycle_to_max)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -1;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(15, value);
}

unittest(cycle_past_min)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 20;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest(cycle_past_max)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -5;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest_main()