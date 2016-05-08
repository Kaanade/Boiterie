#ifndef WIDGETSKELETON_H
#define WIDGETSKELETON_H

#include <vector>
#include <QtWidgets>
#include <Kinect.h>
#include <NuiApi.h>
#include "SkeletonData.h"
#include <iostream>

using namespace std;

#define KINECT1 100
#define KINECT2 200

class WidgetSkeleton : public QWidget {

    Q_OBJECT;

public:

	WidgetSkeleton(QWidget * parent = 0);


	void setDataSkeleton(vector<vector<Vector4>> & vec_s, vector<double> & vec_t);
	void setDataSkeleton(vector<vector<Joint>> & vec_s, vector<double> & vec_t);
	void setIdxSkeleton(int);
	void paintEvent(QPaintEvent *event);
	void setAxes(int);
    
	float convX(float);
	float convY(float);
	float convZ(float);

	void setdrawskeletonpoint(int idx, boolean b);
	void setSliderProperties(QSlider * slider);

    

private:
    std::vector<std::vector<Vector4>> vec_skeletons;
    std::vector<std::vector<Joint>> vec_skeletons2;
    std::vector<double> vec_times;
    float rx, ry, rz, minX, minY, minZ, maxX, maxY, maxZ;
    int axes; // 0 pour XY, 1 pour XZ, 2 pour YZ
    int idx_skeleton;
    int kinectDevice;
    float ttotal;
    std::vector<boolean> vec_draw_skeletonpoint;

	void paintSkeletonData(int);
	void paintSkeletonTimesData();
	QPoint getSkeletonPoint(Vector4 & vec, int axe);
	void lookExtremas(float * minX, float * minY, float * minZ, float * maxX, float *maxY, float * maxZ);


};
#endif
