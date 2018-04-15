#include <ArduinoUnitTests.h>
#include "../Utilities.h"

unittest(no_reverse_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 1;
    bool reversed = Utilities::revrse(value, min_bound, max_bound);
    assertEqual(1, value);
    assertEqual(flase, reversed);
}

unittest(no_reverse_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = min_bound;
    bool reversed = Utilities::revrse(value, min_bound, max_bound);
    assertEqual(min_bound, value);
    assertEqual(flase, reversed);
}

unittest(no_reverse_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = max_bound;
    bool reversed = Utilities::revrse(value, min_bound, max_bound);
    assertEqual(max_bound, value);
    assertEqual(flase, reversed);
}

unittest_main()