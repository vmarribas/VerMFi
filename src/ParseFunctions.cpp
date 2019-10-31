#include "VerifTool.h"
#include "Gates.h"
#include "GatesFactory.h"

string Netlist::get_module_name(string line){

	string module;

	int i = (int)line.find("module");
	//Get to the first space
	while (!isspace(line[i])){
		++i;
    }
    ++i;
    int j = i;
    #ifndef YOSYS
	    while (!isspace(line[j])){
			++j;
	    }
	#else
	    while ((!isspace(line[j])) && (line[j] != '(')){
			++j;
	    }
	#endif
    module = line.substr(i,j-i);

    return module;

};



void Netlist::get_inputs(SuperGate* superGate, string line){

	string inBuff;

	int i = line.find(" input ") + 7;
	int j = i;

	while (line[j] != ';'){

		while ((isspace(line[i])) || (line[i] == ',')){
			++i;
		}
		while ((line[j] != ',') && (line[j] != ';')){
			++j;
		}

		if (isspace(line[j-1])){inBuff = line.substr(i,j-i-1);}
		else{inBuff = line.substr(i,j-i);}

		// cout << inBuff << "#" << endl;

		//Create a new wire
		Wire *w = new Wire(inBuff);
		superGate->inputWires_.push_back(w);
		superGate->inputsMap_[inBuff] = w;

		if (line[j] != ';'){
			j++;
			i = j;
		}
	}
};

void Netlist::get_outputs(SuperGate* superGate, string line){

	string inBuff;

	int i = (int)line.find("output ") + 7;
	int j = i;

	while (line[j] != ';'){

		while ((isspace(line[i])) || (line[i] == ',')){
			++i;
		}
		while ((line[j] != ',') && (line[j] != ';')){
			++j;
		}

		if (isspace(line[j-1])){inBuff = line.substr(i,j-i-1);}
		else{inBuff = line.substr(i,j-i);}

		// cout << inBuff << "#" << endl;

		//Create a new wire
		Wire *w = new Wire(inBuff);
		superGate->outputWires_.push_back(w);
		superGate->outsMap_[inBuff] = w;

		if (line[j] != ';'){
			j++;
			i = j;
		}
	}
};

void Netlist::get_wires(SuperGate* superGate, string line){

	string inBuff;

	int i = (int)line.find("wire") + 5;
	int j = i;

    while (isspace(line[i])){
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
      ++i;
        while (isspace(line[i])){
	      ++i;
	  }
      inBuff = line.substr(i,j-i);

      
      if (a >= b){
		for(int k = b; k <= a; k++){
		  string c = to_string(k);
		  std::string var = inBuff + '[' + c + ']';
		  Wire *w = new Wire(var);
		  superGate->wires_.push_back(w);
		  superGate->wiresMap_[var] = w;
		}
      }else{
		for(int k = a; k <= b; k++){
		  string c = to_string(k);
		  std::string var = inBuff + '[' + c + ']';
		  Wire *w = new Wire(var);
		  superGate->wires_.push_back(w);
		  superGate->wiresMap_[var] = w;
		}
      }
    }
    else{
    	while (line[j] != ';'){

			while ((isspace(line[i])) || (line[i] == ',')){
				++i;
			}
			while ((line[j] != ',') && (line[j] != ';')){
				++j;
			}

			if (isspace(line[j-1])){inBuff = line.substr(i,j-i-1);}
			else{inBuff = line.substr(i,j-i);}

			// cout << inBuff << "#" << endl;

			//Create a new wire
			Wire *w = new Wire(inBuff);
			superGate->wires_.push_back(w);
			superGate->wiresMap_[inBuff] = w;

			if (line[j] != ';'){
				j++;
				i = j;
			}
		}
    }
};


Gate* Netlist::create_primaryInput_buffer(Wire* inputWire){

	//Create buffer
	Gate *g = new BUFFgate("PrimaryInput_" + inputWire->get_name());
	//Create input pin... the wire will be connected when instantiating this module
	Pin *p1 = new Pin("I");
	p1->append_gate(g);
	g->append_input_pin(p1);
	//Create output pin
	Pin *p2 = new Pin("O");
	p2->append_wire(inputWire);
	p2->append_gate(g);
	g->append_output_pin(p2);
	//Now this gate drives the input
	inputWire->append_pin_from(p2);

	return g;
};

Gate* Netlist::create_primaryOutput_buffer(Wire* outputWire){

	//Create buffer
	Gate *g = new BUFFgate("PrimaryOutput_" + outputWire->get_name());
	//Create input pin... 
	Pin *p1 = new Pin("I");
	p1->append_gate(g);
	g->append_input_pin(p1);
	p1->append_wire(outputWire);
	//Create output pin... the wire will be connected when instantiating this module
	Pin *p2 = new Pin("O");
	p2->append_gate(g);
	g->append_output_pin(p2);
	//Now this gate drives the input
	outputWire->append_pin_to(p1);

	return g;
};

