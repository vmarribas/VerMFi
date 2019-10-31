
////////////////////////////////////////////////////////////////////////////////

// COMPANY:     COSIC, KU Leuven 
// AUTHOR:      Victor Arribas

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
#include "color.h"
#include "GatesFactory.h"
#include "FaultsMaxine.h"

using namespace std;

bool VerifTool::read_netlist()
{

    //Read file
    fNetlist_.open("./netlist/" + module + ".v");
    if (!fNetlist_.is_open()) return 0;

    //Ungrouped netlist
    nl_ = Netlist();

    nl_.create_netlist(fNetlist_, inputs_, varsBrackets_, outputs_, randomVars_, regsLayers_);

    return 1;

}

bool VerifTool::read_hy_netlist()
{

    //Hierarchy netlist
    //Read file
    #ifndef YOSYS
        fNetlistHy_.open("./netlist/" + module + "_hierarchy.v");
    #else
        fNetlistHy_.open("./netlist/yosys/" + module + "_yosys.v");
    #endif

    
    if (!fNetlistHy_.is_open()) return 0;
//    free; nlHy_;
    nlHy_ = Netlist(module);
    nlHy_.create_hy_netlist(fNetlistHy_, inputs_, outputs_);

    cout << "Finished reading netlist" << endl;
    
    return 1;

}

VerifTool::VerifTool(){
};

VerifTool::~VerifTool(){
//    delete fM_;
};

