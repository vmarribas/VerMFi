
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

void Netlist::layers_sensitive_data(std::vector<std::vector<std::vector<std::string>>> regsLayers)
{
	for (size_t i = 0; i < regsLayers.size(); ++i)
	{
		// std::cout << "LAYER " << i << "///////////////////" << std::endl;
		std::vector<wires> layer_wires;

		//Look for the wires associated to each layer inputs
		for (size_t j = 0; j < regsLayers[i].size(); ++j)
		{
			std::vector<wires> variable_bits;
			// std::cout << "VARIABLES" << std::endl;

			// Each layer may come as a matrix of sensitive data, as the PIs are sorted
			for (size_t k = 0; k < regsLayers[i][j].size(); ++k)
			{
				wires reg_bits;

				// cout << "Look for wires with the same name as: " << regsLayers[i][j][k] << endl;
				//Get all bits with this name
				bool atLeastOneFound = 0;
				for (size_t l = 0; l < inputsFF_.size(); ++l)
				{
					if ((inputsFF_[l]->name_.find(regsLayers[i][j][k]) != std::string::npos) || // Find it within the inputs, 
						(inputsFF_[l]->get_pinFrom()->get_gate()->get_name().find(regsLayers[i][j][k]) != std::string::npos)) // or find it in the name of the register.
					{
						// std::cout << inputsFF_[l]->name_ << ";";
						reg_bits.push_back(inputsFF_[l]);
						atLeastOneFound = 1;
					}
				}
				variable_bits.push_back(reg_bits);
				if (!atLeastOneFound) cout << "Cannot find signal: " << regsLayers[i][j][k] << endl;
				assert (atLeastOneFound && ("Did not find any virtual PI coming from DFF for signal; it might be that it does not exist anymore in your Netlist cause Synopsys changed the name or trimmed it."));

				// std::cout << std::endl;

			}

			// All bits for this variable shares have been gathered, but the current matrix
			// has the variables as rows and shares as columns => Transpose column by column
			// and push that row to layer_wires
			// std::cout << "TRANSPOSITION" << std::endl;

			for (size_t n = 0; n < variable_bits[0].size(); ++n)
			{
				//Go through matrix column first
				wires layer_wiresrow;

				for (size_t m = 0; m < variable_bits.size(); ++m)
				{
					layer_wiresrow.push_back(variable_bits[m][n]);
					// std::cout << variable_bits[m][n]->name_ << ";";
				}
				layer_wires.push_back(layer_wiresrow);
				// std::cout << std::endl;
			}
		}

		//Layer completely gathered, store in regsSharedVariables_
		regsSharedVariables_.push_back(layer_wires);
	} 

	 // for (size_t i = 0; i < regsSharedVariables_.size(); ++i)
	 // {
	 // 	std::cout << "Layer " << i << std::endl;
	 // 	for (size_t j = 0; j < regsSharedVariables_[i].size(); ++j)
	 // 	{
	 // 		for (size_t k = 0; k < regsSharedVariables_[i][j].size(); ++k)
	 // 		{
	 // 			std::cout << regsSharedVariables_[i][j][k]->name_ << "; ";
	 // 		}
	 // 		std::cout << std::endl;
	 // 	}
	 // }
}
