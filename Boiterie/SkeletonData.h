#ifndef SKELETONDATA_H
#define SKELETONDATA_H

#include <vector>
#include <NuiApi.h>
#include <QVector>
#include <time.h>
#include <fstream>
//using namespace std;

class SkeletonData {
public:
    SkeletonData(std::vector<std::vector<Vector4>> vec_skel, std::vector<double> vec_t) : vec_skeletons(vec_skel) {
        // verifier que la copie se passe bien !!!!
        vec_times = vec_times.fromStdVector(vec_t);
    }
	SkeletonData() {
	}


	void savedata() {
		if (vec_skeletons.size() <= 0) return;
		if (vec_skeletons.size() != vec_times.size()) return;
		char filename[64];
		time_t rawtime; 
		time(&rawtime);
		struct tm *current = localtime(&rawtime);
		sprintf(filename,"log_skeleton_%04d%02d%02d_%02d%02d%02d.yml", current->tm_year+1900, current->tm_mon+1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
		std::ofstream fichier(filename, std::ios::out | std::ios::trunc);
		if (fichier) {
			fichier << vec_skeletons.size() << std::endl;
			fichier << "No data for kinect angle" << std::endl;
            for (int i =0 ; i < vec_skeletons.size(); i++) {
				fichier << vec_times.at(i) << "#";
                for (int j = 0; j < vec_skeletons.at(i).size(); j++) {
					fichier << vec_skeletons.at(i).at(j).x << ";" << vec_skeletons.at(i).at(j).y << ";" << vec_skeletons.at(i).at(j).z << "#"; 
				}
				fichier << std::endl;
			}	
			fichier.close();
		}	
	}

	void loaddata(QString filename){
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
		QTextStream in(&file);
		int nbligne = 0;
		this->vec_skeletons.clear();
		this->vec_times.clear();
		while (!in.atEnd()) {
			QString line = in.readLine();
            if (nbligne == 0 || nbligne == 1) { // saut des 2 premieres lignes (taille du vecteur et angle)
			} else {
				QStringList list = line.split("#");
				if (list.size() > 1) { // on a une donnee
					vec_times.push_back(list.at(0).toDouble()); // remplissage de la donnée temps
					Vector4 vec;
                    std::vector<Vector4> askeleton;
					for (int i = 1; i < list.size(); i++) {
						QString data = list.at(i);
						QStringList listpt = data.split(";");
						if (listpt.size() == 3) { // 3 coordonnees par point
							vec.x = listpt.at(0).toFloat();
							vec.y = listpt.at(1).toFloat();
							vec.z = listpt.at(2).toFloat();
							askeleton.push_back(vec);
						}
					}
					if (askeleton.size() > 0) vec_skeletons.push_back(askeleton);
				}
			}
			nbligne++;
		}
	}

    std::vector<std::vector<Vector4>> & getSkeletonData(){return vec_skeletons;}
    std::vector<double> getSkeletonTimes(){
        std::vector<double> vec_t = vec_times.toStdVector();
		return vec_t;
	}
	int getNbSkeletonPoints(){ return vec_skeletons.at(0).size(); }

	static QColor getColor(int i) {
		switch(i) {
		case NUI_SKELETON_POSITION_HIP_CENTER:
			return QColor(0, 0, 0);
		case NUI_SKELETON_POSITION_SPINE:
			return QColor(255, 200, 200);
		case NUI_SKELETON_POSITION_SHOULDER_CENTER:
			return QColor(255, 200, 100);
		case NUI_SKELETON_POSITION_HEAD:
			return QColor(255, 100, 200);
		case NUI_SKELETON_POSITION_SHOULDER_LEFT:
			return QColor(0, 255, 0);
		case NUI_SKELETON_POSITION_ELBOW_LEFT:
			return QColor(0, 200, 0);
		case NUI_SKELETON_POSITION_WRIST_LEFT:
			return QColor(0, 150, 0);
		case NUI_SKELETON_POSITION_HAND_LEFT:
			return QColor(0, 100, 0);
		case NUI_SKELETON_POSITION_SHOULDER_RIGHT:
			return QColor(0, 0, 255);
		case NUI_SKELETON_POSITION_ELBOW_RIGHT:
			return QColor(0, 0, 200);
		case NUI_SKELETON_POSITION_WRIST_RIGHT:
			return QColor(0, 0, 150);
		case NUI_SKELETON_POSITION_HAND_RIGHT:
			return QColor(0, 0, 100);
		case NUI_SKELETON_POSITION_HIP_LEFT:
			return QColor(255, 255, 0);
		case NUI_SKELETON_POSITION_KNEE_LEFT:
			return QColor(255, 200, 0);
		case NUI_SKELETON_POSITION_ANKLE_LEFT:
			return QColor(255, 150, 0);
		case NUI_SKELETON_POSITION_FOOT_LEFT:
			return QColor(255, 100, 0);
		case NUI_SKELETON_POSITION_HIP_RIGHT:
			return QColor(0, 255, 255);
		case NUI_SKELETON_POSITION_KNEE_RIGHT:
			return QColor(0, 255, 200);
		case NUI_SKELETON_POSITION_ANKLE_RIGHT:
			return QColor(0, 255, 150);
		case NUI_SKELETON_POSITION_FOOT_RIGHT:
			return QColor(0, 255, 100);
		default:
			return Qt::black;
		}
	}


private:
    std::vector<std::vector<Vector4>> vec_skeletons;
	QVector<double> vec_times;
	
};

#endif
