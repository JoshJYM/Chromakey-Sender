#include "widget.h"
#include "ui_widget.h"

/**
 * @brief Widget::Widget
 * @param connection
 * @param id
 * @param pw
 * @param parent
 */
Widget::Widget(TcpConnection *connection, QString id, QString pw, QWidget *parent) :
    QWidget(0, Qt::FramelessWindowHint),
    ui(new Ui::Widget)
{
    Q_UNUSED(parent)

    ui->setupUi(this);
    logoImage.load(":/res/images/Logo.png");

    initModeView();
    initMainImageView();
    initMainWidget();
    initCamera();
    initAudio();
    initImageSocket();
    initImageprocessing();
    initBtnConnect();
    initSettingWidget();
    initTimer();
    initDefaultDevice();
    initMusicPlayer();
    initTcpSocket(connection);
    getLogInInfo(id,pw);
    initVideoPlay();
    initTcpConnection();
    initCaption();
    initSettingTab();
    initRoomInfo();
    initChatting();
    initchattingValue();
}

/**
 * @brief Widget::~Widget
 */
Widget::~Widget()
{
    qDebug("delete Widget...");

    if(isCameraActive)
        cameraDev->stop();

    if(isAudioActive)
        audioDev->stopAudio();

    /*exit main server*/
    if(tcpConnection)
        tcpConnection->disconnectFromHost();

    /*exit chatting server*/
    if(chatting)
        delete chatting;

    /*if music is playing, must stop it*/
    emit stopMusic();

    deleteMemory();
}

/*init Function*/
/**
 * @brief Widget::initMoviePlay
 */
void Widget::initVideoPlay()
{
    videoCodec   = new QVideoDecoder();
    isVideoFile  = false;
}

/**
 * @brief Widget::initTcpSocket
 * @param connection
 */
void Widget::initTcpSocket(TcpConnection *connection)
{
    tcpConnection = connection;
}

/**
 * @brief Widget::initTcpConnection
 */
void Widget::initTcpConnection()
{
    connect(tcpConnection,SIGNAL(signalBroadSet(TcpConnection*,int))
            ,SLOT(slotGetBroadCastInfo(TcpConnection*,int)));
    connect(mainViewWidget, SIGNAL(sendAdverInfo(QString,int,int,int,int)),
            SLOT(slotSendAdverInfo(QString,int,int,int,int)));

    connect(this,SIGNAL(getBeforeBroadInfo(TcpConnection*,int)),
            SLOT(slotGetBeforeBroadInfo(TcpConnection*,int)));
    connect(this,SIGNAL(getModyfiedInfo(TcpConnection*,int)),
            SLOT(slotGetModifiedInfo(TcpConnection*,int)));
    connect(musicPlayer,SIGNAL(sendMusicInfo(int,int,int)),
            SLOT(slotSendMusicInfo(int,int,int)));
}

void Widget::initchattingValue()
{
    ischatConnection = true;
    connect(&reConnectTimer,SIGNAL(timeout()),
            SLOT(slotReConnectChatServer()));
}

/**
 * @brief Widget::initCaption
 */
void Widget::initCaption()
{
    animation  = 0 ;
    fontColor  = YELLOW;

    ui->offBtn->setVisible(true);
    ui->on_bold->setVisible(false);

    connect(ui->ColorBtn,       SIGNAL(clicked()),SLOT(slotSetCaptionColor()));
    connect(ui->showCaptionBtn, SIGNAL(clicked()),SLOT(slotSendCaptionInfo()));
    connect(ui->stopCaptinBtn,  SIGNAL(clicked()),SLOT(slotSendCaptionState()));
    connect(ui->on_bold,        SIGNAL(clicked()),SLOT(slotGetCaptionBold()));
    connect(ui->off_bold,       SIGNAL(clicked()),SLOT(slotGetCaptionBold()));

    /*send caption information to mainViewWidget*/
    connect(this,SIGNAL(sendCaptionInfo(int,int,int,QString)),mainViewWidget,
            SLOT(slotGetCaptionInfo(int,int,int,QString)));
    connect(this,SIGNAL(sendCaptionState(bool)),mainViewWidget,
            SLOT(slotGetCaptionState(bool)));
    connect(this,SIGNAL(sendCaptionBold(bool)),mainViewWidget,
            SLOT(slotGetCaptionBold(bool)));

    /*for caption animation button connect*/
    connect(ui->LRBtn,SIGNAL(clicked()),
            SLOT(slotClickLeftTORightBtn()));
    connect(ui->RLBtn,SIGNAL(clicked()),
            SLOT(slotClickRightTOLeftBtn()));
    connect(ui->UDBtn,SIGNAL(clicked()),
            SLOT(slotClickUpToDownBtn()));
    connect(ui->DUBtn,SIGNAL(clicked()),
            SLOT(slotClickDownToUpBtn()));
    connect(ui->stopBtn,SIGNAL(clicked()),
            SLOT(slotClickStopCaptionBtn()));
}

/**
 * @brief Widget::slotGetCaptionBold
 */
void Widget::slotGetCaptionBold()
{
    qDebug()<<"actionCaptionBold!";
    /*when on_bold button state is active*/
    if(ui->on_bold->isVisible())
    {
        emit sendCaptionBold(false);

        ui->on_bold->setVisible(false);
        ui->off_bold->setVisible(true);
    }

    /*when off_bold button state is active*/
    else
    {
        emit sendCaptionBold(true);

        ui->on_bold->setVisible(true);
        ui->off_bold->setVisible(false);
    }
}

/**
 * @brief Widget::initTimer
 */
void Widget::initTimer()
{
    broadCastTime = 0;
    broadCastTimer = new QTimer();

    connect(broadCastTimer,SIGNAL(timeout()),SLOT(slotBroadCastTime()));

    for( int i = 0 ; i < VIDEOTIMER_NUMBER ; i ++ )
        connect(&videoTimer[i],SIGNAL(timeout()),SLOT(slotStartPlayMoive()));
}

/**
 * @brief Widget::initSettingTab
 */
void Widget::initSettingTab()
{
    ui->RoonInfoTab->setVisible(false);
    ui->ChattingTab->setVisible(false);

    connect(ui->DeviceBtn,SIGNAL(clicked()),SLOT(slotClickDevBtn()));
    connect(ui->RoomBtn,  SIGNAL(clicked()),SLOT(slotClickRoomInfoBtn()));
    connect(ui->ChattBtn, SIGNAL(clicked()),SLOT(slotClickChattingBtn()));
}

/**
 * @brief Widget::initDefaultDevice
 */
void Widget::initDefaultDevice()
{
    connect(this,SIGNAL(setDefaultCamra())          ,SLOT(slotActivateCamera()));
    connect(this,SIGNAL(startDefaultSetBroadCast()) ,SLOT(slotStartBroadCast()));
    connect(this,SIGNAL(setDefaultAudio()), audioDev,SLOT(slotSendAudioData()));
}

/**
 * @brief Widget::initMainImageView
 */
void Widget::initMainImageView()
{
    /*'mainViewWidget' is screen for result image*/
    mainViewWidget = new ImageGLView(this);

    mainViewWidget->setMinimumSize(640,480);
    ui->MainImageViewLay->addWidget(mainViewWidget);

    connect(this,SIGNAL(fullScreenView()),mainViewWidget,
            SLOT(slotFullScreenView()));
}

/**
 * @brief Widget::initModeView
 */
