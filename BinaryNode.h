/*
	Credit to : Matthew Fu
*/

#pragma once

template<class ItemType>
class BinaryNode
{
private:
	ItemType item;
	BinaryNode<ItemType>* leftChildPtr;
	BinaryNode<ItemType>* rightChildPtr;
public:
	BinaryNode();
	BinaryNode(const ItemType& anItem);
	BinaryNode(const ItemType& anItem, BinaryNode<ItemType>* leftPtr, BinaryNode<ItemType>* rightPtr);

	void setItem(const ItemType& anItem);
	ItemType getItem() const;
	bool isLeaf() const;

	BinaryNode<ItemType>* getLeftChildPtr() const;
	BinaryNode<ItemType>* getRightChildPtr() const;

	void setLeftChildPtr(BinaryNode<ItemType>* leftPtr);
	void setRightChildPtr(BinaryNode<ItemType>* rightPtr);

	~BinaryNode();

};

template<class ItemType>
BinaryNode<ItemType>::BinaryNode() : item(nullptr), leftChildPtr(nullptr), rightChildPtr(nullptr)
{
	item = nullptr;
	leftChildPtr = nullptr;
	rightChildPtr = nullptr;
}  // end default constructor

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem) : item(anItem),
leftChildPtr(nullptr),
rightChildPtr(nullptr)
{
}  // end constructor

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem, BinaryNode<ItemType>* leftPtr,
	BinaryNode<ItemType>* rightPtr) : item(anItem), leftChildPtr(leftPtr), rightChildPtr(rightPtr)
{
}  // end constructor

template<class ItemType>
void BinaryNode<ItemType>::setItem(const ItemType& anItem)
{
	item = anItem;
}  // end setItem

template<class ItemType>
ItemType BinaryNode<ItemType>::getItem() const
{
	return item;
}  // end getItem

template<class ItemType>
bool BinaryNode<ItemType>::isLeaf() const
{
	return ((leftChildPtr == nullptr) && (rightChildPtr == nullptr));
}

template<class ItemType>
void BinaryNode<ItemType>::setLeftChildPtr(BinaryNode<ItemType>* leftPtr)
{
	leftChildPtr = leftPtr;
}  // end setLeftChildPtr

template<class ItemType>
void BinaryNode<ItemType>::setRightChildPtr(BinaryNode<ItemType>* rightPtr)
{
	rightChildPtr = rightPtr;
}  // end setRightChildPtr

template<class ItemType>
BinaryNode<ItemType>* BinaryNode<ItemType>::getLeftChildPtr() const
{
	return leftChildPtr;
}  // end getLeftChildPtr		

template<class ItemType>
BinaryNode<ItemType>* BinaryNode<ItemType>::getRightChildPtr() const
{
	return rightChildPtr;
}  // end getRightChildPtr


template<class ItemType>
BinaryNode<ItemType>::~BinaryNode()
{
}  // end default destructor