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


