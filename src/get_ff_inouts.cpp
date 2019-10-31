
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

void Netlist::get_ff_inOuts(){
	
	// cout << "Getting DFF in/outs" << endl;
	for(size_t h = 0; h < components_.size(); ++h){
	  
	  	if ((components_[h]->gateType_ == Gate::DFF) || (components_[h]->gateType_ == Gate::SDFF))
	  	{
	  		outputsFF_.push_back(components_[h]->inputs_[0]->wire_);

	  		inputsFF_.push_back(components_[h]->output_->wire_);
	  	}
	}

	// cout << "END Getting DFF in/outs" << endl;

	//Map the new in/outs
	for (size_t k = 0; k < inputsFF_.size(); ++k){
	  newInputsMap_[inputsFF_[k]->name_] = inputsFF_[k];
	   // std::cout << "Map new input: " << inputsFF_[k]->get_name() << "#" << std::endl;
	}

	//Map outs name->wire, so it is easier to search and retrieve an output
	for (size_t k = 0; k < outputsFF_.size(); ++k){
	  newOutputsMap_[outputsFF_[k]->name_] = outputsFF_[k];
	   // std::cout << "Map new out: " << outputsFF_[k]->get_name() << "#" << std::endl;
	}
}