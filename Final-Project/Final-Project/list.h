#pragma once
#include "node.h"

/*
 *  Computer Science and Engineering, Inha University
 *  Programming Language - Final Project
 *
 *  dlink for BigInteger
 *  2019.11.21
 */

template <typename T>
class list
{
private:
	node<T>* head;
	node<T>* tail;
	int size;
public:
	list();
	list(T);
	T at(int);
	void append(T);
	int remove(int);
	int indexOf(T);
	int insert(int, T);
};