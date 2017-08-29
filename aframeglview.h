#ifndef AFRAMEGLVIEW_H
#define AFRAMEGLVIEW_H

#include <QGLWidget>
#include <QMouseEvent>

class AFrameGLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit AFrameGLView(QObject *parent);
    ~AFrameGLView();

    void ImageLoad(const QPixmap &image);

private:
    QPixmap    sourceImage;
    QPainter   painter;

    int        x;
    int        y;
    int        maskSize;

    bool       isClick;

signals:
    void sendClickPosition(int x, int y);

private slots:
    void slotGetMaskSize(int maskSize);

private:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // AFRAMEGLVIEW_H
