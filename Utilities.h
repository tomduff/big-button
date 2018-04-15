#include <Arduino.h>

class Utilities {
  public:
    static void bound(byte &value, byte min, byte max) {
      if (value < min ) value = min;
      else if (value > max) value = max;
    }
    static void bound(int &value, int min, int max) {
      if (value < min ) value = min;
      else if (value > max) value = max;
    }
    static void bound(long &value, long min, long max) {
      if (value < min ) value = min;
      else if (value > max) value = max;
    }
    static void cycle(byte &value, byte min, byte max) {
      while(value < min || value > max) {
        if (value < min ) value = (max + 1) + value;
        else if (value > max) value = value - (max + 1);
      }
    }
    static void cycle(int &value, int min, int max) {
      while(value < min || value > max) {
        if (value < min ) value = (max + 1) + value;
        else if (value > max) value = value - (max + 1);
      }
    }
    static bool reverse(byte &value, byte min, byte max) {
      bool reversed = false;
      if (value > max) {
        value = max;
        reversed = true;
      } else if (value < min) {
        value = min;
        reversed = true;
      }
      return reversed;
    }
};
