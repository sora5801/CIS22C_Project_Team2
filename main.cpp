/*
	CIS22C Team Project
	Team 2:	Restaurant Management System
	Matthew, James, Phuoc, Leia
*/

#include "InteractionManager.h"

using namespace std;

int main()
{
	InteractionManager* manager = new InteractionManager();
	manager->handle();
	system("pause");
	delete manager;
	manager = nullptr;
	return 0;
}
