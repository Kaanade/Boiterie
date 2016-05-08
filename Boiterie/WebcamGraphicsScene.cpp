#include "WebcamGraphicsScene.h"

WebcamGraphicsScene::WebcamGraphicsScene(int numdevice, int w, int h) {

    framesize.width = w; framesize.height = h;
    this->numdevice = numdevice;
    this->setSceneRect(0,0,framesize.width,framesize.height);
    m_tx = m_ty = m_tx_old = m_ty_old = 0.0;

    if (numdevice == KINECT_DEVICE) {
        this->mykinect = new MyKinect();
        framerate_cam = 15;
        m_time = new QTimer(this);
        m_time->start(framerate_cam);
        m_distance = 1.0;
        connect(mykinect, SIGNAL(signal_changecolorframe()), this, SLOT(update()));
        connect(mykinect, SIGNAL(signal_changeskelpos()), this, SLOT(slot_drawskeletons()));

    }else if(numdevice == KINECT_DEVICE2) {
        this->mykinect2 = new MyKinect2();
        framerate_cam = 15;
        m_time = new QTimer(this);
        m_time->start(framerate_cam);
        m_distance = 1.0;
        connect(mykinect2, SIGNAL(signal_changecolorframe()), this, SLOT(update()));
        connect(mykinect2, SIGNAL(signal_changeskelpos()), this, SLOT(slot_drawskeletons()));

    } else {

        videocap.open(numdevice);
        if (!videocap.isOpened()){
            QString msg = "La camera device " + QString::number(numdevice) + " n'existe pas";
            QMessageBox::critical(NULL, "Erreur", msg);
            return;
        }
        videocap.set(CV_CAP_PROP_FRAME_WIDTH, framesize.width); videocap.set(CV_CAP_PROP_FRAME_HEIGHT, framesize.height);

        framerate_cam = 15;//videocap.get(CV_CAP_PROP_FPS);
        m_time = new QTimer(this);
        connect(m_time, SIGNAL(timeout()), this, SLOT(update()));
        m_time->start(framerate_cam);
        m_distance = 1.0;
    }
}

WebcamGraphicsScene::WebcamGraphicsScene(QString filename, int w, int h) 
{
    framesize.width = w; framesize.height = h;
    this->numdevice = 0;
    this->setSceneRect(0,0,framesize.width,framesize.height);
    videocap.open(filename.toLatin1().data());
    if (!videocap.isOpened()){
        QString msg = "Le fichier vidéo " + filename + " est introuvable";
        QMessageBox::critical(NULL, "Erreur", msg);
        return;
    }
    framerate_cam = 15;
    m_time = new QTimer(this);
    connect(m_time, SIGNAL(timeout()), this, SLOT(update()));
    m_time->start(framerate_cam);
    m_distance = 1.0;
}

WebcamGraphicsScene::~WebcamGraphicsScene() {
    m_time->stop();
    if (numdevice != KINECT_DEVICE) videocap.release();
    if (numdevice == KINECT_DEVICE) delete mykinect;
}

void WebcamGraphicsScene::videopause(){
    m_time->stop();
}

void WebcamGraphicsScene::videostart(){
    m_time->start();
}

void WebcamGraphicsScene::resetView(){
    m_tx = m_ty = m_tx_old = m_ty_old = 0.0;
    m_distance = 1.0;
}

void WebcamGraphicsScene::deleteItems(){
    QList<QGraphicsItem *> list_items = this->items();
    QList<QGraphicsItem *>::Iterator it = list_items.begin();
    for ( ; it != list_items.end(); ++it )
    {
        if ( *it )
        {
            this->removeItem(*it);
            delete *it;
        }
    }
}

void WebcamGraphicsScene::deleteLastItem(){
    QList<QGraphicsItem *> list_items = this->items();
    if (list_items.size() > 0)
    {
        QGraphicsItem * gr = list_items.last();
        if ( gr )
        {
            this->removeItem(gr);
            delete gr;
        }
    }
}

void WebcamGraphicsScene::deleteRectItems(){
    QList<QGraphicsItem *> list_items = this->items();
    if (list_items.size() > 0)
    {
        for (int i = 0; i < list_items.size(); i++)
        {
            QGraphicsItem * gr = list_items.at(i);
            if ( gr->type() == QGraphicsRectItem::Type )
            {
                this->removeItem(gr);
                delete gr;
            }
        }
    }
}

