/*
	Credit to : Jiangfeng Lin
*/

#pragma once

template <typename KeyType, typename ValueType>
class HashPair
{
private:
	KeyType key;
	ValueType value;
public:
	HashPair() = default;
	HashPair(const KeyType& key, const ValueType& value);
	HashPair(const HashPair&) = default;
	HashPair& operator=(const HashPair&) = default;
	~HashPair() = default;
	void setKey(const KeyType& key);
	void setValue(const ValueType& value);
	KeyType getKey() const;
	ValueType getValue() const;
};

template <typename KeyType, typename ValueType>
HashPair<KeyType, ValueType>::HashPair(const KeyType& key, const ValueType& value) {
	this->key = key;
	this->value = value;
}

template <typename KeyType, typename ValueType>
void HashPair<KeyType, ValueType>::setKey(const KeyType &key) {
	this->key = key;
}

template <typename KeyType, typename ValueType>
void HashPair<KeyType, ValueType>::setValue(const ValueType &value) {
	this->value = value;
}

template <typename KeyType, typename ValueType>
KeyType HashPair<KeyType, ValueType>::getKey() const {
	return this->key;
}

template <typename KeyType, typename ValueType>
ValueType HashPair<KeyType, ValueType>::getValue() const {
	return this->value;
}