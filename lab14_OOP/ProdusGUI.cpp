#include "ProdusGUI.h"
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QFormLayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>

void ProdusGUI::initGUIcmps() {
	cos = new CosGUI{ serv };
	cosrd = new CosReadOnlyGUI{ serv.getCos() };
	//fereastra care arata numarul de elemente din cos
	coslb = new CosGUILabel{ serv.getCos() };
	coslb->move(600, 600);
	coslb->show();
	//cos->setWindowModality(Qt::ApplicationModal);
	lst = new QListView;
	btnAdd = new QPushButton{ "Adauga" };
	btnRemove = new QPushButton{ "Sterge" };
	btnUpdate = new QPushButton{ "Modifica" };
	btnSortNume = new QPushButton{ "Sortare dupa nume" };
	btnSortPret = new QPushButton{ "Sortare dupa pret" };
	btnSortNumeTip = new QPushButton{ "Sortare dupa tip" };
	btnSearch = new QPushButton{ "Cauta" };
	btnUNDO = new QPushButton{ "UNDO" };
	btnCos = new QPushButton{ "Cos" };
	btnCosrd = new QPushButton{ "CosReadOnly" };
	btnFProd = new QPushButton{ "Filtru producator" };
	btnFNume = new QPushButton{ "Filtru nume" };
	btnFPret = new QPushButton{ "Filtru pret" };
	btnrmvf = new QPushButton{ "Elimina filtru" };
	btnGolCos = new QPushButton{ "Goleste cos" };
	btnAddCos = new QPushButton{ "Adauga in cos" };
	btnGenCos = new QPushButton{ "Genereaza cos" };
	txtNume = new QLineEdit;
	txtTip = new QLineEdit;
	txtPret = new QLineEdit;
	txtProducator = new QLineEdit;
	//txtSuma = new QLineEdit;
	txtSuma = new QLabel;

	QHBoxLayout* ly = new QHBoxLayout;
	setLayout(ly);

	//lista
	QWidget* widleft = new QWidget;
	QVBoxLayout* vl = new QVBoxLayout;
	widleft->setLayout(vl);
	vl->addWidget(lst);

	//butoanele de sortare sub lista
	QWidget* widBtnLeft = new QWidget;
	QHBoxLayout* btnLly = new QHBoxLayout;
	widBtnLeft->setLayout(btnLly);
	btnLly->addWidget(btnSortNume);
	btnLly->addWidget(btnSortPret);
	btnLly->addWidget(btnSortNumeTip);

	vl->addWidget(widBtnLeft);
	ly->addWidget(widleft);

	//form pentru detalii
	QWidget* widDetails = new QWidget;
	QFormLayout* formLy = new QFormLayout;
	widDetails->setLayout(formLy);
	formLy->addRow(new QLabel("Nume: "), txtNume);
	formLy->addRow(new QLabel("Tip: "), txtTip);
	formLy->addRow(new QLabel("Pret: "), txtPret);
	formLy->addRow(new QLabel("Producator: "), txtProducator);

	QWidget* widBtnRight = new QWidget;
	QHBoxLayout* btnRly = new QHBoxLayout;
	widBtnRight->setLayout(btnRly);
	btnRly->addWidget(btnAdd);
	btnRly->addWidget(btnUpdate);
	btnRly->addWidget(btnRemove);
	btnRly->addWidget(btnSearch);

	formLy->addWidget(widBtnRight);
	formLy->addWidget(btnUNDO);
	formLy->addWidget(btnCos);
	formLy->addWidget(btnCosrd);

	QWidget* widCos = new QWidget;
	QHBoxLayout* btnCosly = new QHBoxLayout;
	widCos->setLayout(btnCosly);
	btnCosly->addWidget(btnAddCos);
	btnCosly->addWidget(btnGolCos);
	//btnCosly->addWidget(btnGenCos);

	QWidget* widBtnRight2 = new QWidget;
	QHBoxLayout* btnRly2 = new QHBoxLayout;
	widBtnRight2->setLayout(btnRly2);
	btnRly2->addWidget(btnFNume);
	btnRly2->addWidget(btnFPret);
	btnRly2->addWidget(btnFProd);
	btnRly2->addWidget(btnrmvf);

	formLy->addWidget(widBtnRight2);
	txtSuma->setText(QString::number(serv.pretTotalCos()));
	formLy->addRow(new QLabel{ "Suma totala din cos: " }, txtSuma);
	formLy->addWidget(widCos);
	ly->addWidget(widDetails);
	//QMessageBox::Information(*this,QString::number(serv.pretTotalCos()));
}

