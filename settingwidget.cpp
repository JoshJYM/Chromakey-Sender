#include "settingwidget.h"
#include "ui_settingwidget.h"
#include <QFileDialog>

SettingWidget::SettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    initViewImage();
    initConnect();
    initValue();
}

/**
 * @brief SettingWidget::~SettingWidget
 */
SettingWidget::~SettingWidget()
{
    delete ui;
    qDebug("delete SettingWidget...");
}

/**
 * @brief SettingWidget::initConnect
 */
void SettingWidget::initConnect()
{
    connect(ui->cancelMorBtn       ,SIGNAL(clicked()),SLOT(slotCancleMorpology()));
    connect(ui->applyMorpology     ,SIGNAL(clicked()),SLOT(slotApplyMorpoloy()));
    connect(ui->returnValueBtn     ,SIGNAL(clicked()),this,SLOT(slotPopValue()));
    connect(ui->addBackgroudImgBtn ,SIGNAL(clicked()),SLOT(slotAddBackgroundImg()));
    connect(ui->applySettingImgBtn ,SIGNAL(clicked()),SLOT(slotApplySettingImg()));
    connect(ui->initValueBtn       ,SIGNAL(clicked()),SLOT(slotinitMorpoloyVal()));
    connect(ui->addAdverImgBtn     ,SIGNAL(clicked()),SLOT(slotAddAdverImg()));
    connect(ui->cancelMorImgBtn    ,SIGNAL(clicked()),SLOT(slotCancleMorpology()));

    connect(ui->maskSize           ,SIGNAL(valueChanged(int)),oneFrameView,
            SLOT(slotGetMaskSize(int)));
    connect(ui->backgroundImgList, SIGNAL(clicked(QModelIndex)),
            SLOT(slotChagebackImage(QModelIndex)));
    connect(ui->adverImgList,      SIGNAL(clicked(QModelIndex)),
            SLOT(slotChageAdverImage(QModelIndex)));

    connect(this,SIGNAL(sendGrabProcessData(int*,int*,int*,int*,int*,int*)),imgprocess,
            SLOT(slotSetThresholdData(int*,int*,int*,int*,int*,int*)));
    connect(oneFrameView,SIGNAL(sendClickPosition(int,int)),SLOT(slotFindMM(int,int)));
}

/**
 * @brief SettingWidget::initMaxMinVal
 */
void SettingWidget::initMaxMinVal()
{
    for( int i=0; i<PARTITION_NUM; i++ )
    {
        maxRed[i]   = 0;
        maxGreen[i] = 0;
        maxBlue[i]  = 0;

        minRed[i]   = 255;
        minGreen[i] = 255;
        minBlue[i]  = 255;

        stackTop[i] = -1;
    }

}

/**
 * @brief SettingWidget::initValue
 */
void SettingWidget::initValue()
{
    QPixmap previewImage;

    backgroundModel = new QStringListModel(this);
    backgroundList  = new QStringList();

    adverModel = new QStringListModel(this);
    adverList  = new QStringList();

    adverImage   = new QImage();

    maxRed = new int[PARTITION_NUM];
    minRed = new int[PARTITION_NUM];

    maxBlue = new int[PARTITION_NUM];
    minBlue = new int[PARTITION_NUM];

    maxGreen = new int[PARTITION_NUM];
    minGreen = new int[PARTITION_NUM];


    backgroundImgCnt = 0;
    adverImgCnt      = 0;
    part             = 0;

    initMaxMinVal();
    for( int i=0; i<PARTITION_NUM; i++ )
        pushValue(maxRed[i],minRed[i],maxGreen[i],minGreen[i],maxBlue[i],minBlue[i],i);

    /*init mask size = 30*/
    ui->maskSize->setValue(DEFAULT_MASK_SIZE);

    /*init advertisement image's size
     * width  = 150
     * height = 100*/
    adverImgWidth  = DEFAULT_ADVERIMAGE_WIDTH;
    adverImgHeight = DEFAULT_ADVERIMAGE_HEIGHT;

    /*init list's default index*/
    adverImgIndex      = DEFAULT_LIST_INDEX;
    backgroundImgIndex = DEFAULT_LIST_INDEX;

    /*init to set previewImage file*/
    previewImage.load(DEFAULT_PREVIEWIMAGE);
    previewImage = previewImage.scaled(PREVIEWIMAGE_WIDTH,PREVIEWIMAGE_HEIGHT);

    ui->backgroundImgLb->setPixmap(previewImage);
    ui->adverImgLb->setPixmap(previewImage);

    setColorLabel(part);
}

