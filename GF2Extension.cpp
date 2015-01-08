// GF2Extension.cpp

#include "GFLib.h"
#include "GF2Extension.h"
#include <memory>

///////////////////////////////////////////////////////////////////////////////
GF2Extension::GF2Extension()
{
	nPolynomial = 0;
	nIndex = 0;
}
///////////////////////////////////////////////////////////////////////////////
GF2Extension::GF2Extension(uint poly) : GF2Extension()
{
	auto b = create(poly);
	assert(b);
}
///////////////////////////////////////////////////////////////////////////////
GF2Extension::GF2Extension(const char *ppoly) : GF2Extension()
{
	auto b = create(StrToUint(ppoly));
	assert(b);
}
///////////////////////////////////////////////////////////////////////////////
bool GF2Extension::create(uint poly)
{
	assert(nPolynomial == 0); // Shouldn't be initialized twice.

	// Two simple tests for reducibility:
	if (!(CountBits(poly) & 1))
		return false;
	if (!(poly & 1))
		return false;

	nPolynomial = poly;				// Need to call clear() if we fail after this point.
	nIndex = HighBit(poly);			// Extension index is the degree of the irreducible poly.

	reciprocalArray.resize(size());
	reciprocalArray[0] = 1;

	/*
	unique_ptr<uint[]> power_array(new uint[size()]);
	uint generator = size() - 1;
	power_array[0] = 1;
	power_array[1] = generator;

	for (uint i = 2;; i++)
	{
		assert(i < (uint) size());
		auto np = multiply(generator, power_array[i - 1]);

		power_array[i] = np;
		cout << i << "  " << np << "\n";

		if (power_array[i] == 1)
		{
			if (i < generator)
			{
				clear();
				return false;
			}
			break;
		}
	}


	for (int i = 1; i < size(); i++)
	{
		uint val = power_array[i];
		uint inv = power_array[generator - i];
		reciprocalArray[val-1] = inv;

#ifndef NDEBUG
		GCDRes res = extendedEucliedean(val, nPolynomial);
		assert(res.afactor == inv);
#endif
	}*/

	for (uint i = 1; i < (uint) size(); i++)
	{
		GCDRes res = extendedEucliedean(i, nPolynomial);
		if (res.gcd > 1)
		{
			clear();
			return false;
		}
		reciprocalArray[(int)(i - 1)] = res.afactor;
	}

#ifndef NDEBUG
	for (int i = 1; i < size(); i++)
	{
		assert(multiply(i, reciprocal(i)) == 1);
	}
#endif
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void GF2Extension::clear()
{
	nPolynomial = 0;
	nIndex = 0;
	reciprocalArray.clear();
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::add(uint a, uint b) const
{
	assert(a < nPolynomial);	// Show already be reduced.
	assert(b < nPolynomial);
	auto t = _add(a, b);
	auto r = _divide(t, nPolynomial);

	return r.remainder;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::subtract(uint a, uint b) const
{
	assert(a < nPolynomial);	// Show already be reduced.
	assert(b < nPolynomial);
	auto t = _subtract(a, b);
	auto r = _divide(t, nPolynomial);

	return r.remainder;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::multiply(uint a, uint b) const
{
	assert(a < nPolynomial);	// Show already be reduced.
	assert(b < nPolynomial);
	auto t = _multiply(a, b);
	auto r = _divide(t, nPolynomial);

	return r.remainder;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::power(uint a, int exponent) const
{
	assert(exponent > 0);
	uint r = 1;
	for (int i = 0; i < exponent; i++)
		r = multiply(r, a);

	return r;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::reciprocal(uint a) const
{
	assert(a != 0);

	/*
	auto cur = reciprocalMap.find(a);

	uint i;
	if (cur == reciprocalMap.end())
	{
		for (i = 2;; i++)
		{
			//assert(i < (uint)size());
			if (i >= (uint)size())
				cout << "****";

			if (i == a)
				continue;
			if (multiply(i, a) == 1)
				break;
		}
		reciprocalMap.insert(pair<uint, uint>(a, i));
	}
	else
	{
		i = cur->second;
	}*/

	return reciprocalArray[(int)(a - 1)];
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::divide(uint dividend, uint divisor) const
{
	assert(dividend < nPolynomial);	// Show already be reduced.
	assert(divisor < nPolynomial);
	assert(divisor != 0);


	uint i = reciprocal(divisor);

	auto r = _multiply(dividend, i);
	return r;
}
///////////////////////////////////////////////////////////////////////////////
int GF2Extension::order(uint a) const
{
	assert(a != 0);
	assert(a < nPolynomial);
	int i = 1;
	uint cur = a;
	while (cur != 1)
	{
		cur = multiply(a, cur);
		i++;
	}
	return i;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::normFromGalois(uint a, int galois_gen_power) const
{
	// Given the power of the generator of the galois group, calculate the
	// field norm over the corresponding fixed field.

	uint res = 1;
	cout << "Galois power " << galois_gen_power << " on " << a << "\n";
	int p = galois_gen_power;
	do
	{
		auto v = power(a, p);
		cout << "   " << a << "^" << p  <<"=" << v;

		res = multiply(res, v);

		p = p * p;
	} while (p <= size() + 1);

	cout << "  product: " << res << "\n";
	

	return res;
}
///////////////////////////////////////////////////////////////////////////////
string GF2Extension::makeMultiplicationTable() const
{
	stringstream ss;

	ss << "\t|";
	uint nelements = (uint) size();
	for (uint i = 0; i < nelements; i++)
	{
		ss << "\t" << i;
	}
	ss << "\n";
	ss << "------\t|";
	for (uint i = 0; i < nelements; i++)
	{
		ss << "------\t";
	}
	ss << "-----\n";

	for (uint i = 0; i < nelements; i++)
	{
		ss << i << "\t|";

		for (uint j = 0; j < nelements; j++)
		{
			auto prod = multiply(i, j);
			ss << "\t" << prod;
		}
		ss << "\n";
	}

	return ss.str();
}
///////////////////////////////////////////////////////////////////////////////
string GF2Extension::makeGeneratorList() const
{
	stringstream ss;
	int mgsize = size() - 1;	// Size of the multiplicative group.

	int cnt = 0;
	for (uint i = 1; i < (uint) size(); i++)
	{
		if (order(i) == mgsize)
		{
			if (cnt) ss << ", ";
			cnt++;
			ss << i;
		}
	}
	ss << "\nTotal of " << cnt << " generators.\n";

	return ss.str();
}
///////////////////////////////////////////////////////////////////////////////
string GF2Extension::makeGaloisData()
{
	stringstream ss;

	// The Galois group is a cyclic group of order nIndex.
	// It is generated by the map: a --> a^2
	int exp = 2;
	for (int i = 1; i <= nIndex; i++, exp *= 2)
	{
		if (nIndex % i)
			continue;

		ss << "Galois group of size " << nIndex / i << " generated by x^" << exp << " fixes the subfield:\n";
		ss << "   " << 0;
		for (uint i = 1; i < size(); i++)
		{
			if (power(i, exp) == i)
				ss << ", " << i;
		}
		ss << "\n";
	}

	return ss.str();
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::_add(uint a, uint b)
{
	return a ^ b;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::_subtract(uint a, uint b)
{
	return a ^ b;
}
///////////////////////////////////////////////////////////////////////////////
uint GF2Extension::_multiply(uint a, uint b)
{
	uint res = 0;

	while (a)
	{
		if (a & 1)
		{
			res = res ^ b;
		}
		a = a >> 1;
		b = b << 1;
	}
	return res;

}
///////////////////////////////////////////////////////////////////////////////
GF2Extension::DivRes GF2Extension::_divide(uint dividend, uint divisor)
{
	assert(divisor != 0);
	DivRes res;
	res.quotient = 0;
	res.remainder = dividend;

	int sb = HighBit(divisor);

	uint cutoff = ((uint)1) << sb;

	for (int i = maxBits - 1;; i--)
	{
		assert(i >= 0);

		if (res.remainder & ((uint)1 << i))
		{
			int shift = i - sb;
			res.quotient |= ((uint) 1 << shift);
			res.remainder = _subtract(res.remainder, divisor << shift);

		}
		if (res.remainder < cutoff)
			break;

	}

	return res;
}
///////////////////////////////////////////////////////////////////////////////
GF2Extension::GCDRes GF2Extension::extendedEucliedean(uint a, uint b)
{
	assert(a != 0 && b != 0);
	uint s = 0, old_s = 1;
	uint t = 1, old_t = 0;
	uint r = b, old_r = a;

	uint prov, quotient;
#define PA(r, old_r, quotient)			\
		prov = r;						\
		r = _subtract(old_r, _multiply(quotient, prov));	\
		old_r = prov;

	while (r != 0)
	{
		quotient = _divide(old_r, r).quotient;
		PA(r, old_r, quotient)
		PA(s, old_s, quotient)
		PA(t, old_t, quotient)
	}
#undef PA

	GCDRes res;
	res.afactor = old_s;
	res.bfactor = old_t;
	res.gcd = old_r;

	assert(_add(_multiply(res.afactor, a), _multiply(res.bfactor, b)) == res.gcd);

	return res;
}
///////////////////////////////////////////////////////////////////////////////
string GF2Extension::format(uint val)
{
	if (val == 0)
		return "0";

	stringstream ss;
	bool leading = true;
	for (int i = maxBits - 1; i >= 0; i--)
	{
		if (val & ((uint)1 << i))
		{
			if (!leading)
				ss << " + ";
			else
				leading = false;

			if (i == 0)
				ss << '1';
			else
			{
				ss << displayVar;
				if (i > 1)
					ss << "^" << i;
			}
		}
	}

	return ss.str();
}
///////////////////////////////////////////////////////////////////////////////
void GF2Extension::dumpPoly(uint val)
{
	cout << format(val);
}
///////////////////////////////////////////////////////////////////////////////
void GF2Extension::testClass()
{
	assert(format(1) == "1");
	assert(format(0) == "0");
	assert(format(2) == "x");
	assert(format(31) == "x^4 + x^3 + x^2 + x + 1");
	assert(format(8) == "x^3");

	auto x3 = StrToUint("1100");
	auto b = StrToUint("111");

	auto mv = _multiply(x3, b);
	auto tv = _add(mv, 1);


	auto dr = _divide(tv, b);

	assert(dr.remainder == 1);
	assert(dr.quotient == x3);

	GF2Extension ext(7);
	auto prod = ext.multiply(StrToUint("11"), StrToUint("11"));
	assert(prod == 2);
	prod = ext.multiply(StrToUint("10"), StrToUint("10"));
	assert(prod == StrToUint("11"));

	GF2Extension ext8(13);
	GF2Extension ext16(31);
	GF2Extension ext32(61);
	GF2Extension ext64(87);
}
///////////////////////////////////////////////////////////////////////////////

