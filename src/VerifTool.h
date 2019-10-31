#ifndef VerifTool_H
#define VerifTool_H

#include <vector>
#include <iostream>
#include <stdio.h> 
#include <stdint.h>     
#include <stdlib.h> 
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <map>
#include <sstream>
#include <ctype.h>
#include <math.h>
#include "color.h"
#include <assert.h>

using namespace std;


class Gate;
typedef std::vector<Gate *> gates;
class SuperGate;
typedef std::vector<SuperGate *> superGates;
class Wire;
class GatesFactory;
class FaultsMaxine;
class Fault;

class Pin
{
friend class Netlist;
friend class GatesFactory;

  std::string name_;
  Wire *wire_;
  Gate *gate_;

protected:

  Pin();
  Pin(std::string name);
  Pin(const Pin &pi);

public:
  ~Pin();
    
  void append_wire(Wire *w);
  void append_gate(Gate *g);

  std::string get_name();
  Wire* get_wire();
  Gate* get_gate();

};
typedef std::vector<Pin *> pins;


class Wire
{
friend class Netlist;
friend class GatesFactory;
  
  std::string name_;
  bool value_;
  bool evaluated_;
  bool isSensitiveInput_;
  bool isSensitiveOutput_;
  bool primaryInput_;
  bool primaryOutput_;
  bool fanOut_;
  Pin *pinFrom_;
  Pin *pinTo_; 
  pins fanOutPins_;

protected:
  
  Wire();
  Wire(std::string name);
  Wire(const Wire &w); //Copy constructor

public:
  ~Wire();
    
  void eval_wire(bool val, gates& eventList);
  void de_evaluate_wire();

  void append_pin_from(Pin *p);
  void append_pin_to(Pin *p);
  void append_fanOut_pin(Pin *p);
  void set_value(bool val);
  void set_is_SensI(bool val);
  void set_is_SensO(bool val);
  void set_is_PI(bool val);
  void set_is_PO(bool val);
  
  std::string get_name();
  bool get_value();
  bool get_is_evaluated();
  bool get_is_SensI();
  bool get_is_SensO();
  bool get_is_PI();
  bool get_is_PO();
  bool get_is_fanOut();
  Pin* get_pinFrom();
  Pin* get_pinTo();
  pins get_fanOutPins();

};
typedef std::vector<Wire *> wires;


class Gate
{
  
friend class Netlist;
friend class GatesFactory;
friend class FaultsMaxine;
  
public:
  enum GateType{AND, NAND, OR, NOR, XOR, XNOR, MUX, AOI21, OAI21, AOI211, OAI211, AOI22, OAI22, AOI221, OAI221, AOI222, OAI222, OAI33, INV, BUFF, DFF, SDFF};

protected:
  std::string gateName_;
  GateType gateType_;
  pins inputs_;
  Pin *output_;
  //Faults new addition
  Fault *f_;
  
  Gate();
  Gate(std::string gateName);
  Gate(const Gate &gt);

  //Faults new addition
  void eval_fault(bool value, gates& eventList, int currentClk);

public:
  virtual ~Gate();
    
  void append_output_pin(Pin *p);
  void append_input_pin(Pin *p);

  std::string get_name();
  GateType get_type();
  pins get_inputs();
  Pin* get_output();
  //Faults new addition
  bool is_faulty();
  int get_when_is_faulty();

  virtual void eval_gate(gates& eventList) = 0;
  virtual void eval_gate(gates& eventList, int currentClk) = 0;
  virtual void eval_gate() = 0;
};


class SuperGate
{

friend class Netlist;
friend class GatesFactory;
friend class FaultsMaxine;
friend class VerifTool;

protected:

  //Inputs/outputs
  std::string type_;
  std::string name_;
  pins inputs_;
  pins outputs_;

  //Internal wires
  wires inputWires_;
  wires outputWires_;
  wires wires_;
  wires zeroOnes_;

  //Internal wires Maps
  std::map<std::string, Wire *> inputsMap_;
  std::map<std::string, Wire *> outsMap_;
  std::map<std::string, Wire *> wiresMap_;

  //Components
  gates logicGates_;
  superGates superGates_;
  //Buffer gates to make the connection with the outside which will allow us to fault also these wires directly
  gates primaryInputs_;
  gates primaryOutputs_;

  SuperGate(std::string type){type_ = type; name_ = "";};
  //Copy constructor
  SuperGate(const SuperGate &sG);
  
  //Declare getters/setters
    
public:
    ~SuperGate();

};


