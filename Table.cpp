/*
	Credit to : Jiangfeng Lin
*/

#include "Table.h"

Table::Table(const std::string& tableID)
{
	if (tableID[0] != 'S' && tableID[0] != 'M' && tableID[0] != 'L')
		throw std::invalid_argument("table ID must start with S, M, L");
	this->tableID = tableID;
	this->condition = Condition::available;
}

Table::Table(const std::string& tableID, const Condition& condition)
{
	if (tableID[0] != 'S' && tableID[0] != 'M' && tableID[0] != 'L')
		throw std::invalid_argument("table ID must start with S, M, L");
	this->tableID = tableID;
	this->condition = condition;
}

void Table::setTableID(const std::string& tableID)
{
	if (tableID[0] != 'S' && tableID[0] != 'M' && tableID[0] != 'L')
		throw std::invalid_argument("table ID must start with S, M, L");
	this->tableID = tableID;
}

void Table::setCondition(const Condition& condition)
{
	this->condition = condition;
}

std::string Table::getTableID() const
{
	return tableID;
}

std::string Table::getTableSize() const
{
	if (tableID[0] == 'S')
		return "small";
	else if (tableID[0] == 'M')
		return "medium";
	else // (tableID[0] == 'L')
		return "large";
}

Condition Table::getCondition() const
{
	return condition;
}

std::ostream &operator<<(std::ostream& out, const Table& t)
{
	out << t.tableID;
	return out;
}