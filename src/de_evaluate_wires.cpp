
#include "VerifTool.h"

void VerifTool::de_evaluate_wires(){

  for(size_t i = 0; i < nl_.sharedVariables_.size(); i++){
    for(size_t j = 0; j < nl_.sharedVariables_[i].size(); j++){
      nl_.sharedVariables_[i][j]->de_evaluate_wire();
    }
  }
  
  for(size_t j = 0; j < nl_.wires_.size(); j++){
    nl_.wires_[j]->de_evaluate_wire();
  }
  
  for(size_t j = 0; j < nl_.outs_.size(); j++){
    nl_.outs_[j]->de_evaluate_wire();
  }

  for(size_t j = 0; j < nl_.randomBits_.size(); j++){
    nl_.randomBits_[j]->de_evaluate_wire();
  }

}