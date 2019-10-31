
#include "VerifTool.h"

void VerifTool::de_evaluate_wires_simul(){
  
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

  if (nl_.start_ != NULL) nl_.start_->de_evaluate_wire();
  nl_.rst_->de_evaluate_wire();
  if (nl_.load_ != NULL)nl_.load_->de_evaluate_wire();

}