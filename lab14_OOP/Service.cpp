#include "Service.h"
#include <assert.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <fstream>
#include <Export.h>

#include <random>
#include <chrono>

int numar = 0;

/*
	Adauga un produs
	Arunca exceptie daca produsul cautat nu este valid sau a fost deja adaugat
*/
void Service::adaugaProdus(const string& nume, const string& tip, int pret, const string& producator) {
	Produs p{ nume, tip, pret, producator };
	valid.validare(p);
	repo.adauga(p);
	UndoActions.push_back(new AdaugaUndo{ repo,p });
}

/*
	Sterge un produs
	Atunca exceptie daca produsul nu exista
*/
void Service::stergeProdus(const string& nume, const string& tip) {
	Produs p{ nume,tip,1,"aa" };
	cos.sterge(nume, tip);
	Produs sters = repo.sterge(p);
	UndoActions.push_back(new StergeUndo{ repo,sters });
}

/*
	Cauta un produs si il returneaza
	Arunca exceptie daca nu exista produsul
*/
Produs Service::cautaProdus(const string& nume, const string& tip) {
	return repo.cauta(nume, tip);
}

/*
	Modifica un produs
	Arunca exceptie daca nu exista produsul sau daca nu este valid
*/
void Service::modificaProdus(const string& nume, const string& tip, int pret, const string& producator) {
	Produs p{ nume, tip, pret, producator };
	valid.validare(p);
	Produs modif = repo.modifica(p);
	UndoActions.push_back(new ModificaUndo{ repo,modif });
	//cos.modifica(new Produs{ p }); 
	cos.modifica(nume, tip, pret, producator);
}

/*
Functie de sortare generala
 maiMicF- functie care compara 2 Produs, verifica daca are loc relatia mai mare
		- poate fi orice functe (in afara clasei) care respecta signatura (returneaza bool are 2 parametrii Produs)
		- poate fi functie lambda (care nu capteaza nimic in capture list)
returneaza o lista sortata dupa criteriu dat ca paramatru
*/
vector<Produs> Service::generalSort(maiMicF cmpf) const {
	vector<Produs> rez{ repo.getAll() };
	sort(rez.begin(), rez.end(), cmpf);
	return rez;
}

/*
Sorteaza lista dupa nume
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::sortareNume() const {
	return generalSort([](const Produs &p1, const Produs &p2)
	{return p1.get_nume() < p2.get_nume(); });
}

/*
Sorteaza lista dupa pret
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::sortarePret() const {
	return generalSort([](const Produs &p1, const Produs &p2)
	{return p1.get_pret() < p2.get_pret(); });
}

/*
Sorteaza lista dupa nume si apoi dupa tip
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::sortareNumeTip() const {
	return generalSort([](const Produs &p1, const Produs &p2)
	{if (p1.get_nume() == p2.get_nume())
		return p1.get_tip() < p2.get_tip();
	return p1.get_nume() < p2.get_nume(); });
}

/*
Functie generala de filtrare
fct - poate fi o functie
fct - poate fi lambda, am folosit function<> pentru a permite si functii lambda care au ceva in capture list
returneaza doar produsele care trec de filtru (fct(produs)==true)
*/

vector<Produs> Service::generalFilter(function<bool(const Produs&)> f) const {

	vector<Produs> l = repo.getAll();
	vector<Produs> rez(l.size());
	/*
	for (const auto& p : repo.getAll()) {
		if (f(p)) {
			rez.push_back(p);
		}
	}
	*/
	vector<Produs>::iterator copyEnd = copy_if(l.begin(), l.end(), rez.begin(), f);
	rez.erase(copyEnd, rez.end());
	return rez;
}

/*
Filtreaza lista de produse dupa nume
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::filtruNume(const string& nume) const {
	return generalFilter([&](const Produs &p) { return strstr(p.get_nume().c_str(), nume.c_str()) != NULL; });
}

/*
Filtreaza lista de produse dupa producator
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::filtruProducator(const string& producator) const {
	return generalFilter([producator](const Produs &p) { return strstr(p.get_producator().c_str(), producator.c_str()) != NULL; });
}

/*
Filtreaza lista de produse dupa pret, produsele cu pret mai mic sau egal cu cel cerut
Returneaza un vector cu obiecte de tip produs
*/
vector<Produs> Service::filtruPret(int pret) const {
	return generalFilter([=](const Produs &p) { return p.get_pret() <= pret; });
}

