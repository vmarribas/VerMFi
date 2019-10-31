
////////////////////////////////////////////////////////////////////////////////

// COMPANY:		COSIC, KU Leuven 
// AUTHOR:		Victor Arribas

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