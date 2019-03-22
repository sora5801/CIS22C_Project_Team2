/*
	Credit to : Phuoc Le
*/

#pragma once

#include "LinkedList.h"

template <class Type>
class Queue : private LinkedList<Type>
{
public:
	Queue() : LinkedList<Type>() {}
	void enQueue(Type);
	Type deQueue();
	Type peek() const;
	bool isEmpty() const;
	int getCount() const;
	void destroyQueue();
	~Queue() {}
};

template <class Type> void Queue<Type>::enQueue(Type n)
{
	this->add(n, this->count + 1);
}

template <class Type> Type Queue<Type>::deQueue()
{
	if (isEmpty())
		throw std::exception("Pop failed: the queue is now empty!");
	Type temp = this->getData(1);
	this->deleteNode(1);
	return temp;
}

template <class Type> Type Queue<Type>::peek() const
{
	if (isEmpty())
	{
		throw std::exception("Peek failed: the queue is now empty!");
	}
	else
	{
		return this->head->getNext()->getItem();
	}
}

template <class Type> bool Queue<Type>::isEmpty() const
{
	return (this->count == 0);
}

template <class Type> int Queue<Type>::getCount() const
{
	return this->count;
}

template <class Type> void Queue<Type>::destroyQueue()
{
	this->destroyList();
}
