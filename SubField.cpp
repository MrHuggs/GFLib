// SubField.cpp

#include <fstream>

#include "GFLib.h"
#include "SubField.h"
///////////////////////////////////////////////////////////////////////////////
Subfield::Subfield(GF2Extension *pext)
{
	pMaster = pext;

	setMembers.insert(0);
	setMembers.insert(1);

}
///////////////////////////////////////////////////////////////////////////////
void Subfield::addElement(uint e)
{
	setMembers.emplace(e);
}
///////////////////////////////////////////////////////////////////////////////
void Subfield::generateField()
{
	setMembers.emplace(0);	// Make sure 0 and 1 are already there.
	setMembers.emplace(1);

	unordered_set<uint> missing;
	int ipass = 0;

#if 0
#define dumpstream cout
#define dump_out() dump()
#else
	ofstream dumpstream;
#define dump_out();
#endif


	for (;;)
	{
		dumpstream << "Generate field pass " << ipass++ << " starting with elements:\n";
		dump_out();

		generateMissingElements(missing);


		if (missing.empty())
			break;

		dumpstream << "Found " << missing.size() << " elements: \n";

		
		for (auto v : missing)
		{
			dumpstream << "\t" << v;
			addElement(v);
		}
		dumpstream << "\n";

		missing.clear();
	}

}
///////////////////////////////////////////////////////////////////////////////
void Subfield::generateMissingElements(unordered_set<uint>  &missing)
{
	for (auto it = setMembers.cbegin(); it != setMembers.cend(); ++it)
	{
		if (*it != 0)
		{
			auto q = pMaster->divide(1, *it);
			if (setMembers.count(q) == 0)
				missing.emplace(q);

			auto n = pMaster->subtract(0, *it);
			if (setMembers.count(n) == 0)
				missing.emplace(n);
		}
		for (auto jt = setMembers.cbegin(); jt != setMembers.cend(); ++jt)
		{
			auto p = pMaster->multiply(*it, *jt);
			if (setMembers.count(p) == 0)
				missing.emplace(p);

			auto s = pMaster->add(*it, *jt);
			if (setMembers.count(s) == 0)
				missing.emplace(s);
		}
	}

}
///////////////////////////////////////////////////////////////////////////////
bool Subfield :: isIdentical(const Subfield *pother) const
{
	if (setMembers.size() != pother->setMembers.size())
		return false;

	for (auto v : setMembers)
	{
		if (pother->setMembers.count(v) == 0)
			return false;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
void Subfield::dump()
{
	cout << "Subfield with " << setMembers.size() << " elememts:\n   ";

	int n = 0;
	for (auto v : setMembers)
	{
		if (n++)
			cout << ", ";
		//string str = pMaster->format(v);
		//cout << "\t" << str << "\n";
		cout << v;
	}
	cout << "\n";
}
