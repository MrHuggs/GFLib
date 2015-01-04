// GFLib.h

#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <cassert>
using namespace std;

using uint = unsigned _int64;

// Utilities
int HighBit(uint val);
int CountBits(uint val);
string UintToStr(uint val);
uint StrToUint(const char *pc);
void TestConvert();
