/*
	Credit to : Phuoc Le
*/

#pragma once

#include "HashTable.h"
#include "AVLTree.h"

template <class Key, class Value, const int size>
class HeaderNode
{
private:
	int count;
	int nsize;
	HashTable<Key, Value, size> * main_map;
	AVLTree<Value> * main_tree;
public:
	HeaderNode();
	void setCount(int);
	void setSize(int);
	void setMap(HashTable<Key, Value, size> *);
	void setTree(AVLTree<Value> *);

	int getCount() const;
	int getSize() const;
	HashTable<Key, Value, size> * getMap();
	AVLTree<Value> * getTree();
	~HeaderNode();

private:

};

template <class Key, class Value, const int size>
HeaderNode<Key, Value, size>::HeaderNode()
{
	count = 0;
	nsize = size;
	main_map = nullptr;
	main_tree = nullptr;
}

template <class Key, class Value, const int size>
void HeaderNode<Key, Value, size>::setCount(int count)
{
	this->count = count;
}

template <class Key, class Value, const int size>
void HeaderNode<Key, Value, size>::setSize(int size)
{
	nsize = size;
}

template <class Key, class Value, const int size>
void HeaderNode<Key, Value, size>::setMap(HashTable<Key, Value, size> * map)
{
	main_map = map;
}

template <class Key, class Value, const int size>
void HeaderNode<Key, Value, size>::setTree(AVLTree<Value> * tree)
{
	main_tree = tree;
}

template <class Key, class Value, const int size>
int HeaderNode<Key, Value, size>::getCount() const
{
	return count;
}

template <class Key, class Value, const int size>
int HeaderNode<Key, Value, size>::getSize() const
{
	return nsize;
}

template <class Key, class Value, const int size>
HashTable<Key, Value, size> * HeaderNode<Key, Value, size>::getMap()
{
	return main_map;
}

template <class Key, class Value, const int size>
AVLTree<Value> * HeaderNode<Key, Value, size>::getTree()
{
	return main_tree;
}

template <class Key, class Value, const int size>
HeaderNode<Key, Value, size>::~HeaderNode()
{
}