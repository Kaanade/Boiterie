#include "FenetrePrincipale.h"

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent) {

    this->setGeometry(250,50,1450,850);
    this->setWindowTitle("ISEN Bretagne : detecting and recording skeleton moves");
    this->setWindowIcon(QIcon(":/Boitement/Resources/cnr-pending.png"));

    /* creation de la fenetre d'aide */
    this->dlghelp = new DialogHelp(":/Boitement/Resources/aide.html", this);

    //---------------------------------------------------------------
    //Barre de menu
    //---------------------------------------------------------------
    QMenu * menufichier = menuBar()->addMenu("&File");

    exitAct = new QAction(QIcon(":/Boitement/Resources/exit.png"), tr("&Exit"),this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    //---------------------------------------------------------------
    //Ajout au menu
    //---------------------------------------------------------------
    menufichier->addAction(exitAct);

    //---------------------------------------------------------------
    //Barre de status
    //---------------------------------------------------------------
    status = statusBar();
    QString msg = "Ready for a new acquisition";
    status->showMessage(msg);

    //------------------TABS-----------------------------------------
    //---------------------------------------------------------------
	widget_analyse = BuildWidgetAnalyse();
    tabwidget = new QTabWidget;
    tabwidget->addTab(BuildWidgetKinectCam(), tr("Kinect"));
    tabwidget->addTab(BuildWidgetKinectCam2(), tr("Kinect v2"));
    tabwidget->addTab(BuildWidgetKinectGraph(), tr("Graphs"));
	tabwidget->addTab(widget_analyse, tr("Analyse"));
	//tabwidget->addTab(BuildWidgetKinectNews(), tr("News"));

    tabwidget->setTabEnabled(2, false);
	tabwidget->setTabEnabled(3, false);
    this->setCentralWidget(tabwidget);
}

QWidget * FenetrePrincipale::BuildWidgetKinectCam() {
    QHBoxLayout * hlay = new QHBoxLayout;

    QVBoxLayout * vlay_buttons = new QVBoxLayout;

    QHBoxLayout * hlay_but_acq = new QHBoxLayout;
    but_record = new QPushButton(tr("Acq : off"), this);
    but_record->setCheckable(true);
    but_record->setToolTip("Start acquisition of skeleton data");
    connect(but_record, SIGNAL(clicked()), this, SLOT(slot_record()));
    but_savedata = new QPushButton(tr("Save*"), this);
    but_savedata->setToolTip("Save acquired data");
    but_savedata->setEnabled(false);
    connect(but_savedata, SIGNAL(clicked()), this, SLOT(slot_savedata()));

    hlay_but_acq->addWidget(but_record);
    hlay_but_acq->addWidget(but_savedata);

    but_loaddata = new QPushButton(tr("Load"), this);
    but_loaddata->setToolTip("Load skeleton data (*.yml)");
    connect(but_loaddata, SIGNAL(clicked()), this, SLOT(slot_loaddata()));

    vlay_buttons->addLayout(hlay_but_acq);
    vlay_buttons->addWidget(but_loaddata);

    QHBoxLayout * hlay_ka = new QHBoxLayout;
    QLabel * lab_ka = new QLabel;
    lab_ka->setText(tr("Kinect angle : "));
    spinbox_angle = new QSpinBox;
    spinbox_angle->setMinimum(-27);
    spinbox_angle->setMaximum(27);
    spinbox_angle->setSingleStep(1);
    connect(spinbox_angle, SIGNAL(valueChanged(int)), this, SLOT(slot_changeKinectAngle(int)));
    but_kinect_inc_angle = new QPushButton("+", this);
    connect(but_kinect_inc_angle, SIGNAL(clicked()), this, SLOT(slot_incKinectAngle()));
    but_kinect_dec_angle = new QPushButton("-", this);
    connect(but_kinect_dec_angle, SIGNAL(clicked()), this, SLOT(slot_decKinectAngle()));
    hlay_ka->addWidget(lab_ka);
    hlay_ka->addWidget(spinbox_angle);
    hlay_ka->addWidget(but_kinect_inc_angle);
    hlay_ka->addWidget(but_kinect_dec_angle);
    vlay_buttons->addLayout(hlay_ka);

    scene = new WebcamGraphicsScene(100, 640, 480);
    WebcamGraphicsView * view = new WebcamGraphicsView;
    view->setScene(scene);

    spinbox_angle->setValue(scene->getKinectAngle());

    hlay->addLayout(vlay_buttons);
    hlay->addWidget(view);

    QWidget * widget = new QWidget;
    widget->setLayout(hlay);
    return widget;
}

QWidget * FenetrePrincipale::BuildWidgetKinectCam2() {
    QHBoxLayout * hlay = new QHBoxLayout;

    QVBoxLayout * vlay_buttons = new QVBoxLayout;

    QHBoxLayout * hlay_but_acq = new QHBoxLayout;
    but_record2 = new QPushButton(tr("Acq : off"), this);
    but_record2->setCheckable(true);
    but_record2->setToolTip("Start acquisition of skeleton data");
    connect(but_record2, SIGNAL(clicked()), this, SLOT(slot_record2()));
    but_savedata2 = new QPushButton(tr("Save*"), this);
    but_savedata2->setToolTip("Save acquired data");
    but_savedata2->setEnabled(false);
    connect(but_savedata2, SIGNAL(clicked()), this, SLOT(slot_savedata()));

    hlay_but_acq->addWidget(but_record2);
    hlay_but_acq->addWidget(but_savedata2);

    but_loaddata2 = new QPushButton(tr("Load"), this);
    but_loaddata2->setToolTip("Load skeleton data (*.yml)");
    connect(but_loaddata2, SIGNAL(clicked()), this, SLOT(slot_loaddata()));

    vlay_buttons->addLayout(hlay_but_acq);
    vlay_buttons->addWidget(but_loaddata2);


    scene2 = new WebcamGraphicsScene(200, 640, 480);
    WebcamGraphicsView * view = new WebcamGraphicsView;
    view->setScene(scene2);

    hlay->addLayout(vlay_buttons);
    hlay->addWidget(view);

    QWidget * widget = new QWidget;
    widget->setLayout(hlay);
    return widget;
}

QWidget * FenetrePrincipale::BuildWidgetKinectGraph() {
    QHBoxLayout * lay = new QHBoxLayout;

    QVBoxLayout * vlay_buttons = new QVBoxLayout;

    but_axesXY = new QPushButton(tr("XY"), this);
    but_axesXZ = new QPushButton(tr("XZ"), this);
    but_axesYZ = new QPushButton(tr("YZ"), this);
    vlay_buttons->addWidget(but_axesXY);
    vlay_buttons->addWidget(but_axesXZ);
    vlay_buttons->addWidget(but_axesYZ);
    connect(but_axesXY, SIGNAL(clicked()), this, SLOT(slot_disp_axesXY()));
    connect(but_axesXZ, SIGNAL(clicked()), this, SLOT(slot_disp_axesXZ()));
    connect(but_axesYZ, SIGNAL(clicked()), this, SLOT(slot_disp_axesYZ()));

    slider_skeleton = new QSlider;
    //slider_skeleton->setOrientation(Qt::Horizontal); // ca fait n'importe quoi !!!
    slider_skeleton->setMaximumSize(QSize(100,100));
    connect(slider_skeleton, SIGNAL(valueChanged(int)), this, SLOT(slot_disp_slider_time(int)));
    vlay_buttons->addWidget(slider_skeleton);

    but_skeleton_traces = new QPushButton(tr("Skeleton parts"), this);
    but_skeleton_traces->setMaximumWidth(100);
    build_actions();
    QMenu * menu_but_traces = new QMenu;
    menu_but_traces->addAction(vec_action_draw_skeleton.at(20)); //draw all
    menu_but_traces->addAction(vec_action_draw_skeleton.at(21)); //draw nothing
    menu_but_traces->addSeparator();
    menu_but_traces->addAction(vec_action_draw_skeleton.at(22)); //draw top
    menu_but_traces->addAction(vec_action_draw_skeleton.at(23)); //draw bottom
    menu_but_traces->addSeparator();
    for (int i = 0; i < 20; i++)
        menu_but_traces->addAction(vec_action_draw_skeleton.at(i));

    but_skeleton_traces->setMenu(menu_but_traces);
    vlay_buttons->addWidget(but_skeleton_traces);

    this->skeletondata = NULL;

    QVBoxLayout * vlay_widget = new QVBoxLayout;
    widgetskeleton = new WidgetSkeleton(this);
    widgetskeleton->setMinimumHeight(500);
	widgetskeleton->setMaximumWidth(500);
    vlay_widget->addWidget(widgetskeleton);

    lay->addLayout(vlay_buttons);
    lay->addLayout(vlay_widget);

    QWidget * widget = new QWidget;
    widget->setLayout(lay);
    return widget;
}

QWidget * FenetrePrincipale::BuildWidgetAnalyse() {
	QHBoxLayout * lay = new QHBoxLayout;
	QWidget fenetre;
	QVBoxLayout * vlay_widget = new QVBoxLayout;
	
	widgetAnalyse = new WidgetAnalyse(this);
	vlay_widget->addWidget(widgetAnalyse);
	//vec_pas = widgetAnalyse->getVecPas();
	//vec_layoutVertical.clear();
	//int nbPas = 1;
	//int i;
	//double val1 = 10.5;
	//double val2 = 40.5;
	//double val3 = 10;
	//double val4 = 39;
	//double values[4] = {
	//	val1, val2, val3, val4
	//};
	////Création des instances MaGroupBox
	//for (i = 0; i < nbPas; i++){
	//	MonLayoutVertical * vLayout = new MonLayoutVertical();
	//	vLayout->addlabel("Premiere information");
	//	vLayout->addlabel2("Deuxieme information");
	//	vLayout->addvbox();
	//	vLayout->addgroupbox();
	//	vLayout->addprogressbar(values);
	//	vLayout->addbutton();
	//	vLayout->addvbox2();
	//	vLayout->addgroupbox2();

	//	//placer layout

	//	lay->addLayout(vLayout);
	//	vec_layoutVertical.push_back(vLayout);
	//}
	//fenetre.setLayout(lay);
	//fenetre.show();

	QWidget * widget = new QWidget;
	widget->setLayout(lay);
	return widget;
}

QWidget * FenetrePrincipale::UpdateWidgetAnalyse( vector<Pas> vec_pas, WidgetAnalyse * widgetAnalyse) {
	QHBoxLayout * lay = new QHBoxLayout;
	QWidget fenetre;
	QVBoxLayout * vlay_widget = new QVBoxLayout;

	vlay_widget->addWidget(widgetAnalyse);
	
	int nbPas = 1;
	int i;
	
	//Création des instances MaGroupBox
	for (i = 0; i < nbPas; i++){

		double values[4] = {
			vec_pas.at(i).percent_A, vec_pas.at(i).percent_B, vec_pas.at(i).percent_C, vec_pas.at(i).percent_D
		};
		
		MonLayoutVertical * vLayout = new MonLayoutVertical();
		vLayout->addlabel(QString::fromStdString(vec_pas.at(i).first));
		vLayout->addlabel2("Demi Pas Droit :" + QString::number(vec_pas.at(i).demiPasD));
		vLayout->addlabel3("Demi Pas Gauche :" + QString::number(vec_pas.at(i).demiPasG));
		vLayout->addvbox();
		vLayout->addgroupbox();
		vLayout->addprogressbar(values);
		vLayout->addbutton();
		vLayout->addvbox2();
		vLayout->addgroupbox2();

		//placer layout

		lay->addLayout(vLayout);
		vec_layoutVertical.push_back(vLayout);
	}
	fenetre.setLayout(lay);
	fenetre.show();

	QWidget * new_widget = new QWidget;
	new_widget->setLayout(lay);
	return new_widget;
}


//QWidget * FenetrePrincipale::BuildWidgetKinectNews(){ //ajout des widgets à le fenetre news
//
//	QWidget fenetre;
//
//	QHBoxLayout * layoutHorizontal = new QHBoxLayout();
//	int nbPas = 1;
//	int i;
//	double val1 = 10.5;
//	double val2 = 40.5;
//	double val3 = 10;
//	double val4 = 39;
//	double values[4] = {
//		val1, val2, val3, val4
//	};
//	//Création des instances MaGroupBox
//	for (i = 0; i < nbPas; i++){
//		MonLayoutVertical * vLayout = new MonLayoutVertical();
//		vLayout->addlabel("Premiere information");
//		vLayout->addlabel2("Deuxieme information");
//		vLayout->addvbox();
//		vLayout->addgroupbox();
//		vLayout->addprogressbar(values);
//		vLayout->addbutton();
//		vLayout->addvbox2();
//		vLayout->addgroupbox2();
//
//		//placer layout
//
//		layoutHorizontal->addLayout(vLayout);
//	}
//	fenetre.setLayout(layoutHorizontal);
//	fenetre.show();
//
//
//	
//	QWidget * widget = new QWidget;
//	widget->setLayout(layoutHorizontal);
//	return widget;
//
//}

void FenetrePrincipale::build_actions() {

    QAction * action;
    action = new QAction(tr("Hip center"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Spine"), this);			vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Shoulder center"), this);	vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Head"), this);				vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Shoulder left"), this);	vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Elbow left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Wrist left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Hand left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Shoulder right"), this);	vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Elbow right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Wrist right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Hand right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Hip left"), this);			vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Knee left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Ankle left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Foot left"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Hip right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Knee right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Ankle right"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Foot right"), this);		vec_action_draw_skeleton.push_back(action);

    action = new QAction(tr("Draw all"), this);			vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Draw nothing"), this);		vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Draw top"), this);			vec_action_draw_skeleton.push_back(action);
    action = new QAction(tr("Draw bottom"), this);		vec_action_draw_skeleton.push_back(action);

    for (uint i = 0; i < vec_action_draw_skeleton.size(); i++) {
        vec_action_draw_skeleton.at(i)->setCheckable(true);
        vec_action_draw_skeleton.at(i)->setChecked(true);
    }
    vec_action_draw_skeleton.at(21)->setChecked(false); // draw nothing

    connect(vec_action_draw_skeleton.at(0), SIGNAL(triggered()), this, SLOT(slot_action_draw_hip_center()));
    connect(vec_action_draw_skeleton.at(1), SIGNAL(triggered()), this, SLOT(slot_action_draw_spine()));
    connect(vec_action_draw_skeleton.at(2), SIGNAL(triggered()), this, SLOT(slot_action_draw_shoulder_center()));
    connect(vec_action_draw_skeleton.at(3), SIGNAL(triggered()), this, SLOT(slot_action_draw_head()));
    connect(vec_action_draw_skeleton.at(4), SIGNAL(triggered()), this, SLOT(slot_action_draw_shoulder_left()));
    connect(vec_action_draw_skeleton.at(5), SIGNAL(triggered()), this, SLOT(slot_action_draw_elbow_left()));
    connect(vec_action_draw_skeleton.at(6), SIGNAL(triggered()), this, SLOT(slot_action_draw_wrist_left()));
    connect(vec_action_draw_skeleton.at(7), SIGNAL(triggered()), this, SLOT(slot_action_draw_hand_left()));
    connect(vec_action_draw_skeleton.at(8), SIGNAL(triggered()), this, SLOT(slot_action_draw_shoulder_right()));
    connect(vec_action_draw_skeleton.at(9), SIGNAL(triggered()), this, SLOT(slot_action_draw_elbow_right()));
    connect(vec_action_draw_skeleton.at(10),SIGNAL(triggered()), this, SLOT(slot_action_draw_wrist_right()));
    connect(vec_action_draw_skeleton.at(11),SIGNAL(triggered()), this, SLOT(slot_action_draw_hand_right()));
    connect(vec_action_draw_skeleton.at(12),SIGNAL(triggered()), this, SLOT(slot_action_draw_hip_left()));
    connect(vec_action_draw_skeleton.at(13),SIGNAL(triggered()), this, SLOT(slot_action_draw_knee_left()));
    connect(vec_action_draw_skeleton.at(14),SIGNAL(triggered()), this, SLOT(slot_action_draw_ankle_left()));
    connect(vec_action_draw_skeleton.at(15),SIGNAL(triggered()), this, SLOT(slot_action_draw_foot_left()));
    connect(vec_action_draw_skeleton.at(16),SIGNAL(triggered()), this, SLOT(slot_action_draw_hip_right()));
    connect(vec_action_draw_skeleton.at(17),SIGNAL(triggered()), this, SLOT(slot_action_draw_knee_right()));
    connect(vec_action_draw_skeleton.at(18),SIGNAL(triggered()), this, SLOT(slot_action_draw_ankle_right()));
    connect(vec_action_draw_skeleton.at(19),SIGNAL(triggered()), this, SLOT(slot_action_draw_foot_right()));

    connect(vec_action_draw_skeleton.at(20),SIGNAL(triggered()), this, SLOT(slot_action_draw_all()));
    connect(vec_action_draw_skeleton.at(21),SIGNAL(triggered()), this, SLOT(slot_action_draw_nothing()));
    connect(vec_action_draw_skeleton.at(22),SIGNAL(triggered()), this, SLOT(slot_action_draw_top()));
    connect(vec_action_draw_skeleton.at(23),SIGNAL(triggered()), this, SLOT(slot_action_draw_bottom()));

    /*
    action_draw_all
    action_draw_nothing
    action_draw_spine
    action_draw_shoulder_center
    action_draw_head
    action_draw_shoulder_left
    action_draw_elbow_left
    action_draw_wrist_left
    action_draw_hand_left
    action_draw_shoulder_right
    action_draw_elbow_right
    action_draw_wrist_right
    action_draw_hand_right
    action_draw_hip_left
    action_draw_knee_left
    action_draw_ankle_left
    action_draw_foot_left
    action_draw_hip_right
    action_draw_knee_right
    action_draw_ankle_right
    action_draw_foot_right
    */
}


