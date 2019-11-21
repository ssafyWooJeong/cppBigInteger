#pragma once
#include <cstddef>
#include "node.h"

template <typename T>
node<T>::node()
{
	prev = nullptr;
	next = nullptr;
	data = NULL;
}

template <typename T>
node<T>::node(T _data, node* _prev, node* _next)
{
	data = _data;
	prev = _prev;
	next = _next;
}

template <typename T>
void node<T>::setData(T _data)
{
	data = _data;
}

template <typename T>
void node<T>::setPrev(node* ptr)
{
	prev = ptr;
}

template <typename T>
void node<T>::setNext(node* ptr)
{
	next = ptr;
}

template <typename T>
T node<T>::getData()
{
	return data;
}

template <typename T>
node<T>* node<T>::getPrev()
{
	return prev;
}

template <typename T>
node<T>* node<T>::getNext()
{
	return next;
}
