#include "Cos.h"
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <fstream>


Cos::Cos() {}


//adauga un produs in cos
void Cos::adauga(const Produs& p) {
	cumparaturi.push_back(p);
	notify();
}

//elimina toate produsele din cos
void Cos::goleste() {
	cumparaturi.clear();
	notify();
}

//returneaza numarul de elemente din cos
size_t Cos::size() const {
	return cumparaturi.size();
}

//returneaza toate produsele din cos
const vector<Produs>& Cos::getAll() const {
	return cumparaturi;
}

//sterge un produs din cos
void Cos::sterge(const string& nume, const string& tip) {
	vector<Produs>::iterator it = remove_if(cumparaturi.begin(), cumparaturi.end(), [&](Produs p) {return p.get_nume() == nume && p.get_tip() == tip; });
	cumparaturi.erase(it, cumparaturi.end());
	notify();
}

//modifica un produs din cos
void Cos::modifica(const string& nume, const string& tip, int pret, const string& producator) {

	for (auto& el : cumparaturi)
		if (el.get_nume() == nume && el.get_tip() == tip) {
			el.set_pret(pret);
			el.set_producator(producator);

		}
	notify();
	//replace_if(cumparaturi.begin(), cumparaturi.end(), [&](Produs p) {return p.get_nume() ==nume && p.get_tip() == tip(); },pr);
}

Cos::~Cos() {
}

void testCos() {
	Cos cos;
	Produs p1{ "mere","fructe",14,"prod.srl" };
	cos.adauga(p1);
	Produs p2{ "pere","fructe",10,"producator.srl" };
	cos.adauga(p2);
	Produs p3("rosii", "legume", 20, "prod2.srl");
	cos.adauga(p3);
	assert(cos.size() == 3);
	cos.goleste();
	assert(cos.size() == 0);
}