void FenetrePrincipale::slot_record() {
    if (but_record->isChecked()) {
        but_savedata->setText(tr("Save*"));
        but_record->setText(tr("Acq : on"));
        but_savedata2->setText(tr("Save*"));
        but_record2->setText(tr("Acq : on"));
        but_record->setChecked(true);
        but_record2->setChecked(true);
        QString msg = "Acquiring skeleton data";
        status->showMessage(msg);
        scene->record_data(true);
        scene2->record_data(true);
    } else {
        but_record->setText(tr("Acq : off"));
        but_record2->setText(tr("Acq : off"));
        but_record->setChecked(false);
        but_record2->setChecked(false);
        QString msg = "Stop acquisition of skeleton data";
        status->showMessage(msg);
        scene->record_data(false);
        scene2->record_data(false);
        but_savedata->setEnabled(true);
        but_savedata2->setEnabled(true);

        /* recuperation des donnees */
        std::vector<std::vector<Vector4>> & skeletons = scene->getSavedSkeletons();
        std::vector<std::vector<Joint>> & skeletons2 = scene2->getSavedSkeletons2();
        if (skeletons.size() > 0 || skeletons2.size() > 0) {
            skeletondata = new SkeletonData(skeletons , scene->getSavedTimes());
            skeletondata2 = new SkeletonData2(skeletons2 , scene2->getSavedTimes());
            widgetskeleton->setDataSkeleton(skeletondata->getSkeletonData(), skeletondata->getSkeletonTimes());
            widgetskeleton->setSliderProperties(slider_skeleton);
            tabwidget->setTabEnabled(2, true); // activation de la visu
            QString msg = "Data loaded";
            status->showMessage(msg);
        }
        else {
            QString msg = "No skeletons data have been recorded - please start a new acquisition";
            status->showMessage(msg);
        }
    }
}