void WebcamGraphicsScene::deleteLineItems(){
    QList<QGraphicsItem *> list_items = this->items();
    if (list_items.size() > 0)
    {
        for (int i = 0; i < list_items.size(); i++)
        {
            QGraphicsItem * gr = list_items.at(i);
            if ( gr->type() == QGraphicsLineItem::Type )
            {
                this->removeItem(gr);
                delete gr;
            }
        }
    }
}

void WebcamGraphicsScene::deleteTextItems(){
    QList<QGraphicsItem *> list_items = this->items();
    if (list_items.size() > 0)
    {
        for (int i = 0; i < list_items.size(); i++)
        {
            QGraphicsItem * gr = list_items.at(i);
            if ( gr->type() == QGraphicsTextItem::Type )
            {
                this->removeItem(gr);
                delete gr;
            }
        }
    }
}

void WebcamGraphicsScene::drawBackground(QPainter *painter, const QRectF &) {

    bool readvideo = false;
    if (numdevice != KINECT_DEVICE && numdevice != KINECT_DEVICE2) {
        readvideo = videocap.read(cvframe);
    }

    if (numdevice == KINECT_DEVICE) {
        if (m_time->isActive()) { // gestion de la pause avec le kinect
            cvframe = mykinect->getwebcam();
            readvideo = true;
        }
    }

    if (numdevice == KINECT_DEVICE2) {
        if (m_time->isActive()) { // gestion de la pause avec le kinect
            cvframe = mykinect2->getwebcam();
            readvideo = true;
        }
    }

    if (readvideo)
    {
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glScalef(m_distance, m_distance, 1.0);
        glTranslatef(m_tx, m_ty, 0.0);
        glOrtho(0, framesize.width, framesize.height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        if (numdevice == KINECT_DEVICE) glTexImage2D( GL_TEXTURE_2D, 0, 4, cvframe.cols, cvframe.rows, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, cvframe.data );
        else if (numdevice == KINECT_DEVICE2) glTexImage2D( GL_TEXTURE_2D, 0, 4, cvframe.cols, cvframe.rows, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, cvframe.data );
        else glTexImage2D( GL_TEXTURE_2D, 0, 4, cvframe.cols, cvframe.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, cvframe.data );
        glBegin(GL_QUADS);

        glTexCoord2f(0,0); glVertex2f(0,0);
        glTexCoord2f(0,1); glVertex2f(0,framesize.height);
        glTexCoord2f(1,1); glVertex2f(framesize.width,framesize.height);
        glTexCoord2f(1,0); glVertex2f(framesize.width,0);

        /*
        glTexCoord2f(0,0); glVertex2f(framesize.width,0);
        glTexCoord2f(0,1); glVertex2f(framesize.width,framesize.height);
        glTexCoord2f(1,1); glVertex2f(0,framesize.height);
        glTexCoord2f(1,0); glVertex2f(0,0);
        */
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glFlush();

        emit signal_imagecaptured(numdevice);
    }

}

void WebcamGraphicsScene::grabimage(QString filename) {
    imwrite(filename.toLatin1().data(), cvframe);
}

void WebcamGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->scenePos() - event->lastScenePos();
        event->accept();
        update();
    }
}

void WebcamGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_tx_old = event->screenPos().rx();
        m_ty_old = event->screenPos().ry();
    }

    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;
    event->accept();
}

void WebcamGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_tx = (event->screenPos().rx() - m_tx_old) / framesize.width;
        m_ty = -(event->screenPos().ry() - m_ty_old) / framesize.height;
        //QString s = "Mouse :  " + QString::number(m_tx) + "," + QString::number(m_ty) + " --- " + QString::number(event->screenPos().rx()) + "," + QString::number(event->screenPos().ry());
        //s += " old " +  QString::number(m_tx_old) + "," + QString::number(m_ty_old) + " --- " + QString::number(framesize.width) + "," + QString::number(framesize.height);
        //qDebug(s.toLatin1().data());
    }

    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;
    event->accept();
    update();
}

void WebcamGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;
    m_distance *= qPow(1.2, -event->delta() / 120);
    //QString s = "Wheel event : " + QString::number(m_distance) + " " + QString::number(m_tx) + "," + QString::number(m_ty);
    //qDebug(s.toLatin1().data());
    event->accept();
    update();
}



