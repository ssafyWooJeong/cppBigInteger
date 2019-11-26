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
	friend class BigInteger;
	friend bool operator==(const BigInteger& left, const BigInteger& right);
	friend bool operator<(const BigInteger& left, const BigInteger& right);
	friend bool operator>(const BigInteger& left, const BigInteger& right);
	list();
	list(T);
	list(list<T>&);
	~list();
	T& at(unsigned int);
	void append(T);
	int remove(unsigned int);
	int indexOf(T);
	int insert(unsigned int, T);
	int getSize();
};