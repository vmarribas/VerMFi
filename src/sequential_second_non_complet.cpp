#include "VerifTool.h"

void VerifTool::sequential_second_non_complet(bool outsSpecified){

	std::ofstream complet_file("subcircuitsNC_2nd.txt");
	bool complete = 0;
	std::vector<int> checkedCircuits;
	std::vector<std::string> completeCircs;

	std::vector<wires> outSensitiveVarsi;
	std::vector<wires> outSensitiveVarsii;


	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		for (size_t ii = i+1; ii < nl_.combinationalSubcircs_.size(); ++ii)
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
				// for (int mm = 0; mm < allOutputsToCheck.size(); ++mm)
				// {
				// 	cout << allOutputsToCheck[mm]->get_name() << ", " << endl;
				// }
				// cout << endl;

				for (size_t j = 0; j < allOutputsToCheck.size(); ++j){
					for (size_t k = j; k < allOutputsToCheck.size(); ++k)
					{
						// Get the sensitive data from the outputs
						outSensitiveVarsi = nl_.outsSensitiveDataMap_[allOutputsToCheck[j]->get_name()];
						outSensitiveVarsii = nl_.outsSensitiveDataMap_[allOutputsToCheck[k]->get_name()];

						if (outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsii[0][0]->get_name())
						{
							// std::cout << "Search back for putput " << allOutputsToCheck[j]->name << std::endl;
							wires inputDependencies;
							if ((allOutputsToCheck[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
								(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
								(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
								(allOutputsToCheck[k]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
								(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
								(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
							{
								nl_.search_back(allOutputsToCheck[j]->get_pinFrom()->get_gate(), inputDependencies);
								nl_.search_back(allOutputsToCheck[k]->get_pinFrom()->get_gate(), inputDependencies);
							}
							// cout << allOutputsToCheck[j]->get_name() << " and " << allOutputsToCheck[k]->get_name() << ":::::" <<endl;

							for (size_t l = 0; l < outSensitiveVarsi.size(); ++l){
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
							      				 << " and " << allOutputsToCheck[k]->get_name()
							      				 << ". All shares used for bit -> " << outSensitiveVarsi[l][0]->get_name() << std::endl;
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
	// With the previous iteration, not all subcircs will be analyzed. In case there is a single subcircuit in one of the stages,
	// this one will not get combined with any of the others and neither analyzed alone so we need to analyzed independently the ones 
	// not analyzed.

	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		if (std::find(checkedCircuits.begin(), checkedCircuits.end(), i) == checkedCircuits.end())
		{
			// Check that the subcircuits are from the sme layer, if yes, get all the outputs to check NC
			// cout << "Subcirc " << i << " WAS NOT analyzed before!!!" << endl;

			wires allOutputsToCheck = nl_.combinationalSubcircs_[i]->get_subcircOuts() ;

			// //Analyze second non-completenes for each comb. of all the outputs from the combination of subcircs. in the same layer
			//Show the limits of each subcircuit
			complet_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " ////////////////////////////" << std::endl;
			complet_file << std::endl;
			bool completeSubcirc = 0;

			// cout << "allOutputsToCheck second: " << allOutputsToCheck.size() << ", i = " << i << endl;
			// for (int mm = 0; mm < allOutputsToCheck.size(); ++mm)
			// {
			// 	cout << allOutputsToCheck[mm]->get_name() << ", " << endl;
			// }
			// cout << endl;

			for (size_t j = 0; j < allOutputsToCheck.size(); ++j){
				for (size_t k = j; k < allOutputsToCheck.size(); ++k)
				{
					// Get the sensitive data from the outputs
					outSensitiveVarsi = nl_.outsSensitiveDataMap_[allOutputsToCheck[j]->get_name()];
					outSensitiveVarsii = nl_.outsSensitiveDataMap_[allOutputsToCheck[k]->get_name()];

					if (outSensitiveVarsi[0][0]->get_name() == outSensitiveVarsii[0][0]->get_name())
					{
						// std::cout << "Search back for putput " << allOutputsToCheck[j]->name << std::endl;
						wires inputDependencies;
						if ((allOutputsToCheck[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
							(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
							(allOutputsToCheck[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF) &&
							(allOutputsToCheck[k]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
							(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
							(allOutputsToCheck[k]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
						{
							nl_.search_back(allOutputsToCheck[j]->get_pinFrom()->get_gate(), inputDependencies);
							nl_.search_back(allOutputsToCheck[k]->get_pinFrom()->get_gate(), inputDependencies);
						}
						// cout << allOutputsToCheck[j]->get_name() << " and " << allOutputsToCheck[k]->get_name() << ":::::" <<endl;

						for (size_t l = 0; l < outSensitiveVarsi.size(); ++l){
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
						      				 << " and " << allOutputsToCheck[k]->get_name()
						      				 << ". All shares used for bit -> " << outSensitiveVarsi[l][0]->get_name() << std::endl;
						    }
					  	}
					}
				}
			}
			if (completeSubcirc)
			{
				string ident = nl_.combinationalSubcircs_[i]->get_ident();
				completeCircs.push_back(ident);
			}
			complet_file << std::endl;
		}
	}

	if (complete)
	{
		std::cout << BOLD(FRED("Subcircuits "));

		for (size_t i = 0; i < completeCircs.size(); ++i)
		{
			std::cout << completeCircs[i] << BOLD(FRED(", "));
		}
		cout << endl;
		std::cout << BOLD(FRED("do not fulfill non_completeness property. Check file \"subcircuitsNC_2nd.txt\" to see all complete bits")) << std::endl;
	}else
	{
		std::cout << BOLD(FGRN("Your program fulfills non_completeness property!")) << std::endl;
	}


}
