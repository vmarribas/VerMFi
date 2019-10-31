#include "../src/VerifTool.h"

// #ifdef YOSYS
	bool VerifTool::build_yosys_tcl(const char* arg)
	{
		//Get files path to analyze
	    std::string path;
	    int i;

	    i = inFile_.find_last_of('/');
	    path = inFile_.substr(0,i+1);

	    std::ofstream outputFile("synthesis.ys");

	    // outputFile << "read_verilog " + std::string(arg) << std::endl;
	    outputFile << "read_verilog " + path + "*" << std::endl;
	    outputFile << std::endl;

	    outputFile << "hierarchy; proc; techmap;" << std::endl;
	    outputFile << std::endl;

		outputFile << "splitnets -ports" << std::endl;
		outputFile << std::endl;

		outputFile << "dfflibmap -liberty ./NangateOpenCellLibrary_PDKv1_3_v2010_12/Liberty/CCS/NangateOpenCellLibrary_typical_ccs.lib" << std::endl;
		outputFile << std::endl;

		outputFile << "abc -liberty ./NangateOpenCellLibrary_PDKv1_3_v2010_12/Liberty/CCS/NangateOpenCellLibrary_typical_ccs.lib" << std::endl;
		outputFile << std::endl;

		outputFile << "clean" << std::endl;
		outputFile << std::endl;

		outputFile << "write_verilog ./netlist/yosys/"+ module +"_yosys.v" << std::endl;

		outputFile.close();
    
    	return 1;

	}

// #endif