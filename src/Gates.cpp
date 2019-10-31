
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


#include "Gates.h"

ANDgate::ANDgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::AND;
	output_ = NULL;
};
void ANDgate::eval_gate(gates& eventList){
	//AND gate
    // std::cout << "Helooooooo! AND!! " << std::endl;
    bool value = 1;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      	if (!inputs_[i]->get_wire()->get_is_evaluated()){
      		allEvaluated = 0;
      		break;
      	}
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      	for(size_t i = 0; i < inputs_.size(); i++){
	       	value &= inputs_[i]->get_wire()->get_value();
      	}
      	output_->get_wire()->eval_wire(value, eventList);
	  }
}
void ANDgate::eval_gate(gates& eventList, int currentClk){
  //AND gate
    // std::cout << "Helooooooo! AND!! " << std::endl;
    bool value = 1;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
        if (!inputs_[i]->get_wire()->get_is_evaluated()){
          allEvaluated = 0;
          break;
        }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
        for(size_t i = 0; i < inputs_.size(); i++){
          value &= inputs_[i]->get_wire()->get_value();
        }
        this->eval_fault(value, eventList, currentClk);
    }
}

NANDgate::NANDgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::NAND;
};
void NANDgate::eval_gate(gates& eventList){	
	//NAND gate
    // std::cout << "Helooooooo! NAND!! " << std::endl;
    bool value = 1;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      	if (!inputs_[i]->get_wire()->get_is_evaluated()){
      		allEvaluated = 0;
      	break;
      	}
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      	for(size_t i = 0; i < inputs_.size(); i++){
	       value &= inputs_[i]->get_wire()->get_value();
      	}
      	output_->get_wire()->eval_wire(value, eventList);
      	// std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      	// << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }
}
void NANDgate::eval_gate(gates& eventList, int currentClk){ 
  //NAND gate
    // std::cout << "Helooooooo! NAND!! " << std::endl;
    bool value = 1;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
        if (!inputs_[i]->get_wire()->get_is_evaluated()){
          allEvaluated = 0;
        break;
        }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
        for(size_t i = 0; i < inputs_.size(); i++){
         value &= inputs_[i]->get_wire()->get_value();
        }
        this->eval_fault(!value, eventList, currentClk);
        // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
        // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }
}

ORgate::ORgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::OR;
};
void ORgate::eval_gate(gates& eventList){
	//OR gate
    // std::cout << "Helooooooo! OR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	allEvaluated = 0;
      	break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
	       value |= inputs_[i]->get_wire()->get_value();
      }
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void ORgate::eval_gate(gates& eventList, int currentClk){
  //OR gate
    // std::cout << "Helooooooo! OR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
         value |= inputs_[i]->get_wire()->get_value();
      }
      this->eval_fault(value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

NORgate::NORgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::NOR;
};
void NORgate::eval_gate(gates& eventList){
	//NOR gate
    // std::cout << "Helooooooo! NOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	allEvaluated = 0;
      	break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
	       value |= inputs_[i]->get_wire()->get_value();
      }
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void NORgate::eval_gate(gates& eventList, int currentClk){
  //NOR gate
    // std::cout << "Helooooooo! NOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
         value |= inputs_[i]->get_wire()->get_value();
      }
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

XORgate::XORgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::XOR;
};
void XORgate::eval_gate(gates& eventList){
	// XOR gate
    // std::cout << "Helooooooo! XOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	allEvaluated = 0;
      	break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
	       value ^= inputs_[i]->get_wire()->get_value();
      }
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void XORgate::eval_gate(gates& eventList, int currentClk){
  // XOR gate
    // std::cout << "Helooooooo! XOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    // cout << "Evaluated checked" << endl;
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
         value ^= inputs_[i]->get_wire()->get_value();
      }
      // cout << "Evaluated" << endl;
      this->eval_fault(value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

XNORgate::XNORgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::XNOR;
};
void XNORgate::eval_gate(gates& eventList){
	//XNOR gate
    // std::cout << "Helooooooo! XNOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	allEvaluated = 0;
      	break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
	       value ^= inputs_[i]->get_wire()->get_value();
      }
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void XNORgate::eval_gate(gates& eventList, int currentClk){
  //XNOR gate
    // std::cout << "Helooooooo! XNOR!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      for(size_t i = 0; i < inputs_.size(); i++){
         value ^= inputs_[i]->get_wire()->get_value();
      }
      this->eval_fault(!value, eventList, currentClk);
      // if (this->get_name() == "_1874_")
      // {
      //     std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value()
      //     << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
      // }
        
    } 
}