void FenetrePrincipale::slot_record2() {
    if (but_record2->isChecked()) {
        but_savedata->setText(tr("Save*"));
        but_record->setText(tr("Acq : on"));
        but_savedata2->setText(tr("Save*"));
        but_record2->setText(tr("Acq : on"));
        but_record->setChecked(true);
        but_record2->setChecked(true);
        QString msg = "Acquiring skeleton data";
        status->showMessage(msg);
        scene->record_data(true);
        scene2->record_data(true);
    } else {
        but_record->setText(tr("Acq : off"));
        but_record2->setText(tr("Acq : off"));
        but_record->setChecked(false);
        but_record2->setChecked(false);
        QString msg = "Stop acquisition of skeleton data";
        status->showMessage(msg);
        scene->record_data(false);
        scene2->record_data(false);
        but_savedata->setEnabled(true);
        but_savedata2->setEnabled(true);

        /* recuperation des donnees */
        std::vector<std::vector<Vector4>> & skeletons = scene->getSavedSkeletons();
        std::vector<std::vector<Joint>> & skeletons2 = scene2->getSavedSkeletons2();
        if (skeletons.size() > 0 || skeletons2.size() > 0) {
            skeletondata = new SkeletonData(skeletons , scene->getSavedTimes());
            skeletondata2 = new SkeletonData2(skeletons2 , scene2->getSavedTimes());
            widgetskeleton->setDataSkeleton(skeletondata2->getSkeletonData(), skeletondata2->getSkeletonTimes());
            widgetskeleton->setSliderProperties(slider_skeleton);
            tabwidget->setTabEnabled(2, true); // activation de la visu
            QString msg = "Data loaded";
            status->showMessage(msg);
        }
        else {
            QString msg = "No skeletons data have been recorded - please start a new acquisition";
            status->showMessage(msg);
        }
    }
}