//adauga in cos produs dupa nume
void Service::adaugaCos(const string& nume) {
	for (auto el : repo.getAll())
		if (el.get_nume() == nume)
			cos.adauga(el);
	notify();
}

//goleste cos de produs
void Service::golesteCos() {
	cos.goleste();
	notify();
}

//generare cos- utilizatorul introduce numarul de produse, cosul se umple aleator cu numarul de produse
void Service::genereazaCos(size_t nr) {
	if (repo.len() != 0) {
		vector<Produs> l{ repo.getAll() };
		while (cos.size() != nr) {
			//auto seed = std::chrono::system_clock::now().time_since_epoch().count();
			shuffle(l.begin(), l.end(), std::default_random_engine(std::random_device{}()));
			for (auto el : l) {
				if (cos.size() != nr)
					cos.adauga(el);
			}
		}
	}
	notify();
}


//returneaza pretul total al produselor din cos
int Service::pretTotalCos() const {
	int sum = 0;
	for (auto& el : cos.getAll())
		sum += el.get_pret();
	return sum;
}

//returneaza toate produsele din cos
const vector<Produs>& Service::getAllCos() const {
	return cos.getAll();
}

//returneaza numarul produselor din cos
size_t Service::cosSize() const {
	return cos.size();
}

//reface ultima operatie care modifica lista de produse -adaugare/stergere/modificare
void Service::undo() {
	if (UndoActions.empty())
		throw RepoException("Nu se mai poate face undo!");
	ActiuneUndo* act = UndoActions.back();
	act->doUndo();
	UndoActions.pop_back();
	delete act;
}

//exporta cosul intr-un fisier CVS
void Service::exportCosCVS(string file_name) const {
	exportToCVS(file_name, repo.getAll());
}

//exporta cosul intr-un fisier Html
void Service::exportCosHtml(string file_name) const {
	exportToHTML(file_name, repo.getAll());
}

//elibereaza memoria -destructorul
Service::~Service() {
	for (auto el : UndoActions)
		delete el;
}

void testOperatii() {
	Repository repo;
	Validator valid;
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "producator");
	serv.adaugaProdus("pantaloni", "haina", 1000, "producator2");
	try {
		serv.adaugaProdus("pantaloni", "haina", 1000, "producator2");
	}
	catch (RepoException&) {
		assert(true);
	}
	assert(serv.getAll().size() == 2);
	Produs p = serv.cautaProdus("tricou", "haina");
	assert(p.get_pret() == 700);
	serv.stergeProdus("pantaloni", "haina");
	try {
		serv.cautaProdus("pantaloni", "haina");
	}
	catch (RepoException&) {
		assert(true);
	}
	assert(serv.getAll().size() == 1);
	serv.modificaProdus("tricou", "haina", 890, "producator3");
	Produs p1 = serv.cautaProdus("tricou", "haina");
	assert(p1.get_pret() == 890);
	try {
		serv.modificaProdus("", "haina", -890, "producator3");
	}
	catch (ValidException&) {
		assert(true);
	}
}

void testSortari() {
	Repository repo;
	Validator valid;
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "adidas");
	serv.adaugaProdus("pantaloni", "haina", 1000, "producator");
	serv.adaugaProdus("geanta", "accesorii", 800, "Guess");
	serv.adaugaProdus("ceas", "accesorii", 1200, "Fossil");
	serv.adaugaProdus("adidasi", "incaltaminte", 1000, "nike");
	serv.adaugaProdus("geanta", "haina", 800, "Guess");

	vector<Produs> v1 = serv.sortareNume();
	assert(v1[0].get_nume() == "adidasi");
	assert(v1[5].get_nume() == "tricou");

	vector<Produs> v2 = serv.sortarePret();
	assert(v2[5].get_nume() == "ceas");
	assert(v2[0].get_nume() == "tricou");

	vector<Produs> v3 = serv.sortareNumeTip();
	assert(v3[2].get_nume() == "geanta");
	assert(v3[3].get_nume() == "geanta");
	assert(v3[2].get_tip() == "accesorii");
	assert(v3[3].get_tip() == "haina");

}

