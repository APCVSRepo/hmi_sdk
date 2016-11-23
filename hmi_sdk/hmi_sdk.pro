#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T16:04:10
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
win32:QT += axcontainer

#QT += multimediawidgets
#DEFINES +=VIDEO_STREAM_WIDGET

DEFINES += __STDINT_MACROS  #for ffmpeg
#CONFIG  += wince  ##wince

TARGET = AppLink_HMI
TEMPLATE = app

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=../bin

#target.path=$$OUT_PWD/../bin
#INSTALLS+=target

#qtdll.path=$$OUT_PWD/bin
#qtdll.files=$$QMAKE_LIBDIR/*.dll
#INSTALLS +=qtdll


SOURCES += \
    main.cpp \
    HMI_SDK/Connect/SDLConnector.cpp \
    HMI_SDK/Connect/Channel.cpp \
    HMI_SDK/AppData/AppList.cpp \
    HMI_SDK/AppData/AppData.cpp \
    HMI_SDK/Connect/BasicCommunication.cpp \
    HMI_SDK/Connect/Buttons.cpp \
    HMI_SDK/Connect/Navigation.cpp \
    HMI_SDK/Connect/SocketsToSDL.cpp \
    HMI_SDK/Connect/TTS.cpp \
    HMI_SDK/Connect/UI.cpp \
    HMI_SDK/Connect/VehicleInfo.cpp \
    HMI_SDK/Connect/VR.cpp \
    HMI_Initial.cpp \
    Tools/json/json_reader.cpp \
    Tools/json/json_value.cpp \
    Tools/json/json_writer.cpp \
    HMI_SDK/Connect/CVideoStream.cpp


INCLUDEPATH += $$PWD/   \
              $$PWD/HMI_SDK \
              $$PWD/Tools

HEADERS  += \
    HMI_SDK/AppData/AppData.h \
    HMI_SDK/AppData/AppList.h \
    HMI_SDK/Connect/SDLConnector.h \
    HMI_SDK/Connect/Channel.h \
    HMI_SDK/Connect/IMessageInterface.h \
    HMI_SDK/Connect/SDLConnector.h \
    HMI_SDK/Connect/ISocketManager.h \
    HMI_SDK/Connect/BasicCommunication.h \
    HMI_SDK/Connect/Buttons.h \
    HMI_SDK/Connect/Navigation.h \
    HMI_SDK/Connect/SocketsToSDL.h \
    HMI_SDK/Connect/TTS.h \
    HMI_SDK/Connect/UI.h \
    HMI_SDK/Connect/VehicleInfo.h \
    HMI_SDK/Connect/VR.h \
    Include/ProtocolDefines.h \
    HMI_Initial.h \
    Include/AppListInterface.h \
    Include/AppDataInterface.h \
    Include/UIInterface.h \
    Tools/json/assertions.h \
    Tools/json/autolink.h \
    Tools/json/config.h \
    Tools/json/features.h \
    Tools/json/forwards.h \
    Tools/json/json.h \
    Tools/json/json_tool.h \
    Tools/json/reader.h \
    Tools/json/value.h \
    Tools/json/version.h \
    Tools/json/writer.h \
    Include/AppCommon.h \
    HMI_SDK/Connect/CVideoStream.h


INCLUDEPATH +=  $$PWD/../Include/ffmpeg \
                $$PWD/../Include/msp   \
                $$PWD/../Include   \

###############################for windows
win32:!wince{
DEFINES +=WIN32
INCLUDEPATH += $$PWD/../Include/pthread \

#DEFINES += SDL_CALL_BACK \
#           SDL_SUPPORT_LIB
#LIBS +=  $$PWD/Library/win32/sdl/smartDeviceLinkCore.lib
#sdllib.path=$$OUT_PWD/../bin
#sdllib.files=$$PWD/Library/win32/sdl/*.*
#INSTALLS+=sdllib

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

pthread.path=$$OUT_PWD/../bin/
pthread.files=$$PWD/Library/win32/pthread/*.dll
INSTALLS+=pthread

ffmpeg.path=$$OUT_PWD/../bin
ffmpeg.files=$$PWD/Library/win32/ffmpeg/*.dll
INSTALLS+=ffmpeg
qt_dll.path=$$OUT_PWD/../bin
qt_dll.files=$$QMAKE_LIBDIR_QT/bin/*.dll
INSTALLS +=qt_dll

#LIBS +=  -L$$OUT_PWD/../bin/UILib -lGen3UI
}


################################for linux


################################for wince
wince{
HEADERS += \
    Include/global_first.h
INCLUDEPATH += $$PWD/../Include/pthread \
               $$PWD/../Include \
               $$PWD/../Include/wince
LIBS +=  $$PWD/Library/ce/pthread.lib
LIBS += -L$$PWD/Library/ce/ffmpeg  -lavcodec-55  -lavdevice-55 -lavfilter-3 -lavformat-55 -lavutil-52 -lswresample-0 -lswscale-2
pthread.path=$$OUT_PWD/../bin
pthread.files=$$PWD/Library/ce/*.dll
ffmpeg.path=$$OUT_PWD/../bin
ffmpeg.files=$$PWD/Library/ce/ffmpeg/*.dll

INSTALLS +=pthread
INSTALLS+=ffmpeg
qt_dll.path=$$OUT_PWD/../bin
qt_dll.files=$$QMAKE_LIBDIR_QT/*.dll
INSTALLS +=qt_dll

#DEFINES += SDL_CALL_BACK

DEFINES +=  OS_WIN32\
#            SDL_SUPPORT_LIB
sdl_lib.path=$$OUT_PWD/../bin
sdl_lib.files=$$PWD/Library/ce/sdl/*.*
INSTALLS +=sdl_lib

#DEFINES += OS_WIN32

LIBS +=  $$PWD/Library/ce/sdl/smartDeviceLinkCore.lib
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

RESOURCES += \
    Library/android/sdl/config/android.qrc \
    Config/config.qrc

QT += androidextras
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android
DISTFILES += \
    ../android/AndroidManifest.xml \
    ../android/src/an/qt/useJar/ExtendsQtNative.java \
    ../android/src/an/qt/useJar/ExtendsQtSurface.java \
    ../android/src/an/qt/useJar/ExtendsQtWithJava.java

}


!android{
configfile.path=$$OUT_PWD/../bin/Config
configfile.files=$$PWD/Config/*
INSTALLS +=configfile
}



#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Library/android/apk

