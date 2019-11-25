#pragma once
#include "header.h"
#include <cstddef>
#include <intrin.h>
#include <cstdint>
#include <cstring>

#define TARGET AMD64
#define USECACHE TRUE

#if TARGET == x86
#define INT uint32_t // change the primitive data type depend on target machine, but when you use less than uint32_t eflag's carry bit unavailable
#elif TARGET == AMD64
#define INT uint64_t
#esle
#define INT uint8_t
#endif

#define MAX_POSITION 10000 // Maximum of positional number for printing eg) 4 -> [0:9999]

#define POSITIVE 0x01
#define DECIAML 0x02
#define OCTAL 0x04
#define Hexagon 0x08
#define CALCULATED 0x10
#define COMPLEMENT 0x20

#define isPositive(flag) POSITIVE & flag
#define isNegative(flag) POSITIVE & ~flag
#define isDecimal(flag) DECIAML & flag
#define isOctal(flag) OCTAL & flag
#define isHexagon(flag) Hexagon & flag
#define isCalculated(flag) CALCULATED & flag
#define isComplement(flag) COMPLEMENT & flag

#if TARGET == x86 || TARGET == AMD64
#define isCarrySet() __readeflags() & 0x001
#else 
 // NOT x86 and AMD64
#endif

/*
 * EFLAG INFOS
 * EFL = OF DF IF TF / SF ZF 0 AF / 0 PF 1 CF
 */

class BigInteger
{
private:
	list<INT> lst;
	unsigned char flags; // 8bit flag variable
	// R R 2C C H O D S
	// R : Reserved 	2C : 2's Complement 	C : Calculated		H : Hexagon
	// O : Octal 		D  : Decimal			S : Sign bit

	void calculate(unsigned char);
#if USECACHE==TRUE
	char* str;
#endif
	
public:
	BigInteger()
	{
		this->lst = list<INT>();
		this->flags = 0;
	}
	BigInteger(INT data, unsigned char flag = DECIAML|POSITIVE)
	{
		this->lst = list<INT>();
		this->lst.append(data);
		this->flags = flag;
	}
	BigInteger(char* num)
	{
		if (num == nullptr) // add exception handling when needed
		{
			this->lst = list<INT>();
			this->flags = 0;

			return;
		}

		if (num[0] != '0') // when starting number is not zero, assume that is decimal
		{
			this->flags = DECIAML;

			INT tmp = 0;
			INT accumulater = 0;
			INT base = 1;
			for(INT cnt = strlen(num); cnt != 0; cnt--)
			{
				if(isCarrySet())
				{
					
				}

				accumulater = accumulater + (num[cnt] - '0') * base;
			}
			
		}
	}
	BigInteger(BigInteger& old)
	{
		
	}
	BigInteger& multi(BigInteger &right);
	BigInteger& div(BigInteger &right);
	BigInteger& add(BigInteger &right)
	{
		INT leftCnt = this->lst.getSize(), rightCnt = right.lst.getSize();
		bool carry = false;

		while(leftCnt != 0 && rightCnt != 0)
		{
			if(leftCnt > 0 && rightCnt > 0)
			{
				this->lst.at(this->lst.getSize() - leftCnt) += right.lst.at(right.lst.getSize() - rightCnt) + (carry ? 1 : 0);
			}else if(leftCnt > 0 || rightCnt == 0)
			{
				if(carry)
				{
					this->lst.at(this->lst.getSize() - leftCnt) += 1;
				}else
				{
					break;
				}
			}else if(leftCnt == 0 && rightCnt > 0)
			{
				this->lst.append(right.lst.at(right.lst.getSize() - rightCnt) + (carry ? 1 : 0));
			}
			
			if (isCarrySet())
			{
				carry = true;
			}
			else
			{
				carry = false;
			}
			leftCnt--;
			rightCnt--;
		}

		if(carry)
		{
			this->lst.append(1);
		}


		return *this;
	}
	BigInteger& sub(BigInteger &right)
	{
		list<INT> nlst(right.lst);
		if(!(right.flags & COMPLEMENT))
		{
			for(INT i = 0; i < nlst.getSize(); i++)
			{
				nlst.at(i) = ~nlst.at(i);
			}
			nlst.at(0) += 1;
		} // make 2's complement

		INT tmp = 255;
		for(INT i = 0; i < sizeof(INT) - 1; i++)
		{
			tmp = tmp | (tmp << 8);
		}
		
		
		if(this->lst.getSize() > nlst.getSize())
		{
			for(INT i = 0; i < this->lst.getSize() - nlst.getSize(); i++)
			{
				this->lst.append(0);
			}
		}else if(this->lst.getSize() < nlst.getSize())
		{
			for(INT i = 0; i < nlst.getSize() - this->lst.getSize(); i++)
			{
				nlst.append(tmp);
			}
		}

		this->lst.append(0);
		nlst.append(tmp);
		// avoid BigInteger's overflow

		INT leftCnt = this->lst.getSize(), rightCnt = nlst.getSize();
		bool carry = false;

		while (leftCnt != 0 && rightCnt != 0)
		{
			if (leftCnt > 0 && rightCnt > 0)
			{
				this->lst.at(this->lst.getSize() - leftCnt) += nlst.at(nlst.getSize() - rightCnt) + (carry ? 1 : 0);
			}
			else if (leftCnt > 0 || rightCnt == 0)
			{
				if (carry)
				{
					this->lst.at(this->lst.getSize() - leftCnt) += 1;
				}
				else
				{
					break;
				}
			}
			else if (leftCnt == 0 && rightCnt > 0)
			{
				this->lst.append(nlst.at(nlst.getSize() - rightCnt) + (carry ? 1 : 0));
			}

			if (isCarrySet())
			{
				carry = true;
			}
			else
			{
				carry = false;
			}
			leftCnt--;
			rightCnt--;
		}

		tmp = 0x80;
		for (INT i = 0; i < sizeof(INT) - 1; i++)
		{
			tmp = (tmp | (tmp << 8)) & ~tmp;
		}

		if(this->lst.tail->getData() & tmp) // sign bit is negative
		{
			flags = flags & ~POSITIVE;
			
			for (INT i = 0; i < this->lst.getSize(); i++)
			{
				this->lst.at(i) = ~this->lst.at(i);
			}
			this->lst.at(0) += 1;
			// convert 2's complement to ordinal number
		}else
		{
			flags = flags | POSITIVE;
		}

		while(this->lst.tail->getData() == 0)
		{
			this->lst.remove(this->lst.size - 1);
			this->lst.size--;
		}
		
		return *this;
	}
	BigInteger& mod(BigInteger &);
	char* get(unsigned char);
	void set(INT, unsigned char);
	void set(char*);
};