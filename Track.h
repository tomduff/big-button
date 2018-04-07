#ifndef Track_h_
#define Track_h_

#include <Arduino.h>
#include "Io.h"

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
  int pattern;
  int start;
  int end;
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
  int pattern;
  int length;
  int position;
  bool forward;
  bool stepped;
  int beat;
  int division;
};

class Track {
public:
  Track();
  void updatePattern(int position);
  void rotatePattern(int offset);
  void setOffset(int offset);
  void setDensity(int offset);
  void setLength(int offset);
  void setStart(int offset);
  void setEnd(int offset);
  void setPlayMode(int offset);
  void setOutMode(int offset);
  void setDivider(int offset);
  void nextPatternType();
  void nextDividerType();
  void setShuffle(int offset);
  void setMutation(int offset);
  void nextMutationSeed();
  int getStart();
  int getEnd();
  int getLength();
  int getPattern();
  int getPosition();
  int getDivider();
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
  void reset();
private:
  bool change = false;
  TrackDef track;
  TrackState state;
  void stepPosition();
  void mutate();
//  void load();
  void initialiseTrack();
  void initialiseState();
  void resetLength();
  void resetDivision();
  void resetPattern();
  void resetProgrammed();
  void resetEuclidean();
  int calculateDivision(int divider, DividerType type);
  int euclidean(int length, int density);
  void build(int pattern[], int level, int counts[], int remainders[]);
  void build(int pattern[], int &step, int level, int counts[], int remainders[]);
  void rotate(int &pattern, int start, int end, int offset);
};

#endif
