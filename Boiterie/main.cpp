#include <QtWidgets/QApplication>
#include "DialogIntro.h"
#include "FenetrePrincipale.h"
#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //DialogIntro dlgintro;
    //dlgintro.show();

    FenetrePrincipale fp;
    fp.show();

    return a.exec();

	return 0;
}
