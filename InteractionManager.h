/*
	Credit to : Jiangfeng Lin
*/

#pragma once

#include "OperationManager.h"

class InteractionManager
{
public:
	InteractionManager();
	~InteractionManager();
	// the handle function is one big loop that interacts with users
	// and it calls operation manager to get access to the data structures
	void handle();
private:
	void mainMenu() const;
private:
	char choice;
	string button, name, group, id;
	OperationManager* manager;
};

InteractionManager::InteractionManager()
{
	manager = new OperationManager;
}

InteractionManager::~InteractionManager()
{
	delete manager;
	manager = nullptr;
}

void InteractionManager::handle()
{
	manager->setupTableInfo();
	manager->readCompletedRequest();
	std::string requestID;
	do
	{
		system("cls");
		mainMenu();
		getline(cin, button);
		while (button != "0" && button != "1" && button != "2" && button != "3"
			&& button != "4" && button != "5" && button != "6" && button != "9")
		{
			cout << "Please enter a valid button: ";
			getline(cin, button);
		}
		switch (stoi(button))
		{
			// analysis
		case 0:
			system("cls");
			manager->requestHashTableEfficiency();
			cout << endl;
			manager->tableHashTableEfficiency();
			cout << endl;
			manager->tableRequestHashTableEfficiency();
			cout << endl;
			break;
			// add new customer
		case 1:
			do
			{
				system("cls");
				cout << "          <New Customer>" << endl;
				cout << "-------------------------------" << endl;
				cout << "Name: ";
				getline(cin, name);
				while (!manager->validCustomerName(name))
				{
					getline(cin, name);
				}
				cout << "Group size: ";
				getline(cin, group);
				while (!manager->validGroupSize(group))
				{
					getline(cin, group);
				}
				manager->addNewCustomer(name, group);
				cout << endl << "Continue adding new customers? (y/n): ";
				cin >> choice;
				cin.ignore();
			} while (choice == 'Y' || choice == 'y');
			break;
			// assign tables
		case 2:
			system("cls");
			manager->printQueueInfo(SIZE::small);
			manager->assignTables(SIZE::small, false);
			system("cls");
			manager->printQueueInfo(SIZE::medium);
			manager->assignTables(SIZE::medium, false);
			system("cls");
			manager->printQueueInfo(SIZE::large);
			manager->assignTables(SIZE::large, false);
			system("cls");
			manager->printQueueInfo(SIZE::small);
			manager->printQueueInfo(SIZE::medium);
			manager->printQueueInfo(SIZE::large);
			system("pause");
			break;
			// food ordering
		case 3:
			do
			{
				system("cls");
				manager->printUnservedTables();
				if (!manager->hasUnservedTables())
				{
					break;
				}
				cout << "Please enter the table ID to order food: ";
				cin >> id;
				cin.ignore();
				while (!manager->validUnservedTableId(id))
				{
					cin >> id;
					cin.ignore();
				}
				system("cls");
				manager->processFoodOrder(id);
				manager->askContinueOrder(choice);
			} while ((choice == 'Y' || choice == 'y'));
			break;
			// check out
		case 4:
			do
			{
				system("cls");
				manager->printUncheckedTables();
				if (!manager->hasUncheckedTables())
				{
					break;
				}
				cout << "Please enter the table ID to check out: ";
				cin >> id;
				cin.ignore();
				while (!manager->validUncheckedTableId(id))
				{
					cin >> id;
					cin.ignore();
				}
				system("cls");
				manager->processCheckout(id);
				manager->askContinueCheck(choice);
			} while (choice == 'Y' || choice == 'y');
			break;
			// completed requests
		case 5:
			manager->printCompletedRequests();
			if (manager->hasCompletedRequests())
			{
				cout << endl;
				cout << "Would you like to delete the root node? (y/n): ";
				cin >> choice;
				cin.ignore();
				if (choice == 'Y' || choice == 'y')
				{
					manager->delRootCompletedRequest();
				}
				cout << endl;
				cout << "Would you like to search a specific request? (y/n): ";
				cin >> choice;
				cin.ignore();
				if (choice == 'Y' || choice == 'y')
				{
					cout << "Please enter the request order ID that you want to search: ";
					cin >> requestID;
					while (!manager->validRequestID(requestID))
					{
						cin >> requestID;
					}
					manager->promptSearchAndComment(requestID);
				}
				cout << endl;
				system("pause");
			}
			break;
			// all requests
		case 6:
			system("cls");
			if (!manager->hasRequests())
			{
				cout << "                                   <All Requests>" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "There are no requests records so far!" << endl;
				system("pause");
			}
			else
			{
				manager->printAllRequestMap();
				cout << endl;
				cout << "Sorting by request ID: " << endl;
				manager->printAllSortedRequestMap();
			}
			break;
		case 9:
			break;
		default:
			break;
		}
	} while (button != "9");
	cout << "Thank you for using this software" << endl;
	//print tree to file
	manager->storeCompletedRequests();
}

void InteractionManager::mainMenu() const
{
	cout << "------------------------------------------" << endl;
	cout << "                <Main Menu>" << endl;
	cout << "  0  ---------- Efficiency" << endl;
	cout << "  1  ---------- Add New Customers" << endl;
	cout << "  2  ---------- Tables Assignment" << endl;
	cout << "  3  ---------- Food Ordering" << endl;
	cout << "  4  ---------- Check-out" << endl;
	cout << "  5  ---------- Completed Requests" << endl;
	cout << "  6  ---------- All Requests" << endl;
	cout << "  9 ----------- Exit" << endl;
	cout << "------------------------------------------" << endl;
	cout << endl << "Please enter your choice: ";
}