/**
 * @brief SettingWidget::setCamSize
 * @param imgWidth
 * @param imgHeight
 */
void SettingWidget::setCamSize(int imgWidth, int imgHeight)
{
    this->imageWidth  = imgWidth;
    this->imageHeight = imgHeight;

      /*입력받는 카메라 이미지 크기에 맞게 크로마키 처리를 할경우
    sourceImg  = new QImage(imageWidth,imageHeight,QImage::Format_RGB888);
    targetImg  = new QImage(imageWidth,imageHeight,QImage::Format_RGB888);
    resultImg  = new QImage(imageWidth,imageHeight,QImage::Format_RGB888);
      */

      /*입력받는 카메라 이미지 크기에 무관하게 무조건 640 x 480으로 크로마키 처리를 할경우 */
    sourceImg  = new QImage(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT,QImage::Format_RGB888);
    targetImg  = new QImage(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT,QImage::Format_RGB888);
    resultImg  = new QImage(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT,QImage::Format_RGB888);
}

/**
 * @brief SettingWidget::slotApplySettingImg
 */
void SettingWidget::slotApplySettingImg()
{
    /*if there is no advertisement or background image,
     * do print error messgae*/
    if(backgroundImgCnt == 0 || adverImgCnt == 0 )
    {
        QPixmap erroPoint;
        erroPoint.load(":/res/images/errPoint.png");
        erroPoint = erroPoint.scaled(20,20);

        if(backgroundImgCnt == 0)
        {
            ui->errLb->setText("Please add background Image.");
            /*notice to sender that position where should check*/
            ui->backgroundP->setPixmap(erroPoint);
        }
        else
        {
            ui->errLb->setText("Please add advertisement Image.");
            ui->backgroundP->clear();
            /*notice to sender that position where should check*/
            ui->adverP->setPixmap(erroPoint);
        }
    }

    /*if there is at least one adevtisement and background image,
     * go to the next step*/
    else
    {
        /*리스트 클릭을 안한 상태에서 적용버튼을 클릭했을경우 예외처리*/
        if(ui->backgroundImgList->currentIndex().row()==-1)
            backgroundImgIndex = 0;
        else
            backgroundImgIndex = ui->backgroundImgList->currentIndex().row();

        if(ui->adverImgList->currentIndex().row()==-1)
            adverImgIndex = 0;
        else
            adverImgIndex = ui->adverImgList->currentIndex().row();

        backgroundImgFileName = backgroundList->at(backgroundImgIndex);
        adverImgFileName      = adverList->at(adverImgIndex);


        checkBackgrountType();
        if(backgroundType==IMAGE)
        {
            targetImg->load(backgroundImgFileName);
            *targetImg = targetImg->scaled(imageWidth,imageHeight);
            *targetImg = imgprocess->toQImageRGB888(*targetImg);

                /*입력받는 카메라 이미지 크기에 맞게 배경이미지를 스케일링하여 크로마키 영상처리에 사용될 이미지를 전송 */
           emit sendBackgroundImg(*targetImg);
        }
        else
        {
            videoCodec.openFile(backgroundImgFileName);
            videoCodec.seekNextFrame();
            videoCodec.getFrame(*targetImg);

            emit sendBackgroundImg(videoCodec);
        }

        /*active image proccessing tab*/
        setActiveTab(IMAGE_PROCCESSING_TAB);

        adverImage->load(adverImgFileName);
        *adverImage = adverImage->scaled(adverImgWidth,adverImgHeight);

                /*입력받는 카메라 이미지 크기에 무관하게 무조건 640 x 480으로 크로마키 처리를 할경우 */
        *targetImg = targetImg->scaled(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT);

        /*reset current error message label,
         * and notice position label*/
        ui->errLb->clear();
        ui->backgroundP->clear();
        ui->adverP->clear();
    }
}

