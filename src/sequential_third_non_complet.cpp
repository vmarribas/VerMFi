
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

void VerifTool::sequential_third_non_complet(bool outsSpecified){

	std::ofstream complet_file("subcircuitsNC_3rd.txt");
	bool complete = 0;
	std::vector<int> checkedCircuits;
	std::vector<std::string> completeCircs;

	std::vector<wires> outSensitiveVarsi;
	std::vector<wires> outSensitiveVarsii;
	std::vector<wires> outSensitiveVarsiii;

	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		for (size_t ii = i+1; ii < nl_.combinationalSubcircs_.size(); ++ii)
		{
			for (size_t iii = ii+1; iii < nl_.combinationalSubcircs_.size(); ++iii)
			{
				if ((nl_.combinationalSubcircs_[i]->get_subcircSharedVars()[0][0]->get_name() == 
					nl_.combinationalSubcircs_[ii]->get_subcircSharedVars()[0][0]->get_name()) && 
					(nl_.combinationalSubcircs_[i]->get_subcircSharedVars()[0][0]->get_name() == 
					nl_.combinationalSubcircs_[iii]->get_subcircSharedVars()[0][0]->get_name()))
				{
					// Check that the subcircuits are from the sme layer, if yes, get all the outputs to check NC
					checkedCircuits.push_back(i);
					checkedCircuits.push_back(ii);
					checkedCircuits.push_back(iii);

					wires allOutputsToCheck = nl_.combinationalSubcircs_[i]->get_subcircOuts() ;

					// cout << "allOutputsToCheck ZERO size = " << allOutputsToCheck.size() << endl;

					// These are circuits in the same layer, but not the same exact one subcirc. Then concatenate all outputs
					for (size_t jj = 0; jj < nl_.combinationalSubcircs_[ii]->get_subcircOuts().size(); ++jj)
					{
						allOutputsToCheck.push_back(nl_.combinationalSubcircs_[ii]->get_subcircOuts()[jj]);
					}
					for (size_t jjj = 0; jjj < nl_.combinationalSubcircs_[iii]->get_subcircOuts().size(); ++jjj)
					{
						allOutputsToCheck.push_back(nl_.combinationalSubcircs_[iii]->get_subcircOuts()[jjj]);
					}

					// //Analyze second non-completenes for each comb. of all the outputs from the combination of subcircs. in the same layer
					//Show the limits of each subcircuit
					complet_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() 
						<< " + " << nl_.combinationalSubcircs_[ii]->get_ident() 
						<< " + " << nl_.combinationalSubcircs_[iii]->get_ident() 
						<< " ////////////////////////////" << std::endl;
					complet_file << std::endl;
					bool completeSubcirc = 0;

					for (size_t j = 0; j < allOutputsToCheck.size(); ++j)
					{
						for (size_t k = j; k < allOutputsToCheck.size(); ++k)
						{
							for (size_t m = k; m < allOutputsToCheck.size(); ++m)
							{
								// Get the sensitive data from the outputs
								outSensitiveVarsi = nl_.outsSensitiveDataMap_[allOutputsToCheck[j]->get_name()];
								outSensitiveVarsii = nl_.outsSensitiveDataMap_[allOutputsToCheck[k]->get_name()];
								outSensitiveVarsiii = nl_.outsSensitiveDataMap_[allOutputsToCheck[m]->get_name()];

								if ((outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsii[0][0]->get_name()) && 
									(outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsiii[0][0]->get_name()))
								{
									// std::cout << "Search back for putput " << allOutputsToCheck[j]->name << std::endl;
									wires inputDependencies;
									if ((allOutputsToCheck[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
										(allOutputsToCheck[k]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
										(allOutputsToCheck[m]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[m]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[m]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
									{
										nl_.search_back(allOutputsToCheck[j]->get_pinFrom()->get_gate(), inputDependencies);
										nl_.search_back(allOutputsToCheck[k]->get_pinFrom()->get_gate(), inputDependencies);
										nl_.search_back(allOutputsToCheck[m]->get_pinFrom()->get_gate(), inputDependencies);
									}
									// cout << allOutputsToCheck[j]->get_name() << " and " << allOutputsToCheck[k]->get_name() << " and " << allOutputsToCheck[m]->get_name() << ":::::" <<endl;

									for (size_t l = 0; l < outSensitiveVarsi.size(); ++l)
									{
									    size_t depsSameVar = 0;
									    
									    for (size_t t = 0; t < outSensitiveVarsi[l].size(); ++t){
									        if (std::find(inputDependencies.begin(), inputDependencies.end(), outSensitiveVarsi[l][t]) != inputDependencies.end()){
												depsSameVar++;
									        }
									    }
								    
									    if (depsSameVar == outSensitiveVarsi[l].size()){
									      	complete = 1;
									      	completeSubcirc = 1;
									      	complet_file << "Probed wires -> " << allOutputsToCheck[j]->get_name()
		      						      				 << ", " << allOutputsToCheck[k]->get_name()
		      						      				 << " and " << allOutputsToCheck[m]->get_name()
		      						      				 << ". All shares used for bit -> " << outSensitiveVarsi[l][0]->get_name() << std::endl;
		      						    }
								  	}
								}
							}
						}
					}
					if (completeSubcirc)
					{
						string ident = nl_.combinationalSubcircs_[i]->get_ident() + " WITH " + nl_.combinationalSubcircs_[ii]->get_ident() + " ANDWITH " + nl_.combinationalSubcircs_[iii]->get_ident();
						completeCircs.push_back(ident);
					}
					complet_file << std::endl;
				}
			}
		}
	}

	// Combinations of just two subcircs not analyzed
	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		for (size_t ii = i+1; ii < nl_.combinationalSubcircs_.size(); ++ii)
		{
			if ((std::find(checkedCircuits.begin(), checkedCircuits.end(), i) == checkedCircuits.end()) && 
				(std::find(checkedCircuits.begin(), checkedCircuits.end(), ii) == checkedCircuits.end()))
			{	
				if (nl_.combinationalSubcircs_[i]->get_subcircSharedVars()[0][0]->get_name() == nl_.combinationalSubcircs_[ii]->get_subcircSharedVars()[0][0]->get_name())
				{
					// Check that the subcircuits are from the sme layer, if yes, get all the outputs to check NC
					checkedCircuits.push_back(i);
					checkedCircuits.push_back(ii);

					wires allOutputsToCheck = nl_.combinationalSubcircs_[i]->get_subcircOuts() ;

					// cout << "allOutputsToCheck first size = " << allOutputsToCheck.size() << endl;

					// These are circuits in the same layer, but not the same exact one subcirc. Then concatenate all outputs
					for (size_t jj = 0; jj < nl_.combinationalSubcircs_[ii]->get_subcircOuts().size(); ++jj)
					{
						allOutputsToCheck.push_back(nl_.combinationalSubcircs_[ii]->get_subcircOuts()[jj]);
					}

					// //Analyze second non-completenes for each comb. of all the outputs from the combination of subcircs. in the same layer
					//Show the limits of each subcircuit
					complet_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " + " << nl_.combinationalSubcircs_[ii]->get_ident() << " ////////////////////////////" << std::endl;
					complet_file << std::endl;
					bool completeSubcirc = 0;

					// cout << "allOutputsToCheck second: " << allOutputsToCheck.size() << ", i = " << i << ", ii = " << ii << endl;
					// for (size_t mm = 0; mm < allOutputsToCheck.size(); ++mm)
					// {
					// 	cout << allOutputsToCheck[mm]->get_name() << ", " << endl;
					// }
					// cout << endl;

					for (size_t j = 0; j < allOutputsToCheck.size(); ++j)
					{
						for (size_t k = j; k < allOutputsToCheck.size(); ++k)
						{
							for (size_t m = k; m < allOutputsToCheck.size(); ++m)
							{
								// Get the sensitive data from the outputs
								outSensitiveVarsi = nl_.outsSensitiveDataMap_[allOutputsToCheck[j]->get_name()];
								outSensitiveVarsii = nl_.outsSensitiveDataMap_[allOutputsToCheck[k]->get_name()];
								outSensitiveVarsiii = nl_.outsSensitiveDataMap_[allOutputsToCheck[m]->get_name()];

								if ((outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsii[0][0]->get_name()) && 
									(outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsiii[0][0]->get_name()))
								{
									// std::cout << "Search back for putput " << allOutputsToCheck[j]->name << std::endl;
									wires inputDependencies;
									if ((allOutputsToCheck[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
										(allOutputsToCheck[k]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
										(allOutputsToCheck[m]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
										(allOutputsToCheck[m]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
										(allOutputsToCheck[m]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
									{
										nl_.search_back(allOutputsToCheck[j]->get_pinFrom()->get_gate(), inputDependencies);
										nl_.search_back(allOutputsToCheck[k]->get_pinFrom()->get_gate(), inputDependencies);
										nl_.search_back(allOutputsToCheck[m]->get_pinFrom()->get_gate(), inputDependencies);
									}
									// cout << allOutputsToCheck[j]->get_name() << " and " << allOutputsToCheck[k]->get_name() << " and " << allOutputsToCheck[m]->get_name() << ":::::" <<endl;

									for (size_t l = 0; l < outSensitiveVarsi.size(); ++l)
									{
									    size_t depsSameVar = 0;
									    
									    for (size_t t = 0; t < outSensitiveVarsi[l].size(); ++t){
									        if (std::find(inputDependencies.begin(), inputDependencies.end(), outSensitiveVarsi[l][t]) != inputDependencies.end()){
												depsSameVar++;
									        }
									    }
								    
									    if (depsSameVar == outSensitiveVarsi[l].size()){
									      	complete = 1;
									      	completeSubcirc = 1;
									      	complet_file << "Probed wires -> " << allOutputsToCheck[j]->get_name()
		      						      				 << ", " << allOutputsToCheck[k]->get_name()
		      						      				 << " and " << allOutputsToCheck[m]->get_name()
		      						      				 << ". All shares used for bit -> " << outSensitiveVarsi[l][0]->get_name() << std::endl;
		      						    }
								  	}
								}
							}
						}
					}
					if (completeSubcirc)
					{
						string ident = nl_.combinationalSubcircs_[i]->get_ident() + " WITH " + nl_.combinationalSubcircs_[ii]->get_ident();
						completeCircs.push_back(ident);
					}
					complet_file << std::endl;
				}
			}
		}
	}

	// Single unanalyzed subcircuits
	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		if (std::find(checkedCircuits.begin(), checkedCircuits.end(), i) == checkedCircuits.end())
		{
			// //Analyze first non-completenes for each subcirc
			//Show the limits of each subcircuit
			// cout << "Subcirc " << i << " WAS NOT analyzed before!!!" << endl;

			complet_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " ////////////////////////////" << std::endl;
			complet_file << std::endl;
			bool completeSubcirc = 0;

			for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++j)
			{
				for (size_t k = j; k < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++k)
				{
					for (size_t m = k; m < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++m)
					{
						// Get the sensitive data from the outputs
						outSensitiveVarsi = nl_.outsSensitiveDataMap_[nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name()];
						outSensitiveVarsii = nl_.outsSensitiveDataMap_[nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_name()];
						outSensitiveVarsiii = nl_.outsSensitiveDataMap_[nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_name()];

						if ((outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsii[0][0]->get_name()) && 
							(outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsiii[0][0]->get_name()))
						{
							// std::cout << "Search back for putput " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->name << std::endl;
							wires inputDependencies;
							if ((nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
								(nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
							{
								nl_.search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate(), inputDependencies);
								nl_.search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_pinFrom()->get_gate(), inputDependencies);
								nl_.search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_pinFrom()->get_gate(), inputDependencies);
							}
							// cout << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name() << " and " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_name() << " and " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_name() << ":::::" <<endl;

							for (size_t l = 0; l < outSensitiveVarsi.size(); ++l)
							{
							    size_t depsSameVar = 0;
							    
							    for (size_t t = 0; t < outSensitiveVarsi[l].size(); ++t){
							        if (std::find(inputDependencies.begin(), inputDependencies.end(), outSensitiveVarsi[l][t]) != inputDependencies.end()){
										depsSameVar++;
							        }
							    }
						    
							    if (depsSameVar == outSensitiveVarsi[l].size()){
							      	complete = 1;
							      	completeSubcirc = 1;
							      	complet_file << "Probed wires -> " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name()
      						      				 << ", " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[k]->get_name()
      						      				 << " and " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[m]->get_name()
      						      				 << ". All shares used for bit -> " << outSensitiveVarsi[l][0]->get_name() << std::endl;
      						    }
						  	}
						}
					}
				}
			}
			if (completeSubcirc)
			{
				completeCircs.push_back(nl_.combinationalSubcircs_[i]->get_ident());
			}
			complet_file << std::endl;
		}
	}

	if (complete)
	{
		std::cout << BOLD(FRED("Subcircuits "));

		for (size_t i = 0; i < completeCircs.size(); ++i)
		{
			std::cout << completeCircs[i] << BOLD(FRED(" AND "));
		}
		std::cout << BOLD(FRED("do not fulfill non_completeness property. Check file \"subcircuitsNC_3rd.txt\" to see all complete bits")) << std::endl;
	}else
	{
		std::cout << BOLD(FGRN("Your program fulfills non_completeness property!")) << std::endl;
	}

}
