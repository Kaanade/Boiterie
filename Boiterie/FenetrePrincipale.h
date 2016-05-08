#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QtWidgets>
#include "DialogHelp.h"
#include "WebcamGraphicsScene.h"
#include "WebcamGraphicsView.h"
#include "WidgetSkeleton.h"
#include "SkeletonData.h"
#include "skeletondata2.h"
#include "WidgetAnalyse.h"
#include "MonLayoutVertical.h"

class FenetrePrincipale: public QMainWindow {
    Q_OBJECT
public:
    FenetrePrincipale(QWidget * parent = 0);

public Q_SLOTS:
    void slot_record();
    void slot_record2();
    void slot_savedata();
    void slot_loaddata();
    void slot_incKinectAngle();
    void slot_decKinectAngle();
    void slot_changeKinectAngle(int);
    void slot_disp_axesXY();
    void slot_disp_axesXZ();
    void slot_disp_axesYZ();
    void slot_disp_slider_time(int);
	void slot_analyse();

    /* Actions */
    void slot_action_draw_all();
    void slot_action_draw_nothing();
    void slot_action_draw_bottom();
    void slot_action_draw_top();
    void slot_action_draw_hip_center();
    void slot_action_draw_spine();
    void slot_action_draw_shoulder_center();
    void slot_action_draw_head();
    void slot_action_draw_shoulder_left();
    void slot_action_draw_elbow_left();
    void slot_action_draw_wrist_left();
    void slot_action_draw_hand_left();
    void slot_action_draw_shoulder_right();
    void slot_action_draw_elbow_right();
    void slot_action_draw_wrist_right();
    void slot_action_draw_hand_right();
    void slot_action_draw_hip_left();
    void slot_action_draw_knee_left();
    void slot_action_draw_ankle_left();
    void slot_action_draw_foot_left();
    void slot_action_draw_hip_right();
    void slot_action_draw_knee_right();
    void slot_action_draw_ankle_right();
    void slot_action_draw_foot_right();

private:
    SkeletonData * skeletondata;
    SkeletonData2 * skeletondata2;

    DialogHelp * dlghelp;
    QStatusBar * status;
    WebcamGraphicsScene * scene;
    WebcamGraphicsScene * scene2;

    QTabWidget * tabwidget;
    QAction *exitAct;

	// Boutons Kinect 1
    QPushButton * but_record;
    QPushButton * but_loaddata;
    QPushButton * but_savedata;
    QPushButton * but_kinect_inc_angle;
    QPushButton * but_kinect_dec_angle;

	// Boutons Kinect 2
    QPushButton * but_record2;
    QPushButton * but_loaddata2;
    QPushButton * but_savedata2;

	// Boutons Graphs
    QPushButton * but_axesXY;
    QPushButton * but_axesXZ;
    QPushButton * but_axesYZ;
    QPushButton * but_skeleton_traces;

	// Boutons Analyse
	QPushButton * but_acquisition;


    QWidget * BuildWidgetKinectCam();
    QWidget * BuildWidgetKinectCam2();
    QWidget * BuildWidgetKinectGraph();
	QWidget * BuildWidgetAnalyse();
	QWidget * widget_analyse, * widget_analyse2;
	QWidget * BuildWidgetKinectNews();

	QWidget * UpdateWidgetAnalyse(vector<Pas> vec_pas, WidgetAnalyse * widgetAnalyse);

    void build_actions();

    void draw_skeletoncurves(int);
    QSpinBox * spinbox_angle;

    WidgetSkeleton * widgetskeleton;
	WidgetAnalyse * widgetAnalyse;
    QSlider * slider_skeleton;

    std::vector<QAction *> vec_action_draw_skeleton;
	vector<MonLayoutVertical*> vec_layoutVertical;



};

#endif
