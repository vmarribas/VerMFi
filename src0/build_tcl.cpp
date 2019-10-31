
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

#include "../src/VerifTool.h"

#ifndef YOSYS
  bool VerifTool::build_tcl()
  {
    //Get files path to analyze
    std::string path;
    int i;
    i = inFile_.find_last_of('/');
    path = inFile_.substr(0,i+1);


    std::ofstream outputFile("compile.tcl");
    
    outputFile << "#Compiling script" << std::endl;
    outputFile << std::endl;

    outputFile << "#Set search path and libraries" << std::endl;
    outputFile << "set search_path ./NangateOpenCellLibrary_PDKv1_3_v2010_12/Liberty/CCS" << std::endl;
    outputFile << "set target_library \"NangateOpenCellLibrary.db\"" << std::endl;
    outputFile << "set link_library \"$target_library\"" << std::endl; 
    outputFile << std::endl;

    outputFile << "#Define design lib" << std::endl;
    outputFile << "define_design_lib WORK -path \"./work\"" << std::endl;
    outputFile << std::endl;
    
    outputFile << "#Define top level and arch" << std::endl;
    outputFile << "set TOPCELL_ENT " + module << std::endl;
    outputFile << "set TOPCELL_ARCH " + architecture << std::endl;
    outputFile << std::endl;
    
    outputFile << "#Analyze target design" << std::endl;
    outputFile << "analyze -library WORK -autoread {./" << path << "} > ./logfiles/analyze.log" << std::endl;
    outputFile << std::endl;

    outputFile << "#Elaborate design" << std::endl;
    outputFile << "elaborate $TOPCELL_ENT -architecture $TOPCELL_ARCH -library DEFAULT > ./logfiles/elaborate.log" << std::endl;
    outputFile << std::endl;

    outputFile << "#Link design with the library" << std::endl;
    outputFile << "link" << std::endl;
    outputFile << std::endl;

    // outputFile << "#Keep all names from the top module" << std::endl;
    outputFile << "set_dont_touch_network -no_propagate [get_pins *]" << std::endl;
    outputFile << "set_dont_touch [get_nets {*}]" << std::endl;
    // outputFile << "set_dont_touch [get_nets {*/*}]" << std::endl;
    // outputFile << "set_dont_touch [get_nets {*/*/*}]" << std::endl;

    outputFile << "#Avoid optimization of registers signals" << std::endl;
    for (size_t i = 0; i < regsLayers_.size(); ++i)
    {
        for (size_t j = 0; j < regsLayers_[i].size(); ++j)
        {
            outputFile << "set_dont_touch [get_nets {";

            for (size_t k = 0; k < regsLayers_[i][j].size(); ++k)
            {
                outputFile << regsLayers_[i][j][k] << "* ";
            }

            outputFile << "}]" << std::endl;
        }
    }
    outputFile << std::endl;

    // outputFile << "define_name_rules change_bus_naming_style -target_bus_naming_style {%s[%d]}" << std::endl;
    // outputFile << "set bus_naming_style {%s[%d]}" << std::endl;  

    outputFile << "set ungroup_keep_original_design true" << std::endl;

    outputFile << "#Compile" << std::endl;
    outputFile << "#compile_ultra -Gateclock -no_autoungroup" << std::endl;

    outputFile << "compile -exact_map > ./logfiles/compile.log" << std::endl;
    outputFile << std::endl;

    outputFile << "#Verilog netlist" << std::endl;
    outputFile << "set verilogout_show_unconnected_pins true" << std::endl;
    outputFile << "set verilogout_single_bit true" << std::endl;
    outputFile << "write -format verilog -hierarchy -output ./netlist/" + module + "_hierarchy.v" << std::endl;
    outputFile << std::endl;

    outputFile << "#Compile again to get un grouped netlist" << std::endl;
    outputFile << "compile -exact_map -ungroup_all > ./logfiles/compile.log" << std::endl;
    outputFile << std::endl;
    
    outputFile << "#Verilog netlist" << std::endl;
    outputFile << "set verilogout_single_bit true" << std::endl;
    outputFile << "write -format verilog -hierarchy -output ./netlist/" + module + ".v" << std::endl;
    outputFile << std::endl;

    outputFile << "#Reports" << std::endl;
    outputFile << "report_timing -transition_time -nets -attributes -nosplit > ./reports/timing.log" << std::endl;
    outputFile << "report_area -nosplit -hierarchy > ./reports/area.log" << std::endl;
    outputFile << "report_power -nosplit -hier > ./reports/power.log" << std::endl;
    outputFile << "report_reference -nosplit -hierarchy > ./reports/ref.log" << std::endl;
    outputFile << "report_resources -nosplit -hierarchy > ./reports/resources.log" << std::endl;
    outputFile << std::endl;

    outputFile << "#Exit" << std::endl;
    outputFile << "exit" << std::endl;
    outputFile << std::endl;
   
    outputFile.close();
    
    return 1;
    
  }
#endif