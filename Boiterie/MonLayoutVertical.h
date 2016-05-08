#pragma once

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtWidgets>
#include <QFrame>
#include <QPainter>
#include "CustomProgressBar.h"
using namespace std;

class MonLayoutVertical : public QVBoxLayout{
public:
	void MonLayoutVertical::addlabel(QString labelcontent);
	void MonLayoutVertical::addlabel2(QString labelcontent);
	void MonLayoutVertical::addvbox();
	void MonLayoutVertical::addgroupbox();
	void MonLayoutVertical::addprogressbar(double values[]);
	void MonLayoutVertical::addbutton();
	void MonLayoutVertical::addvbox2();
	void MonLayoutVertical::addgroupbox2();
	void MonLayoutVertical::updateValuesLayout(double values[]);

private:
	QLabel *label = new QLabel();
	QLabel *label2 = new QLabel();
	QVBoxLayout *vbox = new QVBoxLayout;
	QGroupBox *groupBox1 = new QGroupBox("Longeur du pas");
	QProgressBar *progress = new QProgressBar();
	QPushButton *bouton = new QPushButton("Bouton");
	QVBoxLayout *vbox2 = new QVBoxLayout;
	QGroupBox *groupBox2 = new QGroupBox("Information du pas");
	CustomProgressiveBar * ProgressTotal = new CustomProgressiveBar();

};