/**
 * @brief SettingWidget::setActiveTab
 * @param tabNum
 */
void SettingWidget::setActiveTab(int tabNum)
{
    ui->ImageSettingTab->setTabEnabled(0,tabNum == IMAGE_SET_TAB);
    ui->ImageSettingTab->setTabEnabled(1,tabNum == IMAGE_PROCCESSING_TAB);
}

/**
 * @brief SettingWidget::slotAddBackgroundImg
 */
void SettingWidget::slotAddBackgroundImg()
{
    backgroundImgFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "//",tr("Images (*.png *.xpm *.jpg *.mp4 *.avi *.wma *.wmv *.mov)"));
    QImage *backgroundImg = new QImage();

    /*When file's name is not NULL*/
    if(backgroundImgFileName.size() != 0)
    {
        backgroundList->operator <<(backgroundImgFileName);
        backgroundModel->setStringList(*backgroundList);

        checkBackgrountType();
        if(backgroundType==IMAGE)
            backgroundImg->load(backgroundImgFileName);

        else
        {
            videoCodec.openFile(backgroundImgFileName);
            videoCodec.seekMs(MOVIE_PREVIEWIMAGE_SEEKTIME);
            videoCodec.getFrame(*backgroundImg);
        }

        *backgroundImg = backgroundImg->scaled(PREVIEWIMAGE_WIDTH,PREVIEWIMAGE_HEIGHT);

        ui->backgroundImgList->setModel(backgroundModel);
        ui->backgroundImgLb->setPixmap(QPixmap::fromImage(*backgroundImg));

        backgroundImgCnt++;
    }

    delete backgroundImg;
}

/**
 * @brief SettingWidget::slotAddAdverImg
 */
void SettingWidget::slotAddAdverImg()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "//",tr("Images (*.png *.xpm *.jpg)"));
    /*When file's name is not NULL*/
    if(fileName.size() != 0)
    {
        QPixmap *adverImg = new QPixmap(fileName);

        *adverImg   = adverImg->scaled(PREVIEWIMAGE_WIDTH,PREVIEWIMAGE_HEIGHT);
        *adverImage = adverImg->toImage();

        adverList->operator <<(fileName);
        adverModel->setStringList(*adverList);

        ui->adverImgList->setModel(adverModel);
        ui->adverImgLb->setPixmap(*adverImg);

        adverImgCnt ++;

        delete adverImg;
    }
}

/**
 * @brief SettingWidget::slotChagebackImage
 * @param list
 */
void SettingWidget::slotChagebackImage(QModelIndex list)
{
    /*get clicked index number*/
    int clickedBackImgNum = list.row();

    /*get clicked image file name*/
    QString backFileName = backgroundList->at(clickedBackImgNum);
    QPixmap clickedBackImg;

    /*get clicked background image*/
    clickedBackImg.load(backFileName);
    clickedBackImg = clickedBackImg.scaled(PREVIEWIMAGE_WIDTH,PREVIEWIMAGE_HEIGHT);

    /*update image at label*/
    ui->backgroundImgLb->setPixmap(clickedBackImg);
}

/**
 * @brief SettingWidget::slotChageAdverImage
 * @param list
 */
void SettingWidget::slotChageAdverImage(QModelIndex list)
{
    /*get clicked index number*/
    int clickedAdverImgNum = list.row();

    /*get clicked image file name*/
    QString adverFileName = adverList->at(clickedAdverImgNum);
    QPixmap clickedAdverImg;

    /*get clicked advertisement image*/
    clickedAdverImg.load(adverFileName);
    clickedAdverImg = clickedAdverImg.scaled(PREVIEWIMAGE_WIDTH,PREVIEWIMAGE_HEIGHT);

    /*update image at label*/
    ui->adverImgLb->setPixmap(clickedAdverImg);
}

/**
 * @brief SettingWidget::slotShowBeforeAdverImg
 */
