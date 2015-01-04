

#include "GF2Extension.h"
#include <unordered_set>

class Subfield
{
	GF2Extension *pMaster;
	unordered_set<uint> setMembers;
public:

	Subfield(GF2Extension *pext);

	void addElement(uint e);
	// Comlplete the field based on the current members:
	void generateField();

	void generateMissingElements(unordered_set<uint>  &missing);
	int size() const { return setMembers.size(); }

	bool isIdentical(const Subfield *pother) const;

	void dump();

};