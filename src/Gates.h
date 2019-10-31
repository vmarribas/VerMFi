
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


#ifndef Gates_H
#define Gates_H

#include "VerifTool.h"



class ANDgate : public Gate{

public:
	ANDgate(){};
	ANDgate(std::string name);
	~ANDgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class NANDgate : public Gate{

public:
	NANDgate(){};
	NANDgate(std::string name);
	~NANDgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class ORgate : public Gate{

public:
	ORgate(){};
	ORgate(std::string name);
	~ORgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class NORgate : public Gate{

public:
	NORgate(){};
	NORgate(std::string name);
	~NORgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class XORgate : public Gate{

public:
	XORgate(){};
	XORgate(std::string name);
	~XORgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class XNORgate : public Gate{

public:
	XNORgate(){};
	XNORgate(std::string name);
	~XNORgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class MUXgate : public Gate{

public:
	MUXgate(){};
	MUXgate(std::string name);
	~MUXgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class AOI21gate : public Gate{

public:
	AOI21gate(){};
	AOI21gate(std::string name);
	~AOI21gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI21gate : public Gate{

public:
	OAI21gate(){};
	OAI21gate(std::string name);
	~OAI21gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class AOI211gate : public Gate{

public:
	AOI211gate(){};
	AOI211gate(std::string name);
	~AOI211gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI211gate : public Gate{

public:
	OAI211gate(){};
	OAI211gate(std::string name);
	~OAI211gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class AOI22gate : public Gate{

public:
	AOI22gate(){};
	AOI22gate(std::string name);
	~AOI22gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI22gate : public Gate{

public:
	OAI22gate(){};
	OAI22gate(std::string name);
	~OAI22gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class AOI221gate : public Gate{

public:
	AOI221gate(){};
	AOI221gate(std::string name);
	~AOI221gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI221gate : public Gate{

public:
	OAI221gate(){};
	OAI221gate(std::string name);
	~OAI221gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class AOI222gate : public Gate{

public:
	AOI222gate(){};
	AOI222gate(std::string name);
	~AOI222gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI222gate : public Gate{

public:
	OAI222gate(){};
	OAI222gate(std::string name);
	~OAI222gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class OAI33gate : public Gate{

public:
	OAI33gate(){};
	OAI33gate(std::string name);
	~OAI33gate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class INVgate : public Gate{

public:
	INVgate(){};
	INVgate(std::string name);
	~INVgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class BUFFgate : public Gate{

public:
	BUFFgate(){};
	BUFFgate(std::string name);
	~BUFFgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk);
	void eval_gate(){};
};

class DFFgate : public Gate{

	bool Q_;

public:
	DFFgate(){};
	DFFgate(std::string name);
	~DFFgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk){};
	void eval_gate();
	void set_Q(bool val);
	bool get_Q();
};

class SDFFgate : public Gate{

	bool Q_;

public:
	SDFFgate(){};
	SDFFgate(std::string name);
	~SDFFgate(){};

	void eval_gate(gates& eventList);
	void eval_gate(gates& eventList, int currentClk){};
	void eval_gate();
	void set_Q(bool val);
	bool get_Q();
};
#endif // Gates_H