
#include "GFLib.h"
#include "GF2Extension.h"
#include "SubField.h"
#include <memory>
#include <utility>

///////////////////////////////////////////////////////////////////////////////

using int64 = long long;
static_assert(sizeof(int64) == 8, "int64 size is wrong");

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

GCDRes RecExtendedEuclidean(int64 a, int64 b)
{
	GCDRes res;

	if (b == 0)
	{
		res.afactor = 1;
		res.bfactor = 0;
		res.gcd = a;
		return res;
	}

	// Note that if b > a, we will just end up swapping them and recursing.

	auto q = a / b;
	auto aprime = a - q * b;

	GCDRes temp = RecExtendedEuclidean(b, aprime);

	res.gcd = temp.gcd;
	res.afactor = temp.bfactor;
	res.bfactor = temp.afactor - temp.bfactor * q;
	
	assert(a * res.afactor + b * res.bfactor == res.gcd);

	cout << "ExtendedEuclidean for " << a << " & " << b << " returns " << res.afactor << " , " << res.bfactor << " - " << res.gcd << "\n";
	return res;
}

GCDRes ExtendedEucliedean(int64 a, int64 b)
{
	assert(a != 0 && b != 0);
	int64 s = 0, old_s = 1;
	int64 t = 1, old_t = 0;
	int64 r = b, old_r = a;

	int64 prov, quotient;
#define PA(r, old_r, quotient)			\
		prov = r;						\
		r = old_r - quotient * prov;	\
		old_r = prov;

	while (r != 0)
	{
		quotient = old_r / r;
		PA(r, old_r, quotient)
		PA(s, old_s, quotient)
		PA(t, old_t, quotient)
	}
#undef PA

	GCDRes res;
	res.afactor = old_s;
	res.bfactor = old_t;
	res.gcd = old_r;

	assert(res.afactor * a + res.bfactor * b == res.gcd);

	GCDRes eres = RecExtendedEuclidean(a, b);
	assert(res == eres);

	return res;
}
void AbelienianDecomp(const int64 val, int64 ndim, const int64 *sizes, int64 *pres)
{
	for (int64 i = 0; i < ndim; i++)
		pres[i] = val % sizes[i];

	return;
}

int64 _AbelienianReverse(int64 s1, int64 s2, int64 m1, int64 m2)
{

	int64 del = m2 - m1;

	auto r = ExtendedEucliedean(s1, s2);
	assert(r.gcd == 1);

	int64 res;
	res = r.afactor * s1 * del + m1;
	
	auto p = s1 * s2;

	res = (res + p * s2) % p;
	
	assert(res >= 0 && res < s1 * s2);
	assert(res % s1 == m1);
	assert(res % s2 == m2);

	return res;
}

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

void TestDecomp(const int64 *powers)
{
	int64 prod = 1;
	int64 np;
	for (np = 0; powers[np]; np++)
	{
		prod *= powers[np];
	}
	assert(prod < 10000000000 );	// Amything larger will take a very long time.

	vector<int64> mods;
	mods.resize((unsigned int) prod);
	for (int64 i = 0; i < prod; i++)
	{
		AbelienianDecomp(i, np, powers, mods.data());
		int64 rev = AbelienianReverse(np, powers, mods.data());
		assert(i == rev);
	}
}

void GCDExp()
{
	GCDRes a = RecExtendedEuclidean(27 * 2 * 3, 83 * 83 * 3);
	GCDRes ap = ExtendedEucliedean(27 * 2 * 3, 83 * 83 * 3);
	assert(a == ap);


	int64 r, r1;
	r = EuclideanGCD(83 * 83 * 3, 27 * 2 * 3);
	r1 = EuclideanGCD(27 * 2 * 3, 83 * 83 * 3);
	assert(r == r1);

	int64 pi[] = { 8, 9, 5, 0 };
	TestDecomp(pi);

	int64 pi1[] = { 64, 83 * 83, 27, 5, 0 };
	TestDecomp(pi1);


	cout << _AbelienianReverse(4, 5, 3, 4);

}
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GCDExp();

	TestConvert();
	
/*	{
		GF2Extension ext(7);
		ext.testClass();

		cout << "GF(4) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
	}
	*/
	{
		GF2Extension ext(31);
		ext.testClass();

		cout << "GF("<< ext.size() << ") with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
		cout << ext.makeGeneratorList();
		cout << ext.makeGaloisData();

		for (int p = 2; p <= ext.size(); p *= 2)
		{
			for (uint i = 0; i < ext.size(); i++)
			{
				ext.normFromGalois(i, p);
			}
		}


		/*
		cout << "Subfields generated by a single generator:\n";
		vector<unique_ptr<Subfield>> afields;
		for (uint i = 1; i < ext.size(); i++)
		{
			Subfield *psf = new Subfield(&ext);
			psf->addElement(i);
			psf->generateField();

			bool found = false;
			for (size_t j = 0; j < afields.size(); j++)
			{
				if (afields[j]->isIdentical(psf))
				{ 
					found = true;
					break;
				}
			}
			if (!found)
				afields.emplace_back(psf);
			else
				delete psf;

		}

		for (size_t j = 0; j < afields.size(); j++)
		{
			afields[j]->dump();

		}
		*/
	}


	return 0;
}

