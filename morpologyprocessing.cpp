#include "morpologyprocessing.h"

uchar mask[3][3] = {255,255,255, 255,255,255, 255,255,255};

/**
 * @brief MorPologyProcessing::MorPologyProcessing
 */
MorPologyProcessing::MorPologyProcessing(QObject *parent)
{
    initValue();
}

/**
 * @brief MorPologyProcessing::~MorPologyProcessing
 */
MorPologyProcessing::~MorPologyProcessing()
{
    qDebug("delete MorPologyProcessing...");
}

/**
 * @brief MorPologyProcessing::setCamImageSize
 * @param width
 * @param height
 */
void MorPologyProcessing::setCamImageSize(int width, int height)
{
    qDebug("setCamImageSize");
    orgImage  = new uchar[width*height];
    DestImage = new uchar[width*height];

    cameraWidth  = width;
    cameraHeight = height;
}

/**
 * @brief MorPologyProcessing::setMode
 * @param sImgWidth
 * @param sImgHeight
 * @param mode
 */
void MorPologyProcessing::setMode(int sImgWidth, int sImgHeight, int mode)
{
    this->sImageWidgth = sImgWidth;
    this->sImageHeight = sImgHeight;

    switch(mode)
    {
    case NORMAL:
        modeWSize = cameraWidth;
        modeHSize = cameraHeight;
        break;

    case LEFT_UP:
        modeWSize = cameraWidth;
        modeHSize = cameraHeight;
        break;

    case LEFT_DOWN:
        modeWSize = cameraWidth;
        modeHSize = sImgHeight;
        break;

    case RIGHT_UP:
        modeWSize = sImgWidth;
        modeHSize = cameraHeight;
        break;
    case RIGHT_DOWN:
        modeWSize = sImgWidth;
        modeHSize = sImgHeight;
        break;
    }
    memset(orgImage , 0 ,cameraWidth*cameraHeight);
    memset(DestImage, 0 ,cameraWidth*cameraHeight);
}

/**
 * @brief MorPologyProcessing::slotChangeMode
 * @param mode
 */
void MorPologyProcessing::slotChangeMode(bool mode)
{
    this->isSmall = !mode;
}

/**
 * @brief MorPologyProcessing::initValue
 */
void MorPologyProcessing::initValue()
{
    orgImage  = new uchar[640*480];
    DestImage = new uchar[640*480];

    maxRed = new int[PARTITION_NUM];
    minRed = new int[PARTITION_NUM];

    maxBlue = new int[PARTITION_NUM];
    minBlue = new int[PARTITION_NUM];

    maxGreen = new int[PARTITION_NUM];
    minGreen = new int[PARTITION_NUM];

    isSmall   = false;
}

/**
 * @brief MorPologyProcessing::slotSetThresholdData
 * @param rMax
 * @param rMin
 * @param gMax
 * @param gMin
 * @param bMax
 * @param bMin
 */
void MorPologyProcessing::slotSetThresholdData(int *rMax,int *rMin,int *gMax,
                                               int *gMin,int *bMax,int *bMin)
{
    maxRed   = rMax;
    maxGreen = gMax;
    maxBlue  = bMax;

    minRed   = rMin;
    minGreen = gMin;
    minBlue  = bMin;
}

/**
 * @brief MorPologyProcessing::toQImageRGB888
 * @param image
 * @return
 */
QImage MorPologyProcessing::toQImageRGB888(QImage image)
{
    QImage newQImage = QImage(image.width(), image.height(), QImage::Format_RGB888);
    uchar  *pImageData = image.bits();
    uchar  *pNewImageData = newQImage.bits();

    int dataSize = image.width() * image.height() * 3;

    /*if image file is png*/
    if(image.depth() == PNG_TYPE)
    {
        for (int i = 0, j = 0 ; i < dataSize  ; i += 3, j += 1)
        {
            pNewImageData[i+2]  = pImageData[j];
            pNewImageData[i+1]  = pImageData[j];
            pNewImageData[i]    = pImageData[j];
        }
    }

    /*if image file is jepg or bmp and so on*/
    else
    {
        for (int i = 0, j = 0 ; i < dataSize  ; i += 3, j += 4)
        {
            pNewImageData[i+2]  = pImageData[j];
            pNewImageData[i+1]  = pImageData[j+1];
            pNewImageData[i]    = pImageData[j+2];
        }
    }
    return newQImage;
}

