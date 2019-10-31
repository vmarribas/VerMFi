
////////////////////////////////////////////////////////////////////////////////

// COMPANY:   COSIC, KU Leuven 
// AUTHOR:    Victor Arribas

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



// bool VerifTool::eval_uniformity(std::vector<std::vector<int>> outs_grouped, std::vector<int> freqs){

	// std::ofstream uniform_file("uniformity.txt");
	
	// bool uniform = 1;

	// //Write all non-zero frequencies in the file
 //  	uniform_file << "Output vector in unsigned integer => frequency of occurence of the output" << std::endl;
 //  	uniform_file << std::endl;

 //    for (size_t i = 0; i < outs_grouped.size(); ++i)
 //    {
 //      int frequency = 0;
 //      uniform_file << "Frequencies when the unshared input is: " << decToBin(i,sharedVariables.size()) << " with number of outputs: " <<  outs_grouped[i].size() << std::endl;
 //      for (size_t j = 0; j < outs_grouped[i].size(); ++j)
 //      {
 //        uniform_file << outs_grouped[i][j] << " => " << freqs[outs_grouped[i][j]] << std::endl;

 //        if (frequency == 0)
 //        {
 //          frequency = freqs[outs_grouped[i][j]];
 //        }else if (frequency != freqs[outs_grouped[i][j]])
 //        {
 //          uniform = 0;
 //        }
 //      }
 //    }
  	// for (size_t i = 0; i < v.size(); ++i)
  	// {
   //  	if (v[i] != 0)
   //  	{
   //    		uniform_file << i << " => " << v[i] << std::endl;

   //    		if (frequency == 0)
   //    		{
   //    			frequency = v[i];
   //    		}else if (frequency != v[i])
   //    		{
   //    			uniform = 0;
   //    		}
   // 		}
  	// }

  	// return uniform;
  // return 0;
// }