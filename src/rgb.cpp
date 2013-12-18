#include "rgb.h"

void RGB::clamp(float dMin, float dMax)
{
	if(R < dMin)
		R = dMin;
	else if(R > dMax)
		R = dMax;
		
	if(G < dMin)
		G = dMin;
	else if(G > dMax)
		G = dMax;

	if(B < dMin)
		B = dMin;
	else if(B > dMax)
		B = dMax;
}

std::ostream& operator<<(std::ostream &os, const RGB &c)
{
	os << '<' << c.r() << ", " << c.g() << ", " << c.b() << '>';
	
	return os;
}
