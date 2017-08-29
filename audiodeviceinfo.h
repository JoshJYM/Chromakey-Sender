#ifndef AUDIODEVICEINFO_H
#define AUDIODEVICEINFO_H
#include <QObject>
#include <QAudio>
#include <QAudioDeviceInfo>

class AudioDeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit AudioDeviceInfo(QObject *parent);
    ~AudioDeviceInfo();

    QList<QAudioDeviceInfo> getAudioDevInfo();

private:
    QList<QAudioDeviceInfo>      audioDevNameList;
    QAudioDeviceInfo             audioDev;
};

#endif // AUDIODEVICEINFO_H

