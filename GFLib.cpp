
#include "GFLib.h"
#include "GF2Extension.h"
#include "SubField.h"

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	TestConvert();
/*	{
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
	*/
	{
		GF2Extension ext(31);
		ext.testClass();

		cout << "GF(16) with poly " << ext.format(ext.poly()) << "\n";
		//cout << ext.makeMultiplicationTable();

		for (uint i = 2; i < 16; i++)
		{
			Subfield sf(&ext);
			sf.addElement(i);
			sf.generateField();

			if (sf.size() < 16)
				sf.dump();
		}
	}


	return 0;
}

