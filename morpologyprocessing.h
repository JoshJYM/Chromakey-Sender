#ifndef MORPOLOGYPROCESSING_H
#define MORPOLOGYPROCESSING_H
#include <QObject>
#include <QImage>

#define NORMAL      0
#define LEFT_UP     1
#define LEFT_DOWN   2
#define RIGHT_UP    3
#define RIGHT_DOWN  4

#define PNG_TYPE       8
#define PARTITION_NUM  4
#define ONE_PARTITION    0
#define TWO_PARTITION    1
#define THREE_PARTITION  2
#define FOUR_PARTITION   3


class MorPologyProcessing : public QObject
{
    Q_OBJECT
public:
    explicit MorPologyProcessing(QObject *parent);
    ~MorPologyProcessing();

    QImage toQImageRGB888(QImage image);
    QImage toSmallImage(QImage image);
    void chromakeyProcessMorphology(const QImage& sourceImage,QImage& targetImage,
                                    QImage& resultImage);
    void setCamImageSize(int width, int height);
    void initValue();
    void setMode(int sImgWidth,int sImgHeight, int mode);

private:
    int *maxRed ;
    int *minRed ;

    int *maxGreen ;
    int *minGreen ;

    int *maxBlue ;
    int *minBlue ;

    bool isSmall;

    int sImageWidgth;
    int sImageHeight;

    int modeWSize;
    int modeHSize;

    int cameraWidth;
    int cameraHeight;

    uchar *orgImage;
    uchar *DestImage;

    void fourPartitionProcess(int part, const uchar *data, QImage img);
    void smallFourPartitionProcess(int part, const uchar *data, QImage img);

public slots:
    void slotSetThresholdData(int *rMax, int *rMin, int *gMax,
                              int *gMin, int *bMax, int *bMin);
    void slotChangeMode(bool mode);
};

#endif // MORPOLOGYPROCESSING_H
