/*
	Credit to : Jiangfeng Lin
*/

#pragma once

#include "HeaderNode.h"
#include "Table.h"
#include "Queue.h"
#include "Request.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <fstream>
using namespace std;

const int NUM_TABLES = 30;
const int SERVING_HASHTABLE_CAPACITY = 30;
const int MAIN_HASHTABLE_CAPACITY = 100;
const int SMALL_TABLE_CAPACITY = 2;
const int MEDIUM_TABLE_CAPACITY = 4;
const int LARGE_TABLE_CAPACITY = 8;
const string COMPLETED_REQUEST_FILE = "completed_requests.txt";

enum class SIZE
{
	small, medium, large
};

class OperationManager
{
public:
	OperationManager();
	~OperationManager();
	void setupTableInfo();
	bool hasUnservedTables() const;
	bool hasUncheckedTables() const;
	bool validCustomerName(const string& name) const;
	bool validGroupSize(const string& size) const;
	void printQueueInfo(const SIZE& size);
	void assignTables(const SIZE& size, const bool& isTestMode);
	void addNewCustomer(const string& name, const string& group);
	void printUnservedTables() const;
	void printUncheckedTables() const;
	bool hasRequests() const;
	bool hasCompletedRequests() const;
	void printCompletedRequests();
	bool validUnservedTableId(const string& id);
	bool validUncheckedTableId(const string& id);
	void processFoodOrder(const string& id);
	void processCheckout(const string& id);
	void askContinueOrder(char& choice);
	void askContinueCheck(char& choice);
	void storeCompletedRequests() const;
	void readCompletedRequest();
	bool validRequestID(const string& requestID);
	void promptSearchAndComment(const string& requestID);
	void printTreeByRoot() const;
	void printTreeToFile(ofstream&) const;
	void printAllRequestMap() const;
	void printAllSortedRequestMap() const;
	void requestHashTableEfficiency() const;
	void tableHashTableEfficiency() const;
	void tableRequestHashTableEfficiency() const;
	void delRootCompletedRequest();
private:
	void printFoodMenu() const;
	bool validFoodId(const string& id) const;
private:
	// store the waiting requests in the queue
	Queue<Request*>* small_request_queue;
	Queue<Request*>* medium_request_queue;
	Queue<Request*>* large_request_queue;
	// store the available tables in the queue
	Queue<Table>* small_table_queue;
	Queue<Table>* medium_table_queue;
	Queue<Table>* large_table_queue;
	// store unserved requests & served requests
	LinkedList<Request*>* unserved_request_list;
	LinkedList<Request*>* served_request_list;
	// hash tables & bst
	HashTable<string, Table, SERVING_HASHTABLE_CAPACITY>* table_map; // table ID -> table object
	HashTable<string, Request*, SERVING_HASHTABLE_CAPACITY>* serving_request_map; // table ID -> request object
	HashTable<string, Request*, MAIN_HASHTABLE_CAPACITY>* all_request_map; // request ID -> request object
	AVLTree<Request*>* completed_requests;
	Queue<Request*>* temp_request_queue;
	Queue<Table>* temp_table_queue;
	Request * temp_request;
	Table temp_table;
	int requestIdCounter;
	HeaderNode<string, Request*, MAIN_HASHTABLE_CAPACITY> headNode;
};

OperationManager::OperationManager()
{
	small_request_queue = new Queue<Request*>;
	medium_request_queue = new Queue<Request*>;
	large_request_queue = new Queue<Request*>;
	small_table_queue = new Queue<Table>;
	medium_table_queue = new Queue<Table>;
	large_table_queue = new Queue<Table>;
	unserved_request_list = new LinkedList<Request*>;
	served_request_list = new LinkedList<Request*>;
	table_map = new HashTable<string, Table, SERVING_HASHTABLE_CAPACITY>; // table ID -> table object
	serving_request_map = new HashTable<string, Request*, SERVING_HASHTABLE_CAPACITY>; // table ID -> request object
	all_request_map = new HashTable<string, Request*, MAIN_HASHTABLE_CAPACITY>;
	completed_requests = new AVLTree<Request*>;
	temp_request_queue = new Queue<Request*>;
	temp_table_queue = new Queue<Table>;
	headNode.setMap(all_request_map);
	headNode.setTree(completed_requests);
	ifstream in("last_request_id.txt");
	if (!in.fail()) { in >> requestIdCounter; }
	else { requestIdCounter = 1; }
	in.close();
}

