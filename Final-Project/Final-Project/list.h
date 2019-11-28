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
	friend bool operator>=(const BigInteger& left, const BigInteger& right);

	list<T>& operator=(const list<T>& right)
	{
		if(right.size == 0)
		{
			this->size = 0;
			this->head = nullptr;
			this->tail = nullptr;

			return *this;
		}
		
		this->size = right.size;
		this->head = new node<T>();
		this->tail = this->head;
		this->head->setData(right.head->getData());
		node<T>* ptr = right.head->getNext();
		node<T>* ptr2 = this->head;

		while (ptr != nullptr)
		{
			ptr2->setNext(new node<T>());
			ptr2->getNext()->setData(ptr->getData());
			ptr2->getNext()->setPrev(ptr2);

			ptr = ptr->getNext();
			ptr2 = ptr2->getNext();
			this->tail = ptr2;
		}

		return *this;
	}

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