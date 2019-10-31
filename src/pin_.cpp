#include "VerifTool.h"


Pin::Pin(){
    name_ = "";
    wire_ = NULL;
    gate_ = NULL;
};

Pin::Pin(std::string name){
	name_ = name;
	wire_ = NULL;
    gate_ = NULL;
};

Pin::Pin(const Pin &pi){
	name_ = pi.name_;
	wire_ = pi.wire_;
	gate_ = pi.gate_;
}

Pin::~Pin(){
    /*We do not need to again destroy the wire, it is already
     being destroyed from who first generated the wire destructor */
//    if (wire_ != nullptr) delete wire_;
//    cout << "Pin destructor of " << name_ << endl;

//    if (gate_ != nullptr) delete gate_;
//    cout << "END pin destructor" << endl;
}

void Pin::append_wire(Wire *w){
	this->wire_ = w;
};

void Pin::append_gate(Gate *g){
	this->gate_ = g;
};

std::string Pin::get_name(){
	return this->name_;
};

Wire* Pin::get_wire(){
	return this->wire_;
};

Gate* Pin::get_gate(){
	return this->gate_;
};