OperationManager::~OperationManager()
{
	HashPair<string, Request *> * arr = all_request_map->printByBuckets();
	Request * temp;
	int size = all_request_map->numberOfItems();
	delete small_request_queue;
	small_request_queue = nullptr;
	delete medium_request_queue;
	small_request_queue = nullptr;
	delete large_request_queue;
	large_request_queue = nullptr;
	delete small_table_queue;
	small_table_queue = nullptr;
	delete medium_table_queue;
	medium_table_queue = nullptr;
	delete large_table_queue;
	large_table_queue = nullptr;
	delete unserved_request_list;
	unserved_request_list = nullptr;
	delete served_request_list;
	served_request_list = nullptr;
	delete table_map;
	table_map = nullptr;
	delete serving_request_map;
	serving_request_map = nullptr;
	delete all_request_map;
	delete completed_requests;
	completed_requests = nullptr;
	temp_request_queue = nullptr;
	temp_table_queue = nullptr;
	for (int i = 0; i < size; i++)
	{
		temp = arr[i].getValue();
		temp->clear();
		delete temp;
		arr[i].setValue(nullptr);
	}
	delete[] arr;

	ofstream out("last_request_id.txt");
	if (!out.fail())
		out << requestIdCounter;
	out.close();
}

void OperationManager::setupTableInfo()
{
	for (int i = 0; i < NUM_TABLES; i++)
	{
		if (i < 15)
		{
			//small tables 1-15
			small_table_queue->enQueue(Table("S" + to_string(i + 1), Condition::available));
			table_map->insert("S" + to_string(i + 1), Table("S" + to_string(i + 1), Condition::available));
		}
		else if (i < 25)
		{
			//medium tables 1-10
			medium_table_queue->enQueue(Table("M" + to_string(i - 14), Condition::available));
			table_map->insert("M" + to_string(i - 14), Table("M" + to_string(i - 14), Condition::available));
		}
		else
		{
			//large tables 1-5
			large_table_queue->enQueue(Table("L" + to_string(i - 24), Condition::available));
			table_map->insert("L" + to_string(i - 24), Table("L" + to_string(i - 24), Condition::available));
		}
	}
}

// read comleted requests from file "completed_requests.txt"
void OperationManager::readCompletedRequest()
{
	ifstream in(COMPLETED_REQUEST_FILE);
	if (!in.fail())
	{
		std::string line, word;
		Request * temp;
		float cost;
		std::getline(in, line);
		std::getline(in, line);
		while (in >> word)
		{
			temp = new Request();
			temp->setRequestID(word);
			in >> word;
			temp->setTime(word);
			in >> word;
			temp->setCustomerName(word);
			in >> word;
			temp->setNumberOfCustomers(stoi(word));
			in >> word;
			temp->setTableID(word);
			in >> word;
			temp->setPayable(stof(word));
			in >> word;
			temp->setProfit(stof(word));
			in >> word;
			temp->setComment(word);
			cost = temp->getPayable() / (1 + TAX_RATE) - temp->getProfit();
			temp->setCost(cost);
			all_request_map->insert(temp->getRequestID(), temp);
			headNode.setCount(headNode.getCount() + 1);
			completed_requests->insert(temp);
		}
	}
	in.close();
}

// validation for the name
bool OperationManager::validCustomerName(const string& name) const
{
	if (name == "")
	{
		cout << "Customer name is required, please enter again: ";
		return false;
	}
	// not allowed for long name, because of the width limit in the output file
	if (name.length() > 10)
	{
		cout << "This name is too long, please enter again: ";
		return false;
	}
	// not allowed for space, because of the way we read the name from the file
	if (name.find(" ") != string::npos)
	{
		cout << "Space is not allowed in the name, please enter again: ";
		return false;
	}
	return true;
}

