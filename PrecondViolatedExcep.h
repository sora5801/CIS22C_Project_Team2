/*
	Credit to : Matthew Fu
*/

#pragma once

#include <stdexcept>
#include <string>
using namespace std;

class PrecondViolatedExcep : public logic_error
{
public:
	PrecondViolatedExcep(const string& message = "") :
		logic_error("Precondition Violated Except: " + message) {}
};