#include "MyKinect.h"

MyKinect::MyKinect() {

    is_initialized = false;
    is_recording = false;

    this->webcam = cv::Mat(cv::Size(640, 480), CV_8UC4);

    m_hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    m_hNextVideoFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    m_hNextSkeletonEvent   = CreateEvent( NULL, TRUE, FALSE, NULL );

    DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON;

    HRESULT hr = NuiInitialize(nuiFlags);
    if ( E_NUI_SKELETAL_ENGINE_BUSY == hr )
    {
        nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH |  NUI_INITIALIZE_FLAG_USES_COLOR;
        hr = NuiInitialize( nuiFlags) ;
    }
    if (FAILED(hr)) {
        qDebug("Kinect initialization failed 1.");
        return;
    }

    hr = NuiImageStreamOpen(
                NUI_IMAGE_TYPE_COLOR,
                NUI_IMAGE_RESOLUTION_640x480,
                0,
                NUM_FRAMES,
                m_hNextVideoFrameEvent,
                &m_pVideoStreamHandle);
    if (FAILED(hr)) {
        qDebug("Unable to open video stream.");
        return;
    }

    hr = NuiImageStreamOpen(
                NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
                NUI_IMAGE_RESOLUTION_320x240,
                0,
                NUM_FRAMES,
                m_hNextDepthFrameEvent,
                &m_pDepthStreamHandle );
    if (FAILED(hr)) {
        qDebug("Unable to open depth stream.");
        return;
    }

    hr = NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0 );
    if (FAILED(hr)) {
        qDebug("Unable to open skeleton tracking engine.");
        return;
    }

    is_initialized = true;

    QObject::connect(this, SIGNAL(signal_changedepthframe()), this, SLOT(slot_changedepthframe()));
    QObject::connect(this, SIGNAL(signal_changecolorframe()), this, SLOT(slot_changecolorframe()));
    QObject::connect(this, SIGNAL(signal_changeskelpos()), this, SLOT(slot_changeskelpos()));

    this->start();

    this->timestart = 0.0;

}

MyKinect::~MyKinect() {

    this->stop();
    NuiShutdown();
    if( m_hNextDepthFrameEvent && ( m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE ) ) {
        CloseHandle( m_hNextDepthFrameEvent );
        m_hNextDepthFrameEvent = NULL;
    }
    if( m_hNextVideoFrameEvent && ( m_hNextVideoFrameEvent != INVALID_HANDLE_VALUE ) ) {
        CloseHandle( m_hNextVideoFrameEvent );
        m_hNextVideoFrameEvent = NULL;
    }
    qDebug("Kinect destroyed.");
}


void MyKinect::changeAngle(int a) {
    if (is_initialized) {
        LONG angle = 0;
        HRESULT hr = NuiCameraElevationGetAngle(&angle);
        angle+=a;
        if (angle > NUI_CAMERA_ELEVATION_MINIMUM && angle < NUI_CAMERA_ELEVATION_MAXIMUM)
            hr = NuiCameraElevationSetAngle(angle);
    }
}

void MyKinect::stop() {
    is_running = false;
}

void MyKinect::run() {
    is_running = true;
    int eventIdx;
    HANDLE hEvents[3] = {m_hNextDepthFrameEvent, m_hNextVideoFrameEvent, m_hNextSkeletonEvent};

    while (is_running) {
        eventIdx=WaitForMultipleObjects(sizeof(hEvents)/sizeof(hEvents[0]),hEvents,FALSE,100);
        switch(eventIdx) {
        case 0:
            emit signal_changedepthframe();
            break;
        case 1:
            emit signal_changecolorframe();
            break;
        case 2:
            emit signal_changeskelpos();
            break;
        }
        usleep(15);
    }
}

