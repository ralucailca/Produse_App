#pragma once
#include "Service.h"
#include "CosGUI.h"
#include "ListModel.h"
#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qwidget.h>
#include <qlabel.h>
#include <iterator>
#include <memory>
#include <stdlib.h>

class ProdusGUI : public QWidget
{
	CosGUI* cos;
	CosReadOnlyGUI* cosrd;
	CosGUILabel* coslb;
	Service& serv;
	//QListWidget* lst;
	QListView* lst;
	//ListModel* model;
	QPushButton* btnAdd;
	QPushButton* btnRemove;
	QPushButton* btnUpdate;
	QPushButton* btnSortNume;
	QPushButton* btnSortPret;
	QPushButton* btnSortNumeTip;
	QPushButton* btnSearch;
	QPushButton* btnUNDO;
	QPushButton* btnCos;
	QPushButton* btnCosrd;
	QPushButton* btnFProd;
	QPushButton* btnFNume;
	QPushButton* btnFPret;
	QPushButton* btnrmvf;
	QPushButton* btnGolCos;
	QPushButton* btnAddCos;
	QPushButton* btnGenCos;
	QLineEdit* txtNume;
	QLineEdit* txtTip;
	QLineEdit* txtPret;
	QLineEdit* txtProducator;
	//QLineEdit* txtSuma;
	QLabel* txtSuma;

	void initGUIcmps();
	void connectSignalsSlots();
	void reloadList( vector<Produs> prod);
	void addProd();
	void removeProd();
	void updateProd();
	void searchProd();
	void Undo();
public:
	ProdusGUI(Service& serv) : serv{ serv } {
		initGUIcmps();
		lst->setUniformItemSizes(true);
		reloadList(serv.getAll());
		connectSignalsSlots();
		
	};
	~ProdusGUI();
};

