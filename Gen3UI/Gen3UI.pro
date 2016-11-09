#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T16:06:45
#
#-------------------------------------------------

QT       += widgets
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT +=multimedia

TARGET = Gen3UI
TEMPLATE = lib

CONFIG += ffmpeg
CONFIG  += wince  ##wince

DEFINES += HMIUI_LIBRARY __STDC_CONSTANT_MACROS

INCLUDEPATH += $$PWD/
INCLUDEPATH +=  $$PWD/../Include/ffmpeg \
                $$PWD/../Include/msp   \
                $$PWD/../Include       \

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=../bin/UILib

#target.path=$$OUT_PWD/../bin/UILib
#INSTALLS+=target

SOURCES += \
    Gen3UIManager.cpp \
    UILib.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_writer.cpp \
    MainWindow/MainWindow.cpp \
    AppListView/AppListView.cpp \
    AppListView/CAppButton.cpp \
    Common/MenuButton.cpp \
    CommandView/CommandView.cpp \
    CommandView/CustomCombobox.cpp \
    CommandView/CustomComboboxItem.cpp \
    Common/Button.cpp \
    Common/ScrollBar.cpp \
    Common/AppBase.cpp \
    Common/CustomButton.cpp \
    Show/MainWidget.cpp \
    Show/MediaShow.cpp \
    Config/Config.cpp \
    VideoStream/VideoStream.cpp \
    Alert/AlertView.cpp \
    Common/CustomListView.cpp \
    ChoiceSet/ChoiceSet.cpp \
    ChoiceSet/ChoiceSetVR.cpp \
    ScrollableMessage/ScollMsgView.cpp \
    SliderView/SliderView.cpp \
    AudioPassThru/AudioPassView.cpp \
    VideoStream/MediaCodecStream.cpp \
    VideoStream/JniNative.cpp \
    VideoStream/JniFrame.cpp \
    VideoStream/CeVideoStream.cpp

HEADERS += \
    Gen3UIManager.h \
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
    MainWindow/MainWindow.h \
    AppListView/AppListView.h \
    AppListView/CAppButton.h \
    Common/MenuButton.h \
    CommandView/CommandView.h \
    CommandView/CustomCombobox.h \
    CommandView/CustomComboboxItem.h \
    Common/Button.h \
    Common/ScrollBar.h \
    Common/AppBase.h \
    Common/CustomButton.h \
    Show/MainWidget.h \
    Show/MediaShow.h \
    Config/Config.h \
    main.h \
    VideoStream/VideoStream.h \
    Alert/AlertView.h \
    Common/CustomListView.h \
    ChoiceSet/ChoiceSet.h \
    ChoiceSet/ChoiceSetVR.h \
    ScrollableMessage/ScollMsgView.h \
    SliderView/SliderView.h \
    AudioPassThru/AudioPassView.h \
    VideoStream/MediaCodecStream.h \
    VideoStream/JniNative.h \
    VideoStream/JniFrame.h \
    VideoStream/CeVideoStream.h

unix {
    target.path = /usr/lib
    #INSTALLS += target
}

RESOURCES += \
    image.qrc


###############################for windows
win32:!wince{
DEFINES +=WIN32 \
          OS_WIN32
INCLUDEPATH += $$PWD/../Include/pthread \

DEFINES += SDL_CALL_BACK \
           SDL_SUPPORT_LIB
LIBS +=  $$PWD/Library/win32/sdl/smartDeviceLinkCore.lib
sdllib.path=$$OUT_PWD/../bin
sdllib.files=$$PWD/Library/win32/sdl/*.*
INSTALLS+=sdllib

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
ffmpeg.path=$$OUT_PWD/../bin
#linux 64bit
ffmpeg.files=$$PWD/Library/linux/ffmpeg/*.*
#linux 32bit
#ffmpeg.files=$$PWD/Library/linux/ffmpeg_32/*.*
INSTALLS+=ffmpeg

#linux 64bit
LIBS += -L$$PWD/Library/linux/ffmpeg  -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
#linux 32bit
#LIBS += -L$$PWD/Library/linux/ffmpeg_32  -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
}
################################for wince
wince{
HEADERS += \
    Include/global_first.h
INCLUDEPATH += $$PWD/../Include/pthread \
               $$PWD/../Include \
               $$PWD/../Include/wince
LIBS +=  $$PWD/Library/ce/pthread.lib
#LIBS += -L$$PWD/Library/ce/ffmpeg  -lavcodec-55  -lavdevice-55 -lavfilter-3 -lavformat-55 -lavutil-52 -lswresample-0 -lswscale-2
pthread.path=$$OUT_PWD/../bin
pthread.files=$$PWD/Library/ce/*.dll
#ffmpeg.path=$$OUT_PWD/../bin
#ffmpeg.files=$$PWD/Library/ce/ffmpeg/*.dll

INSTALLS +=pthread
#INSTALLS+=ffmpeg
DEFINES += OS_WIN32

DEFINES += SDL_CALL_BACK \
           SDL_SUPPORT_LIB
LIBS +=  $$PWD/Library/ce/sdl/smartDeviceLinkCore.lib
sdllib.path=$$OUT_PWD/../bin
sdllib.files=$$PWD/Library/ce/sdl/*.*
INSTALLS+=sdllib
}


################################for android
android{
QT += androidextras
#CONFIG += msc
#CONFIG += pico
CONFIG  += espeak
INCLUDEPATH +=  $$PWD/Include/msp \
                $$PWD/Include/msp/android

DEFINES +=ANDROID \
          SDL_SUPPORT_LIB \
          SDL_SUPPORT_VR
DEFINES += SDL_CALL_BACK

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



}

