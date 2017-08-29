#include "imageglview.h"
#include <QMatrix>
#include <QLabel>

ImageGLView::ImageGLView(QObject *parent)
{
    initValue();
    setMouseTracking(true);
}

ImageGLView::~ImageGLView()
{
    qDebug("delete ImageGLView...");
    delete adverSettWidget;
}

/**
 * @brief ImageGLView::initValue
 */
void ImageGLView::initValue()
{
    adverSettWidget = new SettingAdeverImgWidget();

    adverSettWidget->setCurrentAdverImgSize(DEFAULT_ADVERIMAGE_WIDTH,
                                            DEFAULT_ADVERIMAGE_HEIGHT);

    /*load source image*/
    sourceImage.load(":/res/images/Logo.png");
    adverNext.load(":/res/images/adverNext.png");
    adverBefore.load(":/res/images/adverBefore.png");
    settingAdverImg.load(":/res/images/setting.png");
    exitAdverImg.load(":/res/images/cancel.png");
    nextSourceImg.load(":/res/images/sourceNext.png");
    beforeSourceImg.load(":/res/images/sourceBefore.png");
    pauseImg.load(":/res/images/capacytyBack.png");
    restartImg.load(":/res/images/replay.png");

    /*rescale source image*/
    adverBefore      = adverBefore.scaled(CONBTN_SIZE,CONBTN_SIZE);
    adverNext        = adverNext.scaled(CONBTN_SIZE,CONBTN_SIZE);
    settingAdverImg  = settingAdverImg.scaled(ADVER_SET_CONBTN_SIZE,ADVER_SET_CONBTN_SIZE);
    exitAdverImg     = exitAdverImg.scaled(ADVER_SET_CONBTN_SIZE,ADVER_SET_CONBTN_SIZE);
    nextSourceImg    = nextSourceImg.scaled(ADVER_SET_CONBTN_SIZE,ADVER_SET_CONBTN_SIZE);
    beforeSourceImg  = beforeSourceImg.scaled(ADVER_SET_CONBTN_SIZE,ADVER_SET_CONBTN_SIZE);

    adverImg_x = DEFAULT_ADVERIMG_START_X;
    adverImg_y = DEFAULT_ADVERIMG_START_Y;

    urlAddress = DEFAULT_URL_ADDRESS;

    startX = 0;

    adverBtn_b_Position=0;
    adverBtn_n_Position=0;

    captionX = this->width();
    captionY = this->height();
    boldState = false;

    mousePosition = ON_RESULT_IMAGE;

    isAdverImg    = false;
    isOnAdverImg  = false;
    isCaption     = false;
    isPause       = false;

    stopCaptionX  = ( this->width() / 2 );
    stopCaptionY  = ( this->height() - 60 );

    connect(adverSettWidget,SIGNAL(sendSettingVal(int,int)),
            SLOT(slotGetAdverInfo(int,int)));
    connect(adverSettWidget,SIGNAL(sendAdverUrlInfo(QString)),
            SLOT(slotGetUrlAddress(QString)));
}

/**
 * @brief ImageGLView::slotGetAdverInfo
 * @param w
 * @param h
 */
void ImageGLView::slotGetAdverInfo(int w, int h)
{
    adverImage = adverImage.scaled(w,h);
    adverImg_w = w;
    adverImg_h = h;

    emit sendSettingVal(w,h);
    this->repaint();
}

/**
 * @brief ImageGLView::slotFullScreenView
 */
void ImageGLView::slotFullScreenView()
{
    if(this->isFullScreen())
    {
        setWindowFlags(Qt::Widget);
        showNormal();
    }
    else
    {
        setWindowFlags(Qt::Window);
        showFullScreen();
    }
}

void ImageGLView::slotPauseBroadCast(bool state)
{
    /*set current broadcasting state*/
    isPause = !state;

    /*if current broadcasting state is true,
     * do repaint*/
    if(isPause)
    {
        isPause = true;
        this->repaint();
    }
}

