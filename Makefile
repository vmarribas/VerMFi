
##################################################################################

## COMPANY:		COSIC, KU Leuven 
## AUTHOR:		Victor Arribas

##################################################################################

## BSD 3-Clause License

## Copyright (c) 2019, Victor Arribas
## All rights reserved.

## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:

## 1. Redistributions of source code must retain the above copyright notice, this
##    list of conditions and the following disclaimer.

## 2. Redistributions in binary form must reproduce the above copyright notice,
##    this list of conditions and the following disclaimer in the documentation
##    and/or other materials provided with the distribution.

## 3. Neither the name of the copyright holder nor the names of its
##    contributors may be used to endorse or promote products derived from
##    this software without specific prior written permission.

## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
## DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
## DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
## SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
## CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
## OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

##################################################################################


CC=g++
CFLAGS=-std=c++11 -Wall -Wno-format -g -O3

vt:
	$(CC) $(CFLAGS) -o verif_tool ./src/*.cpp

fp:
	$(CC) $(CFLAGS) -o prepr_faults ./src/ParseFunctions.cpp ./src/create_hy_netlist.cpp ./src/GatesFactory.cpp ./src/Gates.cpp ./src/SuperGate.cpp ./src/gate_.cpp ./src/pin_.cpp ./src/read_in_file_faults.cpp ./src/wire_.cpp ./src/get_ff_inouts.cpp ./src/eval_wire.cpp ./src0/*.cpp

debug:
	$(CC) $(CFLAGS) -DVERBOSE -o verif_tool ./src/*.cpp
yss:
	$(CC) $(CFLAGS) -DYOSYS -o verif_tool ./src/*.cpp
	$(CC) $(CFLAGS) -DYOSYS -o prepr_faults ./src/ParseFunctions.cpp ./src/create_hy_netlist.cpp ./src/GatesFactory.cpp ./src/Gates.cpp ./src/SuperGate.cpp ./src/gate_.cpp ./src/pin_.cpp ./src/read_in_file_faults.cpp ./src/wire_.cpp ./src/get_ff_inouts.cpp ./src/eval_wire.cpp ./src0/*.cpp

test:
	$(CC) $(CFLAGS) -DTEST -o ./code_tests/tool_tests ./src/*.cpp ./code_tests/*.cpp ./src0/build_yosys_tcl.cpp
	./code_tests/tool_tests
	rm code_tests/tool_tests
	rm synthesis.ys

all:
	$(CC) $(CFLAGS) -o verif_tool ./src/*.cpp
	$(CC) $(CFLAGS) -o prepr_faults ./src/ParseFunctions.cpp ./src/create_hy_netlist.cpp ./src/GatesFactory.cpp ./src/Gates.cpp ./src/SuperGate.cpp ./src/gate_.cpp ./src/pin_.cpp ./src/read_in_file_faults.cpp ./src/wire_.cpp ./src/get_ff_inouts.cpp ./src/eval_wire.cpp ./src0/*.cpp
clean:
	rm -rf *.o verif_too*
	rm -rf compile.tcl
	rm -rf complete_bits_*
	rm -rf dependencies.txt
	rm -rf prepr_faults*
	rm -rf subcircuits*
	rm -rf synthesis.ys
	rm -rf uniformity.txt