bool OperationManager::validGroupSize(const string& size) const
{
	if (size == "")
	{
		cout << "Group size is required. Please enter again: ";
		return false;
	}
	for (int i = 0; i < size.length(); ++i)
	{
		if (!isdigit(size[i]))
		{
			cout << "Invalid group size. Please enter again: ";
			return false;
		}
	}
	if (stoi(size) > LARGE_TABLE_CAPACITY)
	{
		cout << "Group size is larger than the maximum capacity *" << LARGE_TABLE_CAPACITY << "*.Please enter again: ";
		return false;
	}
	return true;
}

void OperationManager::printQueueInfo(const SIZE& size)
{
	switch (size)
	{
	case SIZE::small:
		cout << "          <Small Tables>" << endl;
		temp_request_queue = small_request_queue;
		temp_table_queue = small_table_queue;
		break;
	case SIZE::medium:
		cout << "          <Medium Tables>" << endl;
		temp_request_queue = medium_request_queue;
		temp_table_queue = medium_table_queue;
		break;
	case SIZE::large:
		cout << "          <Large Tables>" << endl;
		temp_request_queue = large_request_queue;
		temp_table_queue = large_table_queue;
		break;
	default:
		break;
	}
	cout << "-------------------------------" << endl;
	cout << "Waiting customer: " << temp_request_queue->getCount() << endl;
	cout << "Available tables: " << temp_table_queue->getCount() << endl;
}

void OperationManager::assignTables(const SIZE& size, const bool& isTestMode)
{
	switch (size)
	{
	case SIZE::small:
		temp_request_queue = small_request_queue;
		temp_table_queue = small_table_queue;
		break;
	case SIZE::medium:
		temp_request_queue = medium_request_queue;
		temp_table_queue = medium_table_queue;
		break;
	case SIZE::large:
		temp_request_queue = large_request_queue;
		temp_table_queue = large_table_queue;
		break;
	default:
		break;
	}
	// if there're requests in the queue, and available tables in the queue
	while (!temp_request_queue->isEmpty() && !temp_table_queue->isEmpty())
	{
		temp_request = temp_request_queue->deQueue();
		if (!isTestMode)
		{
			char choice;
			cout << "\nCalling: " << temp_request->getCustomerName() << endl;
			cout << "Is " << temp_request->getCustomerName() << " still here? (y/n): ";
			cin >> choice;
			cin.ignore();
			// if the customer has left
			if (choice != 'Y' && choice != 'y')
			{
				temp_request->setTableID("N/A");
				temp_request->setComment("Left before service");
				break;
			}
		}
		// if the customer did not leave, assign them tables:
		// dequeue the tables from the queue
		// delete the table from the hashtable, change its condition, reinsert back to the hashtable
		temp_table = temp_table_queue->deQueue();
		table_map->remove(temp_table.getTableID());
		temp_table.setCondition(Condition::unserved);
		table_map->insert(temp_table.getTableID(), temp_table);
		if (!isTestMode)
		{
			cout << "Assigning " << temp_request->getCustomerName() << " with table "
				<< temp_table.getTableID() << endl;
			temp_request->setComment("unserved");
			system("pause");
		}
		temp_request->setState(RequestState::unserved);
		temp_request->setTableID(temp_table.getTableID());
		// unserved_request_list is for helping print out the tables unserved
		unserved_request_list->add(temp_request, 1);
		// so we actually start to have a relationship of tableID->request from here
		serving_request_map->insert(temp_table.getTableID(), temp_request);
	}
}

void OperationManager::addNewCustomer(const string& name, const string& group)
{
	temp_request = new Request();
	temp_request->setCustomerName(name);
	temp_request->setNumberOfCustomers(stoi(group));
	temp_request->setTableID("N/A");
	temp_request->setComment("waiting");
	ostringstream sout;
	// the requestID starts with a character N, and its total length is 7
	sout << "N" << setw(6) << setfill('0') << requestIdCounter;
	temp_request->setRequestID(sout.str());
	requestIdCounter++;
	temp_request->setTime(time(0));
	if (stoi(group) <= SMALL_TABLE_CAPACITY)
	{
		small_request_queue->enQueue(temp_request);
	}
	else if (stoi(group) <= MEDIUM_TABLE_CAPACITY)
	{
		medium_request_queue->enQueue(temp_request);
	}
	else // <= LARGE_TABLE_CAPACITY
	{
		large_request_queue->enQueue(temp_request);
	}
	all_request_map->insert(temp_request->getRequestID(), temp_request);
	headNode.setCount(headNode.getCount() + 1);
}