void Widget::initModeView()
{
    ui->normalCheck->setChecked(true);

    connect(ui->leftUPCheck    ,SIGNAL(toggled(bool)),
            SLOT(slotChangeLeftUPMode(bool)));
    connect(ui->leftDOWNCheck  ,SIGNAL(toggled(bool)),
            SLOT(slotChangeLeftDOWNMode(bool)));
    connect(ui->rightUPCheck   ,SIGNAL(toggled(bool)),
            SLOT(slotChangeRightUPMode(bool)));
    connect(ui->rightDOWNCheck ,SIGNAL(toggled(bool)),
            SLOT(slotChangeRightDOWNMode(bool)));
    connect(ui->normalCheck    ,SIGNAL(toggled(bool)),
            SLOT(slotChangeNormalMode(bool)));

    ui->viewModeGB->setEnabled(false);
}

/**
 * @brief Widget::initMainWidget
 */
void Widget::initMainWidget()
{
    /*for visualizing voice level*/
    voicePainter = new VoicePainter(this);
    voicePainter->setMinimumSize(100,5);

    /*for moveMouseEvent function */
    setMouseTracking(true);

    isFullWidget = false;
    setBroadCastWidget(STOP);

    ui->isSettingBroadCast->setChecked(true);
    ui->voiceView->addWidget(voicePainter);
    ui->onBtn->setVisible(false);
    ui->pause_btn->setVisible(false);

    /*set for opacitybar*/
    ui->opacityBar->setValue(100);
    ui->opacityBar->setMaximum(100);
    ui->opacityBar->setMinimum(20);
    connect(ui->opacityBar,SIGNAL(valueChanged(int)),this,
            SLOT(slotChageTransparency(int)));

    /*set for MainWidget control Button*/
    connect(ui->closeWidgetBtn          ,SIGNAL(clicked()),
            this,SLOT(slotCloseMainWidget()));
    connect(ui->hideBtn                 ,SIGNAL(clicked()),
            SLOT(slotHideMainWidget()));
    connect(ui->chageBigWidgetBtn       ,SIGNAL(clicked()),
            SLOT(slotChangeBigWidget()));
    connect(ui->FullScreenViewBtn       ,SIGNAL(clicked()),
            SLOT(slotFullScreenView()));
    connect(ui->mainStartBroadCastBtn   ,SIGNAL(clicked()),
            SLOT(slotDefaultSetBroadCast()));
    connect(ui->sendChatBtn             ,SIGNAL(clicked()),
            SLOT(slotSendChat()));
    connect(ui->onBtn                   ,SIGNAL(clicked()),
            SLOT(slotCancelMorpology()));

    connect(this,SIGNAL(pressEnterKey()),SLOT(slotSendChat()));
    connect(this,SIGNAL(sendBroadcastState(bool)),mainViewWidget,
            SLOT(slotPauseBroadCast(bool)));

    connect(mainViewWidget, SIGNAL(restartBroadcat()),
            SLOT(slotRestartBroadcast()));
}

/**
 * @brief Widget::initBtnConnect
 */
void Widget::initBtnConnect()
{
    connect(ui->cancelDeviceBtn     ,SIGNAL(clicked()),this,
            SLOT(slotStopCamera()));
    connect(ui->appyDeviceBtn       ,SIGNAL(clicked()),this,
            SLOT(slotActivateCamera()));
    connect(ui->StartBroadcastBtn   ,SIGNAL(clicked()),this,
            SLOT(slotStartBroadCast()));
    connect(ui->CancelBroadcasrBtn  ,SIGNAL(clicked()),this,
            SLOT(slotCancelBroadCast()));
    connect(ui->stopBroadCastBtn    ,SIGNAL(clicked()),this,
            SLOT(slotStopBroadCast()));
    connect(ui->offBtn              ,SIGNAL(clicked()),this,
            SLOT(slotShowImageSettingWidget()));
    connect(ui->isSettingBroadCast  ,SIGNAL(clicked()),this,
            SLOT(slotShowBroadCastSetting()));
    connect(ui->pause_btn           ,SIGNAL(clicked()),this,
            SLOT(slotPauseBroadcast()));
    connect(ui->StartBroadcastBtn   ,SIGNAL(clicked()),audioDev,
            SLOT(slotSendAudioData()));
    connect(ui->stopBroadCastBtn    ,SIGNAL(clicked()),audioDev,
            SLOT(slotStopSendAudioData()));

}

/**
 * @brief Widget::initAudio
 */
void Widget::initAudio()
{
    audioDev     = new AudioDevice(this);
    bgmDev       = new AudioDevice(this);
    audioDevInfo = new AudioDeviceInfo(this);

    isAudioActive = false;
    audioList.clear();

    /*get current usable audio device info*/
    audioList = audioDevInfo->getAudioDevInfo();

    /*add usable audio device to audioDevice combobox ui*/
    for(int i=0;i<audioList.size();i++)
        ui->AudioDeviceCombo->addItem(audioList.at(i).deviceName());

    /*get current audioDevice combobox index*/
    audioDevIndex = ui->AudioDeviceCombo->currentIndex();
    activateAudio(audioDevIndex);

    connect(ui->AudioDeviceCombo ,SIGNAL(activated(int)),
            SLOT(slotChangeAudioDev(int)));
    connect(ui->audioVolSlider   ,SIGNAL(valueChanged(int)),audioDev,
            SLOT(slotGetAudioVolume(int)));
    connect(audioDev             ,SIGNAL(sendAudiorowData(QByteArray)),
            SLOT(slotGetAudioRowData(QByteArray)));
}

/**
 * @brief Widget::initValAndStopDev
 */
void Widget::initValAndStopDev()
{
    /*when broadcast is stopped,
     * this function will be called and init value,
     * UI for broadcast*/

    broadCastTime  = 0;

    isMorpology    = false;
    isCameraActive = false;

    cameraDev->stop();
    broadCastTimer->stop();

    mainViewWidget->ImageLoad(logoImage);

    pause(STOP);
    setBroadCastWidget(STOP);
    ui->broadCastTime->setText("00:00");

    /*init current chromakey btn to off state*/
    ui->onBtn->setVisible(false);
    ui->offBtn->setVisible(true);
}

/**
 * @brief Widget::initSettingWidget
 */
void Widget::initSettingWidget()
{
    connect(&settingWidget ,SIGNAL(cancelMorpology()),this,SLOT(slotCancelMorpology()));
    connect(&settingWidget ,SIGNAL(applyMorpology()) ,this,SLOT(slotApplyMorpology()));
    connect(&settingWidget ,SIGNAL(sendProcessData(int*,int*,int*,int*,int*,int*)),
            morpoloyPro,   SLOT(slotSetThresholdData(int*,int*,int*,int*,int*,int*)));

    connect(&settingWidget   ,SIGNAL(sendBackgroundImg(QImage&)),this,
            SLOT(slotGetTargetImg(QImage&)));
    connect(&settingWidget   ,SIGNAL(sendBackgroundImg(QVideoDecoder&)),this,
            SLOT(slotGetTargetImg(QVideoDecoder&)));
    connect(&settingWidget   ,SIGNAL(sendAdverImg(QImage&)),
            SLOT(slotGetAdverImg(QImage&)));

    connect(ui->normalCheck ,SIGNAL(toggled(bool)),morpoloyPro,SLOT(slotChangeMode(bool)));
    connect(this,SIGNAL(sendAdverImg(QImage&)),mainViewWidget,SLOT(slotGetAdverImg(QImage&)));

    connect(mainViewWidget,SIGNAL(clickBeforeAdver())  ,&settingWidget,
            SLOT(slotShowBeforeAdverImg()));
    connect(mainViewWidget,SIGNAL(clickNextAdver())    ,&settingWidget,
            SLOT(slotShowNextAdverImg()));
    connect(mainViewWidget,SIGNAL(clickBeforeSource()) ,&settingWidget,
            SLOT(slotShowBeforeSourceImg()));
    connect(mainViewWidget,SIGNAL(clickNextSource())   ,&settingWidget,
            SLOT(slotShowNextSourceImg()));
    connect(mainViewWidget,SIGNAL(sendSettingVal(int,int)),&settingWidget,
            SLOT(slotGetAdverInfo(int,int)));
}