void MyKinect::slot_changecolorframe() {
    const NUI_IMAGE_FRAME * pImageFrame = NULL;
    HRESULT hr = NuiImageStreamGetNextFrame(
                m_pVideoStreamHandle,
                0,
                &pImageFrame );
    if(FAILED(hr)) {
        return;
    }
    INuiFrameTexture * pTexture = pImageFrame->pFrameTexture;
    NUI_LOCKED_RECT LockedRect;
    pTexture->LockRect( 0, &LockedRect, NULL, 0 );
    if( LockedRect.Pitch != 0 ) {
        webcam.data = (BYTE*) LockedRect.pBits;/*
        cvSetData(this->ipl,(BYTE*)p_videoBuffer, ipl->widthStep);
        webcam = ipl;*/
    } else {
        qDebug("Buffer length of received texture is bogus" );
    }

    NuiImageStreamReleaseFrame( m_pVideoStreamHandle, pImageFrame );
}

void MyKinect::slot_changedepthframe() {
    const NUI_IMAGE_FRAME * pImageFrame = NULL;
    HRESULT hr = NuiImageStreamGetNextFrame(
                m_pDepthStreamHandle,
                0,
                &pImageFrame );
    if( FAILED(hr))
        return;
    NuiImageStreamReleaseFrame( m_pDepthStreamHandle, pImageFrame );
}

void MyKinect::slot_changeskelpos() {
    NUI_SKELETON_FRAME SkeletonFrame = {0};
    bool foundSkeleton = false;
    if ( SUCCEEDED(NuiSkeletonGetNextFrame( 0, &SkeletonFrame )) ) {
        for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ ) {
            NUI_SKELETON_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[i].eTrackingState;
            if ( trackingState == NUI_SKELETON_TRACKED || trackingState == NUI_SKELETON_POSITION_ONLY ) {
                foundSkeleton = true;
            }
        }
    }
    // no skeletons!
    if(!foundSkeleton) return;

    // smooth out the skeleton data
    const NUI_TRANSFORM_SMOOTH_PARAMETERS params = {0.5f, 0.5f, 0.5f, 0.05f, 0.05f};
    HRESULT hr = NuiTransformSmooth(&SkeletonFrame, &params);
    if ( FAILED(hr) ) {
        return;
    }

    // draw each skeleton color according to the slot within they are found.
    vec_skeletons.clear();
    for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ ) {
        NUI_SKELETON_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[i].eTrackingState;
        if ( trackingState == NUI_SKELETON_TRACKED ) {
            cv::vector<Vector4> askeleton;
            for (int j = 0; j < 20; j++) {
                askeleton.push_back(SkeletonFrame.SkeletonData[i].SkeletonPositions[j]);
                //qDebug() << SkeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[j];
            }
            //qDebug() << "--------";
            vec_skeletons.push_back(askeleton);
            if (is_recording && askeleton.size() > 0) {
                vec_saved_skeletons.push_back(askeleton);
                clock_t milliseconds = (clock() - this->startingClock) / (CLOCKS_PER_SEC / 1000);
                vec_saved_times.push_back(milliseconds);
            }
        }
        else if ( trackingState == NUI_SKELETON_POSITION_ONLY )
        {
            // we've only received the center point of the skeleton, draw that
        }


    }
}

cv::vector<cv::vector<Vector4>> & MyKinect::getSkeletons() {
    return this->vec_skeletons;
}
cv::vector<cv::vector<Vector4>> & MyKinect::getSavedSkeletons() {
    return this->vec_saved_skeletons;
}
cv::vector<double> & MyKinect::getSavedTimes() {
    return this->vec_saved_times;
}

void MyKinect::getCoordinatesForSkeletonPoint(Vector4 &vec, LONG * x,  LONG  * y) {
    LONG depthX, depthY;
    USHORT depth;
    NuiTransformSkeletonToDepthImage(vec, &depthX, &depthY, &depth, NUI_IMAGE_RESOLUTION_320x240);
    NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_320x240, NULL, depthX, depthY, depth, x, y);
}

void MyKinect::startRecord() {
    vec_saved_skeletons.clear();
    vec_saved_times.clear();
    this->is_recording = true;
    startingClock = clock();
}

void MyKinect::stopRecord(){
    this->is_recording = false;
}

float MyKinect::getAngle(){
    LONG angle;
    NuiCameraElevationGetAngle(&angle);
    return angle;
}
