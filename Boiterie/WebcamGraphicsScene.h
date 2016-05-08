#ifndef WEBCAMGRAPHICSSCENE_H
#define WEBCAMGRAPHICSSCENE_H

#include "MyKinect.h"
#include "mykinect2.h"
#include <QtWidgets>
#include <QGLWidget>
#include <opencv2/opencv.hpp>
//using namespace cv;

#define KINECT_DEVICE       100
#define KINECT_DEVICE2      200

/*!
    Class permettant de gérer l'affichage d'une webcam dans une qgraphicsscene
    Le constructeur prend en paramètres 3 entiers : le numéro de la caméra (paramètre pour opencv), et les dimensions de l'image webcam capturée (largeur et hauteur)
    Chaque objet de cette classe comporte un timer pour gérer le rafraichissement
    La méthode drawbackground est automatiquement appelée par le timer et permet de dessiner l'image via une texture opengl
    L'image est récupérée via opencv puis convertie en une texture opengl ensuite dessinée par Qt
    Les méthode draw_xxx permettent de dessiner sur les images webcams et la méthode deleteItems d'effacer l'ensemble des dessins sur l'image
    Des méthodes mousexxx permettent de gérer la souris (positions, boutons, roulette)
    \author Michael Aron
*/

class WebcamGraphicsScene : public QGraphicsScene
{

    Q_OBJECT

public:
    WebcamGraphicsScene(int, int, int);
    WebcamGraphicsScene(QString, int, int);
    ~WebcamGraphicsScene();
    void drawBackground(QPainter *, const QRectF &);
    cv::Mat & getframe(){return cvframe;}
    void deleteItems();
    void deleteLastItem();
    void deleteRectItems();
    void deleteLineItems();
    void deleteTextItems();
    void grabimage(QString filename);
    int getnumdevice(){return numdevice;}
    void videopause();
    void videostart();
    int getnumframe(){return videocap.get(CV_CAP_PROP_POS_FRAMES);}
    void resetView();

    cv::vector<cv::vector<Vector4>> & getKinectSkeletons() { return mykinect->getSkeletons(); }
    cv::vector<cv::vector<Joint>> & getKinectSkeletons2() { return mykinect2->getSkeletons(); }
    void changeKinectAngle(float);
    float getKinectAngle();

    void draw_points(const std::vector<QPointF> &, QColor);
    void draw_point(const cv::Point2f &, QColor);
    void draw_rect(const cv::Rect &, QColor);
    void draw_rect(const QRectF &, QColor);
    void draw_rect(const std::vector<QRectF> &, QColor);
    void draw_rect(const std::vector<QRectF> &, const std::vector<QColor> &);
    void draw_rect(const std::vector<cv::Rect> &, QColor);
    void draw_rect(const std::vector<cv::Rect> &, const std::vector<QColor> &);
    void draw_rect(const std::vector<QPair<QRectF, QColor>> &);
    void draw_contours(const std::vector<QPoint> &, QColor, int step = 1);
    void draw_itemnumber(const std::vector<QRectF> &, QColor);
    void draw_itemnumber(const std::vector<QPair<QRectF, QColor>> &);
    void draw_polygon(const QPolygonF &, QColor);
    void draw_line(const float, const float, const float, const float, QColor);
    void draw_lines(const std::vector<cv::Point> &, QColor);
    void draw_lines(const QQueue<cv::Point> &, QColor);
    void draw_text(const QString &, QColor, QPoint);
    void draw_filledrect(const std::vector<cv::Rect> &, const std::vector<QColor> &);
    void draw_filledrect(const QRectF &, QColor);
    void draw_kinectskeletons(QColor);

    void record_data(bool b);
    void write_recorded_skeleton_data();

    cv::vector<cv::vector<Vector4>> & getSavedSkeletons() { return mykinect->getSkeletons(); }
    cv::vector<cv::vector<Joint>> & getSavedSkeletons2() { return mykinect2->getSkeletons(); }
    cv::vector<double> & getSavedTimes();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void wheelEvent(QGraphicsSceneWheelEvent *);
    QTimer * m_time;
    cv::VideoCapture videocap;
    CvSize framesize;
    cv::Mat cvframe;
    int framerate_cam;
    int numdevice;
    float m_distance;
    float m_tx, m_ty, m_tx_old, m_ty_old;
    MyKinect * mykinect;
    MyKinect2 * mykinect2;
    cv::Mat saved_cvframe;

signals:
    void signal_imagecaptured(int);

public slots :
    void slot_drawskeletons();

};



#endif
