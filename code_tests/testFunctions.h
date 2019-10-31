

#ifndef testFunctions_H
#define testFunctions_H

void test_simulation_keccakp200();

void test_simulation_keccakp200_randomInput();

void test_fault_injection_in_unprotected_keccak_to_see_coverage();

void test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output();

void test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output();

void test_fault_simulation_in_unprotected_keccak_for_single_sa1();

void test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01();

void test_yosys_netlist_create();

#endif