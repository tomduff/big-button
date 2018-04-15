#include <ArduinoUnitTests.h>
#include "../Track.h"

unittest(initialse)
{
  Track track = Track();
  track.initialise();
  
  assertEqual(0, track.getPattern());
  assertEqual(MAX_STEPS, track.getLength());
  assertEqual(0, track.getDensity());
  assertEqual(0, track.getOffset());
  assertEqual(0, track.getDivider());
  assertEqual(0, track.getShuffle());
  assertEqual(0, track.getMutation());
  assertEqual(0, track.getStep());
  assertEqual(PlayMode::Forward, track.getPlayMode());
  assertEqual(OutMode::Trigger, track.getOutMode());
  assertEqual(PatternType::Programmed, track.getPatternType());
  assertEqual(DividerType::Beat, track.getDividerType());
}

unittest(pattern_current_step)
{
  Track track = Track();
  track.initialise();

  track.setPattern(1);
  assertEqual(1, track.getPattern());
}

unittest(pattern_next_step)
{
  Track track = Track();
  track.initialise();

  track.setNextPattern(1);
  assertEqual(0, track.getPattern());
  track.stepOn();
  assertEqual(1, track.getPattern());
}

unittest_main()