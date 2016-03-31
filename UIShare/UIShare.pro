#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T08:58:24
#
#-------------------------------------------------

QT       += widgets multimedia

TARGET = UIShare
TEMPLATE = lib

DEFINES += HMIUI_LIBRARY

win32:QT += axcontainer

INCLUDEPATH += $$PWD/   \
              $$PWD/UI  \
$$PWD/../Include/ffmpeg \
$$PWD/../Include/msp   \
$$PWD/../Include


DEFINES += __STDINT_MACROS  #for ffmpeg

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=../bin/UILib

target.path=$$OUT_PWD/../bin
INSTALLS+=target

SOURCES += UIShare.cpp \
    UIManager.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_writer.cpp \
    UI/Alert/AlertUI.cpp \
    UI/AudioPassThru/AudioPassThru.cpp \
    UI/Config/Config.cpp \
    UI/Common/AppBase.cpp \
    UI/Common/AppItemWidget.cpp \
    UI/Common/AppListWidget.cpp \
    UI/Common/Background.cpp \
    UI/Common/BaseWidght.cpp \
    UI/Common/Button.cpp \
    UI/Common/MainMenu.cpp \
    UI/Common/MenuButton.cpp \
    UI/Common/ScrollBar.cpp \
    UI/Notify/Notify.cpp \
    UI/Choiceset/Choiceset.cpp \
    UI/Choiceset/ChoicesetVR.cpp \
    UI/Command/Command.cpp \
    UI/ScrollableMessage/ScrollMsg.cpp \
    UI/Show/Show.cpp \
    UI/Slider/Slider.cpp \
    UI/TextSpeech/textspeech.cpp \
    UI/TextSpeech/TextToSpeech.cpp \
    UI/AudioTrans/AudioInput.cpp \
    UI/AudioTrans/AudioOutput.cpp \
    UI/AudioTrans/MspVRAudio.cpp \
    UI/AppListView/AppListView.cpp \
    UI/AppListView/CAppButton.cpp \
    UI/MainWindow/MainWindow.cpp \
    AppLinkMenu.cpp \
    UI/Common/CAppButton.cpp \
    UI/VideoStream/VideoStream.cpp

HEADERS += UIShare.h \
    UIManager.h \
    json/assertions.h \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/json_tool.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    Include/global_first.h \
    Include/ProtocolDefines.h \
    UI/Alert/AlertUI.h \
    UI/AudioPassThru/AudioPassThru.h \
    UI/Config/Config.h \
    UI/Common/AppBase.h \
    UI/Common/AppItemWidget.h \
    UI/Common/AppListWidget.h \
    UI/Common/Background.h \
    UI/Common/BaseWidght.h \
    UI/Common/Button.h \
    UI/Common/MainMenu.h \
    UI/Common/MenuButton.h \
    UI/Common/ScrollBar.h \
    UI/Notify/Notify.h \
    UI/Choiceset/Choiceset.h \
    UI/Choiceset/ChoicesetVR.h \
    UI/Command/Command.h \
    UI/ScrollableMessage/ScrollMsg.h \
    UI/Show/Show.h \
    UI/Slider/Slider.h \
    UI/TextSpeech/textspeech.h \
    UI/TextSpeech/TextToSpeech.h \
    UI/AudioTrans/AudioInput.h \
    UI/AudioTrans/AudioOutput.h \
    UI/AudioTrans/MspVRAudio.h \
    UI/AppListView/AppListView.h \
    UI/AppListView/CAppButton.h \
    UI/MainWindow/MainWindow.h \
    AppLinkMenu.h \
    UI/Common/CAppButton.h \
    UI/VideoStream/VideoStream.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    UI/images.qrc