MUXgate::MUXgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::MUX;
};
void MUXgate::eval_gate(gates& eventList){
	//MUX gate
    // std::cout << "Helooooooo! MUX!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & !inputs_[2]->get_wire()->get_value()) | (inputs_[1]->get_wire()->get_value() & inputs_[2]->get_wire()->get_value()));
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void MUXgate::eval_gate(gates& eventList, int currentClk){
  //MUX gate
    // std::cout << "Helooooooo! MUX!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & !inputs_[2]->get_wire()->get_value()) | (inputs_[1]->get_wire()->get_value() & inputs_[2]->get_wire()->get_value()));
      this->eval_fault(value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

AOI21gate::AOI21gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::AOI21;
};
void AOI21gate::eval_gate(gates& eventList){
	//AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void AOI21gate::eval_gate(gates& eventList, int currentClk){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value());
      #else
      value = ((inputs_[2]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | inputs_[0]->get_wire()->get_value());
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI21gate::OAI21gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::OAI21;
};
void OAI21gate::eval_gate(gates& eventList){
	//OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & inputs_[2]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void OAI21gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & inputs_[2]->get_wire()->get_value());
      #else
      value = ((inputs_[2]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & inputs_[0]->get_wire()->get_value());
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

AOI211gate::AOI211gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::AOI211;
};
void AOI211gate::eval_gate(gates& eventList){
	//AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = (((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value()) | inputs_[3]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void AOI211gate::eval_gate(gates& eventList, int currentClk){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = (((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value()) | inputs_[3]->get_wire()->get_value());
      #else
      value = (((inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value()) | inputs_[1]->get_wire()->get_value()) | inputs_[0]->get_wire()->get_value());
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI211gate::OAI211gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::OAI211;
};
void OAI211gate::eval_gate(gates& eventList){
	//OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = (((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & inputs_[2]->get_wire()->get_value()) & inputs_[3]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void OAI211gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = (((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & inputs_[2]->get_wire()->get_value()) & inputs_[3]->get_wire()->get_value());
      #else
      value = (((inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value()) & inputs_[1]->get_wire()->get_value()) & inputs_[0]->get_wire()->get_value());
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

AOI22gate::AOI22gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::AOI22;
};
void AOI22gate::eval_gate(gates& eventList){
	//AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | (inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value()));
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void AOI22gate::eval_gate(gates& eventList, int currentClk){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | (inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value()));
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI22gate::OAI22gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::OAI22;
};
void OAI22gate::eval_gate(gates& eventList){
	//OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & (inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value()));
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " 
      // 			<< inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value()
      // 			<< ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void OAI22gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & (inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value()));
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " 
      //      << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

AOI221gate::AOI221gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::AOI221;
};
void AOI221gate::eval_gate(gates& eventList){
	//AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | 
      		  ((inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value()) | inputs_[4]->get_wire()->get_value()));
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void AOI221gate::eval_gate(gates& eventList, int currentClk){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | 
            ((inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value()) | inputs_[4]->get_wire()->get_value()));
      #else
      value = ((inputs_[1]->get_wire()->get_value() & inputs_[2]->get_wire()->get_value()) | 
            ((inputs_[3]->get_wire()->get_value() & inputs_[4]->get_wire()->get_value()) | inputs_[0]->get_wire()->get_value()));
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI221gate::OAI221gate(std::string name){
	gateName_ = name;
	gateType_ = Gate::OAI221;
};
void OAI221gate::eval_gate(gates& eventList){
	//OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & 
      		  ((inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value()) & inputs_[4]->get_wire()->get_value()));
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      // 			<< inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      // 			<< ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    }	
}
void OAI221gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      #ifndef YOSYS
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & 
            ((inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value()) & inputs_[4]->get_wire()->get_value()));
      #else
      value = ((inputs_[1]->get_wire()->get_value() | inputs_[2]->get_wire()->get_value()) & 
            ((inputs_[3]->get_wire()->get_value() | inputs_[4]->get_wire()->get_value()) & inputs_[0]->get_wire()->get_value()));
      #endif
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      //      << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

AOI222gate::AOI222gate(std::string name){
  gateName_ = name;
  gateType_ = Gate::AOI222;
};
void AOI222gate::eval_gate(gates& eventList){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | 
               (inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value())) | (inputs_[4]->get_wire()->get_value() & inputs_[5]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}
void AOI222gate::eval_gate(gates& eventList, int currentClk){
  //AOI gate
    // std::cout << "Helooooooo! AOI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()) | 
               (inputs_[2]->get_wire()->get_value() & inputs_[3]->get_wire()->get_value())) | (inputs_[4]->get_wire()->get_value() & inputs_[5]->get_wire()->get_value());
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " << inputs_[2]->get_wire()->get_value()
      // << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI222gate::OAI222gate(std::string name){
  gateName_ = name;
  gateType_ = Gate::OAI222;
};
void OAI222gate::eval_gate(gates& eventList){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & 
               (inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value())) & (inputs_[4]->get_wire()->get_value() | inputs_[5]->get_wire()->get_value());
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      //      << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}
void OAI222gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = ((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) & 
               (inputs_[2]->get_wire()->get_value() | inputs_[3]->get_wire()->get_value())) & (inputs_[4]->get_wire()->get_value() | inputs_[5]->get_wire()->get_value());
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      //      << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

OAI33gate::OAI33gate(std::string name){
  gateName_ = name;
  gateType_ = Gate::OAI33;
};
void OAI33gate::eval_gate(gates& eventList){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = (((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value()) &
              ((inputs_[3]->get_wire()->get_value() | inputs_[4]->get_wire()->get_value()) | inputs_[5]->get_wire()->get_value()) );
      output_->get_wire()->eval_wire(value, eventList);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      //      << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}
void OAI33gate::eval_gate(gates& eventList, int currentClk){
  //OAI gate
    // std::cout << "Helooooooo! OAI!! " << std::endl;
    bool value = 0;
    bool allEvaluated = 1;
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_
      value = (((inputs_[0]->get_wire()->get_value() | inputs_[1]->get_wire()->get_value()) | inputs_[2]->get_wire()->get_value()) &
              ((inputs_[3]->get_wire()->get_value() | inputs_[4]->get_wire()->get_value()) | inputs_[5]->get_wire()->get_value()) );
      this->eval_fault(!value, eventList, currentClk);
      // std::cout << "Evaluating gate: " << gateType_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ", " << inputs_[1]->get_wire()->get_value() << ", " 
      //      << inputs_[2]->get_wire()->get_value() << ", " << inputs_[3]->get_wire()->get_value() << ", " << inputs_[4]->get_wire()->get_value()
      //      << ". "<< output_->get_wire()->get_name() << " => " << output_->get_wire()->get_value() << std::endl;
    } 
}

INVgate::INVgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::INV;
};
void INVgate::eval_gate(gates& eventList){
	//INV gate
    // std::cout << "Helooooooo! INV!! " << std::endl;
    bool allEvaluated = 1;
    
    if(inputs_.empty()){
      output_->get_wire()->eval_wire('1', eventList);
    }else{
      for(size_t i = 0; i < inputs_.size(); i++){
      	if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	  allEvaluated = 0;
      	  break;
      	}
      }
      if (allEvaluated){
      	//All inputs_ evaluated, eval output_
      	output_->get_wire()->eval_wire(!inputs_[0]->get_wire()->get_value(), eventList);
        // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
      }
    }	
}
void INVgate::eval_gate(gates& eventList, int currentClk){
  //INV gate
    // std::cout << "Helooooooo! INV!! " << std::endl;
    bool allEvaluated = 1;
    
    if(inputs_.empty()){
      this->eval_fault('1', eventList, currentClk);
    }else{
      for(size_t i = 0; i < inputs_.size(); i++){
        if (!inputs_[i]->get_wire()->get_is_evaluated()){
          allEvaluated = 0;
          break;
        }
      }
      if (allEvaluated){
        //All inputs_ evaluated, eval output_
        this->eval_fault(!inputs_[0]->get_wire()->get_value(), eventList, currentClk);
        // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
      }
    } 
}

