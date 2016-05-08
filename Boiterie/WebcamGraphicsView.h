#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
#include <QGLWidget>
#include <QtWidgets>

/*!
    Class used with MyGraphicsScene. Manage the view of the scene.
    \author Michael Aron
*/

class WebcamGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    /*!
    Constructor
    Set OpenGL context to be ready to display an opengl texture.
    \param QWidget * which is a pointer to the parent QWidget object.

    */
    WebcamGraphicsView(QWidget *parent = 0) : QGraphicsView(parent){
        this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
        this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        this->setTransformationAnchor(AnchorUnderMouse);
    }


protected:

    void resizeEvent(QResizeEvent *event) {
        if (scene()) {
            QRectF scenerect = sceneRect();
            fitInView(scenerect);
        }
        //    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
        //QString s = "Taille de la vue : (" + QString::number(event->size().width()) + "," + QString::number(event->size().height()) + ")";
        //qWarning(s.toLatin1().data());

    }

};

#endif
