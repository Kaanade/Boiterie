#include "mykinect2.h"

MyKinect2::MyKinect2() :
    m_pKinectSensor(NULL),
    m_pColorFrameReader(NULL)
{
    this->is_recording = false;
    this->webcam = cv::Mat(cv::Size(cColorWidth, cColorHeight), CV_8UC4);
    m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

    InitializeDefaultSensor();

    QObject::connect(this, SIGNAL(signal_changecolorframe()), this, SLOT(Update()));

    this->start();
}

MyKinect2::~MyKinect2()
{
    // done with color frame reader
    SafeRelease(m_pColorFrameReader);

    // close the Kinect Sensor
    if (m_pKinectSensor)
    {
        m_pKinectSensor->Close();
    }

    if (m_pColorRGBX)
    {
        delete [] m_pColorRGBX;
        m_pColorRGBX = NULL;
    }

    SafeRelease(m_pKinectSensor);
}

void MyKinect2::run() {
    is_running = true;
    while (is_running) {
        emit signal_changecolorframe();
        usleep(15);
    }
}

HRESULT MyKinect2::InitializeDefaultSensor()
{
    HRESULT hr;

    hr = GetDefaultKinectSensor(&m_pKinectSensor);
    if (FAILED(hr))
    {
        return hr;
    }

    if (m_pKinectSensor)
    {
        // Initialize the Kinect and get the color reader
        IColorFrameSource* pColorFrameSource = NULL;
        IBodyFrameSource* pBodyFrameSource = NULL;

        hr = m_pKinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
        }

        SafeRelease(pBodyFrameSource);
        SafeRelease(pColorFrameSource);
    }

    if (!m_pKinectSensor || FAILED(hr))
    {
        qDebug("Kinect2 initialization failed 1.");
        return E_FAIL;
    }

    return hr;
}

void MyKinect2::Update()
{
    // récupération de l'image en 2D
    if (!m_pColorFrameReader)
    {
        return;
    }

    IColorFrame* pColorFrame = NULL;

    HRESULT hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;
        IFrameDescription* pFrameDescription = NULL;
        int nWidth = 0;
        int nHeight = 0;
        ColorImageFormat imageFormat = ColorImageFormat_None;
        UINT nBufferSize = 0;


        hr = pColorFrame->get_RelativeTime(&nTime);

        if (SUCCEEDED(hr))
        {
            hr = pColorFrame->get_FrameDescription(&pFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Width(&nWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Height(&nHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
        }


        if (SUCCEEDED(hr) && (nWidth == cColorWidth) && (nHeight == cColorHeight))
        {
            if (imageFormat == ColorImageFormat_Bgra)
            {
                hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&webcam.data));
            }
            else if (m_pColorRGBX)
            {
                nBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
                hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(webcam.data), ColorImageFormat_Bgra);
            }
            else
            {
                hr = E_FAIL;
            }
        }
        SafeRelease(pFrameDescription);
    }

    // récupération du squelette
    if (!m_pBodyFrameReader)
    {
        return;
    }

    IBodyFrame* pBodyFrame = NULL;

    hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;

        hr = pBodyFrame->get_RelativeTime(&nTime);

        IBody* ppBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
        }

        if (SUCCEEDED(hr))
        {
            ProcessBody(BODY_COUNT, ppBodies); // BODY_COUNT est un define de Kinect.h égal à 6... peut etre mettre 1 plus tard afin déviter des problemes lors de la récupération des positions de joints
        }

        for (int i = 0; i < _countof(ppBodies); ++i)
        {
            SafeRelease(ppBodies[i]);
        }
    }

    SafeRelease(pBodyFrame);
    SafeRelease(pColorFrame);
}

void MyKinect2::ProcessBody(int nBodyCount, IBody** ppBodies)
{

    if (m_pCoordinateMapper)
    {
        for (int i = 0; i < nBodyCount; ++i)
        {
            IBody* pBody = ppBodies[i];
            if (pBody)
            {
                BOOLEAN bTracked = false;
                HRESULT hr = pBody->get_IsTracked(&bTracked);

                if (SUCCEEDED(hr) && bTracked)
                {
                    Joint joints[JointType_Count];

                    cv::vector<Joint> askeleton;

                    hr = pBody->GetJoints(_countof(joints), joints);
                    if (SUCCEEDED(hr))
                    {
                        vec_skeletons.clear();
                        for(uint j = 0; j < 20; j++)    // la kinect 2 prend 25 points (JointType_Count) mais on en prend 20 ici pour
                            // ne pas avoir à modifier le reste du code. 4 points supplémentaires correspondent au doigts donc ne sont pas ici necessaires. Le dernier point restant étant le centre des épaules. On ne l'a pas prit ici pour avoir exactement la meme chose qu'avec la kinect 1
                        {
                            askeleton.push_back(joints[j]);
                        }
                        vec_skeletons.push_back(askeleton);
                        if (is_recording && askeleton.size() > 0) {
                            vec_saved_skeletons.push_back(askeleton);
                            clock_t milliseconds = (clock() - this->startingClock) / (CLOCKS_PER_SEC / 1000);
                            vec_saved_times.push_back(milliseconds);
                        }
                        emit signal_changeskelpos();
                    }
                }
            }
        }
    }
}

void MyKinect2::getCoordinatesForSkeletonPoint(Joint &vec, LONG * x,  LONG  * y) {
    CameraSpacePoint bodyPoint = vec.Position;

    ColorSpacePoint colorPoint = {0};
    m_pCoordinateMapper->MapCameraPointToColorSpace(bodyPoint, &colorPoint);
    *x=colorPoint.X;
    *y=colorPoint.Y;
}

void MyKinect2::startRecord() {
    vec_saved_skeletons.clear();
    vec_saved_times.clear();
    this->is_recording = true;
    startingClock = clock();
}