void Netlist::get_superGate_pins(SuperGate *thisSuperGate, SuperGate *subSuperGate, string line){


	int i = 0;
	int j = 0;

//     cout << "CONNECTING MODULE _" << thisSuperGate->type_ << "_ AND SUBMODULE _" << subSuperGate->type_ << endl;
//
//     map<std::string, Wire *>::iterator it;
//     cout << "Wires in Map for thisSuperGate>>>>>>>>>>>>>>>>>>" << endl;
//     for ( it = thisSuperGate->wiresMap_.begin(); it != thisSuperGate->wiresMap_.end(); it++ )
//     {
//         std::cout << it->first
//                   << "_ : _"
//                   << it->second->get_name() << "_"  // string's value
//                   << std::endl ;
//     }
//     cout << "Input in Map for subSuperGate>>>>>>>>>>>>>>>>>>>" << endl;
//     for ( it = subSuperGate->inputsMap_.begin(); it != subSuperGate->inputsMap_.end(); it++ )
//     {
//         std::cout << it->first
//                   << "_ : _"
//                   << it->second->get_name() << "_"  // string's value
//                   << std::endl ;
//     }

	while(1){

		string pinName;
		string wireName;

		//Get pin
		while((line[i] != '.') && (line[i] != ';'))	i++;
		if (line[i] == ';') break;
		j = i;
		while (line[j] != '(') j++;
		while(isspace(line[j-1])) j--;

		pinName = line.substr(i+1,j-i-1);
		// cout << pinName << "#" << endl;

		//Get wire
		i = j;
		while(isspace(line[i])) i++;
		while(line[j] != ')') j++;
		while(isspace(line[j-1])) j--;

		wireName = line.substr(i+1,j-i-1);
		// cout << wireName << "#" << endl;

		//Now make the connections
		if (subSuperGate->inputsMap_.find(pinName) != subSuperGate->inputsMap_.end()){
			// cout << "INPUT PIN" << endl;
			//Connet to Primary Input buffer
			if (thisSuperGate->inputsMap_.find(wireName) != thisSuperGate->inputsMap_.end())
			{
				//Connect one of the inputs
				// cout << "Conneect input wire" << endl;
				subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->append_wire(thisSuperGate->inputsMap_[wireName]);
				thisSuperGate->inputsMap_[wireName]->append_pin_to(subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]);
				// if (subSuperGate->inputsMap_[pinName]->get_name() != "clk")
				// {
				// 	cout << "SUBMODULE wire: " << subSuperGate->inputsMap_[pinName]->get_name() << endl;
				// 	cout << " comes from pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_name() << endl;
				// 	cout << " comes from gate: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_name() << endl;
				// 	cout << " with in Pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->get_name() << endl;
				// }

			}else if (thisSuperGate->wiresMap_.find(wireName) != thisSuperGate->wiresMap_.end())
			{
				//Connect one of the wires
				// cout << "Conneect interm. wire" << endl;
				subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->append_wire(thisSuperGate->wiresMap_[wireName]);
				thisSuperGate->wiresMap_[wireName]->append_pin_to(subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]);
				// if (subSuperGate->inputsMap_[pinName]->get_name() != "clk")
				// {
				// 	cout << "SUBMODULE wire: " << subSuperGate->inputsMap_[pinName]->get_name() << endl;
				// 	cout << " comes from pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_name() << endl;
				// 	cout << " comes from gate: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_name() << endl;
				// 	cout << " with in Pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->get_name() << endl;
				// 	cout << " and from wire: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->get_wire()->get_name() << endl;
				// }


			}else if (thisSuperGate->outsMap_.find(wireName) != thisSuperGate->outsMap_.end())
			{
				//Connect one of the outputs
				// cout << "Conneect output wire" << endl;
				subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->append_wire(thisSuperGate->outsMap_[wireName]);
				thisSuperGate->outsMap_[wireName]->append_pin_to(subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]);
				// if (subSuperGate->inputsMap_[pinName]->get_name() != "clk")
				// {
				// 	cout << "SUBMODULE wire: " << subSuperGate->inputsMap_[pinName]->get_name() << endl;
				// 	cout << " comes from pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_name() << endl;
				// 	cout << " comes from gate: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_name() << endl;
				// 	cout << " with in Pin: " << subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->get_name() << endl;
				// }

			}else if ((wireName == "1'b0") || (wireName == "1'b1") || (wireName == "1'h0") || (wireName == "1'h1"))
			{
				//Found input 1'b0, 1'b0
				//Creat one or zero interconnect wire
				Wire *w;
				if ((wireName == "1'b0") || (wireName == "1'h0"))
				{
					w = new Wire("zero_interconnet");
					w->set_value(0);
				}else
				{
					w = new Wire("one_interconnet");
					w->set_value(1);
				}
				//Connect to the pin
				thisSuperGate->zeroOnes_.push_back(w);

				subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]->append_wire(w);
				w->append_pin_to(subSuperGate->inputsMap_[pinName]->get_pinFrom()->get_gate()->get_inputs()[0]);

				// cout << "Zero or one input found to module: " << subSuperGate->type_ << endl;
				// cout << w->get_name() <<  " goes to gate " << w->get_pinTo()->get_gate()->get_name() << endl;


			}
		}else if (subSuperGate->outsMap_.find(pinName) != subSuperGate->outsMap_.end()){
			// cout << "OUTPUT PIN" << endl;
			//Connet to Primary Output buffer
			if (thisSuperGate->wiresMap_.find(wireName) != thisSuperGate->wiresMap_.end())
			{
				//Connect one of the wires
				// cout << "Conneect intem. wire" << endl;
				subSuperGate->outsMap_[pinName]->get_pinTo()->get_gate()->get_output()->append_wire(thisSuperGate->wiresMap_[wireName]);
				thisSuperGate->wiresMap_[wireName]->append_pin_from(subSuperGate->outsMap_[pinName]->get_pinTo()->get_gate()->get_output());
			}else if (thisSuperGate->outsMap_.find(wireName) != thisSuperGate->outsMap_.end())
			{
				//Connect one of the outputs
				// cout << "Conneect output wire" << endl;
				subSuperGate->outsMap_[pinName]->get_pinTo()->get_gate()->get_output()->append_wire(thisSuperGate->outsMap_[wireName]);
				thisSuperGate->outsMap_[wireName]->append_pin_from(subSuperGate->outsMap_[pinName]->get_pinTo()->get_gate()->get_output());

			}
		}
	}

};

