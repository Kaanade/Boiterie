#include "WidgetSkeleton.h";

WidgetSkeleton::WidgetSkeleton(QWidget *parent) : QWidget(parent) {


	rx = ry = rz = 1.0;
	minX = minY = minZ = maxX = maxY = maxZ = 0.0;
	axes = 0;
	idx_skeleton = 0;
	this->ttotal = 0;


	std::vector<std::vector<Vector4>> vec_skeletons;
	std::vector<std::vector<Joint>> vec_skeletons2;
	std::vector<double> vec_times;
	float rx, ry, rz, minX, minY, minZ, maxX, maxY, maxZ;
	int axes; // 0 pour XY, 1 pour XZ, 2 pour YZ
	int idx_skeleton;
	int kinectDevice;
	float ttotal;
	std::vector<boolean> vec_draw_skeletonpoint;

}

void WidgetSkeleton::setDataSkeleton(vector<vector<Vector4>> & vec_s, vector<double> & vec_t) {
		kinectDevice = KINECT1;
		vec_skeletons = vec_s;
		vec_times = vec_t;
		// pour l'affichage des donnees, on a besoin de calculer les min-max et les ratios
		lookExtremas(&minX, &minY, &minZ, &maxX, &maxY, &maxZ);
		rx = (maxX - minX);
		ry = (maxY - minY);
		rz = (maxZ - minZ);
		// dessin des points du squelette
		for (uint i = 0; i < vec_skeletons.at(0).size(); i++) vec_draw_skeletonpoint.push_back(true);
	}

void WidgetSkeleton::setDataSkeleton(vector<vector<Joint>> & vec_s, vector<double> & vec_t) {
		kinectDevice = KINECT2;
		vec_skeletons2 = vec_s;
		vec_times = vec_t;
		// pour l'affichage des donnees, on a besoin de calculer les min-max et les ratios
		lookExtremas(&minX, &minY, &minZ, &maxX, &maxY, &maxZ);
		rx = (maxX - minX);
		ry = (maxY - minY);
		rz = (maxZ - minZ);
		// dessin des points du squelette
		for (uint i = 0; i < vec_skeletons2.at(0).size(); i++) vec_draw_skeletonpoint.push_back(true);
	}

void WidgetSkeleton::setIdxSkeleton(int i){
		if (kinectDevice == KINECT1)
		{
			idx_skeleton = i * vec_skeletons.size() / ttotal;
			if (idx_skeleton >= vec_skeletons.size()) idx_skeleton = vec_skeletons.size() - 1; // effet de bord au cas ou
		}
		else
		{
			idx_skeleton = i * vec_skeletons2.size() / ttotal;
			if (idx_skeleton >= vec_skeletons2.size()) idx_skeleton = vec_skeletons2.size() - 1; // effet de bord au cas ou
		}
	}

void WidgetSkeleton::paintEvent(QPaintEvent *event) {
		QPainter painter(this);
		QPen pen;
		pen.setColor(Qt::black);
		pen.setWidth(5);
		painter.drawLine(1, 0, 1, height());
		painter.drawLine(1, height() - 1, width(), height() - 1);

		paintSkeletonData(idx_skeleton);
		paintSkeletonTimesData();
	}

void WidgetSkeleton::setAxes(int i) {
		if (i > -1 && i < 3) this->axes = i;
	}



void WidgetSkeleton::lookExtremas(float * minX, float * minY, float * minZ, float * maxX, float *maxY, float * maxZ) {
		float miX, miY, miZ, maX, maY, maZ;
		miX = miY = miZ = 10000;
		maX = maY = maZ = -10000;
		if (kinectDevice == KINECT1)
		{
			for (uint i = 0; i < vec_skeletons.size(); i++) {
				std::vector<Vector4> & askeleton = vec_skeletons.at(i);
				for (uint j = 0; j < askeleton.size(); j++) {
					Vector4 vec = askeleton.at(j);
					if (vec.x < miX) miX = vec.x;
					if (vec.y < miY) miY = vec.y;
					if (vec.z < miZ) miZ = vec.z;
					if (vec.x > maX) maX = vec.x;
					if (vec.y > maY) maY = vec.y;
					if (vec.z > maZ) maZ = vec.z;
				}
			}
		}
		else
		{
			for (uint i = 0; i < vec_skeletons2.size(); i++) {
				std::vector<Joint> & askeleton = vec_skeletons2.at(i);
				for (uint j = 0; j < askeleton.size(); j++) {
					Joint vec = askeleton.at(j);
					if (vec.Position.X < miX) miX = vec.Position.X;
					if (vec.Position.Y < miY) miY = vec.Position.Y;
					if (vec.Position.Z < miZ) miZ = vec.Position.Z;
					if (vec.Position.X > maX) maX = vec.Position.X;
					if (vec.Position.Y > maY) maY = vec.Position.Y;
					if (vec.Position.Z > maZ) maZ = vec.Position.Z;
				}
			}
		}
		*minX = miX; *minY = miY; *minZ = miZ;
		*maxX = maX; *maxY = maY; *maxZ = maZ;
	}

