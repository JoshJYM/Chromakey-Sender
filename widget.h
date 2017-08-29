#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QMenu>
#include <QKeyEvent>
#include <qmath.h>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "cameradevice.h"
#include "cameradeviceinfo.h"
#include "imageglview.h"
#include "imageencoding.h"
#include "audiodeviceinfo.h"
#include "sendimagesocket.h"
#include "settingwidget.h"
#include "morpologyprocessing.h"
#include "audiodevice.h"
#include "musicplayer.h"
#include "tcpconnection.h"
#include "TcpPacketType.h"
#include "UdpPacketType.h"
#include "qvideodecoder.h"
#include "voicepainter.h"
#include "senderchatting.h"
#include "MainWidgetDefineVal.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(TcpConnection *connection,QString id,
                    QString pw, QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget              *ui;
    TcpConnection           *tcpConnection;
    AudioDevice             *audioDev;
    AudioDevice             *bgmDev;
    SettingWidget            settingWidget;
    CameraDevice            *cameraDev;
    CameraDeviceInfo        *cameraDevInfo;
    ImageGLView             *mainViewWidget;
    QPixmap                 logoImage;
    QImage                  *targetImg;
    QImage                  *resultImg;
    QImage                  *tempTargetImg;
    QImage                  *sourceImg;
    QImage                  videoImg;
    QByteArray              encodData;
    ImageEncoding           *encoding;
    AudioDeviceInfo         *audioDevInfo;
    VoicePainter            *voicePainter;

    SendImageSocket         *imgSocket;
    QList<QAudioDeviceInfo> audioList;
    MorPologyProcessing     *morpoloyPro;

    QVideoDecoder           *videoCodec;
    QTimer                  videoTimer[VIDEOTIMER_NUMBER];

    QListWidgetItem         *memberlist;

    bool                    isVideoFile;

    MusicPlayer             *musicPlayer;
    QStringListModel        musicModel;
    QStringList             musicList;
    QHash<int, QString>     musicAddressList;
    int                     nowPlayNum;
    int                     musicListCount;

    SenderChatting          *chatting;
    QTimer                  reConnectTimer;
    QMenu                   *menu;

    QTimer                  *broadCastTimer;
    int                     broadCastTime;

    int                     broadCastState;
    bool                    isBroadCast;
    bool                    isMorpology;
    bool                    isShowSettingWidget;
    bool                    isPause;

    int clicked_x;
    int clicked_y;
    int windowWidth;
    int windowHeight;
    int clicked;
    bool isClick;

    int sScaleWidth;
    int sScaleHeight;
    int screenMode;

    int beforWSize;
    int beforHSize;
    int beforWidgetX;
    int beforWidgetY;

    bool isCameraActive;
    bool isAudioActive;
    bool isFullWidget;
    bool isFirstBroad;

    int audioDevIndex;
    int kickIndex;
    bool ischatConnection;

    QString id;
    QString pw;

    int fontColor;
    int animation;

    int musicTime;
    int musicTime_m;
    int musicTime_s;

    int currentMember;
    int roomLike;
    int roomBookmark;

signals:
    void sendAdverImg(QImage &image);
    void initAdverImg();
    void fullScreenView();
    void setDefaultCamra();
    void setDefaultAudio();
    void startDefaultSetBroadCast();
    void stopMusic();
    void playMusic(QString fileName);
    void startSendAudioData();
    void sendChatContext(QString context);
    void sendKickMemberNick(QString nick);
    void pressEnterKey();
    void sendCaptionInfo(int animation, int size, int color, QString context);
    void sendCaptionState(bool isCaption);
    void playNextMusic();
    void getBeforeBroadInfo(TcpConnection *connection, int msg);
    void getModyfiedInfo(TcpConnection *connection, int msg);
    void sendResultImage();
    void sendCaptionBold(bool state);
    void sendBroadcastState(bool state);

