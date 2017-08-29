#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H
#include <QObject>
#include <QTimer>
#include "cvmattoqimage.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#define GETFRAMEINTERVAL 10

class CameraDevice : public QObject
{
    Q_OBJECT
public:
    explicit CameraDevice(QObject *parent);
    ~CameraDevice();
public:
    void setCameraDeviceNumber( int number );
    void setResolution( int width , int height );
    void stop();
    void start();

private:
    cv::VideoCapture  *capture;
    cv::Mat           frame;
    QTimer            *cameraTimer;

    int cameraDeviceNumber;
    int ResolutionWidth;
    int ResolutionHeight;

signals:
    void readyImage( QImage &image );

private slots:
    void slotGetImageFrame();


};

#endif // CAMERADEVICE_H
