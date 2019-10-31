
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


#include "FaultsMaxine.h"
#include "conversions.h"
#include "Gates.h"
#include <chrono>

using namespace std;
using namespace std::chrono;


void FaultsMaxine::get_checkBits(std::vector<string> checkBits){

	if (checkBits.empty())
	{
		cout << BOLD(FRED("NO CHECK BITS specified")) << endl;
	}
	else
	{
		for (size_t i = 0; i < checkBits.size(); ++i)
		{
			// Check in the outputs
			for (size_t j = 0; j < netList_.outs_.size(); ++j)
			{
				if (netList_.outs_[j]->get_name().find(checkBits[i]) != std::string::npos)
				{
					this->checkBits_.push_back(netList_.outs_[j]);
				}
			}
			// Check in the wires
			for (size_t j = 0; j < netList_.wires_.size(); ++j)
			{
				if (netList_.wires_[j]->get_name().find(checkBits[i]) != std::string::npos)
				{
					this->checkBits_.push_back(netList_.wires_[j]);
				}
			}

			//Continue with registers from sub superGates
			get_sub_checkBits(netList_.superComponents_, checkBits[i]);
		}
	}
};
void FaultsMaxine::get_sub_checkBits(superGates &sGates, string checkBit){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		///For every comp and for every sub superGate
		for (size_t j = 0; j < sGates.size(); ++j)
		{
			for (size_t i = 0; i < sGates[j]->wires_.size(); ++i)
			{
				if (sGates[j]->wires_[i]->get_name().find(checkBit) != std::string::npos)
				{
					this->checkBits_.push_back(sGates[j]->wires_[i]);
				}
			}
			for (size_t i = 0; i < sGates[j]->outputWires_.size(); ++i)
			{
				if (sGates[j]->outputWires_[i]->get_name().find(checkBit) != std::string::npos)
				{
					this->checkBits_.push_back(sGates[j]->outputWires_[i]);
				}
			}
			initialize_sub_registers(sGates[j]->superGates_);
		}
	}
}

void FaultsMaxine::create_random_inputs(){
	
	// Create N_TRACES number of inputs
	for (size_t i = 0; i < numberOfTraces_; ++i)
	{
		string inputVector = "";

		for (size_t j = 0; j < netList_.ins_.size(); ++j)
		{
			if (loadCycles_ > 0)
			{
				for (int k = 0; k < loadCycles_; ++k)
				{
					int r = rand() % 2;
					inputVector += boolToChar((bool)r);
				}
			}
			else
			{
				int r = rand() % 2;
				inputVector += boolToChar((bool)r);
			}

		}

		// cout << "Input test vector: " << inputVector << endl; 
		testVectorsIn_.push_back(inputVector);
	}
}

void FaultsMaxine::read_sim_inputs_file(std::string inputsSimFile){
	std::ifstream fInputs;
	bool hexadecimal = 0;

	//Open file and read input test vector
	fInputs.open(inputsSimFile);
	if (!fInputs.is_open()) {
		std::cout << BOLD(FRED("Could not read inputs file")) << std::endl;
		return ;
	}

	std::string line;
	std::vector<string> lines;

	// Read RESET and LOAD cycles
	while(getline(fInputs,line)){
	  
		if (line.find("N_RESET_CYCLES") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> rstCycles_;

			// cout << rstCycles_  << endl;
		}
		if (line.find("N_LOAD__CYCLES") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> loadCycles_;

			// cout << loadCycles_  << endl;
		}
		if (line.find("N_SHARES") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> nomberOfShares_;

			// cout << nomberOfShares_  << endl;
		}
		if (line.find("N_TRACES") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> numberOfTraces_;

			// cout << numberOfTraces_  << endl;

			break;
		}

	}

	//Decide if the implementation is serial
	if ((loadCycles_ > 1) && (netList_.ready_ != NULL))
	{
		// cout << "Serial implementation" << endl;
		serialImpl_ = 1;
	}else{
		serialImpl_ = 0;
	}

	// Read or provide inputs
	if (numberOfTraces_ == 0)
	{
		// Read inputs provided by the user
		while(getline(fInputs,line)){
	  
			if (line.find("### Inputs") != std::string::npos) break;
		}
		int numVectors = 0;
		while(getline(fInputs,line)){


			if ((numVectors == 0) && (line.find("### End Inputs") != std::string::npos))
			{
				assert(0 && "You must introduce some inputs or have N_TRACES > 0");
			}
			else if ((numVectors > 0) && (line.find("### End Inputs") != std::string::npos))
			{
				break;
			}
			
			std::string buffLine;
			//Get in the line all inputs
		    while (line.find(';') == std::string::npos){
		    	
		      	getline(fInputs,buffLine);
		      	line += buffLine;
		    }
		    lines.push_back(line);
		    // if (line.find('#') != std::string::npos) break;
		    // cout << line << endl;
		    numVectors++;
		}
		fInputs.close();

		assert(lines[0].find(';') != std::string::npos && "I cannot find symbol ; in your inputs file");

		//Get input test vector and number of extra simulation cycles in case in an impl. without ready signal.
		//Check if the inputs are given in binary or hex
		if (lines[0][0] == 'x')
		{
			hexadecimal = 1;
		}
		
		//Get test vector
		int i = lines[0].find(';');
		extraCyclesSim_ = 0;
		std::string testVector;

		if (hexadecimal)
		{
			testVector = lines[0].substr(1,i-1);
		}
		else{
			testVector = lines[0].substr(0,i);
		}
		// cout << "Test vector: " << testVector << endl;
		//Get extra sim cycles
		extraCyclesSim_ = 0;


		// Save test vector as a binary string
		if (hexadecimal)
		{
			testVectorsIn_.push_back(hex_to_bin(testVector));
		}
		else{
			testVectorsIn_.push_back(testVector);
		}

		//Check if the input test vector is longer than the sensitive inputs: in that case, load in several cycles
		if ((netList_.ins_.size()) < testVectorsIn_[0].size())
		{
			assert(((uint)testVectorsIn_[0].size()/(netList_.ins_.size())) == loadCycles_ && 
				"The length of your test vector devided by the number of inputs does not match with the introduced number of leading cycles");
			// std::cout << loadCycles_ << std::endl;
		}

		//Keep getting the next test vectors
		for (size_t k = 1; k < lines.size(); ++k)
		{
			if (hexadecimal)
			{
				// cout << lines[k].substr(1,i-1) << endl;
				testVectorsIn_.push_back(hex_to_bin(lines[k].substr(1,i-1)));
			}
			else{
				testVectorsIn_.push_back(lines[k].substr(0,i));
			}
			// cout << testVectorsIn_[k] << endl;
			// cout << endl;
		}
	}
	else
	{
		// Create random inputs
		create_random_inputs();
	}

};


void FaultsMaxine::create_sub_faults(superGates &sGates){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		//Create a new fault per gate for every sub superGate

		for (size_t k = 0; k < sGates.size(); ++k)
		{
			// cout << "Creating fault for sGate: " << sGates[k]->name_ << endl;
			for (size_t i = 0; i < sGates[k]->logicGates_.size(); ++i){

				Fault *f = new Fault();

				// cout << "Creating fault for gate: " << sGates[k]->logicGates_[i]->get_name() << endl;
				sGates[k]->logicGates_[i]->f_ = f;
				// cout << "done" << endl;
				// cout << endl;
				//INV_Q gates created to handle QN are not in components vector, they also need a fault
				// cout << "Got to FFFFFFFF fault" << endl;
				if ((sGates[k]->logicGates_[i]->get_type() == Gate::DFF) || (sGates[k]->logicGates_[i]->get_type() == Gate::SDFF))
				{
					if (sGates[k]->logicGates_[i]->get_output()->get_wire()->get_is_fanOut())
					{
						for (size_t j = 0; j < sGates[k]->logicGates_[i]->get_output()->get_wire()->get_fanOutPins().size(); ++j)
						{
							if (sGates[k]->logicGates_[i]->get_output()->get_wire()->get_fanOutPins()[j]->get_gate()->get_name() == "INV_Q")
							{
								// cout << "fault for FF!!!!!!" << endl;
								Fault *ff = new Fault();
								sGates[k]->logicGates_[i]->get_output()->get_wire()->get_fanOutPins()[j]->get_gate()->f_ = ff;
								// cout << "DONE" << endl;
							}
						}
					}
				}
			}
			for (size_t j = 0; j < sGates[k]->primaryInputs_.size(); ++j)
			{
				// cout << "Fault primaryInputs_" << endl;
				Fault *f = new Fault();

				sGates[k]->primaryInputs_[j]->f_ = f;
			}
			for (size_t j = 0; j < sGates[k]->primaryOutputs_.size(); ++j)
			{
				// cout << "Fault primaryOutputs_" << endl;
				Fault *f = new Fault();

				sGates[k]->primaryOutputs_[j]->f_ = f;
			}

			create_sub_faults(sGates[k]->superGates_);
		}
	}

};
void FaultsMaxine::create_faults(){

	//Create a new fault per gate
	// cout << "Creating faults" << endl;
	for (size_t i = 0; i < netList_.components_.size(); ++i){

		Fault *f = new Fault();

		netList_.components_[i]->f_ = f;

		//INV_Q gates created to handle QN are not in components vector, they also need a fault
		// cout << "Go to first FF faults" << endl;
		if ((netList_.components_[i]->get_type() == Gate::DFF) || (netList_.components_[i]->get_type() == Gate::SDFF))
		{
			// cout << "FF found" << netList_.components_[i]->get_name() << endl;
			// cout << "with OutputObject @ " << netList_.components_[i]->get_output() << endl;
   //          cout << "with WireObject @ " << netList_.components_[i]->get_output()->get_wire() << endl;

			if (netList_.components_[i]->get_output()->get_wire()->get_is_fanOut())
			{
				// cout << "Check if it has QN" << endl;
				for (size_t j = 0; j < netList_.components_[i]->get_output()->get_wire()->get_fanOutPins().size(); ++j)
				{
					// cout << "Look for QN" << endl;
					if (netList_.components_[i]->get_output()->get_wire()->get_fanOutPins()[j]->get_gate()->get_name() == "INV_Q")
					{
						// cout << "QN found" << endl;
						Fault *ff = new Fault();
						netList_.components_[i]->get_output()->get_wire()->get_fanOutPins()[j]->get_gate()->f_ = ff;
					}
				}
			}
		}
	}

	// cout << "Current block faults done" << endl;
	create_sub_faults(netList_.superComponents_);
	// cout << "Faults created" << endl;
}

