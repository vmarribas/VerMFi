
////////////////////////////////////////////////////////////////////////////////

// COMPANY:		COSIC, KU Leuven 
// AUTHOR:		Victor Arribas

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

void Netlist::search_back(Gate *g, wires &input_deps){

	// std::cout << "Getting in search back! Gate: " << g->gateName_  << std::endl;
	for (size_t i = 0; i < g->inputs_.size(); ++i)
	{
		// std::cout << "Input: " << g->inputs_[i]->wire_->name_ << std::endl;
		if ((inputsMap_.find(g->inputs_[i]->wire_->name_) != inputsMap_.end()) || (newInputsMap_.find(g->inputs_[i]->wire_->name_) != newInputsMap_.end()))	
		{
			//Found an input, stop and store the dependency
			// std::cout << "Saving input dep: " << g->inputs_[i]->wire_->name_ << std::endl;
			if (std::find(input_deps.begin(), input_deps.end(), g->inputs_[i]->wire_) == input_deps.end()){
				input_deps.push_back(g->inputs_[i]->wire_);
			}
		}
		else if ((g->inputs_[i]->wire_->name_ == "one") or 
				 (g->inputs_[i]->wire_->name_ == "zero") or 
				 (randomsMap_.find(g->inputs_[i]->wire_->name_) != randomsMap_.end()) or 
				 (g->inputs_[i]->wire_->get_is_PI()) or 
				 ((g->inputs_[i]->wire_->pinFrom_ != NULL) && (g->inputs_[i]->wire_->pinFrom_->gate_->get_type() == Gate::DFF)) or
				 ((g->inputs_[i]->wire_->pinFrom_ != NULL) && (g->inputs_[i]->wire_->pinFrom_->gate_->get_type() == Gate::SDFF))
				 )
		{
			// Stop backwards propagation
			// return; IF WE RETURN WE DO NOT LET THE NEXT INPUTS TO BE EVALUATED, WE STOP THE RECURSIVENESS
		}
		else{
			//The input is still comming from another gate, go to that gate
			// std::cout << "Recusive function goes to gate: " << g->inputs_[i]->wire_->pinFrom_->gate_->gateName_ << std::endl;

			search_back(g->inputs_[i]->wire_->pinFrom_->gate_, input_deps);
			// cout << "Back to gate: " << g->get_name() << endl;
		}
	}
}