BUFFgate::BUFFgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::BUFF;
};
void BUFFgate::eval_gate(gates& eventList){
	//BUF gate
    bool allEvaluated = 1;
    // std::cout << "Helooooooo BUFFFF!" << std::endl;
    if(inputs_.empty()){
      output_->get_wire()->eval_wire('0', eventList);
    }else{
      for(size_t i = 0; i < inputs_.size(); i++){
      	if (!inputs_[i]->get_wire()->get_is_evaluated()){
      	  allEvaluated = 0;
      	  break;
      	}
      }
      if (allEvaluated){
      	//All inputs_ evaluated, eval output_
        output_->get_wire()->eval_wire(!inputs_[0]->get_wire()->get_value(), eventList);
        // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
      }
    }	
}
void BUFFgate::eval_gate(gates& eventList, int currentClk){
  //BUF gate
    bool allEvaluated = 1;
    // std::cout << "Helooooooo BUFFFF!" << std::endl;
    if(inputs_.empty()){
      this->eval_fault('0', eventList, currentClk);
    }else{
      for(size_t i = 0; i < inputs_.size(); i++){
        if (!inputs_[i]->get_wire()->get_is_evaluated()){
          allEvaluated = 0;
          break;
        }
      }
      if (allEvaluated){
        //All inputs_ evaluated, eval output_
        this->eval_fault(inputs_[0]->get_wire()->get_value(), eventList, currentClk);
        // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
      }
    } 
}

