#ifndef FaultsMaxine_H
#define FaultsMaxine_H

#include "VerifTool.h"

using namespace std;

namespace FaultType
{
    enum FType{SEU, SA0, SA1};
}

typedef tuple<Gate *, int, FaultType::FType> fault;
typedef std::vector<fault> faults;


class Fault
{
	
	friend class Gate;
	friend class FaultsMaxine;

	bool active_; //Is the fault active?
	int cycle_; //Which cycle do we inject it?
	FaultType::FType fType_; //Which kind of fault?

protected:
	Fault(){
		active_ = 0;
		cycle_ = -1;
		fType_ = FaultType::SEU;
	};
    ~Fault(){};

public:
	void set_fault(bool activateFault, int cycle);
	void set_fault_type(FaultType::FType type);
};


class FaultsMaxine
{

friend class VerifTool;

	Netlist netList_; //Netlist to evaluate faults in

	//Check bits
	wires checkBits_;
	bool serialCheckAccumulate_;

	//Clock
	uint clk_;
	int totalCyclesEncryption_;

	//Simulation inputs
	std::vector<string> testVectorsIn_;
	uint extraCyclesSim_;
	uint loadCycles_;
	uint rstCycles_;
	uint numberOfTraces_;
	uint testVectorIndex_; //Count bit position of in testVectorIn_
	uint testVectorSimulating_;

	//Design type
	bool serialImpl_;
	uint nomberOfShares_;

	//Non faulty output
	std::string refOutput_;
	std::string serialOut_;

	//Keep which components were faulted
	std::vector<faults> faultSimulations_;
	gates faultyGates_;
	gates previousFaultyGates_;

	//Faults classification
	int faultsDetected_;
	int faultsIneffective_;
	int faultsNotDetected_;

	//Faults classification over all inputs
	int totalFaultsDetected_;
	int totalFaultsIneffective_;
	int totalFaultsNotDetected_;

	//File for results
	std::ofstream faults_file_;
	std::ofstream faults_file_detailed_;

	void read_sim_inputs_file(std::string inputsSimFile);
	void create_random_inputs();

	void create_faults();
	void create_sub_faults(superGates &sGates);

	// void fault_sub_injection(string compName, superGates &sGates, bool continueDown, superGates &sGatesToFault);
	void hierarchy_sub_fault_injection(std::ifstream& fConfig, superGates &sGates, faults &gatesToFault);
	bool hierarchy_fault_injection(std::string faultsConfigFile, faults &componentsToFault, int &N_SIMS, int &N_FAULTS, int &PER_CYCLE);
	bool components_sub_fault_injection(std::ifstream& fConfig, superGates &sGates, faults &gatesToFault);
	bool components_fault_injection(std::string faultsConfigFile, faults &componentsToFault, int &N_SIMS, int &N_FAULTS, int &PER_CYCLE);
	void hierarchy_fault_sims(const int N_SIMS, const int N_FAULTS, const int PER_CYCLE, const faults componentsToFault);
	void comb(uint n, int r, faults &faultsVec, const faults componentsToFault);
	void components_fault_sims(const int N_SIMS, const int N_FAULTS, const int PER_CYCLE, const faults componentsToFault);
	bool fault_injection(std::string faultsConfigFile);


	void initialize_registers();
	void initialize_sub_registers(superGates &sGates);

	void update_registers();
	void update_sub_registers(superGates &sGates);

	void assign_inputs_values(gates& eventList);
	void assign_control_inputs(bool rst, bool load, bool start, gates& eventList);

	void propagate_sub_zero_ones(superGates &sGates, gates &eventList);
	void propagate_zero_ones(gates &eventList);
	void propagate_regs_newPIs(gates& eventList);

	void activate_faulty_gates(gates& eventList);
	void evaluate_event_list(gates& eventList);

	void one_cycle_simulation(gates &eventList);
	void simulate();

	void start_simulation_nofile();
  	
  	void get_sub_wire_string(string name, superGates &sGates, string &outBuff);
  	string get_wire_string(string componentName);
  	string get_output_string();
  	string get_unshared_output_string(int shares);

  	bool check_checkBits(string output);
  	int clasify_fault(string output);


protected:

	FaultsMaxine(Netlist &netList) : netList_(netList) {
		faults_file_.open("./faults/evaluation_faults_"+netList.topModule_+".md");
		// faults_file_detailed_.open("faults_eval_detailed.txt");
	};
	FaultsMaxine();
	~FaultsMaxine();

  	//Start evaluation
  	void get_checkBits(std::vector<string> checkBits);
  	void get_sub_checkBits(superGates &sGates, string checkBit);
  	void go(std::string inputsSimFile, std::string faultsConfigFile);
};




#endif // FaultsMaxine_H
