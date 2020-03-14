#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Produs.h"

using namespace std;

class Repository
{
private:
	vector<Produs> elems;

	//verifica daca exista in lista un produs
	bool exista(const Produs& p);

public:
	Repository() {};

	//Repository nu poate fi copiat
	Repository(const Repository& ot) = delete;

	/*
	Adauga in lista un produs
	Daca acesta exista deja se arunca eroare
	*/
	virtual void adauga(const Produs& p);

	/*
	Sterge din lista un produs
	Daca nu exista produs care trebuie sters se arunca eroare
	*/
	virtual Produs sterge(const Produs& p);

	/*
	Cauta in lista un produs, daca nu exista este aruncata eroare
	*/
	Produs cauta(const string& nume, const string& tip);

	/*
	Cauta in lista un produs, daca nu exista este aruncata eroare
	*/
	virtual Produs modifica(const Produs& p);

	//returneaza toate elementele din repository
	const vector<Produs>& getAll() const noexcept;

	//returneaza numarul de produse din lista
	size_t len() const;

	virtual ~Repository() {}
};


/*
Folosit pentru a semnala situatii exceptionale care pot aparea in repo
*/
class RepoException {
	string msg;
public:
	RepoException(const string msg) :msg{ msg } {}
	friend ostream& operator<<(ostream& out, const RepoException& ex);
	string getMsg()const { return msg; }
};

ostream& operator<<(ostream& out, const RepoException& ex);

void testRepo();

class FileRepo : public Repository
{
private:
	string filename;
	void loadFromFile();
	void writeToFile();
public:
	FileRepo(const string& fname) : Repository(), filename{ fname }{
		loadFromFile();
	}
	void adauga(const Produs& p) override {
		Repository::adauga(p);
		writeToFile();
	}
	Produs sterge(const Produs& p) override {
		Produs pr = Repository::sterge(p);
		writeToFile();
		return pr;
	}
	Produs  modifica(const Produs& p) override {
		Produs pr = Repository::modifica(p);
		writeToFile();
		return pr;
	}
};

void testFileRepo();