/**
 * @brief Widget::initImageprocessing
 */
void Widget::initImageprocessing()
{
    morpoloyPro = new MorPologyProcessing(this);
    encoding    = new ImageEncoding(this);

    isMorpology = false;
    screenMode = NORMAL;

    setQualityLists();

    connect(this,SIGNAL(initAdverImg()),mainViewWidget,SLOT(slotInitAdverImg()));
    connect(this,SIGNAL(sendResultImage()),SLOT(slotSendResultImage()));
    connect(ui->QualityCombo, SIGNAL(activated(int)),SLOT(slotChangeQualityVal(int)));
}

/**
 * @brief Widget::initImageSocket
 */
void Widget::initImageSocket()
{
    imgSocket   = new SendImageSocket(this);
    isBroadCast = false;
}

/**
 * @brief Widget::initCamera
 */
void Widget::initCamera()
{
    cameraDevInfo = new CameraDeviceInfo(this);
    cameraDev     = new CameraDevice(this);
    sourceImg     = new QImage();

    isCameraActive = false;

    /*get current usable camera device info*/
    QStringList cameraDevList = cameraDevInfo->getCameraDevinfo();


     /*add usable camera device to cameraDevice combobox ui*/
    for(int i=0;i<cameraDevList.size();i++)
        ui->CameraDeviceCombo->addItem(cameraDevList.at(i));

    /*add resolution list to combo box ui*/
    setResolutionLists();

    connect(cameraDev,SIGNAL(readyImage(QImage&)) ,SLOT(slotgetFrame(QImage&)));
}

/**
 * @brief Widget::initChatting
 */
void Widget::initChatting()
{
    chatting = new SenderChatting(id,pw);
    menu     = new QMenu();

    setKickMenuList();

    connect(this,SIGNAL(sendChatContext(QString)),
            chatting,SLOT(slotSendChatContext(QString)));
    connect(this,SIGNAL(sendKickMemberNick(QString)),chatting
            ,SLOT(slotGetKickMember(QString)));

    connect(chatting,SIGNAL(sendChatContext(QString,QString)),
            SLOT(slotGetChatingContext(QString,QString)));
    connect(chatting,SIGNAL(sendChatmember(QString,int)),
            SLOT(slotGetChatMember(QString,int)));
    connect(chatting,SIGNAL(sendChatExitMember(QString)),
            SLOT(slotGetChatExitMeber(QString)));
    connect(chatting,SIGNAL(noticechatConnectionSuccess()),
            SLOT(slotGetnoticeChatConSuccess()));
    connect(chatting,SIGNAL(noticeChatErr()),
            SLOT(slotGetnoticeServerErr()));
    connect(ui->chatMemberList,SIGNAL(clicked(QModelIndex))
            ,SLOT(slotClickMemberList(QModelIndex)));
    connect(menu,SIGNAL(triggered(QAction*)),
            SLOT(slotKickMember(QAction*)));
}

/**
 * @brief Widget::initMusicPlayer
 */
void Widget::initMusicPlayer()
{
    musicPlayer     = new MusicPlayer(this);
    musicListCount  = 0;

    ui->stopMusicBtn->setVisible(false);

    connect(ui->addMusicBtn  ,SIGNAL(clicked()),SLOT(slotAddMusicFile()));
    connect(ui->playMusicBtn ,SIGNAL(clicked()),SLOT(slotPlayMusic()));
    connect(ui->stopMusicBtn ,SIGNAL(clicked()),SLOT(slotStopMusic()));
    connect(ui->nextMusicBtn ,SIGNAL(clicked()),SLOT(slotPlayNextMusic()));
    connect(ui->beforMusicBtn,SIGNAL(clicked()),SLOT(slotPlayBeforMusic()));

    connect(ui->musicFileList,SIGNAL(clicked(QModelIndex)),
            SLOT(slotClickMusicList(QModelIndex)));
    connect(ui->musicVolSlider,SIGNAL(valueChanged(int)),musicPlayer,
            SLOT(slotGetMusicVolume(int)));

    connect(this,SIGNAL(playMusic(QString)),musicPlayer,SLOT(slotPlayMusic(QString)));
    connect(this,SIGNAL(stopMusic()),musicPlayer,SLOT(slotStopMusic()));
    connect(this,SIGNAL(playNextMusic()),this,SLOT(slotPlayNextMusic()));

    connect(musicPlayer,SIGNAL(sendMusicMetaData(QString,QString)),
            SLOT(slotGetMusicMetaData(QString,QString)));
    connect(musicPlayer,SIGNAL(sendMusicCurrentTime(qint64)),
            SLOT(slotGetCurrentMusicTime(qint64)));
    connect(musicPlayer,SIGNAL(sendMusicCurrentDuration(qint64)),
            SLOT(slotGetMusicDuration(qint64)));
    connect(musicPlayer,SIGNAL(sendEndofMusic()),
            SLOT(slotPlayNextMusic()));
}

/**
 * @brief Widget::initRoomInfo
 */
void Widget::initRoomInfo()
{
    currentMember = 0;
    roomLike      = 0;
    roomBookmark  = 0;
}

/**
 * @brief Widget::slotClickMusicList
 * @param list
 */
void Widget::slotClickMusicList(QModelIndex list)
{
    QString musicName;
    int     clikedMusicNum = list.row();

    nowPlayNum = clikedMusicNum;

    /*get clicked music's address*/
    musicName = musicAddressList.value(clikedMusicNum);

    emit stopMusic();
    emit playMusic(musicName);

    ui->playMusicBtn->setVisible(false);
    ui->stopMusicBtn->setVisible(true);
}

/**
 * @brief Widget::slotClickDevBtn
 */
void Widget::slotClickDevBtn()
{
   activeTab(DEVICE_TAB);
}

/**
 * @brief Widget::slotChangeAudioDev
 * @param index
 */
void Widget::slotChangeAudioDev(int index)
{
    audioDev->stopAudio();
    activateAudio(index);
}

/**
 * @brief Widget::slotChangeQualityVal
 * @param index
 */
void Widget::slotChangeQualityVal(int index)
{
    switch(index)
    {
        case LOW_IMAGE_QUALITY:
            encoding->setQuality(LOW_IMAGE_QUALITY_VAL);
            break;

        case NOMAL_IMAGE_QUALITY:
            encoding->setQuality(NOMAL_IMAGE_QUALITY_VAL);
            break;

        case HIGH_IMAGE_QUALITY:
            encoding->setQuality(HIGH_IMAGE_QUALITY_VAL);
            break;
    }
}

/**
 * @brief Widget::slotClickRoomInfoBtn
 */
void Widget::slotClickRoomInfoBtn()
{
    activeTab(ROOMINFOR_TAB);
}

/**
 * @brief Widget::slotClickChattingBtn
 */
void Widget::slotClickChattingBtn()
{
    activeTab(CHATTING_TAB);
}

/**
 * @brief Widget::slotGetMusicDuration
 * @param duration
 */