void Netlist::get_sub_ff_inouts(superGates const &sGates, wires &inputsFF, wires &outputsFF){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		for (size_t i = 0; i < sGates.size(); ++i)
		{	
			//First get the name of all logic gates under this subgate
			for (size_t j = 0; j < sGates[i]->logicGates_.size(); ++j)
			{
				if ((sGates[i]->logicGates_[j]->gateType_ == Gate::DFF) || (sGates[i]->logicGates_[j]->gateType_ == Gate::SDFF))
				{
					outputsFF.push_back(sGates[i]->logicGates_[j]->inputs_[0]->get_wire());
					inputsFF.push_back(sGates[i]->logicGates_[j]->output_->get_wire());
				}
			}
			get_sub_ff_inouts(sGates[i]->superGates_, inputsFF, outputsFF);
		}
	}
}


void Netlist::rename_sub_super_gates(superGates &sGates, string name){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		for (size_t i = 0; i < sGates.size(); ++i)
		{	
			string totalName = name + sGates[i]->name_ + "/";

			//Change this superComponent name
			sGates[i]->name_ = name + sGates[i]->name_;

			//First change the name of all logic gates under this subgate, including Primary InsOuts
			for (size_t j = 0; j < sGates[i]->logicGates_.size(); ++j)
			{
				sGates[i]->logicGates_[j]->gateName_ = totalName + sGates[i]->logicGates_[j]->gateName_;
			}
			for (size_t j = 0; j < sGates[i]->primaryInputs_.size(); ++j)
			{
				sGates[i]->primaryInputs_[j]->gateName_ = totalName + sGates[i]->primaryInputs_[j]->gateName_;
			}
			for (size_t j = 0; j < sGates[i]->primaryOutputs_.size(); ++j)
			{
				sGates[i]->primaryOutputs_[j]->gateName_ = totalName + sGates[i]->primaryOutputs_[j]->gateName_;
			}

			//Change inputs, outputs and wires
			for (size_t j = 0; j < sGates[i]->inputWires_.size(); ++j)
			{
				sGates[i]->inputWires_[j]->name_ = totalName + sGates[i]->inputWires_[j]->name_;
			}
			for (size_t j = 0; j < sGates[i]->outputWires_.size(); ++j)
			{
				sGates[i]->outputWires_[j]->name_ = totalName + sGates[i]->outputWires_[j]->name_;
			}
			for (size_t j = 0; j < sGates[i]->wires_.size(); ++j)
			{
				sGates[i]->wires_[j]->name_ = totalName + sGates[i]->wires_[j]->name_;
			}
			for (size_t j = 0; j < sGates[i]->zeroOnes_.size(); ++j)
			{
				sGates[i]->zeroOnes_[j]->name_ = totalName + sGates[i]->zeroOnes_[j]->name_;
			}

			//Continue with the rest
			rename_sub_super_gates(sGates[i]->superGates_, totalName);
		}
	}
};



