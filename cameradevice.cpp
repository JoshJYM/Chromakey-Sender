#include "cameradevice.h"
#include <QElapsedTimer>

CameraDevice::CameraDevice(QObject *parent)
{
    cameraTimer = new QTimer();
    capture     = new cv::VideoCapture();

    cameraTimer->setTimerType(Qt::PreciseTimer);

    connect(cameraTimer,SIGNAL(timeout()),this,SLOT(slotGetImageFrame()));
}

/**
 * @brief CameraDevice::~CameraDevice
 */
CameraDevice::~CameraDevice()
{
    delete capture;
    delete cameraTimer;
    qDebug("delete CameraDevice...");
}

/**
 * @brief CameraDevice::setCameraDeviceNumber
 * @param number
 */
void CameraDevice::setCameraDeviceNumber(int number)
{
    this->cameraDeviceNumber = number;
}

/**
 * @brief CameraDevice::setResolution
 * @param width
 * @param height
 */
void CameraDevice::setResolution(int width, int height)
{
    this->ResolutionWidth  = width;
    this->ResolutionHeight = height;
}

/**
 * @brief CameraDevice::start
 */
void CameraDevice::start()
{
    capture->open(cameraDeviceNumber);

    capture->set(CV_CAP_PROP_FRAME_WIDTH,ResolutionWidth);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT,ResolutionHeight);
    cameraTimer->start(GETFRAMEINTERVAL);

    qDebug("Info : set <%d> Camera  <%d> width <%d> height!",cameraDeviceNumber,
           ResolutionWidth,ResolutionHeight);
}

/**
 * @brief CameraDevice::stop
 */
void CameraDevice::stop()
{
    if(capture->isOpened())
        capture->release();

    cameraTimer->stop();
}

/**
 * @brief CameraDevice::slotGetImageFrame
 */
void CameraDevice::slotGetImageFrame()
{
    QElapsedTimer t;
    t.start();
    if(!capture->isOpened())
    {
        qDebug(" Error : capure is not open!");
        return;
    }

    *capture >> frame;
    if(frame.cols)
        emit readyImage(QtOcv::mat2Image(frame));
}


