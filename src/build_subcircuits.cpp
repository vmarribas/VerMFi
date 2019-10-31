
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

void Netlist::build_subcircuits(){
	
	wires total_outs = outs_;
	int sub = 1;

	//Merge Primary outputs with Outputs created by flipflops
	total_outs.insert(total_outs.end(), outputsFF_.begin(), outputsFF_.end());
	// std::cout << total_outs.size() << std::endl;
	

	while(!total_outs.empty()){

		// std::cout <<  "////////////////////////   New subcircuit " << sub << std::endl;
		std::string sub_name = std::to_string(sub);
		Subcircuit *combinat_logic = new Subcircuit();
		combinat_logic->ident_ += sub_name;

		subcircuit_search_back(total_outs[0], combinat_logic, total_outs);

		// cout<< "Finished with subcircuit: " << sub << "////////////////////////" << endl;
		//Once subcircuit is done, associate the correspondin sensitive data
		//Pick one of the inputs and look to which layer it corresponds
		bool found = 0;
		for (size_t i = 0; i < regsSharedVariables_.size(); ++i)
		{
			for (size_t j = 0; j < regsSharedVariables_[i].size(); ++j)
			{
				//Look for the element on each row of the layer matrix, if it is included, this is the layer
				if (std::find(regsSharedVariables_[i][j].begin(), regsSharedVariables_[i][j].end(), combinat_logic->subcircuitInputs_[0]) != regsSharedVariables_[i][j].end()){
					combinat_logic->subcircuitSharedVariables_ = regsSharedVariables_[i];
					found = 1;
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
		if (!found)
		{
			//This means the input to this sub-circuit is a PI
			combinat_logic->subcircuitSharedVariables_ = sharedVariables_;
		}

		combinationalSubcircs_.push_back(combinat_logic);

		sub++;
		// std::cout << "Outs still remaining: " << total_outs.size() << std::endl;;
	}

	cout << "Finished creating Subcircuits, with total of ==> " << combinationalSubcircs_.size() << endl;

	// Associate now to every output the corresponding sensitive data matrix
	for (size_t i = 0; i < outs_.size(); ++i)
	{
		wires inputDependencies;

		if ((outs_[i]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
			(outs_[i]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
			(outs_[i]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
		{
			search_back(outs_[i]->get_pinFrom()->get_gate(), inputDependencies);
		}

		bool found = 0;
		for (size_t ii = 0; ii < inputDependencies.size(); ++ii)
		{
			for (size_t k = 0; k < regsSharedVariables_.size(); ++k)
			{
				for (size_t j = 0; j < regsSharedVariables_[k].size(); ++j)
				{
					//Look for the element on each row of the layer matrix, if it is included, this is the layer
					if (std::find(regsSharedVariables_[k][j].begin(), regsSharedVariables_[k][j].end(), inputDependencies[ii]) != regsSharedVariables_[k][j].end()){
						outsSensitiveDataMap_[outs_[i]->get_name()] = regsSharedVariables_[k];
						found = 1;
						break;
					}
				}
				if (found) break;
			}
			if (found) break;
		}

		if (!found) outsSensitiveDataMap_[outs_[i]->get_name()] = sharedVariables_;

		// cout << "For output: " << outs_[i]->get_name() << ", the sensitive data is: " << outsSensitiveDataMap_[outs_[i]->get_name()][0][0]->get_name() << endl;
	}

	for (size_t i = 0; i < outputsFF_.size(); ++i)
	{
		wires inputDependencies;

		if ((outputsFF_[i]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
			(outputsFF_[i]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
			(outputsFF_[i]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
		{
			search_back(outputsFF_[i]->get_pinFrom()->get_gate(), inputDependencies);
		}

		bool found = 0;
		for (size_t ii = 0; ii < inputDependencies.size(); ++ii)
		{
			for (size_t k = 0; k < regsSharedVariables_.size(); ++k)
			{
				for (size_t j = 0; j < regsSharedVariables_[k].size(); ++j)
				{
					//Look for the element on each row of the layer matrix, if it is included, this is the layer
					if (std::find(regsSharedVariables_[k][j].begin(), regsSharedVariables_[k][j].end(), inputDependencies[ii]) != regsSharedVariables_[k][j].end()){
						outsSensitiveDataMap_[outputsFF_[i]->get_name()] = regsSharedVariables_[k];
						found = 1;
						break;
					}
				}
				if (found) break;
			}
			if (found) break;
		}

		if (!found) outsSensitiveDataMap_[outputsFF_[i]->get_name()] = sharedVariables_;

		// cout << "For output: " << outputsFF_[i]->get_name() << ", the sensitive data is: " << outsSensitiveDataMap_[outputsFF_[i]->get_name()][0][0]->get_name() << endl;
	}

	// for (int i = 0; i < combinationalSubcircs_.size(); ++i)
	// {
	// 	std::cout << "/////////////NEW SUBCIRC////////////////" << std::endl;
	// 	std::cout << combinationalSubcircs_[i]->ident << std::endl;

	// 	std::cout << "Inputs: ";
	// 	for (int j = 0; j < combinationalSubcircs_[i]->subcircuitInputs_.size(); ++j)
	// 	{
	// 		std::cout << combinationalSubcircs_[i]->subcircuitInputs_[j]->name << "; ";
	// 	}
	// 	std::cout << std::endl;

	// 	std::cout << "Outputs: ";
	// 	for (int j = 0; j < combinationalSubcircs_[i]->subcircuitOutputs_.size(); ++j)
	// 	{
	// 		std::cout << combinationalSubcircs_[i]->subcircuitOutputs_[j]->name << "; ";
	// 	}
	// 	std::cout << std::endl;

	// 	std::cout << "Gates: ";
	// 	for (int j = 0; j < combinationalSubcircs_[i]->subcircuitGates_.size(); ++j)
	// 	{
	// 		std::cout << combinationalSubcircs_[i]->subcircuitGates_[j]->gateName_ << "; ";
	// 	}
	// 	std::cout << std::endl;

	// }
}