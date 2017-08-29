#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QObject>
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include "sendaudiosocket.h"
#include "UdpPacketType.h"

/*audio define value*/
#define AUDIO_SAMPLERATE    8000
#define AUDIO_CHANNELCOUNT  1
#define AUDIO_SAMPLSIZE     16
#define AUDIO_CODEC         "audio/pcm"
#define AUDIO_BUFFERSIZE    14096

class AudioDevice : public QObject
{
    Q_OBJECT
public:
    explicit AudioDevice(QObject *parent);
    ~AudioDevice();

    void initAudioInfo();
    void setAudioInput(int DevNumber);
    void stopAudio();
    void setDestInfo(QString destAddress[], int serverCnt);

private:
    QList<QAudioDeviceInfo> devices;
    QAudioDeviceInfo    mInputDevice;
    QAudioFormat        mFormat;
    QAudioInput         *mAudioInput;
    QIODevice           *mInput;
    QByteArray          mBuffer;
    SendAudioSocket     *audioSocket;

    bool                isSend;

    void initNetwork();

private slots:
    void slotReadAudioData();
    void slotSendAudioData();
    void slotStopSendAudioData();
    void slotGetAudioVolume(int vol);

signals:
    void sendAudiorowData(QByteArray data);
};

#endif // AUDIODEVICE_H
