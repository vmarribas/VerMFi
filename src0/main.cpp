
////////////////////////////////////////////////////////////////////////////////

// COMPANY:     COSIC, KU Leuven 
// AUTHOR:      Victor Arribas

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