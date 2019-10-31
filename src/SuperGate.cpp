
////////////////////////////////////////////////////////////////////////////////

// COMPANY:     COSIC, KU Leuven 
// AUTHOR:      Victor Arribas

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

SuperGate::SuperGate(const SuperGate &sG){
	
	// cout << "Calling copy constructooooooooooooooorrrrrrr SuperGate!!!!!!!!!!" << endl;
	type_ = sG.type_;
	name_ = sG.name_;

	// for (int i = 0; i < inputs_.size(); ++i)
	// {
	// 	inputs_[i] = Pin(*sG.inputs_[i]);
	// }
    inputs_ = sG.inputs_;
    outputs_ = sG.outputs_;

	inputWires_ = sG.inputWires_;
	outputWires_ = sG.outputWires_;
	wires_ = sG.wires_;
	zeroOnes_ = sG.zeroOnes_;

	inputsMap_ = sG.inputsMap_;
	outsMap_ = sG.outsMap_;
	wiresMap_ = sG.wiresMap_;

	logicGates_ = sG.logicGates_;
	superGates_ = sG.superGates_;

	primaryInputs_ = sG.primaryInputs_;
	primaryOutputs_ = sG.primaryOutputs_;

};

SuperGate::~SuperGate(){
    
//    cout << "SuperGate destructor of " << type_ << endl;
    
    for (pins::iterator it = inputs_.begin(); it != inputs_.end(); ++it) {
        if (*it != nullptr){
            delete *it;
        }
    }
    for (pins::iterator it = outputs_.begin(); it != outputs_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    
    for (wires::iterator it = inputWires_.begin(); it != inputWires_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    for (wires::iterator it = outputWires_.begin(); it != outputWires_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    for (wires::iterator it = wires_.begin(); it != wires_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    for (wires::iterator it = zeroOnes_.begin(); it != zeroOnes_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    
//    cout << "FINISHED NORMAL WIRES DESTRUCTION" << endl;
    // No need to erase maps wires, we just did it
//    for (std::map<std::string, Wire*>::iterator it = inputsMap_.begin(); it != inputsMap_.end(); ++it) {
//        if (it->second != nullptr) delete it->second;
//    }
//    for (std::map<std::string, Wire*>::iterator it = outsMap_.begin(); it != outsMap_.end(); ++it) {
//        if (it->second != nullptr) delete it->second;
//    }
//    for (std::map<std::string, Wire*>::iterator it = wiresMap_.begin(); it != wiresMap_.end(); ++it) {
//        if (it->second != nullptr) delete it->second;
//    }
    
    
    for (gates::iterator it = logicGates_.begin(); it != logicGates_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
//    for (superGates::iterator it = superGates_.begin(); it != superGates_.end(); ++it) {
//        if (*it != nullptr) delete *it;
//    }
    for (gates::iterator it = primaryInputs_.begin(); it != primaryInputs_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    for (gates::iterator it = primaryOutputs_.begin(); it != primaryOutputs_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    
}
