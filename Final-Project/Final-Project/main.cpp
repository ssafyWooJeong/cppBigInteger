#include <iostream>
#include "header.h"
using namespace std;


int main()
{
	//list<int> obj;
	//obj.append(1); obj.append(2); obj.append(3); obj.append(4);
	//obj.remove(1);
	//obj.insert(2, 6);

	//cout << obj.at(3) << endl;

	//// list test code

	//uint32_t a = UINT32_MAX;
	//uint32_t b = UINT32_MAX;
	//
	//uint32_t c = 0;
	//c = a + b;

	//BigInteger obj1(UINT32_MAX);
	//BigInteger obj2(UINT32_MAX - 3);
	//
	//obj1.add(obj2);

	//BigInteger obj3(5);
	//BigInteger obj4(3);
	//obj3.sub(obj4);

	//BigInteger obj5(7);
	//BigInteger obj6(10);
	//obj5.sub(obj6);
	
	BigInteger obj7((char *)"-7");
	BigInteger obj8((char*)"0xf");

	cout << obj7.get(OCTAL) << endl;
	cout << obj8.get() << endl;

	obj7.multi(obj8);

	cout << obj7.get() << endl;
	
	
	return 0;
}