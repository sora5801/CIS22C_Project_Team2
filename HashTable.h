/*
	Credit to : Jiangfeng Lin, Phuoc Le(help adapt it to follow the rules)
*/

#pragma once

#include "LinkedList.h"
#include "HashPair.h"
#include "MyHash.h"
#include "NotFoundException.h"
#include "Sort.h"
#include <exception>
#include <stdexcept>
#include <utility>

template<typename KeyType, typename ValueType, const int size>
class HashTable
{
public:
	HashTable();
	int hashFunction(const KeyType&);
	void insert(const KeyType& key, const ValueType &value);
	bool remove(const KeyType& key);
	ValueType getItem(const KeyType& key);
	bool contains(const KeyType& key);
	bool isEmpty() const;
	void clear();
	HashPair<KeyType, ValueType> * printByBuckets();
	HashPair<KeyType, ValueType> * getWholeValueSortedByKey();
	int numberOfItems() const;
	int numberOfBuckets() const;
	double loadFactor() const;
	int numberOfCollisions() const;
	double averageNonEmptyBucketSize() const;
	int longestBucket() const;
	~HashTable();
private:
	LinkedList<HashPair<KeyType, ValueType>> *table;
	int numData;
	int numBuckets;
	int numUsedBuckets;
	int numCollisions;
};

template<typename KeyType, typename ValueType, const int size>
HashTable<KeyType, ValueType, size>::HashTable()
	: numBuckets(size), numData(0), numUsedBuckets(0), numCollisions(0)
{
	if (numBuckets <= 0)
	{
		throw std::invalid_argument("Invalid argument: the bucket size should be larger than 0");
	}
	else
	{
		table = new LinkedList<HashPair<KeyType, ValueType>>[size];
	}
}

template<typename KeyType, typename ValueType, const int size>
void HashTable<KeyType, ValueType, size>::insert(const KeyType& key, const ValueType& value)
{
	unsigned index = hashFunction(key);
	int elemsInBucket = table[index].getCount();
	if (elemsInBucket == 3)
	{
		throw std::invalid_argument
		("Invalid argument: insert not possible at this time in the hash table");
	}
	// if the current list is empty, add the number of add buckets
	if (elemsInBucket == 0)
	{
		numUsedBuckets++;
	}
	// else, it means we have collisions
	else
	{
		numCollisions++;
	}
	HashPair<KeyType, ValueType> temp(key, value);
	table[index].add(temp, 1);
	numData++;
}

template<typename KeyType, typename ValueType, const int size>
int HashTable<KeyType, ValueType, size>::hashFunction(const KeyType& key)
{
	MyHash hashValue;
	return static_cast<unsigned>(hashValue(key) % numBuckets);
}

template<typename KeyType, typename ValueType, const int size>
bool HashTable<KeyType, ValueType, size>::remove(const KeyType& key)
{
	unsigned index = hashFunction(key);
	for (int i = 1; i <= table[index].getCount(); ++i)
	{
		if (table[index].getData(i).getKey() == key)
		{
			table[index].deleteNode(i);
			numData--;
			return true;
		}
	}
	return false;
}

template<typename KeyType, typename ValueType, const int size>
ValueType HashTable<KeyType, ValueType, size>::getItem(const KeyType& key)
{
	unsigned index = hashFunction(key);
	for (int i = 1; i <= table[index].getCount(); ++i)
	{
		if (table[index].getData(i).getKey() == key)
		{
			return table[index].getData(i).getValue();
		}
	}
	throw NotFoundException("This key is not stored in the hash table");
}

template<typename KeyType, typename ValueType, const int size>
bool HashTable<KeyType, ValueType, size>::contains(const KeyType& key)
{
	unsigned index = hashFunction(key);
	for (int i = 1; i <= table[index].getCount(); ++i)
	{
		if (table[index].getData(i).getKey() == key)
		{
			return true;
		}
	}
	return false;
}

template<typename KeyType, typename ValueType, const int size>
bool HashTable<KeyType, ValueType, size>::isEmpty() const
{
	return numData == 0;
}

template<typename KeyType, typename ValueType, const int size>
void HashTable<KeyType, ValueType, size>::clear()
{
	for (int i = 0; i < numBuckets; ++i)
	{
		if (!table[i].isEmpty())
		{
			table[i].clear();
		}
	}
}

template<typename KeyType, typename ValueType, const int size>
HashPair<KeyType, ValueType> * HashTable<KeyType, ValueType, size>::printByBuckets()
{
	HashPair<KeyType, ValueType> * hashArr = new HashPair<KeyType, ValueType>[numData];
	int k = 0;
	for (int i = 0; i < numBuckets; ++i)
	{
		for (int j = 1; j <= table[i].getCount(); ++j)
		{
			hashArr[k].setKey(table[i].getData(j).getKey());
			hashArr[k++].setValue(table[i].getData(j).getValue());
		}
	}
	return hashArr;
}

template<typename KeyType, typename ValueType, const int size>
HashPair<KeyType, ValueType>*  HashTable<KeyType, ValueType, size>::getWholeValueSortedByKey()
{
	HashPair<KeyType, ValueType> * sortedArr = new HashPair<KeyType, ValueType>[numData];
	KeyType * keyArr = new KeyType[numData];
	int k = 0;
	for (int i = 0; i < numBuckets; i++)
	{
		for (int j = 0; j < table[i].getCount(); j++)
		{
			keyArr[k++] = table[i].getData(j + 1).getKey();
		}
	}
	quickSortRecursive(keyArr, 0, numData - 1);
	for (int i = 0; i < numData; i++)
	{
		sortedArr[i].setKey(keyArr[i]);
		sortedArr[i].setValue(getItem(keyArr[i]));
	}
	delete[] keyArr;
	return sortedArr;
}

template<typename KeyType, typename ValueType, const int size>
int HashTable<KeyType, ValueType, size>::numberOfItems() const
{
	return numData;
}

template<typename KeyType, typename ValueType, const int size>
int HashTable<KeyType, ValueType, size>::numberOfBuckets() const
{
	return size;
}

template<typename KeyType, typename ValueType, const int size>
double HashTable<KeyType, ValueType, size>::loadFactor() const
{
	return static_cast<double>(numData) / numBuckets;
}

template<typename KeyType, typename ValueType, const int size>
int HashTable<KeyType, ValueType, size>::numberOfCollisions() const
{
	return numCollisions;
}

template<typename KeyType, typename ValueType, const int size>
double HashTable<KeyType, ValueType, size>::averageNonEmptyBucketSize() const
{
	if (numUsedBuckets == 0)
	{
		return 0.0;
	}
	else
	{
		return static_cast<double>(numData) / numUsedBuckets;
	}
}

template<typename KeyType, typename ValueType, const int size>
int HashTable<KeyType, ValueType, size>::longestBucket() const
{
	int max = table[0].getCount();
	for (int i = 1; i < numBuckets; ++i)
	{
		if (table[i].getCount() > max)
		{
			max = table[i].getCount();
		}
	}
	return max;
}

template<typename KeyType, typename ValueType, const int size>
HashTable<KeyType, ValueType, size>::~HashTable()
{
	delete[] table;
	table = nullptr;
}