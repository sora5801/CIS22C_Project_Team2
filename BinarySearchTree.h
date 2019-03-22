/*
	Credit to : Matthew Fu
*/

#pragma once

#include "BinaryTreeInterface.h"
#include "BinaryNode.h"
#include "BinaryNodeTree.h"
#include "NotFoundException.h"
#include "PrecondViolatedExcep.h"
#include "Queue.h"
#include "Request.h"
#include <fstream>
#include <stdio.h> 
ofstream textfile;

//Many of BinarySearchTree's operations are inherited from BinaryNodeTree
template <class ItemType>
class BinarySearchTree : public BinaryNodeTree<ItemType>
{
protected:
	BinaryNode<ItemType>* rootPtr;
	BinaryNode<ItemType>* insertInorder(BinaryNode<ItemType>* subTreePtr, BinaryNode<ItemType>* newNode);
	BinaryNode<ItemType>* removeValue(BinaryNode<ItemType>* subTreePtr, const ItemType target, bool& success);
	BinaryNode<ItemType> *removeNode(BinaryNode<ItemType>* nodePtr);
	BinaryNode<ItemType> *removeLeftmostNode(BinaryNode<ItemType>* subTreePtr, ItemType& inorderSuccessor);
	BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType& target) const;
public:
	BinarySearchTree();
	BinarySearchTree(const ItemType& rootItem);
	BinarySearchTree(const BinarySearchTree<ItemType>& tree);
	virtual ~BinarySearchTree();
	//This function is necessary for the Level Order Traversal to work.
	BinaryNode<ItemType>* Root() { return rootPtr; }
	BinaryNode<ItemType>* newNode(ItemType key)
	{
		BinaryNode<ItemType>* temp = new BinaryNode<ItemType>;
		temp->getItem() = key;
		temp->getLeftChildPtr() = temp->getRightChildPtr() = nullptr;
		return temp;
	}
	bool isEmpty() const;
	int getHeight() const;
	int getHeight(BinaryNode<ItemType>*) const;
	int getNumberOfNodes() const;
	bool add(const ItemType& newEntry);
	bool remove(const ItemType& anEntry);
	void clear();
	ItemType getEntry(const ItemType& anEntry) const;
	bool contains(const ItemType& anEntry) const;

	void preorderTraverse(void visit(ItemType&)) const;
	void inorderTraverse(void visit(ItemType&)) const;
	void postorderTraverse(void visit(ItemType&)) const;

	//Also called BFS. This is the thing that is we're going to use to save data to the file.
	//If it works...
   // void levelOrderTraversal(BinaryNode<ItemType>*root, FILE *fp);
	void levelOrderTraversal(BinaryNode<ItemType>*root, ofstream& outFile);
	//This is the one where not printing out is happening.
	void levelOrderTraversal(BinaryNode<ItemType>*root);
	void printLevelOrder();
	BinarySearchTree<ItemType>&operator=(const BinarySearchTree<ItemType> & rightHandSide);

};
template <class ItemType>
void BinarySearchTree<ItemType>::printLevelOrder()
{
	levelOrderTraversal(rootPtr);
}


template<class ItemType>
void BinarySearchTree<ItemType>::levelOrderTraversal(BinaryNode<ItemType>* root, ofstream& outFile)
{
	//The queue that we might use
	Queue<BinaryNode<ItemType>*> queue;
	queue.enQueue(root);

	BinaryNode<ItemType>* curr = nullptr;

	while (!queue.isEmpty())
	{
		curr = queue.peek();
		queue.deQueue();

		outFile << curr->getItem();
		// cout << curr->getItem() << " ";

		if (curr->getLeftChildPtr())
			queue.enQueue(curr->getLeftChildPtr());

		if (curr->getRightChildPtr())
			queue.enQueue(curr->getRightChildPtr());
	}
}
template<class ItemType>
void BinarySearchTree<ItemType>::levelOrderTraversal(BinaryNode<ItemType>* root)
{
	//The queue that we might use
	Queue<BinaryNode<ItemType>*> queue;
	queue.enQueue(root);

	BinaryNode<ItemType>* curr = nullptr;

	while (!queue.isEmpty())
	{
		curr = queue.peek();
		queue.deQueue();


		cout << curr->getItem();

		if (curr->getLeftChildPtr())
			queue.enQueue(curr->getLeftChildPtr());

		if (curr->getRightChildPtr())
			queue.enQueue(curr->getRightChildPtr());
	}
}
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::insertInorder(BinaryNode<ItemType>* subTreePtr,
	BinaryNode<ItemType>* newNodePtr) {
	if (subTreePtr == nullptr)
		return newNodePtr;
	else {
		if (subTreePtr->getItem() > newNodePtr->getItem()) {
			subTreePtr->setLeftChildPtr(insertInorder(subTreePtr->getLeftChildPtr(), newNodePtr));
		}
		else {
			subTreePtr->setRightChildPtr(insertInorder(subTreePtr->getRightChildPtr(), newNodePtr));
		}
		return subTreePtr;
	}  // end else
}  // end insertInorder

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeValue(BinaryNode<ItemType>* subTreePtr,
	const ItemType target,
	bool& success)
{
	if (subTreePtr == nullptr)
	{
		// Not found here
		success = false;
		return nullptr;
	}
	if (subTreePtr->getItem() == target)
	{
		// Item is in the root of some subtree
		subTreePtr = removeNode(subTreePtr);
		success = true;
		return subTreePtr;
	}
	else
	{
		if (subTreePtr->getItem() > target)
			// Search the left subtree
			subTreePtr->setLeftChildPtr(removeValue(subTreePtr->getLeftChildPtr(), target, success));
		else
			// Search the right subtree
			subTreePtr->setRightChildPtr(removeValue(subTreePtr->getRightChildPtr(), target, success));

		return subTreePtr;
	}  // end if
}  // end removeValue