void Widget::slotGetMusicDuration(qint64 duration)
{
    musicTime = duration;
    /*Get music play Minute*/
    musicTime_m = duration / ONE_MINUTE;
    /*Get music play Secend*/
    musicTime_s = ( duration - musicTime_m * ONE_MINUTE ) / ONE_SECOND;

    ui->musicSlider->setRange(0,duration);
    ui->musicSlider->setEnabled(duration > 0);
    ui->musicSlider->setPageStep(duration / 10);
}

/**
 * @brief Widget::slotGetCurrentMusicTime
 * @param time
 */
void Widget::slotGetCurrentMusicTime(qint64 time)
{
    float currentPosition;

    currentPosition = (time / musicTime)*100;
    ui->musicSlider->setValue(time);
}

/**
 * @brief Widget::keyPressEvent
 * @param event
 */
void Widget::keyPressEvent(QKeyEvent *event)
{
    /*when sender click enter key,
     * this slot will be called and
     * send current chatting context to server*/
    if(event->key() == ENTERKEY)
        emit pressEnterKey();
}

/**
 * @brief Widget::slotGetMusicMetaData
 * @param title
 * @param artist
 */
void Widget::slotGetMusicMetaData(QString title, QString artist)
{
    /*get music metadata(music title, artist,
     * and then change ui*/

    /*set default ui text*/
    if(title.size()==0)
        title = "None Title";

    /*set default ui text*/
    if(artist.size()==0)
        artist = "None Artist";

    /*apply music informotion to ui text*/
    ui->MusicTitle->setText(title);
    ui->SingerName->setText(artist);
}

/**
 * @brief Widget::slotBroadCastTime
 */
void Widget::slotBroadCastTime()
{
    QString timeStr_M;
    QString timeStr_S;
    QString timeStr;

    /*when this slot is called per 1 minute,
     * broadCastTime's value is added +1(1m)*/
    broadCastTime+=1;

    /*calculate minute*/
    if(broadCastTime/60 < 10)
        timeStr_M = "0"+timeStr_M.setNum((broadCastTime/60));
    else
        timeStr_M = timeStr_M.setNum((broadCastTime/60));

    /*calculate second*/
    if(broadCastTime%60 < 10)
        timeStr_S = "0"+timeStr_S.setNum((broadCastTime%60));
    else
       timeStr_S = timeStr_S.setNum((broadCastTime%60));

    /*update current broadcast time*/
    timeStr = timeStr_M+":"+timeStr_S;
    ui->broadCastTime->setText(timeStr);
}

/**
 * @brief Widget::slotChangeLeftUPMode
 * @param mode
 */
void Widget::slotChangeLeftUPMode(bool mode)
{
    setScreenMode(mode, LEFT_UP);
}

/**
 * @brief Widget::slotChangeLeftDOWNMode
 * @param mode
 */
void Widget::slotChangeLeftDOWNMode(bool mode)
{
    setScreenMode(mode, LEFT_DOWN);
}

/**
 * @brief Widget::slotChangeRightUPMode
 * @param mode
 */
void Widget::slotChangeRightUPMode(bool mode)
{
    setScreenMode(mode, RIGHT_UP);
}

/**
 * @brief Widget::slotChangeRightDOWNMode
 * @param mode
 */
void Widget::slotChangeRightDOWNMode(bool mode)
{
    setScreenMode(mode, RIGHT_DOWN);
}

/**
 * @brief Widget::slotChangeNormalMode
 * @param mode
 */
void Widget::slotChangeNormalMode(bool mode)
{
    setScreenMode(mode, NORMAL);
}

/**
 * @brief Widget::slotDefaultSetBroadCast
 */
void Widget::slotDefaultSetBroadCast()
{
    if(isCameraActive)
    {
        emit setDefaultAudio();
        emit startDefaultSetBroadCast();
    }

    else
    {
        emit setDefaultCamra();
        emit setDefaultAudio();
        emit startDefaultSetBroadCast();
    }
}

/**
 * @brief Widget::slotFullScreenView
 */
void Widget::slotFullScreenView()
{
    emit fullScreenView();
}

/**
 * @brief Widget::slotChangeBigWidget
 */
void Widget::slotChangeBigWidget()
{
    if(isFullWidget)
    {
        isFullWidget = false;

        this->resize(beforWSize,beforHSize);
        this->move(beforWidgetX,beforWidgetY);
    }

    else
    {
        isFullWidget = true;

        beforWSize = this->width();
        beforHSize = this->height();

        beforWidgetX = this->x();
        beforWidgetY = this->y();

        this->showFullScreen();
    }
}

/**
 * @brief Widget::slotCloseMainWidget
 */
void Widget::slotCloseMainWidget()
{
    this->close();

    if(isCameraActive)
        cameraDev->stop();

    if(isAudioActive)
        audioDev->stopAudio();
}

/**
 * @brief Widget::slotHideMainWidget
 */
void Widget::slotHideMainWidget()
{
    this->showMinimized();
}

/**
 * @brief Widget::slotApplyMorpology
 */
void Widget::slotApplyMorpology()
{
    isMorpology         = true;
    isShowSettingWidget = false;

    ui->viewModeGB->setEnabled(true);
    ui->offBtn->setVisible(false);
    ui->onBtn->setVisible(true);

    /*if there is videofile, this statement
     * will be operate*/
    if(isVideoFile)
        startVideoTimer(VIDEOTIMER_NUMBER);
}

/**
 * @brief Widget::slotChageTransparency
 * @param value
 */
void Widget::slotChageTransparency(int value)
{
    setWindowOpacity((float)(value)/100);
}

/**
 * @brief Widget::slotCancelMorpology
 */
void Widget::slotCancelMorpology()
{
    isMorpology         = false;
    isShowSettingWidget = false;

    ui->viewModeGB->setEnabled(false);

    ui->onBtn->setVisible(false);
    ui->offBtn->setVisible(true);

    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);

    emit initAdverImg();
}

/**
 * @brief Widget::activateAudio
 * @param index
 */
void Widget::activateAudio(int index)
{
    isAudioActive = true;
    audioDev->setAudioInput(index);
}

/**
 * @brief Widget::slotShowBroadCastSetting
 */
void Widget::slotShowBroadCastSetting()
{
    if(ui->isSettingBroadCast->isChecked() == true)
        ui->SettingTap->setVisible(true);

    else
        ui->SettingTap->setVisible(false);
}

/**
 * @brief Widget::slotShowImageSettingWidget
 */
void Widget::slotShowImageSettingWidget()
{
    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);


    settingWidget.show();
    isShowSettingWidget = true;
}

/**
 * @brief Widget::setResolutionLists
 */
void Widget::setResolutionLists()
{
    ui->ResolutionCombo->addItem("640 x 480");
    ui->ResolutionCombo->addItem("1280 x 720");
    ui->ResolutionCombo->addItem("1920 x 1080");
}

/**
 * @brief Widget::slotStartBroadCast
 */
void Widget::slotStartBroadCast()
{
    if( tcpConnection->writePacket(Major_BroadcastSettring,BroadSet_Start) == -1 )
        qDebug("Error infor: Fail to send start broadcasting packet!");
    else
    {
        requestBeforeRoomInfo();
        startBroadCasting();
        activeTab(CHATTING_TAB);

        if(!ischatConnection)
            initChatting();
        qDebug("Info : Succes to send start broadcasting packet!");
    }
}

void Widget::slotReConnectChatServer()
{
    if(!ischatConnection)
        initChatting();
    qDebug("try to reconnect with chat server...");
}

/**
 * @brief Widget::setRoomInfo
 * @param name
 */
void Widget::setRoomInfo(QString name)
{
    ui->roonNameText->setText(name);
}

