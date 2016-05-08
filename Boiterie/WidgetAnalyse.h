#ifndef WIDGETANALYSE_H
#define WIDGETANALYSE_H

#include <QtWidgets/QApplication>
#include <QtWidgets>
#include <QFile>
#include "SkeletonData.h"
#include <vector>
#include <Kinect.h>
#include <algorithm>
#include <Pas.h>
#include <Phase.h>
#include <cmath>
using namespace std;


#define KINECT1 100
#define KINECT2 200

class WidgetAnalyse : public QWidget{

	public:
		WidgetAnalyse(QWidget * parent = 0);

		void longueurPas();
		void demiPas();
		void setDataSkeleton(vector<vector<Vector4>> & vec_s, vector<double> & vec_t);
		void setDataSkeleton(vector<vector<Joint>> & vec_s, vector<double> & vec_t);
		pair<double, int> WidgetAnalyse::min_element(vector<pair<double, int>> vec_norme_pos);
		pair<double, int> WidgetAnalyse::max_element2(vector<pair<double, int>> vec_norme_pos);
		string WidgetAnalyse::quelPied(int pos);
		void WidgetAnalyse::derivee();
		void WidgetAnalyse::phases();
		void WidgetAnalyse::calculCycles();
		vector<Pas> WidgetAnalyse::getVecPas();
		double temp;
		

	private:
		vector<vector<Vector4>> vec_skeletons;
		vector<vector<Joint>> vec_skeletons2;
		vector<double> vec_times;
		vector<vector<double>> vec_norme_pos;
		vector<Pas> vec_pas;
		vector<double> vec_norme, vec_derivee, vec_smooth;
		

		int idx_skeleton;
		int kinectDevice;
		int ankle_left, ankle_right;
		float ttotal;
		int temps;
		vector<boolean> vec_draw_skeletonpoint;

		vector<Phase> vec_phases;
		
		
};


#endif