void Netlist::plot_names_sub_gates(superGates const &sGates){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		for (size_t i = 0; i < sGates.size(); ++i)
		{	
			//Plot this superComponent name
			cout << "SuperComponent: " << sGates[i]->name_ << " with gates: " << endl;

			//First plot the name of all logic gates under this subgate
			for (size_t j = 0; j < sGates[i]->logicGates_.size(); ++j)
			{
				cout << "	" << sGates[i]->logicGates_[j]->gateName_ << endl;
			}
			for (size_t j = 0; j < sGates[i]->primaryInputs_.size(); ++j)
			{
				cout << "	" << sGates[i]->primaryInputs_[j]->gateName_ << endl;
			}
			for (size_t j = 0; j < sGates[i]->primaryOutputs_.size(); ++j)
			{
				cout << "	" << sGates[i]->primaryOutputs_[j]->gateName_ << endl;
			}
			cout << "AND wires: " << endl;
			for (size_t j = 0; j < sGates[i]->inputWires_.size(); ++j)
			{
				cout << "	" << sGates[i]->inputWires_[j]->name_ << endl;
			}
			for (size_t j = 0; j < sGates[i]->outputWires_.size(); ++j)
			{
				cout << "	" << sGates[i]->outputWires_[j]->name_ << endl;
			}
			for (size_t j = 0; j < sGates[i]->wires_.size(); ++j)
			{
				cout << "	" << sGates[i]->wires_[j]->name_ << endl;
			}

			//Continue with the rest
			plot_names_sub_gates(sGates[i]->superGates_);
		}
	}
}



void Netlist::plot_data_path(Wire *w){

	cout << "Wire: " << w->name_ << endl;

	if (w->get_pinTo()->get_gate() == NULL)
	{
		cout << "This wire should be an output" << endl;
		return;
	}

	cout << "goes to Gates: " << endl;

	cout << "  " << w->get_pinTo()->get_gate()->get_name() << endl;
	plot_data_path(w->get_pinTo()->get_gate()->get_output()->get_wire());

	for (size_t i = 0; i < w->get_fanOutPins().size(); ++i)
	{
		w->get_fanOutPins()[i]->get_gate()->get_name();
		plot_data_path(w->get_fanOutPins()[i]->get_gate()->get_output()->get_wire());

	}

}

void Netlist::plot_data_path_output(Wire *w){

	cout << "Wire: " << w->name_ << endl;

	if (w->get_pinFrom() == NULL)
	// if ((w->get_pinFrom() == NULL) || (w->get_pinFrom()->get_gate()->get_type() == Gate::DFF))
	{
		cout << "There is no gate driving this" << endl;
		return;
	}

	cout << "comes from Gates: " << endl;

	cout << "  " << w->get_pinFrom()->get_gate()->get_name() << endl;

	for (size_t i = 0; i < w->get_pinFrom()->get_gate()->get_inputs().size(); ++i)
	{
		plot_data_path_output(w->get_pinFrom()->get_gate()->get_inputs()[i]->get_wire());
	}

}

//Only for Yosys parsing
#ifdef YOSYS

void Netlist::chop_modules_declaration(std::ifstream &fNetlist, map<string, bool> &modulesParsed, vector< vector<string> > &modulesChopped){

	std::string line;
	std::string buffLine;
	string moduleName;

	std::vector<string> moduleDeclarations;

	cout << "hello heloo" << endl;

	while(getline(fNetlist,line)){

		//Avoid comments and blank lines
		if ((line.length() == 0) || 
			((line[0] == '/') && (line[1] == '/')) || 
			((line.find("(*") != std::string::npos) && (line.find("*)") != std::string::npos)) ||
			((line.find("/*") != std::string::npos) && (line.find("*/") != std::string::npos))) continue;

		//Get the whole line
		while ((line.find(';') == std::string::npos) and (line.find("endmodule") == std::string::npos)){
		    if (!getline(fNetlist,buffLine)) break;
		    line += buffLine;
	    }
	    // cout << "    " <<  line << endl; 
	    // cout << "#########" << endl;
		moduleDeclarations.push_back(line);

		//Keep module in map
		if((line.find("endmodule") == std::string::npos) && (line.find("module") != std::string::npos))
		{
			//Get module name
			moduleName = get_module_name(line);
			
			modulesParsed[moduleName] = 0;
			
			cout << moduleName << endl;
		}
		else if(line.find("endmodule") != std::string::npos)
		{//Found endmodule
			//Finished with declarations for one module
			modulesChopped.push_back(moduleDeclarations);
			moduleDeclarations.clear();
		}
	}
	cout << "End chop modules" << endl;
}

