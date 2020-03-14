#include "CosGUI.h"
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QFormLayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>

void CosGUI::initGUIcomps() {
	lst = new QListWidget;
	btnAdd = new QPushButton{ "Adauga" };
	btnGnr = new QPushButton{ "Genereaza" };
	btnGl = new QPushButton{ "Goleste" };
	txtNume = new QLineEdit;
	txtTip = new QLineEdit;
	txtPret = new QLineEdit;
	txtProducator = new QLineEdit;
	txtNr = new QLineEdit;
	txtSuma = new QLabel;

	QHBoxLayout* ly = new QHBoxLayout;
	setLayout(ly);

	ly->addWidget(lst);

	//form pentru detalii
	QWidget* widDetails = new QWidget;
	QFormLayout* formLy = new QFormLayout;
	widDetails->setLayout(formLy);
	formLy->addRow(new QLabel("Nume: "), txtNume);
	formLy->addRow(new QLabel("Tip: "), txtTip);
	formLy->addRow(new QLabel("Pret: "), txtPret);
	formLy->addRow(new QLabel("Producator: "), txtProducator);

	formLy->addWidget(btnAdd);
	formLy->addRow(btnGnr, txtNr);
	formLy->addWidget(btnGl);
	txtSuma->setText(QString::number(serv.pretTotalCos()));
	formLy->addRow(new QLabel{ "Suma totala din cos: " }, txtSuma);

	ly->addWidget(widDetails);
}

void CosGUI::reloadList(const vector<Produs>& prod) {
	lst->clear();
	for (const auto& p : prod) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.get_nume()), lst);
		item->setData(Qt::UserRole, QString::fromStdString(p.get_tip()));
	}
}

void CosGUI::connectSignalSlots() {
	serv.addObserver(this);
	QObject::connect(lst, &QListWidget::itemSelectionChanged, [&]() {
		if (lst->selectedItems().isEmpty()) {
			txtNume->setText("");
			txtPret->setText("");
			txtProducator->setText("");
			txtTip->setText("");
			return;
		}
		auto item = lst->selectedItems().at(0);
		QString nume = item->text();
		txtNume->setText(nume);
		QString tip = item->data(Qt::UserRole).toString();
		txtTip->setText(tip);
		try {
			Produs p = serv.cautaProdus(nume.toStdString(), tip.toStdString());
			txtPret->setText(QString::number(p.get_pret()));
			txtProducator->setText(QString::fromStdString(p.get_producator()));
		}
		catch (RepoException &ex) {
			QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
		}
	});
	QObject::connect(btnAdd, &QPushButton::clicked, this, &CosGUI::adaugareNume);
	QObject::connect(btnGnr, &QPushButton::clicked, this, &CosGUI::generare);
	QObject::connect(btnGl, &QPushButton::clicked, this, &CosGUI::golire);
}

void CosGUI::adaugareNume() {
	serv.adaugaCos(txtNume->text().toStdString());
	txtSuma->setText(QString::number(serv.pretTotalCos()));
	reloadList(serv.getAllCos());
}
void CosGUI::generare() {
	serv.genereazaCos(txtNr->text().toInt());
	txtSuma->setText(QString::number(serv.pretTotalCos()));
	reloadList(serv.getAllCos());
}
void CosGUI::golire() {
	serv.golesteCos();
	txtSuma->setText(QString::number(serv.pretTotalCos()));
	reloadList(serv.getAllCos());
}

CosGUI::~CosGUI() {
	serv.removeObserver(this);
}
