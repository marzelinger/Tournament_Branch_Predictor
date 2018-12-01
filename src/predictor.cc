
#include "predictor.h"

// COUNTER code
Counter::Counter(int pNumBits){
  currCount = 0;
  numBits = pNumBits;
}

void Counter::incrementCounter(){
  if(currCount < std::pow(2, numBits) - 1){
    currCount = currCount + 1;
  }
}

void Counter::decrementCounter(){
  if(currCount > 0){
    currCount = currCount - 1;
  }
}

bool Counter::getPrediction(){
  // printf("Current Count: %d  Prediction: %d\n",currCount,  currCount>= std::pow(2, numBits)/2);
  return currCount >= std::pow(2, numBits)/2;
}

//CorrelatedPredictor FUNCTIONS

CorrelatedPredictor::CorrelatedPredictor(int pNumDIRPBits, int pNumBHRBits){
    DIRPbits = pNumDIRPBits;
    BHRbits = pNumBHRBits;
}

bool CorrelatedPredictor::searchHistory(unsigned int pc, std::string bhr){
  std::map<unsigned int, std::map<std::string, Counter*> >::iterator it;
  it = history.find(pc);
  if(it!=history.end()){
    std::map<std::string, Counter*>::iterator it2;
    it2 = (it->second).find(bhr);
    return it2 != (it->second).end();
  }
  return false;
}

Counter* CorrelatedPredictor::getCounter(unsigned int pc, std::string bhr){
  if(!searchHistory(pc, bhr)){
    std::string s = "";
    for(int i = 0; i < BHRbits; i ++){
      s = s + "N";
    }
    currentBHRMap.insert(std::pair<unsigned int, std::string>(pc, s));

    std::pair<std::string, Counter*> innerMap = std::pair<std::string, Counter*>(s, new Counter(DIRPbits));
    history.insert(std::pair<unsigned int, std::map<std::string, Counter*> >(pc, std::map<std::string, Counter*>()));
    history.find(pc)->second.insert(innerMap);
  }
  return (history.find(pc)->second.find(bhr)->second);
}

std::string CorrelatedPredictor::getBranchHistory(unsigned int pc){
  std::map<unsigned int, std::string >::iterator it;
  it = currentBHRMap.find(pc);
  if(it==currentBHRMap.end()){
    std::string s = "";
    for(int i = 0; i < BHRbits; i ++){
      s = s + "N";
    }
    currentBHRMap.insert(std::pair<unsigned int, std::string>(pc, s));
  }
  return (currentBHRMap.find(pc)->second);
}

void CorrelatedPredictor::train_predictor(unsigned int pc, bool outcome){
    std::string currentBHR = getBranchHistory(pc);
    if(outcome){
      currentBHR = currentBHR + "T";
    }
    else{
      currentBHR = currentBHR + "N";
    }
    currentBHR = currentBHR.substr(1);
    currentBHRMap.insert(std::pair<unsigned int, std::string>(pc, currentBHR));
};

bool CorrelatedPredictor::make_prediction(unsigned int pc){
  std::string bhr = getBranchHistory(pc);
  return getCounter(pc, bhr)->getPrediction();
};


//BHT FUNCTIONS
BHT::BHT(int pNumBits){
  numBits = pNumBits;
}

bool BHT::searchHistory(unsigned int pc){
  std::map<unsigned int, Counter*>::iterator it;
  it = history.find(pc);
  return it != history.end();
}

Counter* BHT::getCounter(unsigned int pc){
  if(!searchHistory(pc)){
    history.insert(std::pair<unsigned int,Counter*>(pc, new Counter(numBits)));
  }
  return (history.find(pc)->second);
}

bool BHT::make_prediction(unsigned int pc){
  return getCounter(pc)->getPrediction();
}

void BHT::train_predictor(unsigned int pc, bool outcome){
  if(make_prediction(pc) == outcome){
      getCounter(pc)->incrementCounter();
  }
  else{
    getCounter(pc)->decrementCounter();
  }
}


// CHOOSER FUNCTIONS
Chooser::Chooser(BHT* pP1, CorrelatedPredictor* pP2){
  p1 = pP1;
  p2 = pP2;
  table = new BHT(2);
}

void Chooser::train_predictor(unsigned int pc, bool outcome){
  if(p1->make_prediction(pc) == outcome && p2->make_prediction(pc) !=outcome){
    //will make the BHT increment
    table->train_predictor(pc, table->make_prediction(pc));
  }
  else if(p1->make_prediction(pc) != outcome && p2->make_prediction(pc) == outcome){
    //will make the BHT decrement
    table->train_predictor(pc, !table->make_prediction(pc));
  }
}

bool Chooser::make_prediction(unsigned int pc){
  return table->make_prediction(pc);
}

// TOURNAMENT_PREDICTOR Functions
Tournament_Predictor::Tournament_Predictor (BHT* pP1, CorrelatedPredictor* pP2)
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
