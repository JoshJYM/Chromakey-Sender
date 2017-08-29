#include "camerathread.h"

CameraThread::CameraThread()
{
}

void CameraThread::run()
{
    cameraTimer.start();
    isRunning = true;

    while(isRunning)
    {
        if(cameraTimer.elapsed()==TIMEOUTINTERVAL)
        {
            emit timeOut();
           cameraTimer.restart();
        }
    }
}

void CameraThread::stop()
{
    isRunning = false;
}
