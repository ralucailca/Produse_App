#pragma once
#include <string>
#include <vector>
#include "Produs.h"
/*
Scrie in fisierul fName lista de produse
in format Comma Separated Values
arunca PetException daca nu poate crea fisierul
*/
void exportToCVS(const std::string& fName, const std::vector<Produs>& prod);
/*
Scrie in fisierul fName lista de produse
in format HTML
arunca PetException daca nu poate crea fisierul
*/
void exportToHTML(const std::string& fName, const std::vector<Produs>& prod);