/**
 * @brief ImageGLView::slotGetUrlAddress
 * @param url
 */
void ImageGLView::slotGetUrlAddress(QString url)
{
    urlAddress = url;
    qDebug("get advertisement url address");
}

/**
 * @brief ImageGLView::ImageLoad
 * @param image
 */
void ImageGLView::ImageLoad(const QPixmap &image)
{
    this->sourceImage = image;
    this->repaint();
}

/**
 * @brief ImageGLView::cutFixedImage
 * @param img
 * @param startX
 * @param startY
 * @param width
 * @param height
 * @return
 */
QImage ImageGLView::cutFixedImage(QImage img, int startX, int startY, int width, int height)
{
    /*cuttedImage is result image to send to client*/
    QImage cuttedImage = QImage(width,height,QImage::Format_RGBX8888);
    uchar *beforeImgData;
    uchar *cuttedImgData;

    cuttedImgData = cuttedImage.bits();
    beforeImgData = img.bits();

    /*for before image data index*/
    int index_b;
    /*for result image data(cuttedImage) index*/
    int index_c;

    int beforeWidth = img.width();

    /*get just picture part(no black part image)*/
    for(int i = startY, m = 0 ; m<height ; i++, m++)
    {
        for(int j = startX, n = 0 ; n<width ; j++, n++)
        {
            index_b = (j*4)+(beforeWidth*i*4);
            index_c = (n*4)+(width*m*4);

            cuttedImgData[index_c+0] = beforeImgData[index_b+0];
            cuttedImgData[index_c+1] = beforeImgData[index_b+1];
            cuttedImgData[index_c+2] = beforeImgData[index_b+2];
        }
    }
    return cuttedImage;
}

/**
 * @brief ImageGLView::resizeEvent
 * @param eventW
 */
void ImageGLView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    sourceBtn_b_Position = this->window()->height();
    sourceBtn_n_Position = this->window()->height();
}

/**
 * @brief ImageGLView::slotGetAdverImg
 * @param image
 */
void ImageGLView::slotGetAdverImg(QImage &image)
{
    adverImage = image;
    isAdverImg = true;

    adverImg_w = image.width();
    adverImg_h = image.height();
}

/**
 * @brief ImageGLView::slotInitAdverImg
 */
void ImageGLView::slotInitAdverImg()
{
    isAdverImg = false;
}

void ImageGLView::slotGetCaptionBold(bool state)
{
    qDebug("chage caption bold");
    boldState = state;
}

/**
 * @brief ImageGLView::paintEvent
 * @param event
 */
