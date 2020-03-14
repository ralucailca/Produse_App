#pragma once
#include "Service.h"
#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpainter.h>
#include "Observer.h"

class CosGUI : public QWidget, public Observer {
	Service& serv;
	QListWidget* lst;
	QPushButton* btnAdd;
	QPushButton* btnGnr;
	QPushButton* btnGl;
	QLineEdit* txtNume;
	QLineEdit* txtTip;
	QLineEdit* txtPret;
	QLineEdit* txtProducator;
	QLineEdit* txtNr;
	QLabel* txtSuma;

	void initGUIcomps();
	void connectSignalSlots();
	void reloadList(const vector<Produs>& prod);
	void adaugareNume();
	void generare();
	void golire();

public:
	CosGUI(Service& serv) :serv{ serv } {
		initGUIcomps();
		connectSignalSlots();
		reloadList(serv.getAllCos());
	}

	void update() override {
		reloadList(serv.getAllCos());
		txtSuma->setText(QString::number(serv.pretTotalCos()));
	}

	~CosGUI();
};

class CosGUILabel : public QLabel, public Observer {
private:
	Cos& cos;
public:
	CosGUILabel(Cos& c) : cos{ c } {
		setFont(QFont{ "arial",24 });
		setAttribute(Qt::WA_DeleteOnClose);
		cos.addObserver(this);
		update();
	}
	void update() {
		setText("Cosul contine: " + QString::number(cos.getAll().size()));
	}
	~CosGUILabel() {
		cos.removeObserver(this);
	}
};

class CosReadOnlyGUI : public QWidget, public Observer {
private:
	Cos& cos;
public:
	CosReadOnlyGUI(Cos& c) : cos{ c } { cos.addObserver(this); }
	void paintEvent(QPaintEvent* ) override {
		QPainter cp{ this };
		int poz = 0, down = 0;
		for (auto prod : cos.getAll()) {
			cp.drawRect(10 + poz * 60, 10 + down * 20, 50, 10);
			poz++;
			if (poz == 10) {
				down++;
				poz = 0;
			}
		}
	}

	void update() override {
		repaint();
	}

	~CosReadOnlyGUI() {
		cos.removeObserver(this);
	}
};

