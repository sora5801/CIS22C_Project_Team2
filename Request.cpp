/*
	Credit to : Jiangfeng Lin
*/

#include "Request.h"

Request::Request()
{
	requestID = "";
	this->setTime(time(0));
	customerName = "";
	number_of_customers = 0;
	state = RequestState::waiting;
	tableID = "";
	payable = .0f;
	cost = .0f;
	profit = .0f;
	comment = "N/A";
}

Request::Request(const std::string& customerName,
	const int& number_of_customers)
{
	requestID = "";
	this->setTime(time(0));
	this->customerName = customerName;
	this->number_of_customers = number_of_customers;
	state = RequestState::waiting;
	tableID = "";
	payable = .0f;
	cost = .0f;
	profit = .0f;
	comment = "N/A";
}

Request::Request(const std::string& requestID, const time_t& timeinfo,
	const std::string& customerName, const int& number_of_customers)
{
	this->requestID = requestID;
	this->setTime(timeinfo);
	this->customerName = customerName;
	this->number_of_customers = number_of_customers;
	state = RequestState::waiting;
	tableID = "";
	payable = .0f;
	cost = .0f;
	profit = .0f;
	comment = "N/A";
}

void Request::setRequestID(const std::string& requestID)
{
	this->requestID = requestID;
}

void Request::setCustomerName(const std::string& customerName)
{
	this->customerName = customerName;
}

void Request::setTime(const time_t& timeinfo)
{
	time_t time = timeinfo;
	tm *ltm = localtime(&time);
	std::ostringstream sout;
	sout << std::setw(2) << std::setfill('0') << +(1 + ltm->tm_mon) << "/"
		<< std::setw(2) << std::setfill('0') << ltm->tm_mday << "/"
		<< 1900 + ltm->tm_year;
	this->timeinfo = sout.str();
}

void Request::setTime(const std::string& time)
{
	timeinfo = time;
}

void Request::setNumberOfCustomers(const int& number_of_customers)
{
	this->number_of_customers = number_of_customers;
}

void Request::setState(const RequestState& state)
{
	this->state = state;
}

void Request::setComment(const std::string& comment)
{
	this->comment = comment;
}

void Request::setTableID(const std::string& tableID)
{
	this->tableID = tableID;
}

void Request::setPayable(const float& paid)
{
	payable = paid;
}

void Request::setCost(const float& cost)
{
	this->cost = cost;
}

void Request::setProfit(const float& profit)
{
	this->profit = profit;
}

void Request::addDishes(const int& index)
{
	dishes.push_back(index);
	payable += PRICE[index - 1];
	cost += COST[index - 1];
}

void Request::removeDishes(const int& index)
{
	dishes.remove(index);
	payable -= PRICE[index - 1];
	cost -= COST[index - 1];
}

void Request::clear()
{
	requestID = "";
	this->setTime("");
	customerName = "";
	number_of_customers = 0;
	state = RequestState::waiting;
	tableID = "";
	payable = .0f;
	cost = .0f;
	profit = .0f;
	dishes.clear();
}

std::string Request::getRequestID() const
{
	return requestID;
}

std::string Request::getDate() const
{
	return timeinfo;
}

std::string Request::getCustomerName() const
{
	return customerName;
}

int Request::getNumberOfCustomers() const
{
	return number_of_customers;
}

RequestState Request::getState() const
{
	return state;
}

std::string Request::getTableID() const
{
	return tableID;
}

float Request::getPayable() const
{
	return payable;
}

float Request::getCost() const
{
	return cost;
}

float Request::getProfit() const
{
	return profit;
}

std::string Request::getComment() const
{
	return comment;
}

void Request::printDishes() const
{
	for (int i = 0; i < dishes.size(); i++)
	{
		std::cout << dishes[i] << "\t" << DISHES[dishes[i] - 1] << std::endl;
	}
}

void Request::checkout()
{
	profit = payable - cost;
	payable *= 1 + TAX_RATE;
}

bool Request::operator==(const Request& req)
{
	return this->requestID == req.requestID;
}

bool Request::operator!=(const Request& req)
{
	return this->requestID != req.requestID;
}

bool Request::operator<(const Request& req)
{
	return this->requestID < req.requestID;
}

bool Request::operator>(const Request& req)
{
	return this->requestID > req.requestID;
}

bool Request::operator<=(const Request& req)
{
	return this->requestID <= req.requestID;
}

bool Request::operator>=(const Request& req)
{
	return this->requestID >= req.requestID;
}

std::ostream &operator<<(std::ostream& out, const Request& req)
{
	out << std::left << std::setw(12) << req.requestID << std::setw(16) << req.timeinfo
		<< std::setw(12) << req.customerName << std::setw(10) << req.number_of_customers
		<< std::setw(12) << req.tableID << std::setw(12) << std::fixed << std::setprecision(2)
		<< req.payable << std::setw(10) << req.profit << std::setw(20) << req.comment << std::endl;
	return out;
}