/** removes specfied child ponter. Improves modularity by removing duplicate code
 */
template<class ItemType>
BinaryNode<ItemType>* removeChildNode(BinaryNode<ItemType>* ChildNodePtr) {
	BinaryNode<ItemType>* nodeToConnectPtr = ChildNodePtr->getRightChildPtr();
	delete ChildNodePtr;
	ChildNodePtr = nullptr;
	return nodeToConnectPtr;
}

/* remove specified node
	Case 1) Node is a leaf - it is deleted
	Case 2) Node has one child - parent adopts child
	Case 3) Node has two children:
			Traditional implementation: Find successor node.
			Alternate implementation: Find successor value and replace node's value;
					alternate does not need pass-by-reference
*/
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeNode(BinaryNode<ItemType>* nodePtr) {
	if (nodePtr->isLeaf()) {
		delete nodePtr;
		return (nodePtr = nullptr); // Assign and return (perhaps student should have separate statements)
	}
	else if (nodePtr->getLeftChildPtr() == nullptr)  // Has rightChild only
	{
		//return removeChildNode(nodePtr->getRightChildPtr());
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightChildPtr();
		delete nodePtr;
		nodePtr = nullptr;
		return nodeToConnectPtr; /**/
	}
	else if (nodePtr->getRightChildPtr() == nullptr) // Has left child only
	{
		//return removeChildNode(nodePtr->getLeftChildPtr());
		/* */BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftChildPtr();
		delete nodePtr;
		nodePtr = nullptr;
		return nodeToConnectPtr;   /**/
	}
	else                                             // Has two children
	{
		// Traditional way to remove a value in a node with two children
		ItemType newNodeValue;
		nodePtr->setRightChildPtr(removeLeftmostNode(nodePtr->getRightChildPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;

	}  // end if
}  // end removeNode

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
	ItemType& inorderSuccessor)
{
	if (nodePtr->getLeftChildPtr() == nullptr)
	{
		inorderSuccessor = nodePtr->getItem();
		return removeNode(nodePtr);
	}
	else
	{
		nodePtr->setLeftChildPtr(removeLeftmostNode(nodePtr->getLeftChildPtr(), inorderSuccessor));
		return nodePtr;
	}  // end if      
}  // end removeLeftmostNode

/*
template<class ItemType>
ItemType BinarySearchTree<ItemType>::findSuccessorValue(BinaryNode<ItemType>* subTreePtr)
{
   BinaryNode<ItemType>* myLeftChildPtr = subTreePtr->getLeftChildPtr();
   if (myLeftChildPtr->getLeftChildPtr() == NULL) {
	  ItemType nodeItemValue = myLeftChildPtr->getItem();
	  subTreePtr->setLeftChildPtr(removeNode(myLeftChildPtr));
	  return nodeItemValue;
   }
   else
   {
	  return findSuccessorValue(subTreePtr->getLeftChildPtr());
   }  // end if
}  // end findSuccessorValue
*/

// Override findNode because now we can use a binary search:
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* subTreePtr,
	const ItemType& target) const
{
	// Uses a binary search 
	if (subTreePtr == nullptr)
		return nullptr;                        // Not found 
	else if (subTreePtr->getItem() == target)
		return subTreePtr;                     // Found
	else if (subTreePtr->getItem() > target)
		// Search left subtree
		return findNode(subTreePtr->getLeftChildPtr(), target);
	else
		// Search right subtree
		return findNode(subTreePtr->getRightChildPtr(), target);
}  // end findNode


