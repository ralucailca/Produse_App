#include "lab14_OOP.h"
#include <QtWidgets/QApplication>
#include "Repository.h"
#include "Validator.h"
#include "Service.h"
#include "Cos.h"
#include "ProdusGUI.h"


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void allTests() {
	testRepo();
	testValidator();
	testService();
	testCos();
	testFileRepo();
}

int main(int argc, char *argv[])
{
	allTests();
	{
		QApplication a(argc, argv);
		//Repository repo;
		FileRepo repo{ "produse.txt" };
		Validator valid;
		Cos cos;
		Service serv{ repo, valid, cos };
		ProdusGUI gui{ serv };
		gui.show();
		a.exec();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
