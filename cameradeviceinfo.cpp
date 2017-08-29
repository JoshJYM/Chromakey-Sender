#include "cameradeviceinfo.h"

CameraDeviceInfo::CameraDeviceInfo(QObject *parent) :
    QObject(parent),
    camera(0)
{}

/**
 * @brief CameraDeviceInfo::~CameraDeviceInfo
 */
CameraDeviceInfo::~CameraDeviceInfo()
{
    delete camera;
    qDebug("delete CameraDeviceInfo...");
}

/**
 * @brief CameraDeviceInfo::getCameraDevinfo
 * @return
 */
QStringList CameraDeviceInfo::getCameraDevinfo()
{
    cameraDevNameLists.clear();

    foreach(const QByteArray &deviceName, QCamera::availableDevices())
    {
        QString description = camera->deviceDescription(deviceName);
        cameraDevNameLists.append(description);
    }

    return cameraDevNameLists;
}


