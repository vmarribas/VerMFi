#include "../src/VerifTool.h"
#include "../src/GatesFactory.h"

using namespace std;


  bool VerifTool::read_hy_netlist()
  {
    
	//Hierarchy netlist
	//Read file
	#ifndef YOSYS
		fNetlistHy_.open("./netlist/" + module + "_hierarchy.v");
	#else
		fNetlistHy_.open("./netlist/yosys/" + module + "_yosys.v");
		// fNetlistHy_.open("synth_yosys.v");
	#endif

	if (!fNetlistHy_.is_open()) return 0;

	cout << "Top module: " << module << endl;

	nlHy_ = Netlist(module);

	nlHy_.create_hy_netlist(fNetlistHy_, inputs_, outputs_);

	cout << "Netlist created" << endl;
	//Plot faults config file
	plot_faults_config();

	plot_components_fault_config();
	
	
	return 1;

  }

VerifTool::VerifTool(){
};

VerifTool::~VerifTool(){
//    delete fM_;
};