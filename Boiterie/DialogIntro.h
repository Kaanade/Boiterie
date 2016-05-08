#ifndef DIALOGINTRO_H
#define DIALOGINTRO_H

#include <QtWidgets>
#include <QDialog>
#include "FenetrePrincipale.h"

class DialogIntro : public QDialog
{
    Q_OBJECT

public:
    DialogIntro() {

        this->setWindowFlags(Qt::SplashScreen);
        this->setModal(true);
        QVBoxLayout * vbl_config = new QVBoxLayout;
        QLabel * labpix = new QLabel;
        QPixmap pix(":/Boitement/Resources/ecranaccueil.png");
        labpix->setPixmap(pix);
        labpix->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        vbl_config->addWidget(labpix);
        this->setLayout(vbl_config);


        fp = new FenetrePrincipale(this);

        this->hide();

        fp->show();
    }

protected:
    void mouseReleaseEvent ( QMouseEvent * event ) {
        this->hide();
        fp->show();
    }

private:
    FenetrePrincipale * fp;
};
#endif