void ImageGLView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    this->clearMask();

    QPainter painter;

    int endX;
    int endY;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painterWidth  = painter.window().width();
    painterHeight = painter.window().height();



    painter.fillRect(0, 0, painterWidth, painterHeight, Qt::black);

    if( painterWidth/4*3 < painterHeight )
    {
        startX = 0;
        endX   = painterWidth;
        startY = (painterHeight-(painterWidth/4*3))/2;
        endY   = painterHeight-startY;
    }
    else
    {
        startX = (painterWidth-painterHeight/3*4)/2;
        endX   = painterWidth-startX;
        startY = 0;
        endY   = painterHeight;
    }

    /*get advertisement's start x,y position*/
    adverImg_startX = adverImg_x + startX;
    adverImg_startY = adverImg_y + startY;

    QRect rectDrawArea(startX, startY, endX-startX, endY-startY);
    sourceImage.scaled(640, 480, Qt::KeepAspectRatio);
    painter.drawPixmap(rectDrawArea, sourceImage);

    if(isCaption)
    {
        /*if sender use caption function
         *,draw caption text on result screen
         * follow caption animation*/

        QFont font = painter.font();

        font.setPointSize(captionFontSize);
        font.setBold(boldState);

        painter.setFont(font);
        painter.setPen(captionColor);


        switch(animation)
        {
        case STOP:
            painter.drawText(stopCaptionX, stopCaptionY, captionText);
            break;

        case RIGHT_TO_LEFT:
            painter.drawText(captionX ,captionY,captionText);
            if(captionX ==(-captionWidth))
                captionX = this->width();

            captionX--;
            break;

        case LEFT_TO_RIGHT:
            painter.drawText(captionX,captionY,captionText);
            if(captionX == painterWidth)
                captionX = -captionWidth;

            captionX++;
            break;

        case UP_TO_DOWN:
            painter.drawText(captionX,captionY,captionText);
            if(captionY == painterHeight+captionHeight)
                captionY = -captionHeight;

            captionY++;
            break;

        case DOWN_TO_UP:
            painter.drawText(captionX,captionY,captionText);
            if(captionY == (-captionHeight))
                captionY = this->height()+captionHeight;

            captionY--;
            break;
        }
    }

    if(isAdverImg)
    {
        /*if sender use advertisement image,
         * draw advertisement image on result screen*/
        painter.drawImage(adverImg_startX,adverImg_startY,adverImage);

        beforeImage = this->grabFrameBuffer();
        beforeImage = cutFixedImage(beforeImage,startX,startY,
                                    endX-startX, endY-startY);

        if(isOnAdverImg)
        {
            /*if sender use advertisement image
             * and mouse's position is on adverimg,
             * draw advertisement control button*/
            adverNextBtn_x   = adverImg_startX + adverImage.width()/2 + CONBTN_POSITION_X;
            adverBeforeBtn_x = adverImg_startX + adverImage.width()/2 - CONBTN_POSITION_X - CONBTN_SIZE;
            setBtn_x         = adverImg_startX + adverImage.width()   - ADVER_SET_CONBTN_SIZE;

            adverNextBtn_y   = adverImg_startY + adverImage.height() - CONBTN_SIZE;
            adverBeforeBtn_y = adverImg_startY + adverImage.height() - CONBTN_SIZE;
            setBtn_y         = adverImg_startY;

            /*draw advertisement next, after button*/
            painter.drawImage(adverBtn_n_Position , adverNextBtn_y   , adverNext);
            painter.drawImage(adverBtn_b_Position , adverBeforeBtn_y , adverBefore);

            painter.drawImage(adverImg_startX  , adverImg_startY  ,exitAdverImg);
            painter.drawImage(setBtn_x         , setBtn_y         , settingAdverImg);

            adverBtn_b_Position = buttonAnimation(adverBtn_b_Position,adverBeforeBtn_x,
                                                  ABUTTON_ANIMATION_SPEED,ANIMATION_DIRECT_UP);
            adverBtn_n_Position = buttonAnimation(adverBtn_n_Position,adverNextBtn_x,
                                                  ABUTTON_ANIMATION_SPEED,ANIMATION_DIRECT_DOWN);
        }

        else if(!isOnAdverImg && mousePosition == ON_RESULT_IMAGE)
        {
            /*if sender's mouse position is on result
             * image screen ,draw source image control
             * button */
            sourceNextBtn_x   = startX + endX/2 + CONBTN_POSITION_X;
            sourceNextBtn_y   = startY + endY   - ADVER_SET_CONBTN_SIZE - GAP;

            sourceBeforeBtn_x = startX + endX/2 - CONBTN_POSITION_X - ADVER_SET_CONBTN_SIZE;
            sourceBeforeBtn_y = startY + endY   - ADVER_SET_CONBTN_SIZE - GAP;

            painter.drawImage(sourceNextBtn_x   , sourceBtn_n_Position , nextSourceImg);
            painter.drawImage(sourceBeforeBtn_x , sourceBtn_b_Position , beforeSourceImg);

            sourceBtn_b_Position = buttonAnimation(sourceBtn_b_Position,sourceNextBtn_y,
                                                   SBUTTON_ANIMATION_SPEED,ANIMATION_DIRECT_DOWN);
            sourceBtn_n_Position = buttonAnimation(sourceBtn_n_Position,sourceNextBtn_y,
                                                   SBUTTON_ANIMATION_SPEED,ANIMATION_DIRECT_DOWN);
        }
    }

    if(isPause)
    {
        int pauseX,pauseY;
        pauseX = painter.window().width()/2  - restartImg.width()/2;
        pauseY = painter.window().height()/2 - restartImg.height()/2;

        pauseImg = pauseImg.scaled(painter.window().width(),
                                   painter.window().height());
        painter.drawImage(0,0,pauseImg);
        painter.drawImage(pauseX, pauseY, restartImg);
    }
}

