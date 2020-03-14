#pragma once
#include "Validator.h"
#include "Repository.h"
#include <functional>
#include "Cos.h"
#include "Undo.h"

typedef bool(*maiMicF)(const Produs&, const Produs&);

class Service : public Observable
{
private:
	Repository& repo;
	Validator& valid;
	//Cos cos;
	//FileCos cos{"cos.txt"};
	Cos& cos;
	vector<ActiuneUndo*> UndoActions;

	/*
	 Functie de sortare generala
	 maiMicF - functie care compara 2 Produs, verifica daca are loc relatia mai mare
			  - poate fi orice functe (in afara clasei) care respecta signatura (returneaza bool are 2 parametrii Produs)
			  - poate fi functie lambda (care nu capteaza nimic in capture list)
	 returneaza o lista sortata dupa criteriu dat ca paramatru
	*/
	vector<Produs> generalSort(maiMicF cmpf) const;

	/*
	Functie generala de filtrare
	fct - poate fi o functie
	fct - poate fi lambda, am folosit function<> pentru a permite si functii lambda care au ceva in capture list
	returneaza doar produsele care trec de filtru (fct(produs)==true)
	*/

	vector<Produs> generalFilter(function<bool(const Produs&)> f) const;


public:
	Service(Repository& r, Validator& v, Cos& c) noexcept :repo{ r }, valid{ v }, cos{ c }{};

	Service(const Service& ot) = delete;

	/*
	Adauga un produs
	Arunca exceptie daca produsul cautat nu este valid sau a fost deja adaugat
	*/
	void adaugaProdus(const string& nume, const string& tip, int pret, const string& producator);

	/*
	Sterge un produs
	Atunca exceptie daca produsul nu exista
	*/
	void stergeProdus(const string& nume, const string& tip);

	/*
	Cauta un produs si il returneaza
	Arunca exceptie daca nu exista produsul
	*/
	Produs cautaProdus(const string& nume, const string& tip);

	/*
	Modifica un produs
	Arunca exceptie daca nu exista produsul sau daca nu este valid
	*/
	void modificaProdus(const string& nume, const string& tip, int pret, const string& producator);

	/*
	Rtuenaza toate produsele
	*/
	const vector<Produs>& getAll() {
		return repo.getAll();
	}

	/*
	Filtreaza lista de produse dupa nume
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> filtruNume(const string& nume) const;

	/*
	Filtreaza lista de produse dupa producator
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> filtruProducator(const string& producator) const;

	/*
	Filtreaza lista de produse dupa pret, produsele cu pret mai mic sau egal cu cel cerut
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> filtruPret(int pret) const;

	/*
	Sorteaza lista dupa nume
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> sortareNume() const;

	/*
	Sorteaza lista dupa pret
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> sortarePret() const;

	/*
	Sorteaza lista dupa nume si apoi dupa tip
	Returneaza un vector cu obiecte de tip produs
	*/
	vector<Produs> sortareNumeTip() const;

	//adauga in cos produs dupa nume
	void adaugaCos(const string& nume);

	//goleste cos de produs
	void golesteCos();

	//generare cos- utilizatorul introduce numarul de produse, cosul se umple aleator cu numarul de produse
	void genereazaCos(size_t nr);

	//returneaza pretul total al produselor din cos
	int pretTotalCos() const;

	//returneaza toate produsele din cos
	const vector<Produs>& getAllCos() const;

	//returneaza numarul produselor din cos
	size_t cosSize() const;

	//exporta cosul intr-un fisier CVS
	void exportCosCVS(string file_name) const;

	//exporta cosul intr-un fisier Html
	void exportCosHtml(string file_name) const;

	//reface ultima operatie care modifica lista de produse -adaugare/stergere/modificare
	void undo();

	//returneaza cosul de cumparturi 
	Cos& getCos() {
		return cos;
	}

	//elibereaza memoria -destructorul
	~Service();
};

void testService();

