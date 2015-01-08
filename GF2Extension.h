
// GF2Extension.h

#pragma once

#include <vector>

class GF2Extension
{

	static const int maxBits = sizeof(uint) * 8;
	static const char displayVar = 'x';

	uint nPolynomial;
	int  nIndex;

	// Array if reciprocals of each non--zero element, starting with 1:
	vector<uint> reciprocalArray;

public:

	GF2Extension();
	GF2Extension(uint poly);
	GF2Extension(const char *ppoly); //poly def in binary.

	bool create(uint poly);
	void clear();

	uint poly() const { return nPolynomial; }
	int size() const { return 1 << nIndex;  }


	uint add(uint a, uint b) const;
	uint subtract(uint a, uint b) const;
	uint multiply(uint a, uint b) const;
	uint power(uint a, int exponent) const;
	uint reciprocal(uint a) const;
	uint divide(uint dividend, uint divisor) const;
	int order(uint a) const;	// Return the order of a alement - lowest  so that a^n = 1

	uint normFromGalois(uint, int galois_gen_power) const;


	string makeMultiplicationTable() const;
	string makeGeneratorList() const;
	string makeGaloisData();

	// Static methods.
	static uint _add(uint a, uint b);
	static uint _subtract(uint a, uint b);
	static uint _multiply(uint a, uint b);

	struct DivRes
	{
		uint quotient;
		uint remainder;
	};

	static DivRes _divide(uint dividend, uint divisor);
	struct GCDRes
	{
		uint afactor;
		uint bfactor;
		uint gcd;
	};
	// Uses the extended Euclindean algorithm to solve:
	// a * afactor + b * bfactor = gcd
	static GCDRes extendedEucliedean(uint a, uint b);


	// Output
	static string format(uint val);
	static void dumpPoly(uint val);

	// Testing
	static void testClass();
};
