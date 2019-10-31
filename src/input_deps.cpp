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