class Subcircuit
{
friend class Netlist;

    std::string ident_;

    wires subcircuitInputs_;
    wires subcircuitRandom_;
    wires subcircuitOutputs_;
    std::vector<wires> subcircuitSharedVariables_;

    gates subcircuitGates_;

protected:
    Subcircuit();
  

public:
    ~Subcircuit();
    
	std::string get_ident();
	wires get_subcircInputs();
	wires get_subcircRandoms();
	wires get_subcircOuts();
	std::vector<wires> get_subcircSharedVars();
	gates get_subcircGates();

};
typedef std::vector<Subcircuit *> subcircuits;

class Netlist
{
friend class VerifTool;
friend class FaultsMaxine;

    string topModule_;
	//Inps, wires and outs from design
	// Wire* clk, rst, load, start, ready;
	Wire* clk_;
	Wire* rst_;
	Wire* load_;
	Wire* start_;
    // Wire* sel_;
    Wire* ready_;
    std::vector<wires> sharedVariables_;
    wires ins_;
    wires wires_;
    wires outs_;
    wires randomBits_;
    wires publicInputs_;
    wires zeroOnes_;
    //New inputs and outputs gotten from flipflops to build subcircuits. The outsputs of the flipflops will be new inputs
    wires inputsFF_;
    wires outputsFF_;
    std::map<std::string, Wire *> newInputsMap_;
    std::map<std::string, Wire *> newOutputsMap_;
    std::vector<std::vector<wires>> regsSharedVariables_;
    std::map<std::string, std::vector<wires> > outsSensitiveDataMap_;

    //Dependencies and declarations
    std::map<std::string, Wire *> inputsMap_;//Dep. values for each input
    std::map<std::string, Wire *> randomsMap_;//Dep. values for each random bit
    std::map<std::string, Wire *> wiresMap_;//Dep. values for each wire
    std::map<std::string, Wire *> outsMap_;//Keep output deps.
    std::map<std::string, SuperGate *> sGatesMap_;//Keep SuperGates for each type
    std::vector<std::string> componentDeclarations_;

    //Gates Factory
    GatesFactory *gFactory_;

    //components_
    gates components_;
    superGates superComponents_;
    SuperGate* topModuleSG_; //Gate to keep the last SuperGate created in hy_netlist


    //Sequential circuit?
    bool sequential_;

    //Keep the different combinational Subcircuits
    subcircuits combinationalSubcircs_;

    #ifdef YOSYS
    string topModuleDeclaration_;
    #endif

    //Get new in/outs
    void get_ff_inOuts();
    //Search
    void search_back(Gate *g, wires &input_deps);
    void subcircuit_search_back(Wire *w, Subcircuit *subcirc, wires &remaining_outs);
    void subcircuit_search_forth(Wire *w, Subcircuit *subcirc, wires &remaining_outs);
    //Build subcircuits
    void layers_sensitive_data(std::vector<std::vector<std::vector<std::string>>> regsLayers);
    void build_subcircuits();

    //Parsing functions
    string get_module_name(string line);
    void get_inputs(SuperGate* superGate, string line);
    void get_outputs(SuperGate* superGate, string line);
    void get_wires(SuperGate* superGate, string line);
    Gate* create_primaryInput_buffer(Wire *inputWire);
    Gate* create_primaryOutput_buffer(Wire* outputWire);
    void get_superGate_pins(SuperGate *thisSuperGate, SuperGate *subSuperGate, string line);
    void rename_sub_super_gates(superGates &sGates, string name);
    void get_sub_ff_inouts(superGates const &sGates, wires &inputsFF, wires &outputsFF);

    void plot_names_sub_gates(superGates const &sGates);
    void plot_data_path(Wire *w);
    void plot_data_path_output(Wire *w);

protected:
    Netlist(){topModule_ = ""; clk_ = NULL; rst_ = NULL; load_ = NULL; start_ = NULL; /*sel_ = NULL;*/ ready_ = NULL; gFactory_ = NULL; topModuleSG_ = NULL;};
    Netlist(string topMod){topModule_ = topMod; clk_ = NULL; rst_ = NULL; load_ = NULL; start_ = NULL; /*sel_ = NULL;*/ ready_ = NULL; gFactory_ = NULL; topModuleSG_ = NULL;};

	void create_netlist(std::ifstream &fNetlist, std::vector<std::vector<std::string>> inputs, std::vector<size_t> varsBrackets,
  							   		std::vector<std::string> outputs, std::vector<std::string> randomVars,
  							   		std::vector<std::vector<std::vector<std::string>>> regsLayers);