/**
 * @brief MorPologyProcessing::toSmallImage
 * @param image
 * @return
 */
QImage MorPologyProcessing::toSmallImage(QImage image)
{
    int inc  = 0;
    int binc = 0;

    QImage  bigImage     = QImage(cameraWidth,cameraHeight,QImage::Format_RGB888);
    uchar  *bigImgData  = bigImage.bits();
    uchar  *smalImgData = image.bits();

    memset(bigImgData,0,bigImage.byteCount());

    for(int i=0;i<image.height();i++)
    {
        for(int j=0;j<image.width();j++)
        {
            bigImgData[binc+0] = smalImgData[inc+0];
            bigImgData[binc+1] = smalImgData[inc+1];
            bigImgData[binc+2] = smalImgData[inc+2];

            inc  += 3;
            binc = (i+cameraHeight-modeHSize)*bigImage.width()*3 + (j+cameraWidth-modeWSize)*3;
        }
    }
    return bigImage;
}

/**
 * @brief MorPologyProcessing::chromakeyProcessMorphology
 * @param sourceImage
 * @param targetImage
 * @param resultImage
 */
void MorPologyProcessing::chromakeyProcessMorphology(const QImage& sourceImage,
                                                     QImage& targetImage,QImage& resultImage)
{
    const uchar *pSourceData = sourceImage.bits();
    uchar *pTargetData = targetImage.bits();
    uchar *pResultData = resultImage.bits();

    int sPixRed, sPixGreen, sPixBlue;
    int index   = 0;

    int width  = sourceImage.width();

    int inc = 0;
    int x = 0, y = 0;
    int part = 0;

    if(isSmall)
    {
        smallFourPartitionProcess(ONE_PARTITION,pSourceData,targetImage);
        smallFourPartitionProcess(TWO_PARTITION,pSourceData,targetImage);
        smallFourPartitionProcess(THREE_PARTITION,pSourceData,targetImage);
        smallFourPartitionProcess(FOUR_PARTITION,pSourceData,targetImage);

        inc = 0;
        for(y = 0 ; y < targetImage.height() ; y++)
        {
            for(x = 0 ; x < targetImage.width() ; x++)
            {
                index = x + (y * width);
                if(DestImage[index] == 0)
                {
                    pResultData[inc+2] = pTargetData[inc+2];
                    pResultData[inc+1] = pTargetData[inc+1];
                    pResultData[inc+0] = pTargetData[inc+0];
                }
                else if(DestImage[index] == 255)
                {
                    pResultData[inc+2] = pSourceData[inc+2];
                    pResultData[inc+1] = pSourceData[inc+1];
                    pResultData[inc+0] = pSourceData[inc+0];
                }

                inc += 3;
            }
        }
    }

    else
    {
        fourPartitionProcess(ONE_PARTITION,pSourceData,targetImage);
        fourPartitionProcess(TWO_PARTITION,pSourceData,targetImage);
        fourPartitionProcess(THREE_PARTITION,pSourceData,targetImage);
        fourPartitionProcess(FOUR_PARTITION,pSourceData,targetImage);

        for(y = 0 ; y < targetImage.height() ; y++)
        {
            for(x = 0 ; x < targetImage.width() ; x++)
            {
                index = x + (y * width);
                if(DestImage[index] == 0)
                {
                    pResultData[inc+2] = pTargetData[inc+2];
                    pResultData[inc+1] = pTargetData[inc+1];
                    pResultData[inc+0] = pTargetData[inc+0];
                }
                else if(DestImage[index] == 255)
                {
                    pResultData[inc+2] = pSourceData[inc+2];
                    pResultData[inc+1] = pSourceData[inc+1];
                    pResultData[inc+0] = pSourceData[inc+0];
                }
                inc += 3;
            }
        }
    }
}

