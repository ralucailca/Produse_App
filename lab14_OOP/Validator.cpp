#include "Validator.h"
#include <assert.h>

void Validator::validare(const Produs& p) {
	vector<string> msgs;
	if (p.get_nume().size() == 0)
		msgs.push_back("Nume invalid!\n");
	if (p.get_tip().size() == 0)
		msgs.push_back("Tip invalid!\n");
	if (p.get_pret() <= 0)
		msgs.push_back("Pret invalid!\n");
	if (p.get_producator().size() == 0)
		msgs.push_back("Producator invalid!\n");
	if (msgs.size() > 0)
		throw ValidException(msgs);
}

ostream& operator<<(ostream& out, const ValidException& ex) {
	for (const auto& msg : ex.msgs) {
		out << msg;
	}
	return out;
}

void testValidator() {
	Validator v;
	v.validare(Produs("scaun", "mobila", 560, "lemn.srl"));
	try {
		v.validare(Produs("", "mobila", 0, "lemn.srl"));
		//assert(false);
	}
	catch (ValidException&) {
		assert(true);
	}

	try {
		v.validare(Produs("", "", -2, ""));
		//assert(false);
	}
	catch (ValidException&) {
		assert(true);
	}
}
