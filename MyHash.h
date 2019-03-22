/*
	Credit to : Phuoc Le
*/

#pragma once

#include <string>
#include "Request.h"
#include "Table.h"

class MyHash
{
public:
	MyHash();
	int operator()(const std::string&);
	int operator()(const char&);
	int operator()(const int&);
	int operator()(const float&);
	int operator()(const double&);
	int operator()(const Request&);
	int operator()(const Table&);
	~MyHash();
};

MyHash::MyHash() {}

int MyHash::operator()(const std::string& id)
{
	if (id.length() > 1)
		return std::stoi(id.substr(1));
	else
		throw std::exception("This is an invalid TableID or RequestID!");
}

int MyHash::operator()(const char& c)
{
	return c;
}

int MyHash::operator()(const int& i)
{
	return i;
}

int MyHash::operator()(const float& f)
{
	return static_cast<int>(f);
}

int MyHash::operator()(const double& d)
{
	return static_cast<int>(d);
}

int MyHash::operator()(const Request& req)
{
	if (req.getRequestID().length() > 1)
		return std::stoi(req.getRequestID().substr(1));
	throw std::exception("This is an invalid RequestID!");
}

int MyHash::operator()(const Table& tab)
{
	if (tab.getTableID().length() > 1)
		return std::stoi(tab.getTableID().substr(1));
	throw std::exception("This is an invalid TableID!");
}

MyHash::~MyHash()
{
}