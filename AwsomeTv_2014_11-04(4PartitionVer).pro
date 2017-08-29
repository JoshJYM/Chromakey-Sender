#-------------------------------------------------
#
# Project created by QtCreator 2014-08-25T14:46:35
#
#-------------------------------------------------

QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AwsomeTv_For_Sender
TEMPLATE = app

# --------------you has to set ffmepg library, include path---------------
FFMPEG_LIBRARY_PATH         = C:\Users\wnrhd_000\Desktop\ffmpeg_lib_win32
FFMPEG_INCLUDE_PATH         = C:\Users\wnrhd_000\Desktop\QTFFmpegWrapper
# ------------------------------------------------------------------------

SOURCES += main.cpp\
        widget.cpp \
    cvmattoqimage.cpp \
    cameradevice.cpp \
    cameradeviceinfo.cpp \
    imageglview.cpp \
    imageencoding.cpp \
    audiodeviceinfo.cpp \
    audiodevice.cpp \
    sendimagesocket.cpp \
    settingwidget.cpp \
    morpologyprocessing.cpp \
    sendaudiosocket.cpp \
    aframeglview.cpp \
    musicplayer.cpp \
    sendmusicdatasocket.cpp \
    loginwidget.cpp \
    tcpconnection.cpp \
    settingadeverimgwidget.cpp \
    voicepainter.cpp \
    senderchatting.cpp \
    signupwidget.cpp \
    qvideodecoder.cpp

HEADERS  += widget.h \
    cvmattoqimage.h \
    cameradevice.h \
    cameradeviceinfo.h \
    imageglview.h \
    imageencoding.h \
    audiodeviceinfo.h \
    audiodevice.h \
    sendimagesocket.h \
    settingwidget.h \
    morpologyprocessing.h \
    sendaudiosocket.h \
    aframeglview.h \
    musicplayer.h \
    sendmusicdatasocket.h \
    loginwidget.h \
    TcpPacketType.h \
    tcpconnection.h \
    UdpPacketType.h \
    settingadeverimgwidget.h \
    voicepainter.h \
    senderchatting.h \
    signupwidget.h \
    MainWidgetDefineVal.h \
    qvideodecoder.h

FORMS    += widget.ui \
    settingwidget.ui \
    loginwidget.ui \
    settingadeverimgwidget.ui \
    signupwidget.ui


INCLUDEPATH += D:\\opencv\\build\\include
LIBS += D:\\opencv\\build\\x86\\vc11\\lib\\*.lib

INCLUDEPATH += C:\libjpeg-turbo\include
LIBS += -LC:\libjpeg-turbo\lib jpeg-static.lib turbojpeg-static.lib

LIBS += -L$$FFMPEG_LIBRARY_PATH
INCLUDEPATH += $$FFMPEG_INCLUDE_PATH

LIBS += -lavutil \
         -lavcodec \
         -lavformat \
         -lswscale

RESOURCES += \
    images.qrc
