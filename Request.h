/*
	Credit to : Jiangfeng Lin
*/

#pragma once
#pragma warning(disable : 4996)

#include "LinkedList.h"
#include "vector.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

enum class RequestState
{
	waiting, unserved, unchecked, completed
};

const std::string DISHES[11] = { "Polish Sausage Scrambles", "Italian Omelette", "Cajun Chicken Benedict", "Hamburger Steak",
							"Avocado Sandwich", "Matcha Tiramisu", "Strawberry Chiffon", "Mango Millicrepe", "Juice", "Tea", "Coffee" };
const float PRICE[11] = { 13.95f, 12.95f, 12.95f, 14.95f, 10.95f, 4.95f, 5.95f, 6.95f, 3.95f, 2.95f, 2.95f };
const float COST[11] = { 12.46f, 11.73f, 10.98f, 12.66f, 8.98f, 3.57f, 4.82f, 5.31f, 2.88f, 1.96f, 1.84f };
const float TAX_RATE = .0725f;

class Request
{
private:
	std::string requestID;
	std::string timeinfo;
	std::string customerName;
	int number_of_customers;
	RequestState state;
	std::string tableID;
	float payable; //how much should the customer pay(price+tax)
	float cost;
	float profit;
	std::string comment;
	vector<int> dishes;
public:
	Request();
	Request(const std::string& customerName, const int& number_of_customers); // when the staff wants to add the requests
	Request(const std::string&, const time_t&, const std::string&, const int&); // for presentation's purpose, to fastly read in some sample requests in file
	Request(const Request&) = default;
	Request& operator=(const Request&) = default;

	void setRequestID(const std::string&);
	void setTime(const time_t&);
	void setTime(const std::string&);
	void setCustomerName(const std::string&);
	void setNumberOfCustomers(const int&);
	void setState(const RequestState&);
	void setTableID(const std::string&);
	void setPayable(const float&);
	void setCost(const float&);
	void setProfit(const float&);
	void setComment(const std::string&);
	void addDishes(const int&);
	void removeDishes(const int&);
	void clear();

	std::string getRequestID() const;
	std::string getDate() const;
	std::string getCustomerName() const;
	int getNumberOfCustomers() const;
	RequestState getState() const;
	std::string getTableID() const;
	float getPayable() const;
	float getCost() const;
	float getProfit() const;
	std::string getComment() const;
	void printDishes() const;

	void checkout();

	bool operator==(const Request&);
	bool operator!=(const Request&);
	bool operator<(const Request&);
	bool operator>(const Request&);
	bool operator<=(const Request&);
	bool operator>=(const Request&);

	friend std::ostream &operator<<(std::ostream&, const Request&);
};