void testFiltru() {
	Repository repo;
	Validator valid;
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "adidas");
	serv.adaugaProdus("pantaloni", "haina", 1000, "nike");
	serv.adaugaProdus("rochie", "haina", 4800, "LV");
	serv.adaugaProdus("geanta", "accesorii", 800, "Guess");
	serv.adaugaProdus("ceas", "accesorii", 1200, "Fossil");
	serv.adaugaProdus("adidasi", "incaltaminte", 1000, "nike");
	serv.adaugaProdus("adidasi", "tip2", 1999, "nike");

	vector<Produs> v1 = serv.filtruNume("adidasi");
	assert(v1[0].get_tip() == "incaltaminte");
	assert(v1[1].get_tip() == "tip2");

	vector<Produs> v2 = serv.filtruPret(1000);
	assert(v2[1].get_nume() == "pantaloni");
	assert(v2[0].get_nume() == "tricou");
	assert(v2[2].get_nume() == "geanta");

	vector<Produs> v3 = serv.filtruProducator("nike");
	assert(v3[0].get_nume() == "pantaloni");
	assert(v3[1].get_nume() == "adidasi");
	assert(v3[2].get_nume() == "adidasi");


}

void testeCos() {
	Repository repo;
	Validator valid;
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "adidas");
	serv.adaugaProdus("pantaloni", "haina", 1000, "nike");
	serv.adaugaProdus("rochie", "haina", 4800, "LV");
	serv.adaugaProdus("geanta", "accesorii", 800, "Guess");
	serv.adaugaProdus("ceas", "accesorii", 1200, "Fossil");
	serv.adaugaProdus("adidasi", "incaltaminte", 1000, "nike");
	serv.adaugaProdus("adidasi", "tip2", 1999, "nike");
	//test adaugare
	serv.adaugaCos("adidasi");
	assert(serv.getAll().size() == 7);
	assert(serv.cosSize() == 2);
	assert(serv.getAllCos().size() == 2);
	serv.modificaProdus("adidasi", "incaltaminte", 1400, "adidas");
	assert(serv.cosSize() == 2);
	serv.stergeProdus("adidasi", "incaltaminte");
	assert(serv.cosSize() == 1);
	//test golire
	serv.golesteCos();
	assert(serv.cosSize() == 0);
	assert(serv.getAll().size() == 6);
	assert(serv.cautaProdus("adidasi", "tip2").get_producator() == "nike");
	assert(serv.cautaProdus("adidasi", "tip2").get_pret() == 1999);
	//test generare
	serv.genereazaCos(9);
	assert(serv.cosSize() == 9);
	serv.genereazaCos(14);
	assert(serv.cosSize() == 14);

}

void testExport() {
	Repository repo;
	Validator valid;
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "adidas");
	serv.adaugaProdus("pantaloni", "haina", 1000, "nike");
	serv.adaugaProdus("rochie", "haina", 4800, "LV");
	serv.exportCosCVS("testExport.CVS");
	ifstream f("testExport.CVS");
	assert(f.is_open());
	int countLines = 0;
	while (!f.eof()) {
		string line;
		f >> line;
		countLines++;
	}
	assert(countLines == 4); //cele 3 produse + o linie libera
	f.close();
	serv.exportCosHtml("testExport.html");
	ifstream in("testExport.html");
	in.open("testExport.html");
	assert(in.is_open());
	in.close();
	//daca se da un nume de fisier invalid se arunca exceptie
	try {
		serv.exportCosCVS("test/Export.cvs");
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
	try {
		serv.exportCosHtml("a/b/c/Export.html");
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
}

void testUndo() {
	Repository repo;
	Validator valid;
	ofstream out("fisTestCos.txt", ios::trunc);
	out.close();
	Cos cos;
	Service serv{ repo, valid, cos };
	serv.adaugaProdus("tricou", "haina", 700, "adidas");
	serv.adaugaProdus("pantaloni", "haina", 1000, "nike");
	serv.adaugaProdus("rochie", "haina", 4800, "LV");
	assert(serv.getAll().size() == 3);
	serv.undo();
	assert(serv.getAll().size() == 2);
	serv.stergeProdus("pantaloni", "haina");
	assert(serv.getAll().size() == 1);
	serv.undo();
	assert(serv.getAll().size() == 2);
	serv.modificaProdus("tricou", "haina", 500, "adidas2");
	Produs p = serv.cautaProdus("tricou", "haina");
	assert(p.get_pret() == 500);
	assert(p.get_producator() == "adidas2");
	serv.undo();
	p = serv.cautaProdus("tricou", "haina");
	assert(p.get_pret() == 700);
	assert(p.get_producator() == "adidas");
	serv.undo();
	serv.undo();
	try {
		serv.undo();
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}
}

void testService() {
	testOperatii();
	testSortari();
	testFiltru();
	testeCos();
	testUndo();
	testExport();
}