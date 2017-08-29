#include "musicplayer.h"
#include <QMediaMetaData>
#include <QImage>

MusicPlayer::MusicPlayer(QObject *parent)
{
    initMusicPlayer();
    initNetwork();
}

MusicPlayer::~MusicPlayer()
{
    qDebug("delete MusicPlayer...");
    delete musicPlayer;
    delete musicProbe;
    delete musicDataBuffer;
}

/**
 * @brief MusicPlayer::initMusicPlayer
 */
void MusicPlayer::initMusicPlayer()
{
    musicPlayer     = new QMediaPlayer();
    musicProbe      = new QAudioProbe();
    musicDataBuffer = new QByteArray();

    musicPlayer->setVolume(50);
    musicProbe->setSource(musicPlayer);

    connect(musicProbe,SIGNAL(audioBufferProbed(QAudioBuffer)),
            SLOT(slotGetMusicData(QAudioBuffer)));
    connect(musicPlayer,SIGNAL(positionChanged(qint64)),
            SLOT(slotGetMusicPosition(qint64)));
    connect(musicPlayer,SIGNAL(durationChanged(qint64)),
            SLOT(slotGetMusciCurrentDuration(qint64)));
    connect(musicPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(slotMediaStateChaged(QMediaPlayer::MediaStatus)));
}

/**
 * @brief MusicPlayer::initNetwork
 */
void MusicPlayer::initNetwork()
{
    musicSocket = new SendMusicDataSocket(this);
}

void MusicPlayer::setDestInfo(QString destAddress[], int serverCnt)
{
    musicSocket->setDestInfo(UDP_MUSIC_PORT, destAddress, serverCnt);
}

/**
 * @brief MusicPlayer::slotGetMusciCurrentDuration
 * @param duration
 */
void MusicPlayer::slotGetMusciCurrentDuration(qint64 duration)
{
    emit sendMusicCurrentDuration(duration);
}

void MusicPlayer::slotGetMusicVolume(int vol)
{
    musicPlayer->setVolume(vol);
}

/**
 * @brief MusicPlayer::slotGetMusicData
 * @param musicBuffer
 */
void MusicPlayer::slotGetMusicData(QAudioBuffer musicBuffer)
{
    /*QAudioBuffer to QByteArray*/
    *musicDataBuffer = musicDataBuffer->fromRawData((char *)musicBuffer.data()
                                                    ,musicBuffer.byteCount());
    /*Send Music Data to server*/
    musicDataBuffer->push_front('\0');
    musicDataBuffer->push_front(UDP_HEADER);
    musicSocket->sendMudicData(*musicDataBuffer);
    musicDataBuffer->clear();
}

void MusicPlayer::slotMediaStateChaged(QMediaPlayer::MediaStatus state)
{
    switch(state)
        {
        case QMediaPlayer::EndOfMedia:
            /*when musis is ended, this slot
             * will be called and send notice to
             * main widget*/
            emit sendEndofMusic();
            break;
        case QMediaPlayer::LoadedMedia:
            /*send music information to server*/
            int channelCnt = musicPlayer->metaData(QMediaMetaData::ChannelCount).toInt();
            int bitRate    = 16;
            int sampleRate = musicPlayer->metaData(QMediaMetaData::SampleRate).toInt();

            /*Get metaData of Music*/
            QString title  = musicPlayer->metaData(QMediaMetaData::AlbumTitle).toByteArray().data();
            QString artist = musicPlayer->metaData(QMediaMetaData::AlbumArtist).toByteArray().data();

            emit sendMusicMetaData(title,artist);
            emit sendMusicInfo(channelCnt, bitRate, sampleRate);

            qDebug()<<"title";
            qDebug()<<title;
            qDebug()<<"artist";
            qDebug()<<artist;

            qDebug()<<"channel count";
            qDebug()<<channelCnt;
            qDebug()<<"bitRate";
            qDebug()<<bitRate;
            qDebug()<<"sampleRate";
            qDebug()<<sampleRate;

            break;
        }
}

/**
 * @brief MusicPlayer::slotPlayMusic
 * @param fileName
 */
void MusicPlayer::slotPlayMusic(QString fileName)
{
    /*get music file directory and play music*/
    musicPlayer->setMedia(QUrl::fromLocalFile(fileName));
    musicPlayer->play();
}

/**
 * @brief MusicPlayer::slotGetMusicPosition
 * @param position
 */
void MusicPlayer::slotGetMusicPosition(qint64 position)
{
    emit sendMusicCurrentTime(position);
}

/**
 * @brief MusicPlayer::slotStopMusic
 */
void MusicPlayer::slotStopMusic()
{
    musicPlayer->stop();
}


