#include "Track.h"
#include "Utilities.h"
#include <Arduino.h>
#include <EEPROM.h>

#define CONFIG_VERSION 105
#define CONFIG_ADDRESS 0
#define MAX_STEP_INDEX 15
#define MAX_BEAT_DIVIDER 6
#define MAX_TRIPLET_DIVIDER 7
#define MAX_SHUFFLE MAX_STEP_INDEX
#define MAX_MUTATION 37
#define MUTATION_FACTOR 100

Track::Track() {
 // load();
  reset();
}

void Track::updatePattern(int position) {
  int step = track.start + position;
  bitWrite(track.pattern, step, !bitRead(track.pattern, step));
  resetPattern();
  change = true;
}

void Track::rotatePattern(int offset) {
  rotate(track.pattern, track.start, track.end, offset);
  resetPattern();
  change = true;
}

void Track::setStart(int offset) {
  track.start += offset;
  Utilities::bound(track.start, 0, track.end);
  resetLength();
  resetPattern();
  change = true;
}

void Track::setEnd(int offset) {
  track.end += offset;
  Utilities::bound(track.end, track.start, MAX_STEP_INDEX);
  resetLength();
  resetPattern();
  change = true;
}

void Track::setLength(int offset) {
  track.length += offset;
  Utilities::bound(track.length, 0, MAX_STEP_INDEX);
  Utilities::bound(track.offset, 0, track.length);
  Utilities::bound(track.density, 0, track.length);
  resetLength();
  resetPattern();
  change = true;
}

void Track::setDensity(int offset) {
  track.density += offset;
  Utilities::bound(track.density, 0, track.length);
  resetPattern();
  change = true;
}

void Track::setOffset(int offset) {
  track.offset += offset;
  Utilities::cycle(track.offset, 0, track.length);
  resetPattern();
  change = true;
}

void Track::nextPatternType() {
  int mode = (int)track.patternType;
  ++mode;
  Utilities::cycle(mode, PatternType::Programmed, PatternType::Euclidean);
  track.patternType = (PatternType) mode;
  resetLength();
  resetPattern();
  change = true;
}

void Track::setPlayMode(int offset) {
  int mode = (int) track.play;
  mode += offset;
  Utilities::cycle(mode, PlayMode::Forward, PlayMode::Random);
  track.play = (PlayMode) mode;
  state.forward = true;
  change = true;
}

void Track::setOutMode(int offset) {
  int mode = (int) track.out;
  mode += offset;
  Utilities::cycle(mode, OutMode::Trigger, OutMode::Gate);
  track.out = (OutMode) mode;
  change = true;
}

void Track::setDivider(int offset) {
  track.divider += offset;
  Utilities::bound(track.divider, 0, track.dividerType == DividerType::Beat ? MAX_BEAT_DIVIDER : MAX_TRIPLET_DIVIDER);
  resetDivision();
  change = true;
}

void Track::nextDividerType() {
  int mode = (int)track.dividerType;
  ++mode;
  Utilities::cycle(mode, DividerType::Beat, DividerType::Triplet);
  track.dividerType = (DividerType) mode;
  resetDivision();
  change = true;
}

void Track::setShuffle(int offset) {
  track.shuffle += offset;
  Utilities::bound(track.shuffle, 0, MAX_SHUFFLE);
  change = true;
}

void Track::setMutation(int offset) {
  track.mutation += offset;
  Utilities::bound(track.mutation, 0, MAX_MUTATION);
  change = true;
}

void Track::nextMutationSeed() {
  int mode = (int)track.mutationSeed;
  ++mode;
  Utilities::cycle(mode, MutationSeed::Original, MutationSeed::LastInverted);
  track.mutationSeed = (MutationSeed) mode;
  change = true;
}

int Track::getStart() {
  return track.start;
}

int Track::getEnd() {
  return track.end;
}

int Track::getLength() {
  return state.length;
}

int Track::getPattern() {
  return state.pattern;
}

int Track::getDivider() {
  return track.divider;
}

DividerType Track::getDividerType() {
  return track.dividerType;
}

int Track::getPosition() {
  return state.position;
}

int Track::getStep() {
  return bitRead(state.pattern, state.position);
}

PatternType Track::getPatternType() {
  return track.patternType;
}

PlayMode Track::getPlayMode() {
  return track.play;
}

OutMode Track::getOutMode() {
  return track.out;
}

int Track::getShuffle() {
  return track.shuffle;
}

int Track::getStepped() {
  return state.stepped;
}

int Track::getMutation() {
  return track.mutation;
}

MutationSeed Track::getMutationSeed(){
  return track.mutationSeed;
}

void Track::reset() {
  initialiseState();
}

void Track::stepOn() {
  ++state.beat;
  if (state.beat >= state.division) {
    state.beat = 0;
    stepPosition();
    state.stepped = true;
  } else {
    state.stepped = false;
  }
}

void Track::stepPosition() {
  switch(track.play) {
    case Forward:
      ++state.position;
      Utilities::cycle(state.position, 0, state.length);
    break;
    case Backward:
      --state.position;
      Utilities::cycle(state.position, 0, state.length);
    break;
    case Random:
      state.position = random(0, state.length + 1);
    break;
    case Pendulum:
      if (state.forward) ++state.position;
      else --state.position;
      if (Utilities::reverse(state.position, 0, state.length)) state.forward  = !state.forward ;
    break;
  }
  if(state.position == 0) mutate();
}

