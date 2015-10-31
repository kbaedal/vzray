#include "rgb.h"

void RGB::clamp(float min_val, float max_val)
{
	if(R < min_val)
		R = min_val;
	else if(R > max_val)
		R = max_val;

	if(G < min_val)
		G = min_val;
	else if(G > max_val)
		G = max_val;

	if(B < min_val)
		B = min_val;
	else if(B > max_val)
		B = max_val;
}

std::ostream& operator<<(std::ostream &os, const RGB &c)
{
	os << '<' << c.r() << ", " << c.g() << ", " << c.b() << '>';

	return os;
}
