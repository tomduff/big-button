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

unittest(position_stepping)
{
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  assertEqual(0, track.getPosition());

  for (int step = 0; step < length; ++step) {
    track.stepOn();
    assertEqual(step, track.getPosition());
  }

  track.stepOn();
  assertEqual(0, track.getPosition());
}

unittest(pattern_current_step)
{
  Track track = Track();
  track.initialise();

  track.setStep(1);
  assertEqual(1, track.getStep());
}

unittest(pattern_next_step)
{
  Track track = Track();
  track.initialise();

  track.setNextStep(1);
  assertEqual(0, track.getStep());
  track.stepOn();
  assertEqual(1, track.getStep());
}

unittest_main()