//////////////////////////////////////////////////////////////
//      PUBLIC METHODS BEGIN HERE
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
//      Constructor and Destructor Section
//////////////////////////////////////////////////////////////

template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree() : rootPtr(nullptr)
{
}  // end default constructor

template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const ItemType& rootItem)
{
	rootPtr = new BinaryNode<ItemType>(rootItem, nullptr, nullptr);
}  // end constructor

template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const BinarySearchTree<ItemType>& treePtr)
{
	rootPtr = this->copyTree(treePtr.rootPtr); // Call inherited method
}  // end copy constructor

template<class ItemType>
BinarySearchTree<ItemType>::~BinarySearchTree()
{
	this->destroyTree(rootPtr); // Call inherited method
}  // end destructor


//////////////////////////////////////////////////////////////
//      Public BinaryTreeInterface Methods Section
//////////////////////////////////////////////////////////////

template<class ItemType>
bool BinarySearchTree<ItemType>::isEmpty() const
{
	return rootPtr == nullptr;
}  // end isEmpty

template<class ItemType>
int BinarySearchTree<ItemType>::getHeight() const
{
	return this->getHeightHelper(rootPtr); // Call inherited method
}  // end getHeight of tree

template<class ItemType>
int BinarySearchTree<ItemType>::getHeight(BinaryNode<ItemType> * current) const
{
	return this->getHeightHelper(current); // Call inherited method
}  // end getHeight

template<class ItemType>
int BinarySearchTree<ItemType>::getNumberOfNodes() const
{
	return this->getNumberOfNodesHelper(rootPtr); // Call inherited method
}  // end getNumberOfNodes

template<class ItemType>
void BinarySearchTree<ItemType>::clear()
{
	this->destroyTree(rootPtr); // Call inherited method
	rootPtr = nullptr;
}  // end clear

template<class ItemType>
bool BinarySearchTree<ItemType>::add(const ItemType& newData)
{
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newData);
	rootPtr = insertInorder(rootPtr, newNodePtr);

	return true;
}  // end add

template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType& target)
{
	bool isSuccessful = false;
	rootPtr = removeValue(rootPtr, target, isSuccessful);
	return isSuccessful;
}  // end remove

// Override getEntry to use our improved findNode:
template<class ItemType>
ItemType BinarySearchTree<ItemType>::getEntry(const ItemType& anEntry) const  // throw(NotFoundException)
{
	BinaryNode<ItemType>* nodeWithEntry = findNode(rootPtr, anEntry);
	if (nodeWithEntry == nullptr) {
		cout << "Entry not found in tree." << endl;
		//throw NotFoundException("Entry not found in tree.");
	} // else
	return nodeWithEntry->getItem();
}  // end getEntry

// Override contains to use our improved findNode:
template<class ItemType>
bool BinarySearchTree<ItemType>::contains(const ItemType& anEntry) const
{

	return findNode(rootPtr, anEntry);  // NULL is same as false
}  // end contains


//////////////////////////////////////////////////////////////
//      Public Traversals Section
//////////////////////////////////////////////////////////////

template<class ItemType>
void BinarySearchTree<ItemType>::preorderTraverse(void visit(ItemType&)) const
{
	this->preorder(visit, rootPtr); // Call inherited method
}  // end preorderTraverse

template<class ItemType>
void BinarySearchTree<ItemType>::inorderTraverse(void visit(ItemType&)) const
{
	this->inorder(visit, rootPtr); // Call inherited method
}  // end inorderTraverse

template<class ItemType>
void BinarySearchTree<ItemType>::postorderTraverse(void visit(ItemType&)) const
{
	this->postorder(visit, rootPtr); // Call inherited method
}  // end postorderTraverse

/* template<class ItemType>
void BinarySearchTree<ItemType>::inorderTraverseIterative(void visit(ItemType&)) const{ // rcs 24 Nov 2013
   this->inorderIterative(visit, rootPtr); // Call inherited method
}
*/


//////////////////////////////////////////////////////////////
//      Overloaded Operator 
//////////////////////////////////////////////////////////////

template<class ItemType>
BinarySearchTree<ItemType>& BinarySearchTree<ItemType>::
operator=(const BinarySearchTree<ItemType>& rightHandSide)
{
	if (!isEmpty())
		clear();
	this = copyTree(&rightHandSide); // Call inherited method

	return *this;
}  // end operator=