#include "VerifTool.h"


void VerifTool::sequential_first_non_complet(bool outsSpecified){

	std::ofstream complet_file("subcircuitsNC_1st.txt");
	bool complete = 0;
	std::vector<std::string> completeCircs;
	std::vector<wires> outSensitiveVars;

	for (size_t i = 0; i < nl_.combinationalSubcircs_.size(); ++i)
	{
		// //Analyze first non-completenes for each subcirc
		//Show the limits of each subcircuit
		complet_file << "//////////////////////////// " << nl_.combinationalSubcircs_[i]->get_ident() << " ////////////////////////////" << std::endl;
		complet_file << std::endl;
		bool completeSubcirc = 0;

		for (size_t j = 0; j < nl_.combinationalSubcircs_[i]->get_subcircOuts().size(); ++j)
		{
			// std::cout << "Search back for putput " << combinationalSubcircs_[i]->get_subcircOuts()[j]->name << std::endl;
			wires inputDependencies;
			if ((nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom() != NULL) && //In case there is an input directly to a register!!!!!!!!!!!!!!!!
				(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF) && // In case a register output goes immediately to a register input so it is both newIn & newOut
				(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF))
			{
				assert(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::DFF && "Feeding a register into the recursiveness for NC analysis");
				assert(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate()->get_type() != Gate::SDFF && "Feeding a register into the recursiveness for NC analysis");
				nl_.search_back(nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_pinFrom()->get_gate(), inputDependencies);
			}
			
			// Get the sensitive data for this output
			outSensitiveVars = nl_.outsSensitiveDataMap_[nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name()];
			for (size_t l = 0; l < outSensitiveVars.size(); ++l){
			    size_t depsSameVar = 0;
			    
			    for (size_t t = 0; t < outSensitiveVars[l].size(); ++t){
			        if (std::find(inputDependencies.begin(), inputDependencies.end(), outSensitiveVars[l][t]) != inputDependencies.end()){
						depsSameVar++;
			        }
			    }
		    
			    if (depsSameVar == outSensitiveVars[l].size()){
			      	complete = 1;
			      	completeSubcirc = 1;
			      	complet_file << "Complete output found -> " << nl_.combinationalSubcircs_[i]->get_subcircOuts()[j]->get_name() << 
			      					". All shares used for bit -> " << outSensitiveVars[l][0]->get_name() << std::endl;
			    }

		  	}

		}
		if (completeSubcirc)
		{
			completeCircs.push_back(nl_.combinationalSubcircs_[i]->get_ident());
		}
		complet_file << std::endl;
	}

	if (complete)
	{
		std::cout << BOLD(FRED("Subcircuits "));

		for (size_t i = 0; i < completeCircs.size(); ++i)
		{
			std::cout << completeCircs[i] << BOLD(FRED(", "));
		}
		std::cout << BOLD(FRED("do not fulfill non_completeness property. Check file \"subcircuitsNC_1st.txt\" to see all complete bits")) << std::endl;
	}else
	{
		std::cout << BOLD(FGRN("Your program fulfills non_completeness property!")) << std::endl;
	}
}
