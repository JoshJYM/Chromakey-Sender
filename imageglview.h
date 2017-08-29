#ifndef IMAGEGLVIEW_H
#define IMAGEGLVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include "settingadeverimgwidget.h"

/*mouse position define value*/
#define ON_ADVER_IMAGE_CLICKED  0
#define ON_RESULT_IMAGE         1
#define ON_CAPTION              2
#define LEAVE_SCREEN            3

/*control button position define value*/
#define CONBTN_POSITION_X      10
#define CONBTN_POSITION_Y      71
#define CONBTN_SIZE            30
#define ADVER_SET_CONBTN_SIZE  40

#define SBUTTON_ANIMATION_SPEED  15
#define ABUTTON_ANIMATION_SPEED  5
#define ANIMATION_DIRECT_DOWN   0
#define ANIMATION_DIRECT_UP     1

#define ANI_POSITION_GAP        25
#define GAP                     15

/*caption animation define value*/
#define STOP            0
#define RIGHT_TO_LEFT   1
#define LEFT_TO_RIGHT   2
#define UP_TO_DOWN      3
#define DOWN_TO_UP      4

/*image define value*/
#define DEFAULT_ADVERIMAGE_WIDTH  150
#define DEFAULT_ADVERIMAGE_HEIGHT 100

#define DEFAULT_ADVERIMG_START_X 10
#define DEFAULT_ADVERIMG_START_Y 10

/*advertisement image information define value*/
#define DEFAULT_URL_ADDRESS "http://www.swmaestro.kr/main.do"

class ImageGLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit ImageGLView(QObject *parent);
    ~ImageGLView();

    void ImageLoad(const QPixmap &image);
    QImage getBeforeImage();

private:
    QPixmap    sourceImage;
    QImage     adverImage;
    QImage     movieImage;
    QImage     beforeImage;
    QImage     adverNext;
    QImage     adverBefore;
    QImage     exitAdverImg;
    QImage     settingAdverImg;
    QImage     nextSourceImg;
    QImage     beforeSourceImg;
    QImage     pauseImg;
    QImage     restartImg;

    QRect      *rectDrawpic;


    SettingAdeverImgWidget *adverSettWidget;

    bool isAdverImg;
    bool isCaption;
    bool isOnAdverImg;
    bool boldState;
    bool isPause;

    int  mousePosition;

    int x;
    int y;

    int painterWidth;
    int painterHeight;

    int sourceBtn_b_Position;
    int sourceBtn_n_Position;

    int adverBtn_b_Position;
    int adverBtn_n_Position;

    int adverImg_w;
    int adverImg_h;

    int adverImg_startX;
    int adverImg_startY;

    int adverImg_x;
    int adverImg_y;

    int x_clicked;
    int y_clicked;

    int adverNextBtn_x;
    int adverBeforeBtn_x;
    int sourceNextBtn_x;
    int sourceBeforeBtn_x;
    int setBtn_x;


    int adverNextBtn_y;
    int adverBeforeBtn_y;
    int sourceNextBtn_y;
    int sourceBeforeBtn_y;
    int setBtn_y;

    int startX;
    int startY;

    int endX;
    int endY;

    QString captionText;
    QString urlAddress;

    int  animation;
    int  captionFontSize;
    int  captionColor;
    int  captionX;
    int  captionY;
    int  captionWidth;
    int  captionHeight;
    int  stopCaptionX;
    int  stopCaptionY;

private:
    void initValue();
    void setCaptionAnimation(int animation);
    void setCaptionFontSize(int fontSize);
    void setCaptionColor(int color);
    void setCaptionContext(QString text);

    int buttonAnimation(int startPosition,int endPosition , int speed, int direct);
    QImage cutFixedImage(QImage img, int startX, int startY, int width, int height);

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);

private slots:
    void slotGetAdverImg(QImage &image);
    void slotGetAdverInfo(int w, int h);
    void slotInitAdverImg();
    void slotFullScreenView();
    void slotGetCaptionInfo(int animation, int size, int color, QString context);
    void slotGetCaptionState(bool isCaption);
    void slotGetUrlAddress(QString url);
    void slotGetCaptionBold(bool state);
    void slotPauseBroadCast(bool state);

signals:
    void clickNextAdver();
    void clickBeforeAdver();
    void clickNextSource();
    void clickBeforeSource();
    void sendSettingVal(int w, int h);
    void sendAdverInfo(QString url, int startX, int startY, int width, int height);
    void restartBroadcat();
};

#endif // IMAGEGLVIEW_H