/**
 * @brief ImageGLView::slotGetCaptionInfo
 * @param animation
 * @param size
 * @param color
 * @param fontType
 * @param context
 */
void ImageGLView::slotGetCaptionInfo(int animation, int size,
                                     int color, QString context)
{
     setCaptionAnimation(animation);
     setCaptionFontSize(size);
     setCaptionColor(color);
     setCaptionContext(context);
}

/**
 * @brief ImageGLView::slotGetCaptionState
 * @param isCaption
 */
void ImageGLView::slotGetCaptionState(bool isCaption)
{
    /* get caption's state
     * if caption is, then state is true
     * if caption is not, then state is false*/
    this->isCaption = isCaption;
    this->clearMask();
    this->repaint();
}

/**
 * @brief ImageGLView::setCaptionFontSize
 * @param fontSize
 */
void ImageGLView::setCaptionFontSize(int fontSize)
{
    /*get caption's font size*/
    this->captionFontSize = fontSize;
}

/**
 * @brief ImageGLView::buttonAnimation
 * @param position
 * @param speed
 * @param direct
 * @return
 */
int ImageGLView::buttonAnimation(int startPosition,int endPosition ,int speed, int direct)
{
    /*this function is for button animation's postion*/

    /*until startPostion and endPosition is almost same*/
    if(abs(startPosition - endPosition) > speed)
    {
        if(direct == ANIMATION_DIRECT_DOWN)
            startPosition = startPosition - speed;

        else if(direct == ANIMATION_DIRECT_UP)
            startPosition = startPosition + speed;
    }
    return startPosition;
}

/**
 * @brief ImageGLView::setCaptionAnimation
 * @param animation
 */
void ImageGLView::setCaptionAnimation(int animation)
{
    /*get caption's animation type*/
    this->animation = animation;
}

/**
 * @brief ImageGLView::setCaptionColor
 * @param color
 */
void ImageGLView::setCaptionColor(int color)
{
    /*get caption's font color*/
    this->captionColor = color;
}

/**
 * @brief ImageGLView::setCaptionContext
 * @param text
 */
void ImageGLView::setCaptionContext(QString text)
{
    /*get caption's text context*/
    this->captionText = text;

    /*get caption's size information*/
    captionWidth  = captionFontSize*text.size();
    captionHeight = captionFontSize;
}

/**
 * @brief ImageGLView::mousePressEvent
 * @param event
 */
