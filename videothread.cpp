#include "videothread.h"

VideoThread::VideoThread()
{
}

void VideoThread::run()
{
    videoTimer.start();
    isRunning = true;

    while(isRunning)
    {
        if(videoTimer.elapsed()==TIMEOUTINTERVAL)
        {
            emit timeOut();
            videoTimer.restart();
        }
    }
}

void VideoThread::stop()
{
    isRunning = false;
}