private:
    void initSettingWidget();
    void initMainImageView();
    void initCamera();
    void initVideoPlay();
    void initMainWidget();
    void initTimer();
    void initAudio();
    void initBtnConnect();
    void initImageSocket();
    void initImageprocessing();
    void initModeView();
    void initDefaultDevice();
    void initMusicPlayer();
    void initTcpSocket(TcpConnection *connection);
    void initTcpConnection();
    void initValAndStopDev();
    void initChatting();
    void initCaption();
    void initSettingTab();
    void initRoomInfo();
    void initchattingValue();
    void resetTimer();
    void pause(bool state);

    void setResolutionLists();
    void setCameraImageSize(int width, int height);
    void setQualityLists();
    void setScreenMode(bool isSmall,int mode);
    void setScaleSize(int w, int h);
    void setImageQuility(int quaility);
    void setBroadCastWidget(bool state);
    void setRoomInfo(QString name);
    void setKickMenuList();
    void setBroadCasting();
    void startBroadCasting();
    void activeTab(int tabNum);
    void requestBeforeRoomInfo();
    void getLogInInfo(QString id, QString pw);
    void activateAudio(int index);
    void stopVideoTimer(int timerNumber);
    void startVideoTimer(int timerNumber);
    void deleteMemory();
    void setDestStreamInfo(TcpConnection *_connection);

    QString chageStrTypr(QString str);
    QByteArray makePacket(QByteArray packet, QString name, QString data);
    QByteArray makePacket(QByteArray packet, QString data);
    int *getMusicName(QString musicFileName);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void slotStopAudio();
    void slotActivateCamera();
    void slotStopCamera();
    void slotCancelBroadCast();
    void slotStopBroadCast();
    void slotgetFrame(QImage &image);
    void slotStartBroadCast();
    void slotShowBroadCastSetting();
    void slotShowImageSettingWidget();
    void slotCancelMorpology();
    void slotApplyMorpology();
    void slotGetTargetImg(QImage &image);
    void slotGetTargetImg(QVideoDecoder &decoder);
    void slotGetAdverImg(QImage &image);
    void slotBroadCastTime();
    void slotChangeLeftUPMode(bool mode);
    void slotChangeLeftDOWNMode(bool mode);
    void slotChangeRightUPMode(bool mode);
    void slotChangeRightDOWNMode(bool mode);
    void slotChangeNormalMode(bool mode);
    void slotChageTransparency(int value);
    void slotCloseMainWidget();
    void slotHideMainWidget();
    void slotChangeBigWidget();
    void slotFullScreenView();
    void slotDefaultSetBroadCast();
    void slotPlayMusic();
    void slotPlayNextMusic();
    void slotPlayBeforMusic();
    void slotAddMusicFile();
    void slotStopMusic();
    void slotStartPlayMoive();
    void slotChangeAudioDev(int index);
    void slotChangeQualityVal(int index);
    void slotGetBroadCastInfo(TcpConnection* _connection, int msg);
    void slotGetAudioRowData(QByteArray data);
    void slotGetMusicMetaData(QString title, QString artist);
    void slotGetChatingContext(QString nick, QString context);
    void slotGetChatMember(QString nick, int idNum);
    void slotSendChat();
    void slotGetChatExitMeber(QString nick);
    void slotClickMemberList(QModelIndex mList);
    void slotKickMember(QAction* a);
    void slotSetCaptionColor();
    void slotSendCaptionInfo();
    void slotSendCaptionState();
    void slotClickDevBtn();
    void slotClickRoomInfoBtn();
    void slotClickChattingBtn();
    void slotGetCurrentMusicTime(qint64 time);
    void slotGetMusicDuration(qint64 duration);
    void slotClickLeftTORightBtn();
    void slotClickRightTOLeftBtn();
    void slotClickUpToDownBtn();
    void slotClickDownToUpBtn();
    void slotClickStopCaptionBtn();
    void slotGetBeforeBroadInfo(TcpConnection *connection, int msg);
    void slotGetModifiedInfo(TcpConnection *connection, int msg);
    void slotSendResultImage();
    void slotGetnoticeServerErr();
    void slotGetnoticeChatConSuccess();
    void slotClickMusicList(QModelIndex list);
    void slotSendAdverInfo(QString url, int startX, int startY,int width, int height);
    void slotSendMusicInfo(int channelCnt, int bitRate, int sampleRate);
    void slotGetCaptionBold();
    void slotPauseBroadcast();
    void slotRestartBroadcast();
    void slotReConnectChatServer();
};

#endif // WIDGET_H
