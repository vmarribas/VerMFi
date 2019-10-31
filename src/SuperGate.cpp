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