void Netlist::get_submodules(std::ifstream &fNetlist, const map<string, bool> modulesParsed, vector< vector<string> > &modulesHy){

	std::string line;
	std::string buffLine;
	std::vector<string> submodules;
	string moduleName;

	while(getline(fNetlist,line)){

		//Avoid comments and blank lines
		if ((line.length() == 0) || 
			((line[0] == '/') && (line[1] == '/')) || 
			((line.find("(*") != std::string::npos) && (line.find("*)") != std::string::npos)) ||
			((line.find("/*") != std::string::npos) && (line.find("*/") != std::string::npos))) continue;
		
		//Get first word of the declaration (used later to identify modules)
		int tt = 0;
	    int yy = 0;
	    string moduleBuff;
    	while (isspace(line[tt])) tt++;
	    yy=tt;
	    while ((!isspace(line[yy])) && (line[yy] != '(')) yy++;
	    moduleBuff = line.substr(tt,yy-tt);

	    if((line.find("endmodule") == std::string::npos) && (line.find("module") != std::string::npos))
		{
			//Get module name
			moduleName = get_module_name(line);
			
			submodules.push_back(moduleName);
		}
		else if (modulesParsed.find(moduleBuff) != modulesParsed.end())
		{//Found a suprGate declaration
			submodules.push_back(moduleBuff);
		}
		else if(line.find("endmodule") != std::string::npos)
		{//Found endmodule
			//Finished with declarations for one module
			modulesHy.push_back(submodules);
			submodules.clear();
		}
	}

	cout << "End submodules" << endl;
}

