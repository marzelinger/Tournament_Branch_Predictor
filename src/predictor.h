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
#include <cmath>

using namespace std;

class Predictor{
public:
  virtual void train_predictor(unsigned int pc, bool outcome) = 0;
  virtual bool make_prediction(unsigned int pc) = 0;
};

class Counter{
public:
  int currCount;
  int numBits;
  void incrementCounter();
  void decrementCounter();
  bool getPrediction();
  Counter(int pNumBits);
};

class CorrelatedPredictor : public Predictor{
  private:
    int DIRPbits;
    int BHRbits;
    std::map<unsigned int, std::map<std::string, Counter*> > history;
    std::map<unsigned int, std::string> currentBHRMap;
    bool searchHistory(unsigned int pc, std::string bhr);
    Counter* getCounter(unsigned int pc, std::string bhr);
    std::string getBranchHistory(unsigned int pc);
  public:
    CorrelatedPredictor(int pNumDIRPBits, int pNumBHRBits);
    void train_predictor(unsigned int pc, bool outcome);
    bool make_prediction(unsigned int pc);
};

class BHT : public Predictor{
private:
  std::map<unsigned int, Counter*> history;
  int numBits;
  bool searchHistory(unsigned int pc);
  Counter* getCounter(unsigned int pc);
public:
  BHT(int numBits);
  void train_predictor(unsigned int pc, bool outcome);
  bool make_prediction(unsigned int pc);
};

class Chooser : public Predictor{
private:
  BHT* p1;
  CorrelatedPredictor* p2;
  BHT* table;
public:
  void train_predictor(unsigned int pc, bool outcome);
  bool make_prediction(unsigned int pc);
  Chooser(BHT* pP1, CorrelatedPredictor* pP2);
};

class Tournament_Predictor : public Predictor{
private:
  Chooser* chooser;
  BHT* p1;
  CorrelatedPredictor* p2;
public:
  void train_predictor(unsigned int pc, bool outcome);
  bool make_prediction(unsigned int pc);
  Tournament_Predictor(BHT* pP1, CorrelatedPredictor* pP2);
};



#endif