void FenetrePrincipale::slot_savedata() {
    scene->write_recorded_skeleton_data();
    scene2->write_recorded_skeleton_data();
    //skeletondata->savedata(); // pas de sauvegarde de l'image
    but_savedata->setText(tr("Save"));
    but_savedata2->setText(tr("Save"));
    QString msg = "Data saved";
    status->showMessage(msg);
}

void FenetrePrincipale::slot_loaddata() 
{
	vector<Pas> vec_pas;
	int nb_pas;

    QString filename = QFileDialog::getOpenFileName(this, tr("Open data file"), QString(), tr("Skeleton data (*.yml)"));
    if(QFile(filename).exists()) {
        //if (skeletondata == NULL) skeletondata = new SkeletonData();
		skeletondata = new SkeletonData();
		widgetAnalyse->reset();
        skeletondata->loaddata(filename);
        widgetskeleton->setDataSkeleton(skeletondata->getSkeletonData(), skeletondata->getSkeletonTimes());
		widgetAnalyse->setDataSkeleton(skeletondata->getSkeletonData(), skeletondata->getSkeletonTimes());
		slot_analyse();
		vec_pas = widgetAnalyse->getVecPas();
        widgetskeleton->setSliderProperties(slider_skeleton);
		widget_analyse2 = UpdateWidgetAnalyse(vec_pas, widgetAnalyse);
		this->tabwidget->removeTab(3);
		this->tabwidget->addTab(widget_analyse2, tr("Analyse"));

        tabwidget->setTabEnabled(2, true); // activation de la visu
		tabwidget->setTabEnabled(3, true); // activation de la visu
        QString msg = "Data loaded";
        status->showMessage(msg);
    }
}