/**
 * @brief Widget::setBroadCastWidget
 * @param state
 */
void Widget::setBroadCastWidget(bool state)
{
    ui->MusicAndTextViewRay->setVisible(state);
    ui->mainStartBroadCastBtn->setVisible(!state);
    ui->stopBroadCastBtn->setVisible(state);
    ui->broadCastTime->setVisible(state);
    ui->roomInfoLay_2->setVisible(state);
    ui->StartBroadcastBtn->setEnabled(!state);
    ui->pause_btn->setVisible(state);

    isBroadCast = state;
}

/**
 * @brief Widget::setScreenMode
 * @param isSmall
 * @param mode
 */
void Widget::setScreenMode(bool isSmall, int mode)
{
    if(isSmall)
        screenMode = mode;
    else
    {
        sScaleWidth  = 640;
        sScaleHeight = 480;
    }
}

/**
 * @brief Widget::setImageQuility
 * @param quaility
 */
void Widget::setImageQuility(int quaility)
{
    switch(quaility)
    {
        case LOW_IMAGE_QUALITY:
            encoding->setQuality(LOW_IMAGE_QUALITY_VAL);
            break;

        case NOMAL_IMAGE_QUALITY:
            encoding->setQuality(NOMAL_IMAGE_QUALITY_VAL);
            break;

        case HIGH_IMAGE_QUALITY:
            encoding->setQuality(HIGH_IMAGE_QUALITY_VAL);
            break;
    }
}

/**
 * @brief Widget::slotCancelBroadCast
 */
void Widget::slotCancelBroadCast()
{
    isBroadCast = false;
    isMorpology = false;

    ui->viewModeGB->setEnabled(false);

    /*init current chromakey btn to off state*/
    ui->onBtn->setVisible(false);
    ui->offBtn->setVisible(true);


    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);

    emit initAdverImg();
}

/**
 * @brief Widget::slotStopMusic
 */
void Widget::slotStopMusic()
{
    emit stopMusic();

    ui->playMusicBtn->setVisible(true);
    ui->stopMusicBtn->setVisible(false);

    ui->MusicTitle->setText(MUSIC_TITLE);
    ui->SingerName->setText(MUSIC_ARTIST);
}

/**
 * @brief Widget::slotPlayNextMusic
 */
void Widget::slotPlayNextMusic()
{
    qDebug("play next music");
    emit stopMusic();

    /*find next music's index number*/
    if((musicList.size()-1)-nowPlayNum > 0)
        nowPlayNum++;
    else
        nowPlayNum=0;

    /*get next music's file directory*/
    QString fileName = musicAddressList.value(nowPlayNum);;

    /*play next music*/
    emit playMusic(fileName);
}

/**
 * @brief Widget::slotPlayBeforMusic
 */
void Widget::slotPlayBeforMusic()
{
    qDebug("play before music");
    emit stopMusic();

    /*find before music's index number*/
    if(nowPlayNum==0)
        nowPlayNum = musicList.size()-1;
    else
        nowPlayNum--;

    /*get before music's file directory*/
    QString fileName = musicAddressList.value(nowPlayNum);;

    /*play before music*/
    emit playMusic(fileName);
}

/**
 * @brief Widget::slotPlayMusic
 */
void Widget::slotPlayMusic()
{
    nowPlayNum       = ui->musicFileList->currentIndex().row();
    if(musicList.size()>0)
    {
        if(nowPlayNum == -1)
            nowPlayNum = DEFAULT_MUSIC_INDEX;

        QString fileName = musicAddressList.value(nowPlayNum);
        emit playMusic(fileName);

        ui->playMusicBtn->setVisible(false);
        ui->stopMusicBtn->setVisible(true);
    }
    else
    {
        /* print error */
        qDebug("Error Infor : There is no playlist");
        QMessageBox::information(this,tr("ERROR"),tr("Please add at least one music"));
    }
}

/**
 * @brief Widget::setQuailtyLists
 */
void Widget::setQualityLists()
{
    ui->QualityCombo->addItem("Low");
    ui->QualityCombo->addItem("Normal");
    ui->QualityCombo->addItem("High");
}

/**
 * @brief Widget::slotAddMusicFile
 */
void Widget::slotAddMusicFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "//",tr("Images (*.mp3 *.wma)"));
    int *musicNameposion;

    /*when file's name is not NULL,
     * add music list*/
    if(fileName.size()!=0)
    {
        /*store music's file address in hash table*/
        musicAddressList.insert(musicListCount,fileName);
        musicListCount++;

        /*get music file name's start,end position*/
        musicNameposion = getMusicName(fileName);

        /*get music name*/
        fileName = fileName.mid(musicNameposion[0],musicNameposion[1]);

        musicList.operator <<(fileName);
        musicModel.setStringList(musicList);

        ui->musicFileList->setModel(&musicModel);
    }
}

/**
 * @brief Widget::setScaleSize
 * @param w
 * @param h
 */
void Widget::setScaleSize(int w, int h)
{
    this->sScaleWidth  = w;
    this->sScaleHeight = h;
}

/**
 * @brief Widget::getMusicName
 * @param musicFileName
 * @return
 */
int* Widget::getMusicName(QString musicFileName)
{
    int musicNamePosition[2];
    int strSize = musicFileName.length();

    int startPositon;
    int size;

    for(int i=0;i<strSize;i++)
    {
        /*find '.' position : (ex)'.'mp3*/
        if(musicFileName.at(i)=='.')
        {
            size=i-1;
            for(int j = i-1 ;j<i;j--)
            {
                /*find last '/' position : (ex) music'/'a.mp3*/
                if(musicFileName.at(j)=='/')
                {
                    startPositon=j+1;
                    break;
                }
            }
        }
        musicNamePosition[0] = startPositon;
        musicNamePosition[1] = size - startPositon + 1;
    }
    return musicNamePosition;
}

/**
 * @brief Widget::slotSetCaptionColor
 */
void Widget::slotSetCaptionColor()
{
    QColor color = QColorDialog::getColor();
    QString r,g,b;

    ui->ColorBtn->setStyleSheet("background-color: rgb("+r.setNum(color.red())+","+
                                g.setNum(color.green())+","+ b.setNum(color.blue())+")");
    fontColor = color.rgb();
}

/**
 * @brief Widget::slotClickLeftTORightBtn
 */
void Widget::slotClickLeftTORightBtn()
{
    animation = LEFT_TO_RIGHT;
}

/**
 * @brief Widget::slotClickRightTOLeftBtn
 */
void Widget::slotClickRightTOLeftBtn()
{
    animation = RIGHT_TO_LEFT;
}

/**
 * @brief Widget::slotClickUpToDownBtn
 */
void Widget::slotClickUpToDownBtn()
{
    animation = UP_TO_DOWN;
}

/**
 * @brief Widget::slotClickDownToUpBtn
 */
void Widget::slotClickDownToUpBtn()
{
    animation = DOWN_TO_UP;
}

/**
 * @brief Widget::slotClickStopCaptionBtn
 */
void Widget::slotClickStopCaptionBtn()
{
    animation = STOP;
}

/**
 * @brief Widget::slotSendCaptionInfo
 */
void Widget::slotSendCaptionInfo()
{
    QString captionText = ui->CaptionText->text();
    int     fontSize    = ui->FontSize->text().toInt();

    emit sendCaptionInfo(animation,fontSize,fontColor,captionText);
    emit sendCaptionState(true);
}

/**
 * @brief Widget::slotSendCaptionState
 */
void Widget::slotSendCaptionState()
{
    emit sendCaptionState(false);
}

