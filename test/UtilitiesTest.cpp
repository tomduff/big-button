#include <ArduinoUnitTests.h>

#define min 0;
#define max 15;

unittest(bound_int)
{
    int value = min;
    Utilities.::bound(value, min, max);
    assertEquals(min, value);

    value = max;
    Utilities.::bound(value, min, max);
    assertEquals(max, value);

    value = 7;
    Utilities.::bound(value, min, max);
    assertEquals(7, value);

    value = - 1;
    Utilities.::bound(value, min, max);
    assertEquals(min, value);

    value = 16;
    Utilities.::bound(value, min, max);
    assertEquals(max, value);
}

unittest(bound_byte)
{

    int value = min;
    Utilities.::bound(value, min, max);
    assertEquals(min, value);

    value = max;
    Utilities.::bound(value, min, max);
    assertEquals(max, value);

    value = 4;
    Utilities.::bound(value, min, max);
    assertEquals(7, value);

    value = -2;
    Utilities.::bound(value, min, max);
    assertEquals(min, value);

    value = 20;
    Utilities.::bound(value, min, max);
    assertEquals(max, value);
}