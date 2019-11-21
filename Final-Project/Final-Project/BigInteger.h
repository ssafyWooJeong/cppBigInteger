#pragma once
#include "header.h"
#include "cstdint"

#define INT uint32_t // change the primitive data type depend on target machine
#define MAX_POSITION 10000 // Maximum of positional number for printing eg) 4 -> [0:9999]

#define isPositive(flag) 0x01 & flag
#define isNegative(flag) 0x01 & ~flag
#define isDecimal(flag) 0x02 & flag
#define isOctal(flag) 0x04 & flag
#define isHexagon(flag) 0x08 & flag
#define isCalculated(flag) 0x10 & flag

class BigInteger
{
private:
	list<INT> lst;
	unsigned flags;
	// R R R C H O D S
	// R : Reserved,	C : Calculated,		H : Hexagon
	// O : Octal,		D : Decimal,		S : Sign bit
	char* str;
	
public:
	BigInteger();
};