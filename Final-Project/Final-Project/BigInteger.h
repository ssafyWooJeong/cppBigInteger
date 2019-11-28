#pragma once
#pragma warning(disable:4996)
#include "header.h"
#include <cstddef>
#include <intrin.h>
#include <cstdint>
#include <cstring>
#include <memory>

#define TARGET AMD64
#define USECACHE TRUE

#if TARGET == x86
#define INT uint32_t // change the primitive data type depend on target machine, but when you use less than uint32_t eflag's carry bit unavailable
#elif TARGET == AMD64
#define INT uint64_t
#esle
#define INT uint8_t
#endif

#define MAX_POSITION 1000 // Maximum of positional number for printing eg) 4 -> [0:9999]

#define POSITIVE 0x01
#define DECIMAL 0x02
#define OCTAL 0x04
#define HEXAGON 0x08
#define CALCULATED 0x10
#define COMPLEMENT 0x20
#define DIVIDEBYZERO 0x40
#define ERROR 0x80

#define isPositive(flag) POSITIVE & flag
#define isNegative(flag) !(POSITIVE & flag)
#define isDecimal(flag) DECIMAL & flag
#define isOctal(flag) OCTAL & flag
#define isHexagon(flag) HEXAGON & flag
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

template<typename T>
struct ArrayDeleter
{
	void operator () (T* p)
	{
		delete[] p;
	}
};

