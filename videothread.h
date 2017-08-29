#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#define TIMEOUTINTERVAL 10

#include <QThread>
#include <QObject>
#include <QElapsedTimer>

class VideoThread : public QThread
{
    Q_OBJECT
public:
    VideoThread();

    QThread         videoThread;
    QElapsedTimer   videoTimer;

    void run();
    void stop();

private:
    bool isRunning;

signals:
    void timeOut();

};

#endif // VIDEOTHREAD_H
