 // © 2020 Marcin Witek 


#include "EdibleInterface.h"

// Add default functionality here for any IEdibleInterface functions that are not pure virtual.
 float IEdibleInterface::GetHungerSatisfactionValue()
 {
	return 0.f;
 }

 void IEdibleInterface::GetBitten(float BiteStrength)
 {
 	return;
 }
