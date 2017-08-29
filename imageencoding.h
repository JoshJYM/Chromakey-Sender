#ifndef IMAGEENCODING_H
#define IMAGEENCODING_H
#include <QObject>
#include <QImage>
#include "jpeglib.h"
#include "jerror.h"

class ImageEncoding : public QObject
{
    Q_OBJECT
public:
    explicit ImageEncoding(QObject *parent);
    ~ImageEncoding();

    void setQuality(int quality);
    void writeJPEGMem(QImage& resultImage, QByteArray& jpegEncData);

private:
    int encodingQuality;


};

#endif // IMAGEENCODING_H
