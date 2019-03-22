/*
	Credit to : Matthew Fu
*/

#pragma once

#include <stdexcept>
#include <string>
using namespace std;

class NotFoundException : public logic_error
{
public:
	NotFoundException(const string& message = "") noexcept
		: logic_error("Precondition Violated Exception: " + message) {}
}; // end NotFoundException