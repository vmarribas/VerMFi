
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

Subcircuit::Subcircuit(){
    ident_ = "Combinational_subcirc_";
  };

Subcircuit::~Subcircuit(){
//    for (wires::iterator it = subcircuitInputs_.begin(); it != subcircuitInputs_.end(); ++it) {
//        delete *it;
//    }
//    for (wires::iterator it = subcircuitRandom_.begin(); it != subcircuitRandom_.end(); ++it) {
//        delete *it;
//    }
//    for (wires::iterator it = subcircuitOutputs_.begin(); it != subcircuitOutputs_.end(); ++it) {
//        delete *it;
//    }
//    for (std::vector<wires>::iterator it = subcircuitSharedVariables_.begin(); it != subcircuitSharedVariables_.end(); ++it) {
//        for (wires::iterator itt = it->begin(); itt != it->end(); ++itt) {
//            delete *itt;
//        }
//    }
//    
//    for (gates::iterator it = subcircuitGates_.begin(); it != subcircuitGates_.end(); ++it) {
//        delete *it;
//    }
}

std::string Subcircuit::get_ident(){
	return this->ident_;
};

wires Subcircuit::get_subcircInputs(){
	return this->subcircuitInputs_;
};

wires Subcircuit::get_subcircRandoms(){
	return this->subcircuitRandom_;
};

wires Subcircuit::get_subcircOuts(){
	return this->subcircuitOutputs_;
};

std::vector<wires> Subcircuit::get_subcircSharedVars(){
	return this->subcircuitSharedVariables_;
};

gates Subcircuit::get_subcircGates(){
	return this->subcircuitGates_;
};


