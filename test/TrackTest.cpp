#include <ArduinoUnitTests.h>

unittest(pretend_equal_things_arent)
{
  int x = 3;
  int y = 3;
  assertNotEqual(x, y);
}

unittest(initialse)
{
  Track track = TRack();
  track.initialise();
  assertEqual(0, track.getPattern();
  assertEqual(MAX_STEPS, track.getLength());
  assertEqual(0, track.getDensity());
  assertEqual(0, track.getOffset());
  assertEqual(0, track.getDivider());
  assertEqual(0, track.getShuffle());
  assertEqual(PlayMode::Forward, track.getPlayMode()));
  assertEqual(OutMode::Trigger, track.getOutMode();
  assertEqual(PatternType::Programmed, track.getPatternType());
  assertEqual(DividerType::Beat, track.getDividerType());
}

unittest_main()