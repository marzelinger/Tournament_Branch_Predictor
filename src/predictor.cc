// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

Counter::Counter(){
  currCount = 0;
}

void Counter::updatePrediction(bool correctPrediction){
  if(correctPrediction && currCount == 0){
    currCount = 1;
  }
  else if(!correctPrediction && currCount == 1){
    currCount = 0;
  }
}

bool Counter::getPrediction(){
  return currCount == 1;
}

PREDICTOR::PREDICTOR ()
{
  c = new Counter();
}

bool PREDICTOR::make_prediction (unsigned int pc)
{
  return c->getPrediction();
}

void PREDICTOR::train_predictor (unsigned int pc, bool outcome)
{
  // printf("Outcome: %d, Guess: %d, ", outcome, c->getPrediction());
  c->updatePrediction(outcome);
  // printf("UpdatedGuess: %d\n", c->getPrediction());
}