void Netlist::create_yosys_netlist(const vector<string> moduleDeclarations){

	//Read first all the different modules in the design
	std::vector<std::string> modules;
	string topModule;


	std::string line;
	std::string buffLine;
	SuperGate *sGateBuff = NULL;
	string moduleName;

	for(unsigned i = 0; i < moduleDeclarations.size(); ++i) {
		
		line = moduleDeclarations[i];
		
		//Get first word of the declaration (used later to identify modules)
		int tt = 0;
	    int yy = 0;
	    string moduleBuff;

    	while (isspace(line[tt])) tt++;
	    yy=tt;
	    while ((!isspace(line[yy])) && (line[yy] != '(')) yy++;
	    moduleBuff = line.substr(tt,yy-tt);

	    // cout << moduleBuff << endl;

	    // cout << line << endl;


		if((line.find("endmodule") == std::string::npos) && (line.find("module") != std::string::npos))
		{

			//New superGate
			if (sGateBuff != nullptr) delete sGateBuff;

			//Get module name
			moduleName = get_module_name(line);
			modules.push_back(moduleName);
			// cout << "@@@@@@@@@@@@@@@@@@" << moduleName << endl;

			//Create new super gate (module)
			sGateBuff = new SuperGate(moduleName);

			if (moduleName == topModule_)
			{
				topModuleDeclaration_ = line;
			}

		}
		else if(line.find("endmodule") != std::string::npos)
		{//Found endmodule

			// cout << "FINISHED CREATING MODULE!!!!!" << endl;
			// cout << "sGate>>>>>>> " << sGateBuff->type_ << " Components:" << endl;
			// // Check normal gates are ok en submodule?
			// for (size_t i = 0; i < sGateBuff->logicGates_.size(); ++i)
			// {
			// 	cout << sGateBuff->logicGates_[i]->get_name() << endl;
			// 	for (size_t j = 0; j < sGateBuff->logicGates_[i]->get_inputs().size(); ++j)
			// 	{
			// 		cout << "	Pin: " << sGateBuff->logicGates_[i]->get_inputs()[j]->get_name() << endl; 
			// 		cout << " 	 receives wire: " << sGateBuff->logicGates_[i]->get_inputs()[j]->get_wire()->get_name() << endl;
			// 	}
			// 	cout << "	and has output pin: " << sGateBuff->logicGates_[i]->get_output()->get_name() << endl;
			// 	cout << "	 driving wire: " << sGateBuff->logicGates_[i]->get_output()->get_wire()->get_name() << endl;
			// }
			
			//SuperGate is finished and ready to be stored
			//This sGateBuff will be overwrited later, and hence we need a new sGate to be created, so copy it with the copy constructor
			if (moduleName != topModule_)
			{
				SuperGate *sGateKeep = new SuperGate(*sGateBuff);
				sGatesMap_[moduleName] = sGateKeep;

				// cout<< "Module: " << sGateBuff->type_ << " number of zero/ones: " << sGateBuff->zeroOnes_.size() << endl;
				// cout<< "Module: " << sGatesMap_[moduleName]->type_ << " number of zero/ones: " << sGatesMap_[moduleName]->zeroOnes_.size() << endl;
				

			}
			else
			{//This is the top module, so we save the data into the netslit

				//Save inputs, selecing clk, rst, load and en
				for (size_t i = 0; i < sGateBuff->inputWires_.size(); ++i)
				{
					if (sGateBuff->inputWires_[i]->get_name() == "clk")
					{
						clk_ = sGateBuff->inputWires_[i];
					    // std::cout << "Clock saved: " << clk_->get_name() << "#" << std::endl;
					}else if (sGateBuff->inputWires_[i]->get_name() == "rst")
					{
						rst_ = sGateBuff->inputWires_[i];
					    // std::cout << "Reset saved: " << rst_->get_name() << "#" << std::endl;
					}else if (sGateBuff->inputWires_[i]->get_name() == "load")
					{
						load_ = sGateBuff->inputWires_[i];
					    // std::cout << "Load saved: " << load_->get_name() << "#" << std::endl;
					}else if (sGateBuff->inputWires_[i]->get_name() == "start")
					{
						start_ = sGateBuff->inputWires_[i];
					    // std::cout << "Start saved: " << start_->get_name() << "#" << std::endl;
					}
					else
					{
						ins_.push_back(sGateBuff->inputWires_[i]);
						// cout << sGateBuff->inputWires_[i]->get_name() << endl;
					}

				}

				//Save outputs selecting ready
				for (size_t i = 0; i < sGateBuff->outputWires_.size(); ++i)
				{
					if ((sGateBuff->outputWires_[i]->get_name() == "ready") || (sGateBuff->outputWires_[i]->get_name() == "done"))
					{
						ready_ = sGateBuff->outputWires_[i];
					}
					else
					{
						outs_.push_back(sGateBuff->outputWires_[i]);
						// cout << sGateBuff->outputWires_[i]->get_name() << endl;
					}
				}

				//Save wires
				wires_ = sGateBuff->wires_;
				zeroOnes_= sGateBuff->zeroOnes_;

				//Save maps
				inputsMap_ = sGateBuff->inputsMap_;
				wiresMap_ = sGateBuff->wiresMap_;
				outsMap_ = sGateBuff->outsMap_;

				//Save components
				components_ = sGateBuff->logicGates_;
				superComponents_ = sGateBuff->superGates_;

				//And finally delete the pointer??? Probably ok, same as when deleting it for the Map
			}
				

			// cout << "First input subSuperGate: " << sGateBuff->inputWires_[0]->get_pinTo() << endl;
			// cout << "First input sGateBuff: " << sGateKeep->inputWires_[0]->get_pinTo() << endl;
			// cout << "First input from Map: " << sGatesMap_[moduleName]->inputWires_[0]->get_pinTo() << endl;
			
			
			

		}
		else if(line.find(" input ") != std::string::npos)
		{//Found input declaration


		    //Store input wires
		    get_inputs(sGateBuff, line);

		    //Create Primary input buffers and connect the inputs, but not if we are in the Top module
		    if (moduleName != topModule_)
		    {
		    	for (size_t i = 0; i < sGateBuff->inputWires_.size(); ++i)
			    {
			    	Gate *g = create_primaryInput_buffer(sGateBuff->inputWires_[i]);
			    	//Add it to Primary Inputs
			    	sGateBuff->primaryInputs_.push_back(g);
			    	// cout << "New gate: " << sGateBuff->primaryInputs_[i]->get_name() << "# " << "Input pin: " << 
			    	// g->get_inputs()[0]->get_name() << "# " << "Output pin: " << 
			    	// g->get_output()->get_name() << "# Driving wire: " <<  
			    	// g->get_output()->get_wire()->get_name() << endl;

			    	// cout << "Wire: " << sGateBuff->inputWires_[i]->get_name() << " Droven by pin: " <<
			    	// sGateBuff->inputWires_[i]->get_pinFrom()->get_name() << endl;
			    }
		    }
		}
		else if(line.find(" output ") != std::string::npos)
		{//Found output declaration

		    //Store input wires
		    get_outputs(sGateBuff, line);


		    //Create Primary input buffers and connect the outputs, but not if we are in the Top module
		    if (moduleName != topModule_)
		    {
		    	for (size_t i = 0; i < sGateBuff->outputWires_.size(); ++i)
			    {
			    	Gate *g = create_primaryOutput_buffer(sGateBuff->outputWires_[i]);
			    	//Add it to Primary Outputs
			    	sGateBuff->primaryOutputs_.push_back(g);
			    	// cout << "New gate: " << endl; cout<< sGateBuff->primaryOutputs_[i]->get_name() << "# " << "Input pin: " << 
			    	// g->get_inputs()[0]->get_name() << "# " << "# Receiving wire: " <<  
			    	// g->get_inputs()[0]->get_wire()->get_name() << "Output pin: " << 
			    	// g->get_output()->get_name() <<  endl;

			    	// cout << "Wire: " << sGateBuff->outputWires_[i]->get_name() << " Goign to pin: " <<
			    	// sGateBuff->outputWires_[i]->get_pinTo()->get_name() << endl;
			    }
		    }
		}
		else if(line.find(" wire ") != std::string::npos)
		{//Found wire declaration

		    //Store input wires
		    get_wires(sGateBuff, line);

		}
		else if (sGatesMap_.find(moduleBuff) != sGatesMap_.end())
		{//Found a suprGate declaration

			//Module instantiation found
			SuperGate *subSuperGate;

			//Get the corresponding SuperGate
			if (sGatesMap_.find(moduleBuff) == sGatesMap_.end())	
				std::cout << BOLD(FRED("Trying to access a hierarchy module not saved before")) << std::endl;
			else 
				subSuperGate = sGatesMap_[moduleBuff];

			
			//Get name
			while (isspace(line[yy])) yy++;
		    tt=yy;
		    while (!isspace(line[yy])) yy++;
		    subSuperGate->name_ = line.substr(tt,yy-tt);
		    subSuperGate->type_ = moduleBuff;

		    //Get and connect pins
		    get_superGate_pins(sGateBuff, subSuperGate, line);

		    //Store the subSG in the crrent module
		    sGateBuff->superGates_.push_back(subSuperGate);
		}
		else
		{//Found normal gate declaration
			// cout << "NORMAL MODULE FOUND" << endl;
		    // cout << line << endl;

			GatesFactory moduleFactory = GatesFactory(sGateBuff->inputsMap_, sGateBuff->inputsMap_, sGateBuff->wiresMap_, sGateBuff->outsMap_);
			sGateBuff->logicGates_.push_back(moduleFactory.create_gate(line,sGateBuff->zeroOnes_, sGateBuff->logicGates_));
		}


	}//End while reading file, netlist created

};

