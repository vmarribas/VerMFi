# VerMFi
## Verification tool for Masked implementations and Fault injection

This tool allows you to perform Side-Channel and Fault Analysis evaluations. The first part, VerMI, verifies whether your algorithm fulfills  the Threshold Implementation properties, i.e. Non-completeness and Uniformity. The second part of the tool performs fault simulation to evaluate the resistance of your design against faults. The tool supports random or crafted fault injections with extreme flexibility, allowing it to operate in multiple different fault models.


## Requirements

* C++11 standard to compile the code  
* Synopsys Design Compiler to generate the netlist  
* Alternatively: open source YOSYS: refer to Installing_YOSYS.md for more information  
  
  

## Compilation

To compile the code, use the attached Bash file "run". 
This file will create several folders needed for the synthesis and optionally 
source your DC Compiler if you add the correct path to the "run" file. It will 
also call "make" directly to compile the program.

Compile:  
	`$ source run` (this initializes all the folders, by default already included) or  
  `$ make all`

To execute VerMI:  
​	`$ ./verif_tool $top_module_file`  

To execute VerFI:
* Preprocessing (generate config. files): `$ ./prepr_faults $top_module_file`  
* Fault evaluation: `$ ./verif_tool $top_module_file $inputs_file $fault_config_file`  

Other options:
* Compile only VerMI/VerFI: `make`  
* Compile only preprocessing stage: `make fp`  
* If you want to use Yosys (only for VerFI): `make yss`  
* To run the code tests: `make test`  
* To clean created results files: `make clean`  


## YOSYS
Include all the files you want to synthesize in the destination folder within "/test_files".  
Do not include any sub-directories. Do not include any test-bench.  
Yosys for SCA analysis is not yet supported in this version.

To get Yosys working, clone source code from GIT repository, in the main path of the tool:
```
$ git clone https://github.com/cliffordwolf/yosys.git
```
To install:
##### Linux 
```
$ sudo apt-get install build-essential clang bison flex \
	  libreadline-dev gawk tcl-dev libffi-dev git \
	  graphviz xdot pkg-config python3 libboost-system-dev \
	  libboost-python-dev libboost-filesystem-dev

```
Configure build and/or change build settings in Makefile:
```
$ make config-clang
$ vi Makefile
$ vi Makefile.conf
```
Build, test and install:
```
$ make
$ make test
$ sudo make install
```
NOTE: the last step, `sudo make install` is not needed if you do not have superuser rights.

##### Mac OS X
Install with Hombrew (alternatives in original Yosys Git)
```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

$ brew tap Homebrew/bundle && brew bundle

$ sudo port install bison flex readline gawk libffi \
	  git graphviz pkgconfig python36 boost
```
```
$ brew install yosys
```
  
For more information, you can check the Yosys git page.

!! NOTE: when analyzing a design with Yosys, include only the files to analyze in the target directory WITHOUT any testbench or subdirectories


## Examples

Several examples are included in the Git, to show how VerMI and VerFI work:

##### SCA evaluation with VerMI

Evaluate insecure 2nd-order DOM AND gate:  
```
$ ./verif_tool test_files/examples/AND/and_dom2_insecure.vhd
```

