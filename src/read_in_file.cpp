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