#include <iostream>
#include <string>
#include "header.h"
#include "BigIntNerualNet.h"
#include <fstream>
#include <queue>
using namespace std;

//void BigIntegerTest(BigInteger& left, BigInteger& right)
//{
//	cout << "--------------------------------------" << endl;
//	cout << "Hexagon value of left : " << left.get(HEXAGON) << endl;
//	cout << "Hexagon value of right : " << right.get(HEXAGON) << endl;
//	cout << "--------------------------------------" << endl;
//	cout << "left + right : " << (left + right).get(HEXAGON) << endl;
//	cout << "left - right : " << (left - right).get(HEXAGON) << endl;
//	cout << "left * right : " << (left * right).get(HEXAGON) << endl;
//	cout << "left / right : " << (left / right).get(HEXAGON) << endl;
//	cout << "left % right : " << (left % right).get(HEXAGON) << endl << endl;;
//
//	
//	return;
//}

void BigIntegerTest1(unsigned int op1, unsigned int op2)
{
	BigInteger left(op1);
	BigInteger right(op2);
	BigInteger tmp(1024);
	left.multi(tmp);
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

void BigIntegerTest2(string testfile) // sigma Wi * Xi from i = 1 to n
{
	string tmp;
	ifstream file(testfile.c_str());
	queue<BigInteger> w;
	queue<BigInteger> x;

	getline(file, tmp);
	while(tmp.find(" ") != string::npos)
	{
		string substr = tmp.substr(0, tmp.find(" "));
		BigInteger obj(const_cast<char *>(substr.c_str()));
		w.push(obj); // 만든 list를 사용할 경우(list.append()) C2679 에러가 나며 오른쪽 피연산자로 int 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다. 가 발생하는데
					   // 원인을 모르겠어 템플릿과 관련된 오류로 간주하고, 확인을 하기 위한 부분임으로 C++ STL의 Vector를 사용하겠습니다.(로직상 list를 사용하는게 맞는데, STL list와 만든 list가 이름이 같아
					   // ambigous 하기 때문에 vector를 사용합니다..)
		tmp = tmp.substr(tmp.find(" ") + 1);
	}
	w.push(BigInteger(const_cast<char*>(tmp.c_str())));

	getline(file, tmp);
	while (tmp.find(" ") != string::npos)
	{
		string substr = tmp.substr(0, tmp.find(" "));
		BigInteger obj(const_cast<char*>(substr.c_str()));
		x.push(obj);
		tmp = tmp.substr(tmp.find(" ") + 1);
	}
	x.push(BigInteger(const_cast<char*>(tmp.c_str())));

	BigInteger sum((uint32_t)0);
	
	while( w.size() > 0)
	{
		BigInteger tmpObj = w.front();
		tmpObj.multi(x.front());
		sum.add(tmpObj);
		w.pop();
		x.pop();
	}

	cout << "sigma Wi * Xi from i = 1 to n : " << sum.get(DECIMAL) << endl << endl << endl;

	return;
}

int main()
{
	BigIntegerTest1(UINT32_MAX, UINT32_MAX - 10);
	//BigIntegerTest2("Test2.txt");
	
	//BigInteger a(UINT32_MAX);
	//BigInteger b(UINT32_MAX);
	//BigIntegerTest(a, b);
	
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
	//BigInteger obj3(15);
	//BigInteger obj4(3);
	//cout << "Decimal of 5 : " <<  obj3.get() << endl;
	//cout << "Decimal of 3 : " <<  obj4.get() << endl;
	//cout << obj3.div(obj4).get(8) << endl;;
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

	BigInteger* arr = new BigInteger[3];
	arr[0] = BigInteger(1);
	arr[1] = BigInteger(10);
	arr[2] = BigInteger(7);
	int* ptr[2];
	ptr[0] = new int[3];
	ptr[0][0] = 3;
	ptr[0][1] = 1;
	ptr[0][2] = 2;
	ptr[1] = new int;
	ptr[1][0] = 5;
	
	
	BigIntNuralNet nn;
	nn.TestFFAlgorithm("FFTestFile.txt");
	//nn.InitNN(arr, ptr); // int **sizes; // size[0][0] -> input, size[0][1]->layers size[0][2] -> output, size[1][n] ->hidden's node
	//nn.InitNNTest();
	//nn.FeedForwardTest();
	nn.FeedForward();
	nn.ShowResult();

	
	return 0;
}