void SettingWidget::slotShowBeforeAdverImg()
{
    if(adverImgIndex>0)
        adverImgIndex--;

    else if(adverImgIndex==0)
        adverImgIndex=adverList->size()-1;

    adverImgFileName = adverList->at(adverImgIndex);
    adverImage->load(adverImgFileName);
    *adverImage = adverImage->scaled(adverImgWidth,adverImgHeight);

    emit sendAdverImg(*adverImage);
}

/**
 * @brief SettingWidget::slotShowBeforeSourceImg
 */
void SettingWidget::slotShowBeforeSourceImg()
{
    if(backgroundImgIndex>0)
        backgroundImgIndex--;

    else if(backgroundImgIndex==0)
        backgroundImgIndex=backgroundList->size()-1;

    backgroundImgFileName = backgroundList->at(backgroundImgIndex);
    checkBackgrountType();

    if(backgroundType==IMAGE)
    {
        targetImg->load(backgroundImgFileName);
        *targetImg = targetImg->scaled(this->imageWidth,this->imageHeight);
        *targetImg = imgprocess->toQImageRGB888(*targetImg);

            /*입력받는 카메라 이미지 크기에 맞게 배경이미지를 스케일링하여 크로마키 영상처리에 사용될 이미지를 전송 */
        emit sendBackgroundImg(*targetImg);
    }
    else
    {
        videoCodec.openFile(backgroundImgFileName);
        emit sendBackgroundImg(videoCodec);
    }
}

/**
 * @brief SettingWidget::slotShowNextSourceImg
 */
void SettingWidget::slotShowNextSourceImg()
{
    if(backgroundImgIndex == backgroundList->size()-1)
        backgroundImgIndex = 0;

    else
       backgroundImgIndex++;

    backgroundImgFileName = backgroundList->at(backgroundImgIndex);
    checkBackgrountType();

    if(backgroundType==IMAGE)
    {
        targetImg->load(backgroundImgFileName);
        *targetImg = targetImg->scaled(this->imageWidth,this->imageHeight);
        *targetImg = imgprocess->toQImageRGB888(*targetImg);

            /*입력받는 카메라 이미지 크기에 맞게 배경이미지를 스케일링하여 크로마키 영상처리에 사용될 이미지를 전송 */
       emit sendBackgroundImg(*targetImg);
    }
    else
    {
        videoCodec.openFile(backgroundImgFileName);
        emit sendBackgroundImg(videoCodec);
    }
}

/**
 * @brief SettingWidget::slotShowNextAdverImg
 */
void SettingWidget::slotShowNextAdverImg()
{
    if(adverImgIndex == adverList->size()-1)
        adverImgIndex = 0;

    else
        adverImgIndex++;

    adverImgFileName = adverList->at(adverImgIndex);
    checkBackgrountType();

    adverImage->load(adverImgFileName);
    *adverImage = adverImage->scaled(adverImgWidth,adverImgHeight);

    emit sendAdverImg(*adverImage);
}


/**
 * @brief SettingWidget::slotGetAdverInfo
 * @param w
 * @param h
 */
void SettingWidget::slotGetAdverInfo(int w, int h)
{
    /*when sender change advertisement
     * image's size, this slot will be
     * called and image will be resized*/
    adverImgWidth  = w;
    adverImgHeight = h;
}

/**
 * @brief SettingWidget::slotinitMorpoloyVal
 */
void SettingWidget::slotinitMorpoloyVal()
{
    initMaxMinVal();
    emit sendGrabProcessData(maxRed,minRed,maxGreen,
                             minGreen,maxBlue,minBlue);
    setColorLabel(part);
}


