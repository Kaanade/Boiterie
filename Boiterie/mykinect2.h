#ifndef MYKINECT2_H
#define MYKINECT2_H

#define NOMINMAX //probleme de compilation avec qt et windows.h (conflit avec qdatetime.h)
#include <Windows.h>
#include "stdafx.h"

#include <QtGui>
#include <opencv2/opencv.hpp>
//using namespace cv;
#include <time.h>
#include <fstream>


class MyKinect2 : public QThread {

    Q_OBJECT

    static const int        cColorWidth  = 1920;
    static const int        cColorHeight = 1080;
    static const int        cDepthWidth  = 512;
    static const int        cDepthHeight = 424;


public :

    MyKinect2();

    ~MyKinect2();

    int getColorWidth() { return cColorWidth; }
    int getColorHeight() { return cColorHeight; }

    void stop() { is_running = false; }

    cv::Mat & getwebcam(){return webcam;}
    cv::vector<cv::vector<Joint>> & getSkeletons() { return this->vec_skeletons; }
    void getCoordinatesForSkeletonPoint(Joint &vec, LONG * x,  LONG  * y);
    void startRecord();
    void stopRecord() { this->is_recording = false; }
    cv::vector<cv::vector<Joint>> & getSavedSkeletons() { return this->vec_saved_skeletons; }
    cv::vector<double> & getSavedTimes() { return this->vec_saved_times; }

protected :
    void run();


signals:
    void signal_changecolorframe();
    void signal_changedepthframe();
    void signal_changeskelpos();

private:

    // Current Kinect
    IKinectSensor*          m_pKinectSensor;

    // Color reader
    IColorFrameReader*      m_pColorFrameReader;
    // Body reader
    IBodyFrameReader*       m_pBodyFrameReader;

    RGBQUAD*                m_pColorRGBX;

    ICoordinateMapper*   m_pCoordinateMapper;


    bool is_running;

    double timestart;


    cv::Mat webcam;

    cv::vector<cv::vector<Joint>> vec_skeletons;

    bool is_recording;
    cv::vector<cv::vector<Joint>> vec_saved_skeletons;
    cv::vector<double> vec_saved_times;
    clock_t startingClock;


private slots :

    HRESULT             InitializeDefaultSensor();

    void                    Update();

    void                    ProcessBody( int nBodyCount, IBody** ppBodies);

};

#endif // MYKINECT2_H
