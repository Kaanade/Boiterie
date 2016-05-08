#ifndef BOITERIE_H
#define BOITERIE_H

#include <QtWidgets/QMainWindow>
#include "ui_boiterie.h"

class Boiterie : public QMainWindow
{
	Q_OBJECT

public:
	Boiterie(QWidget *parent = 0);
	~Boiterie();

private:
	Ui::BoiterieClass ui;
};

#endif // BOITERIE_H
