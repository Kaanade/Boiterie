#ifndef SKELETONDATA2_H
#define SKELETONDATA2_H

#include <vector>
#include <kinect.h>
#include <QVector>
#include <time.h>
#include <fstream>
//using namespace std;

class SkeletonData2 {
public:
    SkeletonData2(std::vector<std::vector<Joint>> vec_skel, std::vector<double> vec_t) : vec_skeletons(vec_skel) {
        // verifier que la copie se passe bien !!!!
        vec_times = vec_times.fromStdVector(vec_t);
    }
    SkeletonData2() {
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
                    fichier << vec_skeletons.at(i).at(j).Position.X << ";" << vec_skeletons.at(i).at(j).Position.Y << ";" << vec_skeletons.at(i).at(j).Position.Z << "#";
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
                    Joint vec;
                    std::vector<Joint> askeleton;
                    for (int i = 1; i < list.size(); i++) {
                        QString data = list.at(i);
                        QStringList listpt = data.split(";");
                        if (listpt.size() == 3) { // 3 coordonnees par point
                            vec.Position.X = listpt.at(0).toFloat();
                            vec.Position.Y = listpt.at(1).toFloat();
                            vec.Position.Z = listpt.at(2).toFloat();
                            askeleton.push_back(vec);
                        }
                    }
                    if (askeleton.size() > 0) vec_skeletons.push_back(askeleton);
                }
            }
            nbligne++;
        }
    }

    std::vector<std::vector<Joint>> & getSkeletonData(){return vec_skeletons;}
    std::vector<double> getSkeletonTimes(){
        std::vector<double> vec_t = vec_times.toStdVector();
        return vec_t;
    }

    int getNbSkeletonPoints(){ return vec_skeletons.at(0).size(); }

    static QColor getColor(int i) {
        switch(i) {
        case JointType_SpineBase:
            return QColor(0, 0, 0);
        case JointType_SpineMid:
            return QColor(255, 200, 200);
        case JointType_Neck:
            return QColor(255, 200, 100);
        case JointType_Head:
            return QColor(255, 100, 200);
        case JointType_ShoulderLeft:
            return QColor(0, 255, 0);
        case JointType_ElbowLeft:
            return QColor(0, 200, 0);
        case JointType_WristLeft:
            return QColor(0, 150, 0);
        case JointType_HandLeft:
            return QColor(0, 100, 0);
        case JointType_ShoulderRight:
            return QColor(0, 0, 255);
        case JointType_ElbowRight:
            return QColor(0, 0, 200);
        case JointType_WristRight:
            return QColor(0, 0, 150);
        case JointType_HandRight:
            return QColor(0, 0, 100);
        case JointType_HipLeft:
            return QColor(255, 255, 0);
        case JointType_KneeLeft:
            return QColor(255, 200, 0);
        case JointType_AnkleLeft:
            return QColor(255, 150, 0);
        case JointType_FootLeft:
            return QColor(255, 100, 0);
        case JointType_HipRight:
            return QColor(0, 255, 255);
        case JointType_KneeRight:
            return QColor(0, 255, 200);
        case JointType_AnkleRight:
            return QColor(0, 255, 150);
        case JointType_FootRight:
            return QColor(0, 255, 100);
        default:
            return Qt::black;
        }
    }


private:
    std::vector<std::vector<Joint>> vec_skeletons;
    QVector<double> vec_times;

};

#endif // SkeletonData2

