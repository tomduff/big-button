#include <ArduinoUnitTests.h>
#include "../Track.h"

unittest(initialse)
{
  Track track = Track();
  track.initialise();
  
  assertEqual(0, track.getPattern());
  assertEqual(0, track.getPosition());
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

  // Start at 0
  assertEqual(0, track.getPosition());

  // Step through the length
  for (int step = 1; step < length; ++step) {
    track.stepOn();
    assertEqual(step, track.getPosition());
  }

  // And back to 0 against
  track.stepOn();
  assertEqual(0, track.getPosition());
}

unittest(set_current_step)
{
  Track track = Track();
  track.initialise();

  track.setStep(1);
  assertEqual(1, track.getStep());
}

unittest(set_next_step)
{
  Track track = Track();
  track.initialise();

  track.setNextStep(1);
  assertEqual(0, track.getStep());
  track.stepOn();
  assertEqual(1, track.getStep());
}

unittest(offset_one_step)
{
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);

  // Step through the length
  for (int step = 1; step < length; ++step) {
    assertEqual(1, track.getStep());
    track.offsetPattern(step);
    assertEquals(step, track.getOffset());
    assertEqual(0, track.getStep());
    track.stepOn();
  }
}

unittest(offset_four_steps)
{
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);

  // Step through the length
  for (int step = 1; step < length; step+=4) {
    assertEqual(1, track.getStep());
    track.offsetPattern(step);
    assertEquals(step, track.getOffset());
    assertEqual(0, track.getStep());
    track.stepOn();
    track.stepOn();
    track.stepOn();
    track.stepOn();
  }

  assertEqual(1, track.getStep());
}

unittest(offset_through_length_steps)
{
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);

  // Offset to next step 16 + 1
  track.offsetPattern(17); 
   assertEquals(1, track.getOffset());

  track.stepOn();
  assertEqual(1, track.getStep());
}

unittest(offset_through_start_steps)
{
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);

  // Offset back to last step
  track.offsetPattern(-1); 
  assertEquals(15, track.getOffset());

  // Step to end
  for(int step =0; step < length - 1; ++step) track.stepOn();
  assertEqual(1, track.getStep());
}

unittest_main()