void Track::mutate() {
  int seed;
  switch(track.mutationSeed) {
    case MutationSeed::Original:
      resetPattern();
      seed = state.pattern;
      break;
    case MutationSeed::Last:
      seed = state.pattern;
      break;
    case MutationSeed::LastInverted:
      seed = state.pattern;
      seed = ~seed;
      break;
  }

  for (int index = 0; index <= state.length; ++index) {
    bool step = bitRead(seed, index);
    if (random(1, MUTATION_FACTOR) <= pow(track.mutation,2)) step = !step;
    bitWrite(state.pattern, index, step);
  }
}

//void Track::load() {
//  Settings settings;
//  EEPROM.get(CONFIG_ADDRESS, settings);
//  if(settings.version != CONFIG_VERSION) for( = 0; track < TRACKS; ++ track) initialiseTrack();
//  else for( = 0; track < TRACKS; ++ track) track = settings.track;
//}
//
//void Track::save() {
//  if (change) {
//    Settings settings = {{tracks[0], tracks[1], tracks[2]}, CONFIG_VERSION};
//    EEPROM.put(CONFIG_ADDRESS, settings);
//    change = false;
//  }
//}

void Track::initialiseTrack() {
  track.pattern = 0;
  track.start = 0;
  track.end = MAX_STEP_INDEX;
  track.length = MAX_STEP_INDEX;
  track.density = 0;
  track.offset = 0;
  track.divider = 0;
  track.shuffle = 0;
  track.play = PlayMode::Forward;
  track.out = OutMode::Trigger;
  track.patternType = PatternType::Programmed;
  track.dividerType = DividerType::Beat;
}

void Track::initialiseState() {
  state.position = 0;
  state.beat = 0;
  state.forward = true;
  state.stepped = false;
  resetLength();
  resetDivision();
  resetPattern();
}

void Track::resetLength() {
  switch(track.patternType) {
    case Programmed:
      state.length = track.end - track.start;
      break;
    case Euclidean:
      state.length = track.length;
      break;
  }
}

void Track::resetPattern() {
  switch(track.patternType) {
    case Programmed:
      resetProgrammed();
      break;
    case Euclidean:
      resetEuclidean();
      break;
  }
}

void Track::resetProgrammed() {
  state.pattern = 0;
  int index = 0;
  for (int step = track.start; step <= track.end; ++step) {
    bitWrite(state.pattern, index, bitRead(track.pattern, step));
    ++index;
  }
}

void Track::resetEuclidean() {
  state.pattern = 0;
  state.pattern = euclidean(track.length + 1, track.density + 1);
  if (track.offset > 0) rotate(state.pattern, 0, track.length, track.offset);
}

void Track::resetDivision() {
  Utilities::bound(track.divider, 0, track.dividerType == DividerType::Beat ? MAX_BEAT_DIVIDER : MAX_TRIPLET_DIVIDER);
  state.division = calculateDivision(track.divider, track.dividerType);
  state.beat = 0;
}

int Track::calculateDivision(int divider, DividerType type) {
  int division = 1;
  if (type == DividerType::Beat) division = 1 << divider;
  else if (type == DividerType::Triplet) division = (divider + 1) * 3;
  return division;
}

int Track::euclidean(int length, int density) {
  int euclidean = 0;
  if(density >= length) density = length;
  int level = 0;
  int divisor = length - density;
  int remainders[MAX_STEP_INDEX + 1];
  int counts[MAX_STEP_INDEX + 1];
  remainders[0] = density;
  do {
    counts[level] = divisor / remainders[level];
    remainders[level + 1] = divisor % remainders[level];
    divisor = remainders[level];
    ++level;
  } while (remainders[level] > 1);
  counts[level] = divisor;
  int pattern[MAX_STEP_INDEX + 1];
  build(pattern, level, counts, remainders);
  for(int step = 0; step < length; ++step) bitWrite(euclidean, step, pattern[step]);
  if(euclidean != 0) while(!bitRead(euclidean, 0)) rotate(euclidean, 0, length, 1);
  return euclidean;
}

void Track::build(int pattern[], int level, int counts[], int remainders[]) {
  int step = 0;
  build(pattern, step, level, counts, remainders);
}

void Track::build(int pattern[], int &step, int level, int counts[], int remainders[]) {
  if (level == -1) {
    pattern[step] = 0;
    ++step;
  } else if (level == -2)  {
    pattern[step] = 1;
    ++step;
  } else {
    for (int index = 0; index < counts[level]; ++index) build(pattern, step, level - 1, counts, remainders);
    if (remainders[level] != 0) build(pattern, step, level - 2, counts, remainders);
  }
}

void Track::rotate(int &pattern, int start, int end, int offset) {
  int original = pattern;
  int length = end - start;
  for(int index = start; index <= end; ++index) {
    int set = index + offset;
    if (set < start) set = set + length + 1;
    else if (set > end) set = set - length - 1;
    bitWrite(pattern, set, bitRead(original, index));
  }
}
