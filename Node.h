/*
	Credit to : Phuoc Le
*/

#pragma once

template <class type>
class Node
{
private:
	type item;
protected:
	Node<type> * next;
public:
	Node();
	Node(type);
	Node(const Node&) = delete;             // disable copy ctor
	Node& operator=(const Node&) = delete;  // disable assignment operator

	void setItem(type);
	void setNext(Node<type>*);

	type getItem() const;
	Node<type> * getNext() const;

	~Node();
};

template <class type> Node<type>::Node()
{
	item = type();
	next = nullptr;
}

template <class type> Node<type>::Node(type i)
{
	item = i;
	next = nullptr;
}

template <class type> void Node<type>::setItem(type i)
{
	item = i;
}

template <class type> void Node<type>::setNext(Node<type>* nextptr)
{
	next = nextptr;
}

template <class type> type Node<type>::getItem() const
{
	return item;
}

template <class type> Node<type> * Node<type>::getNext() const
{
	return next;
}

template <class type> Node<type>::~Node()
{

}
