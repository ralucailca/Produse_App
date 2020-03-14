#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab14_OOP.h"

class lab14_OOP : public QMainWindow
{
	Q_OBJECT

public:
	lab14_OOP(QWidget *parent = Q_NULLPTR);

private:
	Ui::lab14_OOPClass ui;
};