void MorPologyProcessing::smallFourPartitionProcess(int part, const uchar *data,
                                                    QImage img)
{
    int startX,endX;
    int startY,endY;
    int inc;
    int index;
    int sPixRed,sPixGreen,sPixBlue;
    int width = img.width();

    switch(part)
    {
    case 0:
        startX  = 0;
        startY  = 0;

        endX = sImageWidgth/2;
        endY = sImageHeight/2;
        break;
    case 1:
        startX  = sImageWidgth/2;
        startY  = 0;

        endX = sImageWidgth;
        endY = sImageHeight/2;
        break;
    case 2:
        startX  = 0;
        startY  = sImageHeight/2;

        endX = sImageWidgth/2;
        endY = sImageHeight;
        break;
    case 3:
        startX  = sImageWidgth/2;
        startY  = sImageHeight/2;

        endX = sImageWidgth;
        endY = sImageHeight;
        break;
    }

    for( int y = startY ; y < endY ; y++)
    {
        for( int x = startX ; x < endX; x++)
        {
            inc = ((y + cameraHeight - modeHSize))*3*cameraWidth+3*(x + cameraWidth - modeWSize);
            index = x + cameraWidth - modeWSize  + ((y + cameraHeight-modeHSize) * width);

            sPixRed   = data[inc+0];
            sPixGreen = data[inc+1];
            sPixBlue  = data[inc+2];

            if (((sPixRed   <= maxRed[part])   && (sPixRed   >= minRed[part]))   &&
                ((sPixGreen <= maxGreen[part]) && (sPixGreen >= minGreen[part])) &&
                ((sPixBlue  <= maxBlue[part])  && (sPixBlue  >= minBlue[part])))
                DestImage[index] = 0;
            else
                DestImage[index] = 255;
        }
    }
}

/**
 * @brief MorPologyProcessing::fourPartitionProcess
 * @param part
 * @param data
 * @param img
 */
void MorPologyProcessing::fourPartitionProcess(int part, const uchar *data, QImage img)
{
    int startX,endX;
    int startY,endY;
    int inc;
    int index;
    int sPixRed,sPixGreen,sPixBlue;
    int width = img.width();

    switch(part)
    {
    case 0:
        startX  = 0;
        startY  = 0;

        endX = img.width()/2;
        endY = img.height()/2;
        break;
    case 1:
        startX  = img.width()/2;
        startY  = 0;

        endX = img.width();
        endY = img.height()/2;
        break;
    case 2:
        startX  = 0;
        startY  = img.height()/2;

        endX = img.width()/2;
        endY = img.height();
        break;
    case 3:
        startX  = img.width()/2;
        startY  = img.height()/2;

        endX = img.width();
        endY = img.height();
        break;
    }

    for(int y = startY ; y < endY ; y++)
    {
        for(int x = startX ; x < endX; x++)
        {
            inc = x*3 + y*3*width;
            index = x + (y * width);

            sPixRed   = data[inc+0];
            sPixGreen = data[inc+1];
            sPixBlue  = data[inc+2];

            if ( ((sPixRed   <= maxRed[part])   && (sPixRed   >= minRed[part]))   &&
                 ((sPixGreen <= maxGreen[part]) && (sPixGreen >= minGreen[part])) &&
                 ((sPixBlue  <= maxBlue[part])  && (sPixBlue  >= minBlue[part])))
                    DestImage[index] = 0;
            else
                DestImage[index] = 255;
        }
    }
}
