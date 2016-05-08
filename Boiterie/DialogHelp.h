#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#include <QtWidgets>
#include <QDialog>


class DialogHelp : public QDialog
{
public:
    DialogHelp(QString t, QWidget *parent = 0) : QDialog(parent) {
        this->setGeometry(50, 50, 600, 800);
        QFile f(t);
        this->texte = new QTextEdit();
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            texte->setHtml(QString(f.readAll()));
            texte->setReadOnly(true);
        }

        fermer = new QPushButton("Fermer");
        QVBoxLayout * layoutPrincipal = new QVBoxLayout;
        layoutPrincipal->addWidget(texte);
        layoutPrincipal->addWidget(fermer);

        setLayout(layoutPrincipal);

        connect(fermer, SIGNAL(clicked()), this, SLOT(accept()));
    }

private:
    QTextEdit * texte;
    QPushButton * fermer;

};


#endif // DIALOGHELP_H
