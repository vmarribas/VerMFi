
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


#include "GatesFactory.h"


Gate* GatesFactory::create_gate(std::string compDeclaration, wires &zeroOnes, gates &components){

	Gate *g;

	//Two types of compDeclaration: assign or regular gates

	if (compDeclaration.find("assign") != std::string::npos)
	{
		// std::cout << "Getting in assign decl." << std::endl;
		//When assign compDeclaration
		g = new BUFFgate("BUFF");

		//Create input and output pins
		Pin *p_in= new Pin("I");
		p_in->append_gate(g);	
		Pin *p_out = new Pin("O");
		p_out->append_gate(g);		

		//Append pins to gate
		g->append_input_pin(p_in);
		g->append_output_pin(p_out);

		//Connect pins to wires and viceversa
		int tt = 0;
	    int yy = 0;
	    std::string wire;

	    tt = (int)compDeclaration.find("assign");
	    tt += 7;
	    while (compDeclaration[yy] != '=') yy++;
	    if (compDeclaration[yy-2] == ' '){wire = compDeclaration.substr(tt,yy-tt-2);}
	    else{wire = compDeclaration.substr(tt,yy-tt-1);}
	    // std::cout << "Wire1: " << wire << "#" << std::endl;
	    //Save output wire
		if (outsMap_.find(wire) != outsMap_.end())
		{
			// std::cout << "Save out output" << std::endl;
			p_out->append_wire(outsMap_[wire]);
			outsMap_[wire]->append_pin_from(p_out);
		}else if (wiresMap_.find(wire) != wiresMap_.end())
		{
			// std::cout << "Save out wire" << std::endl;
			p_out->append_wire(wiresMap_[wire]);
			wiresMap_[wire]->append_pin_from(p_out); 

		}

		tt = yy + 2;
		while (compDeclaration[yy] != ';') yy++;
		if (compDeclaration[yy-1] == ' '){wire = compDeclaration.substr(tt,yy-tt-1);}
	    else{wire = compDeclaration.substr(tt,yy-tt);}
		// std::cout << "Wire2: " << wire << "#" << std::endl;
	    //Save input wire
		if (outsMap_.find(wire) != outsMap_.end())
		{
			// std::cout << "Save in output" << std::endl;
			p_in->append_wire(outsMap_[wire]);
			//Check if the wire is already connected to a pin_to. If that is the case, start a fanaout
			if (outsMap_[wire]->get_pinTo() == NULL)
    		{
    			outsMap_[wire]->append_pin_to(p_in);
    		}else{
    			//fanOut_ found
    			outsMap_[wire]->append_fanOut_pin(p_in);
    		}
		}else if (inputsMap_.find(wire) != inputsMap_.end())
		{
			// std::cout << "Save in input" << std::endl;
			p_in->append_wire(inputsMap_[wire]);
			if (inputsMap_[wire]->get_pinTo() == NULL)
    		{
    			inputsMap_[wire]->append_pin_to(p_in);
    		}else{
    			//fanOut_ found
    			inputsMap_[wire]->append_fanOut_pin(p_in);
    		}
		}else if (wiresMap_.find(wire) != wiresMap_.end())
		{
			// std::cout << "Save in wire" << std::endl;
			p_in->append_wire(wiresMap_[wire]);
			if (wiresMap_[wire]->get_pinTo() == NULL)
    		{
    			wiresMap_[wire]->append_pin_to(p_in);
    		}else{
    			//fanOut_ found
    			wiresMap_[wire]->append_fanOut_pin(p_in);
    		}

		}else if (randomsMap_.find(wire) != randomsMap_.end())
		{
			// std::cout << "Save in rand" << std::endl;
			p_in->append_wire(randomsMap_[wire]);
    		if (randomsMap_[wire]->get_pinTo() == NULL)
    		{
    			randomsMap_[wire]->append_pin_to(p_in);
    		}else{
    			//fanOut_ found
    			randomsMap_[wire]->append_fanOut_pin(p_in);
    		}
		}else{
			if ((wire == "1'b0") || (wire == "1'h0"))
			{
				Wire *w = new Wire("zero");
				w->set_value(0);
				w->append_pin_to(p_in);

				p_in->append_wire(w);
				zeroOnes.push_back(w);
			}else if ((wire == "1'b1") || (wire == "1'h1"))
			{
				Wire *w = new Wire("one");
				w->set_value(1);
				w->append_pin_to(p_in);

				p_in->append_wire(w);
				zeroOnes.push_back(w);
			}			
		}
	}//Finished "assign" statements
	else if(compDeclaration.find("HA_X") != std::string::npos){
		//Half adder gate: two outputs => two gates
		//Get gate's name and type
		// std::cout << BOLD(FRED("ATTENTION!! Half adder found!!!")) << std::endl;
		std::string gateType, gateName;
		Gate* gAux;

	    int tt = 0;
	    int yy = 0;
	    while (compDeclaration[tt] == ' ') tt++;
	    yy=tt;
	    while (compDeclaration[yy] != ' ') yy++;
	    gateType = compDeclaration.substr(tt,yy-tt);
	    // std::cout << "Gate type: " << gateType << std::endl;
	    while (compDeclaration[yy] == ' ') yy++;
	    tt=yy;
	    while (compDeclaration[yy] != ' ') yy++;
	    gateName = compDeclaration.substr(tt,yy-tt);
	    // std::cout << "Gate naem::::::::: " << gateName << std::endl;

	    //Create both gates
	    g = new ANDgate(gateName+"_HA_AND");
	    gAux = new XORgate(gateName+"_HA_XOR");

	    //HA_X1 \add_83/U1_1_1  ( .A(round[1]), .B(round[0]), .CO(\add_83/carry[2] ), .S(N11) );

	    //Get pins
	    while (1){

	    	std::string wire;

	    	while (compDeclaration[tt] != '.'){
	    		tt++;
	    		if (compDeclaration[tt] == ';') break;
	    	}
	    	yy = tt;
	    	if (compDeclaration[tt] == ';') break; 
	    	while (compDeclaration[yy] != '(') yy++;

	    	//Save pin
	    	Pin *p = new Pin(compDeclaration.substr(tt+1,yy-tt-1));
	    	// std::cout << "Pin name: " << p->get_name() << std::endl;
	    	//Save wire and gate's pin
	    	tt=yy + 1;
	    	while (compDeclaration[tt] == ' ') tt++;
		    while (compDeclaration[yy] != ')') yy++;
		    while (compDeclaration[yy-1] == ' ') yy--;
	    	wire = compDeclaration.substr(tt,yy-tt);
		    // std::cout << "Wire: " << wire << "#"<< std::endl;
		    if (p->get_name() == "CO")
		    {
		    	p->append_gate(g);
		    	if (outsMap_.find(wire) != outsMap_.end())
		    	{
		    		// std::cout << "Save out output" << std::endl;
		    		p->append_wire(outsMap_[wire]);
		    		outsMap_[wire]->append_pin_from(p);
		    	}else if (wiresMap_.find(wire) != wiresMap_.end())
		    	{
		    		// std::cout << "Save out wire" << std::endl;
		    		p->append_wire(wiresMap_[wire]);
		    		wiresMap_[wire]->append_pin_from(p); 

		    	}
		    	g->append_output_pin(p);
		    }else if (p->get_name() == "S")
		    {
		    	p->append_gate(gAux);
		    	if (outsMap_.find(wire) != outsMap_.end())
		    	{
		    		// std::cout << "Save out output" << std::endl;
		    		p->append_wire(outsMap_[wire]);
		    		outsMap_[wire]->append_pin_from(p);
		    	}else if (wiresMap_.find(wire) != wiresMap_.end())
		    	{
		    		// std::cout << "Save out wire" << std::endl;
		    		p->append_wire(wiresMap_[wire]);
		    		wiresMap_[wire]->append_pin_from(p); 

		    	}
		    	gAux->append_output_pin(p);
		    }
		    else
		    {//Pins A and B need special treat: we need to create the fanOut pins for the input wires
		    	Pin *p2 = new Pin(compDeclaration.substr(tt+1,yy-tt-1)+"_2");

		    	p->append_gate(g);
		    	g->append_input_pin(p);	

		    	p2->append_gate(gAux); 
		    	gAux->append_input_pin(p2);

		    	//Save input wires
		    	if (outsMap_.find(wire) != outsMap_.end())
		    	{
		    		// std::cout << "Save in output" << std::endl;
		    		//Never initialize pinTo_ in Primary Outputs. If there is an output used by a gate, directly instantiate fan out
		    		p->append_wire(outsMap_[wire]);
		    		p2->append_wire(outsMap_[wire]);
		    		outsMap_[wire]->append_fanOut_pin(p);
		    		outsMap_[wire]->append_fanOut_pin(p2);
		    		
		    		
		    	}else if (inputsMap_.find(wire) != inputsMap_.end())
		    	{
		    		// std::cout << "Save in input" << std::endl;
		    		p->append_wire(inputsMap_[wire]);
		    		p2->append_wire(inputsMap_[wire]);
		    		if (inputsMap_[wire]->get_pinTo() == NULL)
		    		{
		    			inputsMap_[wire]->append_pin_to(p);
		    		}else{
		    			//fanOut_ found
		    			inputsMap_[wire]->append_fanOut_pin(p);
		    		}
		    		inputsMap_[wire]->append_fanOut_pin(p2);
		    		
		    	}else if (wiresMap_.find(wire) != wiresMap_.end())
		    	{
		    		// std::cout << "Save in wire" << std::endl;
		    		p->append_wire(wiresMap_[wire]);
		    		p2->append_wire(wiresMap_[wire]);
		    		if (wiresMap_[wire]->get_pinTo() == NULL)
		    		{
		    			wiresMap_[wire]->append_pin_to(p); 
		    		}else{
		    			//fanOut_ found
		    			wiresMap_[wire]->append_fanOut_pin(p);
		    		}
		    		wiresMap_[wire]->append_fanOut_pin(p2);
		    		
		    	}else if (randomsMap_.find(wire) != randomsMap_.end())
		    	{
		    		// std::cout << "Save in random" << std::endl;
		    		p->append_wire(randomsMap_[wire]);
		    		p2->append_wire(randomsMap_[wire]);
		    		if (randomsMap_[wire]->get_pinTo() == NULL)
		    		{
		    			randomsMap_[wire]->append_pin_to(p);
		    		}else{
		    			//fanOut_ found
		    			randomsMap_[wire]->append_fanOut_pin(p);
		    		}
		    		randomsMap_[wire]->append_fanOut_pin(p2);
		    	} 
		    }
	    }
	    components.push_back(gAux);
	    // std::cout << "Half adder produced to gates" << std::endl;
	    // std::cout << g->get_name() << " with inputs " << g->get_inputs()[0]->get_wire()->get_name()
	    // 		  << " and " << g->get_inputs()[1]->get_wire()->get_name() << ", and output: " << g->get_output()->get_wire()->get_name() << std::endl;
	   	// std::cout << gAux->get_name() << " with inputs " << gAux->get_inputs()[0]->get_wire()->get_name()
	    // 		  << " and " << gAux->get_inputs()[1]->get_wire()->get_name() << ", and output: " << gAux->get_output()->get_wire()->get_name() << std::endl;
	}//Finished "assign" statements
	else{
		//Normal component compDeclarations
		//Get gate's name and type
		std::string gateType, gateName;

	    int tt = 0;
	    int yy = 0;
	    while (compDeclaration[tt] == ' ') tt++;
	    yy=tt;
	    while (compDeclaration[yy] != ' ') yy++;
	    gateType = compDeclaration.substr(tt,yy-tt);
	    // std::cout << "Gate type: " << gateType << std::endl;
	    while (compDeclaration[yy] == ' ') yy++;
	    tt=yy;
	    while (compDeclaration[yy] != ' ') yy++;
	    gateName = compDeclaration.substr(tt,yy-tt);
	    // std::cout << "Gate naem::::::::: " << gateName << std::endl;

		if ((gateType.find("AND") != std::string::npos) and (gateType.find("NAND") == std::string::npos)){

			g = new ANDgate(gateName);

		}else if ((gateType.find("OR") != std::string::npos) and (gateType.find("NOR") == std::string::npos) and 
				  (gateType.find("XNOR") == std::string::npos) and (gateType.find("XOR") == std::string::npos)){

			g = new ORgate(gateName);

		}else if (gateType.find("NAND") != std::string::npos){

			g = new NANDgate(gateName);

		}else if ((gateType.find("NOR") != std::string::npos) and (gateType.find("XNOR") == std::string::npos)){

			g = new NORgate(gateName);

		}else if (gateType.find("XOR") != std::string::npos){

			g = new XORgate(gateName);

		}else if (gateType.find("XNOR") != std::string::npos){

			g = new XNORgate(gateName);

		}else if ((gateType.find("AOI21") != std::string::npos) and (gateType.find("AOI211") == std::string::npos)){

			g = new AOI21gate(gateName);

		}else if ((gateType.find("OAI21") != std::string::npos) and (gateType.find("OAI211") == std::string::npos)) {

			g = new OAI21gate(gateName);

		}else if (gateType.find("AOI211") != std::string::npos){

			g = new AOI211gate(gateName);

		}else if (gateType.find("OAI211") != std::string::npos){

			g = new OAI211gate(gateName);

		}else if ((gateType.find("AOI22") != std::string::npos) and (gateType.find("AOI221") == std::string::npos) and (gateType.find("AOI222") == std::string::npos)){

			g = new AOI22gate(gateName);

		}else if ((gateType.find("OAI22") != std::string::npos) and (gateType.find("OAI221") == std::string::npos) and (gateType.find("OAI222") == std::string::npos)){

			g = new OAI22gate(gateName);

		}else if (gateType.find("AOI221") != std::string::npos){

			g = new AOI221gate(gateName);

		}else if (gateType.find("OAI221") != std::string::npos){

			g = new OAI221gate(gateName);

		}else if (gateType.find("AOI222") != std::string::npos){

			g = new AOI222gate(gateName);

		}else if (gateType.find("OAI222") != std::string::npos){

			g = new OAI222gate(gateName);

		}else if (gateType.find("OAI33") != std::string::npos){

			g = new OAI33gate(gateName);

		}else if (gateType.find("MUX") != std::string::npos){

			g = new MUXgate(gateName);

		}else if ((gateType.find("BUF") != std::string::npos) or (gateType == "assign")){

			g = new BUFFgate(gateName);

		}else if (gateType.find("INV") != std::string::npos){

			g = new INVgate(gateName);

		}else if ((gateType.find("DFF") != std::string::npos) and (gateType.find("SDFF") == std::string::npos)){

			g = new DFFgate(gateName);

		}else if (gateType.find("SDFF") != std::string::npos){

			g = new SDFFgate(gateName);

		}else if (gateType.find("DLH") != std::string::npos){

			// std::cout << "Latch found in component declaration: " << std::endl;
			// std::cout << compDeclaration << std::endl;
			assert(1 < 0 && "Look at your HDL code and try to fix the latch");

		}
		else{
			// std::cout << "Gate: " << gateType << std::endl;
			assert(1 < 0 && "Found a gate that is not implemented");
		}

		//Store Q wire (if exists) to connect it to QN output
	    Wire *Q_out = NULL;
	    bool exists_Q = 0;

	    //Get pins, wires and connect them
	    while (1){

	    	std::string wire;

	    	while (compDeclaration[tt] != '.'){
	    		tt++;
	    		if (compDeclaration[tt] == ';') break;
	    	}
	    	yy = tt;
	    	if (compDeclaration[tt] == ';') break; 
	    	while (compDeclaration[yy] != '(') yy++;

	    	//Save pin
	    	Pin *p = new Pin(compDeclaration.substr(tt+1,yy-tt-1));
	    	// std::cout << "Pin name: " << p->get_name() << std::endl;
	    	p->append_gate(g);
	    	// std::cout << "Saved gate?: " << p->get_gate()->get_name() << std::endl;
	    	//Save wire and gate's pin
	    	tt=yy + 1;
	    	while (compDeclaration[tt] == ' ') tt++;
		    while (compDeclaration[yy] != ')') yy++;
		    while (compDeclaration[yy-1] == ' ') yy--;
	    	wire = compDeclaration.substr(tt,yy-tt);
		    // std::cout << "Wire: " << wire << "#"<< std::endl;

	    	if ((p->get_name() == "Z") || (p->get_name() == "ZN"))
	    	{	
	    		//Save output wire
		    	if (outsMap_.find(wire) != outsMap_.end())
		    	{
		    		// std::cout << "Save out output" << std::endl;
		    		p->append_wire(outsMap_[wire]);
		    		outsMap_[wire]->append_pin_from(p);
		    	}else if (wiresMap_.find(wire) != wiresMap_.end())
		    	{
		    		// std::cout << "Save out wire" << std::endl;
		    		p->append_wire(wiresMap_[wire]);
		    		wiresMap_[wire]->append_pin_from(p); 

		    	}

		    	g->append_output_pin(p);
	    	}else if ((p->get_name() == "Q"))
	    	{
	    		if (wire == "") continue;
	    		//Flipflop output
	    		if (outsMap_.find(wire) != outsMap_.end())
		    	{
		    		// std::cout << "Save ff output" << std::endl;
		    		p->append_wire(outsMap_[wire]);
		    		outsMap_[wire]->append_pin_from(p);
		    		Q_out = outsMap_[wire];
		    	}else if (wiresMap_.find(wire) != wiresMap_.end())
		    	{
		    		// std::cout << "Save ff wire" << std::endl;
		    		p->append_wire(wiresMap_[wire]);
		    		wiresMap_[wire]->append_pin_from(p); 
		    		Q_out = wiresMap_[wire];
		    	}
		    	g->append_output_pin(p);
		    	exists_Q = 1;
	    	}else if ((p->get_name() == "QN"))
	    	{
	    		if (wire == "") continue;
	    		//If the flipflop compDeclaration only specifies QN (no Q at all) follow same procedre as when it is a Q pin
	    		//the distinction will be done in the evaluation.
	    		if (!exists_Q)
	    		{
	    			//Flipflop output
		    		if (outsMap_.find(wire) != outsMap_.end())
			    	{
			    		// std::cout << "Save ff output" << std::endl;
			    		p->append_wire(outsMap_[wire]);
			    		outsMap_[wire]->append_pin_from(p);
			    		Q_out = outsMap_[wire];
			    	}else if (wiresMap_.find(wire) != wiresMap_.end())
			    	{
			    		// std::cout << "Save ff wire" << std::endl;
			    		p->append_wire(wiresMap_[wire]);
			    		wiresMap_[wire]->append_pin_from(p); 
			    		Q_out = wiresMap_[wire];
			    	}
			    	g->append_output_pin(p);
	    		}else{
	    			//If there was already a Q pin, fanOut the output wire to a new inverter
	    			//Flipflop neg. output, create new gate inverter
	    			// std::cout << "This is QN wire" << endl;
		    		Gate *gg = new INVgate("INV_Q");

					//Input pin to the new gate
					Pin *pp_in = new Pin("I");
					pp_in->append_wire(Q_out);
					pp_in->append_gate(gg);

					gg->append_input_pin(pp_in);

					Q_out->append_fanOut_pin(pp_in);
					
					//New inverter gate output goes to QN pin "p" already created
		    		if (outsMap_.find(wire) != outsMap_.end())
			    	{
			    		// std::cout << "Save ff output" << std::endl;
			    		p->append_wire(outsMap_[wire]);
			    		outsMap_[wire]->append_pin_from(p);
			    	}else if (wiresMap_.find(wire) != wiresMap_.end())
			    	{
			    		// std::cout << "Save ff wire" << std::endl;
			    		p->append_wire(wiresMap_[wire]);
			    		wiresMap_[wire]->append_pin_from(p); 
			    	}
			    	gg->append_output_pin(p);

			    	p->append_gate(gg);
					
	    		}
	    	}else{
		    	if (p->get_name() != "CK")
		    	{
		    		//Save input wires
			    	if (outsMap_.find(wire) != outsMap_.end())
			    	{
			    		// std::cout << "Save in output" << std::endl;
			    		//Never initialize pinTo_ in Primary Outputs. If there is an output used by a gate, directly instantiate fan out
			    		p->append_wire(outsMap_[wire]);
			    		outsMap_[wire]->append_fanOut_pin(p);
			    		
			    		
			    	}else if (inputsMap_.find(wire) != inputsMap_.end())
			    	{
			    		// std::cout << "Save in input" << std::endl;
			    		p->append_wire(inputsMap_[wire]);
			    		if (inputsMap_[wire]->get_pinTo() == NULL)
			    		{
			    			inputsMap_[wire]->append_pin_to(p);
			    		}else{
			    			//fanOut_ found
			    			inputsMap_[wire]->append_fanOut_pin(p);
			    		}
			    		
			    	}else if (wiresMap_.find(wire) != wiresMap_.end())
			    	{
			    		// std::cout << "Save in wire" << std::endl;
			    		p->append_wire(wiresMap_[wire]);
			    		if (wiresMap_[wire]->get_pinTo() == NULL)
			    		{
			    			wiresMap_[wire]->append_pin_to(p); 
			    		}else{
			    			//fanOut_ found
			    			wiresMap_[wire]->append_fanOut_pin(p);
			    		}
			    		
			    	}else if (randomsMap_.find(wire) != randomsMap_.end())
			    	{
			    		// std::cout << "Save in random" << std::endl;
			    		p->append_wire(randomsMap_[wire]);
			    		if (randomsMap_[wire]->get_pinTo() == NULL)
			    		{
			    			randomsMap_[wire]->append_pin_to(p);
			    		}else{
			    			//fanOut_ found
			    			randomsMap_[wire]->append_fanOut_pin(p);
			    		}
			    		
			    	}else if ((wire == "1'b0") || (wire == "1'h0") || (wire == "1'b1") || (wire == "1'h1"))
					{
						//Found input 1'b0, 1'b0
						//Creat one or zero interconnect wire
						Wire *w;
						if ((wire == "1'b0") || (wire == "1'h0"))
						{
							w = new Wire("zero_input");
							w->set_value(0);
						}else
						{
							w = new Wire("one_input");
							w->set_value(1);
						}
						//Connect to the pin
						zeroOnes.push_back(w);

						p->append_wire(w);
						w->append_pin_to(p);

					}
			    	g->append_input_pin(p);
		    	}
                else
                    if (p != nullptr) delete p;
	    	}
	    }
	}

	return g;

};

GatesFactory::~GatesFactory(){
    
//    cout << "GatesFactory destructor" << endl;
//    cout << inputsMap_.size() << endl;
//    for (std::map<std::string, Wire*>::iterator it = inputsMap_.begin(); it != inputsMap_.end(); ++it) {
//        if (it->second != nullptr) {
//            delete it->second;
//        }
//    }
//    cout << "Finished deleting inputs map" << endl;
//    for (std::map<std::string, Wire*>::iterator it = outsMap_.begin(); it != outsMap_.end(); ++it) {
//        if (it->second != nullptr) {
//            delete it->second;
//        }
//    }
//    for (std::map<std::string, Wire*>::iterator it = wiresMap_.begin(); it != wiresMap_.end(); ++it) {
//        if (it->second != nullptr) {
//            delete it->second;
//        }
//    }
//    for (std::map<std::string, Wire*>::iterator it = randomsMap_.begin(); it != randomsMap_.end(); ++it) {
//        if (it->second != nullptr) {
//            delete it->second;
//        }
//    }
}
