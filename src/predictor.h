#ifndef PREDICTOR_H
#define PREDICTOR_H

/*
  Define all your tables and their sizes here.
  All tables must be statically-sized.
  Please don't call malloc () or any of its relatives within your
  program.  The predictor will have a budget, namely (32K + 256) bits
  (not bytes).  That encompasses all storage (all tables, plus GHR, if
  necessary -- but not PC) used for the predictor.  That may not be
  the amount of storage your predictor uses, however -- for example,
  you may implement 2-bit predictors with a table of ints, in which
  case the simulator will use more memory -- that's okay, we're only
  concerned about the memory used by the simulated branch predictor.
*/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <math.h>

#include <vector>
#include <iostream>
#include <iterator>
#include <map>

using namespace std;

class Predictor{
public:
  virtual void train_predictor(unsigned int pc, bool outcome) = 0;
  virtual bool make_prediction() = 0;
};

class Counter{
public:
  int currCount;
  void incrementCounter();
  void decrementCounter();
  Counter();
};

class Chooser : public Predictor{
private:
  Predictor p1, p2;
public:
  void train_predictor(unsigned int pc, bool outcome);
  bool make_prediction();
  Chooser(Predictor* pP1, Predictor* pP2);
}

class Tournament_Predictor : public Predictor {
private:
  Chooser* chooser;
  Predictor p1, p2;
public:
  void train_predictor(unsigned int pc, bool outcome);
  bool make_prediction();
  Tournament_Predictor(Predictor* pP1, Predictor* pP2);
};



#endif
