#pragma once
#pragma warning(disable:4996)
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
#define DECIMAL 0x02
#define OCTAL 0x04
#define Hexagon 0x08
#define CALCULATED 0x10
#define COMPLEMENT 0x20
#define DIVIDEBYZERO 0x40
#define ERROR 0x80

#define isPositive(flag) POSITIVE & flag
#define isNegative(flag) POSITIVE & ~flag
#define isDecimal(flag) DECIMAL & flag
#define isOctal(flag) OCTAL & flag
#define isHexagon(flag) Hexagon & flag
#define isCalculated(flag) CALCULATED & flag
#define isComplement(flag) COMPLEMENT & flag
#define isDivideByZero(flag) DIVIDEBYZERO & flag
#define errorOccured(flag) ERROR & flag

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
	// E DivZ 2C C H O D S
	// E : Error		Divz : Divided by Zero 	2C : 2's Complement 	C : Calculated		
	// H : Hexagon		O	 : Octal 			D  : Decimal			S : Sign bit

	void calculate(unsigned char);
#if USECACHE==TRUE
	char* str;
#endif
	
public:
	friend bool operator==(const BigInteger& left, const BigInteger& right)
	{
		if(left.lst.size != right.lst.size)
		{
			return false;
		}else if((left.flags & POSITIVE) ^ (right.flags & POSITIVE))
		{
			return false;
		}else
		{
			for(INT i = 0; i < left.lst.size; i++)
			{
				if(const_cast<BigInteger &>(left).lst.at(i) != const_cast<BigInteger &>(right).lst.at(i))
				{
					return false;
				}
			}
		}

		return true;
	}
	friend bool operator<(const BigInteger& left, const BigInteger& right)
	{
		if(isPositive(left.flags) && isPositive(right.flags))
		{
			if(left.lst.size < right.lst.size)
			{
				return true;
			}else if(left.lst.size > right.lst.size)
			{
				return false;
			}else // same list size
			{
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) < const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
				{
					return true;
				}
				else return false;
			}
		}else if(isNegative(left.flags) && isPositive(right.flags))
		{
			return true;
		}else if(isPositive(left.flags) && isNegative(right.flags))
		{
			return false;
		}else // both negative
		{
			if (left.lst.size < right.lst.size)
			{
				return false;
			}
			else if (left.lst.size > right.lst.size)
			{
				return true;
			}
			else // same list size
			{
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) >= const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
				{
					return true;
				}
				else return false;
			}
		}
	}
	friend bool operator>(const BigInteger& left, const BigInteger& right)
	{
		if (isPositive(left.flags) && isPositive(right.flags))
		{
			if (left.lst.size > right.lst.size)
			{
				return true;
			}
			else if (left.lst.size < right.lst.size)
			{
				return false;
			}
			else // same list size
			{
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) > const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
				{
					return true;
				}
				else return false;
			}
		}
		else if (isNegative(left.flags) && isPositive(right.flags))
		{
			return false;
		}
		else if (isPositive(left.flags) && isNegative(right.flags))
		{
			return true;
		}
		else // both negative
		{
			if (left.lst.size < right.lst.size)
			{
				return true;
			}
			else if (left.lst.size > right.lst.size)
			{
				return false;
			}
			else // same list size
			{
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) <= const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
				{
					return true;
				}
				else return false;
			}
		}
	}
	BigInteger()
	{
		this->lst = list<INT>();
		this->flags = 0;
#if USECACHE == TRUE
		str = nullptr;
#endif
	}
	BigInteger(INT data, unsigned char flag = DECIMAL|POSITIVE)
	{
		this->lst = list<INT>();
		this->lst.append(data);
		this->flags = flag;
#if USECACHE == TRUE
		str = nullptr;
#endif
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
			this->flags = DECIMAL;

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

#if USECACHE == TRUE
		str = new char[strlen(num) + 1];
		strcpy(str, num);
		str[strlen(num)] = '\0';
#endif
		
	}
	BigInteger(BigInteger& old)
	{
		this->lst = list<INT>(old.lst);
		this->flags = old.flags;
#if USECACHE == TRUE
		this->str = new char[strlen(old.str) + 1];
		strcpy(this->str, old.str);
		this->str[strlen(old.str)] = '\0';
#endif
	}
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

		this->flags &= ~ERROR;
		return *this;
	}
	
	BigInteger& add(INT right)
	{
		BigInteger tmp(right);
		this->add(tmp);

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

		this->flags &= ~ERROR;
		return *this;
	}
	BigInteger& sub(INT& right)
	{
		BigInteger tmp(right);
		this->sub(tmp);
		return *this;
	}
	BigInteger& multi(BigInteger& right)
	{
		BigInteger lTmp(*this);
		BigInteger rTmp(right);
		INT iTmp = 1;

		if(rTmp.lst.getSize() == 1 && rTmp.lst.at(0) ==0) // when right is 0
		{
			this->lst = list<INT>();
			lst.append(0);
			this->flags |= POSITIVE;
			
			return *this;
		}

		while(1) // rTmp.lst.getSize() >= 1
		{
			if (rTmp.lst.getSize() == 1 && rTmp.lst.at(0) == 1)
				break;
			
			this->add(lTmp);
			rTmp.sub(iTmp);
		}

		if(!(isPositive(this->flags) ^ isPositive(right.flags)))
		{
			this->flags |= POSITIVE;
		}else
		{
			this->flags &= ~POSITIVE;
		}

		this->flags &= ~ERROR;
		return *this;
	}
	BigInteger& div(BigInteger& right)
	{
		BigInteger lTmp(*this);
		BigInteger rTmp(right);
		INT iTmp = 1;

		if (rTmp.lst.getSize() == 1 && rTmp.lst.at(0) == 0) // when right is 0 -> dived by zero, add exception handling when need
		{
			this->lst = list<INT>();
			this->flags |= ERROR;
			return *this;
		}

		while (1)
		{
			if (rTmp.lst.getSize() == 1 && rTmp.lst.at(0) == 0)
				break;

			if(*this < lTmp)
			{
				this->lst = list<INT>(0);
				return *this;
			}
			
			this->sub(lTmp);
			rTmp.sub(iTmp);
		}

		if (!(isPositive(this->flags) ^ isPositive(right.flags)))
		{
			this->flags |= POSITIVE;
		}
		else
		{
			this->flags &= ~POSITIVE;
		}

		this->flags &= ~ERROR;
		return *this;
	}
	BigInteger& mod(BigInteger& right)
	{
		BigInteger ZERO((INT)0);
		
		if (right.lst.getSize() == 1 && right.lst.at(0) == 0) // when right is 0 -> dived by zero, add exception handling when need
		{
			this->lst = list<INT>();
			this->flags |= ERROR;
			return *this;
		}

		if(isPositive(this->flags) && isPositive(right.flags)) // both are positive
		{
			while(1)
			{
				if(*this > right)
				{
					this->sub(right);
				}else
				{
					return *this;
				}
			}
		}else if(isNegative(right.flags)) // According to Mathematical law(Number's Theory), modulo should be positive
		{
			this->lst = list<INT>();
			this->flags |= ERROR;
			return *this;
		}else // left->negative, right->positive
		{
			while(1)
			{
				if(*this > ZERO)
				{
					this->flags &= ~POSITIVE;
					return *this;
				}else
				{
					this->add(right);
				}
			}
		}

		return *this;
	}
	char* get(unsigned char mode = DECIMAL);
	void set(INT, unsigned char);
	void set(char*);
	unsigned char getFlags()
	{
		return this->flags;
	}
};