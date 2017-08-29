#ifndef CAMERADEVICEINFO_H
#define CAMERADEVICEINFO_H
#include <QObject>
#include <QCamera>

class CameraDeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit CameraDeviceInfo(QObject *parent = 0);
    ~CameraDeviceInfo();

    QStringList getCameraDevinfo();

private:
    QCamera         *camera;
    QStringList     cameraDevNameLists;
};

#endif // CAMERADEVICEINFO_H