void WebcamGraphicsScene::draw_points(const std::vector<QPointF> & vqpoints, QColor col){
    unsigned int l;
    for (l = 0; l < vqpoints.size(); l++ ) {
        QPointF pt = vqpoints.at(l);
        QGraphicsEllipseItem * it = new QGraphicsEllipseItem(pt.x(), pt.y(), 3, 3, 0);
        QPen pen;
        pen.setColor(col);
        it->setPen(pen);
        this->addItem(it);
    }
}

void WebcamGraphicsScene::draw_point(const cv::Point2f & p, QColor col){
    QGraphicsEllipseItem * it = new QGraphicsEllipseItem(p.x, p.y, 3, 3, 0);
    QPen pen;
    pen.setColor(col);
    it->setPen(pen);
    this->addItem(it);
}


void WebcamGraphicsScene::draw_rect(const QRectF & r, QColor col){
    QPen pen;
    pen.setColor(col);
    QGraphicsRectItem * qgri = new QGraphicsRectItem(r, 0);
    qgri->setPen(pen);
    this->addItem(qgri);
}

void WebcamGraphicsScene::draw_rect(const cv::Rect & r, QColor col) {
    QPen pen;
    pen.setColor(col);
    QGraphicsRectItem * qgri = new QGraphicsRectItem(r.x, r.y, r.width, r.height, 0);
    qgri->setPen(pen);
    this->addItem(qgri);
}

