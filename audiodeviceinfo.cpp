#include "audiodeviceinfo.h"


AudioDeviceInfo::AudioDeviceInfo(QObject *parent)
{}

AudioDeviceInfo::~AudioDeviceInfo()
{
    qDebug("delete AudioDeviceInfo...");
}

/**
 * @brief AudioDeviceInfo::getAudioDevInfo
 * @return
 */
QList<QAudioDeviceInfo> AudioDeviceInfo::getAudioDevInfo()
{

    audioDevNameList.clear();
    audioDevNameList = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    return audioDevNameList;
}