DFFgate::DFFgate(std::string name){
	gateName_ = name;
	gateType_ = Gate::DFF;
};
void DFFgate::eval_gate(gates& eventList){
	//INV gate
    // std::cout << "Helooooooo! INV!! " << std::endl;
    bool allEvaluated = 1;
    
    if (!inputs_[0]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
    }
    if (allEvaluated){
	    //All inputs_ evaluated, eval output_

	    if (output_->get_name() == "Q")
	    {
	        output_->get_wire()->eval_wire(inputs_[0]->get_wire()->get_value(), eventList);
	    }else
	    {
	        output_->get_wire()->eval_wire(!inputs_[0]->get_wire()->get_value(), eventList);        
	    // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
	    }
    }	
}
void DFFgate::eval_gate(){

	if (inputs_[0]->get_wire()->get_is_evaluated()){
        Q_ = inputs_[0]->get_wire()->get_value();
    }

}
void DFFgate::set_Q(bool val){
	this->Q_ = val;
}
bool DFFgate::get_Q(){
	return this->Q_;
}


SDFFgate::SDFFgate(std::string name){
  gateName_ = name;
  gateType_ = Gate::SDFF;
};
void SDFFgate::eval_gate(gates& eventList){
  //INV gate
    // std::cout << "Helooooooo! INV!! " << std::endl;
    bool allEvaluated = 1;
    bool value = 0;
    
    for(size_t i = 0; i < inputs_.size(); i++){
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
    }
    if (allEvaluated){
      //All inputs_ evaluated, eval output_

      if (output_->get_name() == "Q")
      {
          value = ((inputs_[0]->get_wire()->get_value() & !inputs_[2]->get_wire()->get_value()) | (inputs_[1]->get_wire()->get_value() & inputs_[2]->get_wire()->get_value()));
          output_->get_wire()->eval_wire(value, eventList);
      }else
      {
          output_->get_wire()->eval_wire(!value, eventList);        
      // std::cout << "Evaluating gate: " << gateName_ <<". inputs_ value: " << inputs_[0]->get_wire()->get_value() << ". "<< output_->get_wire()->name << " => " << output_->get_wire()->get_value() << std::endl;
      }
    } 
}
void SDFFgate::eval_gate(){

  bool allEvaluated = 1;

  for(size_t i = 0; i < 3; i++){ //We do not want to check all input pins coz the clk wire is not appended
      if (!inputs_[i]->get_wire()->get_is_evaluated()){
        allEvaluated = 0;
        break;
      }
  }
  if (allEvaluated){
      #ifndef YOSYS
      Q_ = ((inputs_[0]->get_wire()->get_value() & !inputs_[2]->get_wire()->get_value()) | (inputs_[1]->get_wire()->get_value() & inputs_[2]->get_wire()->get_value()));
      #else
      Q_ = ((inputs_[0]->get_wire()->get_value() & !inputs_[1]->get_wire()->get_value()) | (inputs_[2]->get_wire()->get_value() & inputs_[1]->get_wire()->get_value()));
      #endif
  }

}
void SDFFgate::set_Q(bool val){
  this->Q_ = val;
}
bool SDFFgate::get_Q(){
  return this->Q_;
}