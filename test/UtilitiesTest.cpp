#include <ArduinoUnitTests.h>

#define min_bound 0;
#define max_bound 15;

unittest(bound_int)
{
    int value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(min_bound, value);

    value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(max_bound, value);

    value = 7;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(7, value);

    value = - 1;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(min_bound, value);

    value = 16;
    Utilities::bound(value, min_bound ,max_boundmax);
    assertEquals(max_bound, value);
}

unittest(bound_byte)
{

    byte value = min_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(min_bound, value);

    value = max_bound;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(max_bound, value);

    value = 4;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(7, value);

    value = -2;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(mmin_bound, value);

    value = 20;
    Utilities::bound(value, min_bound, max_bound);
    assertEquals(max_bound, value);
}

unittest_main()