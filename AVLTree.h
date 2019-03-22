/*
	Credit to : Phuoc Le
*/

#pragma once

#include "BinarySearchTree.h"
#include <cmath>

//AVL tree base on BinarySearchTree
template <class T> class AVLTree : public BinarySearchTree<T>
{
protected:
	//different hight of left node and right node
	int diff(BinaryNode<T> *);
	BinaryNode<T> * rr_rotation(BinaryNode<T> *);
	BinaryNode<T> * ll_rotation(BinaryNode<T> *);
	BinaryNode<T> * lr_rotation(BinaryNode<T> *);
	BinaryNode<T> * rl_rotation(BinaryNode<T> *);
	BinaryNode<T> * balance(BinaryNode<T> *);
	//find min data of a tree or subtree
	BinaryNode<T> * findMin(BinaryNode<T>*);
	BinaryNode<T> * insertHelper(BinaryNode<T> *, const T&);
	//level from 0, root has level 0
	int getHeightLevel(BinaryNode<T>*);
	//help print data to file with pretty way (look like a tree)
	void inorderHelper(BinaryNode<T>*, std::ofstream&);
	void preorderHelper(BinaryNode<T>*, std::ofstream&);
	void postorderHelper(BinaryNode<T>*, std::ofstream&);
	void breathFirstHelper(BinaryNode<T>*, std::ofstream&);
public:
	AVLTree();
	bool insert(const T&);
	void removeAVL(const T&);
	BinaryNode<T> * getRoot();

	//print data to file with pretty way (look like a tree)
	void printInOrder(std::ofstream&);
	void printPostOrder(std::ofstream&);
	void printPreOrder(std::ofstream&);
	void printBreathFirst(std::ofstream&);
	~AVLTree();
};

//call default constructor of BinarySearchTree
template <class T> AVLTree<T>::AVLTree() : BinarySearchTree<T>()
{
}

//return the root node, need for print in main
template <class T> BinaryNode<T> * AVLTree<T>::getRoot()
{
	return this->rootPtr;
}

template <class T> int AVLTree<T>::diff(BinaryNode<T> *current)
{
	return this->getHeight(current->getLeftChildPtr()) - this->getHeight(current->getRightChildPtr());
}

template <class T> BinaryNode<T> * AVLTree<T>::rr_rotation(BinaryNode<T> * parent)
{
	//do a left rotation
	BinaryNode<T> * temp = parent->getRightChildPtr();
	parent->setRightChildPtr(temp->getLeftChildPtr());
	temp->setLeftChildPtr(parent);
	return temp;
}

template <class T> BinaryNode<T> * AVLTree<T>::ll_rotation(BinaryNode<T> * parent)
{
	//do a right rotation
	BinaryNode<T> * temp = parent->getLeftChildPtr();
	parent->setLeftChildPtr(temp->getRightChildPtr());
	temp->setRightChildPtr(parent);
	return temp;
}

template <class T> BinaryNode<T> * AVLTree<T>::lr_rotation(BinaryNode<T> * parent)
{
	//do a left rotation of left child first
	//then do a right rotation
	BinaryNode<T> * temp = parent->getLeftChildPtr();
	parent->setLeftChildPtr(rr_rotation(temp));
	return ll_rotation(parent);
}

template <class T> BinaryNode<T> * AVLTree<T>::rl_rotation(BinaryNode<T> * parent)
{
	//do a right rotation of right child first
	//then do a left rotation
	BinaryNode<T> * temp = parent->getRightChildPtr();
	parent->setRightChildPtr(ll_rotation(temp));
	return rr_rotation(parent);
}

template <class T> BinaryNode<T> * AVLTree<T>::balance(BinaryNode<T> * current)
{
	//check different
	int bal_factor = diff(current);
	if (bal_factor > 1) //heavy on the left side
	{
		//ll situation
		if (diff(current->getLeftChildPtr()) > 0)
			current = ll_rotation(current);
		else
			current = lr_rotation(current);
	}
	else if (bal_factor < -1) //heavy on the right side
	{
		if (diff(current->getRightChildPtr()) > 0)
			current = rl_rotation(current);
		else
			current = rr_rotation(current);
	}
	return current;
}

template <class T> BinaryNode<T> * AVLTree<T>::insertHelper(BinaryNode<T> * root, const T& data)
{
	if (!root)
	{
		root = new BinaryNode<T>(data);
		return root;
	}
	else if (root->getItem() > data)
	{
		//recursion add to left side
		root->setLeftChildPtr(insertHelper(root->getLeftChildPtr(), data));
		//re-balance the current subtree
		root = balance(root);
	}
	else if (root->getItem() < data)
	{
		//recursion add to right side
		root->setRightChildPtr(insertHelper(root->getRightChildPtr(), data));
		//re-balance current subtree
		root = balance(root);
	}
	else
		//throw exception when the item is in the tree before
		throw std::exception("This item is already in the tree!");
	return root;
}

template <class T> bool AVLTree<T>::insert(const T& data)
{
	try
	{
		this->rootPtr = insertHelper(this->rootPtr, data);
		return true;
	}
	catch (const std::exception& e)
	{
		cerr << e.what();
		return false;
	}
}

template <class T> BinaryNode<T> * AVLTree<T>::findMin(BinaryNode<T> * current)
{
	if (!current)
		return nullptr;
	else if (!current->getLeftChildPtr())
		return current;
	else
		//go through the left side
		return findMin(current->getLeftChildPtr());
}

