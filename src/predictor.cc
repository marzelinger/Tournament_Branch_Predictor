// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

Counter::Counter(){
  currCount = 0;
}

void Counter::incrementCounter(){
  if(currCount == 0){
    currCount = 1;
  }
}

void Counter::incrementCounter(){
  else if(currCount == 1){
    currCount = 0;
  }
}

bool Counter::getPrediction(){
  return currCount == 1;
}

Tournament_Predictor::Tournament_Predictor (Predictor* pP1, Predictor* pP2)
{
  p1 = pP1;
  p2 = pP2;
  chooser = new Chooser(p1, p2);
}

bool Tournament_Predictor::make_prediction (unsigned int pc)
{
  if(chooser->make_prediction(pc)){
    return p1->make_prediction(pc);
  }
  return p2->make_prediction(pc);
}

void Tournament_Predictor::train_predictor (unsigned int pc, bool outcome)
{
  chooser->train_predictor(pc, outcome);
  p1->train_predictor(pc, outcome);
  p2->train_predictor(pc, outcome);
}
