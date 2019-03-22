/*
	Credit to : Jiangfeng Lin
*/

#pragma once
#include "Request.h"
#include <exception>
#include <stdexcept>

enum class Condition
{
	available, unserved, unchecked
};

class Table
{
private:
	std::string tableID;
	Condition condition;
public:
	Table() : tableID(""), condition(Condition::available) {}
	Table(const std::string&);
	Table(const std::string&, const Condition&);

	void setTableID(const std::string&);
	void setCondition(const Condition&);

	std::string getTableID() const;
	std::string getTableSize() const;
	Condition getCondition() const;

	friend std::ostream &operator<<(std::ostream&, const Table&);
};