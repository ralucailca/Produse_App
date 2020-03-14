#pragma once
#include <QAbstractListModel>
#include "Produs.h"
#include <vector>


class ListModel :public QAbstractListModel {
private:
	std::vector<Produs> produse;
public:
	ListModel(std::vector<Produs> p) :produse{ p } {};
	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return produse.size();
	}
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override{
		if (role == Qt::DisplayRole) {
			auto nume = produse[index.row()].get_nume();
			return QString::fromStdString(nume);
		}
		if (role == Qt::UserRole) {
			auto tip = produse[index.row()].get_tip();
			return QString::fromStdString(tip);
		}
		return QVariant{};
	}
};