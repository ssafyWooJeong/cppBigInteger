#pragma once
#include "list.h"
#include "stddef.h"

template <typename T>
list<T>::list()
{
	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;
}

template <typename T>
list<T>::list(T data)
{
	node<T>* ptr = new node<T>;
	ptr->setData(data);
	this->head = ptr;
	this->tail = ptr;
	this->size = 1;
}

template <typename T>
list<T>::list(list<T>& old)
{
	this->size = old.size;
	this->head = new node<T>();
	this->head->setData(old.head->getData());
	node<T>* ptr = old.head->getNext();
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
}

template <typename T>
list<T>::~list()
{
	node<T>* ptr = head;
	node<T>* tmp;

	while(ptr != nullptr)
	{
		tmp = ptr;
		ptr = ptr->getNext();
		delete tmp;
	}
}

template <typename T>
T& list<T>::at(unsigned int index)
{
	//if (index >= this->size)
	//	return NULL;
	
	node<T>* ptr = this->head;
	int iter = 0;

	while(iter < index)
	{
		ptr = this->head->getNext();
		iter++;
	}

	return ptr->getData();
}

template <typename T>
void list<T>::append(T data)
{
	if(this->size == 0)
	{
		node<T>* ptr = new node<T>();
		ptr->setData(data);
		this->head = ptr;
		this->tail = ptr;
		this->size = 1;

		return;
	}
	
	if(this->size == 1)
	{
		this->tail = new node<T>();
		this->tail->setData(data);
		this->head->setNext(this->tail);
		this->tail->setPrev(this->head);
		this->size = 2;
		
		return;
	}
	
	this->tail = new node<T>(data, this->tail, nullptr);
	this->tail->getPrev()->setNext(this->tail);
	this->size++;

	return;
}

template <typename T>
int list<T>::remove(unsigned int index)
{
	if(index >= this->size)
	{
		return -1;
	}

	node<T>* ptr = nullptr;
	
	if(index == 0) // remove head
	{
		ptr = this->head;
		this->head = ptr->getNext();
		this->head->setPrev(nullptr);
	}else if(index == this->size -1) // remove tail
	{
		ptr = this->tail;
		this->tail = ptr->getPrev();
		this->tail->setNext(nullptr);
	}else
	{
		ptr = this->head;
		for(int i = 0; i < index; i++)
		{
			ptr = ptr->getNext();
		}

		ptr->getPrev()->setNext(ptr->getNext());
		ptr->getNext()->setPrev(ptr->getPrev());
	}
	
	this->size--;
	delete ptr;

	return 0;
}

template <typename T>
int list<T>::indexOf(T data)
{
	node<T>* ptr = this->head;
	int cnt = 0;
	while(ptr != nullptr)
	{
		if(ptr->getData() == data)
		{
			return cnt;
		}

		ptr = ptr->getNext();
		cnt++;
	}

	return -1;
}

template <typename T>
int list<T>::insert(unsigned int index, T data)
{
	if(this->size < index) // out of bound
	{
		return -1;
	}else if(this->size == index)
	{
		append(data);
		return 0;
	}else if(index == 0)
	{
		this->head = new node<T>(data, nullptr, this->head);
		this->head->getNext()->setPrev(this->head);
		this->size++;
		return 0;
	}

	node<T>* ptr = this->head;
	for(int cnt = 0; cnt < index - 1; cnt++)
	{
		ptr = ptr->getNext();
	}

	ptr->setNext(new node<T>(data, ptr, ptr->getNext()));
	ptr->getNext()->getNext()->setPrev(ptr->getNext());
	this->size++;

	return 0;
}

template <typename T>
int list<T>::getSize()
{
	return this->size;
}

