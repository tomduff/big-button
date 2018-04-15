#include <ArduinoUnitTests.h>
#include "../Utilities.h"

unittest(no_reverse_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 1;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(1, value);
    assertEqual(flase, reversed);
}

unittest(no_reverse_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = min_bound;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(min_bound, value);
    assertEqual(flase, reversed);
}

unittest(no_reverse_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = max_bound;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(max_bound, value);
    assertEqual(flase, reversed);
}

unittest(reverse_to_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -1;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(0, value);
    assertEqual(true, reversed);
}

unittest(reverse_to_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 16;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(15, value);
    assertEqual(true, reversed);
}

unittest(reverse_past_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -5;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(4, value);
    assertEqual(true, reversed);
}

unittest(reverse_past_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 20;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(11, value);
    assertEqual(true, reversed);
}

unittest(multi_reverse_past_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -44;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(12, value);
    assertEqual(true, reversed);
}

unittest(multi_reverse_past_max_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 51;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(12, value);
    assertEqual(true, reversed);
}

unittest(multi_reverse_past_min_with_non_zero_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -44;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(12, value);
    assertEqual(true, reversed);
}

unittest(multi_reverse_past_max_with_non_zero_min_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 51;
    bool reversed = Utilities::reverse(value, min_bound, max_bound);
    assertEqual(12, value);
    assertEqual(true, reversed);
}

unittest_main()