bool OperationManager::hasUnservedTables() const
{
	return !unserved_request_list->isEmpty();
}

bool OperationManager::hasUncheckedTables() const
{
	return !served_request_list->isEmpty();
}

void OperationManager::printUnservedTables() const
{
	if (unserved_request_list->isEmpty())
	{
		cout << "            <Food Ordering>" << endl;
		cout << "------------------------------------------" << endl;
		cout << "There are no customers waiting to be served right now." << endl;
		system("pause");
	}
	else
	{
		cout << "            <Food Ordering>" << endl;
		cout << "------------------------------------------" << endl;
		cout << "These tables are waiting to be served: " << endl;
		for (int i = 1; i <= unserved_request_list->getCount(); i++)
		{
			cout << unserved_request_list->getData(i)->getTableID() << " ";
			if (i % 10 == 0)
			{
				cout << endl;
			}
		}
		cout << endl;
	}
}

void OperationManager::printUncheckedTables() const
{
	if (served_request_list->isEmpty())
	{
		system("cls");
		cout << "               <Check-out>" << endl;
		cout << "------------------------------------------" << endl;
		cout << "There are no customers waiting to check out right now." << endl;
		system("pause");
	}
	else
	{
		system("cls");
		cout << "               <Check-out>" << endl;
		cout << "------------------------------------------" << endl;
		cout << "These tables are waiting to be checked out: " << endl;
		for (int i = 1; i <= served_request_list->getCount(); i++)
		{
			cout << served_request_list->getData(i)->getTableID() << " ";
			if (i % 10 == 0)
			{
				cout << endl;
			}
		}
		cout << endl;
	}
}

void OperationManager::printTreeByRoot() const
{
	BinaryNode<Request*> * root = completed_requests->getRoot();
	if (!root)
		return;
	Queue<BinaryNode<Request*>*> queue;
	// enqueue the root
	queue.enQueue(root);

	BinaryNode<Request*> * current;
	// if the queue is not empty
	while (!queue.isEmpty())
	{
		// dequeue the first node
		current = queue.deQueue();
		// print it out
		std::cout << *(current->getItem());
		// enqueue its left & right child (if any)
		if (current->getLeftChildPtr())
			queue.enQueue(current->getLeftChildPtr());
		if (current->getRightChildPtr())
			queue.enQueue(current->getRightChildPtr());
	}
}

// print tree to the file in breath first traversal
void OperationManager::printTreeToFile(ofstream& out) const
{
	out << "                              <Completed Requests>" << endl;
	out << "---------------------------------------------------------------------------------------" << endl;
	BinaryNode<Request*> * root = completed_requests->getRoot();
	if (!root)
		return;
	Queue<BinaryNode<Request*>*> queue;
	queue.enQueue(root);

	BinaryNode<Request*> * current;

	while (!queue.isEmpty())
	{
		current = queue.deQueue();

		out << *(current->getItem());

		if (current->getLeftChildPtr())
			queue.enQueue(current->getLeftChildPtr());

		if (current->getRightChildPtr())
			queue.enQueue(current->getRightChildPtr());
	}
}

void OperationManager::printAllRequestMap() const
{
	HashPair<string, Request *> * arr = all_request_map->printByBuckets();
	Request * temp;
	int size = all_request_map->numberOfItems();
	cout << "There are totally " << size << " records" << endl;
	cout << "                                   <All Requests>" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < size; i++)
	{
		temp = arr[i].getValue();
		cout << *temp;
	}
	system("pause");
	delete[] arr;
}

void OperationManager::printAllSortedRequestMap() const
{
	HashPair<string, Request *> * arr = all_request_map->getWholeValueSortedByKey();
	Request * temp;
	int size = all_request_map->numberOfItems();
	cout << "                                   <All Requests>" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < size; i++)
	{
		temp = arr[i].getValue();
		cout << *temp;
	}
	system("pause");
	delete[] arr;
}

bool OperationManager::hasCompletedRequests() const
{
	return !completed_requests->isEmpty();
}

