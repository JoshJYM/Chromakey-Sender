#include "voicepainter.h"

VoicePainter::VoicePainter(QObject *parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

VoicePainter::~VoicePainter()
{
    qDebug("delete VoicePainter...");
}

/**
 * @brief VoicePainter::getVoiceData
 * @param val
 */
void VoicePainter::getVoiceData(int val)
{
    voiceVal = val;
    this->repaint();
}

/**
 * @brief VoicePainter::paintEvent
 * @param event
 */
void VoicePainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter;

    painter.begin(this);

    short w = this->width();
    short h = this->height();

    painter.fillRect(0,0,w,h,0x414141);

    painter.fillRect(0, 0, w         , 15,0x414141);
    painter.fillRect(0, 0, voiceVal*2, 15,0x5B9BD5);

    painter.end();
}
