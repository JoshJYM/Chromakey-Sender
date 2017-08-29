#include "audiodevice.h"


AudioDevice::AudioDevice( QObject *parent = 0) :
    mInputDevice(QAudioDeviceInfo::defaultInputDevice()),
    mAudioInput(0),
    mInput(0),
    mBuffer(AUDIO_BUFFERSIZE, 0)
{
    initAudioInfo();
    initNetwork();
}

/**
 * @brief AudioDevice::~AudioDevice
 */
AudioDevice::~AudioDevice()
{
    delete audioSocket;
    delete mAudioInput;
    delete mInput;
    qDebug("delete AudioDevice...");
}


/**
 * @brief AudioDevice::initNetwork
 */
void AudioDevice::initNetwork()
{
    audioSocket = new SendAudioSocket(this);
    isSend = false;
}

void AudioDevice::setDestInfo(QString destAddress[], int serverCnt)
{
    audioSocket->setDestInfo(UDP_AUDIO_PORT, destAddress, serverCnt);
}

/**
 * @brief AudioDevice::initAudioInfo
 */
void AudioDevice::initAudioInfo()
{
    devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    mFormat.setSampleRate(AUDIO_SAMPLERATE);
    mFormat.setChannelCount(AUDIO_CHANNELCOUNT);
    mFormat.setSampleSize(AUDIO_SAMPLSIZE);
    mFormat.setSampleType(QAudioFormat::SignedInt);
    mFormat.setByteOrder(QAudioFormat::LittleEndian);
    mFormat.setCodec(AUDIO_CODEC);

    QAudioDeviceInfo infoInDev(QAudioDeviceInfo::defaultInputDevice());
    if(!infoInDev.isFormatSupported(mFormat))
        mFormat = infoInDev.nearestFormat(mFormat);


    QAudioDeviceInfo infoOutDev(QAudioDeviceInfo::defaultOutputDevice());
    if(!infoOutDev.isFormatSupported(mFormat))
        mFormat = infoOutDev.nearestFormat(mFormat);


    if(mInput != 0)
    {
        disconnect(mInput, 0, this, 0);
        mInput = 0;
    }
}

/**
 * @brief AudioDevice::setAudioInput
 * @param DevNumber
 */
void AudioDevice::setAudioInput(int DevNumber)
{
    mInputDevice = devices.at(DevNumber);
    mAudioInput = new QAudioInput(mInputDevice, mFormat, this);

    mAudioInput->setVolume(qreal(100) / 100);

    mInput = mAudioInput->start();
    connect(mInput, SIGNAL(readyRead()), this, SLOT(slotReadAudioData()));
}

/**
 * @brief AudioDevice::stopAudio
 */
void AudioDevice::stopAudio()
{
    mAudioInput->stop();
}

/**
 * @brief AudioDevice::slotGetAudioVolume
 * @param vol
 */
void AudioDevice::slotGetAudioVolume(int vol)
{
     mAudioInput->setVolume(qreal(vol) / 100);
}

/**
 * @brief AudioDevice::slotReadAudioData
 */
void AudioDevice::slotReadAudioData()
{
    if(!mAudioInput)
        return;

    qint64 len = mAudioInput->bytesReady();

    mInput->read(mBuffer.data(), len);

    if(isSend)
    {
        mBuffer.push_front('\0');
        mBuffer.push_front(UDP_HEADER);
        audioSocket->sendAudioData(mBuffer.left(len+sizeof(UDP_HEADER)));
    }
    else
    {
       emit sendAudiorowData(mBuffer.left(len));
    }
}

/**
 * @brief AudioDevice::slotSendAudioData
 */
void AudioDevice::slotSendAudioData()
{
    isSend = true;
}

/**
 * @brief AudioDevice::slotStopSendAudioData
 */
void AudioDevice::slotStopSendAudioData()
{
    isSend = false;
}