template <class T> void AVLTree<T>::removeAVL(const T& data)
{
	if (!this->rootPtr)
		return;
	BinaryNode<T> * current = this->rootPtr,
		*parent = this->rootPtr,
		*delNode = nullptr,
		*child = this->rootPtr;
	while (child)
	{
		parent = current;
		current = child;
		//take the node need to delete out
		if (current->getItem() == data)
			delNode = current;
		//run until min node of deleteNode->right or null
		if (current->getItem() > data)
			child = current->getLeftChildPtr();
		else
			child = current->getRightChildPtr();
	}

	if (delNode)
	{
		//current item now is min node in the right of delNode or itself
		delNode->setItem(current->getItem());

		//mean current and delNode are same
		if (current->getLeftChildPtr())
			child = current->getLeftChildPtr(); //left child of delNode (delNode doesn't have right child)
		else
			child = current->getRightChildPtr(); //right child of the min node in the right side of delNode
		if (this->rootPtr->getItem() == data) //tree has only root
			this->rootPtr = child;
		else
		{
			//connect parent of the node that will be deleted out with its child
			if (parent->getLeftChildPtr() == current)
				parent->setLeftChildPtr(child);
			else
				parent->setRightChildPtr(child);
			//re-balance the parent
			parent = balance(parent);
			//re-balance the tree
			this->rootPtr = balance(this->rootPtr);
			delete current;
		}
	}
}

template <class T> int AVLTree<T>::getHeightLevel(BinaryNode<T>* root)
{
	BinaryNode<T>* temp = root;
	BinaryNode<T>* parent = this->rootPtr;
	int heightLevel = 0;//root level 0
	while (parent != temp)
	{
		//count the height from root to the current node
		heightLevel++;
		if (parent->getItem() > temp->getItem())
			parent = parent->getLeftChildPtr();
		else
			parent = parent->getRightChildPtr();
	}
	return heightLevel;
}

//inorder helper
//print out to file with pretty look
template <class T> void AVLTree<T>::inorderHelper(BinaryNode<T>* root, std::ofstream& out)
{
	if (!root)
		return;
	int heightLevel = getHeightLevel(root);
	inorderHelper(root->getLeftChildPtr(), out);
	for (int i = 0; i < heightLevel; i++)
	{
		out << "\t";
	}
	out << root->getItem() << std::endl;
	inorderHelper(root->getRightChildPtr(), out);
}

template <class T> void AVLTree<T>::printInOrder(ofstream& out)
{
	inorderHelper(this->rootPtr, out);
}

//preorder helper
//print out to file with pretty look
template <class T> void AVLTree<T>::preorderHelper(BinaryNode<T> * root, std::ofstream& out)
{
	if (!root)
		return;
	int heightLevel = getHeightLevel(root);
	for (int i = 0; i < heightLevel; i++)
	{
		out << "\t";
	}
	out << root->getItem() << std::endl;
	preorderHelper(root->getLeftChildPtr(), out);
	preorderHelper(root->getRightChildPtr(), out);
}

template <class T> void AVLTree<T>::printPreOrder(ofstream& out)
{
	preorderHelper(this->rootPtr, out);
}

//postorder helper
//print out to file with pretty look
template <class T> void AVLTree<T>::postorderHelper(BinaryNode<T> * root, std::ofstream& out)
{
	if (!root)
		return;
	int heightLevel = getHeightLevel(root);
	postorderHelper(root->getLeftChildPtr(), out);
	postorderHelper(root->getRightChildPtr(), out);
	for (int i = 0; i < heightLevel; i++)
	{
		out << "\t";
	}
	out << root->getItem() << std::endl;
}

template <class T> void AVLTree<T>::printPostOrder(ofstream& out)
{
	postorderHelper(this->rootPtr, out);
}

//breadth first helper using queue
//print out to file with pretty look
template <class T> void AVLTree<T>::breathFirstHelper(BinaryNode<T> * root, std::ofstream& out)
{
	if (!root)
		return;
	int height = this->getHeight(root);
	int level = 0;
	Queue<BinaryNode<T>*> queue;
	BinaryNode<T> * temp;
	BinaryNode<T> * empty = new BinaryNode<T>(); //if a node doesn't hava a child, the child will point to empty
	queue.enQueue(root);
	queue.enQueue(nullptr);//end of level 0
	while (!queue.isEmpty())
	{
		temp = queue.deQueue();

		if (temp == nullptr) //end of current level
		{
			if (!queue.isEmpty())
				queue.enQueue(nullptr);//add the end of a level
			out << std::endl;
			level++;
		}
		else
		{
			//print space to keep look like a tree
			for (int i = 0; i < pow(2, height - level) / 2; i++)
			{
				out << "                ";
			}
			if (temp != empty)
				out << temp->getItem();
			else
				out << "      null      ";
			for (int i = 0; i < pow(2, height - level) / 2 - 1; i++)
			{
				out << "                ";
			}
			//add the next level node into the queue
			if (level < height) {
				if (temp->getLeftChildPtr())
					queue.enQueue(temp->getLeftChildPtr());
				else
					queue.enQueue(empty);
				if (temp->getRightChildPtr())
					queue.enQueue(temp->getRightChildPtr());
				else
					queue.enQueue(empty);
			}
		}
	}
	//free empty memory
	delete empty;
}

template <class T> void AVLTree<T>::printBreathFirst(ofstream& out)
{
	breathFirstHelper(this->rootPtr, out);
}

template <class T> AVLTree<T>::~AVLTree()
{
	//call clear to free tree memorry
	this->clear();
}