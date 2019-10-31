#include "VerifTool.h"

void VerifTool::analyze_third_non_complet(){
	
	if (nl_.sequential_)
	{
		sequential_third_non_complet(outsSpecified_);
	}
	else
	{
		third_non_complet(outsSpecified_);
	}
}