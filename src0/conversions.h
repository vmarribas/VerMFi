#ifndef binToDec_H
#define binToDec_H

#include <iostream>
#include <stdio.h> 
#include <stdint.h>     
#include <stdlib.h> 
#include <math.h>

bool charToBool(char s);

uint64_t binToDec(std::string bin_num);

std::string decToBin(size_t dec, size_t size);

std::string bin_to_hex(std::string binNum);

std::string hex_to_bin(std::string hexNum);

#endif