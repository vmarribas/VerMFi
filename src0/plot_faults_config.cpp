
#include "../src/VerifTool.h"



// void VerifTool::print_names_sub_modules(std::ofstream &file, superGates const &sGates, int i, string &spaces){
void VerifTool::print_names_sub_modules(std::ofstream &file, superGates const &sGates){

	// if ((sGates.empty() == 1) || (i == 0))
	if ((sGates.empty() == 1))
	{
		// file << endl;
		return;
	}
	else
	{
		// --i;
		// spaces += "  ";
		for (size_t j = 0; j < sGates.size(); ++j)
		{	

			// cout << "Getting in supGate: " << sGates[j]->name_ << endl;
			size_t n = count(sGates[j]->name_.begin(), sGates[j]->name_.end(), '/');
			string spaces = "  ";
			while (n != 0){
				spaces += "  ";
				n--;
			}
			//Plot this superComponent name
			file << spaces << "* " << sGates[j]->name_ << " ->   F?: `0`   PI/O?: `0`   When?: `-1`   Type?: `SEU`  " << endl;

			//Continue with the rest
			// print_names_sub_modules(file, sGates[j]->superGates_, i, spaces);
			print_names_sub_modules(file, sGates[j]->superGates_);

			
		}
	}
}

void VerifTool::plot_faults_config(){
	
	
	std::ofstream faults_config("faults/config_faults_"+nlHy_.topModule_+".md");
	string spaces = "";

	faults_config << "# FAULTS CONFIGURATION FILE" << endl;
	faults_config << endl;

	faults_config << "## HYERARCHY" << endl;
	faults_config << endl;


	faults_config << "First introduce the number of fault simulations you want to run, how many faults  " << endl; 
	faults_config << "to inject in total per simulation and the maximum number of faults in the same cycle  " << endl;
	faults_config << "(substitute XX fields with your choice): " << endl;
	faults_config << endl;
	faults_config << "  N_SIMULS = `XX`  " << endl;
	faults_config << "  N_FAULTS = `XX`  " << endl;
	faults_config << "  PER_CYCLE = `XX`  " << endl;
	faults_config << endl;
	faults_config << endl;

	faults_config << "A hierarchy of your design is plot below. Several options are given to customize your fault injection:  " << endl;
	faults_config << "* F?: allow fault injection in this module  " << endl;
	faults_config << "* PI/PO?: allow fault injection in the PIs and POs, allowing single wires to be faulted  " << endl;
	faults_config << "* When?: choose clk cycle where faults should be inserted. You have several options:  " << endl;
	faults_config << "A specific cycle, -1 (meaning that the fault will be injected in every cycle or a range. Only ranges  " << endl;
	faults_config << "greater than or smaller than, like: `>7` or `<7` (cycle will be chosen at random within the range given)  " << endl;
	faults_config << "* Type?: SEU, SA0 or SA1  " << endl;
	faults_config << endl;
	faults_config << "## NETLIST" << endl;
	faults_config << endl;
	faults_config << nlHy_.topModule_ << " ->   F?: `0`   When?: `-1`   Type?: `SEU`  " << endl;

	// cout << "Number of total supGates: " << nlHy_.superComponents_.size() << endl;
	//3 decides the hierarchy levels to traverse
	// print_names_sub_modules(faults_config, nlHy_.superComponents_, hierarchyLevels, spaces);
	print_names_sub_modules(faults_config, nlHy_.superComponents_);


	faults_config.close();
	

}

// void VerifTool::print_names_sub_modules(std::ofstream &file, superGates const &sGates, int i, string &spaces){
void VerifTool::plot_sub_components_fault_config(std::ofstream &file, superGates const &sGates){

	// if ((sGates.empty() == 1) || (i == 0))
	if ((sGates.empty() == 1))
	{
		// file << endl;
		return;
	}
	else
	{
		// --i;
		// spaces += "  ";
		for (size_t j = 0; j < sGates.size(); ++j)
		{	

			// cout << "Getting in supGate: " << sGates[j]->name_ << endl;
			size_t n = count(sGates[j]->name_.begin(), sGates[j]->name_.end(), '/');
			string spaces = "  ";
			while (n != 0){
				spaces += "  ";
				n--;
			}
			//Plot this superComponent name
			file << spaces << "* " << sGates[j]->name_ << " components:  " << endl;

			//Plot primary inputs
			// for (size_t k = 0; k < sGates[j]->primaryInputs_.size(); ++k)
			// {
			// 	file << spaces << "  $ " << sGates[j]->primaryInputs_[k]->get_name() << " ->   F?: `0`   When?: `-1`   Type?: `SEU`  " << endl;
			// }
			
			for (size_t k = 0; k < sGates[j]->logicGates_.size(); ++k)
			{
				file << spaces << "  $ " << sGates[j]->logicGates_[k]->get_name() << " ->   F?: `0`   When?: `-1`   Type?: `SEU`  " << endl;
			}

			//Continue with the rest
			// print_names_sub_modules(file, sGates[j]->superGates_, i, spaces);
			plot_sub_components_fault_config(file, sGates[j]->superGates_);

			
		}
	}
}

void VerifTool::plot_components_fault_config(){
	
	
	std::ofstream faults_config("faults/config_components_fault_"+nlHy_.topModule_+".md");
	string spaces = "";

	faults_config << "# FAULTS CONFIGURATION FILE" << endl;
	faults_config << endl;

	faults_config << "## COMPONENTS" << endl;
	faults_config << endl;

	faults_config << "First introduce the number of fault simulations you want to run and how many faults  " << endl; 
	faults_config << "should be injected on each simulation (substitute XX fields with your choice): " << endl;
	faults_config << endl;
	faults_config << "  N_SIMULS = `XX`  " << endl;
	faults_config << "  N_FAULTS = `XX`  " << endl;
	faults_config << endl;
	faults_config << "When selecting FAULTS = 1, the simulation will be exhaustive. SIMULS is taken into account  " << endl;
	faults_config << "just when FAULTS > 1, so the tool will look for SIMULS combinations of the allowed components to fault.  " << endl;
	faults_config << endl;


	faults_config << "A detailed list of components of your design is plot below. Several options are given to customize your fault injection:  " << endl;
	faults_config << "* F?: allow fault injection in this module  " << endl;
	faults_config << "* When?: choose clk cycle where faults should be inserted. Ranges are allowed, but only  " << endl;
	faults_config << "greater than or smaller than, like: `>7` or `<7` (cycle will be chosen at random)  " << endl;
	faults_config << "* Type?: SEU, SA0 or SA1  " << endl;
	faults_config << endl;
	faults_config << "## NETLIST" << endl;
	faults_config << endl;
	faults_config << nlHy_.topModule_ << endl;

	for (size_t k = 0; k < nlHy_.components_.size(); ++k)
	{
		faults_config  << nlHy_.components_[k]->get_name() << " ->   F?: `0`   When?: `-1`   Type?: `SEU`  " << endl;
	}

	// cout << "Number of total supGates: " << nlHy_.superComponents_.size() << endl;
	//3 decides the hierarchy levels to traverse
	// print_names_sub_modules(faults_config, nlHy_.superComponents_, hierarchyLevels, spaces);
	plot_sub_components_fault_config(faults_config, nlHy_.superComponents_);


	faults_config.close();
	

}