void WebcamGraphicsScene::draw_rect(const std::vector<QRectF> & vr, QColor col) {
    unsigned int l;
    QPen pen;
    pen.setColor(col);
    for (l = 0; l < vr.size(); l++ ) {
        QRectF r = vr.at(l);
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r, 0);
        qgri->setPen(pen);
        // exemple pour faire du semi-transparent (0 = total transparent, 1 = pas transparent)
        qgri->setOpacity(0.7);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_rect(const std::vector<cv::Rect> & vr, QColor col) {
    unsigned int l;
    QPen pen;
    pen.setColor(col);
    for (l = 0; l < vr.size(); l++ ) {
        cv::Rect r = vr.at(l);
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r.x, r.y, r.width, r.height, 0);
        qgri->setPen(pen);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_filledrect(const std::vector<cv::Rect> & vr, const std::vector<QColor> & col) {
    unsigned int l;
    QPen pen;
    QBrush b;
    b.setStyle( Qt::SolidPattern);
    for (l = 0; l < vr.size(); l++ ) {
        cv::Rect r = vr.at(l);
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r.x, r.y, r.width, r.height, 0);
        pen.setColor(col.at(l));
        b.setColor(col.at(l));
        qgri->setPen(pen);
        qgri->setBrush(b);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_filledrect(const QRectF & r, QColor col) {
    QPen pen;
    QBrush b;
    b.setStyle( Qt::SolidPattern);
    QGraphicsRectItem * qgri = new QGraphicsRectItem(r.x(), r.y(), r.width(), r.height(), 0);
    pen.setColor(col);
    b.setColor(col);
    qgri->setPen(pen);
    qgri->setBrush(b);
    this->addItem(qgri);
}

void WebcamGraphicsScene::draw_rect(const std::vector<QRectF> & vr, const std::vector<QColor> & vcol){
    unsigned int l;
    QPen pen;
    for (l = 0; l < vr.size(); l++ ) {
        QRectF r = vr.at(l);
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r, 0);
        QColor col = vcol.at(l);
        pen.setColor(col);
        qgri->setPen(pen);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_rect(const std::vector<cv::Rect> & vr, const std::vector<QColor> & vcol){
    unsigned int l;
    QPen pen;
    for (l = 0; l < vr.size(); l++ ) {
        cv::Rect r = vr.at(l);
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r.x, r.y, r.width, r.height, 0);
        QColor col = vcol.at(l);
        pen.setColor(col);
        qgri->setPen(pen);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_rect(const std::vector<QPair<QRectF, QColor>> & vp){
    unsigned int l;
    QPen pen;
    for (l = 0; l < vp.size(); l++ ) {
        QRectF r = vp.at(l).first;
        QGraphicsRectItem * qgri = new QGraphicsRectItem(r, 0);
        QColor col = vp.at(l).second;
        pen.setColor(col);
        qgri->setPen(pen);
        this->addItem(qgri);
    }
}

void WebcamGraphicsScene::draw_contours(const std::vector<QPoint> & vp, QColor col, int step){
    int l;
    QPen pen;
    pen.setColor(col);
    int s = vp.size();
    if ((s - step) > 0) {
        for (l = 0; l < (s - step); l+=step ) {
            QPoint pt1 = vp.at(l);
            QPoint pt2 = vp.at(l+step);
            QGraphicsLineItem * qgli = new QGraphicsLineItem(pt1.x(), pt1.y(), pt2.x(), pt2.y(), 0);
            this->addItem(qgli);
            qgli->setPen(pen);
        }
        /* fermeture du contour */
        QPoint pt1 = vp.at(s - step - 1);
        QPoint pt2 = vp.at(0);
        QGraphicsLineItem * qgli = new QGraphicsLineItem(pt1.x(), pt1.y(), pt2.x(), pt2.y(), 0);
        this->addItem(qgli);
        qgli->setPen(pen);
    }
}

void WebcamGraphicsScene::draw_itemnumber(const std::vector<QRectF> & vec, QColor col){
    unsigned int l;
    for (l = 0; l < vec.size(); l++ ) {
        QRectF r = vec.at(l);
        QString buf = QString::number(l);
        QGraphicsTextItem * qgti = new QGraphicsTextItem(buf, 0);
        qgti->setDefaultTextColor(col);
        qgti->setPos(r.x(), r.y());
        this->addItem(qgti);
    }
}

void WebcamGraphicsScene::draw_itemnumber(const std::vector<QPair<QRectF, QColor>> & vec){
    unsigned int l;
    for (l = 0; l < vec.size(); l++ ) {
        QRectF r = vec.at(l).first;
        QString buf = QString::number(l);
        QGraphicsTextItem * qgti = new QGraphicsTextItem(buf, 0);
        qgti->setDefaultTextColor(vec.at(l).second);
        qgti->setPos(r.x(), r.y());
        this->addItem(qgti);
    }
}

void WebcamGraphicsScene::draw_polygon(const QPolygonF & poly, QColor col){
    QGraphicsPolygonItem * qgpi = new QGraphicsPolygonItem(poly, 0);
    QPen pen;
    pen.setColor(col);
    qgpi->setPen(pen);
    qgpi->setBrush(QBrush(col, Qt::DiagCrossPattern));
    this->addItem(qgpi);
}

void WebcamGraphicsScene::draw_line(const float x1, const float y1, const float x2, const float y2, QColor col) {
    QGraphicsLineItem * qgli = new QGraphicsLineItem(x1,y1,x2,y2,0);
    QPen pen;
    pen.setColor(col);
    qgli->setPen(pen);
    this->addItem(qgli);
}

void WebcamGraphicsScene::draw_lines(const std::vector<cv::Point> & vec, QColor col) {
    unsigned int l;
    if (vec.size() > 1 ) {
        cv::Point p1 = vec.at(0);
        cv::Point p2;
        QPen pen;
        pen.setColor(col);
        for (l = 1; l < vec.size(); l++ ) {
            p2 = vec.at(l);
            QGraphicsLineItem * qgli = new QGraphicsLineItem(p1.x,p1.y,p2.x,p2.y,0);
            qgli->setPen(pen);
            p1 = p2;
            this->addItem(qgli);
        }
    }
}

void WebcamGraphicsScene::draw_lines(const QQueue<cv::Point> & vec, QColor col) {
    int l;
    if (vec.size() > 1 ) {
        cv::Point p1 = vec.at(0);
        cv::Point p2;
        QPen pen;
        pen.setColor(col);
        for (l = 1; l < vec.size(); l++ ) {
            p2 = vec.at(l);
            QGraphicsLineItem * qgli = new QGraphicsLineItem(p1.x,p1.y,p2.x,p2.y,0);
            qgli->setPen(pen);
            p1 = p2;
            this->addItem(qgli);
        }
    }
}

void WebcamGraphicsScene::draw_text(const QString & text, QColor col, QPoint p) {
    QGraphicsTextItem * qgti = new QGraphicsTextItem(text, 0);
    qgti->setScale(3.0);
    qgti->setDefaultTextColor(col);
    qgti->setPos(p.x(), p.y());
    this->addItem(qgti);
}

void WebcamGraphicsScene::changeKinectAngle(float a) {
    mykinect->changeAngle(a);
}

void WebcamGraphicsScene::draw_kinectskeletons(QColor col) {
    if (numdevice == KINECT_DEVICE)
    {
        QPen pen;
        pen.setColor(col);
        QBrush brush;
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(col);

        if (mykinect->getSkeletons().size() > 0) {
            cv::vector<Vector4> & skeleton = mykinect->getSkeletons().at(0); // on prend le 1er en esperant qu'il n'y ait que un seul squelette de detecte !!!
            LONG x, y;
            const int rayon = 3;
            for (uint i = 0; i < skeleton.size(); i++) {
                Vector4 & vec = skeleton.at(i);
                mykinect->getCoordinatesForSkeletonPoint(vec, &x, &y);
                QGraphicsEllipseItem * it = new QGraphicsEllipseItem(x-rayon, y-rayon, rayon*2, rayon*2, 0);
                it->setPen(pen);
                it->setBrush(brush);
                this->addItem(it);
            }

            if (skeleton.size() == 20) {// squelette en entier
                LONG x1, y1, x2, y2;
                Vector4 v1, v2, vm;
                v1 = skeleton[NUI_SKELETON_POSITION_HEAD];
                v2 = skeleton[NUI_SKELETON_POSITION_SHOULDER_CENTER];
                vm = v2;
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_SHOULDER_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_ELBOW_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_WRIST_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_HAND_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = vm;
                v1 = skeleton[NUI_SKELETON_POSITION_SHOULDER_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                mykinect->getCoordinatesForSkeletonPoint(vm, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_ELBOW_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_WRIST_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_HAND_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = vm;
                v2 = skeleton[NUI_SKELETON_POSITION_SPINE];
                mykinect->getCoordinatesForSkeletonPoint(vm, &x1, &y1);
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_HIP_CENTER];
                vm = v1;
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_HIP_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_KNEE_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_ANKLE_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_FOOT_LEFT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = vm;
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                v2 = skeleton[NUI_SKELETON_POSITION_HIP_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_KNEE_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[NUI_SKELETON_POSITION_ANKLE_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[NUI_SKELETON_POSITION_FOOT_RIGHT];
                mykinect->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                this->addLine(x1, y1, x2, y2, pen);
            }
        }
    }
    else if(numdevice == KINECT_DEVICE2)
    {
        QPen pen;
        pen.setColor(col);
        QBrush brush;
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(col);

        if (mykinect2->getSkeletons().size() > 0) {
            cv::vector<Joint> & skeleton = mykinect2->getSkeletons().at(0); // on prend le 1er en esperant qu'il n'y ait que un seul squelette de detecte !!!
            LONG x, y;
            const int rayon = 3;
            for (uint i = 0; i < skeleton.size(); i++) {
                Joint & vec = skeleton.at(i);
                mykinect2->getCoordinatesForSkeletonPoint(vec, &x, &y);
                x = x * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y = y * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                QGraphicsEllipseItem * it = new QGraphicsEllipseItem(x-rayon, y-rayon, rayon*2, rayon*2, 0);
                it->setPen(pen);
                it->setBrush(brush);
                this->addItem(it);
            }

            if (skeleton.size() == 20) {// squelette en entier (sans les doigts dans ce cas...)
                LONG x1, y1, x2, y2;
                Joint v1, v2, vm;
                v1 = skeleton[JointType_Head];
                v2 = skeleton[JointType_Neck];
                vm = v2;
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_ShoulderLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_ElbowLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_WristLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_HandLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = vm;
                v1 = skeleton[JointType_ShoulderRight];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                mykinect2->getCoordinatesForSkeletonPoint(vm, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_ElbowRight];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_WristRight];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_HandRight];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = vm;
                v2 = skeleton[JointType_SpineMid];
                mykinect2->getCoordinatesForSkeletonPoint(vm, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_SpineBase];
                vm = v1;
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_HipLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_KneeLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_AnkleLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_FootLeft];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = vm;
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                v2 = skeleton[JointType_HipRight];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_KneeRight];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v2 = skeleton[JointType_AnkleRight];
                mykinect2->getCoordinatesForSkeletonPoint(v2, &x2, &y2);
                x2 = x2 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y2 = y2 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
                v1 = skeleton[JointType_FootRight];
                mykinect2->getCoordinatesForSkeletonPoint(v1, &x1, &y1);
                x1 = x1 * this->width() / mykinect2->getColorWidth();     // les coordonées sont fournie pour un écran de 1920 par 1080
                y1 = y1 * this->height() / mykinect2->getColorHeight();   // on les remet selon la taille de la scene.
                this->addLine(x1, y1, x2, y2, pen);
            }
        }
    }
    else
        return;
}

void WebcamGraphicsScene::slot_drawskeletons() {
    this->deleteItems();
    draw_kinectskeletons(Qt::red);


}

void  WebcamGraphicsScene::record_data(bool b) {
    if(numdevice == KINECT_DEVICE)
    {
        if (b)  mykinect->startRecord(); else {
            mykinect->stopRecord();
            saved_cvframe = cvframe.clone();
        }
    }
    else if(numdevice == KINECT_DEVICE2)
    {
        if (b)  mykinect2->startRecord(); else {
            mykinect2->stopRecord();
            saved_cvframe = cvframe.clone();
        }
    }
}

void  WebcamGraphicsScene::write_recorded_skeleton_data() {
    if (numdevice == KINECT_DEVICE)
    {
        cv::vector<cv::vector<Vector4>> vec = mykinect->getSavedSkeletons();
        if (vec.size() <= 0) return;
        cv::vector<double> & vect = mykinect->getSavedTimes();
        if (vec.size() != vect.size()) return;
        char filename[64];
        time_t rawtime;
        time(&rawtime);
        struct tm *current = localtime(&rawtime);
        sprintf(filename,"log_skeleton_%04d%02d%02d_%02d%02d%02d.yml", current->tm_year+1900, current->tm_mon+1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
        std::ofstream fichier(filename, std::ios::out | std::ios::trunc);
        if (fichier) {
            fichier << vec.size() << std::endl;
            fichier << mykinect->getAngle() << std::endl;
            for (uint i =0 ; i < vec.size(); i++) {
                fichier << vect.at(i) << "#";
                for (uint j = 0; j < vec.at(i).size(); j++) {
                    fichier << vec.at(i).at(j).x << ";" << vec.at(i).at(j).y << ";" << vec.at(i).at(j).z << "#";
                }
                fichier << std::endl;
            }
            fichier.close();
        }
        sprintf(filename,"log_skeleton_%04d%02d%02d_%02d%02d%02d.jpg", current->tm_year+1900, current->tm_mon+1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
        imwrite(filename, saved_cvframe);
    }
    else if (numdevice == KINECT_DEVICE2)
    {
        cv::vector<cv::vector<Joint>> vec = mykinect2->getSavedSkeletons();
        if (vec.size() <= 0) return;
        cv::vector<double> & vect = mykinect2->getSavedTimes();
        if (vec.size() != vect.size()) return;
        char filename[64];
        time_t rawtime;
        time(&rawtime);
        struct tm *current = localtime(&rawtime);
        sprintf(filename,"log_skeleton_v2_%04d%02d%02d_%02d%02d%02d.yml", current->tm_year+1900, current->tm_mon+1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
        std::ofstream fichier(filename, std::ios::out | std::ios::trunc);
        if (fichier) {
            fichier << vec.size() << std::endl;
            fichier << 0 << std::endl;
            for (uint i =0 ; i < vec.size(); i++) {
                fichier << vect.at(i) << "#";
                for (uint j = 0; j < vec.at(i).size(); j++) {
                    fichier << vec.at(i).at(j).Position.X << ";" << vec.at(i).at(j).Position.Y << ";" << vec.at(i).at(j).Position.Z << "#";
                }
                fichier << std::endl;
            }
            fichier.close();
        }
        sprintf(filename,"log_skeleton_v2_%04d%02d%02d_%02d%02d%02d.jpg", current->tm_year+1900, current->tm_mon+1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
        imwrite(filename, saved_cvframe);
    }
}

cv::vector<double> & WebcamGraphicsScene::getSavedTimes() {
    if(numdevice == KINECT_DEVICE)
        return mykinect->getSavedTimes();
    else
        return mykinect2->getSavedTimes();
}

float  WebcamGraphicsScene::getKinectAngle(){
    return mykinect->getAngle();
}