void SettingWidget::setColorLabel(int part)
{
    QImage *color_Max = new QImage(30,30,QImage::Format_RGB888);
    QImage *color_Min = new QImage(30,30,QImage::Format_RGB888);

    color_Max->load(":/res/images/max.png");
    color_Min->load(":/res/images/max.png");

    uchar *maxData = color_Max->bits();
    uchar *minData = color_Min->bits();

    int inc = 0;
    /*set current max, min color on QImage*/
    for( int i=0; i < 900 ; i ++ )
    {
        maxData[inc+0] = maxRed[part];
        maxData[inc+1] = maxGreen[part];
        maxData[inc+2] = maxBlue[part];

        minData[inc+0] = minRed[part];
        minData[inc+1] = minGreen[part];
        minData[inc+2] = minBlue[part];

        inc = inc+4;
    }

    /*load current max, min color on label*/
    ui->colorLb->setPixmap(QPixmap::fromImage(*color_Max));
    ui->colorLb_min->setPixmap(QPixmap::fromImage(*color_Min));

    delete color_Max;
    delete color_Min;

}

/**
 * @brief SettingWidget::pushValue
 * @param rMax
 * @param rMin
 * @param gMax
 * @param gMin
 * @param bMax
 * @param bMin
 */
void SettingWidget::pushValue(int rMax, int rMin, int gMax, int gMin,
                              int bMax, int bMin, int part)
{
    stackTop[part]++;

    if(stackTop[part] == MAX_STACK_SIZE)
    {
        for(int i=0;i<MAX_STACK_SIZE;i++)
        {
            setStackVal(redMaxStack[part][stackTop[part]+1],redMinStack[part][stackTop[part]+1]
                       ,greenMaxStack[part][stackTop[part]+1],greenMinStack[part][stackTop[part]+1]
                       ,blueMaxStack[part][stackTop[part]+1],blueMinStack[part][stackTop[part]+1],part);
        }
        stackTop[part]--;
        setStackVal(rMax,rMin,gMax,gMin,bMax,bMin,part);
    }

    else if(stackTop[part] == 0)
        setStackVal(0,255,0,255,0,255,part);

    else
    {
        if((redMaxStack  [part][stackTop[part]-1] != rMax) ||
           (greenMaxStack[part][stackTop[part]-1] != gMax) ||
           (blueMaxStack [part][stackTop[part]-1] != bMax) ||
           (redMinStack  [part][stackTop[part]-1] != rMin) ||
           (greenMinStack[part][stackTop[part]-1] != gMin) ||
           (blueMinStack [part][stackTop[part]-1] != bMin))
        {
            setStackVal(rMax,rMin,gMax,gMin,bMax,bMin,part);
        }

        else
            stackTop[part]--;
    }
}

/**
 * @brief SettingWidget::slotPopValue
 */
void SettingWidget::slotPopValue()
{
    if(stackTop[part]>-1)
    {
        maxRed[part] = redMaxStack[part][stackTop[part]];
        minRed[part] = redMinStack[part][stackTop[part]];


        maxGreen[part] = greenMaxStack[part][stackTop[part]];
        minGreen[part] = greenMinStack[part][stackTop[part]];

        maxBlue[part] = blueMaxStack[part][stackTop[part]];
        minBlue[part] = blueMinStack[part][stackTop[part]];

        stackTop[part]--;
    }

    emit sendGrabProcessData(maxRed,minRed,maxGreen,
                             minGreen,maxBlue,minBlue);
}

/**
 * @brief SettingWidget::slotCancleMorpology
 */
void SettingWidget::slotCancleMorpology()
{
    initMaxMinVal();

    /*active image setting tab*/
    setActiveTab(IMAGE_SET_TAB);

    emit cancelMorpology();
    this->close();
}

/**
 * @brief SettingWidget::slotApplyMorpoloy
 */
void SettingWidget::slotApplyMorpoloy()
{
    /*active image setting tab*/
    setActiveTab(IMAGE_SET_TAB);

    emit applyMorpology();
    emit sendAdverImg(*adverImage);
    emit sendProcessData(maxRed,minRed,maxGreen,
                         minGreen,maxBlue,minBlue);

    this->close();
}

/**
 * @brief SettingWidget::initViewImage
 */
void SettingWidget::initViewImage()
{
    oneFrameView = new AFrameGLView(this);
    imgprocess   = new MorPologyProcessing(this);

    oneFrameView->setMinimumSize(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT);
    oneFrameView->setMaximumSize(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT);

    /*active image setting tab*/
    setActiveTab(IMAGE_SET_TAB);

    ui->ImageProcessingViewLay->addWidget(oneFrameView);
}

