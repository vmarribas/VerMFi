#include "FaultsMaxine.h"
#include "Gates.h"

using namespace std;

void FaultsMaxine::start_simulation_nofile(){

	//Make sure all signals of the framework have been initialized
	assert(netList_.ready_ != NULL  &&  "\"ready\" signal not correctly declared");
	assert(netList_.clk_ != NULL  &&  "\"clk\" signal not correctly declared");
	assert(netList_.rst_ != NULL  &&  "\"rst\" signal not correctly declared");
	assert(netList_.load_ != NULL  &&  "\"load\" signal not correctly declared");
	assert(netList_.start_ != NULL  &&  "\"start\" signal not correctly declared");


	// int variables = netList_.sharedVariables_.size();
 //    int shares = netList_.sharedVariables_[0].size();

	gates eventList; //Simulation vector

	//Initialize registers to zero
	for (size_t i = 0; i < netList_.components_.size(); ++i)
	{
		if (netList_.components_[i]->get_type() == Gate::DFF)
		{
			dynamic_cast<DFFgate*>(netList_.components_[i])->set_Q(0);

		}
	}

	//Start simulation (should we use start signal?) and do not stop till ready=1
	int cnt=0;
	while(!netList_.ready_->get_value()){

		std::cout << "NEW CYCLE!!!!!!!!!!!!!!!!!!!!" << std::endl;
		//Erase values from previous cycle
		// de_evaluate_wires_simul();
		//Get inputs... Order?????
		for(size_t k = 0; k < netList_.sharedVariables_.size(); k++){
	        for(size_t t = 0; t < netList_.sharedVariables_[0].size(); t++){ 
	            // std::cout << "What is going: " << testVector[l] << " From index " << l << std::endl;
	            netList_.sharedVariables_[k][t]->eval_wire(1, eventList);
	            // std::cout << netList_.sharedVariables_[k][j]->name << " => " << netList_.sharedVariables_[k][j]->value << std::endl;
	        }
	    }

		if (cnt < 4)	
		{
			//Reset state
			netList_.load_->eval_wire(0, eventList);
			netList_.start_->eval_wire(0, eventList);
			netList_.rst_->eval_wire(1, eventList);
		}else if (cnt < 8){
			//Load state
			netList_.load_->eval_wire(1, eventList);
			netList_.start_->eval_wire(0, eventList);
			netList_.rst_->eval_wire(0, eventList);
		}else{
			//Start state
			netList_.load_->eval_wire(0, eventList);
			netList_.start_->eval_wire(1, eventList);
			netList_.rst_->eval_wire(0, eventList);
		}
		//First evaluate new PI coming from DFFs
		for (size_t i = 0; i < netList_.inputsFF_.size(); ++i)
		{
			//Set the wire with the value of the Register that is driving it
			assert(netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_type() == Gate::DFF && "A new PI was stored that is not comming from a DFF");
			if (netList_.inputsFF_[i]->get_pinFrom()->get_name() == "QN")
			{
				netList_.inputsFF_[i]->eval_wire(!(dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q()), eventList);
			}else{
				netList_.inputsFF_[i]->eval_wire(dynamic_cast<DFFgate*>(netList_.inputsFF_[i]->get_pinFrom()->get_gate())->get_Q(), eventList);
			}
			// std::cout << "Wire: " << netList_.inputsFF_[i]->get_name() << " getting value: " <<
						 // netList_.inputsFF_[i]->get_value() << " from reg: " <<  netList_.inputsFF_[i]->get_pinFrom()->get_gate()->get_name() << std::endl;
		}

		//Continue evaluating the elements in eventList => Simulation!
		
		while(!eventList.empty())
        {
        	if (eventList[0]->get_type() == Gate::DFF)
        	{
        		//When evaluating a FF, do not propagate, just store the value
        		// std::cout << "evaluating DFF, state value: " << dynamic_cast<DFFgate*>(eventList[0])->get_Q() << std::endl;
        		eventList[0]->eval_gate();
        		
        	}else{
        		// std::cout << "evaluating gate: " << eventList[0]->get_name();
        		// std::cout << "  Output wire: " << eventList[0]->get_output()->get_wire()->get_name();
        		eventList[0]->eval_gate(eventList);
        		if(!eventList[0]->get_output()->get_wire()->get_is_evaluated()) eventList.push_back(eventList[0]);
        		// std::cout << " With value: " << eventList[0]->get_output()->get_wire()->get_value() << eventList[0]->get_output()->get_wire()->get_is_evaluated() << std::endl;
        	}

            eventList.erase(eventList.begin());
            eventList.shrink_to_fit();

        }

        cnt++;
        
        for (size_t i = 0; i < netList_.wires_.size(); ++i)
	    {
	    	if (netList_.wires_[i]->get_name().find("round_reg") != std::string::npos)
	    	{
	    		std::cout << netList_.wires_[i]->get_value();
	    	}
	    }
	    std::cout<< std::endl;

        for (size_t i = 0; i < netList_.outs_.size(); ++i)
        {
        	std::cout << netList_.outs_[i]->get_value();
        }
        std::cout << std::endl;

        // if (cnt == 19) break;

	}
}