/*
	Credit to : Phuoc Le
*/

#pragma once

#include "HeadNode.h"
#include <stdexcept>
#include <iostream>

template <class Type> class LinkedList
{
protected:
	HeadNode<Type>* head;
	Node<Type>* tail;
	int count;
public:
	LinkedList();
	~LinkedList();
	void add(const Type&, int);
	bool deleteNode(int);
	bool contains(const Type&) const;
	Node<Type>* findNode(const Type&) const;
	Node<Type>* gotoNode(int) const;
	Type getData(int) const;
	bool deleteData(const Type&);
	int findPosition(const Type&) const;
	int getCount() const;
	bool isEmpty() const;
	void clear();
};

template <class Type> LinkedList<Type>::LinkedList()
{
	head = new HeadNode<Type>;
	tail = head;
	count = 0;
	head->setNumOfItems(0);
}

//add a new node to list at position x by dynamic allocated
//position from 1
template <class Type> void LinkedList<Type>::add(const Type& data, int x)
{
	if (x > count + 1 || x <= 0)
	{
		throw std::invalid_argument("Invalid argument: Invalid position entered!");
	}
	else if (isEmpty())
	{
		head->setNext(new Node<Type>);
		head->getNext()->setItem(data);
		tail = head->getNext();
	}
	else if (x == count + 1)
	{
		Node<Type>* temp = new Node<Type>;
		temp->setItem(data);
		tail->setNext(temp);
		tail = temp;
	}
	else
	{
		Node<Type>* temp2 = new Node<Type>;
		temp2->setItem(data);
		Node<Type>* temp = head;
		for (int i = 0; i < x - 1; i++)
		{
			temp = temp->getNext();
		}
		temp2->setNext(temp->getNext());
		temp->setNext(temp2);
	}
	count++;
	head->setNumOfItems(count);
}

//position form 1
template <class Type> bool LinkedList<Type>::deleteNode(int x)
{
	Type tempItem = this->gotoNode(x)->getItem();
	if (isEmpty())
		return false;
	if (x <= 0 || x > count)
	{
		return false;
	}
	else
	{
		Node<Type>* temp = head;
		Node<Type>* temp2;
		for (int i = 0; i < x - 1; i++)
		{
			temp = temp->getNext();
		}
		temp2 = temp->getNext();
		if (!temp2->getNext()) tail = temp;
		temp->setNext(temp2->getNext());
		temp2->setNext(nullptr);
		delete temp2;
		temp2 = nullptr;
		count--;
		head->setNumOfItems(count);
	}
	return true;
}

//return address of Node has data or nullptr
template <class Type> Node<Type>* LinkedList<Type>::findNode(const Type& data) const
{
	Node<Type>* temp = head->getNext();
	while (temp)
	{
		if (temp->getItem() == data) return temp;
		temp = temp->getNext();
	}
	return nullptr;
}

//
template <class Type>
bool LinkedList<Type>::contains(const Type& data) const
{
	Node<Type>* temp = head->getNext();
	while (temp)
	{
		if (temp->getItem() == data) return true;
		temp = temp->getNext();
	}
	return false;
}

//return position of Node has data or -1
template <class Type> int LinkedList<Type>::findPosition(const Type& data) const
{
	Node<Type>* temp = head->getNext();
	int i = 1;
	while (temp)
	{
		if (temp->getItem() == data) return i;
		temp = temp->getNext();
		i++;
	}
	return -1;
}

//return Node at position x, from 0 or nullptr
template <class Type> Node<Type>* LinkedList<Type>::gotoNode(int x) const
{
	if (x <= 0 || x > count)
		return nullptr;
	Node<Type>* temp = head->getNext();
	for (int i = 1; i < x; i++)
	{
		temp = temp->getNext();
	}
	return temp;
}

//get data at the position of x
template <class Type> Type LinkedList<Type>::getData(int x) const
{
	if (x <= 0 || x > count)
		throw std::invalid_argument("Invalid argument: Invalid position entered!");
	Node<Type>* nodeptr = head->getNext();
	for (int i = 1; i < x; i++)
	{
		nodeptr = nodeptr->getNext();
	}
	Type temp = nodeptr->getItem();
	return temp;
}

//delete data at the position of x
template <class Type> bool LinkedList<Type>::deleteData(const Type& data)
{
	int position = findPosition(data);
	if (position == -1)
	{
		return false;
	}
	return deleteNode(position);
}

//return number of nodes of the list
template <class Type> int LinkedList<Type>::getCount() const
{
	return count;
}

//return true if list is empty
template <class Type> bool LinkedList<Type>::isEmpty() const
{
	return count == 0;
}

//destroy list and set up as new list
template <class Type> void LinkedList<Type>::clear()
{
	Node<Type>* temp = head->getNext();
	if (temp) {
		return;
	}
	Node<Type>* ptr = temp;
	head->setNext(nullptr);
	while (ptr)
	{
		ptr = ptr->getNext();
		delete temp;
		temp = ptr;
	}
	tail = head;
	count = 0;
	head->setNumOfItems(count);
}

//free memory
template <class Type> LinkedList<Type>::~LinkedList()
{
	clear();
	delete head;
	head = nullptr;
	tail = nullptr;
}