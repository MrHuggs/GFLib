
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
	*/
	{
		GF2Extension ext(13);
		ext.testClass();

		cout << "GF(8) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();
		cout << ext.makeGeneratorList();
	}
	/*
	{
		GF2Extension ext(31);
		ext.testClass();

		cout << "GF(16) with poly " << ext.format(ext.poly()) << "\n";
		cout << ext.makeMultiplicationTable();

		cout << ext.format(1) << " / " << ext.format(13) << " = ";
		auto q = ext.divide(1, 13);

		cout << ext.format(q) << "\n";



#if 1
		//for (uint i = 2; i < 16; i++)
		uint i = 12;
		{
			Subfield sf(&ext);
			sf.addElement(i);
			sf.generateField();

			if (sf.size() < 16)
				sf.dump();
		}
#endif
		
	}*/


	return 0;
}

