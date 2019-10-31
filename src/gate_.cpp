
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
#include "FaultsMaxine.h"

Gate::Gate(){
    gateName_ = "";
    output_ = NULL;
    f_ = NULL;
};

Gate::Gate(std::string gateName){
	gateName_ = gateName;
	output_ = NULL;
	f_ = NULL;
};

Gate::Gate(const Gate &gt){
	gateName_ = gt.gateName_;
	gateType_ = gt.gateType_;

	inputs_ = gt.inputs_;
	output_ = gt.output_;
	f_ = gt.f_;
};

Gate::~Gate() {
//    cout << "Gate destructor of " << get_name() << endl;
//    Wire destructor is in charge of destroying the pins
    for (pins::iterator it = inputs_.begin(); it != inputs_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
    if (output_ != nullptr) delete output_;
    if (f_ != nullptr) delete f_;
//    cout << "END gate destructor" << endl;
};

void Gate::append_output_pin(Pin *p){
	this->output_ = p;
};

void Gate::append_input_pin(Pin *p){
	this->inputs_.push_back(p);
};

std::string Gate::get_name(){
	return this->gateName_;
};

Gate::GateType Gate::get_type(){
	return this->gateType_;
};

pins Gate::get_inputs(){
	return this->inputs_;
};

Pin* Gate::get_output(){
	return this->output_;
};

void Gate::eval_fault(bool value, gates& eventList, int currentClk){

	//If a gate has been faulted, apply the corresponding fault
	// std::cout << "eval_fault" << std::endl;
	if (output_->get_wire() != NULL)
	{
		if (((f_->active_ == 1) && (f_->cycle_ == currentClk)) || ((f_->active_ == 1) && (f_->cycle_ == -1)))
		{
			// std::cout << "FAULT ACTIVATED!!" << std::endl;
			if (f_->fType_ == FaultType::SEU)
			{
				output_->get_wire()->eval_wire(!value, eventList);
			}
			else if (f_->fType_ == FaultType::SA0)
			{
				output_->get_wire()->eval_wire(0, eventList);
			}
			else if (f_->fType_ == FaultType::SA1)
			{
				output_->get_wire()->eval_wire(1, eventList);
			}
			else
			{
				assert(false && "Incorrect kind of fault selected");
			}
			
		}
		else{
			// std::cout << "FAULT nnnooottt ACTIVATED!!" << std::endl;
			output_->get_wire()->eval_wire(value, eventList);
		}	
	}
		

	// std::cout << "Gate: " << this->get_name() << " outs value in wire " << output_->get_wire()->get_name() << " = " << output_->get_wire()->get_value() << std::endl;

}

bool Gate::is_faulty(){
	return this->f_->active_;
};

int Gate::get_when_is_faulty(){
	return this->f_->cycle_;
};