/**
 * @brief Widget::slotGetBeforeBroadInfo
 * @param connection
 * @param msg
 */
void Widget::slotGetBeforeBroadInfo(TcpConnection *connection, int msg)
{
    Q_UNUSED(msg)

    QString title  = connection->getToken().data();
    QString notice = connection->getToken().data();

    QString max_connect = connection->getToken().data();
    QString bookmark    = connection->getToken().data();
    QString like        = connection->getToken().data();

        /*방을 처음 개설한 경우 like에 대한 정보가 없기때문에 예외처리를 한다*/
    if(like.size() == 0)
    {
        roomLike = 0 ;
        like     = "0";
    }

        /*방을 처음 개설한 경우 bookmark에 대한 정보가 없기때문에 예외처리를 한다*/
    if(bookmark.size() == 0)
    {
        roomBookmark = 0;
        bookmark = "0";
    }

    ui->roomBookMark->setText(bookmark);
    ui->roomLike->setText(like);
}
/**
 * @brief Widget::slotActivateCamera
 */
void Widget::slotActivateCamera()
{
    isCameraActive = true;

    /*when sender apply device tap,
     * this, slot will be called and camera device
     * will start operating*/
    cameraDev->setCameraDeviceNumber(ui->CameraDeviceCombo->currentIndex());

    /*set image value and camera resolution size*/
    switch(ui->ResolutionCombo->currentIndex())
    {
        case IMAGE_RESOLUTION_640_480:
            setCameraImageSize(W_640,H_480);
            break;

        case IMAGE_RESOLUTION_1280_720:
            setCameraImageSize(W_1280,H_720);
            break;

        case IMAGE_RESOLUTION_1920_1080:
            setCameraImageSize(W_1920,H_1080);
            break;
    }
    cameraDev->start();
    activeTab(ROOMINFOR_TAB);
}

/**
 * @brief Widget::slotPauseBroadcast
 */
void Widget::slotPauseBroadcast()
{
    pause(START);
}

void Widget::slotRestartBroadcast()
{
    pause(STOP);
}

void Widget::pause(bool state)
{
    isBroadCast = !state;
    isPause     = state;

    ui->pause_btn->setVisible(!state);
    emit sendBroadcastState(!state);
}

/**
 * @brief Widget::slotGetTargetImg
 * @param image
 */
void Widget::slotGetTargetImg(QImage &image)
{
    *targetImg = image.scaled(targetImg->width(),targetImg->height(),
                              Qt::IgnoreAspectRatio);

    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);
}

/**
 * @brief Widget::slotGetTargetImg
 * @param decoder
 */
void Widget::slotGetTargetImg(QVideoDecoder &decoder)
{
    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);

    isVideoFile = true;
    *videoCodec = decoder;

    /*start video timer to get a movie
     * frame image*/
    startVideoTimer(VIDEOTIMER_NUMBER);
}

/**
 * @brief Widget::slotGetAdverImg
 * @param image
 */
void Widget::slotGetAdverImg(QImage &image)
{
    emit sendAdverImg(image);
}

/**
 * @brief Widget::slotStartPlayMoive
 */
void Widget::slotStartPlayMoive()
{
    /*when videoTimer timeout,
     * this slot will be called and get
     * a moive frame by ffmepeg library*/

    videoCodec->seekNextFrame();
    videoCodec->getFrame(videoImg);

    *targetImg = videoImg.scaled(targetImg->width() ,targetImg->height());
}

/**
 * @brief Widget::slotgetFrame
 * @param image
 */
void Widget::slotgetFrame(QImage &image)
{
    /*when sender apply devices(camera, audio),
     * this slot will be called and get camera frame*/

    *sourceImg = image;
    if(isBroadCast == true)
    {
        /*when sender start broadcasting*/
        if(screenMode!=NORMAL)
        {
            /*when sender apply to change small screen mode*/
            if(sScaleWidth!=200 || sScaleHeight!=150)
            {
                if(sScaleWidth!=200)
                    sScaleWidth  = sScaleWidth-44;
                if(sScaleHeight!=150)
                    sScaleHeight = sScaleHeight-33;
            }

            *sourceImg = image.scaled(sScaleWidth,sScaleHeight);
            morpoloyPro->setMode(sScaleWidth,sScaleHeight,screenMode);
            *sourceImg = morpoloyPro->toSmallImage(*sourceImg);
        }
        emit sendResultImage();
    }
    else if(isBroadCast == false && isPause != true)
    {
        /*when sender do not start broadcasting*/
        if(isMorpology == true)
        {
            /*when sender apply chromarkey process*/
            if(screenMode != NORMAL)
            {
                /*when sender apply to change small screen mode*/
                if(sScaleWidth!=200 || sScaleHeight!=150)
                {
                    if(sScaleWidth!=200)
                         sScaleWidth  = sScaleWidth-44;
                    if(sScaleHeight!=150)
                        sScaleHeight = sScaleHeight-33;
                }
                *sourceImg = image.scaled(sScaleWidth,sScaleHeight);
                morpoloyPro->setMode(sScaleWidth,sScaleHeight,screenMode);
                *sourceImg = morpoloyPro->toSmallImage(*sourceImg);

            }
            morpoloyPro->chromakeyProcessMorphology(*sourceImg,*targetImg,*resultImg);
            mainViewWidget->ImageLoad(QPixmap::fromImage(*resultImg));
        }
        else
            mainViewWidget->ImageLoad(QPixmap::fromImage(image));
    }

    /*when sender active setting widget,
     * 'isShowSettingWidget' value will
     * be chaged to true and get camera frame*/
    if(isShowSettingWidget == true)
        settingWidget.getImageFrame(image);
}

/**
 * @brief Widget::slotSendResultImage
 */
void Widget::slotSendResultImage()
{
    encodData.clear();
    if(isMorpology)
    {
        /*when sender apply chromarkey process*/
        morpoloyPro->chromakeyProcessMorphology(*sourceImg,*targetImg,*resultImg);
        mainViewWidget->ImageLoad(QPixmap::fromImage(*resultImg));
        encoding->writeJPEGMem(morpoloyPro->toQImageRGB888(
                               mainViewWidget->getBeforeImage()),
                               encodData);
    }
    else
    {
        /*when sender do not apply chromarkey process*/
        mainViewWidget->ImageLoad(QPixmap::fromImage(*sourceImg));
        encoding->writeJPEGMem(morpoloyPro->toQImageRGB888(
                               mainViewWidget->grab().toImage())
                               ,encodData);
    }

    encodData.push_front('\0');
    encodData.push_front(UDP_HEADER);
    imgSocket->sendImageData(encodData);
}

/**
 * @brief Widget::slotGetAudioRowData
 * @param data
 */
void Widget::slotGetAudioRowData(QByteArray data)
{
    qint32   audioData  = 0;
    qint32   maxValue   = 0;
    qreal    voiceLevel = 0;

    for(int i=0; i<data.size();i++)
    {
        /*get a audio row data*/
        audioData = abs(data.at(i));
        /*find max amplitude*/
        maxValue = qFromLittleEndian<quint16>(abs(data.at(i)));
    }

    /*255 => max amplitude*/
    maxValue   = qMin(maxValue, 255);
    voiceLevel = qreal(maxValue) / 255;

    /*paint current voice level*/
    voicePainter->getVoiceData(voiceLevel*100);
}

/**
 * @brief Widget::slotGetBroadCastInfo
 * @param _connection
 * @param msg
 */