  void create_hy_netlist(std::ifstream &fNetlist, std::vector<std::vector<std::string>> inputs,
                         std::vector<std::string> outputs);

  #ifdef YOSYS
    void chop_modules_declaration(std::ifstream &fNetlist, map<std::string, bool> &modulesParsed, vector<vector<string>> &modulesChopped);
    void create_yosys_netlist(const vector<string> moduleDeclarations);
    void get_submodules(std::ifstream &fNetlist, const map<string, bool> modulesParsed, vector<vector<string>> &modulesHy);
    void low_endian_ins(wires &vecOfWires);
    void low_endian_outs(wires &vecOfWires);
  #endif
    
public:
    ~Netlist();
};


class VerifTool
{
 
private:
  
  //Files
  std::string inFile_;
  std::ifstream fIn_;
  std::ifstream fNetlist_;
  std::ifstream fNetlistHy_;
  
  //VHDL, Verilog or SVerilog??
  std::string prog_lang;
  std::string module;
  std::string architecture;
  
  //Inputs, random vars and registers from file
  bool outsSpecified_;
  std::vector<size_t> varsBrackets_; //Count number of brackets (if there are) in the input data specified for the different vars (only one per variable)
  std::vector<std::vector<std::string>> inputs_;
  std::vector<std::string> outputs_;
  std::vector<std::string> randomVars_;
  std::vector<std::vector<std::vector<std::string>>> regsLayers_;
  std::vector<std::string> checkBits_;
  
  //Netlists
  Netlist nl_;
  Netlist nlHy_;

  //Faults Generator
  FaultsMaxine *fM_;
  
  //Functions
  //Non-completeness
  void first_non_complet(bool outsSpecified);
  void sequential_first_non_complet(bool outsSpecified);
  void second_non_complet(bool outsSpecified);
  void sequential_second_non_complet(bool outsSpecified);
  void third_non_complet(bool outsSpecified);
  void sequential_third_non_complet(bool outsSpecified);
  //Uniformity
  void de_evaluate_wires();
  void de_evaluate_wires_simul();
  // bool eval_uniformity(std::vector<std::vector<int>> outs_grouped, std::vector<int> freqs);

  void plot_faults_config();
  void print_names_sub_modules(std::ofstream &file, superGates const &sGates);
  void plot_components_fault_config();
  void plot_sub_components_fault_config(std::ofstream &file, superGates const &sGates);
  
public:
  
  VerifTool(std::string inp_file) : inFile_(inp_file){
    
    fIn_.open(inp_file);
    if (!fIn_.is_open()) {
			std::cout << BOLD(FRED("Could not read input file")) << std::endl;
			return ;
		}
    //Check prog language
    if(inp_file.find(".vhd") != std::string::npos){
      prog_lang = "vhdl";
    }else if(((inp_file.find(".v") != std::string::npos) and (inp_file.find(".vhd") == std::string::npos)) or (inp_file.find(".sv") != std::string::npos)){
      prog_lang = "verilog";
    }

  };

    VerifTool();//int argc_, char * argv_[], std::string language_type);
    ~VerifTool();

  //Read input file
  bool read_in_file();
  bool read_in_file_faults();
  
  //Build TCL file
  // #ifndef YOSYS
    bool build_tcl();
  // #else
    bool build_yosys_tcl(const char* arg);
  // #endif
  
  //Read netlist file. Get inputs, wires and outputs, as well as wires and outs declarations
  bool read_netlist();
  bool read_hy_netlist();
  
  //Evaluate non_completeness from the file produced by Synopsis
  void analyze_first_non_complet();
  void analyze_second_non_complet();
  void analyze_third_non_complet();
  //Uniformity evaluation
  bool uniformity_check();

  //Get input dependencies
  bool input_deps();

  //Plot subcircuits
  void plot_subcircuits();

  //Fault Simulation
  void create_faults_maxine();
  void faults_evaluation(std::string inputsSimFile, std::string faultsConfigFile);

  
  #ifdef TEST
  //Define here test functions for the Verification Tool
    void test_simulation_keccakp200();
    void test_simulation_keccakp200_randomInput();
    void test_fault_injection_in_unprotected_keccak_to_see_coverage();
    void test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output();
    void test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output();
    void test_fault_simulation_in_unprotected_keccak_for_single_sa1();
    void test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01();
    void test_yosys_netlist_create();

  #endif //TEST

};

bool compare_wires(Wire *wireA, Wire *wireB);

#endif // VerifTool_H
