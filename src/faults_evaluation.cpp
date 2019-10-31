
#include "VerifTool.h"
#include "FaultsMaxine.h"

void VerifTool::create_faults_maxine(){

	//Fault simulation with inputs specified form the user
	fM_ = new FaultsMaxine(nlHy_);

	//Pass the check bits gotten from input file
	fM_->get_checkBits(checkBits_);

	srand(time(NULL));

};


void VerifTool::faults_evaluation(std::string inputsSimFile, std::string faultsConfigFile){

	fM_->go(inputsSimFile, faultsConfigFile);

}
