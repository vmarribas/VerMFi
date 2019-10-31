
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

void Netlist::subcircuit_search_forth(Wire *w, Subcircuit *subcirc, wires &remaining_outs){
	//We need reamaining_outs in order for the algorithm to be able to erase them when one is stored in a subcirc
	// cout << "Getting in search forth subcirc! wire_: " << w->name_  << " Fanout? " << w->fanOut_ << " " << w->fanOutPins_.size() << std::endl;
	if ((outsMap_.find(w->name_) != outsMap_.end()) || (newOutputsMap_.find(w->name_) != newOutputsMap_.end()))	
	{
		//The wire_ is an output_. Include it in the sub_outs and erase it from remaining_outs
		//Save first the current gate driving this wire_ if it does not exist already
		//We need to store it here as well in case this is an output_ comming directly from a PI, what means that in this case for that run the algorith did not pass by the else
		// cout << "It is an output" << std::endl;

		if ((w->pinFrom_ != NULL) && //In case an input is a new output_!!!!!!!!!!!!!!!!!!!!1111
			(newInputsMap_.find(w->name_) == newInputsMap_.end())) // In case a register output goes immediately to a register input so it is both newIn & newOut
		{
			if ((std::find(subcirc->subcircuitGates_.begin(), subcirc->subcircuitGates_.end(), w->pinFrom_->gate_) == subcirc->subcircuitGates_.end()) && // PROBALY NOT NEEDED COZ DONE IN THE ELSE?????
				(w->pinFrom_->gate_->get_type() != Gate::DFF))
			{
				// cout << "Saving gate: " << w->pinFrom_->gate_->gateName_ << std::endl;
				assert(w->pinFrom_->gate_->get_type() != Gate::DFF && "Saving a register into a combinational subcircuit");
				assert(w->pinFrom_->gate_->get_type() != Gate::SDFF && "Saving a register into a combinational subcircuit");
				subcirc->subcircuitGates_.push_back(w->pinFrom_->gate_);
			}
		}

		if (newInputsMap_.find(w->name_) != newInputsMap_.end())// In case a register output goes immediately to a register input so it is both newIn & newOut
		{ // This will propagate the recursion further into the output of the register, changing subcirc... 
			if ((w->pinTo_ != NULL) && // In case newInput is a output as well!
				(w->pinTo_->gate_->get_type() != Gate::DFF))
			{//Only do it if the register is stored as part of the fanout
				// cout << "Recursive function goes to wire_ forth: " << w->pinTo_->gate_->output_->wire_->name_ << std::endl;
				subcircuit_search_forth(w->pinTo_->gate_->output_->wire_, subcirc, remaining_outs);
			}
			// cout << "Back to wire0: " << w->name_ << endl;
		}			

		if (w->fanOut_)
		{
			// cout << "fanout in output_!!" << std::endl;
			for (size_t i = 0; i < w->fanOutPins_.size(); ++i)
			{
				if (w->fanOutPins_[i]->gate_->get_type() != Gate::DFF)
				{
					// cout << "Recursive function forth goes to fanout wire_: " << w->fanOutPins_[i]->gate_->output_->wire_->name_ << std::endl;
					subcircuit_search_forth(w->fanOutPins_[i]->gate_->output_->wire_, subcirc, remaining_outs);
				}
			}
			// cout << "Back to wire1: " << w->name_ << endl;
		}

		//Save the wire_ if not saved already
		if (std::find(remaining_outs.begin(), remaining_outs.end(), w) != remaining_outs.end())
		{
			// cout << "Saving output_: " << w->name_ << std::endl;
			subcirc->subcircuitOutputs_.push_back(w);
			remaining_outs.erase(std::find(remaining_outs.begin(), remaining_outs.end(), w));
			subcircuit_search_back(w, subcirc, remaining_outs);
			// cout << "Back to wire2: " << w->name_ << endl;
		}
	}
	else{
		//Continue forward 
		//Save first the current gate driving this wire_if it does not exist already
		// cout << "It is other stuff" << endl;

		// Instead of assert, simply stop the search forward!
		if (w->pinTo_ != NULL)
		{
			if ((w->pinTo_->gate_->get_type() != Gate::DFF) && (w->pinTo_->gate_->get_type() != Gate::SDFF))
			{
				if (std::find(subcirc->subcircuitGates_.begin(), subcirc->subcircuitGates_.end(), w->pinTo_->gate_) == subcirc->subcircuitGates_.end())
				{
					// cout << "Saving gate: " << w->pinTo_->gate_->gateName_ << std::endl;
					assert(w->pinTo_->gate_->get_type() != Gate::DFF && "Saving a register into a combinational subcircuit");
					assert(w->pinTo_->gate_->get_type() != Gate::SDFF && "Saving a register into a combinational subcircuit");
					subcirc->subcircuitGates_.push_back(w->pinTo_->gate_);
				}

				//Apply the same to the output_ and every fanout wire_ 
				// cout << "Recursive function goes to wire_ forth: " << w->pinTo_->gate_->output_->wire_->name_ << std::endl;
				subcircuit_search_forth(w->pinTo_->gate_->output_->wire_, subcirc, remaining_outs);
				// cout << "Back to wire3: " << w->name_ << endl;
			}

			if (w->fanOut_)
			{
				// cout << "fanout in wire_ !!: " << w->name_ << std::endl;
				for (size_t i = 0; i < w->fanOutPins_.size(); ++i)
				{
					// cout << "Recursive function forth goes to fanout wire_: " << w->fanOutPins_[i]->gate_->output_->wire_->name_ << std::endl;
					subcircuit_search_forth(w->fanOutPins_[i]->gate_->output_->wire_, subcirc, remaining_outs);
				}
				// cout << "Back to wire4: " << w->name_ << endl;
			}
		}
	}
	// cout << "Finished forth: " << w->name_ << endl;
}
