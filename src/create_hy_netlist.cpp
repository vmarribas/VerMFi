
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
#include "GatesFactory.h"


void Netlist::create_hy_netlist(std::ifstream &fNetlist, std::vector<std::vector<std::string>> inputs,
                         std::vector<std::string> outputs)
{
#ifndef YOSYS	
	//Synopsys netlist
	//Read first all the different modules in the design
	std::vector<std::string> modules;
	string topModule;


	std::string line;
	std::string buffLine;
	SuperGate *sGateBuff = NULL;
	string moduleName;

	while(getline(fNetlist,line)){

		//Avoid comments and blank lines
		if ((line.length() == 0) || ((line[0] == '/') && (line[1] == '/'))) continue;
        bool skip = true;
        for (int i=0; i<line.length(); i++){
            if (!isspace(line[i])){
                skip = false;
            }
        }
        if (skip) continue;
		//Get first word of the declaration (used later to identify modules)
		int tt = 0;
	    int yy = 0;
	    string moduleBuff;

    	while (isspace(line[tt])) tt++;
	    yy=tt;
	    while (!isspace(line[yy])) yy++;

	    moduleBuff = line.substr(tt,yy-tt);

	    // cout << line << endl;


		if((line.find("endmodule") == std::string::npos) && (line.find("module") != std::string::npos))
		{
			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }

			//New superGate
//            cout << "Now it goes delete previous Supergate" << endl;
//            delete sGateBuff; // I do not want to delete the carefully created objects!!

			//Get module name
			moduleName = get_module_name(line);
			modules.push_back(moduleName);
            // cout << modulmeName << endl;

			//Create new super gate (module)
			sGateBuff = new SuperGate(moduleName);


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
//                SuperGate *sGateKeep = new SuperGate(*sGateBuff);
				sGatesMap_[moduleName] = sGateBuff;

				// cout<< "Module: " << sGateBuff->type_ << " number of zero/ones: " << sGateBuff->zeroOnes_.size() << endl;
				// cout<< "Module: " << sGatesMap_[moduleName]->type_ << " number of zero/ones: " << sGatesMap_[moduleName]->zeroOnes_.size() << endl;
//                cout << "Saved SuperGate" << endl;

			}
			else
			{//This is the top module, so we save the data into the netslit
                //Fisrt save the last sGateBuff so that there is no memory leak. It will be destroyed later with Netlis
                topModuleSG_ = sGateBuff;
                
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
					if ((sGateBuff->outputWires_[i]->get_name() == "ready") || (sGateBuff->outputWires_[i]->get_name() == "done") ||
						(sGateBuff->outputWires_[i]->get_name().find("ready") != std::string::npos) || (sGateBuff->outputWires_[i]->get_name().find("done") != std::string::npos))
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
//            cout << "get_inputs" << endl;

			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }

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
//            cout << "get_outputs" << endl;
			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }

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
//            cout << "get_wires" << endl;
			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }

		    //Store input wires
		    get_wires(sGateBuff, line);

		}
		else if (std::find(modules.begin(), modules.end(), moduleBuff) != modules.end())
		{//Found a suprGate declaration
            // cout << "get_supergate" << endl;
			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }

			//Module instantiation found
            SuperGate *subSuperGate = nullptr;

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
//             cout << "NORMAL MODULE FOUND" << endl;
			//Get the full declaration
			while (line.find(';') == std::string::npos){
			    getline(fNetlist,buffLine);
			    line += buffLine;
		    }
		    // cout << line << endl;

			GatesFactory moduleFactory = GatesFactory(sGateBuff->inputsMap_, sGateBuff->inputsMap_, sGateBuff->wiresMap_, sGateBuff->outsMap_);
			sGateBuff->logicGates_.push_back(moduleFactory.create_gate(line,sGateBuff->zeroOnes_, sGateBuff->logicGates_));
		}
	}//End while reading file, netlist created
	// cout << "Finished reading hy netlist" << endl;

	//Other operations
	rename_sub_super_gates(superComponents_, "");
	// plot_names_sub_gates(superComponents_);
	// for (size_t i = 0; i < components_.size(); ++i)
	// {
	// 	cout << components_[i]->get_name() << endl;
	// }
	//Gather every flip flop ins and outs 
	//First get netlist FF
	for (size_t j = 0; j < components_.size(); ++j)
	{
		if ((components_[j]->gateType_ == Gate::DFF) || (components_[j]->gateType_ == Gate::SDFF))
		{
			outputsFF_.push_back(components_[j]->inputs_[0]->get_wire());
			inputsFF_.push_back(components_[j]->output_->get_wire());
			if (components_[j]->output_->get_wire() == nullptr){
                cout << "Found component "<< components_[j]->get_name() <<" with output wire nullptr" << endl;
            }
            if (components_[j]->inputs_[0]->get_wire() == nullptr){
                cout << "Found component "<< components_[j]->get_name() <<" with input wire nullptr" << endl;
            }
			// cout << "Storing new FF input: " << components_[j]->output_->get_wire()->get_name() << 
			// 		"With fanOuts number: " << 1+components_[j]->output_->get_wire()->get_fanOutPins().size() << endl;
			// for (int k = 0; k < components_[j]->output_->get_wire()->get_fanOutPins().size(); ++k)
			// {
			// 	cout << components_[j]->output_->get_wire()->get_fanOutPins()[k]->get_gate()->get_name() << endl;
			// }

		}
	}
	get_sub_ff_inouts(superComponents_, inputsFF_, outputsFF_);
	//Plot all FF inouts
	// for (size_t i = 0; i < inputsFF_.size(); ++i)
	// {
	// 	cout << inputsFF_[i]->get_name() << endl;
	// }
	// for (size_t i = 0; i < outputsFF_.size(); ++i)
	// {
	// 	cout << outputsFF_[i]->get_name() << endl;
	// }

	//Plot desired datapath for an input:
	// for (int i = 0; i < ins_.size(); ++i)
	// {
	// 	if (ins_[i]->get_name() == "\\s_in[0]")
	// 	{
	// 		plot_data_path(ins_[i]);
	// 	}
		
	// }

