
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