void FenetrePrincipale::slot_incKinectAngle() {
    scene->changeKinectAngle(5);
}

void FenetrePrincipale::slot_decKinectAngle() {
    scene->changeKinectAngle(-5);
}

void FenetrePrincipale::slot_changeKinectAngle(int value) {
    float newanglek = value;
    float oldanglek = scene->getKinectAngle();
    //qDebug() << "newanglek : " << newanglek << "oldanglek :" << oldanglek;
    int inc = (((newanglek-oldanglek) > 0) ? 2 : -2);
    scene->changeKinectAngle(inc);
}

void FenetrePrincipale::slot_disp_axesXY(){
    widgetskeleton->setAxes(0);
    widgetskeleton->repaint();
}
void FenetrePrincipale::slot_disp_axesXZ(){
    widgetskeleton->setAxes(1);
    widgetskeleton->repaint();
}
void FenetrePrincipale::slot_disp_axesYZ(){
    widgetskeleton->setAxes(2);
    widgetskeleton->repaint();
}

void FenetrePrincipale::slot_disp_slider_time(int v){
    QString msg = "Time value : " + QString::number(v) + " milliseconds";
    status->showMessage(msg);
    widgetskeleton->setIdxSkeleton(v);
    widgetskeleton->repaint();
}


/**********************
* ACTIONS ************
**********************/

