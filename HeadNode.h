/*
	Credit to : Jiangfeng Lin
*/

#pragma once

#include "Node.h"

template <class type>
class HeadNode : public Node<type>
{
private:
	int numOfItems;
public:
	HeadNode();
	void setNumOfItems(int count);
	int getNumOfItems() const;
	~HeadNode();
};

template <class type> HeadNode<type>::HeadNode()
{
	numOfItems = 0;
}

template <class type>
void HeadNode<type>::setNumOfItems(int count) {
	this->numOfItems = count;
}

template <class type>
int HeadNode<type>::getNumOfItems() const {
	return this->numOfItems;
}

template <class type> HeadNode<type>::~HeadNode()
{
}