Evaluate secure 2nd-order DOM AND gate:  
```
$ ./verif_tool test_files/examples/AND/and_dom2.vhd
```
![VerMI](https://github.com/vmarribas/VerMFi/blob/master/VerMI_gif2.gif)

##### FA evaluation with VerFI

Evaluate an unprotected Keccak implementation (preprocessing not needed since netlist is provided already):  
```
$ ./verif_tool test_files/examples/Keccak_p200/Keccak_p200.vhd inputs/kecca_ins2randIns.txt faults/faults_config_Keccak_p200_test.md
```

There are several more faults comfiguration files you can play with to inject different faults. VHDL code running simulations with the same fault injection are included in ./test_files/examples/Keccak_p200/vhd_* so that you can compare the output of VerFI with actual HDL simulation.  

Fault simulation using Yosys is also included. We have included a simple uprotected Arbiter HDL module:  
```
$ make yss
$ ./prepr_faults test_files/examples/Arbiter/code_hdl_models_arbiter.v
$ ./verif_tool test_files/examples/Arbiter/code_hdl_models_arbiter.v inputs/arbiter.md faults/config_faults_arbiter.md
```

![VerFI](https://github.com/vmarribas/VerMFi/blob/master/VerFI_gif.gif)

## How does it work

* The tool supports VHDL (not with Yosys) and Verilog files.
* Copy the files to analyze to the folder "/test_files", preferably separating different files 
for different tests in different directories inside "/test_files" (the tool will synthesize all 
the files within the same directory as the provided TOP module).
* When executing the tool, the ONLY file you have to specify as argument is the TOP MODULE. 
The tool will synthesize all needed files (as soon as all of them are in the same directory). 
Your input file must contain either: only your top module or if more modules 
are included in the given file, the top module must be declared first.  
* The tool will output several files: analyze and compile processes logfiles ("logfiles" folder), 
where you can see if the files are analyzed correctly and if the compilation went well; 
the generated netlist in verilog ("netlist"), and several reports ("reports"), 
including area among others. By looking at these files you can debug errors in your HDL.
* It generates a TCL script "compile.tcl", where you can find the full setup to perform the synthesis.
* Once the tool is started you can select either Side-Channel evaluation (1) or Fault Injection (2).  


## Header 

In order to perform the check, we need to pass the tool some information, including 
sensitive data, randomness, check bits, ...  

The header structure should have the next format:
```
-- Input_shares: sharedInVariable_1_1, sharedInVariable_1_2, ..., sharedInVariable_1_numberOfInShares; ...; 
sharedInVariable_numberOfVariables_1, sharedInVariable_numberOfVariables_2, ...,  
sharedInVariable_numberOfVariable_numberOfInShare.  
-- Random_vars: randomVar_1, ..., randomVar_N.  
-- Regs_layer: layer_1_InVariable_1_1, layer_1_InVariable_1_2, ..., layer_1_InVariable_1_numberOfInShares; ...; 
layer_1_InVariable_numberOfVariables_1, layer_1_InVariable_numberOfVariables_2, ...,  
layer_1_InVariable_numberOfVariable_numberOfInShare.  
-- Regs_layer: layer_2... .  
-- Regs_layer: layer_TotalNumbOfPipelineRegs... .  
-- Check_bits: checkVar_1, ..., checkVar_2.  
```

You can also optionally include which outputs to evaluate. Include this only 
if there is an output in your module you do not it to be analyzed (the tool will 
analyze the specified outputs only):  
```
-- Outputs_check: outputToCheck_1, ..., outputToCheck_N.  
```


!! Make sure you do not forget the last dot, otherwise you will get "Segmentation fault".  
!! Inputs should go in the same order as in the module declaration.  
!! VerMI will only take into account the info. until Check_bits (not included), and VerFI will only read Check_bits.  
!! Several things to take into account when specifying Registers Layers:
  * You will specify the signal that goes out of your register.  
  * Synopsys (Yosys) might not use this specific name if for example you have an immediate buffer or a permutation.  
  * Normally, if the tool does not find your specified name in the output wire, it will look at the register name and save the signal being used in that register (usually the registers are name as your output signal).  
  * VerMI sets don_touch constraints on the signals you specified only up to 5 levels of hierarchy. If the name is not preserved and the tool identifies the name of the register, it might be that a signal with the name "nX" (signals created by the synthesizer) is found. Several constrains are specified that may not be used, and so Synopsys throws an Error while synthesizing. this does not harm the synthesis process, so do not mind these errors.  
  * If the tool does not find the name you specified in the register output and neither it finds it in the name of the register, an error will be shown informing about the problematic signal.
  * Only include sensitive data from registers, do not include control signals.  

!! Check_bits signal does not need to be an output, you can select any intermediate wire. Note that, if you select an intermediate wire, the synthesizer might change the name of this wire and VerFI will not get which signal to look at.  


Example:
8-bit AND gate first-order secure, with two shares and one layer of registers:
```
input [7:0] x1, x2;  
input [7:0] y1, y2;  
input [7:0] r;  
output[7:0] z1, z2;  
```
Header:
```
-- Input_shares: x1, x2; y1, y2. (Separate variables with ";", do not forget the "." at the end)
-- Random_vars: r.  
-- Regs_layer: crossTerm1_reg, crossTerm2_reg, crossTerm3_reg, crossTerm4_reg.  
```


## VerMI
* Part of the tool performing Side-Channel Analysis.  
* Important, add the header with the sensitive and the random variables as explained above.
* The tool produces several files, a different one for each different test and evaluation. The 
tool points you to the adequate file when finished the given evaluation.


## VerFI
* Part of the tool performing Fault Analysis.  
* The second part of the tool runs in two steps, first preprocessing, to generate necessary configuration
files, and then the actual analysis. You can see how to execute VerFI in the Compilation section.
* You can decide how many traces to evaluate in the form of input test vectors by defining them your self 
or by letting the tool feed random inputs. The tool will repeat the same analysis for all the input test vectors.
* For the tool to be able to perform the simulation, only certain signals are recognized as control signals:  
```
"clk", "rst", "load", "start" as inputs 
"done" OR "ready" as output. Check_bits can be any wire, specified in the header.
```
Please, make sure your control signals are spelled in the same way (capital sensitive). Your 
framework can have any combination of these signals, or none of them, but no more than those. Other 
inputs will be taken as regular inputs, to which value should be given in the input test vectors. The 
output will similarly include all outputs except the ready signal and the check bits (if these were 
output wires). Some more clues can be given to the simulator by means of the Inputs file.
* The ciphertext output name should NEVER include the term "ready" or "done" in the name.
* You have two means of fault injection: hierarchical or component-wise injection. The first one 
is a more high-level injection to perform random fault injection on blocks of the design, while 
the second one is for more precise faults or to perform exhaustive fault simulation. You can 
find more information in the configuration files.


## Not Supported (yet!)
* VerMI does not support yet the use of Yosys
* VerMI does not support multivariate security at higher orders, it only provides univariate assesment
* VerFI: clock cycles range selection is not fully working in the componont-wise fault injection
* VerFI: debug option for Yosys not supported


## Citation

If you use this tool for your work and you will publish it as a paper, you can cite the tool as follows.  
If you performed a Side-Channel evaluation, you can cite VerMI:  
BibTeX entry for LaTeX  
```bibtex
@inproceedings{DBLP:conf/icecsys/ArribasNR18,
  author    = {Victor Arribas and
               Svetla Nikova and
               Vincent Rijmen},
  title     = {VerMI: Verification Tool for Masked Implementations},
  booktitle = {{ICECS}},
  pages     = {381--384},
  publisher = {{IEEE}},
  year      = {2018}
}
```
Or  
> V. Arribas, S. Nikova, and V. Rijmen, “VerMI: Verification Tool for Masked Implementations,” in 25th IEEE International Conference on Electronics, Circuits, and Systems. Bordeaux,FR: IEEE, 2018, p. 4.

If you performed a fault evaluation, you can cite VerFI:  
```bibtex
comming soon
```

And finally, if you used both, you can cite both :)


