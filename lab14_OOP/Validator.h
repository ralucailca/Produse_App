#pragma once
#include <iostream>
#include "Produs.h"
#include <vector>
#include <string>
using namespace std;

class Validator
{
public:
	Validator() = default;
	void validare(const Produs& p);
};

class ValidException {
	vector<string> msgs;
public:
	ValidException(vector<string> m) :msgs{ m } {};
	friend ostream& operator<<(ostream& out, const ValidException& ex);
	vector<string>& getMsgs() { return msgs; }
};

ostream& operator<<(ostream& out, const ValidException& ex);

void testValidator();

