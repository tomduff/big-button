#ifndef Track_h_
#define Track_h_

#include <Arduino.h>
#include "Io.h"

#define MIN_STEPS 1
#define MAX_STEPS 32
#define MAX_BEAT_DIVIDER 6
#define MAX_TRIPLET_DIVIDER 7
#define MAX_SHUFFLE 15
#define MAX_MUTATION 37

enum PlayMode {
  Forward = 0,
  Backward = 1,
  Pendulum = 2,
  Random = 3
};

enum PatternType {
  Programmed,
  Euclidean
};

enum DividerType {
  Beat,
  Triplet
};

enum MutationSeed {
  Original,
  Last,
  LastInverted
};

struct TrackDef {
  long pattern;
  byte length;
  byte density;
  byte offset;
  byte divider;
  byte shuffle;
  byte mutation;
  PlayMode play;
  OutMode out;
  PatternType patternType;
  DividerType dividerType;
  MutationSeed mutationSeed;
};

struct TrackState {
  long pattern;
  byte length;
  byte position;
  bool forward;
  bool stepped;
  byte beat;
  int division;
  bool fill;
};

class Track {
public:
  Track();
  void initialise();
  void clearPattern();
  void resetPattern();
  void setPattern(byte value);
  void setNextPattern(byte value);
  void setPattern(byte step, byte value);
  void updatePattern(byte position);
  void rotatePattern(byte offset);
  void setOffset(byte offset);
  void setDensity(byte density);
  void setLength(byte length);
  void setPlayMode(byte offset);
  void setOutMode(byte offset);
  void setDivider(byte offset);
  void nextPatternType();
  void nextDividerType();
  void setShuffle(byte offset);
  void setMutation(byte offset);
  void nextMutationSeed();
  void setFill(bool fill);
  byte getLength();
  byte getPattern();
  byte getPosition();
  byte getDivider();
  byte getDensity();
  byte getStep();
  byte getStepped();
  byte getMutation();
  PatternType getPatternType();
  DividerType getDividerType();
  PlayMode getPlayMode();
  OutMode getOutMode();
  MutationSeed getMutationSeed();
  byte getShuffle();
  void stepOn();
//  void save();
  void clear();
  void reset();
private:
  bool change = false;
  TrackDef track;
  TrackState state;
  byte stepOn(byte current);
  void mutate();
//  void load();
  void initialiseTrack();
  void initialiseState();
  void resetLength();
  void resetDivision();
  void resetProgrammed();
  void resetEuclidean();
  int calculateDivision(int divider, DividerType type);
  long euclidean(byte density);
  void build(int pattern[], int level, int counts[], int remainders[]);
  void build(int pattern[], int &step, int level, int counts[], int remainders[]);
  void rotate(long &pattern, int offset);
};

#endif
