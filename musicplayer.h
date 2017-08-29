#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QObject>
#include <QMediaPlayer>
#include <QAudioProbe>
#include "sendmusicdatasocket.h"
#include "UdpPacketType.h"

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent);
    ~MusicPlayer();

    void setDestInfo(QString destAddress[], int serverCnt);

private:
    QMediaPlayer    *musicPlayer;
    QAudioProbe     *musicProbe;
    QByteArray      *musicDataBuffer;

    SendMusicDataSocket  *musicSocket;

    void initMusicPlayer();
    void initNetwork();

private slots:
    void slotPlayMusic(QString fileName);
    void slotStopMusic();
    void slotGetMusicData(QAudioBuffer musicBuffer);
    void slotGetMusicPosition(qint64 position);
    void slotGetMusciCurrentDuration(qint64 duration);
    void slotMediaStateChaged(QMediaPlayer::MediaStatus state);
    void slotGetMusicVolume(int vol);

signals:
    void sendMusicMetaData(QString title, QString artist);
    void sendMusicInfo(int channelCnt, int bitRate, int sampleRate);
    void sendMusicCurrentTime(qint64 time);
    void sendMusicCurrentDuration(qint64 duration);
    void sendEndofMusic();
};

#endif // MUSICPLAYER_H