#else

	//YOSYS netlist
    // cout << "Hello YOSYS" << endl;
	map<string, bool> modulesParsed; //Name of the module, and if it has been already parsed
	vector< vector<string> > modulesHy; //Keep the netlist hierarchy... Each row: module, submodule1, ..., submN
	vector< vector<string> > modulesChopped; //Keep every module declaration separated

	//Gather all different modules and initialize map
	chop_modules_declaration(fNetlist, modulesParsed, modulesChopped);		
	fNetlist.clear();
	fNetlist.seekg(0, ios::beg);
	//Get submodules of every module
	get_submodules(fNetlist, modulesParsed, modulesHy);

	// for (std::map<string, bool>::iterator i = modulesParsed.begin(); i != modulesParsed.end(); ++i)
	// {
	// 	cout << i->first << " evaluated: " << i->second << endl;
	// }

	// for(unsigned i = 0; i < modulesHy.size(); ++i) {
	// 	for(unsigned j = 0; j < modulesHy[i].size(); ++j) {
	// 		cout << modulesHy[i][j] << ";";
	// 	}
	// 	cout << endl;
	// }
	while(!modulesChopped.empty()) {
		
		//Alsway work on first element of the vecotrs, the erase or rotate
		bool allSubmodsComputed = 1;
		if (modulesHy[0].size() == 1)
		{
			//If there is only one element, that means there is no submods declarations
			allSubmodsComputed = 1;
		}
		else
		{
			//Check if the submods are already parsed
			for(unsigned i = 1; i < modulesHy[0].size(); ++i) {
				
				// allSubmodsComputed &= modulesParsed[modulesHy[0][i]];
				if (!modulesParsed[modulesHy[0][i]])
				{
					allSubmodsComputed = 0;
					break;
				}
			}
		}
		//Finish checking if we should analyze this module
		// cout << "allSubmodsComputed?? " << allSubmodsComputed << endl;

		if (allSubmodsComputed)
		{
			create_yosys_netlist(modulesChopped[0]);
			modulesParsed[modulesHy[0][0]] = 1;
			modulesChopped.erase(modulesChopped.begin());
			modulesHy.erase(modulesHy.begin());
		}
		else
		{
			std::rotate(modulesChopped.begin(), modulesChopped.begin() + 1, modulesChopped.end());
			std::rotate(modulesHy.begin(), modulesHy.begin() + 1, modulesHy.end());
		}

		// for (std::map<string, bool>::iterator i = modulesParsed.begin(); i != modulesParsed.end(); ++i)
		// {
		// 	cout << i->first << " evaluated: " << i->second << endl;
		// }

		// for(unsigned i = 0; i < modulesHy.size(); ++i) {
		// 	for(unsigned j = 0; j < modulesHy[i].size(); ++j) {
		// 		cout << modulesHy[i][j] << ";";
		// 	}
		// 	cout << endl;
		// }
	}
	
		// create_yosys_netlist(modulesChopped);
	// cout << "Finished reading hy netlist" << endl;


	// for(unsigned i = 0; i < modulesChopped.size(); ++i) {
	// 	cout << "      " << modulesChopped[i][0] << endl;
	// 	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	// }
	

	//Other operations
	rename_sub_super_gates(superComponents_, "");
	// plot_names_sub_gates(superComponents_);
	// for (size_t i = 0; i < components_.size(); ++i)
	// {
	// 	cout << components_[i]->get_name() << endl;
	// }
	//Gather every flip flop ins and outs 
	//First get netlist FF
	for (size_t j = 0; j < components_.size(); ++j)
	{
		if ((components_[j]->gateType_ == Gate::DFF) || (components_[j]->gateType_ == Gate::SDFF))
		{
			outputsFF_.push_back(components_[j]->inputs_[0]->get_wire());
			inputsFF_.push_back(components_[j]->output_->get_wire());
			// cout << "Storing new FF input: " << components_[j]->output_->get_wire()->get_name() << 
			// 		"With fanOuts number: " << 1+components_[j]->output_->get_wire()->get_fanOutPins().size() << endl;
			// for (int k = 0; k < components_[j]->output_->get_wire()->get_fanOutPins().size(); ++k)
			// {
			// 	cout << components_[j]->output_->get_wire()->get_fanOutPins()[k]->get_gate()->get_name() << endl;
			// }

		}
	}
	get_sub_ff_inouts(superComponents_, inputsFF_, outputsFF_);

	//Reorder inputs and outputs to small endian
	// cout << "change inputs outputs order" << endl;
	// for(unsigned i = 0; i < ins_.size(); ++i) {
	// 	cout << ins_[i]->get_name() << endl;
	// }
	// for(unsigned i = 0; i < outs_.size(); ++i) {
	// 	cout << outs_[i]->get_name() << endl;
	// }

	low_endian_ins(ins_);
	low_endian_outs(outs_);

	// for(unsigned i = 0; i < ins_.size(); ++i) {
	// 	cout << ins_[i]->get_name() << endl;
	// }
	// for(unsigned i = 0; i < outs_.size(); ++i) {
	// 	cout << outs_[i]->get_name() << endl;
	// }

	//Plot all FF inouts
	// for (size_t i = 0; i < inputsFF_.size(); ++i)
	// {
	// 	cout << inputsFF_[i]->get_name() << endl;
	// }
	// for (size_t i = 0; i < outputsFF_.size(); ++i)
	// {
	// 	cout << outputsFF_[i]->get_name() << endl;
	// }

	//Plot desired datapath for an input:
	// for (int i = 0; i < ins_.size(); ++i)
	// {
	// 	if (ins_[i]->get_name() == "\\n_rounds[0]")
	// 	{
	// 		plot_data_path(ins_[i]);
	// 	}
		
	// }

	//Plot desired datapath for an ooutput:
	// cout << ready_->get_name() << endl;
	// // for (int i = 0; i < ins_.size(); ++i)
	// // {
	// // 	if (ins_[i]->get_name() == "\\n_rounds[0]")
	// // 	{
	// 		plot_data_path_output(ready_);
// 	// 	}
		
// 	// }
#endif
};

Netlist::~Netlist(){
    if (gFactory_ != nullptr) delete gFactory_;
    if (topModuleSG_ != nullptr) delete topModuleSG_;
    for (superGates::iterator it = superComponents_.begin(); it != superComponents_.end(); ++it) {
        if (*it != nullptr) delete *it;
    }
};
