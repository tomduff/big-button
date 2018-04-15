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
  int length;
  int density;
  int offset;
  int divider;
  int shuffle;
  int mutation;
  PlayMode play;
  OutMode out;
  PatternType patternType;
  DividerType dividerType;
  MutationSeed mutationSeed;
};

struct TrackState {
  long pattern;
  int length;
  int position;
  bool forward;
  bool stepped;
  int beat;
  int division;
  bool fill;
};

class Track {
public:
  Track();
  void initialise();
  void clearPattern();
  void resetPattern();
  void setStep(int value);
  void setNextStep(int value);
  void setStep(int step, int value);
  void updateStep(int position);
  void rotatePattern(int offset);
  void setOffset(int offset);
  void setDensity(int density);
  void setLength(int length);
  void setPlayMode(int offset);
  void setOutMode(int offset);
  void setDivider(int offset);
  void nextPatternType();
  void nextDividerType();
  void setShuffle(int offset);
  void setMutation(int offset);
  void nextMutationSeed();
  void setFill(bool fill);
  int getLength();
  int getPattern();
  int getPosition();
  int getDivider();
  int getDensity();
  int getOffset();
  int getStep();
  int getStepped();
  int getMutation();
  PatternType getPatternType();
  DividerType getDividerType();
  PlayMode getPlayMode();
  OutMode getOutMode();
  MutationSeed getMutationSeed();
  int getShuffle();
  void stepOn();
//  void save();
  void clear();
  void reset();
private:
  bool change = false;
  TrackDef track;
  TrackState state;
  int stepOn(int current);
  void mutate();
//  void load();
  void initialiseTrack();
  void initialiseState();
  void resetLength();
  void resetDivision();
  void resetProgrammed();
  void resetEuclidean();
  int calculateDivision(int divider, DividerType type);
  long euclidean(int density);
  void build(int pattern[], int level, int counts[], int remainders[]);
  void build(int pattern[], int &step, int level, int counts[], int remainders[]);
  void rotate(long &pattern, int offset);
};

#endif
