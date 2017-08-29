#ifndef VOICEPAINTER_H
#define VOICEPAINTER_H

#include <QGLWidget>

class VoicePainter : public QGLWidget
{
    Q_OBJECT
public:
    explicit VoicePainter(QObject *parent);
    ~VoicePainter();
    void getVoiceData(int val);

private:
    int voiceVal;

    virtual void paintEvent(QPaintEvent *event);
};

#endif // VOICEPAINTER_H
