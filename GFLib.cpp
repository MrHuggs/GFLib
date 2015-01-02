
#include "GFLib.h"



///////////////////////////////////////////////////////////////////////////////
class GF2Extension
{

	static const int maxBits = sizeof(uint) * 8;
	static const char displayVar = 'x';

	uint nPolynomial;
	int  nIndex;

public:

	GF2Extension(uint poly)
	{
		assert(CountBits(poly) & 1);	// Polynomial has to be irreducible over GF(2).
		nPolynomial = poly;

		nIndex = HighBit(poly);			// Extension index is the degree of the irreducible poly.
	}

	uint poly() const { return nPolynomial; }

	struct DivRes
	{
		uint quotient;
		uint remainder;
	};

	uint add(uint a, uint b)
	{
		assert(a < nPolynomial);	// Show already be reduced.
		assert(b < nPolynomial);
		auto t = _add(a, b);
		auto r = _divide(t, nPolynomial);

		return r.remainder;
	}

	uint subtract(uint a, uint b)
	{
		assert(a < nPolynomial);	// Show already be reduced.
		assert(b < nPolynomial);
		auto t = _subtract(a, b);
		auto r = _divide(t, nPolynomial);

		return r.remainder;
	}

	uint multiply(uint a, uint b)
	{
		assert(a < nPolynomial);	// Show already be reduced.
		assert(b < nPolynomial);
		auto t = _multiply(a, b);
		auto r = _divide(t, nPolynomial);

		return r.remainder;
	}

	DivRes divide(uint dividend, uint divisor)
	{
		assert(dividend < nPolynomial);	// Show already be reduced.
		assert(divisor < nPolynomial);

		if (dividend < divisor)
		{
			dividend = _add(dividend, nPolynomial);
		}


		auto r = _divide(dividend, divisor);

		assert(r.quotient < nPolynomial);
		assert(r.remainder < nPolynomial);

		return r;
	}

	string makeMultiplicationTable()
	{
		stringstream ss;
	

		ss << "\t|";
		uint nelements = ((uint) 1) << nIndex;
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

	// Static methods.
	static uint _add(uint a, uint b)
	{
		return a ^ b;
	}

	static uint _subtract(uint a, uint b)
	{
		return a ^ b;
	}

	static uint _multiply(uint a, uint b)
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


	static DivRes _divide(uint dividend, uint divisor)
	{ 
		assert(divisor != 0);
		DivRes res;
		res.quotient = 0;
		res.remainder = dividend;

		int sb = HighBit(divisor);

		uint cutoff = ((uint) 1) << sb;

		for (int i = maxBits - 1; ; i--)
		{
			assert(i >= 0);

			if (res.remainder & (1 << i))
			{
				int shift = i - sb;
				res.quotient |= (1 << shift);
				res.remainder = _subtract(res.remainder, divisor << shift);

			}
			if (res.remainder < cutoff)
				break;

		}

		return res;
	}

	static string format(uint val)
	{
		if (val == 0)
			return "0";

		stringstream ss;
		bool leading = true;
		for (int i = maxBits - 1; i >= 0; i--)
		{
			if (val & (1 << i))
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

	static void dump(uint val)
	{
		cout << format(val);
	}

	
	static void testClass()
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

	}

};




int main(int argc, char* argv[])
{
	TestConvert();

	{
		GF2Extension ext(7);
		ext.testClass();

		cout << "GF(4) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
	}

	{
		GF2Extension ext(13);
		ext.testClass();

		cout << "GF(8) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
	}

	{
		GF2Extension ext(31);
		ext.testClass();

		cout << "GF(16) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
	}


	return 0;
}

