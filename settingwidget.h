#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include "imageglview.h"
#include "morpologyprocessing.h"
#include "aframeglview.h"
#include "qvideodecoder.h"
#include <QWidget>
#include <QStringListModel>

/*stack define value*/
#define MAX_STACK_SIZE 100
#define PARTITION_NUM  4

/*image define value*/
#define DEFAULT_ADVERIMAGE_WIDTH  150
#define DEFAULT_ADVERIMAGE_HEIGHT 100

#define PROCCESSIMAGE_WIDTH  640
#define PROCCESSIMAGE_HEIGHT 480

#define FORMAT_RGB888  3

#define PREVIEWIMAGE_WIDTH   180
#define PREVIEWIMAGE_HEIGHT  130

#define DEFAULT_PREVIEWIMAGE ":/res/images/noImage.png"

#define DEFAULT_MASK_SIZE  30
#define DEFAULT_LIST_INDEX -1

/*widget define value*/
#define IMAGE_SET_TAB         0
#define IMAGE_PROCCESSING_TAB 1

/*videoTimer define value*/
#define MOVIE_PREVIEWIMAGE_SEEKTIME 2000

/*targetImage type define value*/
#define IMAGE          0
#define MOVIE          1


namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = 0);
    ~SettingWidget();

    void getImageFrame(QImage &frame);
    void setCamSize(int imgWidth, int imgHeight);

private:
    Ui::SettingWidget   *ui;
    AFrameGLView        *oneFrameView;
    uchar               *oneFrameData;
    QString             backgroundImgFileName;
    QString             adverImgFileName;
    QImage              *targetImg;
    QImage              *resultImg;
    QImage              *sourceImg;
    QImage              *adverImage;
    MorPologyProcessing *imgprocess;
    QStringListModel    *backgroundModel;
    QStringList         *backgroundList;
    QStringListModel    *adverModel;
    QStringList         *adverList;

    QVideoDecoder       videoCodec;

    int backgroundType;

    int *maxRed ;
    int *minRed ;

    int *maxGreen ;
    int *minGreen ;

    int *maxBlue ;
    int *minBlue ;

    int maskSize;

    int stackTop[PARTITION_NUM];

    int redMaxStack[PARTITION_NUM][MAX_STACK_SIZE];
    int redMinStack[PARTITION_NUM][MAX_STACK_SIZE];

    int greenMaxStack[PARTITION_NUM][MAX_STACK_SIZE];
    int greenMinStack[PARTITION_NUM][MAX_STACK_SIZE];

    int blueMaxStack[PARTITION_NUM][MAX_STACK_SIZE];
    int blueMinStack[PARTITION_NUM][MAX_STACK_SIZE];

    int imageWidth;
    int imageHeight;

    int adverImgIndex;
    int backgroundImgIndex;

    int adverImgWidth;
    int adverImgHeight;

    int backgroundImgCnt ;
    int adverImgCnt;

    int part;

    void initViewImage();
    void initValue();
    void initConnect();
    void pushValue(int rMax,int rMin,int gMax,int gMin,int bMax,int bMin, int part);
    void checkBackgrountType();
    void initMaxMinVal();
    void setStackVal(int rMax, int rMin, int gMax, int gMin, int bMax, int bMin, int part);
    void setActiveTab(int tabNum);
    void setColorLabel(int part);

private slots:
    void slotFindMM(int x, int y);
    void slotCancleMorpology();
    void slotApplyMorpoloy();
    void slotPopValue();
    void slotAddBackgroundImg();
    void slotAddAdverImg();
    void slotApplySettingImg();
    void slotinitMorpoloyVal();
    void slotShowNextAdverImg();
    void slotShowBeforeAdverImg();
    void slotShowNextSourceImg();
    void slotShowBeforeSourceImg();
    void slotGetAdverInfo(int w, int h);
    void slotChagebackImage(QModelIndex list);
    void slotChageAdverImage(QModelIndex list);

signals:
    void sendProcessData(int *rMax,int *rMin,int *gMax,
                         int *gMin,int *bMax,int *bMin);
    void sendGrabProcessData(int *rMax,int *rMin,int *gMax,
                             int *gMin,int *bMax,int *bMin);
    void sendProcessImgData(QImage &source, QImage &target, QImage &result);
    void cancelMorpology();
    void applyMorpology();
    void sendBackgroundImg(QImage &image);
    void sendBackgroundImg(QVideoDecoder &decoder);
    void sendAdverImg(QImage &image);
    void sendUrlAddress(QString url);
};

#endif // SETTINGWIDGET_H
