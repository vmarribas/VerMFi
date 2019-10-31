
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

#include "VerifTool.h"



  bool VerifTool::read_in_file()
  {
	std::string line;
	int got_inputs = 0;
	outsSpecified_ = 0;
	int entitySavedAlready = 0;
	
	//Read file
	while(getline(fIn_,line)){
	  
	  if(line.find("Input_shares") != std::string::npos){
	    //Save the name of the shared variables
		got_inputs = 1;
		
	    size_t i = 0;
	    size_t j = 0;
	    std::string v_buff;
	    std::vector<std::string> inp_shares;
	    i = line.find(':');
	    while ((line[j] != ',') && (line[j] != '.') && (line[j] != ';')){
	      ++j;
	    }
	    v_buff = line.substr(i+2,j-i-2);
	    inp_shares.push_back(v_buff);
	    inp_shares.push_back("\\" + v_buff);
	    if (line[j] == ';'){
	    	varsBrackets_.push_back(std::count(v_buff.begin(), v_buff.end(), '['));
	      	inputs_.push_back(inp_shares);
	      	inp_shares.clear();
	    }
	    if (line[j] != '.') ++j;
	    while (line[j] != '.'){
	      while ((line[i] != ',') && (line[i] != ';')){
			++i;
	      }
	      while ((line[j] != ',') && (line[j] != '.') && (line[j] != ';')){
			++j;
	      }
	      v_buff = line.substr(i+2,j-i-2);
	      inp_shares.push_back(v_buff);
	      inp_shares.push_back("\\" + v_buff);
	      ++i;
	      if (line[j] == ';'){
	      	varsBrackets_.push_back(std::count(v_buff.begin(), v_buff.end(), '['));
	      	inputs_.push_back(inp_shares);
	      	inp_shares.clear();
	      }
	      if (line[j] != '.') ++j;
	    }
	    varsBrackets_.push_back(std::count(v_buff.begin(), v_buff.end(), '['));
	   	inputs_.push_back(inp_shares);
	  }

	  if(line.find("Random_vars") != std::string::npos){
	    //Save the name of the random variables
		
	    size_t i = 0;
	    size_t j = 0;
	    std::string v_buff;
	    i = line.find(':');
	    while ((line[j] != ',') && (line[j] != '.')){
	      ++j;
	    }
	    v_buff = line.substr(i+2,j-i-2);
	    randomVars_.push_back(v_buff);
	    randomVars_.push_back("\\" + v_buff);
	    if (line[j] != '.') ++j;
	    while (line[j] != '.'){
	      while (line[i] != ','){
			++i;
	      }
	      while ((line[j] != ',') && (line[j] != '.')){
			++j;
	      }
	      v_buff = line.substr(i+2,j-i-2);
	      randomVars_.push_back(v_buff);
	      randomVars_.push_back("\\" + v_buff);
	      ++i;
	      if (line[j] != '.') ++j;
	    }
	  }
	  if(line.find("Outputs_check") != std::string::npos){
	    //Save the name of the output shared variables
	    outsSpecified_ = 1;

	    size_t i = 0;
	    size_t j = 0;
	    std::string v_buff;
	    i = line.find(':');
	    while ((line[j] != ',') && (line[j] != '.')){
	      ++j;
	    }
	    v_buff = line.substr(i+2,j-i-2);
	    outputs_.push_back(v_buff);
	    outputs_.push_back("\\" + v_buff);
	    // std::cout << "Pushed outputs: " << v_buff << " and " << ("\\" + v_buff) << std::endl; 
	    if (line[j] != '.') ++j;
	    while (line[j] != '.'){
	      while (line[i] != ','){
			++i;
	      }
	      while ((line[j] != ',') && (line[j] != '.')){
			++j;
	      }
	      v_buff = line.substr(i+2,j-i-2);
	      outputs_.push_back(v_buff);
	      outputs_.push_back("\\" + v_buff);
	      // std::cout << "Pushed outputs: " << v_buff << " and " << ("\\" + v_buff) << std::endl;
	      ++i;
	      if (line[j] != '.') ++j;
	    }
	  }
	  if(line.find("Regs_layer") != std::string::npos){
	    //Save the name of the register output variables
		
	    size_t i = 0;
	    size_t j = 0;
	    std::vector<std::vector<std::string>> regs_vars;
	    std::vector<std::string> reg_shares;
	    std::string v_buff;
	    i = line.find(':');
	    while ((line[j] != ',') && (line[j] != '.') && (line[j] != ';')){
	      ++j;
	    }
	    v_buff = line.substr(i+2,j-i-2);
	    reg_shares.push_back(v_buff);
	    if (line[j] != '.') ++j;
	    while (line[j] != '.'){
	      while ((line[i] != ',') && (line[i] != ';')){
			++i;
	      }
	      while ((line[j] != ',') && (line[j] != '.') && (line[j] != ';')){
			++j;
	      }
	      v_buff = line.substr(i+2,j-i-2);
	      reg_shares.push_back(v_buff);
	      ++i;
	      if (line[j] == ';'){
	      	regs_vars.push_back(reg_shares);
	      	reg_shares.clear();
	      }
	      if (line[j] != '.') ++j;
	    }
	   	regs_vars.push_back(reg_shares);
	    regsLayers_.push_back(regs_vars);
	  }
	  if(line.find("Check_bits") != std::string::npos){
	    //Save the name of the check bits for fault detection
		
	    size_t i = 0;
	    size_t j = 0;
	    std::string v_buff;
	    i = line.find(':');
	    while ((line[j] != ',') && (line[j] != '.')){
	      ++j;
	    }
	    v_buff = line.substr(i+2,j-i-2);
	    checkBits_.push_back(v_buff);
	    checkBits_.push_back("\\" + v_buff);
	    if (line[j] != '.') ++j;
	    while (line[j] != '.'){
	      while (line[i] != ','){
			++i;
	      }
	      while ((line[j] != ',') && (line[j] != '.')){
			++j;
	      }
	      v_buff = line.substr(i+2,j-i-2);
	      checkBits_.push_back(v_buff);
	      checkBits_.push_back("\\" + v_buff);
	      ++i;
	      if (line[j] != '.') ++j;
	    }
	  }
	  // Get module and architecture names to build later the correspondant .tcl file
	  if(prog_lang == "vhdl"){
	    
	    if((line.find("entity") != std::string::npos) && (!entitySavedAlready)){
	      size_t i = 0;
	      size_t j = 0;
	      
	      i = line.find("entity");
	      i += 6;
	      while (isspace(line[i])){
			++i;
	      }
	      j = line.find(" is");
	      while (isspace(line[j])){
			j--;
	      }
	      module = line.substr(i,j-i+1);
	      entitySavedAlready = 1;
	    }
	    if(line.find("architecture") != std::string::npos){
	      size_t i = 0;
	      size_t j = 0;
	      
	      i = line.find("architecture");
	      i += 12;
	      while (isspace(line[i])){
		++i;
	      }
	      j = line.find(" of");
	      while (isspace(line[j])){
		j--;
	      }
	      architecture = line.substr(i,j-i+1);
	      
	    }
	    
	  }else if(prog_lang == "verilog"){
	    
	    if(((line.find("module") != std::string::npos) and (line.find("endmodule") == std::string::npos)) && (!entitySavedAlready)){
	      size_t i = 0;
	      size_t j = 0;
	      
	      i = line.find("module");
	      i += 6;
	      while (isspace(line[i])){
		++i;
	      }
	      j = line.find("(");
	      j -= 1;
	      while (isspace(line[j])){
		j--;
	      }
	      module = line.substr(i,j-i+1);

	    }
	    architecture = "verilog";
	    entitySavedAlready = 1;
	    cout << "Top module: " << module << " ATTENTION code changed, move top module to be at the top of the file" << endl;
	    
	  }else return 0;
	}
	assert(got_inputs  &&  "Header is missing in analyzed file!!");

	// for (size_t k = 0; k < checkBits_.size(); ++k)
	// {
	// 	std::cout << varsBrackets_[k] << ": ";
	// 	for (size_t i = 0; i < inputs_[k].size(); ++i)
	// 	{
	// 		std::cout << inputs_[k][i];
	// 	}
	// 	std::cout << std::endl;
	// }

	return 1;
  }