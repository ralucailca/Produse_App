#include "Export.h"
#include "Produs.h"
#include "Repository.h"
#include <fstream>
#include <string>
#include <vector>
/*
Scrie in fisierul fName lista de animale
in format Comma Separated Values
arunca PetException daca nu poate crea fisierul
*/
void exportToCVS(const std::string& fName, const std::vector<Produs>& prod) {
	std::ofstream out(fName, std::ios::trunc);
	if (!out.is_open()) {
		throw RepoException("Unable to open file:" + fName);
	}
	for (const auto& p : prod) {
		out << p.get_nume() << ",";
		out << p.get_tip() << ",";
		out << p.get_pret() << ",";
		out << p.get_producator() << std::endl;
	}
	out.close();
}
/*
Scrie in fisierul fName lista de animale
in format HTML
arunca PetException daca nu poate crea fisierul
*/
void exportToHTML(const std::string& fName, const std::vector<Produs>& prod) {
	std::ofstream out(fName, std::ios::trunc);
	if (!out.is_open()) {
		throw RepoException("Unable to open file:" + fName);
	}
	out << "<html><body>" << std::endl;
	out << "<table border=\"1\" style=\"width:100 % \">" << std::endl;
	for (const auto& p : prod) {
		out << "<tr>" << std::endl;
		out << "<td>" << p.get_nume() << "</td>" << std::endl;
		out << "<td>" << p.get_tip() << "</td>" << std::endl;
		out << "<td>" << p.get_pret() << "</td>" << std::endl;
		out << "<td>" << p.get_producator() << "</td>" << std::endl;
		out << "</tr>" << std::endl;
	}
	out << "</table>" << std::endl;
	out << "</body></html>" << std::endl;
	out.close();
}
