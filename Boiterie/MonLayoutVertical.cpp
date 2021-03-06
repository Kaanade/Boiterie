#include "MonLayoutVertical.h"

MonLayoutVertical::MonLayoutVertical()
{
	ProgressTotal = new CustomProgressiveBar();
}

void MonLayoutVertical::addlabel(QString labelcontent)
{
	label->setText(labelcontent);
	vbox->addWidget(label);
}

void MonLayoutVertical::addlabel2(QString labelcontent)
{
	label2->setText(labelcontent);
	vbox->addWidget(label2);
}

void MonLayoutVertical::addlabel3(QString labelcontent)
{
	label3->setText(labelcontent);
	vbox->addWidget(label3);
}

void MonLayoutVertical::addvbox()
{
	groupBox1->setLayout(vbox);
}

void MonLayoutVertical::addgroupbox()
{
	this->addWidget(groupBox1);
}

void MonLayoutVertical::addprogressbar(double values[])
{
	vbox2->addLayout(ProgressTotal);
	ProgressTotal->initProgressBar(values);
}

void MonLayoutVertical::addbutton()
{
	vbox2->addWidget(bouton);
}

void MonLayoutVertical::addvbox2()
{
	groupBox2->setLayout(vbox2);
}

void MonLayoutVertical::addgroupbox2(){
	this->addWidget(groupBox2);
}

void MonLayoutVertical::updateValuesLayout(double values[])
{
	this->ProgressTotal->update(values);
}
