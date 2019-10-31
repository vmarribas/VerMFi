
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

bool VerifTool::input_deps(){
	
	std::ofstream deps_file("dependencies.txt");

	if (nl_.sequential_)
	{
		for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
		{
			//Show the limits of each subcircuit
			deps_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " ////////////////////////////" << std::endl;
			deps_file << std::endl;

			for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++j)
			{

				// std::cout << "Search back for putput " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->name << std::endl;
				wires inputDependencies;
				nl_.search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate(), inputDependencies);


			    deps_file << "Output -> " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name() << " depends on " << inputDependencies.size() << " input bits" << std::endl;

			    deps_file << "	" << std::endl;

			    for (size_t l = 0; l < inputDependencies.size(); ++l){

			    	deps_file << inputDependencies[l]->get_name() << " || ";
			  	}

			  	deps_file << std::endl;
			  	deps_file << "//////////////////////////////////////////////////////////////////////////////" << std::endl;
			  	deps_file << std::endl;

			}
		}
	}
	else{

		for (size_t i = 0; i < nl_.outs_.size(); ++i)
		{

			// std::cout << "Search back for putput " << outs[i]->name << std::endl;
			wires inputDependencies;
			nl_.search_back(nl_.outs_[i]->get_pinFrom()->get_gate(), inputDependencies);


		    deps_file << "Output -> " << nl_.outs_[i]->get_name() << " depends on " << inputDependencies.size() << " input bits" << std::endl;

		    deps_file << "	" << std::endl;

		    for (size_t l = 0; l < inputDependencies.size(); ++l){

		    	deps_file << inputDependencies[l]->get_name() << " || ";
		  	}

		  	deps_file << std::endl;
		  	deps_file << "//////////////////////////////////////////////////////////////////////////////" << std::endl;
		  	deps_file << std::endl;

		}

	}


	std::cout << BOLD(FGRN("Check file \"dependencies.txt\" to see all dependencies")) << std::endl;

	deps_file.close();

	return 1;
}