
#include "testFunctions.h"
#include "../src/VerifTool.h"
#include "../src/conversions.h"
#include "../src/color.h"
#include "../src/FaultsMaxine.h"

// #ifndef YOSYS

///////////////////////////////////////////////////////////////////////////////////
void test_simulation_keccakp200(){
	
	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_simulation_keccakp200();

}
void VerifTool::test_simulation_keccakp200()
{
	// cout << "hello KECCAK test!!!" << endl;
	
	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->read_sim_inputs_file("./code_tests/testing_purpose_files/kecca_ins2.txt");

	fM_->create_faults();

	//First simulation
	fM_->testVectorSimulating_ = 0;
	fM_->testVectorIndex_ = 0;
	fM_->totalCyclesEncryption_ = -1;

	fM_->simulate();

	// cout << bin_to_hex(fM_->get_output_string()) << endl;
	assert((bin_to_hex(fM_->get_output_string()) == "3C2826841CB35C171EAAE9B811134CEAA3852C69D2C5ABAFEA") && "Failed first output simulations for Keccak_p200");

	//Second simulation
	fM_->testVectorSimulating_ = 1;
	fM_->testVectorIndex_ = 0;

	fM_->simulate();

	assert((bin_to_hex(fM_->get_output_string()) == "1BEF689492A8A543A5999FDB834E3166A14BE827D95040479E") && "Failed second output simulations for Keccak_p200");

	cout << BOLD(FGRN("test_simulation_keccakp200 test PASSED")) << endl;

}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_simulation_keccakp200_randomInput(){
	
	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_simulation_keccakp200_randomInput();

}
void VerifTool::test_simulation_keccakp200_randomInput()
{
	// cout << "hello KECCAK test!!!" << endl;
	
	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2randIns.txt", "./code_tests/testing_purpose_files/faults_config_Keccak_p200.md");

	assert(fM_->testVectorsIn_.size() == 10 && "10 random inputs should be generated");
	assert(fM_->testVectorsIn_[0].size() == 200 && "Input vector length should be 200");

	assert((fM_->totalFaultsNotDetected_ + fM_->totalFaultsIneffective_ + fM_->totalFaultsDetected_) == 30 && "Faults simulated are not equal to 3, which are the number of faults selected in the file");

	cout << BOLD(FGRN("test_simulation_keccakp200_randomInput test PASSED")) << endl;

}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_fault_injection_in_unprotected_keccak_to_see_coverage(){

	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_fault_injection_in_unprotected_keccak_to_see_coverage();
}
void VerifTool::test_fault_injection_in_unprotected_keccak_to_see_coverage(){


	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2.txt", "./code_tests/testing_purpose_files/faults_config_Keccak_p200.md");


	float totalCoverage = 1 - ((float)fM_->totalFaultsNotDetected_ / (float)(fM_->totalFaultsNotDetected_ + fM_->totalFaultsDetected_));
	assert(totalCoverage == 0 && "Coverage should be zero, it is a non-protected implementation");
	assert((fM_->totalFaultsNotDetected_ + fM_->totalFaultsIneffective_) == 6 && "Faults simulated are not equal to 3, which are the number of faults selected in the file");

	remove("./faults/evaluation_faults_Keccak_p200.pdf");
	remove("./faults/evaluation_faults_Keccak_p200.md");
	remove("./faults/nonDetected_faults_Keccak_p200.md");
	remove("./faults/ineffective_faults_Keccak_p200.md");

	cout << BOLD(FGRN("test_fault_injection_in_unprotected_keccak_to_see_coverage test PASSED")) << endl;

}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output(){

	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output();

};
void VerifTool::test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output(){

	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2.txt", "./code_tests/testing_purpose_files/config_components_fault_Keccak_p200_singleFault.md");

	
	assert((bin_to_hex(fM_->get_output_string()) == "9418BA95CA62E30F895AEEFCE4EC6B936F53C9BCAA32D6CAA2") && "Failed single gate faulted simulation");
	

	remove("./faults/evaluation_faults_Keccak_p200.pdf");
	remove("./faults/evaluation_faults_Keccak_p200.md");
	remove("./faults/nonDetected_faults_Keccak_p200.md");
	remove("./faults/ineffective_faults_Keccak_p200.md");

	cout << BOLD(FGRN("test_fault_simulation_in_unprotected_keccak_for_a_single_fault_to_verify_wrong_output test PASSED")) << endl;
	
};
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output(){

	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output();

};
void VerifTool::test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output(){

	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2.txt", "./code_tests/testing_purpose_files/config_components_fault_Keccak_p200 _doubleFault.md");

	// try{
		assert((bin_to_hex(fM_->get_output_string()) == "922855A80581977ACBDCFF2C91D309547112575508D3352718") && "Failed two-fault simulation");
	// }
		
	remove("./faults/evaluation_faults_Keccak_p200.pdf");
	remove("./faults/evaluation_faults_Keccak_p200.md");
	remove("./faults/nonDetected_faults_Keccak_p200.md");
	remove("./faults/ineffective_faults_Keccak_p200.md");

	cout << BOLD(FGRN("test_fault_simulation_in_unprotected_keccak_for_a_twoFault_injection_to_verify_wrong_output test PASSED")) << endl;
	
};
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_fault_simulation_in_unprotected_keccak_for_single_sa1(){

	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_fault_simulation_in_unprotected_keccak_for_single_sa1();

};
void VerifTool::test_fault_simulation_in_unprotected_keccak_for_single_sa1(){

	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2.txt", "./code_tests/testing_purpose_files/config_components_fault_Keccak_p200_singleSA1.md");

	
	assert((bin_to_hex(fM_->get_output_string()) == "8459AA1FE2BE6D20F4B3B884A0EEE2EC7740EAD6A3890FE1BB") && "Failed single gate faulted simulation");
	

	remove("./faults/evaluation_faults_Keccak_p200.pdf");
	remove("./faults/evaluation_faults_Keccak_p200.md");
	remove("./faults/nonDetected_faults_Keccak_p200.md");
	remove("./faults/ineffective_faults_Keccak_p200.md");

	cout << BOLD(FGRN("test_fault_simulation_in_unprotected_keccak_for_single_sa1 test PASSED")) << endl;
	
};
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
void test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01(){

	VerifTool vt("./code_tests/testing_purpose_files/Keccak_p200/Keccak_p200.vhd");

	vt.test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01();

};
void VerifTool::test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01(){

	read_in_file();

	read_hy_netlist();

	fM_ = new FaultsMaxine(nlHy_);

	fM_->go("./code_tests/testing_purpose_files/kecca_ins2.txt", "./code_tests/testing_purpose_files/config_components_fault_Keccak_p200_multipleSA01.md");

	
	assert((bin_to_hex(fM_->get_output_string()) == "ED9B54629C8B6CB4995A09AD4BBEEE1160B2487CC05953876D") && "Failed single gate faulted simulation");
	

	remove("./faults/evaluation_faults_Keccak_p200.pdf");
	remove("./faults/evaluation_faults_Keccak_p200.md");
	remove("./faults/nonDetected_faults_Keccak_p200.md");
	remove("./faults/ineffective_faults_Keccak_p200.md");

	cout << BOLD(FGRN("test_fault_simulation_in_unprotected_keccak_for_multilpe_sa01 test PASSED")) << endl;
	
};
///////////////////////////////////////////////////////////////////////////////////



// #else
// YOSYS tests

///////////////////////////////////////////////////////////////////////////////////
void test_yosys_netlist_create(){

	VerifTool vt("./code_tests/testing_purpose_files/test_yosys/code_hdl_models_arbiter.v");

	vt.test_yosys_netlist_create();

};
void VerifTool::test_yosys_netlist_create(){

	read_in_file_faults();

	build_yosys_tcl("./code_tests/testing_purpose_files/test_yosys/code_hdl_models_arbiter.v");

    system("./yosys/yosys synthesis.ys");
    system("yosys synthesis.ys");

    std::ifstream testFile;
    testFile.open("./netlist/yosys/arbiter_yosys.v"
    	);
    if (testFile.is_open())
    {
    	cout << BOLD(FGRN("test_yosys_netlist_create test PASSED")) << endl;
    }
    else
    {
    	cout << BOLD(FRED("test_yosys_netlist_create test FAILED")) << endl;
    	assert(false);
    }
    testFile.close();

}
///////////////////////////////////////////////////////////////////////////////////


// #endif


