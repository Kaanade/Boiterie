#include "CustomProgressBar.h"

void CustomProgressiveBar::addProgressBar1(int value1)
{
	this->addWidget(progress1);
	progress1->setStyleSheet("QProgressBar:horizontal {" + styleProgress + ";}");
	progress1->setStyleSheet("QProgressBar::chunk{background-color:green}");
	progress1->setFormat("%v%");
	progress1->setValue(value1);
	progress1->setMaximumWidth(value1);
	progress1->setMaximum(value1);
}

void CustomProgressiveBar::addProgressBar2(int value2)
{
	this->addWidget(progress2);
	progress2->setStyleSheet("QProgressBar:horizontal {" + styleProgress + ";}");
	progress2->setStyleSheet("QProgressBar::chunk{background-color:blue}");
	progress2->setFormat("%v%");
	progress2->setValue(value2);
	progress2->setMaximumWidth(value2);
	progress2->setMaximum(value2);
}

void CustomProgressiveBar::addProgressBar3(int value3)
{
	this->addWidget(progress3);
	progress3->setStyleSheet("QProgressBar:horizontal {" + styleProgress + ";}");
	progress3->setStyleSheet("QProgressBar::chunk{background-color:yellow}");
	progress3->setFormat("%v%");
	progress3->setValue(value3);
	progress3->setMaximumWidth(value3);
	progress3->setMaximum(value3);
}

void CustomProgressiveBar::addProgressBar4(int value4)
{
	this->addWidget(progress4);
	progress4->setStyleSheet("QProgressBar:horizontal {" + styleProgress + ";}");
	progress4->setStyleSheet("QProgressBar::chunk{background-color:red}");
	progress4->setFormat("%v%");
	progress4->setValue(value4);
	progress4->setMaximumWidth(value4);
	progress4->setMaximum(value4);

	progress4->setGeometry(0, 0, 200, 200);
	this->addStretch();
	this->setSpacing(0);
}

void CustomProgressiveBar::initProgressBar(double values[])
{
	/*int max = values[0] + values[1] + values[2] + values[3];
	for (int i = 0; i < 4; i++){
	values[i] = (values[i] * 400)/max;
	}*/
	addProgressBar1(values[0]);
	addProgressBar2(values[1]);
	addProgressBar3(values[2]);
	addProgressBar4(values[3]);
}

void CustomProgressiveBar::update(double values[])
{
	progress1->setValue(values[0]);
	progress1->setMaximumWidth(values[0]);
	progress1->setMaximum(values[0]);

	progress2->setValue(values[1]);
	progress2->setMaximumWidth(values[1]);
	progress2->setMaximum(values[1]);

	progress3->setValue(values[2]);
	progress3->setMaximumWidth(values[2]);
	progress3->setMaximum(values[2]);

	progress4->setValue(values[3]);
	progress4->setMaximumWidth(values[3]);
	progress4->setMaximum(values[3]);
}