###############################for windows
win32:!wince{
DEFINES +=WIN32
INCLUDEPATH += $$PWD/../Include/pthread \

LIBS +=  $$PWD/Library/win32/pthread/pthreadVC2.lib
LIBS +=  $$PWD/Library/win32/pthread/pthreadVCE2.lib
LIBS +=  $$PWD/Library/win32/pthread/pthreadVSE2.lib
LIBS +=  $$PWD/Library/win32/pthread/WS2_32.Lib
#LIBS += -L$$PWD/lib/win32/ffmpeg -lavcodec -lavfilter -lavformat -lavutil -lswscale
#win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec
LIBS += $$PWD/Library/win32/ffmpeg/libavcodec.a  \
$$PWD/Library/win32/ffmpeg/libavfilter.a  \
$$PWD/Library/win32/ffmpeg/libavformat.a  \
$$PWD/Library/win32/ffmpeg/libavutil.a  \
$$PWD/Library/win32/ffmpeg/libswscale.a

#pthread.path=$$OUT_PWD/bin
#pthread.files=$$PWD/Library/win32/*.dll
ffmpeg.path=$$OUT_PWD/bin
ffmpeg.files=$$PWD/Library/win32/ffmpeg/*.dll  \
$$PWD/Library/win32/pthread/*.dll
#INSTALLS+=pthread
INSTALLS+=ffmpeg
qt_dll.path=$$DESTDIR
qt_dll.files=$$(QT_DIR)/bin/*.dll
INSTALLS +=qt_dll
}


################################for linux
#unix:!android:LIBS += -L$$PWD/Library/linux/ffmpeg -lavcodec  -lavformat -lavutil -lswscale
unix:!android{
ffmpeg.path=/usr/lib
ffmpeg.files=$$PWD/Library/linux/ffmpeg/*.*
INSTALLS+=ffmpeg

LIBS += -L$$PWD/Library/linux/ffmpeg  -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
}
################################for wince
wince{
HEADERS += \
    Include/global_first.h
INCLUDEPATH += $$PWD/Include/pthread \
               $$PWD/Include
LIBS +=  $$PWD/Library/ce/pthread.lib
LIBS += -L$$PWD/Library/ce/ffmpeg  -lavcodec-55  -lavdevice-55 -lavfilter-3 -lavformat-55 -lavutil-52 -lswresample-0 -lswscale-2
pthread.path=$$OUT_PWD/bin
pthread.files=$$PWD/Library/ce/*.dll
ffmpeg.path=$$OUT_PWD/bin
ffmpeg.files=$$PWD/Library/ce/ffmpeg/*.dll

INSTALLS +=pthread
INSTALLS+=ffmpeg
}


################################for android
android{

#CONFIG += msc
#CONFIG += pico
CONFIG  += espeak
INCLUDEPATH +=  $$PWD/Include/msp \
                $$PWD/Include/msp/android

DEFINES +=ANDROID \
          SDL_SUPPORT_LIB \
          SDL_SUPPORT_VR

LIBS += -L$$PWD/Library/android/ffmpeg -lffmpeg
LIBS += -L$$PWD/Library/android/sdl -lsmartDeviceLinkCore
LIBS += -L$$PWD/Library/android/msp  -llib_msp_vr
ANDROID_EXTRA_LIBS = \
        $$PWD/Library/android/ffmpeg/libffmpeg.so \
        $$PWD/Library/android/sdl/libsmartDeviceLinkCore.so
msc{
DEFINES += TTS_FLY_MSC
LIBS += -L$$PWD/Library/android/msp  -lmsc
#RESOURCES += Library/android/sdl/tts/msctts.qrc
ANDROID_EXTRA_LIBS +=$$PWD/Library/android/msp/libmsc.so
}
pico{
DEFINES +=TTS_ANDROID_SELF
LIBS += -L$$PWD/Library/android/msp  -lttspico -lttscompat
ANDROID_EXTRA_LIBS +=$$PWD/Library/android/msp/libttspico.so \
       $$PWD/Library/android/msp/libttscompat.so
}
espeak{
DEFINES += TTS_ESPEAK
LIBS += -L$$PWD/Library/android/msp -lttsespeak
ANDROID_EXTRA_LIBS +=$$PWD/Library/android/msp/libttsespeak.so
}

RESOURCES += \
    Library/android/sdl/config/android.qrc
}


#!android{
#configfile.path=$$OUT_PWD/bin/Config
#configfile.files=$$PWD/Config/*
#INSTALLS +=configfile
#}


