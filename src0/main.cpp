
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h> 
#include "../src/VerifTool.h"
#include <chrono>

using namespace std::chrono;

#ifndef TEST

int main(int argc, char **argv) {
   
    if (argc < 1){
        std::cout << "Not enough files introduced" << std::endl;
    }
    else{

        VerifTool vt(argv[1]);

        if (!vt.read_in_file_faults()){
        	std::cout << BOLD(FRED("Wrong file type entry")) << std::endl;
        	return 0;
        }

        #ifndef YOSYS
            vt.build_tcl(); 
            //Run TCL script
            system("dc_shell -f compile.tcl");
        #else
            vt.build_yosys_tcl(argv[1]);

            system("./yosys/yosys synthesis.ys");
            system("yosys synthesis.ys");
        #endif

        
        std::cout << "Building Faults config files: \"faults/config_faults_$TOPMODULE.md\" and \"faults/config_components_fault_$TOPMODULE.md\"" << std::endl;


    	if (!vt.read_hy_netlist()){
    		std::cout << BOLD(FRED("Could not read hierarchy netlist")) << std::endl;;
    		return 0;
    	}
    }

    return 0;
}

#endif