void ProdusGUI::reloadList( std::vector<Produs> prod) {
	ListModel* model = new ListModel( prod );
	lst->setModel(model);
	QObject::connect(lst->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
		if (lst->selectionModel()->selectedIndexes().isEmpty()) {
			txtNume->setText("");
			txtPret->setText("");
			txtProducator->setText("");
			txtTip->setText("");
			return;
		}
		auto item = lst->selectionModel()->selectedIndexes().at(0);
		//QString nume = item.data(Qt::DisplayRole).toString();
		QString nume = lst->model()->data(item, Qt::DisplayRole).toString();
		txtNume->setText(nume);
		//QString tip = item.data(Qt::UserRole).toString();
		QString tip = lst->model()->data(item, Qt::UserRole).toString();
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
	//QModelIndex topLeft = createIndex(0);  
	//QModelIndex bottomRight = createIndex(rowCount());  
	//emit dataChanged(topLeft, bottomRight);
}

void ProdusGUI::connectSignalsSlots() {
	QObject::connect(btnSortNume, &QPushButton::clicked, [&]() {reloadList(serv.sortareNume()); });
	QObject::connect(btnSortPret, &QPushButton::clicked, [&]() {reloadList(serv.sortarePret()); });
	QObject::connect(btnSortNumeTip, &QPushButton::clicked, [&]() {reloadList(serv.sortareNumeTip()); });
	QObject::connect(lst->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
		if (lst->selectionModel()->selectedIndexes().isEmpty()) {
			txtNume->setText("");
			txtPret->setText("");
			txtProducator->setText("");
			txtTip->setText("");
			return;
		}
		auto item = lst->selectionModel()->selectedIndexes().at(0);
		//QString nume = item.data(Qt::DisplayRole).toString();
		QString nume = lst->model()->data(item, Qt::DisplayRole).toString();
		txtNume->setText(nume);
		//QString tip = item.data(Qt::UserRole).toString();
		QString tip = lst->model()->data(item, Qt::UserRole).toString();
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
	QObject::connect(lst->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {btnAddCos->setEnabled(!lst->selectionModel()->selectedIndexes().isEmpty()); });
	QObject::connect(btnAdd, &QPushButton::clicked, this, &ProdusGUI::addProd);
	QObject::connect(btnUpdate, &QPushButton::clicked, this, &ProdusGUI::updateProd);
	QObject::connect(btnRemove, &QPushButton::clicked, this, &ProdusGUI::removeProd);
	QObject::connect(btnSearch, &QPushButton::clicked, this, &ProdusGUI::searchProd);
	QObject::connect(btnUNDO, &QPushButton::clicked, this, &ProdusGUI::Undo);
	QObject::connect(btnCos, &QPushButton::clicked, [&]() { cos->show(); txtSuma->setText(QString::number(serv.pretTotalCos())); });
	QObject::connect(btnCosrd, &QPushButton::clicked, [&]() { cosrd->show(); });
	QObject::connect(btnFNume, &QPushButton::clicked, [&]() {reloadList(serv.filtruNume(txtNume->text().toStdString())); });
	QObject::connect(btnFPret, &QPushButton::clicked, [&]() {reloadList(serv.filtruPret(txtPret->text().toInt())); });
	QObject::connect(btnFProd, &QPushButton::clicked, [&]() {reloadList(serv.filtruProducator(txtProducator->text().toStdString())); });
	QObject::connect(btnrmvf, &QPushButton::clicked, [&]() { reloadList(serv.getAll()); });
	QObject::connect(btnAddCos, &QPushButton::clicked, [&]() {serv.adaugaCos(txtNume->text().toStdString()); });
	QObject::connect(btnGolCos, &QPushButton::clicked, [&]() {serv.golesteCos(); });
	//QObject::connect(btnGenCos, &QpushButton::cliked, [&](){});
}

void ProdusGUI::addProd() {
	try {
		serv.adaugaProdus(txtNume->text().toStdString(), txtTip->text().toStdString(), txtPret->text().toInt(), txtProducator->text().toStdString());
		reloadList(serv.getAll());
	}
	catch (RepoException &ex) {
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
	}
	catch (ValidException &ex) {
		string m{ "" };
		for (const auto& msg : ex.getMsgs()) {
			m = m + " " + msg;
		}
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(m));
	}

}

void ProdusGUI::updateProd() {
	try {
		serv.modificaProdus(txtNume->text().toStdString(), txtTip->text().toStdString(), txtPret->text().toInt(), txtProducator->text().toStdString());
		reloadList(serv.getAll());
	}
	catch (RepoException &ex) {
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
	}
	catch (ValidException &ex) {
		string m{ "" };
		for (const auto& msg : ex.getMsgs()) {
			m = m + " " + msg;
		}
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(m));
	}
}

void ProdusGUI::removeProd() {
	try {
		serv.stergeProdus(txtNume->text().toStdString(), txtTip->text().toStdString());
		reloadList(serv.getAll());
	}
	catch (RepoException &ex) {
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
	}
}

void ProdusGUI::searchProd() {
	try {
		Produs p = serv.cautaProdus(txtNume->text().toStdString(), txtTip->text().toStdString());
		txtPret->setText(QString::number(p.get_pret()));
		txtProducator->setText(QString::fromStdString(p.get_producator()));
	}
	catch (RepoException &ex) {
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
	}
}

void ProdusGUI::Undo() {
	try {
		serv.undo();
		reloadList(serv.getAll());
	}
	catch (RepoException &ex) {
		QMessageBox::warning(nullptr, "Warning: ", QString::fromStdString(ex.getMsg()));
	}
}

ProdusGUI::~ProdusGUI() {
}