bool OperationManager::hasRequests() const
{
	return !all_request_map->isEmpty();
}

void OperationManager::printCompletedRequests()
{
	system("cls");
	if (completed_requests->isEmpty())
	{
		cout << "          <Completed Requests>" << endl;
		cout << "------------------------------------------" << endl;
		cout << "There are no completed requests so far." << endl;
	}
	else
	{
		cout << "There are totally " << completed_requests->getNumberOfNodes() << " records" << endl;
		cout << "                              <Completed Requests>" << endl;
		cout << "---------------------------------------------------------------------------------------" << endl;
		cout << left << setw(12) << "RequestID" << setw(16) << "Date"
			<< setw(12) << "Name" << setw(10) << "Group"
			<< setw(12) << "TableID" << setw(12) << fixed << setprecision(2)
			<< "Amount" << setw(10) << "Profit" << setw(20) << "Comment" << endl;
		this->printTreeByRoot();
	}
	system("pause");
}

void OperationManager::storeCompletedRequests() const
{
	if (!completed_requests->isEmpty())
	{
		//print completed request out file in breath first traversal
		ofstream outfile(COMPLETED_REQUEST_FILE);
		cout << "Storing the completed requests to the file..." << endl;
		printTreeToFile(outfile);
		outfile.close();

		//print AVL tree with its type out file and look like a tree
		//print out memory address in this case
		//inorder traversal
		outfile.open("inorder_tree.txt");
		outfile << "\t\t\t<Current AVL Tree - In-order Traversal>" << std::endl
			<< "-----------------------------------------------------------------------------------" << std::endl;
		completed_requests->printInOrder(outfile);
		outfile.close();

		//BF traversal
		outfile.open("breadthfirst_tree.txt");
		outfile << "\t\t\t<Current AVL Tree - Breadth First Traversal>" << std::endl
			<< "-----------------------------------------------------------------------------------" << std::endl;
		completed_requests->printBreathFirst(outfile);
		outfile.close();

	}
}

bool OperationManager::validUnservedTableId(const string& id)
{
	if (!table_map->contains(id))
	{
		cout << "Invalid table ID. Please enter again: ";
		return false;
	}
	temp_table = table_map->getItem(id);
	if (temp_table.getCondition() == Condition::available)
	{
		cout << "This table has no customer yet, please enter again: ";
		return false;
	}
	if (temp_table.getCondition() == Condition::unchecked)
	{
		cout << "This customer has ordered food already, please enter again: ";
		return false;
	}
	return true;
}

bool OperationManager::validUncheckedTableId(const string& id)
{
	while (!table_map->contains(id))
	{
		cout << "Invalid table ID. Please enter again: ";
		return false;
	}
	temp_table = table_map->getItem(id);
	if (temp_table.getCondition() == Condition::available)
	{
		cout << "This table has no customer yet, please enter again: ";
		return false;
	}
	if (temp_table.getCondition() == Condition::unserved)
	{
		cout << "This table has not ordered food yet, please enter again: ";
		return false;
	}
	return true;
}

void OperationManager::processFoodOrder(const string& table_id)
{
	temp_request = serving_request_map->getItem(table_id);
	// we already make sure the table_id is valid in the interaction manager
	// so its safe to update the unserved request list here
	unserved_request_list->deleteData(temp_request);

	string food_id;
	char choice;
	printFoodMenu();
	// prompt adding dishes
	do
	{
		cout << "What would " << temp_request->getCustomerName() << " like to have?: ";
		getline(cin, food_id);
		while (!validFoodId(food_id))
		{
			getline(cin, food_id);
		}
		temp_request->addDishes(stoi(food_id));
		cout << "continue? (y/n): ";
		cin >> choice;
		cin.ignore();
	} while (choice == 'Y' || choice == 'y');

	// show the staff what has the customer ordered
	system("cls");
	printFoodMenu();
	cout << temp_request->getCustomerName() << " has ordered: " << endl;
	temp_request->printDishes();

	// update the state of the request to unchecked
	temp_request->setState(RequestState::unchecked);
	temp_request->setComment("unchecked");

	// update served list
	served_request_list->add(temp_request, 1);

	// update the table hash table
	temp_table = table_map->getItem(table_id);
	table_map->remove(table_id);
	temp_table.setCondition(Condition::unchecked);
	table_map->insert(temp_table.getTableID(), temp_table);
	cout << endl;
}