void WidgetSkeleton::paintSkeletonData(int idx) {
		QPainter painter(this);
		QPen pen;
		pen.setColor(Qt::black);
		pen.setWidth(2.0);
		painter.setPen(pen);
		std::vector<Vector4> & skel = vec_skeletons.at(idx);



		painter.drawLine(getSkeletonPoint(skel.at(3), axes), getSkeletonPoint(skel.at(2), axes));
		painter.drawLine(getSkeletonPoint(skel.at(2), axes), getSkeletonPoint(skel.at(4), axes));
		painter.drawLine(getSkeletonPoint(skel.at(4), axes), getSkeletonPoint(skel.at(5), axes));
		painter.drawLine(getSkeletonPoint(skel.at(5), axes), getSkeletonPoint(skel.at(6), axes));
		painter.drawLine(getSkeletonPoint(skel.at(6), axes), getSkeletonPoint(skel.at(7), axes));
		painter.drawLine(getSkeletonPoint(skel.at(2), axes), getSkeletonPoint(skel.at(8), axes));
		painter.drawLine(getSkeletonPoint(skel.at(8), axes), getSkeletonPoint(skel.at(9), axes));
		painter.drawLine(getSkeletonPoint(skel.at(9), axes), getSkeletonPoint(skel.at(10), axes));
		painter.drawLine(getSkeletonPoint(skel.at(10), axes), getSkeletonPoint(skel.at(11), axes));
		painter.drawLine(getSkeletonPoint(skel.at(2), axes), getSkeletonPoint(skel.at(1), axes));
		//painter.drawLine(getSkeletonPoint(skel.at(2), axes), getSkeletonPoint(skel.at(0), axes));
		painter.drawLine(getSkeletonPoint(skel.at(0), axes), getSkeletonPoint(skel.at(12), axes));
		painter.drawLine(getSkeletonPoint(skel.at(12), axes), getSkeletonPoint(skel.at(13), axes));
		painter.drawLine(getSkeletonPoint(skel.at(13), axes), getSkeletonPoint(skel.at(14), axes));
		painter.drawLine(getSkeletonPoint(skel.at(14), axes), getSkeletonPoint(skel.at(15), axes));
		painter.drawLine(getSkeletonPoint(skel.at(0), axes), getSkeletonPoint(skel.at(16), axes));
		painter.drawLine(getSkeletonPoint(skel.at(16), axes), getSkeletonPoint(skel.at(17), axes));
		painter.drawLine(getSkeletonPoint(skel.at(17), axes), getSkeletonPoint(skel.at(18), axes));
		painter.drawLine(getSkeletonPoint(skel.at(18), axes), getSkeletonPoint(skel.at(19), axes));

		for (int i = 0; i < 20; i++) painter.drawEllipse(getSkeletonPoint(skel.at(i), axes), 5, 5);


	}

void WidgetSkeleton::paintSkeletonTimesData() {
		QPainter painter(this);
		QPen pen;
		if (vec_skeletons.size() < 1) return;
		for (uint i = 1; i < vec_skeletons.size(); i++) {
			std::vector<Vector4> & prevskel = vec_skeletons.at(i - 1);
			std::vector<Vector4> & curskel = vec_skeletons.at(i);
			for (uint j = 0; j < curskel.size(); j++) {
				pen.setColor(SkeletonData::getColor(j));
				painter.setPen(pen);
				if (axes == 0 && vec_draw_skeletonpoint.at(j))
					painter.drawLine(convX(prevskel.at(j).x), convY(prevskel.at(j).y), convX(curskel.at(j).x), convY(curskel.at(j).y));
				if (axes == 1 && vec_draw_skeletonpoint.at(j))
					painter.drawLine(convZ(prevskel.at(j).z), convX(prevskel.at(j).x), convZ(curskel.at(j).z), convX(curskel.at(j).x));
				if (axes == 2 && vec_draw_skeletonpoint.at(j))
					painter.drawLine(convZ(prevskel.at(j).z), convY(prevskel.at(j).y), convZ(curskel.at(j).z), convY(curskel.at(j).y));
			}

		}
	}

QPoint WidgetSkeleton::getSkeletonPoint(Vector4 & vec, int axe) {
		float x = convX(vec.x);
		float y = convY(vec.y);
		float z = convZ(vec.z);
		QPoint pt(0, 0);
		if (axe == 0) pt = QPoint(x, y);
		if (axe == 1) pt = QPoint(z, x);
		if (axe == 2) pt = QPoint(z, y);
		return pt;
	}


	float WidgetSkeleton::convX(float x) {
		if (axes == 1) return height() - ((x - minX) * height() / rx); else return (x - minX) * width() / rx;
	}

	float WidgetSkeleton::convY(float y) {
		//if (axes == 0) return (y - minY) * width() / ry; else return height() - ((y - minY) * height() / ry);
		return height() - ((y - minY) * height() / ry);
	}

	float WidgetSkeleton::convZ(float z) {
		return (z - minZ) * width() / rz;
	}

	void WidgetSkeleton::setSliderProperties(QSlider * slider) {
		if (vec_times.size() < 1) return;
		this->ttotal = (vec_times.at(vec_times.size() - 1) - vec_times.at(0)); // le temps est en millisecondes
		slider->setMinimum(0);
		slider->setMaximum(ttotal);
	}

	void WidgetSkeleton::setdrawskeletonpoint(int idx, boolean b) {
		if (idx >= 0 && idx < vec_draw_skeletonpoint.size()) vec_draw_skeletonpoint.at(idx) = b;
	}
