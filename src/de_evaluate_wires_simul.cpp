
////////////////////////////////////////////////////////////////////////////////

// COMPANY:   COSIC, KU Leuven 
// AUTHOR:    Victor Arribas

////////////////////////////////////////////////////////////////////////////////

// BSD 3-Clause License

// Copyright (c) 2019, Victor Arribas
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

////////////////////////////////////////////////////////////////////////////////

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