void Widget::slotGetBroadCastInfo(TcpConnection *_connection, int msg)
{

    /*when server give some information sender,
     * this slot will be called and sender use
     * this data*/
    switch(msg)
    {
    case BroadSet_Ok:
        setDestStreamInfo(tcpConnection);
        break;
    case BroadSet_Error:
        isBroadCast = false;
        qDebug("Unsuccess start!!");
        break;
    case BroadSet_All:
        emit getBeforeBroadInfo(_connection,msg);
        qDebug("get BroadInfor");
        break;
    case BroadSet_Modify:
        emit getModyfiedInfo(_connection,msg);
        break;
    }
}

/**
 * @brief Widget::slotGetModifiedInfo
 * @param connection
 * @param msg
 */
void Widget::slotGetModifiedInfo(TcpConnection *connection, int msg)
{
    Q_UNUSED(msg)
    /*when client click bookmark or like,
     * this slot will be called*/

    /*modified data : bookmark , like*/
    QString type = connection->getToken().data();

    if( type == BOOKMARK )
    {
        ui->roomBookMark->setText(connection->getToken().data());
        roomBookmark = ui->roomBookMark->text().toInt();
    }

    else if( type == LIKE )
    {
        ui->roomLike->setText(connection->getToken().data());
        roomLike = ui->roomLike->text().toInt();
    }
}

/**
 * @brief Widget::slotSendAdverInfo
 * @param url
 * @param startX
 * @param startY
 * @param width
 * @param height
 */
void Widget::slotSendAdverInfo(QString url, int startX, int startY, int width, int height)
{
    /*when adverImage's information is changed,
     * this slot will be called
     * INFORMATION CHANGED
     * - position change
     * - url change
     * - size change
     * - end so on
     */
    QByteArray packet;

    QString x,y,w,h;

    x = x.setNum(startX);
    y = y.setNum(startY);
    w = w.setNum(width);
    h = h.setNum(height);

    packet = makePacket(packet,url);
    packet = makePacket(packet,x);
    packet = makePacket(packet,y);
    packet = makePacket(packet,w);
    packet = makePacket(packet,h);

    tcpConnection->writePacket(Major_BroadcastSettring,BroadSet_AdvertiseInfo,packet);
}

void Widget::slotSendMusicInfo(int channelCnt, int bitRate, int sampleRate)
{
    /* when music infomation is changed,
     * this slot will be called
     * MUSIC INFOMATION CHANGE
     * - play new music
     * - play next music
     * - play before music
     * - end so on
     */
    QByteArray packet;

    QString chCnt, bRate, sRate;

    chCnt = chCnt.setNum(channelCnt);
    bRate = bRate.setNum(bitRate);
    sRate = sRate.setNum(sampleRate);

    packet = makePacket(packet, chCnt);
    packet = makePacket(packet, bRate);
    packet = makePacket(packet, sRate);

    tcpConnection->writePacket(Major_BroadcastSettring,BroadSet_MusicInfo,packet);
}

/**
 * @brief Widget::slotSendChat
 */
void Widget::slotSendChat()
{
    /*This is for Sender chat part*/
    /*When Sender send chatting cotext, this slot
     * will be called*/

    QString context = ui->chatContext->text();
    emit sendChatContext(context);

    ui->chatContext->clear();
}

/**
 * @brief Widget::slotGetChatMember
 * @param nick
 */
void Widget::slotGetChatMember(QString nick, int idNum)
{
    /*when client join sender's broadcast room,
     * this slot will be called and added +1 current member*/
    QString memberCnt;
    QListWidgetItem *member = new QListWidgetItem();

    /*if member is sender*/
    if(idNum == tcpConnection->getIDNum())
        member->setIcon(QIcon(":/res/images/sender.png"));

    /*if member is client*/
    else
        member->setIcon(QIcon(":/res/images/client.png"));

    member->setText(nick);

    ui->chatMemberList->addItem(member);
    ui->chattContextList->append("<font color='blue'>Infor : ["+nick+ "] join</font>");
    currentMember++;

    /*current member count change*/
    ui->roomMember->setText(memberCnt.setNum(currentMember));
}

/**
 * @brief Widget::slotGetChatExitMeber
 * @param nick
 */
void Widget::slotGetChatExitMeber(QString nick)
{
    /*when client exit sender's broadcast room,
     * this slot will be called and added -1 current member*/
    QString memberCnt;

    delete ui->chatMemberList->findItems(nick,Qt::MatchExactly).at(0);

    ui->chattContextList->append("<font color='red' ; align='center';>Infor : ["+nick+ "] exit</font>");
    currentMember--;

    ui->roomMember->setText(memberCnt.setNum(currentMember));
}

/**
 * @brief Widget::slotGetnoticeChatConSuccess
 */
void Widget::slotGetnoticeChatConSuccess()
{
    /*when making chatting connection is
     * successful, this slot will be called*/
    ui->sendChatBtn->setEnabled(true);
    ischatConnection = true;

    /*if reconnection timer is active,
     * do stop*/
    if(reConnectTimer.isActive())
    {
        reConnectTimer.stop();
        qDebug("reconnect timer stop...");
    }
}

/**
 * @brief Widget::slotClickMemberList
 * @param mList
 */
void Widget::slotClickMemberList(QModelIndex mList)
{
    /*when sender click member list joined,
     * this slot will be called*/
    kickIndex = mList.row();
    menu->exec(QCursor::pos());
}

/**
 * @brief Widget::slotGetChatingContext
 * @param nick
 * @param context
 */
void Widget::slotGetChatingContext(QString nick, QString context)
{
    /*This is for Client chat part*/
    /*When Client send chatting cotext, this slot
     * will be called*/
    ui->chattContextList->append(" "+nick+":"+context);
}

/**
 * @brief Widget::stopVideoTimer
 * @param timerNumber
 */
void Widget::stopVideoTimer(int timerNumber)
{
    /* this function make video timer to stop*/
    for(int i = 0 ; i < timerNumber; i++)
        videoTimer[i].stop();

    isVideoFile = false;
}

/**
 * @brief Widget::startVideoTimer
 * @param timerNumber
 */
void Widget::startVideoTimer(int timerNumber)
{
     /* this function make video timer to start*/
    for(int i = 0 ; i < timerNumber; i++)
        videoTimer[i].start(VIDEOTIMEOUT);
}

/**
 * @brief Widget::slotStopBroadCast
 */
void Widget::slotStopBroadCast()
{
    /*when sender stop their broadcast,
     * this slot wiil be called*/

    /*send stop broadcast signal to server*/
    if( tcpConnection->writePacket(Major_BroadcastSettring,BroadSet_Stop) == -1 )
        qDebug("Error : Fail to send stop broadcasting packet!");
    else
        qDebug("Info : Succes to send stop broadcasting packet!");


    initValAndStopDev();
    activeTab(DEVICE_TAB);
    resize(beforWSize,beforHSize);
    resetTimer();

    /*when stop broadcasting,
     * init draw adverimag and caption text
     * stop music*/
    emit initAdverImg();
    emit sendCaptionState(STOP);
    emit stopMusic();
}

/**
 * @brief Widget::slotGetnoticeServerErr
 */
void Widget::slotGetnoticeServerErr()
{
    /*notice to sender about current server state*/
    ui->chattContextList->append("<font color='red'>chatting server is unstable</font>");

    /*protect sending context to chatting server*/
    ui->sendChatBtn->setEnabled(false);

    /*init current chatting member ui, value*/
    ui->chatMemberList->clear();
    currentMember = 0;
    ischatConnection = false;

    /*exit chatting server*/
    delete chatting;
    chatting = NULL;

    /*try to reconnect chatting sever*/
    if(!reConnectTimer.isActive())
    {
        reConnectTimer.start(RECONNECT_INTERVAL);
        qDebug("reconnect timer active...");
    }

}

