
#include "VerifTool.h"
#include "color.h"
#include "GatesFactory.h"
#include <fstream>

using namespace std;

  void Netlist::create_netlist(std::ifstream &fNetlist, std::vector<std::vector<std::string>> inputs, std::vector<size_t> varsBrackets,
  							   std::vector<std::string> outputs, std::vector<std::string> randomVars,
  							   std::vector<std::vector<std::vector<std::string>>> regsLayers)
  {
    
    std::string line;
	std::string buffLine;
	std::string lastVarFound = "";
	uint varNum = 0; //This variable marks the current sensitive variable being stored. Just used when more that one variable is declared in the header.
	std::vector<std::vector<std::string>> varMatrix;
	std::vector<std::vector<std::vector<std::string>>> inputShares;

	//Read file
	while(getline(fNetlist,line)){
	  //Get the information needed from the header before evaluate the function
	  if(line.find("input") != std::string::npos){
	    //Save the name of the shared variables
	    
	    //Check if the declaration is written in more than one line
	    while (line.find(';') == std::string::npos){
	      getline(fNetlist,buffLine);
	      line += buffLine;
	    }
	    
	    size_t i = 0;
	    size_t j = 0;
	    std::string in_buff;
	    std::vector<std::string> var_buff;
	    
	    i = line.find("input") + 6;
	    if (line[i] == '['){
	      //Bus found
	      int a,b;
	      std::stringstream str1, str2;
	      
	      //Get input and output numbers... might be more that one character
	      j = i;
	      while (line[j] != ':'){
			++j;
	      }
	      str1 << line.substr(i+1,j-i);
	      str1 >> a;
	      while (line[i] != ']'){
			++i;
	      }
	      str2 << line.substr(j+1,i-j);
	      str2 >> b;
	      
	      while (line[j] != ';'){
			++j;
	      }
	      
	      in_buff = line.substr(i+2,j-i-2);

	      if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
	      	//Keep it if you find it in the inputs provided
			if (a >= b){
			  for(int k = b; k <= a; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    var_buff.push_back(var);
			    // std::cout << "Bus saved!" << std::endl;
			  }
			}else{
			  for(int k = a; k <= b; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    var_buff.push_back(var);
			    // std::cout << "Bus saved!" << std::endl;
			  }
			}
			varMatrix.push_back(var_buff);
      	  }else if ((varNum+1 < inputs.size()) && (std::find(inputs[varNum+1].begin(), inputs[varNum+1].end(), in_buff) != inputs[varNum+1].end())){
	      	//Keep it if you find it in the inputs provided
	      	varNum += 1;
			inputShares.push_back(varMatrix);
			varMatrix.clear();
			var_buff.clear();
			if (a >= b){
			  for(int k = b; k <= a; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    var_buff.push_back(var);
			    // std::cout << "Bus saved!" << std::endl;
			  }
			}else{
			  for(int k = a; k <= b; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    var_buff.push_back(var);
			    // std::cout << "Bus saved!" << std::endl;
			  }
			}
			varMatrix.push_back(var_buff);
      	  }else if (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end())
      	  {
      	  	//Keep it if you find it in the randoms provided
			if (a >= b){
			  for(int k = b; k <= a; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    Wire *w = new Wire(var);
			    randomBits_.push_back(w);
			    // std::cout << "Bus rand saved!" << std::endl;
			  }
			}else{
			  for(int k = a; k <= b; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    Wire *w = new Wire(var);
			    randomBits_.push_back(w);
			    // std::cout << "Bus rand saved!" << std::endl;
			  }
			}
      	  }
      	  else
      	  {
      	  	//This is a public value, so just keep it in wires
			if (a >= b){
			  for(int k = b; k <= a; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    Wire *w = new Wire(var);
			    w->set_is_PI(1);
			    wires_.push_back(w);
			    // std::cout << "Public saved1: " << in_buff << "#" << std::endl;
			    // std::cout << "Bus rand saved!" << std::endl;
			  }
			}else{
			  for(int k = a; k <= b; k++){
			    string c = to_string(k);
			    std::string var = in_buff + '[' + c + ']';
			    Wire *w = new Wire(var);
			    w->set_is_PI(1);
			    wires_.push_back(w);
			    // std::cout << "Public saved1: " << in_buff << "#" << std::endl;
			    // std::cout << "Bus rand saved!" << std::endl;
			  }
			}
      	  }
	      
	    }//End saving input bus
	    else{
	      //Not a bus
		    while ((line[j] != ',') && (line[j] != '[') && (line[j] != ';')){
				++j;
		    }
		    //First variable
		    in_buff = line.substr(i,j-i);
		    if ((line[j] == '[') && (varsBrackets[varNum] == 0)){ //Input sensitives specified without any bracket
				in_buff = line.substr(i,j-i);
				lastVarFound = in_buff;
				if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
					while ((line[j] != ',')){
					    ++j;
					}
					if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					else{in_buff = line.substr(i,j-i);}
					var_buff.push_back(in_buff);
					// std::cout << "Input saved: " << in_buff << "#" << std::endl;
				}else if (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end())
				{
					while ((line[j] != ',')){
				    	++j;
				  	}
				  	if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
				  	else{in_buff = line.substr(i,j-i);}
				 	Wire *w = new Wire(in_buff);
				    randomBits_.push_back(w);
				    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				}else
				{
					while ((line[j] != ',')){
				    	++j;
				  	}
				  	if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
				  	else{in_buff = line.substr(i,j-i);}
				 	Wire *w = new Wire(in_buff);
				 	w->set_is_PI(1);
				    wires_.push_back(w);
				    // std::cout << "Public saved2: " << in_buff << "#" << std::endl;
				}
		    }else if ((line[j] == '[') && (varsBrackets[varNum] > 0) && (std::find(randomVars.begin(), randomVars.end(), in_buff) == randomVars.end()))
		    {//Found a bracket and the user notified that his shares come in brackets
		    	size_t bracketsFound = 0;
		    	++j;
		    	while (bracketsFound < varsBrackets[varNum]){
		    		while ((line[j] != '[') && (line[j] != ',') && (line[j] != ' ') && (line[j] != ';')){
						++j;
				    }
				    bracketsFound += 1;
		    	}
		    	if (line[j] == '['){
		    		in_buff = line.substr(i,j-i);
		    		// std::cout << "Variable with brackets found1: " << in_buff << std::endl;
					lastVarFound = in_buff;
					if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
						while ((line[j] != ',')){
						    ++j;
						}
						if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
						else{in_buff = line.substr(i,j-i);}
						var_buff.push_back(in_buff);
						// std::cout << "Input saved: " << in_buff << "#" << std::endl;
					}
		    	}else{
		    		if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					else{in_buff = line.substr(i,j-i);}
					// std::cout << "Variable with brackets found2: " << in_buff << std::endl;
					if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
					    var_buff.push_back(in_buff);
					    varMatrix.push_back(var_buff);
					    // std::cout << "New inputShares row1" << std::endl;
					    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					    var_buff.clear();
					}
		    	}
		    }else if ((line[j] == '[') && (varsBrackets[varNum] > 0) && (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end()))
		    {
		    	while ((line[j] != ',')){
			    	++j;
			  	}
			  	if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
			  	else{in_buff = line.substr(i,j-i);}
			 	Wire *w = new Wire(in_buff);
			    randomBits_.push_back(w);
			    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
			    // std::cout << "Random bit: " << in_buff << std::endl;
		    }else{
				if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
				else{in_buff = line.substr(i,j-i);}
				if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
				    var_buff.push_back(in_buff);
				    varMatrix.push_back(var_buff);
				    // std::cout << "New inputShares row1" << std::endl;
				    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
				    var_buff.clear();
				}else if (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end())
				{
					Wire *w = new Wire(in_buff);
				    randomBits_.push_back(w);
				    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				}else if (in_buff == "clk")
				{
					clk_ = new Wire(in_buff);
					clk_->set_is_PI(1);
				    // std::cout << "Clock saved: " << in_buff << "#" << std::endl;
				}else if (in_buff == "rst")
				{
					rst_ = new Wire(in_buff);
					rst_->set_is_PI(1);
				    // std::cout << "Reset saved: " << in_buff << "#" << std::endl;
				}else if (in_buff == "load")
				{
					load_ = new Wire(in_buff);
					load_->set_is_PI(1);
				    // std::cout << "Load saved: " << in_buff << "#" << std::endl;
				}else if (in_buff == "start")
				{
					start_ = new Wire(in_buff);
					start_->set_is_PI(1);
				    // std::cout << "Start saved: " << in_buff << "#" << std::endl;
				}else
				{
					Wire *w = new Wire(in_buff);
					w->set_is_PI(1);
				    wires_.push_back(w);
				    // std::cout << "Public saved3: " << in_buff << "#" << std::endl;
				}
		    }
		    //Continue with the rest
		    while (line[j] != ';'){
				++j;
				i = j + 1;
                while (line[i] == ' '){
				  ++i;
				}
				while ((line[j] != ',') && (line[j] != '[') && (line[j] != ';')){
				  ++j;
				}
				in_buff = line.substr(i,j-i);
				if ((line[j] == '[') && (varsBrackets[varNum] == 0)){
				    in_buff = line.substr(i,j-i);
				    if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
					    while ((line[j] != ',') && (line[j] != ';')){
					      ++j;
					    }
					    if (in_buff == lastVarFound){
					      if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					      else{in_buff = line.substr(i,j-i);}
					      var_buff.push_back(in_buff);
					      // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					    }else{
					      if (!var_buff.empty()) varMatrix.push_back(var_buff);
					      // std::cout << "New inputShares row2" << std::endl;
					      var_buff.clear();
					      lastVarFound = in_buff;
					      if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					      else{in_buff = line.substr(i,j-i);}
					      // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					      var_buff.push_back(in_buff);
					    }
					}else if ((varNum+1 < inputs.size()) && (std::find(inputs[varNum+1].begin(), inputs[varNum+1].end(), in_buff) != inputs[varNum+1].end()))
					{
						//The program access this if clause only when there is a change in the variables. The variable Number counter
						//is increased so that next iteration the program will get in the above if-clause. The only difference from before
						//is that the variables row to look at has changed. THIS WILL NOT WORK IF THE VARIABLES ARE DECLARED IN DIFFERENT
						//ORDER IN THE NETLIST THAT IN THE HEADER
						varNum += 1;
						if (!var_buff.empty()) {varMatrix.push_back(var_buff); inputShares.push_back(varMatrix);}
						varMatrix.clear();
						var_buff.clear();
						// std::cout << "New variable found" << std::endl;
					    while ((line[j] != ',') && (line[j] != ';')){
					      ++j;
					    }
					    lastVarFound = in_buff;
					    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					    else{in_buff = line.substr(i,j-i);}
					    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					    var_buff.push_back(in_buff);
				    }else if (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end())
				    {
					  	while ((line[j] != ',') && (line[j] != ';')){
					      ++j;
					    }
					    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					    else{in_buff = line.substr(i,j-i);}
					    Wire *w = new Wire(in_buff);
					    randomBits_.push_back(w);
					    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				    }else
				    {
				    	// Public value input, save it just as wire
					  	while ((line[j] != ',') && (line[j] != ';')){
					      ++j;
					    }
					    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					    else{in_buff = line.substr(i,j-i);}
					    Wire *w = new Wire(in_buff);
					    w->set_is_PI(1);
					    wires_.push_back(w);
					    // std::cout << "Public saved4: " << in_buff << "#" << std::endl;
					    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				    }
				}else if ((line[j] == '[') && (varsBrackets[varNum] > 0) && (std::find(randomVars.begin(), randomVars.end(), in_buff) == randomVars.end()))
		    	{//found a bracket and the user notified that his shares come in brackets
		    		size_t bracketsFound = 0;
		    		++j;
			    	while (bracketsFound < varsBrackets[varNum]){
			    		while ((line[j] != '[') && (line[j] != ',') && (line[j] != ' ') && (line[j] != ';')){
							++j;
					    }
					    bracketsFound += 1;
			    	}
			    	if (line[j] == '['){
			    		in_buff = line.substr(i,j-i);
			    		// std::cout << "Variable with brackets found3: " << in_buff << std::endl;
					    if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
						    while ((line[j] != ',') && (line[j] != ';')){
						      ++j;
						    }
						    if (in_buff == lastVarFound){
							    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
							    else{in_buff = line.substr(i,j-i);}
							    var_buff.push_back(in_buff);
							    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
						    }else{
							    if (!var_buff.empty()) varMatrix.push_back(var_buff);
							    // std::cout << "New inputShares row2" << std::endl;
							    var_buff.clear();
							    lastVarFound = in_buff;
							    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
							    else{in_buff = line.substr(i,j-i);}
							    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
							    var_buff.push_back(in_buff);
						    }
						}else if ((varNum+1 < inputs.size()) && (std::find(inputs[varNum+1].begin(), inputs[varNum+1].end(), in_buff) != inputs[varNum+1].end()))
						{
							//The program access this if clause only when there is a change in the variables. The variable Number counter
							//is increased so that next iteration the program will get in the above if-clause. The only difference from before
							//is that the variables row to look at has changed. THIS WILL NOT WORK IF THE VARIABLES ARE DECLARED IN DIFFERENT
							//ORDER IN THE NETLIST THAT IN THE HEADER
							varNum += 1;
							if (!var_buff.empty()) {varMatrix.push_back(var_buff); inputShares.push_back(varMatrix);}
							varMatrix.clear();
							var_buff.clear();
							// std::cout << "New variable found" << std::endl;
						    while ((line[j] != ',') && (line[j] != ';')){
						      ++j;
						    }
						    lastVarFound = in_buff;
						    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
						    else{in_buff = line.substr(i,j-i);}
						    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
						    var_buff.push_back(in_buff);
					    }
			    	}else{
			    		if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
					    else{in_buff = line.substr(i,j-i);}
					    // std::cout << "Variable with brackets found4: " << in_buff << std::endl;
					    if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
						    var_buff.push_back(in_buff);
						    varMatrix.push_back(var_buff);
						    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
						    var_buff.clear();
					    }else if ((varNum+1 < inputs.size()) && (std::find(inputs[varNum+1].begin(), inputs[varNum+1].end(), in_buff) != inputs[varNum+1].end()))
						{
							varNum += 1;
							inputShares.push_back(varMatrix);
							// std::cout << "Variable matrix saved with shares = " << varMatrix.size() << std::endl;
							varMatrix.clear();
							var_buff.clear();
							// std::cout << "New variable found" << std::endl;
						 //    std::cout << "Input saved: " << in_buff << "#" << std::endl;
						    var_buff.push_back(in_buff);
						    varMatrix.push_back(var_buff);
						    var_buff.clear();
					    }
			    	}
				}else if ((line[j] == '[') && (varsBrackets[varNum] > 0) && (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end()))
			    {
			    	while ((line[j] != ',') && (line[j] != ';')){
				    	++j;
				  	}
				  	if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
				  	else{in_buff = line.substr(i,j-i);}
				 	Wire *w = new Wire(in_buff);
				    randomBits_.push_back(w);
				    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				    // std::cout << "Random bit: " << in_buff << std::endl;
			    }else{
				    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
				    else{in_buff = line.substr(i,j-i);}
				    if (std::find(inputs[varNum].begin(), inputs[varNum].end(), in_buff) != inputs[varNum].end()){
					    var_buff.push_back(in_buff);
					    varMatrix.push_back(var_buff);
					    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					    var_buff.clear();
				    }else if ((varNum+1 < inputs.size()) && (std::find(inputs[varNum+1].begin(), inputs[varNum+1].end(), in_buff) != inputs[varNum+1].end()))
					{
						varNum += 1;
						inputShares.push_back(varMatrix);
						// std::cout << "Variable matrix saved with shares = " << varMatrix.size() << std::endl;
						varMatrix.clear();
						var_buff.clear();
						// std::cout << "New variable found" << std::endl;
					    // std::cout << "Input saved: " << in_buff << "#" << std::endl;
					    var_buff.push_back(in_buff);
					    varMatrix.push_back(var_buff);
					    var_buff.clear();
				    }else if (std::find(randomVars.begin(), randomVars.end(), in_buff) != randomVars.end())
				    {
					  	Wire *w = new Wire(in_buff);
					    randomBits_.push_back(w);
					    // std::cout << "Rand saved: " << in_buff << "#" << std::endl;
				    }else if (in_buff == "clk")
					{
						clk_ = new Wire(in_buff);
						clk_->set_is_PI(1);
					    // std::cout << "Clock saved: " << in_buff << "#" << std::endl;
					}else if (in_buff == "rst")
					{
						rst_ = new Wire(in_buff);
						rst_->set_is_PI(1);
					    // std::cout << "Reset saved: " << in_buff << "#" << std::endl;
					}else if (in_buff == "load")
					{
						load_ = new Wire(in_buff);
						load_->set_is_PI(1);
					    // std::cout << "Load saved: " << in_buff << "#" << std::endl;
					}else if (in_buff == "start")
					{
						start_ = new Wire(in_buff);
						start_->set_is_PI(1);
					    // std::cout << "Start saved: " << in_buff << "#" << std::endl;
					}else
					{
						Wire *w = new Wire(in_buff);
						w->set_is_PI(1);
					    wires_.push_back(w);
					    // std::cout << "Public saved5: " << in_buff << "#" << std::endl;
					}
				}
		    }
		    if (!var_buff.empty()) {varMatrix.push_back(var_buff); inputShares.push_back(varMatrix); varMatrix.clear();}
		    else if (!varMatrix.empty()) {inputShares.push_back(varMatrix); varMatrix.clear();};
	    }//End saving inputs
	    // std::cout << "End saving inputs" << std::endl;
	  }else if(line.find("output") != std::string::npos){
	    //Save the name of the outputs
	    
	    //Check if the declaration is written in more than one line
	    while (line.find(';') == std::string::npos){
	      getline(fNetlist,buffLine);
	      line += buffLine;
	    }
	    
	    size_t i = 0;
	    size_t j = 0;
	    std::string in_buff;
	    
	    i = line.find("output") + 7;
	    if (line[i] == '['){
	      //Bus found
	      int a,b;
	      std::stringstream str1, str2;
	      
	      //Get input and output numbers... might be more that one character
	      j = i;
	      while (line[j] != ':'){
			++j;
	      }
	      str1 << line.substr(i+1,j-i);
	      str1 >> a;
	      while (line[i] != ']'){
			++i;
	      }
	      str2 << line.substr(j+1,i-j);
	      str2 >> b;
	      
	      while (line[j] != ';'){
			++j;
	      }
	      
	      in_buff = line.substr(i+2,j-i-2);
	      
	      if (a >= b){
			for(int k = b; k <= a; k++){
			  string c = to_string(k);
			  std::string var = in_buff + '[' + c + ']';
			  Wire *w = new Wire(var);
			  if (std::find(outputs.begin(), outputs.end(), in_buff) != outputs.end()){
			  	//Set as sensitive output if provided in the header
			  	w->set_is_SensO(1);
			  }
			  outs_.push_back(w);
			}
	      }else{
			for(int k = a; k <= b; k++){
			  string c = to_string(k);
			  std::string var = in_buff + '[' + c + ']';
			  Wire *w = new Wire(var);
			  if (std::find(outputs.begin(), outputs.end(), in_buff) != outputs.end()){
			  	//Set as sensitive output if provided in the header
			  	w->set_is_SensO(1);
			  }
			  outs_.push_back(w);
			}
	      }
	    }//End saving output bus
	    else{
	      //Not a bus
	      while ((line[j] != ',') && (line[j] != ';')){
			++j;
	      }
	      if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
	   	  else{in_buff = line.substr(i,j-i);}
	      Wire *w = new Wire(in_buff);
	      //We use this function to check if in_buff contains any of the strings from outputs vector
	      if (std::any_of(outputs.begin(), outputs.end(), [&in_buff](const std::string & str) {
														return (in_buff.find(str) != string::npos);
													})){
	      	//Set as sensitive output if provided in the header
		  	w->set_is_SensO(1);
		  }
	      outs_.push_back(w);
		
	      //Continue with the rest
	      while (line[j] != ';'){
			++j;
			i = j + 1;
              while (line[i] == ' '){
			  ++i;
			}
			while ((line[j] != ',') && (line[j] != ';')){
			  ++j;
			}

			if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
	    	else{in_buff = line.substr(i,j-i);}
			Wire *w = new Wire(in_buff);
			// std::cout << in_buff << std::endl;
			if (std::any_of(outputs.begin(), outputs.end(), [&in_buff](const std::string & str) {
									// std::cout << "Str is:" << str << " Inbuffff is:" << in_buff << " is it contained?: " << (in_buff.find(str) != string::npos) << std::endl;
														return (in_buff.find(str) != std::string::npos);
													})){
		    	//Set as sensitive output if provided in the header
		    	// std::cout << "Set as sensitive" << std::endl;
				w->set_is_SensO(1);
			}
			outs_.push_back(w);
	      }
	    }//End saving outputs
	    // std::cout << "End saving outputs" << std::endl;
	  }else if(line.find("wire") != std::string::npos){
	    //std::cout << "Saving wires" << std::endl;
	    //Check if the declaration is written in more than one line
	    while (line.find(';') == std::string::npos){
	      getline(fNetlist,buffLine);
	      line += buffLine;
	    }
	    
	    size_t i = 0;
	    size_t j = 0;
	    std::string in_buff;
	    
	    i = line.find("wire") + 5;
          while (line[i] == ' '){
	      ++i;
	    }
	    if (line[i] == '[')
	    {
	    	//Bus found
	      int a,b;
	      std::stringstream str1, str2;
	      //Get input and output numbers... might be more that one character
	      j = i;
	      while (line[j] != ':'){
			++j;
	      }
	      str1 << line.substr(i+1,j-i);
	      str1 >> a;
	      while (line[i] != ']'){
			++i;
	      }
	      str2 << line.substr(j+1,i-j);
	      str2 >> b;
	      
	      while (line[j] != ';'){
			++j;
	      }
	      
	      in_buff = line.substr(i+2,j-i-2);
	      
	      if (a >= b){
			for(int k = b; k <= a; k++){
			  string c = to_string(k);
			  std::string var = in_buff + '[' + c + ']';
			  Wire *w = new Wire(var);
			  wires_.push_back(w);
			}
	      }else{
			for(int k = a; k <= b; k++){
			  string c = to_string(k);
			  std::string var = in_buff + '[' + c + ']';
			  Wire *w = new Wire(var);
			  wires_.push_back(w);
			}
	      }
	    }else{
	    	while ((line[j] != ',') && (line[j] != ';')){
		      ++j;
		    }
		    if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
		    else{in_buff = line.substr(i,j-i);}
		    Wire *w = new Wire(in_buff);
		    wires_.push_back(w);
		      
		    //Continue with the rest
		    while (line[j] != ';'){
		      ++j;
		      i = j + 1;
                while (line[i] == ' '){
				++i;
		      }
		      while ((line[j] != ',') && (line[j] != ';')){
				++j;
		      }
		      if (line[j-1] == ' '){in_buff = line.substr(i,j-i-1);}
		      else{in_buff = line.substr(i,j-i);}
		      Wire *w = new Wire(in_buff);
		      wires_.push_back(w);
	    	}
		    
	    }
	    // std::cout << "End saving wires" << std::endl;
	  }//End saving wires

	  //Check if the declaration is written in more than one line
	  while (line.find(';') == std::string::npos){
	    if (line.find("endmodule") != std::string::npos) break;
	    getline(fNetlist,buffLine);
	    line += buffLine;
	  }//Intermediate value declaration saved completely
	  if (line.find("endmodule") != std::string::npos) break;
	  
	  //Look for each component declaration in the file
	  if ((line.find("input") == std::string::npos) and 
	  	  (line.find("output") == std::string::npos) and 
	  	  (line.find("wire") == std::string::npos) and
	  	  (line.find("module") == std::string::npos) and 
	  	  (line.find("endmodule") == std::string::npos)){

	  		componentDeclarations_.push_back(line);
	  }
	  
	}//End reading file

	fNetlist.close();
	std::cout << "Closing netlist" << std::endl;

	// for (size_t i = 0; i < inputShares.size(); ++i)
	// {
	// 	std::cout << inputShares[i].size() << std::endl;
	// }

	//Change order of input shares in format Vars*Shares matrix and create Wire for each input
	// assert(inputShares.size() > 0);
	if (!(inputShares.size() > 0))
	{
		std::cout << BOLD(FRED("No sensitive input data gathered. Simulation will be OK but not security evaluation")) << std::endl;
	}
	for (size_t i = 0; i < inputShares.size(); ++i)
	{
		// std::cout << "Var " << i << std::endl;
		for (size_t k = 0; k < inputShares[i][0].size(); ++k)
		{
			wires var_buff;
			for (size_t j = 0; j < inputShares[i].size(); ++j)
			{	
				// std::cout << inputShares[i][j][k];
				Wire *w = new Wire(inputShares[i][j][k]);
				var_buff.push_back(w);
				inputsMap_[w->get_name()] = w;
			}
			// std::cout << std::endl;
			sharedVariables_.push_back(var_buff);
		}
	}
	//Add clk, rst, start and load to inputs map
	if (clk_ != NULL) inputsMap_[clk_->get_name()] = clk_;
	if (rst_ != NULL) inputsMap_[rst_->get_name()] = rst_;
	if (start_ != NULL) inputsMap_[start_->get_name()] = start_;
	if (load_ != NULL) inputsMap_[load_->get_name()] = load_;

	// while(!inputShares[0].empty()){
	//   wires var_buff;
	//   for (size_t k = 0; k < inputShares.size(); ++k){
	//     Wire *w = new Wire(inputShares[k].front());
	//     var_buff.push_back(w);
	//     inputsMap_[w->get_name()] = w;
	//     // std::cout << "Map inputs: " << w->get_name() << "#" << std::endl;
	//     inputShares[k].erase(inputShares[k].begin());
	//     //if (inputShares[k].empty()) inputShares.erase(inputShares.begin()+k);
	//   }
	//   sharedVariables_.push_back(var_buff);
	// }
	
	//Plot shared variables
	// for (size_t l = 0; l < sharedVariables_.size(); ++l){	    
	//     for (size_t t = 0; t < sharedVariables_[l].size(); ++t){
	//     	std::cout << sharedVariables_[l][t]-> get_name() << "; ";
	//     }
	//     std::cout << std::endl;
	// }
	

	//Map randommbits name->wire, so it is easier to search and retrieve a wire
	for (size_t k = 0; k < randomBits_.size(); ++k){
	  randomsMap_[randomBits_[k]->get_name()] = randomBits_[k];
	  // std::cout << "Map random: " << randomBits_[k]->get_name() << "#" << std::endl;
	}

	//Map wires name->wire, so it is easier to search and retrieve a wire
	for (size_t k = 0; k < wires_.size(); ++k){
	  wiresMap_[wires_[k]->get_name()] = wires_[k];
	  // std::cout << "Map wire: " << wires_[k]->get_name() << "#" << std::endl;
	}

	//Map outs name->wire, so it is easier to search and retrieve an output get the ready signal as well
	for (size_t k = 0; k < outs_.size(); ++k){
		outsMap_[outs_[k]->get_name()] = outs_[k];
		if ((outs_[k]->get_name() == "ready") || (outs_[k]->get_name() == "done"))
		{
			ready_ = outs_[k];
		}
	  // std::cout << "Map out: " << outs_[k]->get_name() << "#" << std::endl;
	}
	//Instantiate FACTORY
	gFactory_ = new GatesFactory(inputsMap_, randomsMap_, wiresMap_, outsMap_);
	
	//Create gates
	sequential_ = 0;
	for (size_t i = 0; i < componentDeclarations_.size(); ++i){
	  	      	     
	    components_.push_back(gFactory_->create_gate(componentDeclarations_[i], zeroOnes_, components_));
	    if (((componentDeclarations_[i].find("DFF") != std::string::npos) || (componentDeclarations_[i].find("SDFF") != std::string::npos)) && (sequential_ == 0)){
	    	sequential_ = 1;
	    }
	}//End saving interm values
	std::cout << "End creating/saving components" << std::endl;
	
	//Get inputs/outputs from flipflips
	if (regsLayers.empty())
	{
		std::cout << BOLD(FRED("No registers sensitive data were specifyed")) << std::endl;
		assert(!sequential_ && "Your design is sequential, you must specify regs layers");
	}
	if (sequential_)
	{
		get_ff_inOuts();
		if (!regsLayers.empty())
		{
			layers_sensitive_data(regsLayers);
			build_subcircuits();
		}		
	}
	// std::cout << "End Get inputs/outputs from flipflips" << std::endl;

	// for (int i = 0; i < outs.size(); ++i)
	// {
	// 	std::cout << outs[i]->name << std::endl;
	// 	std::cout << "Comming from: " << outs[i]->pinFrom->gate->gateget_name() << " pinTo?? " << (outs[i]->pinTo != NULL)<< std::endl;
	// 	if (outs[i]->pinTo != NULL)
	// 	{
	// 		std::cout << " and going to: " << outs[i]->pinTo->gate->gateget_name();
	// 		if (outs[i]->fanOut)	
	// 		{
	// 			for (int j = 0; j < outs[i]->fanOutPins.size(); ++j)
	// 			{
	// 				std::cout << " fanOut to: " << outs[i]->fanOutPins[j]->gate->gateget_name();
	// 			}
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// }


	// std::cout << "New outputs" << std::endl;
	// for (int i = 0; i < outputsFF_.size(); ++i)
	// {
	// 	std::cout << outputsFF_[i]->name << std::endl;
	// }
	// std::cout << "New inputs" << std::endl;
	// for (int i = 0; i < inputsFF_.size(); ++i)
	// {
	// 	std::cout << inputsFF_[i]->name << std::endl;
	// }



	// for(size_t h = 0; h < components_.size(); ++h){
	  
	//   std::cout << "gate: " << components_[h]->get_name() << "@" << " With type: " << components_[h]->get_type() << "@" << " and inputs= " << components_[h]->get_inputs().size() << std::endl;
	//   for(size_t k = 0; k < components_[h]->get_inputs().size(); ++k){
	//       std::cout << "    " << components_[h]->get_inputs()[k]->get_name() <<  " getting wire: " << components_[h]->get_inputs()[k]->get_wire()->get_name();
	//   }
	//   std::cout << std::endl;
	//   std::cout << "Output pin: " << components_[h]->get_output()->get_name() << " driving wire: " << components_[h]->get_output()->get_wire()->get_name() << " Coming form pin " << components_[h]->get_output()->get_wire()->get_pinFrom()->get_name() << std::endl;
	// }



	// for (int i = 0; i < sharedVariables.size(); ++i)
	// {
	// 	for (int j = 0; j < sharedVariables[i].size(); ++j)
	// 	{
	// 		std::cout << sharedVariables[i][j]->name << std::endl;
	// 		std::cout << "Going to: " << sharedVariables[i][j]->pinTo->gate->gateget_name();


	// 			if (sharedVariables[i][j]->fanOut)	
	// 			{
	// 				for (int k = 0; k < sharedVariables[i][j]->fanOutPins.size(); ++k)
	// 				{
	// 					std::cout << " fanOut to: " << sharedVariables[i][j]->fanOutPins[k]->gate->gateget_name();
	// 				}
	// 			}

	// 		std::cout << std::endl;
	// 	}
	// }



  }
