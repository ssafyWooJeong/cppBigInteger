#include <iostream>
#include "header.h"
#include "BigIntNerualNet.h"
using namespace std;

void BigIntegerTest(BigInteger& left, BigInteger& right)
{
	cout << "--------------------------------------" << endl;
	cout << "Hexagon value of left : " << left.get(HEXAGON) << endl;
	cout << "Hexagon value of right : " << right.get(HEXAGON) << endl;
	cout << "--------------------------------------" << endl;
	cout << "left + right : " << (left + right).get(HEXAGON) << endl;
	cout << "left - right : " << (left - right).get(HEXAGON) << endl;
	cout << "left * right : " << (left * right).get(HEXAGON) << endl;
	cout << "left / right : " << (left / right).get(HEXAGON) << endl;
	cout << "left % right : " << (left % right).get(HEXAGON) << endl << endl;;

	
	return;
}

int main()
{
	BigInteger a(UINT32_MAX);
	BigInteger b(10);
	BigIntegerTest(a, b);
	
	//list<int> obj;
	//obj.append(1); obj.append(2); obj.append(3); obj.append(4);
	//obj.remove(1);
	//obj.insert(2, 6);
	//
	//cout << obj.at(3) << endl;
	//
	//// list test code
	//
	//uint32_t a = UINT32_MAX;
	//uint32_t b = UINT32_MAX;
	//
	//uint32_t c = 0;
	//c = a + b;

	//BigInteger obj1(UINT32_MAX);
	//BigInteger obj2(UINT32_MAX - 3);
	//
	//cout << "Hexagon of UINT32_MAX : " << obj1.get(HEXAGON) << endl;
	//cout << "Hexagon of UINT32_MAX - 3 : " << obj2.get(HEXAGON) << endl;
	//obj1.add(obj2);
	//cout << "UINT32_MAX + (UINT32_MAX - 3) " << endl;
	//cout << "Hexagon : " << obj1.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj1.get(OCTAL) << endl << endl;
	//
	//BigInteger obj3(5);
	//BigInteger obj4(3);
	//cout << "Decimal of 5 : " <<  obj3.get() << endl;
	//cout << "Decimal of 3 : " <<  obj4.get() << endl;
	//obj3.sub(obj4);
	//cout << "5 - 3" << endl;
	//cout << "Decimal : " <<  obj3.get(DECIMAL) << endl;
	//cout << "Hexagon : " << obj3.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj3.get(OCTAL) << endl << endl;
	//
	//
	//BigInteger obj5(7);
	//BigInteger obj6(10);
	//cout << "Octal of  7 : " << obj5.get(OCTAL) << endl;
	//cout << "Octal of 10 : " << obj6.get(OCTAL) << endl;
	//obj5.multi(obj6);
	//cout << "7 * 10" << endl;
	//cout << "Decimal : " << obj5.get(DECIMAL) << endl;
	//cout << "Hexagon : " << obj5.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj5.get(OCTAL) << endl << endl;
	//
	//BigInteger obj7((char *)"-7");
	//BigInteger obj8((char*)"0xf");
	//
	//cout << "Decimal of -7 : " << obj7.get(DECIMAL) << endl;
	//cout << "Deciaml of 0xf : " << obj8.get(DECIMAL) << endl;
	//obj7.multi(obj8);
	//cout << "-7 * 0xf" << endl;
	//cout << "Decimal : " << obj7.get(DECIMAL) << endl;
	//cout << "Hexagon : " << obj7.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj7.get(OCTAL) << endl << endl;
	//
	//BigInteger obj9((char*)"30");
	//BigInteger obj10((char*)"10");
	//
	//cout << "Decimal of 30 : " << obj9.get(DECIMAL) << endl;
	//cout << "Deciaml of 10 : " << obj10.get(DECIMAL) << endl;
	//obj9.div(obj10);
	//cout << "30 / 10" << endl;
	//cout << "Decimal : " << obj9.get(DECIMAL) << endl;
	//cout << "Hexagon : " << obj9.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj9.get(OCTAL) << endl << endl;
	//
	//BigInteger obj11((char*)"35");
	//BigInteger obj12((char*)"10");
	//
	//cout << "Decimal of 35 : " << obj11.get(DECIMAL) << endl;
	//cout << "Deciaml of 10 : " << obj12.get(DECIMAL) << endl;
	//obj11.mod(obj12);
	//cout << "35 % 10" << endl;
	//cout << "Decimal : " << obj11.get(DECIMAL) << endl;
	//cout << "Hexagon : " << obj11.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj11.get(OCTAL) << endl << endl;
	//
	//BigInteger obj13((char*)"0xffffffff\0");
	//BigInteger obj14((char*)"2");
	//
	//cout << "Decimal of 0xFFFFFFFF‬ : " << obj13.get(HEXAGON) << endl;
	//cout << "Deciaml of 2 : " << obj14.get(HEXAGON) << endl;
	//obj13.multi(obj14);
	//cout << "0xFFFFFFFF * 2" << endl;
	//cout << "Hexagon : " << obj13.get(HEXAGON) << endl;
	//cout << "OCTAL   : " << obj13.get(OCTAL) << endl << endl;

	//BigIntNerualNet nn;
	//nn.InitNN();
	//nn.FeedForward();
	//nn.ShowResult();
	
	
	return 0;
}