void ImageGLView::mousePressEvent(QMouseEvent *event)
{
    /*get advertisement image's end x,y position*/
    int adverImage_w = adverImg_startX + adverImg_w;
    int adverImage_h = adverImg_startY + adverImg_h;

    /*get before button's end x,y position for advertisement*/
    int beforeAdverBtn_w = adverBeforeBtn_x + CONBTN_SIZE;
    int beforeAdverBtn_h = adverBeforeBtn_y + CONBTN_SIZE;

    /*get after button's end x,y position for advertisement*/
    int afterAdverBtn_w = adverNextBtn_x + CONBTN_SIZE;
    int afterAdverBtn_h = adverNextBtn_y + CONBTN_SIZE;

    /*get setting button's end x,y position for advertisement*/
    int setAdverBtn_w = setBtn_x + CONBTN_SIZE;
    int setAdverBtn_h = setBtn_y + CONBTN_SIZE;

    /*get before button's end x,y position for source image*/
    int beforeSourceBtn_w = sourceBeforeBtn_x + CONBTN_SIZE;
    int beforeSourceBtn_h = sourceBeforeBtn_y + CONBTN_SIZE;

    /*get after button's end x,y position for source image*/
    int afterSourceBtn_w = sourceNextBtn_x + CONBTN_SIZE;
    int afterSourceBtn_h = sourceNextBtn_y + CONBTN_SIZE;

    /*get close button's end x,y position for advertisement*/
    int closeAdverBtn_w = adverImg_startX + CONBTN_SIZE;
    int closeAdverBtn_h = adverImg_startY + CONBTN_SIZE;

    int pauseX,pauseY;

    pauseX = painterWidth/2  - restartImg.width()/2;
    pauseY = painterHeight/2 - restartImg.height()/2;


    if(event->x() > adverImg_startX && event->y() > adverImg_startY &&
       event->x() < adverImage_w    && event->y() < adverImage_h)
    {
        /*when sender clicked advertisement image*/

        /*when sender clicked before advertisement button*/
        if(event->x() > adverBeforeBtn_x && event->x() < beforeAdverBtn_w
        && event->y() > adverBeforeBtn_y && event->y() < beforeAdverBtn_h )
            emit clickBeforeAdver();

        /*when sender clicked next advertisement button*/
        else if(event->x() > adverNextBtn_x && event->x() < afterAdverBtn_w
             && event->y() > adverNextBtn_y && event->y() < afterAdverBtn_h )
            emit clickNextAdver();

        /*when sender clicked setting advertisement button*/
        else if(event->x() > setBtn_x && event->x() < setAdverBtn_w
             && event->y() > setBtn_y && event->y() < setAdverBtn_h )
            adverSettWidget->show();

        /*when sender clicked close advertisement button*/
        else if((event->x() > adverImg_startX && event->y() > adverImg_startY &&
                 event->x() < closeAdverBtn_w && event->y() < closeAdverBtn_h ))
            isAdverImg = false;

        else
        {
            /*when sender clicked just advertisement's image part*/
            mousePosition = ON_ADVER_IMAGE_CLICKED;

            x_clicked = event->x();
            y_clicked = event->y();

            x = adverImg_x;
            y = adverImg_y;
        }
    }

    /*when sender's mouse position is on result screen
     * and clicked next sourceimage button */
    else if(event->x() > sourceNextBtn_x && event->x() < afterSourceBtn_w
         && event->y() > sourceNextBtn_y && event->y() < afterSourceBtn_h )
        emit clickNextSource();

    /*when sender's mouse position is on result screen
     * and clicked before sourceimage button */
    else if(event->x() > sourceBeforeBtn_x && event->x() < beforeSourceBtn_w
         && event->y() > sourceBeforeBtn_y && event->y() < beforeSourceBtn_h )
        emit clickBeforeSource();

    /*when sender's mouse position is on caption text image
     * and click*/
    else if(event->x() > stopCaptionX && event->x() < stopCaptionX + captionWidth
         && event->y() > stopCaptionY - captionHeight  && event->y() < stopCaptionY )
        mousePosition = ON_CAPTION;

    /*when sender click restart button, do emit restartBroadcat()*/
    else if(event->x() > pauseX && event->x() < pauseX + restartImg.width() &&
            event->y() > pauseY && event->y() < pauseY + restartImg.height())
        emit restartBroadcat();

    else
        mousePosition = ON_RESULT_IMAGE;
}

/**
 * @brief ImageGLView::getBeforeImage
 * @return
 */
QImage ImageGLView::getBeforeImage()
{
    /*get image that do not draw control button
     * control button
     * -> next advertisement button
     * -> before advertisement button
     * -> next sourceimage button
     * and so on*/
    return beforeImage;
}

/**
 * @brief ImageGLView::mouseMoveEvent
 * @param event
 */