void OperationManager::processCheckout(const string& id)
{
	printFoodMenu();
	temp_request = serving_request_map->getItem(id);
	cout << temp_request->getCustomerName() << " has ordered: " << endl;
	temp_request->printDishes();
	temp_request->checkout();
	cout << endl << temp_request->getCustomerName() << " should pay: $"
		<< fixed << setprecision(2) << temp_request->getPayable() << endl;
	system("pause");

	// update the state of the request
	temp_request->setState(RequestState::completed);
	temp_request->setComment("N/A");
	// add it to the tree
	completed_requests->insert(temp_request);
	// update the served linked list
	served_request_list->deleteData(temp_request);
	// remove from the map since the customer is leaving
	serving_request_map->remove(id);

	// for the table, reset it to available, and then reinsert again in the hash table
	temp_table = table_map->getItem(id);
	table_map->remove(temp_table.getTableID());
	temp_table.setCondition(Condition::available);
	table_map->insert(temp_table.getTableID(), temp_table);

	// check the size of the table, and then enqueue them back to the queue
	string size = temp_table.getTableSize();
	if (size == "small")
	{
		small_table_queue->enQueue(temp_table);
	}
	else if (size == "medium")
	{
		medium_table_queue->enQueue(temp_table);
	}
	else
	{
		large_table_queue->enQueue(temp_table);
	}
	cout << endl;
}

bool OperationManager::validFoodId(const string& id) const
{
	if (id == "")
	{
		cout << "A food index is required. Please enter again: ";
		return false;
	}
	for (int i = 0; i < id.length(); ++i)
	{
		if (!isdigit(id[i]))
		{
			cout << "Invalid food index. Please enter again: ";
			return false;
		}
	}
	if (stoi(id) == 0)
	{
		cout << "Maybe we should have started the food index from 0. Please enter again: ";
		return false;
	}
	if (stoi(id) > 11)
	{
		cout << "We are still on our way to develop dish " << id << ". Please enter again: ";
		return false;
	}
	return true;
}

void OperationManager::askContinueOrder(char& choice)
{
	if (!unserved_request_list->isEmpty())
	{
		cout << "You still have tables waiting to be served." << endl
			<< "Continue ordering food for the other customers? (y/n): ";
		cin >> choice;
		cin.ignore();
	}
	else
	{
		cout << "Congratulations, there are no tables waiting to be served now!" << endl;
		choice = 'n';
		system("pause");
	}
}

void OperationManager::askContinueCheck(char& choice)
{
	if (!served_request_list->isEmpty())
	{
		cout << "You still have tables waiting to be checked out." << endl
			<< "Continue helping other customers check out? (y/n): ";
		cin >> choice;
		cin.ignore();
	}
	else
	{
		cout << "Congratulations, there are no tables waiting to be checked out now!" << endl;
		choice = 'n';
		system("pause");
	}
}

bool OperationManager::validRequestID(const string& requestID)
{
	if (all_request_map->contains(requestID))
	{
		return true;
	}
	else
	{
		cout << "Can't find this request ID in our past records! Please enter again: ";
		return false;
	}
}

void OperationManager::promptSearchAndComment(const string& requestID)
{
	if (completed_requests->isEmpty())
	{
		cout << "Sorry, but there is no records of completed requests.";
		return;
	}
	temp_request = all_request_map->getItem(requestID);
	cout << endl;
	// show the record
	cout << "The request that you want to search is: " << endl;
	cout << left << setw(12) << "RequestID" << setw(16) << "Date"
		<< setw(12) << "Name" << setw(10) << "Group"
		<< setw(12) << "TableID" << setw(12) << fixed << setprecision(2)
		<< "Amount" << setw(10) << "Profit" << setw(20) << "Comment" << endl;
	cout << *temp_request;
	cout << endl;
	// prompt comment
	cout << "Do you want to add a comment? (y/n): ";
	char choice;
	cin >> choice;
	cin.ignore();
	if (choice == 'Y' || choice == 'y')
	{
		string comment;
		cout << "Please enter your comment: ";
		getline(cin, comment);
		temp_request->setComment(comment);
		cout << endl;
		// show the new record
		cout << "Thank you, the comment is successfully added. The updated request is: " << endl;
		cout << left << setw(12) << "RequestID" << setw(16) << "Date"
			<< setw(12) << "Name" << setw(10) << "Group"
			<< setw(12) << "TableID" << setw(12) << fixed << setprecision(2)
			<< "Amount" << setw(10) << "Profit" << setw(20) << "Comment" << endl;
		cout << *temp_request;
	}
}

