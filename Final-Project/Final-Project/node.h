#pragma once
/*
 *  Computer Science and Engineering, Inha University
 *  Programming Language - Final Project
 *
 *  node for dlink
 *  2019.11.21
 */

template <typename T>
class node
{
private:
	node* prev;
	node* next;
	T data;
public:
	node();
	node(T, node*, node*); // data, prev, next
	void setData(T);
	void setPrev(node*);
	void setNext(node*);
	T getData();
	node* getPrev();
	node* getNext();
};