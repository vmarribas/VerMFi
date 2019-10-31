#include "VerifTool.h"

void VerifTool::analyze_first_non_complet(){
	
	if (nl_.sequential_)
	{
		sequential_first_non_complet(outsSpecified_);
	}
	else
	{
		first_non_complet(outsSpecified_);
	}
}