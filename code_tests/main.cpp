
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h> 
#include "../src/VerifTool.h"
#include "testFunctions.h"
#include <chrono>


int main(int argc, char **argv)
{
// #ifndef YOSYS
	cout << "Hello tests!!" << endl;

	//Test keccak simulation
	test_simulation_keccakp200();

	//Test keccak simulation with random inputs
	test_simulation_keccakp200_randomInput();

	//Test keccak fault injection
	test_fault_injection_in_unprotected_keccak_to_see_coverage();

	//Test single fault sim
	test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output();

	//Test two-fault injection
	test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output();

	//Test single SA1
	test_fault_simulation_in_unprotected_keccak_for_single_sa1();

	//Test multilpe SA0 and SA1s, set at a particular cycle
	test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01();

// #else
	// Yosys tests
	test_yosys_netlist_create();
	
// #endif

	cout << BOLD(FGRN("ALL TEST PASSED!!!!!!!!")) << endl;

	return 0;
}