void FaultsMaxine::initialize_sub_registers(superGates &sGates){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		///For every comp and for every sub superGate
		for (size_t j = 0; j < sGates.size(); ++j)
		{
			for (size_t i = 0; i < sGates[j]->logicGates_.size(); ++i){

				if (sGates[j]->logicGates_[i]->get_type() == Gate::DFF)
				{
					dynamic_cast<DFFgate*>(sGates[j]->logicGates_[i])->set_Q(0);
				}
				else if (sGates[j]->logicGates_[i]->get_type() == Gate::SDFF)
				{
					dynamic_cast<SDFFgate*>(sGates[j]->logicGates_[i])->set_Q(0);
				}
			}

			initialize_sub_registers(sGates[j]->superGates_);
		}
	}
}
void FaultsMaxine::initialize_registers(){
	//Initialize registers to zero
	// cout << "initialize_registers" << endl;
	for (size_t i = 0; i < netList_.components_.size(); ++i)
	{
		if (netList_.components_[i]->get_type() == Gate::DFF)
		{
			dynamic_cast<DFFgate*>(netList_.components_[i])->set_Q(0);
		}
		else if (netList_.components_[i]->get_type() == Gate::SDFF)
		{
			dynamic_cast<SDFFgate*>(netList_.components_[i])->set_Q(0);
		}
	}
	//Continue with registers from sub superGates
	initialize_sub_registers(netList_.superComponents_);
	// cout << "Regs initialized" << endl;
};	


void FaultsMaxine::update_sub_registers(superGates &sGates){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		//For every comp and for every sub superGate
		for (size_t j = 0; j < sGates.size(); ++j)
		{
			for (size_t i = 0; i < sGates[j]->logicGates_.size(); ++i){

				if (sGates[j]->logicGates_[i]->get_type() == Gate::DFF)
				{
					dynamic_cast<DFFgate*>(sGates[j]->logicGates_[i])->eval_gate();
				}
				else if (sGates[j]->logicGates_[i]->get_type() == Gate::SDFF)
				{
					dynamic_cast<SDFFgate*>(sGates[j]->logicGates_[i])->eval_gate();
				}
			}

			update_sub_registers(sGates[j]->superGates_);
		}
	}

}
void FaultsMaxine::update_registers(){
	//Get value Q of register
	for (size_t i = 0; i < netList_.components_.size(); ++i)
	{
		if (netList_.components_[i]->get_type() == Gate::DFF)
		{
			dynamic_cast<DFFgate*>(netList_.components_[i])->eval_gate();
		}
		else if (netList_.components_[i]->get_type() == Gate::SDFF)
		{
			dynamic_cast<SDFFgate*>(netList_.components_[i])->eval_gate();
		}
	}
	//Continue with registers from sub superGates
	update_sub_registers(netList_.superComponents_);
	// cout << "Regs updated" << endl;
};


void FaultsMaxine::assign_inputs_values(gates& eventList){

    //Assign previously read test vector to inputs
    // cout << "Number of inputs: " << netList_.ins_.size() << " test vector size: " << testVectorsIn_[testVectorSimulating_].size() << " " << testVectorsIn_.size() << endl;

    assert((netList_.ins_.size()) <= testVectorsIn_[testVectorSimulating_].size() && "Not enough input bits given by the test vector (check if 'x' for hexadecimal is included or check if 'rst' and 'clk' signals are specified with these names)") ;
	if (serialImpl_ && (clk_ == 0))
	{
		for(size_t k = 0; k < netList_.ins_.size(); k++){
			// In a serial implementation that has a load signal, the first reset should not feed yet the first chunk of inputs, and should not let testVectorIndex_ advance
	        netList_.ins_[k]->eval_wire(0, eventList);
	    }
	}
	else
	{
		for(size_t k = 0; k < netList_.ins_.size(); k++){
	        netList_.ins_[k]->eval_wire(charToBool(testVectorsIn_[testVectorSimulating_][testVectorIndex_]), eventList);
	        //Stop counting when last input bit has been given. Keep feeding the same bit or reset testVectorIndex_ last loaded in
	        // if (clk_ <= 15) std::cout << netList_.ins_[k]->get_name() << " evals to: " << netList_.ins_[k]->get_value() << std::endl;
	        // if (clk_ <= 15) cout << "testVectorIndex_: " << testVectorIndex_ << endl;
	        if (testVectorIndex_ < (testVectorsIn_[testVectorSimulating_].size()-1)){
	        	testVectorIndex_++;
	        }else
	        {
	        	// cout << testVectorIndex_ << endl;
	        	testVectorIndex_ = testVectorsIn_[testVectorSimulating_].size() - netList_.ins_.size();
	        	// std::cout << "Reseting testVectorIndex_ to: " << testVectorIndex_ << std::endl;
	        }
	    }
	    //Give values to random variables
	    for (size_t k = 0; k < netList_.randomBits_.size(); ++k){
			netList_.randomBits_[k]->eval_wire(1, eventList);
			// std::cout << netList_.randomBits_[k]->get_name() << " evals to: " << netList_.randomBits_[k]->get_value() << std::endl;
		}
	}

};


void FaultsMaxine::assign_control_inputs(bool rst, bool load, bool start, gates& eventList){
	//Give values to rst, load, start if they exist
	// cout << "assign_control_inputs" << endl;
	if (netList_.rst_ != NULL) netList_.rst_->eval_wire(rst, eventList);
	if (netList_.load_ != NULL) netList_.load_->eval_wire(load, eventList);
	if (netList_.start_ != NULL) netList_.start_->eval_wire(start, eventList);

};


void FaultsMaxine::propagate_sub_zero_ones(superGates &sGates, gates &eventList){

	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		//For every comp and for every sub superGate
		for (size_t j = 0; j < sGates.size(); ++j)
		{
			// cout << "Sub module name: " << sGates[j]->type_ << " number of zeros: " << sGates[j]->zeroOnes_.size() << endl;
			for (size_t i = 0; i < sGates[j]->zeroOnes_.size(); ++i)
			{
				// cout << "Wire name: " << sGates[j]->zeroOnes_[i]->get_name() << endl;
				//We need to push it directly, coz no value changed obviously
				eventList.push_back(sGates[j]->zeroOnes_[i]->get_pinTo()->get_gate());
				// cout << "Adding gate to event list: " << sGates[j]->zeroOnes_[i]->get_pinTo()->get_gate()->get_name() << endl;
			}
			propagate_sub_zero_ones(sGates[j]->superGates_, eventList);
		}
	}
}
void FaultsMaxine::propagate_zero_ones(gates &eventList){

	for (size_t i = 0; i < netList_.zeroOnes_.size(); ++i)
	{
		eventList.push_back(netList_.zeroOnes_[i]->get_pinTo()->get_gate());
		// cout << "Adding gate to event list: " << netList_.zeroOnes_[i]->get_pinTo()->get_gate()->get_name() << endl;
	}

	propagate_sub_zero_ones(netList_.superComponents_, eventList);

}

