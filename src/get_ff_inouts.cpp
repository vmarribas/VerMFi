
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