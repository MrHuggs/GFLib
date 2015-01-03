
// GF2Extension.h

#pragma once

class GF2Extension
{

	static const int maxBits = sizeof(uint) * 8;
	static const char displayVar = 'x';

	uint nPolynomial;
	int  nIndex;

public:

	GF2Extension(uint poly);

	uint poly() const { return nPolynomial; }

	struct DivRes
	{
		uint quotient;
		uint remainder;
	};

	uint add(uint a, uint b);
	uint subtract(uint a, uint b);
	uint multiply(uint a, uint b);
	uint power(uint a, int exponent);
	DivRes divide(uint dividend, uint divisor);

	string makeMultiplicationTable();

	// Static methods.
	static uint _add(uint a, uint b);
	static uint _subtract(uint a, uint b);
	static uint _multiply(uint a, uint b);
	static DivRes _divide(uint dividend, uint divisor);

	// Output
	static string format(uint val);
	static void dump(uint val);

	// Testing
	static void testClass();
};
