#pragma once
#include "header.h"
#include "cstdint"

#define TARGET AMD64
#define USECACHE TRUE

#if TARGET == x86
#define INT uint32_t // change the primitive data type depend on target machine
#elif TARGET == AMD64
#define INT uint64_t
#esle
#define INT uint8_t
#endif

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
	unsigned char flags; // 8bit flag variable
	// R R 2C C H O D S
	// R : Reserved 	2C : 2's Complement 	C : Calculated		H : Hexagon
	// O : Octal 		D  : Decimal			S : Sign bit

	void calculate(unsigned char);
#if USECACHE TRUE
	char* str;
#endif
	
public:
	BigInteger();
	BigInteger(INT, unsigned flag);
	BigInteger(char*);
	BigInteger multi(BigInteger);
	BigInteger div(BigInteger);
	BigInteger add(BigInteger);
	BigInteger sub(BigInteger);
	BigInteger mod(BigInteger);
	char* get(unsigned char);
	void set(INT, unsigned char);
	void set(char*);
};