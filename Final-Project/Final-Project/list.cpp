#include "list.h"

template <typename T>
list<T>::list()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
}

template <typename T>
list<T>::list(T data)
{
	node<T> ptr = new node<T>(data);
	head = ptr;
	tail = ptr;
	size = 1;
}


template <typename T>
T list<T>::at(int index)
{
	if (index >= size)
		return nullptr;
	
	node<T>* ptr = head;
	int iter = 0;

	while(iter < index)
	{
		ptr = head->getNext();
		iter++;
	}

	return head->getData();
}

template <typename T>
void list<T>::append(T data)
{
	if(size == 1)
	{
		tail = new node<T>(data);
		head->setNext(tail);
		tail->setPrev(head);
		size = 2;
		
		return;
	}
	
	tail = new node<T>(data, tail, nullptr);
	tail->getPrev()->setNext(tail);
	size++;

	return;
}

template <typename T>
int list<T>::remove(int index)
{
	if(index >= size)
	{
		return -1;
	}

	node<T>* ptr = nullptr;
	
	if(index == 0) // remove head
	{
		ptr = head;
		head = ptr->getNext();
		head->setPrev(nullptr);
	}else if(index == size -1) // remove tail
	{
		ptr = tail;
		tail = ptr->getPrev();
		tail->setNext(nullptr);
	}else
	{
		ptr = head;
		for(int i = 0; i < index; i++)
		{
			ptr = ptr->getNext();
		}

		ptr->getPrev()->setNext(ptr->getNext());
		ptr->getNext()->setPrev(ptr->getPrev());
	}
	
	size--;
	delete ptr;

	return 0;
}

template <typename T>
int list<T>::indexOf(T data)
{
	node<T>* ptr = head;
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
void list<T>::insert(int index, T data)
{
}
