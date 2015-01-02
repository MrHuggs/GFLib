// SubField.cpp

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

	vector<uint> missing;
	for (;;)
	{
		generateMissingElements(missing);
		if (missing.empty())
			break;
		
		for (auto v : missing)
		{
			addElement(v);
		}

		missing.clear();
	}

}
///////////////////////////////////////////////////////////////////////////////
void Subfield::generateMissingElements(vector<uint> &missing)
{
	for (auto it = setMembers.cbegin(); it != setMembers.cend(); ++it)
	{
		if (*it != 0)
		{
			auto r = pMaster->divide(1, *it);
			if (setMembers.count(r.quotient) == 0)
				missing.push_back(r.quotient);

			auto n = pMaster->subtract(0, *it);
			if (setMembers.count(n) == 0)
				missing.push_back(n);
		}
		for (auto jt = setMembers.cbegin(); jt != setMembers.cend(); ++jt)
		{
			auto p = pMaster->multiply(*it, *jt);
			if (setMembers.count(p) == 0)
				missing.push_back(p);

			auto s = pMaster->add(*it, *jt);
			if (setMembers.count(s) == 0)
				missing.push_back(s);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
void Subfield::dump()
{
	cout << "Subfield with " << setMembers.size() << " elememts.\n";

	for (auto v : setMembers)
	{
		string str = pMaster->format(v);
		cout << "\t" << str << "\n";
	}
}
