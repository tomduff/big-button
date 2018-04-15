#include <ArduinoUnitTests.h>
#include "../Utilities.h"

unittest(min_bound_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(max_bound_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(mid_bound_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 7;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(7, value);
}

unittest(less_than_bound_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = -1;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(greater_than_bound_int)
{
    int min_bound = 0;
    int max_bound = 15;
    int value = 16;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(min_bound_byte)
{
    byte min_bound = 4;
    byte max_bound = 15;
    byte value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(max_bound_byte)
{
    byte min_bound = 4;
    byte max_bound = 15;
    byte value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(mid_bound_byte)
{
    byte min_bound = 4;
    byte max_bound = 15;
    byte value = 7;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(7, value);
}

unittest(less_than_bound_byte)
{
    byte min_bound = 4;
    byte max_bound = 15;
    byte value = 2;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(greater_than_bound_byte)
{
    byte min_bound = 4;
    byte max_bound = 15;
    byte value = 16;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(min_bound_long)
{
    long min_bound = 0;
    long max_bound = 150;
    long value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(max_bound_long)
{
    long min_bound = 0;
    long max_bound = 150;
    long value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest(mid_bound_long)
{
    long min_bound = 0;
    long max_bound = 150;
    long value = 83;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(7, value);
}

unittest(less_than_bound_long)
{
    long min_bound = 0;
    long max_bound = 150;
    long value = -41;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(min_bound, value);
}

unittest(greater_than_bound_long)
{
    long min_bound = 0;
    long max_bound = 150;
    long value = 200;
    Utilities::bound(value, min_bound, max_bound);
    assertEqual(max_bound, value);
}

unittest_main()