void FenetrePrincipale::slot_action_draw_all(){
    for (uint i = 0; i < vec_action_draw_skeleton.size(); i++) vec_action_draw_skeleton.at(i)->setChecked(true);
    vec_action_draw_skeleton.at(21)->setChecked(false); // draw nothing

    for (int i = 0; i < skeletondata->getNbSkeletonPoints(); i++) draw_skeletoncurves(i);

}
void FenetrePrincipale::slot_action_draw_nothing(){
    for (uint i = 0; i < vec_action_draw_skeleton.size(); i++) vec_action_draw_skeleton.at(i)->setChecked(false);
    vec_action_draw_skeleton.at(21)->setChecked(true); // draw nothing

    for (int i = 0; i < skeletondata->getNbSkeletonPoints(); i++) draw_skeletoncurves(i);
}
void FenetrePrincipale::slot_action_draw_top(){
    for (uint i = 0; i < vec_action_draw_skeleton.size(); i++) vec_action_draw_skeleton.at(i)->setChecked(false);
    vec_action_draw_skeleton.at(22)->setChecked(true);
    for (int i = 0; i < 12; i++) vec_action_draw_skeleton.at(i)->setChecked(true);

    for (int i = 0; i < skeletondata->getNbSkeletonPoints(); i++) draw_skeletoncurves(i);
}
void FenetrePrincipale::slot_action_draw_bottom(){
    for (uint i = 0; i < vec_action_draw_skeleton.size(); i++) vec_action_draw_skeleton.at(i)->setChecked(false);
    vec_action_draw_skeleton.at(23)->setChecked(true);
    for (int i = 12; i < 20; i++) vec_action_draw_skeleton.at(i)->setChecked(true);

    for (int i = 0; i < skeletondata->getNbSkeletonPoints(); i++) draw_skeletoncurves(i);
}

