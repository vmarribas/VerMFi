#include "VerifTool.h"

Wire::Wire(){
    name_ = "";
    value_ = 0;
    evaluated_ = 0;
    isSensitiveInput_ = 0;
    isSensitiveOutput_ = 0;
    primaryInput_ = 0;
    primaryOutput_ = 0;
    fanOut_ = 0;
    pinFrom_ = NULL;
    pinTo_ = NULL;
};
 
Wire::Wire(std::string name){
    name_ = name;
    value_ = 0;
    evaluated_ = 0;
    isSensitiveInput_ = 0;
    isSensitiveOutput_ = 0;
    primaryInput_ = 0;
    primaryOutput_ = 0;
    fanOut_ = 0;
    pinFrom_ = NULL;
    pinTo_ = NULL;
};

Wire::Wire(const Wire &w){
    name_ = w.name_;
    value_ = w.value_;
    evaluated_ = w.evaluated_;
    isSensitiveInput_ = w.isSensitiveInput_;
    isSensitiveOutput_ = w.isSensitiveOutput_;
    primaryInput_ = w.primaryInput_;
    primaryOutput_ = w.primaryOutput_;
    fanOut_ = w.fanOut_;
    pinFrom_ = w.pinFrom_;
    pinTo_ = w.pinTo_;
    fanOutPins_ = w.fanOutPins_;
}

Wire::~Wire(){
    
//    cout << "Wire destructor of " << name_ << endl;
    
//    if (pinFrom_ != nullptr) delete pinFrom_;
//    if (pinTo_ != nullptr) delete pinTo_;
//    
//    cout << "Wire destructor first part done" << endl;
//
//    for (pins::iterator it = fanOutPins_.begin(); it != fanOutPins_.end(); ++it) {
//        if (*it != nullptr) delete *it;
//    }
//    cout << "END wire destructor" << endl;
}


void Wire::append_pin_from(Pin *p){
	this->pinFrom_ = p;
};

void Wire::append_pin_to(Pin *p){
	//Check if we already have an output pin, thus put in fanoutpins
    if (this->pinTo_ == NULL)
    {
        this->pinTo_ = p;
    }
    else
    {
        this->fanOut_ = 1;
        this->fanOutPins_.push_back(p);
    }
};

void Wire::append_fanOut_pin(Pin *p){
	this->fanOut_ = 1;
	this->fanOutPins_.push_back(p);
};

void Wire::set_value(bool val){
    if (!evaluated_)
    {
        this->value_ = val;
        this->evaluated_ = 1;
    }
};

void Wire::set_is_SensI(bool val){
    this->isSensitiveInput_ = val;
};

void Wire::set_is_SensO(bool val){
    this->isSensitiveOutput_ = val;
};

void Wire::set_is_PI(bool val){
    this->primaryInput_ = val;
};

void Wire::set_is_PO(bool val){
    this->primaryOutput_ = val;
};


std::string Wire::get_name(){
    return this->name_;
};

bool Wire::get_value(){
    return this->value_;
};

bool Wire::get_is_evaluated(){
    return this->evaluated_;
};

bool Wire::get_is_SensI(){
    return this->isSensitiveInput_;
};

bool Wire::get_is_SensO(){
    return this->isSensitiveOutput_;
};

bool Wire::get_is_PI(){
    return this->primaryInput_;
};

bool Wire::get_is_PO(){
    return this->primaryOutput_;
};

bool Wire::get_is_fanOut(){
    return this->fanOut_;
};

Pin* Wire::get_pinFrom(){
    return this->pinFrom_;
};

Pin* Wire::get_pinTo(){
    return this->pinTo_;
};

pins Wire::get_fanOutPins(){
    return this->fanOutPins_;
};

void Wire::de_evaluate_wire(){
    this->value_ = 0;
    this->evaluated_ = 0;
};
