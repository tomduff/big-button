#include <Arduino.h>

class Utilities {
  public:
    // 0,1,2,3
    static void bound(int &value, int min, int max) {
      if (value < min ) value = min;
      else if (value > max) value = max;
    }
    // 0,1,2,3
    static void bound(long &value, long min, long max) {
      if (value < min ) value = min;
      else if (value > max) value = max;
    }
    // 0,1,2,3,0,1,2,3,0,1,2,3
    static void cycle(long &value, long min, long max) {
      while(value < min || value > max) {
        if (value < min ) value = (max + 1) + value;
        else if (value > max) value = value - (max + 1) + min;
      }
    }
    // 0,1,2,3,0,1,2,3,0,1,2,3
    static void cycle(int &value, int min, int max) {
      while(value < min || value > max) {
        if (value < min ) value = (max + 1) + value;
        else if (value > max) value = value - (max + 1) + min;;
      }
    }
    // 0,1,2,3,3,2,1,0,0,1,2,3,3,2,1,0
    static bool reverse(int &value, int min, int max) {
      bool reversed = false;
      while(value < min || value > max) {
        if (value > max) {
          value = (max + 1) - (value - max);
          reversed = !reversed;
        } else if (value < min) {
          value = (min - 1) + abs(min - value);
          reversed = !reversed;
        }
      }
      return reversed;
    }
};
