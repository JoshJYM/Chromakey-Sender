#include "aframeglview.h"

AFrameGLView::AFrameGLView(QObject *parent)
{}

AFrameGLView::~AFrameGLView()
{
    qDebug("delete AFrameGLView...");
}

/**
 * @brief AFrameGLView::ImageLoad
 * @param image
 */
void AFrameGLView::ImageLoad(const QPixmap &image)
{
    this->sourceImage = image;
    this->repaint();
}

/**
 * @brief AFrameGLView::resizeEvent
 * @param event
 */
void AFrameGLView::resizeEvent(QResizeEvent *event )
{
     Q_UNUSED(event)
}

/**
 * @brief AFrameGLView::mouseMoveEvent
 * @param event
 */
void AFrameGLView::mouseMoveEvent(QMouseEvent *event)
{
    x = event->x();
    y = event->y();

    emit sendClickPosition(event->x(),event->y());
}

/**
 * @brief AFrameGLView::mouseReleaseEvent
 * @param event
 */
void AFrameGLView::mouseReleaseEvent(QMouseEvent *event)
{
    isClick = false;
    this->repaint();
}

/**
 * @brief AFrameGLView::mousePressEvent
 * @param event
 */
void AFrameGLView::mousePressEvent(QMouseEvent *event)
{
    isClick = true;
    x = event->x();
    y = event->y();
}

/**
 * @brief AFrameGLView::slotGetMaskSize
 * @param maskSize
 */
void AFrameGLView::slotGetMaskSize(int maskSize)
{
    this->maskSize = maskSize;
}

/**
 * @brief AFrameGLView::paintEvent
 * @param event
 */
void AFrameGLView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    painter.begin(this);

    int scaledWidth     = painter.window().width();
    int scaledHeight    = painter.window().height();

    sourceImage.scaled(scaledWidth,scaledHeight,Qt::IgnoreAspectRatio);
    painter.drawPixmap(0,0,scaledWidth,scaledHeight,sourceImage);

    /*draw remove mask rectangle image*/
    if(isClick)
        painter.drawRect(x-maskSize/2,y-maskSize/2,maskSize,maskSize);

    painter.end();
}



