
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
#include "color.h"


  //File non_completeness
  void VerifTool::first_non_complet(bool outsSpecified)
  {
	
	std::ofstream complet_file("complete_bits_1st.txt");
	bool complete = 0;
	// std::cout << "OutSpecified? " << outsSpecified << std::endl;
	for (size_t i = 0; i < nl_.outs_.size(); ++i)
	{
		// std::cout << nl_.outs_[i]->get_name() << " is sens output?? " << nl_.outs_[i]->get_is_SensO() << std::endl;
		if ((!outsSpecified) || (nl_.outs_[i]->get_is_SensO()))
		{
		//Analyze the ouput when: there is no sensitive outputs specified or, if they are, when it is a sensitive one-> NOT enought just with the name
		// std::cout << "Search back in output: " << nl_.outs_[i]->get_name() << std::endl;
			wires inputDependencies;
			nl_.search_back(nl_.outs_[i]->get_pinFrom()->get_gate(), inputDependencies);


		    for (size_t l = 0; l < nl_.sharedVariables_.size(); ++l){
			    size_t depsSameVar = 0;
			    
			    for (size_t t = 0; t < nl_.sharedVariables_[l].size(); ++t){
			        if (std::find(inputDependencies.begin(), inputDependencies.end(), nl_.sharedVariables_[l][t]) != inputDependencies.end()){
						depsSameVar++;
			        }
			    }
		    
			    if (depsSameVar == nl_.sharedVariables_[l].size()){
			      	complete = 1;
			      	complet_file << "Complete output found -> " << nl_.outs_[i]->get_name() << ". All shares used for bit -> " << nl_.sharedVariables_[l][0]->get_name() << std::endl;
			    }
		  	}
		}

	}

	if (!complete){
	  	std::cout << BOLD(FGRN("Your program fulfills non_completeness property!")) << std::endl;
	}else{
	  	std::cout << BOLD(FRED("Your program does not fulfill non_completeness property. Check file \"complete_bits.txt\" to see all complete bits")) << std::endl;
	}

	complet_file.close();
		
  }
  