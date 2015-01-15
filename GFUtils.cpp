
#include "GFLib.h"
#include <memory>
#include <utility>
#include <vector>
///////////////////////////////////////////////////////////////////////////////
int HighBit(uint val)
{
	int i;
	for (i = sizeof(uint) * 8 - 1; i >= 0; i--)
	{
		if (val & ((uint) 1 << i))
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
		char c = (val & ((uint) 1 << i)) ? '1' : '0';
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
int64 EuclideanGCD(int64 a, int64 b)
{
	// Note that this works even if b > a, as the first iteration will just end
	// up swapping the two.
	while (b != 0)
	{
		auto t = a % b;
		a = b;
		b = t;
	}
	return a;
}

GCDRes ExtendedEucliedean(int64 a, int64 b)
{
	struct PartialEE { int64 s, t, r; };

	PartialEE prev, cur, next;

	prev.s = 1;
	prev.t = 0;
	prev.r = a;

	cur.s = 0;
	cur.t = 1;
	cur.r = b;

	while (cur.r != 0)
	{
		auto q = prev.r / cur.r;

		next.s = prev.s - cur.s * q;
		next.t = prev.t - cur.t * q;
		next.r = prev.r - cur.r * q;

		prev = cur;
		cur = next;
	}

	GCDRes res;
	res.afactor = prev.s;
	res.bfactor = prev.t;
	res.gcd = prev.r;

	assert(res.afactor * a + res.bfactor * b == res.gcd);
	return res;
}
///////////////////////////////////////////////////////////////////////////////
void AbelienianDecomp(const int64 val, int64 ndim, const int64 *sizes, int64 *pres)
{
	for (int64 i = 0; i < ndim; i++)
		pres[i] = val % sizes[i];

	return;
}
///////////////////////////////////////////////////////////////////////////////
int64 _AbelienianReverse(int64 s1, int64 s2, int64 m1, int64 m2)
{

	int64 del = m2 - m1;

	auto r = ExtendedEucliedean(s1, s2);
	assert(r.gcd == 1);

	int64 res;
	res = r.afactor * s1 * del + m1;

	// We want the result modulo p. Unfortunately, the CRT rounds towards
	// 0, not down.

	auto p = s1 * s2;
	if (res < 0)
	{
		auto t = res % p;
		res = t ? p + t : t;
	}
	else
		res = res % p;

	assert(res >= 0 && res < s1 * s2);
	assert(res % s1 == m1);
	assert(res % s2 == m2);

	return res;
}
///////////////////////////////////////////////////////////////////////////////
int64 AbelienianReverse(int64 ndim, const int64 *powers, const int64 *mods)
{
	assert(ndim >= 2);
	int64 cp = powers[0];
	int64 cm = mods[0];

	for (int64 i = 1; i < ndim; i++)
	{
		cm = _AbelienianReverse(cp, powers[i], cm, mods[i]);
		cp *= powers[i];
	}
	return cm;
}
///////////////////////////////////////////////////////////////////////////////
#ifndef NDEBUG
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
///////////////////////////////////////////////////////////////////////////////
void TestDecomp(const int64 *powers)
{
	int64 prod = 1;
	int64 np;
	for (np = 0; powers[np]; np++)
	{
		prod *= powers[np];
	}
	assert(prod < 10000000000);	// Amything larger will take a very long time.

	vector<int64> mods;
	mods.resize((unsigned int)prod);
	for (int64 i = 0; i < prod; i++)
	{
		AbelienianDecomp(i, np, powers, mods.data());
		int64 rev = AbelienianReverse(np, powers, mods.data());
		assert(i == rev);
	}
}
///////////////////////////////////////////////////////////////////////////////
void TestGCDExp()
{
	GCDRes ap;

	ap = ExtendedEucliedean(27 * 2 * 3, 83 * 83 * 3);
	ap = ExtendedEucliedean(-27 * 2 * 3, 83 * 83 * 3);
	ap = ExtendedEucliedean(-27 * 2 * 3, -83 * 83 * 3);
	ap = ExtendedEucliedean(27 * 2 * 3, -83 * 83 * 3);

	int64 r, r1;
	r = EuclideanGCD(83 * 83 * 3, 27 * 2 * 3);
	r1 = EuclideanGCD(27 * 2 * 3, 83 * 83 * 3);
	assert(r == r1);

	int64 pi[] = { 8, 9, 5, 0 };
	TestDecomp(pi);

	int64 pi1[] = { 4, 83 * 83, 9, 5, 0 };
	TestDecomp(pi1);


	//cout << _AbelienianReverse(4, 5, 3, 4);

}
///////////////////////////////////////////////////////////////////////////////

void TestGFUtils()
{
	TestConvert();
	TestGCDExp();

}
#endif
