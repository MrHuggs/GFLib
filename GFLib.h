// GFLib.h

#include <string>
#include <iostream>     // std::cout
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <sstream>      // std::stringstream

#undef assert
#define assert(x) { if (!(x)) { _asm int 3 } }

using namespace std;

using uint = unsigned _int64;
using int64 = long long;

// Utilities
int HighBit(uint val);
int CountBits(uint val);
string UintToStr(uint val);
uint StrToUint(const char *pc);
void TestConvert();

int64 EuclideanGCD(int64 a, int64 b);

struct GCDRes
{
	int64 afactor;
	int64 bfactor;
	int64 gcd;

	bool operator ==(const GCDRes &other)
	{
		return afactor == other.afactor && bfactor == other.bfactor && gcd == other.gcd;
	}
};

// Given a, b, find the gcd(a,b) and numbers so that afactor * a + bfactor * b = gcd
GCDRes ExtendedEucliedean(int64 a, int64 b);

// Given an element a cyclic group, and the prime factorization of the order of the group,
// find the decompostion into the director product of cyclic groups of prime power order.
void AbelienianDecomp(const int64 val, int64 ndim, const int64 *sizes, int64 *pres);

// Given elements of groups of prime power order, find the value in the corresponding 
// cyclic group.
int64 AbelienianReverse(int64 ndim, const int64 *powers, const int64 *mods);


void TestGFUtils()
#ifdef NDEBUG
{}
#endif
;