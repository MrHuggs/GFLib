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

// Utilities
int HighBit(uint val);
int CountBits(uint val);
string UintToStr(uint val);
uint StrToUint(const char *pc);
void TestConvert();
