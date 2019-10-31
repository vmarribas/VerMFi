
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
