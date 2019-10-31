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
  