class BigInteger
{
private:
	list<INT> lst;
	unsigned char flags; // 8bit flag variable
	// E DivZ 2C C H O D S
	// E : Error		Divz : Divided by Zero 	2C : 2's Complement 	C : Calculated		
	// H : Hexagon		O	 : Octal 			D  : Decimal			S : Sign bit

#if USECACHE == TRUE
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
	friend bool operator>=(const BigInteger& left, const BigInteger& right)
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
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) >= const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
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
				if (const_cast<BigInteger&>(left).lst.at(left.lst.size - 1) < const_cast<BigInteger&>(right).lst.at(left.lst.size - 1))
				{
					return true;
				}
				else return false;
			}
		}
	}
	void set(char* num)
	{
		this->lst = list<INT>();
		
		if (num == nullptr) // add exception handling when needed
		{
			this->flags = ERROR;

			return;
		}

		if(num[0] == '-') // when starting character is minus, the number is negative
		{
			this->flags = 0;
		}else
		{
			this->flags = POSITIVE;
		}

		for(INT i = (0 + (isNegative(this->flags)?1:0)); i < strlen(num); i++)
		{
			if(num[i] <'0' || (num[i] > '9' && num[i] < 'A') || (num[i] > 'Z'  && num[i] < 'a') || num[i] > 'z')
			{
				this->flags = ERROR;

				return;
			}
		}
		
		if (num[0 + (isNegative(this->flags) ? 1 : 0) ] != '0') // when starting number is not zero, assume that is decimal
		{
			this->flags |= DECIMAL;

			INT tmp = 0;
			INT accumulater = 0;
			INT base = 1;
			INT cnt = strlen(num) - 1;
			while(1)
			{
				tmp = accumulater;
				accumulater = accumulater + (num[cnt] - '0') * base;
				base *= 10;
				if (isCarrySet())
				{
					this->lst.append(tmp);
					base = 1;
					accumulater = 0;
				}

				if (--cnt == 0)
					break;
			}
			this->lst.append(accumulater);
		}else if(num[0 + (isNegative(this->flags) ? 1 : 0)] == '0')
		{
			if(num[1 + (isNegative(this->flags) ? 1 : 0)] == 'x' || num[1 + (isNegative(this->flags) ? 1 : 0)] == 'X') // hexagon
			{
				this->flags |= HEXAGON;
				this->lst.append((num[2] <= '9'? (num[2] - '0'):(num[2] >= 'a'?num[2]-'a' + 10: num[2]-'A' + 10)));
				INT iter = 3;
				node<INT>* ptr;
				INT buffer = 0;
				INT buffer2 = 0;

				while(iter < strlen(num))
				{
					if(this->lst.tail->getData() >= (1 << 4 * (sizeof(INT) - 1)))
					{
						lst.append(0);
					}
					ptr = this->lst.head;
					buffer = buffer2 = 0;
					while (ptr != nullptr)
					{
						buffer2 = buffer;
						buffer = (ptr->getData() & (0xf << 4 * (sizeof(INT) - 1)));
						ptr->setData((ptr->getData() << 4) | buffer2);

						ptr = ptr->getNext();
					}
					this->lst.head->setData(this->lst.head->getData() | (num[iter] <= '9' ? (num[iter] - '0') : (num[iter] >= 'a' ? num[iter] - 'a' : num[iter] - 'A')));
					iter++;
				}
				
			}else // octal
			{
				this->flags |= OCTAL;

				this->lst.append(num[2] - '0');
				INT iter = 2;
				node<INT>* ptr;
				INT buffer = 0;
				INT buffer2 = 0;

				while (iter < strlen(num))
				{
					if (this->lst.tail->getData() >= (1 << 3 * (sizeof(INT) - 1)))
					{
						lst.append(0);
					}
					ptr = this->lst.head;
					buffer = buffer2 = 0;
					while (ptr != nullptr)
					{
						buffer2 = buffer;
						buffer = (ptr->getData() & (07 << 3 * (sizeof(INT) - 1)));
						ptr->setData((ptr->getData() << 3) | buffer2);

						ptr = ptr->getNext();
					}
					this->lst.head->setData(this->lst.head->getData() | num[iter] - '0');
					iter++;
				}
			}
		}

#if USECACHE == TRUE
		str = new char[strlen(num) + 1];
		strcpy(str, num);
		str[strlen(num)] = '\0';
#endif
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
		this->set(num);
	}
	BigInteger(BigInteger& old)
	{
		this->lst = list<INT>(old.lst);
		this->flags = old.flags;
#if USECACHE == TRUE
		if(old.str != nullptr)
		{
			this->str = new char[strlen(old.str) + 1];
			strcpy(this->str, old.str);
			this->str[strlen(old.str)] = '\0';
		}
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

#if USECACHE == TRUE
		flags = flags & ~CALCULATED;
#endif
		
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

		while(this->lst.tail->getData() == 0 && lst.getSize() > 1)
		{
			this->lst.remove(this->lst.size - 1);
		}
		
#if USECACHE == TRUE
		flags = flags & ~CALCULATED;
#endif
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
#if USECACHE == TRUE
		flags = flags & ~CALCULATED;
#endif
		this->flags &= ~ERROR;
		return *this;
	}
	BigInteger& div(BigInteger& right)
	{
		BigInteger lTmp(*this);
		BigInteger rTmp(right);
		BigInteger iTmp((INT)0);
		BigInteger one(1);

		if (rTmp.lst.getSize() == 1 && rTmp.lst.at(0) == 0) // when right is 0 -> dived by zero, add exception handling when need
		{
			this->lst = list<INT>();
			this->flags |= ERROR;
			return *this;
		}

		while (*this >= right)
		{
			iTmp.add(one);
			this->sub(right);
		}

		this->lst = iTmp.lst;
		this->flags |= ~CALCULATED;

		if (!(isPositive(this->flags) ^ isPositive(right.flags)))
		{
			this->flags |= POSITIVE;
		}
		else
		{
			this->flags &= ~POSITIVE;
		}
#if USECACHE == TRUE
		flags = flags & ~CALCULATED;
#endif
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
				if(*this >= right)
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
#if USECACHE == TRUE
		flags = flags & ~CALCULATED;
#endif
		return *this;
	}
	std::shared_ptr<char[]> get(unsigned char mode = DECIMAL|OCTAL|HEXAGON) // use shared pointer, because we can't know whether the caller will free the memory or not
	{
		if (mode == (DECIMAL | OCTAL | HEXAGON))
		{
			mode = this->flags & (DECIMAL | OCTAL | HEXAGON);
		}
#if USECACHE == TRUE
		if(isCalculated(this->flags))
		{
			if(this->flags & mode)// saved string match user's want or calculated, return this by shared pointer
			{		
				std::shared_ptr<char[]> nptr(new char[strlen(this->str) + 1]);
				for (int i = 0; i < strlen(this->str); i++)
				{
					nptr[i] = this->str[i];
				}
				nptr[strlen(this->str)] = '\0';

				return nptr;
			}
		}
#endif

		char* tmpResult;
		INT cnt = 0;
		INT i;
		std::shared_ptr<char[]> result;
		
		if(mode & DECIMAL)
		{
			tmpResult = new char[MAX_POSITION];
			BigInteger tmp(*this);
			tmp.flags |= POSITIVE;
			BigInteger zero((INT)0);
			BigInteger ten((INT)10);

			while(tmp > zero)
			{
				BigInteger tmp2(tmp);
				tmp2.mod(ten);
				tmpResult[cnt++] = tmp2.lst.at(0) + '0';
				tmp.div(ten);
			}
			tmpResult[cnt] = '\0';

			result = std::shared_ptr<char[]>(new char[strlen(tmpResult) + 2], ArrayDeleter<char>());

			if (isNegative(this->flags))
				result[0] = '-';
			
			for(i = 0 + (isNegative(this->flags)?1:0); i < strlen(tmpResult) + (isNegative(this->flags) ? 1 : 0); i++)
			{
				result[i] = tmpResult[--cnt];
			}

			result[i] = '\0';
		}else if(mode & HEXAGON)
		{
			BigInteger tmp(*this);
			INT buffer = 0;
			INT buffer2 = 0;
			node<INT>* ptr;
			tmpResult = new char[MAX_POSITION];

			while(1)
			{
				tmpResult[cnt++] = ((tmp.lst.at(0) & 0xf) >=10 ? (tmp.lst.at(0) &0xf) + 55 : (tmp.lst.at(0) & 0x0f) + '0'); // A's ASCII code : 65
				ptr = tmp.lst.tail;
				
				while(ptr != nullptr)
				{
					buffer2 = buffer;
					buffer = (ptr->getData() & 0xf);
					ptr->setData((ptr->getData() >> 4) | (buffer2 << 4));

					ptr = ptr->getPrev();
					if(ptr == nullptr)
					{
						break;
					}
					
					if(ptr->getNext()->getData() == 0)
					{
						delete ptr->getNext();
						ptr->setNext(nullptr);
						tmp.lst.size--;
					}
				}

				if((tmp.lst.getSize() == 1) && (tmp.lst.at(0) ==0))
				{
					break;
				}
			}
			tmpResult[cnt] = '\0';
			result = std::shared_ptr<char[]>(new char[strlen(tmpResult) + 4]);

			if (isNegative(this->flags))
				result[0] = '-';
			result[0 + (isNegative(this->flags) ? 1 : 0)] = '0';
			result[1 + (isNegative(this->flags) ? 1 : 0)] = 'x';
			for (i = 2 + (isNegative(this->flags) ? 1 : 0); i < strlen(tmpResult) + 2 + (isNegative(this->flags) ? 1 : 0); i++)
			{
				result[i] = tmpResult[--cnt];
			}

			result[i] = '\0';
			
		}else if(mode & OCTAL)
		{
			BigInteger tmp(*this);
			INT buffer = 0;
			INT buffer2 = 0;
			node<INT>* ptr;
			tmpResult = new char[MAX_POSITION];

			while (1)
			{
				tmpResult[cnt++] = (tmp.lst.at(0) & 07) + '0';
				ptr = tmp.lst.tail;

				while (ptr != nullptr)
				{
					buffer2 = buffer;
					buffer = (ptr->getData() & 07);
					ptr->setData((ptr->getData() >> 3) | (buffer2 << 5));

					ptr = ptr->getPrev();
					if (ptr == nullptr)
					{
						break;
					}
					if (ptr->getNext()->getData() == 0)
					{
						delete ptr->getNext();
						ptr->setNext(nullptr);
						tmp.lst.size--;
					}
				}

				if ((tmp.lst.getSize() == 1) && (tmp.lst.at(0) == 0))
				{
					break;
				}
			}
			tmpResult[cnt] = '\0';
			result = std::shared_ptr<char[]>(new char[strlen(tmpResult) + 3], ArrayDeleter<char>());

			if (isNegative(this->flags))
				result[0] = '-';
			
			result[0 + (isNegative(this->flags) ? 1 : 0)] = '0';
			for (i = 1 + (isNegative(this->flags) ? 1 : 0); i < strlen(tmpResult) + 1 + (isNegative(this->flags) ? 1 : 0); i++)
			{
				result[i] = tmpResult[--cnt];
			}

			result[i] = '\0';

		}else // error 
		{
			this->flags |= ERROR;
			return std::shared_ptr<char[]>(nullptr);
		}
#if USECACHE == TRUE
		INT tmp = 0;
		while(result[tmp++] != '\0'){}
		this->str = new char[tmp + 1];
		for(INT i = 0; i <= tmp; i++)
		{
			this->str[i] = result[i];
		}
		this->flags |= CALCULATED;
#endif
		
		return result;
	}
	void set(INT data, unsigned char flag)
	{
		this->lst = list<INT>();
		this->lst.append(data);
		this->flags = flag;
#if USECACHE == TRUE
		str = nullptr;
#endif
	}
	unsigned char getFlags()
	{
		return this->flags;
	}
};