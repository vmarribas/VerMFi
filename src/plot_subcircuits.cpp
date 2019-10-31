
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


	void VerifTool::plot_subcircuits(){

		std::ofstream combinat_file("subcircuits.txt");
		// bool complete = 0;

		for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
		{
			//Show the limits of each subcircuit
			combinat_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " ////////////////////////////" << std::endl;
			combinat_file << std::endl;
			combinat_file << "LAYER  ==>>  " << nl_.combinationalSubcircs_[i]->get_subcircSharedVars()[0][0]->get_name() << std::endl;
			combinat_file << std::endl;
			combinat_file << "Numb. of components: " << nl_.combinationalSubcircs_[i]->get_subcircGates().size() << std::endl;

			//Print inputs
			combinat_file << "subcircuit inputs?: " << nl_.combinationalSubcircs_[i]->get_subcircInputs().size() << std::endl;
			if (nl_.inputsMap_.find(nl_.combinationalSubcircs_[i]->get_subcircInputs()[0]->get_name()) != nl_.inputsMap_.end())
			{
				combinat_file << "Inputs: " << std::endl;
				for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircInputs().size(); ++j)
				{
					combinat_file << nl_.combinationalSubcircs_[i]->get_subcircInputs()[j]->get_name() << "; " << std::endl;
				}
				combinat_file << std::endl;
			}else{
				combinat_file << "Inputs from registers: "<< std::endl;
				for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircInputs().size(); ++j)
				{
					combinat_file << nl_.combinationalSubcircs_[i]->get_subcircInputs()[j]->get_name() << "; "<< std::endl;
				}
				combinat_file << std::endl;
			}

			//Print random bits
			combinat_file << "subcircuit random vars?: " << nl_.combinationalSubcircs_[i]->get_subcircRandoms().size() << std::endl;
			combinat_file << "Rands: " << std::endl;
			for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircRandoms().size(); ++j)
			{
				combinat_file << nl_.combinationalSubcircs_[i]->get_subcircRandoms()[j]->get_name() << "; " << std::endl;
			}
			combinat_file << std::endl;

			//Print outputs
			if (nl_.outsMap_.find(nl_.combinationalSubcircs_[i]->get_subcircOuts()[0]->get_name()) != nl_.outsMap_.end())
			{
				combinat_file << "Outputs: ";
				for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++j)
				{
					combinat_file << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name() << "; "<< std::endl;
				}
				combinat_file << std::endl;
			}else{
				combinat_file << "Outputs to registers: ";
				for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++j)
				{
					combinat_file << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name() << "; "<< std::endl;
				}
				combinat_file << std::endl;
			}

			// //Analyze first non-completenes for each subcirc
			// for (size_t j = 0; i < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++i)
			// {

			// 	wires input_dependencies;
			// 	search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->pinFrom->gate, input_dependencies);


			//     for (size_t l = 0; l < sharedVariables.size(); ++l){
			// 	    size_t depsSameVar = 0;
				    
			// 	    for (size_t t = 0; t < sharedVariables[l].size(); ++t){
			// 	        if (std::find(input_dependencies.begin(), input_dependencies.end(), sharedVariables[l][t]) != input_dependencies.end()){
			// 				depsSameVar++;
			// 	        }
			// 	    }
			    
			// 	    if (depsSameVar == sharedVariables[l].size()){
			// 	      	complete = 1;
			// 	      	combinat_file << "Complete output found -> " << outs[i]->name << ". All shares used for bit -> " << sharedVariables[l][0]->name << std::endl;
			// 	    }

			//   	}

			// }

			combinat_file << std::endl;
			combinat_file << std::endl;
		}
		std::cout << BOLD(FGRN("Check file \"subcircuits.txt\" to see all combinational subcircuits")) << std::endl;
	}
