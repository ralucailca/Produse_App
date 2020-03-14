#include "Repository.h"
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <iostream>
/*
	Adauga in lista un produs
	Daca acesta exista deja se arunca eroare
*/
void Repository::adauga(const Produs& p) {
	if (exista(p)) {
		throw RepoException("Exista deja un produs numit: " + p.get_nume() + " de tipul: " + p.get_tip());
	}
	elems.push_back(p);
}


/*
	Cauta in lista un produs, daca nu exista este aruncata eroare
*/
Produs Repository::cauta(const string& nume, const string& tip) { //const inainte de modificare cu find_if
	/*
	for (const auto& p : elems) {
		if (p.get_nume() == nume && p.get_tip() == tip)
			return p;
	}*/
	vector<Produs>::iterator it = find_if(elems.begin(), elems.end(), [&](Produs p) {return nume == p.get_nume() && p.get_tip() == tip; });
	if (it != elems.end()) return *it;
	throw RepoException("Nu exista un produs cu numele si tipul specificat!");
}

/*
 verifica daca exista in lista un produs
 returneaza true daca produsul se afla in lista si false in caz contrar
*/
bool Repository::exista(const Produs& p) { //const inainte de modificare cu find_if
	try {
		cauta(p.get_nume(), p.get_tip());
		return true;
	}
	catch (RepoException&) {
		return false;
	}
}

//returneaza toate elementele din repository
const vector<Produs>& Repository::getAll() const noexcept {
	return elems;
}

/*
	Sterge din lista un produs
	Daca nu exista produs care trebuie sters se arunca eroare
*/
Produs Repository::sterge(const Produs& p) {
	if (!exista(p)) {
		throw RepoException("Nu exista produsul in lista!");
	}
	/*
	vector<Produs>::iterator it = elems.begin();
	while (it != elems.end()) {
		if ((*it).get_nume() == p.get_nume() && (*it).get_tip() == p.get_tip()){
			elems.erase(it);
			break;
		}
		it++;
	}
	*/
	vector<Produs>::iterator it = find_if(elems.begin(), elems.end(), [&](Produs p1) {return p1.get_nume() == p.get_nume() && p.get_tip() == p1.get_tip(); });
	Produs p1 = *it;
	elems.erase(it);
	return p1;
}

/*
	Cauta in lista un produs, daca nu exista este aruncata eroare
*/
Produs Repository::modifica(const Produs& p) {
	if (!exista(p)) {
		throw RepoException("Nu exista produsul in lista!");
	}
	vector<Produs>::iterator it = find_if(elems.begin(), elems.end(), [&](Produs p1) {return p1.get_nume() == p.get_nume() && p.get_tip() == p1.get_tip(); });
	Produs p1 = *it;
	replace_if(elems.begin(), elems.end(), [&](Produs p1) {return p1.get_nume() == p.get_nume() && p.get_tip() == p1.get_tip(); }, p);
	return p1;
	/*
	for (auto& prod : elems) {
		if (prod.get_nume() == p.get_nume() && prod.get_tip() == p.get_tip()) {
			prod.set_pret(p.get_pret());
			prod.set_producator(p.get_producator());
			break;
		}

	}*/
}

//returneaza numarul de produse din lista
size_t Repository::len() const {
	return elems.size();
}

ostream& operator<<(ostream& out, const RepoException& ex) {
	out << ex.msg;
	return out;
}



void testAdauga() {
	Repository repo;
	repo.adauga(Produs("paine", "panificatie", 4, "pan.srl"));
	repo.adauga(Produs("lapte", "lactate", 6, "aaa"));
	assert(repo.len() == 2);

	try {
		repo.adauga(Produs("paine", "panificatie", 4, "pan.srl"));
		//assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
	assert(repo.len() == 2);
	vector<Produs> a{ repo.getAll() };
	assert(a.size() == 2);
}

void testCauta() {
	Repository repo;
	repo.adauga(Produs("paine", "panificatie", 4, "pan.srl"));
	repo.adauga(Produs("lapte", "lactate", 6, "aaa"));
	Produs p = repo.cauta("lapte", "lactate");
	assert(p.get_nume() == "lapte");
	assert(p.get_producator() == "aaa");
	try {
		Produs p1 = repo.cauta("bbb", "aaa"); //assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
}

void testSterge() {
	Repository repo;
	repo.adauga(Produs("paine", "panificatie", 4, "pan.srl"));
	repo.adauga(Produs("lapte", "lactate", 6, "aaa"));
	assert(repo.len() == 2);
	repo.sterge(Produs("paine", "panificatie", 4, "pan.srl"));
	assert(repo.len() == 1);
	try {
		repo.sterge(Produs("bbb", "aaa", 1, "a"));
		//assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
	repo.sterge(Produs("lapte", "lactate", 6, "aaa"));
	assert(repo.len() == 0);
	try {
		repo.sterge(Produs("lapte", "lactate", 6, "aaa"));
		//assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
}

void testModifica() {
	Repository repo;
	repo.adauga(Produs("paine", "panificatie", 4, "pan.srl"));
	repo.adauga(Produs("lapte", "lactate", 6, "aaa"));
	repo.modifica(Produs("lapte", "lactate", 5, "bbb"));
	Produs p = repo.cauta("lapte", "lactate");
	assert(p.get_nume() == "lapte");
	assert(p.get_producator() == "bbb");
	assert(p.get_pret() == 5);
	try {
		repo.modifica(Produs("bbb", "aaa", 1, "a"));
		//assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
}

void testRepo() {
	testAdauga();
	testCauta();
	testSterge();
	testModifica();
}

void FileRepo::loadFromFile() {
	ifstream in(filename);
	if (in.fail())
		throw RepoException("Eroare la deschiderea fisierului.");
	while (!in.eof()) {
		string nume;
		in >> nume;
		string tip;
		in >> tip;
		int pret;
		in >> pret;
		if (in.eof()) break;
		string producator;
		in >> producator;
		Produs p{ nume, tip, pret, producator };
		Repository::adauga(p);
	}
	in.close();
}

void FileRepo::writeToFile() {
	ofstream out(filename);
	if (!out.is_open())
		throw RepoException("Eroare la deschiderea fisierului");
	for (auto& el : getAll()) {
		out << el.get_nume();
		out << endl;
		out << el.get_tip();
		out << endl;
		out << el.get_pret();
		out << endl;
		out << el.get_producator();
		out << endl;
	}
	out.close();
}

void testFileRepo() {
	//cream un fisier gol
	ofstream out("test.txt", ios::trunc);
	out.close();
	FileRepo repof{ "test.txt" };
	repof.adauga(Produs("mere", "tip1", 5, "prod.srl"));
	repof.adauga(Produs("pere", "tip2", 7, "prod2"));
	FileRepo repof2{ "test.txt" };
	Produs p = repof2.cauta("pere", "tip2");
	assert(p.get_pret() == 7);
	repof2.sterge(Produs("pere", "tip2", 7, "prod2"));
	repof2.modifica(Produs("mere", "tip1", 12, "mereprod.srl"));
	FileRepo repof3{ "test.txt" };
	assert(repof3.getAll().size() == 1);
	p = repof3.cauta("mere", "tip1");
	assert(p.get_producator() == "mereprod.srl");
	assert(p.get_pret() == 12);
	try {
		FileRepo repof4{ "testProd.txt" };
	}
	catch (RepoException&) {
		assert(true);
	}
}
