#pragma once
#include "Repository.h"
class ActiuneUndo
{
public:
	ActiuneUndo() {};
	virtual void doUndo() = 0;
	virtual ~ActiuneUndo() = default;
};

class AdaugaUndo : public ActiuneUndo
{
private:
	Repository& repo;
	Produs prAdaugat;
public:
	AdaugaUndo(Repository& r, Produs& p) :repo{ r }, prAdaugat{ p }{};
	void doUndo() override {
		repo.sterge(prAdaugat);
	}
	~AdaugaUndo() {};
};

class StergeUndo : public ActiuneUndo
{
private:
	Repository& repo;
	Produs prSters;
public:
	StergeUndo(Repository& r, Produs& p) :repo{ r }, prSters{ p }{};
	void doUndo() override {
		repo.adauga(prSters);
	}
	~StergeUndo() {};
};

class ModificaUndo : public ActiuneUndo
{
private:
	Repository& repo;
	Produs prModif;
public:
	ModificaUndo(Repository& r, Produs& p) :repo{ r }, prModif{ p }{};
	void doUndo() override {
		repo.modifica(prModif);
	}
	~ModificaUndo() {};
};
