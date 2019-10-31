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
