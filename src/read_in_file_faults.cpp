#include "VerifTool.h"

  bool VerifTool::read_in_file_faults()
  {
	std::string line;
	int entitySavedAlready = 0;
	
	//Read file
	while(getline(fIn_,line)){
	  
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
	      while (line[i] == ' '){
			++i;
	      }
	      j = line.find(" is");
	      while (line[j] == ' '){
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
	      while (line[i] == ' '){
		++i;
	      }
	      j = line.find(" of");
	      while (line[j] == ' '){
		j--;
	      }
	      architecture = line.substr(i,j-i+1);
	      
	    }
	    
	  }else if(prog_lang == "verilog"){
	    
	    if((line.find("module") != std::string::npos) and (line.find("endmodule") == std::string::npos) and (!entitySavedAlready)){
	      size_t i = 0;
	      size_t j = 0;
	      
	      i = line.find("module");
	      i += 6;
	      while (line[i] == ' '){
		++i;
	      }
	      j = line.find("(");
	      j -= 1;
	      while (line[j] == ' '){
		j--;
	      }
	      module = line.substr(i,j-i+1);
	      entitySavedAlready = 1;

	    }
	    architecture = "verilog";
	    
	  }else return 0;
	}

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