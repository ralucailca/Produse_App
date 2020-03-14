#pragma once
#include <vector>
#include "Produs.h"
#include "Repository.h"
#include "Observer.h"

class Cos : public Observable
{
private:
	vector<Produs> cumparaturi;

public:
	Cos();
	Cos(const Cos& ot) = delete;
	//Cos& operator=(const Cos& ot);
	//adauga un produs in cos
	void adauga(const Produs& p);
	//elimina toate produsele din cos
	void goleste();

	//returneaza numarul de elemente din cos
	size_t size() const;

	//returneaza toate produsele din cos
	const vector<Produs>& getAll() const;

	//sterge un produs din cos
	void sterge(const string& nume, const string& tip);

	//modifica un produs din cos
	void modifica(const string& nume, const string& tip, int pret, const string& producator);

	~Cos();
};

void testCos();



