#include "VerifTool.h"
#include "conversions.h"

using namespace std;

bool VerifTool::uniformity_check(){


  std::vector<std::string> testVectors;
  std::ofstream uniform_file("uniformity.txt");
  uniform_file << "Output vector in unsigned integer => frequency of occurence of the output" << std::endl;
  uniform_file << std::endl;
  
  int variables = (int)nl_.sharedVariables_.size();
  int shares = (int)nl_.sharedVariables_[0].size();
  uint64_t totalVectors = (uint64_t)pow(2.0, (double)(variables*shares + nl_.randomBits_.size()));
  uint64_t totalOuts    = (uint64_t)pow(2.0, (double)(nl_.outs_.size()));
  uint64_t outsToStoreFreq = totalOuts / (uint64_t)pow(2.0, (double)(nl_.outs_.size()/shares)); //When fixing input shares there will be 
  // some outs that we will not hit, i.e. the freq will be zero. We do not allocate space for these zeros. 
  uint64_t unshared_inputs = (uint64_t)pow(2.0, (double)(variables)); //Number of unshared inputs, all variables considered together
  bool uniform = 1;

  
  for (uint i = 0; i < unshared_inputs; ++i)
  {
    std::vector<int> out_freqs(outsToStoreFreq,0);
    uniform_file << "Unshared input: " << i << endl;
    for (size_t j = 0; j < totalVectors; ++j)
    {
      //Test vector
      std::string testVector = decToBin(j, variables*shares + nl_.randomBits_.size());

      //Unshare test vector
      uint unshared_value = 0;
      std::string unshared_value_bin = "";
      int l = 0;

      for(int k = 0; k < variables; k++){
        uint unshared_val = 0; //Unshare k variable
        for(int t = 0; t < shares; t++){ 
          unshared_val ^= (testVector[l] == '0' ? 0:1);
          ++l;
        }
        unshared_value_bin += decToBin(unshared_val,1);
      }
      unshared_value = (uint)binToDec(unshared_value_bin);

      //If the test vecotr corresponds to this unshared input(i)
      if (unshared_value == i)
      {
        // cout << "Test vector: " << testVector << endl;
        // Perform simulation
        //Give values to inputs
        std::string out_vector = "";
        gates eventList;
        int m = 0;
        for(int k = 0; k < variables; k++){
          for(int t = 0; t < shares; t++){ 
            // std::cout << "What is going: " << testVector[l] << " From index " << l << std::endl;
            nl_.sharedVariables_[k][t]->eval_wire(charToBool(testVector[m]), eventList);
            // std::cout << nl_.sharedVariables_[k][j]->name << " => " << nl_.sharedVariables_[k][j]->value << std::endl;
            ++m;
          }
        }
        //Give values to ran bits
        for(size_t t = 0; t < nl_.randomBits_.size(); t++){
          // std::cout << "What is going: " << testVector[l] << " From index " << l << std::endl;
          nl_.randomBits_[t]->eval_wire(charToBool(testVector[m]), eventList);
          // std::cout << nl_.randomBits_[j]->name << " => " << nl_.randomBits_[j]->value << std::endl;
          ++m;
        }
        //Give values to public values
        for(size_t t = 0; t < nl_.publicInputs_.size(); t++){
          // std::cout << "What is going: " << testVector[l] << " From index " << l << std::endl;
          nl_.publicInputs_[t]->eval_wire(0, eventList);
          // std::cout << nl_.publicInputs_[j]->name << " => " << nl_.publicInputs_[j]->value << std::endl;
        }
        // std::cout << "Event list size: " << eventList.size() << std::endl;
        //Evaluate all gates
        while(!eventList.empty())
        {
          // for (size_t p = 0; p < eventList.size(); ++p)
          // {
          //   std::cout << eventList[p]->get_name() << " ";
          // }
          // std::cout << std::endl;
          eventList[0]->eval_gate(eventList);
          if(!eventList[0]->get_output()->get_wire()->get_is_evaluated()) eventList.push_back(eventList[0]);
          eventList.erase(eventList.begin());
          // std::cout << "Still " << eventList.size() << " components_ in the Event list" << std::endl;
        }
        //Simulation finished

        for (size_t k = 0; k < nl_.outs_.size(); ++k){
          out_vector += std::to_string((int)nl_.outs_[k]->get_value());
        }
        out_freqs[binToDec(out_vector) % outsToStoreFreq] += 1;
        // cout << "Out: " << out_vector << " with freq already: " << out_freqs[binToDec(out_vector) % outsToStoreFreq] << endl;
        //Reset value of the wires to zero and non-evaluated status
        // de_evaluate_wires();
      }
    }

    for (size_t gg = 0; gg < out_freqs.size(); ++gg)
    {
      uniform_file << "Output % " << outsToStoreFreq << " = " << gg << " has freq: " << out_freqs[gg] << endl;
    }
    // cout << "Are all of them equal? => " << !(std::adjacent_find(out_freqs.begin(), out_freqs.end(), std::not_equal_to<int>()) != out_freqs.end()) << endl;

    //Check uniformity for this unshared input
    if (std::adjacent_find(out_freqs.begin(), out_freqs.end(), std::not_equal_to<int>() ) != out_freqs.end() )
    {
        uniform = 0;
    }

  }


  if (uniform)
  {
    std::cout << BOLD(FGRN("Your program is Uniform! Check file \"uniformity.txt\" to see the output frequencies")) << std::endl;
  }else{
    std::cout << BOLD(FRED("Your program is not Uniform. Check file \"uniformity.txt\" to see the output frequencies")) << std::endl;
  }

  // for (size_t i = 0; i < out_freqs.size(); ++i)
  // {
  //   std::cout << out_freqs[i] << std::endl;
  // }
  return 1;
}
