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

