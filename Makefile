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