void FaultsMaxine::propagate_regs_newPIs(gates& eventList){


	for (size_t i = 0; i < netList_.inputsFF_.size(); ++i)
	{
		//Set the wire with the value of the Register that is driving it
		assert(((netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF) || (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::SDFF)) && "A new PI was stored that is not comming from a DFF");
		if (netList_.inputsFF_[i]->get_pinFrom()->get_name() == "QN")
		{
			//Check if there is a fault in a register
			if ((netList_.inputsFF_[i]->get_pinFrom()->get_gate()->is_faulty() && (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->f_->cycle_ == int(clk_))) ||
				(netList_.inputsFF_[i]->get_pinFrom()->get_gate()->is_faulty() && (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->f_->cycle_ == -1)))
			{
				// std::cout << "FAULT ACTIVATED!!" << std::endl;
				if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF)
				{
					netList_.inputsFF_[i]->eval_wire((dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
				else if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::SDFF)
				{
					netList_.inputsFF_[i]->eval_wire((dynamic_cast<SDFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
					
			}
			else{
				if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF)
				{
					netList_.inputsFF_[i]->eval_wire(!(dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
				else if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::SDFF)
				{
					netList_.inputsFF_[i]->eval_wire(!(dynamic_cast<SDFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
			}
				
		}else{
			//Check if there is a fault in a register
			if ((netList_.inputsFF_[i]->get_pinFrom()->get_gate()->is_faulty() && (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->f_->cycle_ == int(clk_))) ||
				(netList_.inputsFF_[i]->get_pinFrom()->get_gate()->is_faulty() && (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->f_->cycle_ == -1)))
			{
				// std::cout << "FAULT ACTIVATED!!" << std::endl;
				if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF)
				{
					netList_.inputsFF_[i]->eval_wire(!(dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
				else if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::SDFF)
				{
					netList_.inputsFF_[i]->eval_wire(!(dynamic_cast<SDFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
				}
			}
			else{
				// cout << "Evaluating new FF input wire: " << netList_.inputsFF_[i]->get_name() << " that activates gate " <<endl;
				// cout << netList_.inputsFF_[i]->get_pinTo()->get_gate()->get_name() << endl;
				if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF)
				{
					netList_.inputsFF_[i]->eval_wire(dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q(), eventList);
				}
				else if (netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::SDFF)
				{
					netList_.inputsFF_[i]->eval_wire(dynamic_cast<SDFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q(), eventList);
				}
			}
				
		}
		// std::cout << "Wire: " << netList_.inputsFF_[i]->get_name() << " getting value: " <<
		// 			 netList_.inputsFF_[i]->get_value() << " from reg: " <<  netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_name() << std::endl;
	}

};


void FaultsMaxine::activate_faulty_gates(gates& eventList){


	//Activate also "unfaulted" gates from previous test
	if (clk_ == 0)
	{
		for (size_t i = 0; i < previousFaultyGates_.size(); ++i)
		{
			//Only activate combinational logic, DFFs are already taken care of in the previous function
			if ((previousFaultyGates_[i]->get_type() != Gate::DFF) || (previousFaultyGates_[i]->get_type() != Gate::SDFF))
			{
				previousFaultyGates_[i]->eval_gate(eventList, clk_);
			}
		}
	}

	//Activate faulty gates
	for (size_t i = 0; i < faultyGates_.size(); ++i)
	{
		//Only activate combinational logic, DFFs are already taken care of in the previous function
		if ((faultyGates_[i]->get_type() != Gate::DFF) || (faultyGates_[i]->get_type() != Gate::SDFF))
		{
			faultyGates_[i]->eval_gate(eventList, clk_);
		}
	}
}

static int totalToggles;
void FaultsMaxine::evaluate_event_list(gates& eventList){
	//Evaluate elements until list is finished
	int count = 0;
	while(!eventList.empty())
	{
		if ((eventList[0]->get_type() == Gate::DFF) || (eventList[0]->get_type() == Gate::SDFF))
		{
			//When evaluating a FF, do not propagate, just store the value
			// if((eventList[0]->get_name().find("_2076_") != std::string::npos) || (eventList[0]->get_name().find("_2077_") != std::string::npos) || (eventList[0]->get_name().find("_2078_") != std::string::npos) || (eventList[0]->get_name().find("_2079_") != std::string::npos) || (eventList[0]->get_name().find("_2080_") != std::string::npos)){
				// cout << "FF found of type: " << eventList[0]->get_type() << " and Name: " << eventList[0]->get_name() << endl;
			// }

			eventList[0]->eval_gate();	

			// if((eventList[0]->get_name().find("_2076_") != std::string::npos) || (eventList[0]->get_name().find("_2077_") != std::string::npos) || (eventList[0]->get_name().find("_2078_") != std::string::npos) || (eventList[0]->get_name().find("_2079_") != std::string::npos) || (eventList[0]->get_name().find("_2080_") != std::string::npos)){
				// cout << "Evaluated" << endl;
				// if ((eventList[0]->get_type() == Gate::DFF))
				// {
				// 	std::cout << "evaluating DFF: " << eventList[0]->get_name() << " state value: " << dynamic_cast<DFFgate*>(eventList[0])->get_Q() << std::endl;
				// }
				// else 
				// 	std::cout << "evaluating SDFF: " << eventList[0]->get_name() << " state value: " << dynamic_cast<SDFFgate*>(eventList[0])->get_Q() << std::endl;
				
			// }
		}else{
			if (eventList[0]->get_output()->get_wire() != NULL)
			{
				// if(count < 10650){
				// // if(eventList[0]->get_name().find("_2078") != std::string::npos){
//                    std::cout << "evaluating gate in clk " << clk_ << ": " << eventList[0]->get_name() << endl;
					// std::cout << "    Output wire: " << eventList[0]->get_output()->get_wire()->get_name() << endl;
				// }

				eventList[0]->eval_gate(eventList, clk_);
				// if(!eventList[0]->get_output()->get_wire()->get_is_evaluated()) eventList.push_back(eventList[0]);

				// if(count < 10650){
				// // if(eventList[0]->get_name().find("_2078") != std::string::npos){
//                    std::cout << "    With value: " << eventList[0]->get_output()->get_wire()->get_value() << " " << eventList[0]->get_output()->get_wire()->get_is_evaluated() << std::endl;
					// if (eventList[0]->get_output()->get_wire()->get_pinTo() != NULL)
					// 	std::cout << "  Triggers gate: " << eventList[0]->get_output()->get_wire()->get_pinTo()->get_gate()->get_name() << std::endl;
					// else 
					// 	std::cout << "  This output does not connect to any gate" << endl;
				// }
			}
		}
	    eventList.erase(eventList.begin());
	    eventList.shrink_to_fit();
	    count++; //Plot purpose
	    totalToggles++;
	}
	// cout << "Number of toggles: " << count << endl;

};


void FaultsMaxine::one_cycle_simulation(gates &eventList){

    // std::cout << "NEW CYCLE!!!!!!!!!!!!!!!!!!!!" << std::endl;

	#ifdef VERBOSE
    cout << "CLK = " << clk_ << " >>>>>>>>>>>" << endl;
    #endif
	// Erase values from previous cycle
	// de_evaluate_wires_simul();

	// Get inputs... Order?????
	//Inputs are give every cycle, even if they do not change. We need to trigger their gates anyway
	// cout << "assign_inputs_values" << endl;
	assign_inputs_values(eventList);

	if (clk_ == 0)	
	{
		//Reset state
		assign_control_inputs(1, 0, 0, eventList);
		// cout << "Rst" << endl;
	}else if (clk_ <= loadCycles_){
		//Load state
		assign_control_inputs(0, 1, 0, eventList);
		// cout << "Load" << endl;
	}else{
		//Start state
		assign_control_inputs(0, 0, 1, eventList);
		// cout << "Start" << endl;
	}

	//Evaluate gates with assign zero ones
	// cout << "prop zero ones" << endl;
	propagate_zero_ones(eventList);

	// First evaluate new PI coming from DFFs
	// cout << "propagate_regs_newPIs" << endl;
	propagate_regs_newPIs(eventList);

	// Activate faulty gates. This is needed in case no inputs of the faulty gate change in this cycle (hence never activated)
	// cout << "activate_faulty_gates" << endl;
	activate_faulty_gates(eventList);

	// Continue evaluating the elements in eventList => Simulation!
	// cout << "evaluate_event_list" << endl;
	evaluate_event_list(eventList);

	//Update registers values in case it was not already done when evaluating event_list
	//It might happen that no output (of the sub-circuit) changed and hence no register was activated. They would keep reset value
	// cout << "update_registers" << endl;
	update_registers();
	
    clk_++;


    // Output debugging

    // string outString = "SBox/val_inv/St4_Os1";
    // cout << outString << ": " << bin_to_hex(get_wire_string(outString)) << endl;
    #ifdef VERBOSE
    cout << "Out = " << bin_to_hex(get_unshared_output_string(nomberOfShares_)) << endl;
    cout << "Ready = " << netList_.ready_->get_value() << endl;
    #endif

}

// }
void FaultsMaxine::simulate(){


	gates eventList; //Simulation vector

	//Initialize registers to zero
	initialize_registers();

	// std::cout << testVectorsIn_[testVectorSimulating_] << std::endl;
	//Start simulation and do not stop till ready=1.
	clk_=0; //Cycles count
	totalToggles = 0;

	if (netList_.ready_ != NULL)
	{
		// cout << netList_.ready_->get_value() << endl;
		if (serialImpl_)
		{
			int readySims = loadCycles_;

			serialCheckAccumulate_ = 0;

			//Empty serial output from the previous simulation
			serialOut_ = "";
			assert(readySims > 0 && "You need to specify an input vector longer than the actual module input");
			while(!netList_.ready_->get_value() || (readySims != 0)){ 
				if ((clk_ == totalCyclesEncryption_) || (readySims == 0))
				{
					//Stop simulating if a eternal loop from a fault is found, or if the number of simulations during ready=1 is finished
					// an implementation setting ready=0 when sim is finished would probably continue then simulating since netList_.ready_->get_value() =0
					// cout << FRED("Found eternal loop") << " " << clk_ << " " << totalCyclesEncryption_ << endl;
					break;
				}
				// if (clk_ == 2) break;
				one_cycle_simulation(eventList);

				if (netList_.ready_->get_value())
				{
					//Count down the number of simulations performed during ready = 1
					readySims--;

					//Keep the outputstring.... What if it is shared?
					serialOut_ += get_unshared_output_string(nomberOfShares_);

					//Accumulate possible faulty bytes detected across the N output cycles
					serialCheckAccumulate_ |= check_checkBits(get_unshared_output_string(nomberOfShares_));
				}
			}
			// cout << bin_to_hex(serialOut_) << endl;
		}
		else
		{
			while(!netList_.ready_->get_value()){
				if (clk_ == totalCyclesEncryption_)
				{
					// cout << FRED("Found eternal loop") << " " << clk_ << " " << totalCyclesEncryption_ << endl;
					break;
				}
				// if (clk_ == 2) break;
				one_cycle_simulation(eventList);
			}
		}
	}//Finished simulation when there is ready signal
	else{
		//Simulation when ready is not specified
		// cout << "Simulating cycle" << endl;
		while(clk_ <= (loadCycles_ - 1) + extraCyclesSim_){

			one_cycle_simulation(eventList);

		}
	}
	if (netList_.ready_ != NULL) netList_.ready_->de_evaluate_wire();
    // cout << "Total Number of toggles: " << totalToggles << endl;
};

void FaultsMaxine::get_sub_wire_string(string name, superGates &sGates, string &outBuff){
	//Return string with the value of the wire chosen continue down the hierarchy
	if (sGates.empty() == 1)
	{
		return;
	}
	else
	{
		///For every comp and for every sub superGate
		for (size_t j = 0; j < sGates.size(); ++j)
		{
			for (size_t i = 0; i < sGates[j]->wires_.size(); ++i){

				if ((sGates[j]->wires_[i]->get_name().find(name) != std::string::npos))
		    	{
		    		cout << sGates[j]->wires_[i]->get_name() << ": " << sGates[j]->wires_[i]->get_value() << endl;
		    		outBuff += std::to_string((int)sGates[j]->wires_[i]->get_value());
		    	}
			}
			get_sub_wire_string(name, sGates[j]->superGates_, outBuff);
		}
	}

}
string FaultsMaxine::get_wire_string(string componentName){
	//Return string with the value of the wire chosen
	string outBuff = "";

	for (size_t i = 0; i < netList_.wires_.size(); ++i)
    {
    	//Maybe when getting the outputs in create_hy_netlist I should just leave out also the check bits? I already know them from the beginning
    	if ((netList_.wires_[i]->get_name().find(componentName) != std::string::npos))
    	{
    		cout << netList_.wires_[i]->get_name() << endl;
    		outBuff += std::to_string((int)netList_.wires_[i]->get_value());
    	}
    }

    get_sub_wire_string(componentName, netList_.superComponents_, outBuff);

    return outBuff;
};

string FaultsMaxine::get_output_string(){
	//Return string with exclusively output values, no READY or CHECKBITS
	string outBuff = "";

	for (size_t i = 0; i < netList_.outs_.size(); ++i)
    {
    	//Maybe when getting the outputs in create_hy_netlist I should just leave out also the check bits? I already know them from the beginning
    	if ((netList_.outs_[i]->get_name() != "ready") && (netList_.outs_[i]->get_name() != "done") && ((std::find(checkBits_.begin(), checkBits_.end(), netList_.outs_[i]) == checkBits_.end())))
    	{
    		outBuff += std::to_string((int)netList_.outs_[i]->get_value());
    	}
    }
    return outBuff;
};

string FaultsMaxine::get_unshared_output_string(int shares){
	//Return string with exclusively output values, no READY or CHECKBITS
	string outBuff = "";
	std::vector<string> outShares;

	for (size_t i = 0; i < netList_.outs_.size(); ++i)
    {
    	//Maybe when getting the outputs in create_hy_netlist I should just leave out also the check bits? I already know them from the beginning
    	if ((netList_.outs_[i]->get_name() != "ready") && (netList_.outs_[i]->get_name() != "done") && ((std::find(checkBits_.begin(), checkBits_.end(), netList_.outs_[i]) == checkBits_.end())))
    	{
    		outBuff += std::to_string((int)netList_.outs_[i]->get_value());
    	}
    }

    //Split string in shares of the same length
    assert((outBuff.size() % shares) == 0 && "Your output string is not divisible in equal length strings for each share. Check that your signal ready is properly named and that you specified the check bits");
    size_t shareLength = outBuff.size()/shares;

    for (size_t i = 0; i < outBuff.length(); i += shareLength) 
    {
    	outShares.push_back(outBuff.substr(i, shareLength));
	}


	//XOR them together
	string unsharedOut = "";

	for (size_t i = 0; i < shareLength; ++i)
	{
		string bit = "0";
		for (int j = 0; j < shares; ++j)
		{
			bit = std::to_string((int)(charToBool(bit[0]) ^ charToBool(outShares[j][i])));
		}
		unsharedOut += bit;
	}
    return unsharedOut;
};

bool FaultsMaxine::check_checkBits(string output){

	bool faultDetected = 0;
	// cout << "clasify_fault" << endl;
	//Was the fault detected by any of the bits??
	if (!checkBits_.empty())
	{
		//Case when there is check/status bits
		for (size_t i = 0; i < checkBits_.size(); ++i)
		{
			faultDetected |= checkBits_[i]->get_value();
			cout << checkBits_[i]->get_name() << ": " << checkBits_[i]->get_value() << endl;
		}
		// cout << endl;
	}else{
		//Case when there is not: only all zeros allowed
		for (size_t i = 0; i < output.size(); ++i)
		{
			if (output[i] != '0') break;
			//If output bits are all zero then the fault was detected
			if (i == (output.size()-1)) faultDetected = 1;
		}

	}

	return faultDetected;
}

int FaultsMaxine::clasify_fault(string output){

	bool faultDetected = 0;

	if (serialImpl_)
	{
		faultDetected = serialCheckAccumulate_;
	}
	else
		faultDetected = check_checkBits(output);
	
	if (faultDetected)
	{
		faultsDetected_ += 1;
		totalFaultsDetected_ += 1;
		// cout << "Fault Detected!" << endl;
		return 2;
	}
	else if ((!faultDetected) && (output == refOutput_))
	{
		faultsIneffective_ += 1;
		totalFaultsIneffective_ += 1;
		// cout << "Fault Ineffective!" << endl;
		return 1;
	}
	else{
		faultsNotDetected_ += 1;
		totalFaultsNotDetected_ += 1;
		// cout << "Fault Not Detected!" << endl;
		return 0;
	}

};

// //Function to let the user introduce by keeboard the faults.... NOT GOOOD!!!
// void FaultsMaxine::fault_sub_injection(string compName, superGates &sGates, bool continueDown, superGates &sGatesToFault){

// 	int selection;
// 	bool actuallyFault = 1;
// 	bool contHyDown = 0;
// 	bool moreComponents = 0;

// 	if ((sGates.empty() == 1) || (!continueDown))
// 	{
// 		return;
// 	}
// 	else
// 	{
// 		cout << "Components in: " << compName << endl;
// 		for (size_t i = 0; i < sGates.size(); ++i)
// 		{
// 			cout << sGates[i]->name_ << " (" << i << ")" << endl;
// 		}
		
// 		while(1){
// 			cout << "Introduce index" << endl;
// 			cin >> selection;
// 			if (selection < sGates.size())
// 			{
// 				cout << "Do you actually want to fault this block or just continue further in the hierarchy (not faulting it)? Fault(1), Do NOT Fault(0)" <<endl;
// 				cin >> actuallyFault;
// 				if (actuallyFault)
// 				{
// 					sGatesToFault.push_back(sGates[selection]);
// 				}
// 				cout << "Do you want to fault more components inside this block? Yes(1), No(0)" <<endl;
// 				cin >> contHyDown;
// 				fault_sub_injection(sGates[selection]->name_, sGates[selection]->superGates_, contHyDown, sGatesToFault);
// 			}
// 			else if (selection >= sGates.size())
// 			{
// 				cout << "No component found with this index" << endl;
// 			}

// 			cout << "Do you want to fault more blocks in \"" << compName <<  "\"? Yes(1), No(0)" << endl;
// 			cin >> moreComponents;
// 			if (!moreComponents) break;
// 		}
// 	}
// }
void FaultsMaxine::hierarchy_sub_fault_injection(std::ifstream& fConfig, superGates &sGates, faults &gatesToFault){
	//Continue parsing file config_faults...

	if ((sGates.empty() == 1))
	{
		return;
	}
	else
	{
		//New superGate found check what to do
		// --hyLevels;
		// cout << "hyLevels: " << hyLevels << endl;

		for (size_t i = 0; i < sGates.size(); ++i)
		{	
			//Get current super Gate name
			bool faultComponent = 0;
			bool faultPIPO = 0;
			int cycleToFaultComp = 0;
			FaultType::FType faultType = FaultType::SEU;

			stringstream str;
			string line, superComponent;
			
			// getline(fConfig,line);
			// while (line.length() == 0){
			// 	getline(fConfig,line);
			// }
			
			if (line.length() == 0) getline(fConfig,line);
			int j = (int)line.find('*')+2;
			int k = j+1;
			while (line[k] != ' ') k++;

			superComponent = line.substr(j,k-j);

			// cout << superComponent << " " << sGates[i]->name_ << " " << (superComponent == sGates[i]->name_) << endl;
			assert((superComponent == sGates[i]->name_) && "Printed netlist does not follow same pattern as the modeled one in the program... Did you modify it?");
			

			//Parse if to fault and when
			j = (int)line.find("F?:")+5;
			if (line.substr(j,1) == "1")
			{
				faultComponent = 1;
			}
			j = (int)line.find("PI/O?:")+8;
			if (line.substr(j,1) == "1")
			{
				faultPIPO = 1;
			}
			j = (int)line.find("When?:")+8;
			k = j;
			while (line[k] != '`') k++;
			if (line[j] == '<')
			{
				str << line.substr(j+1,k-j-1);
				str >> cycleToFaultComp;

				cycleToFaultComp = -(totalCyclesEncryption_ - cycleToFaultComp);
				// cycleToFaultComp = rand() % cycleToFaultComp;
				// cout << "Smaller: " << cycleToFaultComp << endl;
			}
			else if (line[j] == '>')
			{
				str << line.substr(j+1,k-j-1);
				str >> cycleToFaultComp;

				cycleToFaultComp = totalCyclesEncryption_ + cycleToFaultComp;
				// cycleToFaultComp = (totalCyclesEncryption_-1) - (rand() % ((totalCyclesEncryption_-1)-cycleToFaultComp));
				// cout << "Greater: " << cycleToFaultComp << endl;
			}else if (line[j] == '-')
			{
				cycleToFaultComp = 1234567890;
			}
			else
			{
				str << line.substr(j,k-i);
				str >> cycleToFaultComp;
			}
			// cout << faultComponent << " " << faultPIPO << " " << cycleToFaultComp << " #" << line.substr(j,k-j) << "#" << endl;

			//Type of fault
			int m = (int)line.find("Type?:")+8;
			if (line.substr(m,3) == "SEU")
			{
				faultType = FaultType::SEU;
			}else if (line.substr(m,3) == "SA0")
			{
				faultType = FaultType::SA0;
			}else if (line.substr(m,3) == "SA1")
			{
				faultType = FaultType::SA1;
			}
			// cout << faultType << endl;

			//Create all possible faults to inject in this module
			if (faultComponent && faultPIPO)
			{
				//Fault only PI/POs from this component
				for (size_t l = 0; l < sGates[i]->primaryInputs_.size(); ++l)
				{
					//Create the fault and add it to the list of possible faults
					gatesToFault.push_back(make_tuple(sGates[i]->primaryInputs_[l], cycleToFaultComp, faultType));
					// cout << sGates[i]->primaryInputs_[l]->get_name() << endl;
				}

				for (size_t l = 0; l < sGates[i]->logicGates_.size(); ++l)
				{
					//Create the fault and add it to the list of possible faults
					gatesToFault.push_back(make_tuple(sGates[i]->logicGates_[l], cycleToFaultComp, faultType));
					// cout << sGates[i]->logicGates_[l]->get_name() << endl;
				}
			}
			else if (faultComponent && !faultPIPO)
			{
				for (size_t l = 0; l < sGates[i]->logicGates_.size(); ++l)
				{
					//Create the fault and add it to the list of possible faults
					gatesToFault.push_back(make_tuple(sGates[i]->logicGates_[l], cycleToFaultComp, faultType));
				}

			}
			//Continue with the rest
			hierarchy_sub_fault_injection(fConfig, sGates[i]->superGates_, gatesToFault);

		}
	}
}
bool FaultsMaxine::hierarchy_fault_injection(std::string faultsConfigFile, faults &componentsToFault, int &N_SIMS, int &N_FAULTS, int &PER_CYCLE){
	//Parse file config_faults....

	std::ifstream fConfig;
	string line;

	//Open file and gather the allowed faults to inject
	fConfig.open(faultsConfigFile);
	if (!fConfig.is_open()) {
		std::cout << BOLD(FRED("Could not read faults config file")) << std::endl;
		std::cout << "in hierachy read" << std::endl;
		std::cout << "file name: " << faultsConfigFile << std::endl;
		return 0;
	}

	bool faultTopModule = 0;
	int cycleToFaultTopMod = 0;
	FaultType::FType faultType = FaultType::SEU;


	while(getline(fConfig,line)){

		//Avoid comments and blank lines
		if (((line[0] == '/') && (line[1] == '/')) || (line.length() == 0)) continue;

		if (line.find("N_SIMULS") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> N_SIMS;

			// cout << N_SIMS  << endl;
		}
		if (line.find("N_FAULTS") != std::string::npos)
		{
			//Get number of faults
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> N_FAULTS;

			// cout << N_FAULTS << endl;
		}
		if (line.find("PER_CYCLE") != std::string::npos)
		{
			//Get number of faults
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> PER_CYCLE;

			// cout << PER_CYCLE << endl;
		}

		if (line.find(netList_.topModule_) != std::string::npos)
		{
			//Found where netlist plot starts

			//Fist check if top module has to be faulted
			stringstream str;
			int i = (int)line.find("F?:")+5;
			if (line.substr(i,1) == "1")
			{
				faultTopModule = 1;
			}
			// cout << faultTopModule << endl;
			i = (int)line.find("When?:")+8;
			int j = i;
			while (line[j] != '`') j++;
			if (line[i] == '<')
			{
				str << line.substr(i+1,j-i-1);
				str >> cycleToFaultTopMod;

				cycleToFaultTopMod = -(totalCyclesEncryption_ - cycleToFaultTopMod);
				// cycleToFaultTopMod = rand() % cycleToFaultTopMod;
			}
			else if (line[i] == '>')
			{
				str << line.substr(i+1,j-i-1);
				str >> cycleToFaultTopMod;

				cycleToFaultTopMod = totalCyclesEncryption_ + cycleToFaultTopMod;
				// cycleToFaultTopMod = totalCyclesEncryption_ - (rand() % (totalCyclesEncryption_-cycleToFaultTopMod));
			}else
			{
				str << line.substr(i,j-i);
				str >> cycleToFaultTopMod;
			}
			// cout << cycleToFaultTopMod << " " <<  line.substr(i,j-i) << endl;
			//Type of fault
			i = (int)line.find("Type?:")+8;
			if (line.substr(i,3) == "SEU")
			{
				faultType = FaultType::SEU;
			}else if (line.substr(i,3) == "SA0")
			{
				faultType = FaultType::SA0;
			}else if (line.substr(i,3) == "SA1")
			{
				faultType = FaultType::SA1;
			}
			// cout << faultType << endl;

			if (faultTopModule)
			{
				for (size_t l = 0; l < netList_.components_.size(); ++l)
				{
					componentsToFault.push_back(make_tuple(netList_.components_[l], cycleToFaultTopMod, faultType));
				}
			}

			// Then continue faulting rest of the subComponents
			hierarchy_sub_fault_injection(fConfig, netList_.superComponents_, componentsToFault);
			break;
		}

	}//Finished reading the file

	return 1;
}

bool FaultsMaxine::components_sub_fault_injection(std::ifstream& fConfig, superGates &sGates, faults &gatesToFault){
	//Continue parsin file config_components....

	// if ((sGates.empty() == 1) || (i == 0))
	if ((sGates.empty() == 1))
	{
		// file << endl;
		return 1;
	}
	else
	{
		// --i;
		// spaces += "  ";
		for (size_t j = 0; j < sGates.size(); ++j)
		{	
			string line;
			//Line per superGate
			getline(fConfig,line);
			while (line.length() == 0) getline(fConfig,line);;

			for (size_t k = 0; k < sGates[j]->logicGates_.size(); ++k)
			{
				string gate;
				//Line per gate
				getline(fConfig,line);
				while (line.length() == 0) getline(fConfig,line);
				int l = (int)line.find('$')+2;
				int m = l+1;
				while (line[m] != ' ') m++;

				gate = line.substr(l,m-l);

				// cout << gate << " " << sGates[j]->logicGates_[k]->get_name() << " " << (gate == sGates[j]->logicGates_[k]->get_name()) << endl;
				assert((gate == sGates[j]->logicGates_[k]->get_name()) && "Printed netlist does not follow same pattern as the modeled one in the program... Did you modify it?");

				bool faultComponent = 0;
				int cycleToFaultComp = 0;
				FaultType::FType faultType = FaultType::SEU;
				std::stringstream str;

				l = (int)line.find("F?:")+5;
				if (line.substr(l,1) == "1")
				{
					faultComponent = 1;
				}
				l = (int)line.find("When?:")+8;
				m = l;
				while (line[m] != '`') m++;
				if (line[l] == '<')
				{
					str << line.substr(l+1,m-l-1);
					str >> cycleToFaultComp;

					cycleToFaultComp = rand() % cycleToFaultComp;
					// cout << "Smaller: " << cycleToFaultComp << endl;
				}
				else if (line[l] == '>')
				{
					str << line.substr(l+1,m-l-1);
					str >> cycleToFaultComp;

					cycleToFaultComp = totalCyclesEncryption_ - (rand() % (totalCyclesEncryption_-cycleToFaultComp));
					// cout << "Greater: " << cycleToFaultComp << endl;
				}else
				{
					str << line.substr(l,m-l);
					str >> cycleToFaultComp;
				}
				// cout << faultComponent << " " << cycleToFaultComp << " #" << line.substr(l,m-l) << "#" << endl;
				//Type of fault
				l = (int)line.find("Type?:")+8;
				if (line.substr(l,3) == "SEU")
				{
					faultType = FaultType::SEU;
				}else if (line.substr(l,3) == "SA0")
				{
					faultType = FaultType::SA0;
				}else if (line.substr(l,3) == "SA1")
				{
					faultType = FaultType::SA1;
				}

				//Create all possible faults to inject in this module
				if (faultComponent)
				{
					//Create the fault and add it to the list of possible faults
					gatesToFault.push_back(make_tuple(sGates[j]->logicGates_[k], cycleToFaultComp, faultType));
				}
			}

			//Continue with the rest
			components_sub_fault_injection(fConfig, sGates[j]->superGates_, gatesToFault);

			
		}
	}
	return 1;

};
bool FaultsMaxine::components_fault_injection(std::string faultsConfigFile, faults &componentsToFault, int &N_SIMS, int &N_FAULTS, int &PER_CYCLE){
	//Parse file config_components...

	std::ifstream fConfig;
	string line;

	//Open file and gather the allowed faults to inject
	fConfig.open(faultsConfigFile);
	if (!fConfig.is_open()) {
		std::cout << BOLD(FRED("Could not read faults config file")) << std::endl;
		std::cout << "in components read" << std::endl;
		return 0;
	}


	while(getline(fConfig,line)){

		//Avoid comments and blank lines
		if (((line[0] == '/') && (line[1] == '/')) || (line.length() == 0)) continue;

		if (line.find("N_SIMULS") != std::string::npos)
		{	
			//Get number of simulations
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> N_SIMS;

			// cout << N_SIMS  << endl;
		}
		if (line.find("N_FAULTS") != std::string::npos)
		{
			//Get number of faults
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> N_FAULTS;

			// cout << N_FAULTS << endl;
		}
		if (line.find("PER_CYCLE") != std::string::npos)
		{
			//Get number of faults
			std::stringstream str;

			int i = (int)line.find("=")+3;
			int j = i;
			while (line[j] != '`') j++;
			str << line.substr(i,j-i);
			str >> PER_CYCLE;

			// cout << PER_CYCLE << endl;
		}

		if (line.find(netList_.topModule_) != std::string::npos)
		{
			//Found where netlist plot starts

			for (size_t k = 0; k < netList_.components_.size(); ++k)
			{
				getline(fConfig,line);
				while (line.length() == 0) getline(fConfig,line);

				// cout << line<< endl;

				bool faultGate= 0;
				int cycleToFaultGate = 0;
				FaultType::FType faultType = FaultType::SEU;

				stringstream str;
				//Fault yes or no
				int i = (int)line.find("F?:")+5;
				if (line.substr(i,1) == "1")
				{
					faultGate = 1;
				}
				// cout << faultTopModule << endl;
				//When to fault
				i = (int)line.find("When?:")+8;
				int j = i;
				while (line[j] != '`') j++;
				if (line[i] == '<')
				{
					str << line.substr(i+1,j-i-1);
					str >> cycleToFaultGate;

					cycleToFaultGate = rand() % cycleToFaultGate;
				}
				else if (line[i] == '>')
				{
					str << line.substr(i+1,j-i-1);
					str >> cycleToFaultGate;

					cycleToFaultGate = (totalCyclesEncryption_-1) - (rand() % ((totalCyclesEncryption_-1)-cycleToFaultGate));
				}else
				{
					str << line.substr(i,j-i);
					str >> cycleToFaultGate;
				}
				//Type of fault
				i = (int)line.find("Type?:")+8;
				if (line.substr(i,3) == "SEU")
				{
					faultType = FaultType::SEU;
				}else if (line.substr(i,3) == "SA0")
				{
					faultType = FaultType::SA0;
				}else if (line.substr(i,3) == "SA1")
				{
					faultType = FaultType::SA1;
				}

				if (faultGate)
				{
					componentsToFault.push_back(make_tuple(netList_.components_[k], cycleToFaultGate, faultType));
				}

			}
			// cout << cycleToFaultTopMod << " " <<  line.substr(i,j-i) << endl;

			// Then continue faulting rest of the subComponents
			components_sub_fault_injection(fConfig, netList_.superComponents_, componentsToFault);
			break;
		}
	}

	return 1;
};

void FaultsMaxine::hierarchy_fault_sims(const int N_SIMS, const int N_FAULTS, const int PER_CYCLE, const faults componentsToFault){
	//Create all fault simulations for a hierarchy fault injection

	//Matrix to keep faults injected already
	std::vector<std::vector<int> > injectedFaults;

//    cout << "componentsToFault: " << componentsToFault.size() << endl;

	if (N_SIMS > 0)
	{
		assert(componentsToFault.size() > 0 && "No logic gates were selected to fault");
	}

    //Analysis for a certain number of simulations
	for (int i = 0; i < N_SIMS; ++i)
	{
		faults faultsVec;
		std::vector<int> faultIdent; //index_f1 -clk_f1, index_f2 -clk_f2, ..., index_fn -clk_fn (negative clk values to make sure they do not collide with indexes)
		//Vector to keep number of injections in this cycle
		std::vector<int> cyclesInjections(totalCyclesEncryption_,0);

		for (int j = 0; j < N_FAULTS; ++j)
		{   
            int gateFault = 0;
            int clkForRange = 0;
            int clkFault = 0;
            int secLoop2 = 0;
            bool injectionIsOk = 0;
            fault faultt;
            while (1){
            	// Loop forever until you find the right place to fault
            	int secLoop1 = 0; // To give some time to look for a valid cycle to inject
            	// Get a random fault
            	gateFault = rand() % componentsToFault.size();
            	// cout << "Received from the parsing for injection: " << get<1>(componentsToFault[gateFault]) << endl;
            	// cout << "Total encryptions: " << totalCyclesEncryption_ << endl;
            	// cout << "Compare?: " << ((get<1>(componentsToFault[gateFault])) > totalCyclesEncryption_) << endl;

            	if ((get<1>(componentsToFault[gateFault])) == 1234567890)
            	{
            		// This means fault will be injected at every cycle
            		bool areAllCyclesAvailable = 1;
            		for (int l = 0; l < totalCyclesEncryption_; ++l) {
	                    areAllCyclesAvailable = cyclesInjections[l] < PER_CYCLE;

	                    if (!areAllCyclesAvailable) break;
	                }

	                if (areAllCyclesAvailable)
	                {
	                	clkFault = -1;
	                	injectionIsOk = 1;

	                }
            	}
            	else if ((get<1>(componentsToFault[gateFault])) > totalCyclesEncryption_)
            	{
            		// Upper range fault
            		clkForRange = get<1>(componentsToFault[gateFault]) - totalCyclesEncryption_; // This is the cycle from which to inject faults
					clkFault = (totalCyclesEncryption_-1) - (rand() % ((totalCyclesEncryption_-1)-clkForRange));

					while(cyclesInjections[clkFault] == PER_CYCLE) {
						//If for the chosen cycle we already have the max injections, change it. Change until we find one cycle that has not max of injections allowed.
						clkFault = (totalCyclesEncryption_-1) - (rand() % ((totalCyclesEncryption_-1)-clkForRange));
						secLoop1++;
						if (secLoop1 > 1000) break;
					}
					if (secLoop1 <= 1000) injectionIsOk = 1; // The previous loop did not need to be forcefully stopped, which means a correct cycle was found

					// cout << "Uper range cycle: " << clkForRange << ", chosen cycle: " << clkFault << ", " << injectionIsOk << endl;
            	}
            	else if (get<1>(componentsToFault[gateFault]) < 0)
            	{
            		// Lower range fault
            		// cout << "Lower range" << endl;
            		clkForRange = totalCyclesEncryption_ + get<1>(componentsToFault[gateFault]); // This is the cycle until which to inject faults
					clkFault = rand() % clkForRange;

					while(cyclesInjections[clkFault] == PER_CYCLE) {
						//If for the chosen cycle we already have the max injections, change it. Change until we find one cycle that has not max of injections allowed.
						clkFault = rand() % clkForRange;
						secLoop1++;
						if (secLoop1 > 1000) break;
					}
					if (secLoop1 <= 1000) injectionIsOk = 1; // The previous loop did not need to be forcefully stopped, which means a correct cycle was found
            	}
            	else if (get<1>(componentsToFault[gateFault]) == totalCyclesEncryption_)
            	{
            		// Random fault on whichever cycle of the entire computation
            		// cout << "All random range" << endl;
            		clkForRange = totalCyclesEncryption_ - 1;
					clkFault = rand() % clkForRange;

					while(cyclesInjections[clkFault] == PER_CYCLE) {
						//If for the chosen cycle we already have the max injections, change it. Change until we find one cycle that has not max of injections allowed.
						clkFault = rand() % clkForRange;
						secLoop1++;
						if (secLoop1 > 1000) break;
					}
					if (secLoop1 <= 1000) injectionIsOk = 1; // The previous loop did not need to be forcefully stopped, which means a correct cycle was found
            	}
            	else
            	{
            		// Exact cycle fault
            		clkFault = get<1>(componentsToFault[gateFault]);
            		// Check if we can still inject in this clk cycle
            		if (cyclesInjections[clkFault] < PER_CYCLE)
            		{
            			injectionIsOk = 1;
            		}
            	}

            	secLoop2++;
            	// Run this infinite for up to 10000 iterations, if 1000 is reached that means no faults can be injected anymore
            	assert(secLoop2 < 10000 && "All clock cycles have already reached the maximum number of allowed faults. Unable to finish the fault injection, modify it");
            	if (!injectionIsOk) continue; // If we could not find an element/cycle for injection, try another element

            	break;

            }
            // cout << "Infinite while finished" <<endl;

            // Assign the decided cycle for the fault
            faultt = componentsToFault[gateFault];
            get<1>(faultt) = clkFault;

            // cout << "Gate: " << get<0>(faultt)->get_name() << " will be faulted in clk: " << get<1>(faultt) << endl;

            //Include fault in the faults vector, and cycle in the injection
			faultsVec.push_back(faultt);
            if (clkFault == -1) {
                for (int l = 0; l < totalCyclesEncryption_; ++l) {
                    cyclesInjections[l] += 1;
                }
            }
            else
            	cyclesInjections[get<1>(faultt)] += 1;

			//Build identifier
			faultIdent.push_back(gateFault);
			if (get<1>(faultt) != -1)
			{
				//If the clk is a positive integer, the identifier just needs it negative
				// cout << "Cycle to fault = " << get<1>(componentsToFault[gateFault]) << endl;
				faultIdent.push_back(-get<1>(faultt));
			}
			else
				faultIdent.push_back(get<1>(faultt));

		}
       	// cout << "Finished gathering faults for this simul" <<endl;

		//Sort the identifier to make sure all identifiers look the same no matter when certain pair was chosen
		sort(faultIdent.begin(), faultIdent.end());

		//Check if this fault injection is present already
		bool injection_repeated = 0;
		for (size_t j = 0; j < injectedFaults.size(); ++j)
		{
			if (equal(faultIdent.begin(), faultIdent.end(), injectedFaults[j].begin()))
			{
				injection_repeated = 1;
				break;
			}
		}
       // cout << "Finished checking if fault repeated, injection repeated? " << injection_repeated <<endl;
       // cout << "Faults vec size: " << faultsVec.size() << endl;

		//If this injection is not repeated, add it to the simulations list and put it as already injected
		if (!injection_repeated)
		{
			faultSimulations_.push_back(faultsVec);

			injectedFaults.push_back(faultIdent);

			// cout << "Fault simulation added succesfully" << endl;;
		}
			
       // cout << "Fault makeident.: ";
		// for (int j = 0; j < faultIdent.size(); ++j)
		// {
		// 	cout << faultIdent[j] << " ";
		// }
		// cout << "Repeated?: " << injection_repeated << endl;
	}// End N_SIMS loop
};


void FaultsMaxine::comb(uint n, int r, faults &faultsVec, const faults componentsToFault) {
    
    for (int i = n; i >= r; i --) {
        // choose the first element
        int clkFault = rand() % totalCyclesEncryption_;
		// int clkFault = -1;

        fault faultt = componentsToFault[i-1];
		//Check if we should assign this fault a random clk cycle to be injected
		if (get<1>(faultt) == -2)	
		{
			get<1>(faultt) = clkFault;
			// cout << "GAte: " << get<0>(componentsToFault[gateFault])->get_name() << " will be faulted in clk: " << get<1>(componentsToFault[gateFault]) << endl;
		}

        faultsVec[r - 1] = faultt;

        if (r > 1) 
        { // if still needs to choose
            // recursive into smaller problem
            comb(i - 1, r - 1, faultsVec, componentsToFault);
        } 
        else 
        {
            // print out one solution
            faultSimulations_.push_back(faultsVec);
        }
    }
}
void FaultsMaxine::components_fault_sims(const int N_SIMS, const int N_FAULTS, const int PER_CYCLE, const faults componentsToFault){

	cout << "componentsToFault: " << componentsToFault.size() << " " << N_FAULTS << endl;

    double possibleCombinations = 1;

    if (componentsToFault.size() != (uint)N_FAULTS)
    {
    	for (int i = 0; i < N_FAULTS; ++i)
	    {
	    	possibleCombinations *= ((double)(componentsToFault.size() - i)/(double)(i + 1));

	    	if (possibleCombinations > 1000000)
	    	{
	    		cout << BOLD(FRED("The number of possible combinations of faults is too high, proceed with random simulation")) << endl;
	    		break;
	    	}
	    }
    }

	if (N_SIMS > 0)
	{
		assert(componentsToFault.size() > 0 && "No logic gates were selected to fault");
	}
	assert(N_FAULTS > 0 && "Number of faults to inject should be greater than 0");

    //Analysis for a certain number of simulations
	if (possibleCombinations <= 1000000)
	{
		if (N_FAULTS == 1)
		{
			for (size_t i = 0; i < componentsToFault.size(); ++i)
			{
				faults faultss;
				// cout <<"Hello " << componentsToFault[i].first->get_name()  << endl;
				//Get a clk cylce at random
				int clkFault = rand() % totalCyclesEncryption_;
				// int clkFault = -1;

				fault faultt = componentsToFault[i];
				//Check if we should assign this fault a random clk cycle to be injected
				if (get<1>(faultt) == -2)	
				{
					get<1>(faultt) = clkFault;
					// cout << "GAte: " << get<0>(componentsToFault[gateFault])->get_name() << " will be faulted in clk: " << get<1>(componentsToFault[gateFault]) << endl;
				}
				faultss.push_back(faultt);

				faultSimulations_.push_back(faultss);
			}
		}
		else if (componentsToFault.size() == (uint)N_FAULTS)
		{
			faults faultss;

			for (size_t i = 0; i < componentsToFault.size(); ++i)
			{
				// cout <<"Hello " << componentsToFault[i].first->get_name()  << endl;
				//Get a clk cylce at random
				int clkFault = rand() % totalCyclesEncryption_;
				// int clkFault = -1;

				fault faultt = componentsToFault[i];
				//Check if we should assign this fault a random clk cycle to be injected
				if (get<1>(faultt) == -2)	
				{
					get<1>(faultt) = clkFault;
					// cout << "GAte: " << get<0>(componentsToFault[gateFault])->get_name() << " will be faulted in clk: " << get<1>(componentsToFault[gateFault]) << endl;
				}
				faultss.push_back(faultt);
			}
			faultSimulations_.push_back(faultss);
		}
		else{
			faults faultsVec(N_FAULTS); //Hold one injection

			// cout << "Comb!!!!" << endl;
			comb((uint)componentsToFault.size(), N_FAULTS, faultsVec, componentsToFault);
		}
	}
	else
	{
		for (int i = 0; i < N_SIMS; ++i)
		{
			//Matrix to keep faults injected already
			std::vector<std::vector<int> > injectedFaults;

			faults faultss;
			std::vector<int> faultIdent; //index_f1 -clk_f1, index_f2 -clk_f2, ..., index_fn -clk_fn (negative clk values to make sure they do not collide with indexes)

			for (int j = 0; j < N_FAULTS; ++j)
			{
				//Get a clk cylce at random
				int clkFault = rand() % totalCyclesEncryption_;
				// int clkFault = -1;

				//Get one of the available faults at random
				int gateFault = rand() % componentsToFault.size();

				fault faultt = componentsToFault[gateFault];
				//Check if we should assign this fault a random clk cycle to be injected
				if (get<1>(faultt) == -2)	
				{
					get<1>(faultt) = clkFault;
					// cout << "GAte: " << get<0>(componentsToFault[gateFault])->get_name() << " will be faulted in clk: " << get<1>(componentsToFault[gateFault]) << endl;
				}

				//Include par in the injection if it is not included
				if (find(faultss.begin(), faultss.end(), faultt) == faultss.end())
				{
					faultss.push_back(faultt);

					//Build identifier
					faultIdent.push_back(gateFault);
					if (get<1>(faultt) != -1)
					{
						//If the clk is a positive integer, the identifier just needs it negative
						// cout << "Cycle to fault = " << get<1>(componentsToFault[gateFault]) << endl;
						faultIdent.push_back(-get<1>(faultt));
					}
					else
						faultIdent.push_back(get<1>(faultt));
				}

			}
			// cout << "Finished gathering faults for this simul" <<endl;

			//Sort the identifier to make sure all identifiers look the same no matter when certain pair was chosen
			sort(faultIdent.begin(), faultIdent.end());

			//Check if this fault injection is present already
			bool injection_repeated = 0;
			for (size_t j = 0; j < injectedFaults.size(); ++j)
			{
				if (equal(faultIdent.begin(), faultIdent.end(), injectedFaults[j].begin()))
				{
					injection_repeated = 1;
					break;
				}
			}
			// cout << "Finished checking if fault repeated" <<endl;

			//If this injection is not repeated, add it to the simulations list and put it as already injected
			//Only add this injection if it has the exact numbe of faults that are supposed to be injected
			if ((!injection_repeated) && (faultIdent.size() == uint(N_FAULTS*2)))
			{
				faultSimulations_.push_back(faultss);

				injectedFaults.push_back(faultIdent);
			}
				
			// cout << "Fault ident.: ";
			// for (int j = 0; j < faultIdent.size(); ++j)
			// {
			// 	cout << faultIdent[j] << " ";
			// }
			// cout << "Repeated?: " << injection_repeated << endl;
		}
	}
	// cout << "Fault simulations: " << faultSimulations_.size() << endl;
};

bool FaultsMaxine::fault_injection(std::string faultsConfigFile){
//Perform the fault injection, decide which faults will be tested
  
	int N_SIMS = 1;
	int N_FAULTS = 1;
	int PER_CYCLE = 1;

	bool componentWiseFaultInj = 0;

	faults componentsToFault;
    cout << faultsConfigFile << endl;

	if (faultsConfigFile.find("config_components") != std::string::npos)
	{
		if (!components_fault_injection(faultsConfigFile, componentsToFault, N_SIMS, N_FAULTS, PER_CYCLE))
		{
			return 0;
		}
		componentWiseFaultInj = 1;
	}else
	{
		if (!hierarchy_fault_injection(faultsConfigFile, componentsToFault, N_SIMS, N_FAULTS, PER_CYCLE))
		{
			return 0;
		}
	}
	cout << "Finished reading injection file" << endl;

	//Proceed with the creation of the different injections
	if (!componentWiseFaultInj)
	{
		hierarchy_fault_sims(N_SIMS, N_FAULTS, PER_CYCLE, componentsToFault);
	}
	else
	{
		components_fault_sims(N_SIMS, N_FAULTS, PER_CYCLE, componentsToFault);
	}
	

	//See faults that will be evaluated
	// cout << "Number of fault simulations: " << endl;
	// cout << faultSimulations_.size() << endl;
	// // cout << faultSimulations_[0].size() << endl;
	// for (size_t i = 0; i < faultSimulations_.size(); ++i)
	// {
	// 	for (size_t j = 0; j < faultSimulations_[i].size(); ++j)
	// 	{
	// 		cout << get<0>(faultSimulations_[i][j])->get_name() << " " << get<1>(faultSimulations_[i][j]) << " " << get<2>(faultSimulations_[i][j]) << ", ";
	// 	}
	// 	cout << endl;
	// }

	faults_file_ << "Evaluation parameters summary: " << endl;
	faults_file_ << "N_SIMULS  =" << N_SIMS << endl; 
	faults_file_ << "N_FAULTS  =" << N_FAULTS << endl;
	faults_file_ << "PER_CYCLE =" << PER_CYCLE << endl; 
	
	return 1;
};


void FaultsMaxine::go(std::string inputsSimFile, std::string faultsConfigFile){

	faults_file_ << "> File generated with VerFI, from https://github.com/vmarribas/VerMFi" << endl;
	faults_file_ << endl;

	faults_file_ << "# FAULTS EVALUATION SUMMARY" << endl;

	float coverage = 0;
	float totalCoverage = 0;

	//Initialize total errors
	totalFaultsDetected_ = 0;
	totalFaultsIneffective_ = 0;
	totalFaultsNotDetected_ = 0;

	//Fault files
	std::ofstream ineffectiveFaultsf;
	ineffectiveFaultsf.open("./faults/ineffective_faults_"+netList_.topModule_+".md");

	std::ofstream nonDetectedFaultsf;
	nonDetectedFaultsf.open("./faults/nonDetected_faults_"+netList_.topModule_+".md");

	nonDetectedFaultsf << "> File generated with VerFI, from https://github.com/vmarribas/VerMFi" << endl;
	nonDetectedFaultsf << endl;

	ineffectiveFaultsf << "> File generated with VerFI, from https://github.com/vmarribas/VerMFi" << endl;
	ineffectiveFaultsf << endl;

	
	nonDetectedFaultsf << "## NON DETECTED FAULTS" << endl;
	ineffectiveFaultsf << "## INEFFECTIVE FAULTS" << endl;


	//Read inputs file
	read_sim_inputs_file(inputsSimFile);
	cout << "Inputs file read finished" << endl;
	//Create faults
	create_faults();
	cout << "Finished creating faults" << endl;

	//Inject faults (perform before one first simulation to get total exec cycles)
	testVectorSimulating_ = 0;
	testVectorIndex_ = 0;
	totalCyclesEncryption_ = -1;
	simulate();
	// if (netList_.ready_ != NULL) netList_.ready_->de_evaluate_wire(); //Thus next simul can start
	// cout << "Reference output: " << bin_to_hex(get_unshared_output_string(nomberOfShares_)) << endl;
	totalCyclesEncryption_ = clk_;
	cout << "Total cycles: " << totalCyclesEncryption_ << endl;
	// cout << "Hello" << endl;
	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //TIME

	if (!fault_injection(faultsConfigFile))
	{
		cout << "Failed fault injection" << endl;
		return;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now(); //TIME
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout << "Done fault injection. Time: " << duration << endl;

	//Perform same experiment for every test vector in the inputs file
	t1 = high_resolution_clock::now(); //TIME
	for (size_t j = 0; j < testVectorsIn_.size(); ++j)
	{
		//Keep the current test vector
		testVectorSimulating_ = (uint)j;
		faults_file_<< "### " << "For Input: 0x" << bin_to_hex(testVectorsIn_[testVectorSimulating_]) << endl;
		nonDetectedFaultsf<< "### " << "For Input: 0x" << bin_to_hex(testVectorsIn_[testVectorSimulating_]) << endl;
		ineffectiveFaultsf<< "### " << "For Input: 0x" << bin_to_hex(testVectorsIn_[testVectorSimulating_]) << endl;

		//Non-faulty simulation to get correct output
		cout << "Non-faulty simulation" << endl;
		testVectorIndex_ = 0;
		simulate();
		if (serialImpl_)
			refOutput_ = serialOut_;
		else
			refOutput_ = get_unshared_output_string(nomberOfShares_);	
		nonDetectedFaultsf<< "##### Reference output: 0x" << bin_to_hex(refOutput_) << endl;
		nonDetectedFaultsf<< endl;
		nonDetectedFaultsf<< "---" << endl;
		ineffectiveFaultsf<< "##### Reference output: 0x" << bin_to_hex(refOutput_) << endl;
		ineffectiveFaultsf<< endl;
		ineffectiveFaultsf<< "---" << endl;

		cout << "Reference output: " << bin_to_hex(refOutput_) << endl;


	    //Initialize errors
	    faultsDetected_ = 0;
		faultsIneffective_ = 0;
		faultsNotDetected_ = 0;

		// cout << "Simulations total: " << faultSimulations_.size() << " first sumulation faults: " << faultSimulations_[0].size() << endl;	
		//Inject errors
		for (size_t i = 0; i < faultSimulations_.size(); ++i)
		{
			for (size_t k = 0; k < faultSimulations_[i].size(); ++k)
			{
				//Inject fault
				get<0>(faultSimulations_[i][k])->f_->set_fault(1,get<1>(faultSimulations_[i][k]));
				get<0>(faultSimulations_[i][k])->f_->set_fault_type(get<2>(faultSimulations_[i][k]));
				faultyGates_.push_back(get<0>(faultSimulations_[i][k]));
				#ifdef VERBOSE
				cout << "<<<<<<<<<Faulting gate: " << get<0>(faultSimulations_[i][k])->get_name() << endl;
				cout << " with output " << get<0>(faultSimulations_[i][k])->get_output()->get_wire()->get_name() << " In cycle: " << get<1>(faultSimulations_[i][k])
					 << ">>>>>>>>>" << endl;
				#endif
				// cout << "<<<<<<<<<Faulting gate: " << get<0>(faultSimulations_[i][k])->get_name() << endl;
				// cout << " with output " << get<0>(faultSimulations_[i][k])->get_output()->get_wire()->get_name() << " In cycle: " << get<1>(faultSimulations_[i][k])
				// 	 << ">>>>>>>>>" << endl;
			}

			//Simulate
			testVectorIndex_ = 0;
			simulate();
			// if (netList_.ready_ != NULL) netList_.ready_->de_evaluate_wire(); //Thus next simul can start
			// cout << bin_to_hex(get_unshared_output_string(nomberOfShares_) << endl;
			string faultyOutput = "";
			if (serialImpl_)
				faultyOutput = serialOut_;
			else
				faultyOutput = get_unshared_output_string(nomberOfShares_);

			//Classify fault
			int faultResult = clasify_fault(faultyOutput);
			if (faultResult == 0)
			{
				//Injection not detected
				// nonDetectedFaultsf << "===============================================" << endl;
				nonDetectedFaultsf << "FAULT(S) INJECTION NOT DETECTED" << endl;
				nonDetectedFaultsf << "```Verilog"  << endl;
				for (size_t k = 0; k < faultSimulations_[i].size(); ++k)
				{
					if (get<0>(faultSimulations_[i][k])->get_output()->get_wire() != NULL)
					{
						nonDetectedFaultsf << "Gate: " << get<0>(faultSimulations_[i][k])->get_name() 
									 << "  Output: " << get<0>(faultSimulations_[i][k])->get_output()->get_wire()->get_name()
									 << "  Cycle: " << get<1>(faultSimulations_[i][k]) << endl;
					}else
					{
						nonDetectedFaultsf << "Gate: " << get<0>(faultSimulations_[i][k])->get_name() 
									 << "  Cycle: " << get<1>(faultSimulations_[i][k]) << endl;
					}	
				}
				nonDetectedFaultsf << "```"  << endl;
				nonDetectedFaultsf << "##### With output: 0x" << bin_to_hex(faultyOutput) << endl;
				nonDetectedFaultsf << "---" << endl;

			}else if (faultResult == 1)
			{
				//Injection ineffective
				// ineffectiveFaultsf << "===============================================" << endl;
				ineffectiveFaultsf << "INEFFECTIVE FAULT(S) INJECTION" << endl;
				ineffectiveFaultsf << "```Verilog"  << endl;
				for (size_t k = 0; k < faultSimulations_[i].size(); ++k)
				{
					if (get<0>(faultSimulations_[i][k])->get_output()->get_wire() != NULL)
					{
						ineffectiveFaultsf << "Gate: " << get<0>(faultSimulations_[i][k])->get_name() 
									 << "  Output: " << get<0>(faultSimulations_[i][k])->get_output()->get_wire()->get_name()
									 << "  Cycle: " << get<1>(faultSimulations_[i][k]) << endl;
					}else
					{
						ineffectiveFaultsf << "Gate: " << get<0>(faultSimulations_[i][k])->get_name() 
									 << "  Cycle: " << get<1>(faultSimulations_[i][k]) << endl;
					}
					
				}
				ineffectiveFaultsf << "```"  << endl;
				ineffectiveFaultsf << endl;
				ineffectiveFaultsf << "---" << endl;

			}
			
			//Unset fault
			for (size_t k = 0; k < faultSimulations_[i].size(); ++k)
			{
				get<0>(faultSimulations_[i][k])->f_->set_fault(0,-1);
				// cout << "Unsetting fault" << endl;
			}
			previousFaultyGates_.clear();
			previousFaultyGates_ = faultyGates_;
			faultyGates_.clear();
			// cout << "Finished with this fault" << endl;
			// cout << ". ";
				
		}
		// cout << "Finished with this input" << endl;

		//Calculate faults coverage
		coverage = 1 - ((float)faultsNotDetected_/(float)(faultsNotDetected_ + faultsDetected_));

		faults_file_ << "CONCLUSION: " << endl;
		faults_file_ << endl;
		faults_file_ << "| Detected | Ineffective | Not Detected |" << endl;
		faults_file_ << "| :----: | :----: | :----: |" <<  endl;
		faults_file_ << "| "<< faultsDetected_ << " | " << faultsIneffective_ << " | " << faultsNotDetected_ << " |" << endl;
		faults_file_ << "###### Coverage = " << coverage << endl;
		faults_file_ << "---" << endl;

		// cout << "_" << endl;
	}
	t2 = high_resolution_clock::now(); //TIME
	auto duration2 = duration_cast<microseconds>( t2 - t1 ).count();
	cout << "Fault simulation done. Time: " << duration2 << endl;


	faults_file_ << "### OVERALL RESULTS" << endl;

	//Total coverage over all inputs
	totalCoverage = 1 - ((float)totalFaultsNotDetected_ / (float)(totalFaultsNotDetected_ + totalFaultsDetected_));

	faults_file_ << "| Total Detected | Total Ineffective | Total Not Detected |" << endl;
	faults_file_ << "| :----: | :----: | :----: |" <<  endl;
	faults_file_ << "| "<< totalFaultsDetected_ << " | " << totalFaultsIneffective_ << " | " << totalFaultsNotDetected_ << " |" << endl;
	faults_file_ << "##### Total Coverage = " << totalCoverage << endl;
	faults_file_ << endl;

	nonDetectedFaultsf.close();
	ineffectiveFaultsf.close();

	//Put everything together
	ifstream data1;
	data1.open("./faults/nonDetected_faults_"+netList_.topModule_+".md");
	string line;
	while(getline(data1, line)){
		faults_file_ << line << endl;
	}
	data1.close();
	// ifstream data2;
	// data2.open("./faults/ineffective_faults_"+netList_.topModule_+".md");
	// string line2;
	// while(getline(data2, line2)){
	// 	faults_file_ << line2 << endl;
	// }
	// data2.close();

	faults_file_.close();
	
	std::cout << BOLD(FGRN("Fault simulations finished!. Check file \"./faults/evaluation_faults_"+netList_.topModule_+".md \" to see the results")) << std::endl;


};


void Fault::set_fault(bool activateFault, int cycle){
	this->active_ = activateFault;
	this->cycle_ = cycle;
};

void Fault::set_fault_type(FaultType::FType type){
	this->fType_ = type;
};