/**
 * @brief Widget::slotStopAudio
 */
void Widget::slotStopAudio()
{
    audioDev->stopAudio();
}

/**
 * @brief Widget::slotStopCamera
 */
void Widget::slotStopCamera()
{
    isCameraActive = false;

    cameraDev->stop();
    mainViewWidget->ImageLoad(logoImage);
}

/**
 * @brief Widget::slotKickMenber
 * @param a
 */
void Widget::slotKickMember(QAction *a)
{
    /*when sender click member list and click out button,
     * this slot will be called and kicked a member clicked*/
    Q_UNUSED(a)

    QString nick = ui->chatMemberList->item(kickIndex)->text();
    delete ui->chatMemberList->findItems(nick,Qt::MatchExactly).at(0);

    ui->chattContextList->append("<font color='red'>------Infor : ["+nick+ "] kicked.------</font>");
    emit sendKickMemberNick(nick);
}

/**
 * @brief Widget::getLogInInfo
 * @param id
 * @param pw
 */
void Widget::getLogInInfo(QString id, QString pw)
{
    /* get login data(id,passward) ,for login
     chatting server*/
    this->id = id;
    this->pw = pw;
}

/**
 * @brief Widget::setCameraImageSize
 * @param width
 * @param height
 */
void Widget::setCameraImageSize(int width, int height)
{
    targetImg = new QImage(width, height,QImage::Format_RGB888);
    resultImg = new QImage(width, height,QImage::Format_RGB888);

    cameraDev->setResolution(width, height);
    settingWidget.setCamSize(width, height);
    morpoloyPro->setCamImageSize(width, height);
    setScaleSize(width, height);
}

/**
 * @brief Widget::setKickMenuList
 */
void Widget::setKickMenuList()
{
    menu->addAction("out");
}

void Widget::setDestStreamInfo(TcpConnection *_connection)
{
    QString   *destAddress;
    int        serverNumber;

    /*get server's number for streaming*/
    serverNumber = tcpConnection->getToken().toInt();
    destAddress = new QString[serverNumber];

    /*get server's addresses*/
    for(int i=0;i<serverNumber;i++)
    {
        destAddress[i]=tcpConnection->getToken();
        qDebug("Server Num (%d)'s ip adress...",i);
        qDebug()<<destAddress[i];
    }

    audioDev->setDestInfo(destAddress, serverNumber);
    musicPlayer->setDestInfo(destAddress, serverNumber);
    imgSocket->setDestInfo(UDP_IMAGE_PORT, destAddress, serverNumber);

}

/**
 * @brief Widget::activeTab
 * @param tabNum
 */
void Widget::activeTab(int tabNum)
{
    /* tabNum == 1 -> cameraTab   will be active
     * tabNum == 2 -> roominfoTab will be active
     * tabNum == 3 -> chattingTab will be active*/

    ui->CameraTab->setVisible  (tabNum == DEVICE_TAB);
    ui->RoonInfoTab->setVisible(tabNum == ROOMINFOR_TAB);
    ui->ChattingTab->setVisible(tabNum == CHATTING_TAB);
}
/**
 * @brief Widget::setBroadCasting
 */
void Widget::setBroadCasting()
{
    /*when broadcast is started,
     * this function will be called and init value,
     * UI for broadcast*/
    beforWSize = this->width();
    beforHSize = this->height();

    setRoomInfo(ui->roomName->text());
    setImageQuility(ui->QualityCombo->currentIndex());
    setBroadCastWidget(START);
    broadCastTimer->start(ONE_SECOND);

    emit sendBroadcastState(true);
}
/**
 * @brief Widget::startBroadCasting
 */
void Widget::startBroadCasting()
{
    QByteArray packet;

    packet = makePacket(packet,"title",ui->roomName->text());
    packet = makePacket(packet,"password",ui->roomPW->text());
    packet = makePacket(packet,"max_connect",ui->roomMaxPeople->currentText());

    tcpConnection->writePacket(Major_BroadcastSettring,BroadSet_Modify,packet);
    setBroadCasting();
}

void Widget::deleteMemory()
{
    /*when main widget is close,
     * do delete all of instants on memory*/

    delete cameraDevInfo;
    delete mainViewWidget;
    delete encoding;
    delete audioDevInfo;
    delete voicePainter;
    delete imgSocket;
    delete morpoloyPro;
    delete videoCodec;
    delete musicPlayer;
    delete menu;
    delete broadCastTimer;
    delete ui;
}

/**
 * @brief Widget::makePacket
 * @param packet
 * @param name
 * @param data
 * @return
 */
QByteArray Widget::makePacket(QByteArray packet, QString name, QString data)
{
    packet.append(name);
    packet.push_back('\0');
    packet.append(data);
    packet.push_back('\0');

    return packet;
}

/**
 * @brief Widget::makePacket
 * @param packet
 * @param name
 * @return
 */
QByteArray Widget::makePacket(QByteArray packet, QString data)
{
    packet.append(data);
    packet.push_back('\0');

    return packet;
}

void Widget::resetTimer()
{
    /*if videoTimer is active,
     * do stop active videoTimer*/
    if(videoTimer->isActive())
        stopVideoTimer(VIDEOTIMER_NUMBER);
}

/**
 * @brief Widget::requestBeforeRoomInfo
 */
void Widget::requestBeforeRoomInfo()
{
    tcpConnection->writePacket(Major_BroadcastSettring, BroadSet_Request_All);
}
/**
 * @brief Widget::mousePressEvent
 * @param event
 */
void Widget::mousePressEvent(QMouseEvent *event)
{
    windowHeight = this->height();
    windowWidth = this->width();

    clicked_x = event->x();
    clicked_y = event->y();

    isClick = true;

    if(clicked_x > this->width()-WINDOW_RESIZE_GAB)
    {
        if(clicked_y > this->height()-WINDOW_RESIZE_GAB)
            clicked = CLICK_VERTEX;

        else
            clicked = CLICK_X_EDGE;
    }

    else if(clicked_y > this->height()-WINDOW_RESIZE_GAB)
        clicked = CLICK_Y_EDGE;
}
/**
 * @brief Widget::mouseMoveEvent
 * @param event
 */
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(isClick == true)
    {
        if( clicked == CLICK_X_EDGE )
            resize(windowWidth-(clicked_x - event->x()),this->height());

        else if( clicked == CLICK_Y_EDGE)
            resize(this->width(),windowHeight-(clicked_y - event->y()));

        else if( clicked == CLICK_VERTEX )
            resize(windowWidth-(clicked_x - event->x()),windowHeight-(clicked_y - event->y()));

        else
            this->move(this->x()-clicked_x + ((event->x())),this->y()-clicked_y+((event->y())));
    }
    else
    {
        if(event->x() > this->width()-WINDOW_RESIZE_GAB)
        {

            if(event->y() > this->height()-WINDOW_RESIZE_GAB)
                this->setCursor(Qt::SizeFDiagCursor);

            else
                this->setCursor(Qt::SizeHorCursor);
        }

        else if(event->y() > this->height()-WINDOW_RESIZE_GAB)
            this->setCursor(Qt::SizeVerCursor);

        else
            this->setCursor(Qt::ArrowCursor);
    }
}

/**
 * @brief Widget::mouseReleaseEvent
 * @param event
 */
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    isClick = false;
    clicked = ClICK_MIDDLE;

    this->setCursor(Qt::ArrowCursor);
}
