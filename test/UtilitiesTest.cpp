#include <ArduinoUnitTests.h>
#include "../Utilities.h"

#define min_bound 0
#define max_bound 15

unittest(min_bound_int)
{
    int value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(max_bound_int)
{
    int value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(mid_bound_int)
{
    int value = 7;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(7, value);
}

unittest(less_than_bound_int)
{
    int value = -1;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(greater_than_bound_int)
{
    int value = 16;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(min_bound_byte)
{
    byte value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(max_bound_byte)
{
    byte value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(mid_bound_byte)
{
    byte value = 7;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(7, value);
}

unittest(less_than_bound_byte)
{
    byte value = -1;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(greater_than_bound_byte)
{
    byte value = 16;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest_main()