bool compare_wires(Wire *wireA, Wire *wireB){
	//Comparator function to return the order of two wires
	int indexA, indexB;
	std::stringstream str1, str2;

	str1 << wireA->get_name().substr(wireA->get_name().find('[')+1,wireA->get_name().find(']')-wireA->get_name().find('['));
	str2 << wireB->get_name().substr(wireB->get_name().find('[')+1,wireB->get_name().find(']')-wireB->get_name().find('['));

	str1 >> indexA;
	str2 >> indexB;

	cout << "#" << indexA << "#" << endl;

	return indexA < indexB;
}


void Netlist::low_endian_ins(wires &vecOfWires){
	//Function to reverse the order of ins outs in Yosys netlist to low endians.

	wires lowEndianVec;
	wires bufferInputs;
	std::vector<wires> allInputsVectors;
	std::vector<string> inputsOrder;

	string inputName;
	//Get name of the first input
	if ((vecOfWires[0]->get_name()).find('[') != std::string::npos)
	{
		//This wire belongs to a bus, get the name without the [N]
		inputName = (vecOfWires[0]->get_name()).substr(0, (vecOfWires[0]->get_name()).find('['));
	}
	else
	{
		inputName = vecOfWires[0]->get_name();
	}
	// cout << inputName << endl;
	bufferInputs.push_back(vecOfWires[0]);

	for(unsigned i = 1; i < vecOfWires.size(); ++i) {
		
		string thisInputName;

		//Get name of this input
		if ((vecOfWires[i]->get_name()).find('[') != std::string::npos)
		{
			//This wire belongs to a bus, get the name without the [N]
			thisInputName = (vecOfWires[i]->get_name()).substr(0, (vecOfWires[i]->get_name()).find('['));
		}
		else
		{
			thisInputName = vecOfWires[i]->get_name();
		}

		cout << inputName << " " << thisInputName << " " << vecOfWires[i]->get_name() << endl;

		//Check if this inputs is of the same bus
		if (thisInputName == inputName)
		{
			//Then keep in the same vector
			bufferInputs.push_back(vecOfWires[i]);
		}
		else
		{
			//It is a different bus/wire
			//Invert the order of the buffer Inputs and put it next into newIns
			if (bufferInputs.size() > 1)
			{
				sort (bufferInputs.begin(), bufferInputs.end(), compare_wires);
				reverse(bufferInputs.begin(),bufferInputs.end());
			}
			allInputsVectors.push_back(bufferInputs);
			inputsOrder.push_back(inputName);

			//Clear buffer Inputs
			bufferInputs.clear();

			//Update new bus wires name, and add the first one
			inputName = thisInputName;
			bufferInputs.push_back(vecOfWires[i]);
		}
	}
	//Last iteration never enters the last else, so we have to repeat at the end of the loop
	//Invert the order of the buffer Inputs and put it next into newIns
	if (bufferInputs.size() > 1)
	{
		sort (bufferInputs.begin(), bufferInputs.end(), compare_wires);
		reverse(bufferInputs.begin(),bufferInputs.end());
	}
	allInputsVectors.push_back(bufferInputs);
	inputsOrder.push_back(inputName);
	//Clear buffer Inputs
	bufferInputs.clear();

	//Now check how ordered are the inputs in the module declaration
	int i = 0;
	int j = 0;
	int cnt = 0;

	inputName = "";

	string inputBuff;
	std::vector<string> orderInputsModule;

	i = topModuleDeclaration_.find('(') + 1;
	
	while((uint)cnt < ins_.size()) {
		while(isspace(topModuleDeclaration_[i])){
			++i;
		}
		j = i;
		while(topModuleDeclaration_[j] != ','){
			++j;
		}

		if (isspace(topModuleDeclaration_[j-1])){inputBuff = topModuleDeclaration_.substr(i,j-i-1);}
		else{inputBuff = topModuleDeclaration_.substr(i,j-i);}

		i = j+1;
		
		if ((inputBuff == "clk") || (inputBuff == "load") || (inputBuff == "rst") || (inputBuff == "start"))
		{
			continue;
		}

		string thisInputName;

		//Similarly as above, this time we just keep the bus name, so that we get in which order are they declared
		if (inputBuff.find('[') != std::string::npos)
		{
			//This wire belongs to a bus, get the name without the [N]
			thisInputName = inputBuff.substr(0, (inputBuff.find('[')));
		}
		else
		{
			thisInputName = inputBuff;
		}

		if ((inputName == "") || (inputName != thisInputName))
		{
			inputName = thisInputName;
			orderInputsModule.push_back(inputName);
			// cout << "wuhuuuuuuu " << inputName << endl;
		}

		cnt++;
		// if (cnt == 202) break;
	}

	// cout << "Order module declared inputs:" << endl;
	// for(unsigned k = 0; k < orderInputsModule.size(); ++k) {
	// 	cout << orderInputsModule[k] << endl;
	// }
	// cout << "Order inputs:" << endl;
	// for(unsigned k = 0; k < inputsOrder.size(); ++k) {
	// 	cout << inputsOrder[k] << endl;
	// }

	//Append in order the corresponding inputs vectors
	for(unsigned k = 0; k < orderInputsModule.size(); ++k) {
		
		lowEndianVec.insert(lowEndianVec.end(), 
							allInputsVectors[find(inputsOrder.begin(), inputsOrder.end(), orderInputsModule[k]) - inputsOrder.begin()].begin(),
							allInputsVectors[find(inputsOrder.begin(), inputsOrder.end(), orderInputsModule[k]) - inputsOrder.begin()].end());
	}

	vecOfWires.clear();
	vecOfWires = lowEndianVec;
};

