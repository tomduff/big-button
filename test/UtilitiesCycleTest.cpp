#include <ArduinoUnitTests.h>
#include "../Utilities.h"

unittest(no_cycle_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 1;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(1, value);
}

unittest(cycle_to_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 16;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(0, value);
}

unittest(cycle_to_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -1;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(15, value);
}

unittest(cycle_past_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 20;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest(cycle_past_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -5;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(11, value);
}

unittest(multi_cycle_past_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 36;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest(multi_cycle_past_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -21;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(11, value);
}

unittest(multi_cycle_past_non_zero_min_int)
{
    int min_bound = 5;
    int max_bound = 10;
    int value = 10;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(8, value);
}

unittest(no_cycle_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = 1;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(1, value);
}

unittest(cycle_to_min_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = 16;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(0, value);
}

unittest(cycle_to_max_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = -1;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(15, value);
}

unittest(cycle_past_min_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = 20;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest(cycle_past_max_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = -5;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(11, value);
}

unittest(multi_cycle_past_min_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = 36;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(4, value);
}

unittest(multi_cycle_past_max_long)
{
    long min_bound = 0;
    long max_bound = 15;
    long value = -21;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(11, value);
}

unittest(multi_cycle_past_non_zero_min_long)
{
    long min_bound = 5;
    long max_bound = 10;
    long value = 10;
    Utilities::cycle(value, min_bound, max_bound);
    assertEqual(8, value);
}

unittest_main()