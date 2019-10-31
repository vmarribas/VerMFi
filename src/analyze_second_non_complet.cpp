#include "VerifTool.h"

void VerifTool::analyze_second_non_complet(){
	
	if (nl_.sequential_)
	{
		sequential_second_non_complet(outsSpecified_);
	}
	else
	{
		second_non_complet(outsSpecified_);
	}
}