void Netlist::low_endian_outs(wires &vecOfWires){
	//Function to reverse the order of ins outs in Yosys netlist to low endians.

	wires lowEndianVec;
	wires bufferInputs;

	string inputName;
	//Get name of the first input
	if ((vecOfWires[0]->get_name()).find('[') != std::string::npos)
	{
		//This wire belongs to a bus, get the name without the [N]
		inputName = (vecOfWires[0]->get_name()).substr(0, (vecOfWires[0]->get_name()).find('['));
	}
	else
	{
		inputName = vecOfWires[0]->get_name();
	}
	// cout << inputName << endl;
	bufferInputs.push_back(vecOfWires[0]);

	for(unsigned i = 1; i < vecOfWires.size(); ++i) {
		
		string thisInputName;

		//Get name of this input
		if ((vecOfWires[i]->get_name()).find('[') != std::string::npos)
		{
			//This wire belongs to a bus, get the name without the [N]
			thisInputName = (vecOfWires[i]->get_name()).substr(0, (vecOfWires[i]->get_name()).find('['));
		}
		else
		{
			thisInputName = vecOfWires[i]->get_name();
		}

		cout << inputName << " " << thisInputName << " " << vecOfWires[i]->get_name() << endl;

		//Check if this inputs is of the same bus
		if (thisInputName == inputName)
		{
			//Then keep in the same vector
			bufferInputs.push_back(vecOfWires[i]);
		}
		else
		{
			//It is a different bus/wire
			//Invert the order of the buffer Inputs and put it next into newIns
			if (bufferInputs.size() > 1)
			{
				sort (bufferInputs.begin(), bufferInputs.end(), compare_wires);
				reverse(bufferInputs.begin(),bufferInputs.end());
			}
			lowEndianVec.insert(lowEndianVec.end(), bufferInputs.begin(), bufferInputs.end());

			//Clear buffer Inputs
			bufferInputs.clear();

			//Update new bus wires name, and add the first one
			inputName = thisInputName;
			bufferInputs.push_back(vecOfWires[i]);
		}
	}
	//Last iteration never enters the last else, so we have to repeat at the end of the loop
	//Invert the order of the buffer Inputs and put it next into newIns
	if (bufferInputs.size() > 1)
	{
		sort (bufferInputs.begin(), bufferInputs.end(), compare_wires);
		reverse(bufferInputs.begin(),bufferInputs.end());
	}
	lowEndianVec.insert(lowEndianVec.end(), bufferInputs.begin(), bufferInputs.end());
	//Clear buffer Inputs
	bufferInputs.clear();

	vecOfWires.clear();
	vecOfWires = lowEndianVec;
};
#endif