void FenetrePrincipale::draw_skeletoncurves(int idx) {
    widgetskeleton->setdrawskeletonpoint(idx, vec_action_draw_skeleton.at(idx)->isChecked());
    widgetskeleton->repaint();
}

void FenetrePrincipale::slot_action_draw_hip_center(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HIP_CENTER);
}
void FenetrePrincipale::slot_action_draw_spine(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_SPINE);
}
void FenetrePrincipale::slot_action_draw_shoulder_center(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_SHOULDER_CENTER);
}
void FenetrePrincipale::slot_action_draw_head(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HEAD);
}
void FenetrePrincipale::slot_action_draw_shoulder_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_SHOULDER_LEFT);
}
void FenetrePrincipale::slot_action_draw_elbow_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_ELBOW_LEFT);
}
void FenetrePrincipale::slot_action_draw_wrist_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_WRIST_LEFT);
}
void FenetrePrincipale::slot_action_draw_hand_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HAND_LEFT);
}
void FenetrePrincipale::slot_action_draw_shoulder_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_SHOULDER_RIGHT);
}
void FenetrePrincipale::slot_action_draw_elbow_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_ELBOW_RIGHT);
}
void FenetrePrincipale::slot_action_draw_wrist_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_WRIST_RIGHT);
}
void FenetrePrincipale::slot_action_draw_hand_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HAND_RIGHT);
}
void FenetrePrincipale::slot_action_draw_hip_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HIP_LEFT);
}
void FenetrePrincipale::slot_action_draw_knee_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_KNEE_LEFT);
}
void FenetrePrincipale::slot_action_draw_ankle_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_ANKLE_LEFT);
}
void FenetrePrincipale::slot_action_draw_foot_left(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_FOOT_LEFT);
}
void FenetrePrincipale::slot_action_draw_hip_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_HIP_RIGHT);
}
void FenetrePrincipale::slot_action_draw_knee_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_KNEE_RIGHT);
}
void FenetrePrincipale::slot_action_draw_ankle_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_ANKLE_RIGHT);
}
void FenetrePrincipale::slot_action_draw_foot_right(){
    draw_skeletoncurves(NUI_SKELETON_POSITION_FOOT_RIGHT);
}

void FenetrePrincipale::slot_analyse()
{
	widgetAnalyse->longueurPas();
	widgetAnalyse->demiPas();
	widgetAnalyse->derivee();
	widgetAnalyse->phases();
	widgetAnalyse->calculCycles();
}