/**
 * @brief SettingWidget::getImageFrame
 * @param frame
 */
void SettingWidget::getImageFrame(QImage &frame)
{
    frame        = frame.scaled(PROCCESSIMAGE_WIDTH,PROCCESSIMAGE_HEIGHT);
    oneFrameData = frame.bits();

    *sourceImg = frame;

    imgprocess  ->chromakeyProcessMorphology(*sourceImg,*targetImg,*resultImg);
    oneFrameView->ImageLoad(QPixmap::fromImage(*resultImg));
}

/**
 * @brief SettingWidget::slotFindMM
 * @param x
 * @param y
 */
void SettingWidget::slotFindMM(int x, int y)
{
    maskSize = ui->maskSize->value();

    if(x+maskSize/2 <PROCCESSIMAGE_WIDTH && x-maskSize/2 > -1 &&
       y+maskSize/2<PROCCESSIMAGE_HEIGHT && y-maskSize/2 > -1 )
    {
        /*Find maximun RGB value each sied*/
        for (int i = y-maskSize/2 ; i < y+maskSize/2 ; i ++ )
        {
            for (int n = x-maskSize/2 ; n < x+maskSize/2 ; n++)
            {
                if(x<320 && y <240)                 part=0;
                else if(x>=320  && y <240)          part=1;
                else if(x<320 && y<640 && y >=240)  part=2;
                else                                part=3;

                int index = (i*PROCCESSIMAGE_WIDTH*FORMAT_RGB888)+((n)*FORMAT_RGB888);

                if( oneFrameData[index] > maxRed[part] )
                    maxRed[part] = oneFrameData[index];

                if( oneFrameData[index+1] > maxGreen[part] )
                    maxGreen[part] = oneFrameData[index+1];

                if( oneFrameData[index+2] > maxBlue[part] )
                    maxBlue[part] = oneFrameData[index+2];


                /*Find minimun RGB value each sied*/
                if( oneFrameData[index] < minRed[part] )
                    minRed[part] = oneFrameData[index];

                if( oneFrameData[index+1] < minGreen[part] )
                    minGreen[part] = oneFrameData[index+1];

                if( oneFrameData[index+2] < minBlue[part] )
                    minBlue[part] = oneFrameData[index+2];
            }
        }
    }
    pushValue(maxRed[part],minRed[part],maxGreen[part],
              minGreen[part],maxBlue[part],minBlue[part], part);

    emit sendGrabProcessData(maxRed,minRed,maxGreen,
                             minGreen,maxBlue,minBlue);
    setColorLabel(part);
}

/**
 * @brief SettingWidget::checkBackgrountType
 */
void SettingWidget::checkBackgrountType()
{
    for(int i=0;i<backgroundImgFileName.size();i++)
    {
        if(backgroundImgFileName.at(i)==".")
        {
            /*if file first alphabet is "j,J,p,P",
             * backgroundType will be image or not
             * backgroundType will be MOVIE*/
            if(backgroundImgFileName.at(i+1) == "J" || backgroundImgFileName.at(i+1) == "j"
            || backgroundImgFileName.at(i+1) == "P" || backgroundImgFileName.at(i+1) == "p")
                backgroundType = IMAGE;
            else
                backgroundType = MOVIE;
        }
    }
}

/**
 * @brief SettingWidget::setStackVal
 * @param rMax
 * @param rMin
 * @param gMax
 * @param gMin
 * @param bMax
 * @param bMin
 */
void SettingWidget::setStackVal(int rMax, int rMin, int gMax,
                                int gMin, int bMax, int bMin, int part)
{
    redMaxStack[part][stackTop[part]]=rMax;
    redMinStack[part][stackTop[part]]=rMin;

    greenMaxStack[part][stackTop[part]]=gMax;
    greenMinStack[part][stackTop[part]]=gMin;

    blueMaxStack[part][stackTop[part]]=bMax;
    blueMinStack[part][stackTop[part]]=bMin;
}
