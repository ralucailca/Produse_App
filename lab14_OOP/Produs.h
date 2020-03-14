#pragma once
#include <iostream>
#include <string>
extern int numar;


using namespace std;

class Produs
{
private:
	std::string nume;
	std::string tip;
	int pret;
	std::string producator;
public:
	Produs(const string n, const string t, int p, const string pr) :nume{ n }, tip{ t }, pret{ p }, producator{ pr }{numar++; }
	Produs() = default;

	string get_nume() const {
		return nume;
	}

	string get_tip() const {
		return tip;
	}

	int get_pret() const {
		return pret;
	}

	string get_producator() const {
		return producator;
	}

	void set_pret(int value) {
		pret = value;
	}

	void set_producator(const string value) {
		producator = value;
	}
};


