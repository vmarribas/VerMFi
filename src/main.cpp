
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h> 
#include "VerifTool.h"
#include <chrono>

using namespace std::chrono;

#ifndef TEST

int main(int argc, char **argv) {
   
  if (argc < 1){
    std::cout << "Not enough files introduced" << std::endl;
  }
  else{

      VerifTool vt(argv[1]);
      size_t evaluation = 0;
      int property = 0;
      int order = 0;
      bool exit = 0;
      

  	  std::cout << "Which analysis would you like to run? Side-Channel Analysis(1), Fault Analysis(2)" << std::endl;
        std::cin >> evaluation;
//      evaluation = 2;
  	  switch(evaluation){

  		case 1:
  		{
  			std::cout << "SCA evaluation" << endl;

  			if (!vt.read_in_file()){
				std::cout << BOLD(FRED("Wrong file type entry")) << std::endl;
				return 0;
		    }
		      #ifndef YOSYS
		      vt.build_tcl();

		      // Run TCL script
		      system("dc_shell -f compile.tcl");
		      #endif
		     

  			std::cout << "Reading netlist..." << std::endl;
        
	    	if (!vt.read_netlist()){
				std::cout << BOLD(FRED("Could not read generated netlist")) << std::endl;;
				return 0;
	        }

	        std::cout << "Finished parsing netlist, circuit completed!" << endl;

	        while(!exit){

		        std::cout << "Which property would you like to verify? Subcircuits(1), Input dependencies(2), Non-completeness(3), Uniformity(4), Exit(5)" << std::endl;
		        std::cin >> property;

		        switch (property){

			      	case 1:
					{
						//Uniformity
						std::cout << "Getting Subcircuits" << std::endl;
					    vt.plot_subcircuits();
				            break;
					}

					case 2:
					{
						//Output bits dependencies from the inputs
						std::cout << "Getting Input dependencies" << std::endl;
					    vt.input_deps();
				            break;
					}

				    case 3:
					{
						//non_completeness
					    std::cout << "Evaluating Non-completeness" << std::endl;
					    std::cout << "Which order? (1, 2 or 3)" << std::endl;
					    std::cin >> order;
					    if (order == 1){
					      
					      	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //TIME
							vt.analyze_first_non_complet();
							high_resolution_clock::time_point t2 = high_resolution_clock::now(); //TIME

						    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
						    std::cout << "Time duration: " << duration << std::endl;

					    }else if (order == 2){
					    	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //TIME
					      	vt.analyze_second_non_complet();
					      	high_resolution_clock::time_point t2 = high_resolution_clock::now(); //TIME

					      	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
						    std::cout << "Time duration: " << duration << std::endl;

					    }else if (order == 3){
					    	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //TIME
					      	vt.analyze_third_non_complet();
					      	high_resolution_clock::time_point t2 = high_resolution_clock::now(); //TIME

					      	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
						    std::cout << "Time duration: " << duration << std::endl;
					    }

				            break;
					}
					    
					case 4:
					{
						//Uniformity
						std::cout << "Evaluating Uniformity" << std::endl;
						high_resolution_clock::time_point t1 = high_resolution_clock::now(); //TIME

					    vt.uniformity_check();

					    high_resolution_clock::time_point t2 = high_resolution_clock::now(); //TIME

					    auto duration = duration_cast<seconds>( t2 - t1 ).count();
						std::cout << "Time duration: " << duration << std::endl;
				            break;
					}

					case 5:
					{
						//Uniformity
						std::cout << "Exit" << std::endl;
						exit = 1;
				        break;
					}

			        default:
			        	std::cout << "No property selected" << std::endl;
		        }
			}
			break;
  		}

  		case 2:
  		{
  			if (!vt.read_in_file_faults()){
				std::cout << BOLD(FRED("Wrong input file type entry")) << std::endl;
				return 0;
		    }

  			if (!vt.read_hy_netlist()){
				std::cout << BOLD(FRED("Could not read hierarchy netlist")) << std::endl;;
				return 0;
	        }

  			vt.create_faults_maxine();

  			//Fault Simulation
			std::cout << "Fault Simulation" << std::endl;
			if (argc < 3)
			{
				std::cout << BOLD(FRED("Missing inputs file")) << std::endl;
				return 0;
			}
			else if (argc < 4)
			{
				std::cout << BOLD(FRED("Missing faults config. file")) << std::endl;
				return 0;
			} 
			else{

				vt.faults_evaluation(argv[2], argv[3]);
				
			}
			
	        break;
      	}


    }
      	
  }return 0;
}

#endif
