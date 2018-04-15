#include <ArduinoUnitTests.h>
#include "../Track.h"

unittest(initialse) {
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

unittest(position_stepping) {
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

unittest(reduce_length) {
  int length = 8;
  int reducedLength = 4;
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
  track.stepOn();
  assertEqual(0, track.getPosition());

  track.setLength(reducedLength);
  for (int step = 1; step < reducedLength; ++step) {
    track.stepOn();
    assertEqual(step, track.getPosition());
  }
  track.stepOn();
  assertEqual(0, track.getPosition());
}

unittest(increase_length) {
  int length = 4;
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

  track.setLength(length + 2);

  // And it now goes beyond the old length
  track.stepOn();
  assertEqual(length, track.getPosition());
  track.stepOn();
  assertEqual(length + 1, track.getPosition());
}

unittest(set_current_step) {
  Track track = Track();
  track.initialise();

  track.setStep(1);
  assertEqual(1, track.getStep());
}

unittest(set_next_step) {
  Track track = Track();
  track.initialise();

  track.setNextStep(1);
  assertEqual(0, track.getStep());
  track.stepOn();
  assertEqual(1, track.getStep());
}

unittest(offset_one_step) {
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);
  assertEqual(1, track.getStep());

  // Step through the length
  for (int step = 1; step < length; ++step) {
    track.stepOn();
    assertEqual(0, track.getStep());
    track.setOffset(step);
    assertEqual(step, track.getOffset());
    assertEqual(1, track.getStep());
  }
}

unittest(offset_four_steps) {
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);
  assertEqual(1, track.getStep());
  track.stepOn();
  track.stepOn();
  track.stepOn();
  track.stepOn();
  assertEqual(0, track.getStep());
  track.setOffset(4);
  assertEqual(4, track.getOffset());
  assertEqual(1, track.getStep());
}

unittest(offset_through_length_steps) {
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);
  assertEqual(0, track.getOffset());

  // Offset to next step 16 + 1
  track.setOffset(17); 
  assertEqual(1, track.getOffset());

  track.stepOn();
  assertEqual(1, track.getStep());
}

unittest(offset_through_start_steps) {
  int length = 16;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  track.setStep(1);
  assertEqual(0, track.getOffset());

  // Offset back to last step
  track.setOffset(-1); 
  assertEqual(15, track.getOffset());

  // Step to end
  for(int step = 1; step < length; ++step) track.stepOn();
  assertEqual(1, track.getStep());
}

unittest(reset) {
  Track track = Track();
  track.initialise();

  track.stepOn();
  track.stepOn();
  track.stepOn();
  track.stepOn();
  track.stepOn();

  assertEqual(5, track.getPosition());

  track.reset();

  assertEqual(0, track.getPosition());
}

unittest(clearPattern) {
  int length = 16;
  Track track = Track();
  track.initialise();

  track.setStep(1);
  track.setNextStep(1);

  assertNotEqual(0, track.getPattern());
  track.clearPattern();
  assertEqual(0, track.getPattern());
}

unittest(increase_length_new_sets_unset) {
  int length = 8;
  Track track = Track();
  track.initialise();
  track.setLength(length);

  for (int step = 1; step < length; ++step) {
    track.setStep(1);
    track.stepOn();
  }

  // Reduce and then increase length
  track.setLength(length - 4);
  track.setLength(length);
  track.reset();

  // Check steps inside the shorter step remain set
  // and steps that were lost when shortened have been reset
  for (int step = 1; step < length; ++step) {
     if(step > length - 4) assertEqual(0, track.getStep());
     else assertEqual(1, track.getStep());
     track.stepOn();
  }
}

unittest_main()