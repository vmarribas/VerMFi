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
