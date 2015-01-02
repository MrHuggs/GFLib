
#include "GFLib.h"


///////////////////////////////////////////////////////////////////////////////
int HighBit(uint val)
{
	int i;
	for (i = sizeof(uint) * 8 - 1; i >= 0; i--)
	{
		if (val & (1 << i))
			break;
	}
	return i;
}
///////////////////////////////////////////////////////////////////////////////
int CountBits(uint val)
{
	int i = 0;
	while (val)
	{
		if (val & 1)
			i++;
		val = val >> 1;
	}
	return i;
}
///////////////////////////////////////////////////////////////////////////////
string UintToStr(uint val)
{
	if (val == 0)
		return "0";

	stringstream ss;
	int i = HighBit(val);

	ss << '1';
	i--;
	while (i >= 0)
	{
		char c = (val & (1 << i)) ? '1' : '0';
		ss << c;
		i--;
	}

	return ss.str();
}
///////////////////////////////////////////////////////////////////////////////
uint StrToUint(const char *pc)
{
	uint res = 0;
	char c;
	while (c = *pc++)
	{
		res = res << 1;
		assert(c == '1' || c == '0');
		if (c == '1')
			res |= 1;
	}
	return res;
}
///////////////////////////////////////////////////////////////////////////////
void TestConvert()
{
	const char *ps1 = "0010010";
	auto v1 = StrToUint(ps1);
	assert(v1 == 18);

	ps1 = "10010";
	v1 = StrToUint(ps1);
	assert(v1 == 18);
	auto s1 = UintToStr(v1);
	assert(s1.compare(ps1) == 0);
}