void OperationManager::delRootCompletedRequest()
{
	temp_request = completed_requests->getRoot()->getItem();
	completed_requests->remove(temp_request);
	cout << "you've successfully deleted the following record: " << endl;
	cout << *temp_request;
}

void OperationManager::printFoodMenu() const
{
	cout << "----------------------------------------------------" << endl;
	cout << "                     <Food Menu>                     " << endl;
	cout << "			PLATES                       " << endl;
	cout << " 1  ---------- Polish Sausage Scrambles	  $" << PRICE[0] << endl;
	cout << " 2  ---------- Italian Omelette           $" << PRICE[1] << endl;
	cout << " 3  ---------- Cajun Chicken Benedict     $" << PRICE[2] << endl;
	cout << " 4  ---------- Hamburger Steak            $" << PRICE[3] << endl;
	cout << " 5  ---------- Avocado Sandwich           $" << PRICE[4] << endl;
	cout << "			SWEETS " << endl;
	cout << " 6  ---------- Matcha Tiramisu            $" << PRICE[5] << endl;
	cout << " 7  ---------- Strawberry Chiffon         $" << PRICE[6] << endl;
	cout << " 8  ---------- Mango Millicrepe           $" << PRICE[7] << endl;
	cout << "			DRINKS " << endl;
	cout << " 9  ---------- Juice                      $" << PRICE[8] << endl;
	cout << " 10 ---------- Tea                        $" << PRICE[9] << endl;
	cout << " 11 ---------- Coffee                     $" << PRICE[10] << endl;
	cout << "\n *****Note: " << TAX_RATE * 100 << "% tax rate will be applied*****" << endl;
	cout << "----------------------------------------------------" << endl;
}

void OperationManager::requestHashTableEfficiency() const
{
	cout << "Efficiency of HashTable(Request ID -> Request object):" << endl
		<< "Total items of data: " << all_request_map->numberOfItems() << endl
		<< "Number of buckets: " << all_request_map->numberOfBuckets() << endl
		<< "Number of collisions: " << all_request_map->numberOfCollisions() << endl
		<< "Load factor: " << all_request_map->loadFactor() * 100 << "%" << endl
		<< "Average non-empty bucket size: " << all_request_map->averageNonEmptyBucketSize() << endl
		<< "Longest bucket size: " << all_request_map->longestBucket() << endl;
	system("pause");
}

void OperationManager::tableHashTableEfficiency() const
{
	cout << "Efficiency of HashTable(Table ID -> Table object):" << endl
		<< "Total items of data: " << table_map->numberOfItems() << endl
		<< "Number of buckets: " << table_map->numberOfBuckets() << endl
		<< "Number of collisions: " << table_map->numberOfCollisions() << endl
		<< "Load factor: " << table_map->loadFactor() * 100 << "%" << endl
		<< "Average non-empty bucket size: " << table_map->averageNonEmptyBucketSize() << endl
		<< "Longest bucket size: " << table_map->longestBucket() << endl;

	system("pause");
}

void OperationManager::tableRequestHashTableEfficiency() const
{
	cout << "Efficiency of HashTable(Table ID -> Request object):" << endl
		<< "Total items of data: " << serving_request_map->numberOfItems() << endl
		<< "Number of buckets: " << serving_request_map->numberOfBuckets() << endl
		<< "Number of collisions: " << serving_request_map->numberOfCollisions() << endl
		<< "Load factor: " << serving_request_map->loadFactor() * 100 << "%" << endl
		<< "Average non-empty bucket size: " << serving_request_map->averageNonEmptyBucketSize() << endl
		<< "Longest bucket size: " << serving_request_map->longestBucket() << endl;
	system("pause");
}