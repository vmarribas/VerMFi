
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
#ifndef GatesFactory_H
#define GatesFactory_H

#include "VerifTool.h"
#include "Gates.h"

class Gate;
class Wire;

class GatesFactory
{
	friend class Netlist;

	std::map<std::string, Wire *> inputsMap_;//Dep. values for each input
	std::map<std::string, Wire *> randomsMap_;//Dep. values for each random bit
	std::map<std::string, Wire *> wiresMap_;//Dep. values for each wire
	std::map<std::string, Wire *> outsMap_;//Keep output deps.
	// std::string compDeclaration_;

protected:

	GatesFactory(std::map<std::string, Wire *> &inputsMap,
				 std::map<std::string, Wire *> &randomsMap,
				 std::map<std::string, Wire *> &wiresMap,
				 std::map<std::string, Wire *> &outsMap) : inputsMap_(inputsMap), randomsMap_(randomsMap), wiresMap_(wiresMap), outsMap_(outsMap) {};
	GatesFactory();

	Gate* create_gate(std::string compDeclaration, wires &zeroOnes, gates &components);

public:
    ~GatesFactory();
};

#endif // GatesFactory_H
