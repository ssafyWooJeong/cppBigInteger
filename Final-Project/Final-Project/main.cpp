#include <iostream>
#include "header.h"
using namespace std;

int main()
{
	list<int> obj;
	obj.append(1); obj.append(2); obj.append(3); obj.append(4);
	obj.remove(1);
	obj.insert(2, 6);

	cout << obj.at(3) << endl;

	// list test code

	return 0;
}