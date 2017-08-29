#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <QThread>
#include <QElapsedTimer>
#define TIMEOUTINTERVAL 30

class CameraThread : public QThread
{
    Q_OBJECT
public:
    CameraThread();

    QThread         cameraThread;
    QElapsedTimer   cameraTimer;

    void run();
    void stop();

private:
    bool isRunning;

signals:
    void timeOut();
};

#endif // CAMERATHREAD_H