void ImageGLView::mouseMoveEvent(QMouseEvent *event)
{
    if(isPause)
    {
        qDebug("ispause");
        int pauseX,pauseY;

        pauseX = painterWidth/2  - restartImg.width()/2;
        pauseY = painterHeight/2 - restartImg.height()/2;

        qDebug("%d %d %d %d",pauseX,pauseY,event->x(),event->y());

        if(event->x() > pauseX && event->x() < pauseX + restartImg.width() &&
           event->y() > pauseY && event->y() < pauseY + restartImg.height() )
        {
            qDebug("ispause_o");
            restartImg.load(":/res/images/replay_o.png");
        }
        else
        {
            qDebug("ispause_E");
            restartImg.load(":/res/images/replay.png");
        }

        this->repaint();
    }
    else
    {
        /*when sender click avertisement image, and then
         * move mouse to another position*/
        if( mousePosition == ON_ADVER_IMAGE_CLICKED )
        {
            adverImg_x = (event->x())-(x_clicked-x);
            adverImg_y = (event->y())-(y_clicked-y);

            isOnAdverImg = false;
        }

        /*when sender click caption text, and then
         * move mouse to another position*/
        else if( mousePosition == ON_CAPTION )
        {
            stopCaptionX = (event->x())-(x_clicked-x);
            stopCaptionY = (event->y())-(y_clicked-y);

            captionX = (event->x())-(x_clicked-x);
            captionY = (event->y())-(y_clicked-y);
        }

        /*when sender's mouse position is on
         * advertisement image, 'siOnAdverImg
         * will be set true*/
        else if( isAdverImg )
        {
            /*when sender's mouse position is on
             * advertisement image, 'siOnAdverImg
             * will be set true*/
            if( event->x() > adverImg_startX && event->x() < adverImg_startX + adverImg_w
             && event->y() > adverImg_startY && event->y() < adverImg_startY + adverImg_h)
            {
                isOnAdverImg = true;

                /*init source button animation position*/
                sourceBtn_b_Position = this->window()->height();
                sourceBtn_n_Position = this->window()->height();
            }

            /*when sender's mouse position is not on
             * advertisement image, 'iOnAdverImg
             * will be set false*/
            else
            {
                isOnAdverImg = false;
                mousePosition = ON_RESULT_IMAGE;

                /*init advertisement next,before button position*/
                adverBtn_b_Position = adverImg_startX + adverImg_w/2
                                    - ANI_POSITION_GAP -CONBTN_SIZE;
                adverBtn_n_Position = adverImg_startX + adverImg_w/2
                                    + ANI_POSITION_GAP;
            }
        }
    }
}

/**
 * @brief ImageGLView::mouseReleaseEvent
 * @param event
 */
void ImageGLView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    adverNextBtn_x   = adverImg_startX + adverImage.width()/2 + CONBTN_POSITION_X;
    adverBeforeBtn_x = adverImg_startX + adverImage.width()/2 - CONBTN_POSITION_X - CONBTN_SIZE;

    /*when sender depressed mouse */
    mousePosition = ON_RESULT_IMAGE;

    /*init advertisement next,before button position*/
    adverBtn_b_Position = adverBeforeBtn_x;
    adverBtn_n_Position = adverNextBtn_x;
}

/**
 * @brief ImageGLView::mouseDoubleClickEvent
 * @param event
 */
void ImageGLView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(this->isFullScreen())
    {
        setWindowFlags(Qt::Widget);
        showNormal();
    }
    else
    {
        setWindowFlags(Qt::Window);
        showFullScreen();
    }
}

/**
 * @brief ImageGLView::leaveEvent
 * @param event
 * @return
 */
void ImageGLView::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    /*when sender mouse go out of result screen*/
    mousePosition = LEAVE_SCREEN;

    /*init source button position*/
    sourceBtn_b_Position = this->window()->height();
    sourceBtn_n_Position = this->window()->height();

    adverBtn_b_Position = adverImg_startX + adverImg_w/2
                        - ANI_POSITION_GAP -CONBTN_SIZE;
    adverBtn_n_Position = adverImg_startX + adverImg_w/2
                        + ANI_POSITION_GAP;
}
