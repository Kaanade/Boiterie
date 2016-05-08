#pragma once

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtWidgets>
#include <QFrame>
#include <QPainter>
using namespace std;

class CustomProgressiveBar : public QHBoxLayout
{
public:
	void CustomProgressiveBar::addProgressBar1(int value1);
	void CustomProgressiveBar::addProgressBar2(int value2);
	void CustomProgressiveBar::addProgressBar3(int value3);
	void CustomProgressiveBar::addProgressBar4(int value4);
	void CustomProgressiveBar::initProgressBar(double values[]);
	void CustomProgressiveBar::update(double values[]);

private:
	QString styleProgress = "margin-right:0; margin-left:0; border:0;";
	QProgressBar *progress1 = new QProgressBar();
	QProgressBar *progress2 = new QProgressBar();
	QProgressBar *progress3 = new QProgressBar();
	QProgressBar *progress4 = new QProgressBar();
};

