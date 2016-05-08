#ifndef MYKINECT_H
#define MYKINECT_H

#define NOMINMAX //probleme de compilation avec qt et windows.h (conflit avec qdatetime.h)
#include <Windows.h>

#include <NuiApi.h>
#include <QtGui>
#include <opencv2/opencv.hpp>
//using namespace cv;
#include <time.h>
#include <fstream>

//lookups for color tinting based on player index
static const int g_IntensityShiftByPlayerR[] = { 1, 2, 0, 2, 0, 0, 2, 0 };
static const int g_IntensityShiftByPlayerG[] = { 1, 2, 2, 0, 2, 0, 0, 1 };
static const int g_IntensityShiftByPlayerB[] = { 1, 0, 2, 2, 0, 2, 0, 2 };

class MyKinect : public QThread {

    Q_OBJECT

public:
    MyKinect();
    ~MyKinect();

    void changeAngle(int);
    void stop();
    cv::Mat & getwebcam(){return webcam;}
    cv::vector<cv::vector<Vector4>> & getSkeletons();
    void getCoordinatesForSkeletonPoint(Vector4 &vec, LONG * x,  LONG  * y);
    void startRecord();
    void stopRecord();
    cv::vector<cv::vector<Vector4>> & getSavedSkeletons();
    cv::vector<double> & getSavedTimes();
    float getAngle();

protected:
    void run();


signals:
    void signal_changecolorframe();
    void signal_changedepthframe();
    void signal_changeskelpos();

public slots:
    void slot_changecolorframe();
    void slot_changedepthframe();
    void slot_changeskelpos();

private:

    static const int NUM_FRAMES = 2; // nombre d'images a buffuriser, 2 suffisent (max = 4)
    static const int WAIT_TIME = 100;
    static const int MAX_DIM = 2000;

    bool is_running;
    bool is_initialized;

    HANDLE m_hNextDepthFrameEvent;
    HANDLE m_hNextVideoFrameEvent;
    HANDLE m_pDepthStreamHandle;
    HANDLE m_pVideoStreamHandle;
    HANDLE m_hNextSkeletonEvent;

    double timestart;

    cv::Mat webcam;

    cv::vector<cv::vector<Vector4>> vec_skeletons;

    bool is_recording;
    cv::vector<cv::vector<Vector4>> vec_saved_skeletons;
    cv::vector<double> vec_saved_times;
    clock_t startingClock;

};

#endif
