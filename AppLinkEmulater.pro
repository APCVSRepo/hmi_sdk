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
DESTDIR=bin

target.path=$$OUT_PWD/bin
INSTALLS+=target

#qtdll.path=$$OUT_PWD/bin
#qtdll.files=$$QMAKE_LIBDIR/*.dll
#INSTALLS +=qtdll



SOURCES += \
    main.cpp \
    HMI_SDK/Connect/SDLConnector.cpp \
    HMI_SDK/Connect/Channel.cpp \
    Tools/json/json_writer.cpp \
    Tools/json/json_valueiterator.inl \
    Tools/json/json_value.cpp \
    Tools/json/json_reader.cpp \
    HMI_SDK/AppData/AppList.cpp \
    HMI_SDK/AppData/AppData.cpp \
    UI/Alert/AlertUI.cpp \
    UI/AudioPassThru/AudioPassThru.cpp \
    UI/Choiceset/ChoicesetVR.cpp \
    UI/Choiceset/Choiceset.cpp \
    UI/Command/Command.cpp \
    UI/Common/ScrollBar.cpp \
    UI/Common/Button.cpp \
    UI/Common/MainMenu.cpp \
    UI/Common/BaseWidght.cpp \
    UI/Common/AppBase.cpp \
    UI/Common/AppItemWidget.cpp \
    UI/Config/Config.cpp \
    UI/ScrollableMessage/ScrollMsg.cpp \
    UI/Show/Show.cpp \
    UI/UIManager.cpp \
    UI/Slider/Slider.cpp \  
    UI/Notify/Notify.cpp \
    UI/Common/Background.cpp \
    UI/VideoStream/VideoStream.cpp \
    UI/TextSpeech/textspeech.cpp \
    UI/AudioTrans/AudioInput.cpp \
    UI/AudioTrans/MspVRAudio.cpp \
    UI/TextSpeech/TextToSpeech.cpp \
    UI/AudioTrans/AudioOutput.cpp \
    AppManager.cpp \
    UI/Common/AppListWidget.cpp \
    UI/Common/MenuButton.cpp \
    UI/AppLinkMenu.cpp \
    UI/Common/CAppButton.cpp  \
    HMI_SDK/Connect/BasicCommunication.cpp \
    HMI_SDK/Connect/Buttons.cpp \
    HMI_SDK/Connect/Navigation.cpp \
    HMI_SDK/Connect/SocketsToSDL.cpp \
    HMI_SDK/Connect/TTS.cpp \
    HMI_SDK/Connect/UI.cpp \
    HMI_SDK/Connect/VehicleInfo.cpp \
    HMI_SDK/Connect/VR.cpp \
    UI/AppList/AppListUI.cpp




INCLUDEPATH += $$PWD/   \
              $$PWD/HMI_SDK \
              $$PWD/Tools  \
              $$PWD/UI

HEADERS  += HMI_SDK/AppData/AppListInterface.h \
    HMI_SDK/AppData/AppDataInterface.h \
    HMI_SDK/AppData/AppData.h \
    HMI_SDK/AppData/AppList.h \
    HMI_SDK/Connect/SDLConnector.h \
    HMI_SDK/Connect/Channel.h \
    HMI_SDK/Connect/IMessageInterface.h \
    HMI_SDK/Connect/SDLConnector.h \
    HMI_SDK/Connect/ISocketManager.h \
    Tools/json/version.h.in \
    Tools/json/json_tool.h \
    Tools/json/writer.h \
    Tools/json/version.h \
    Tools/json/value.h \
    Tools/json/reader.h \
    Tools/json/json.h \
    Tools/json/forwards.h \
    Tools/json/features.h \
    Tools/json/config.h \
    Tools/json/autolink.h \
    Tools/json/assertions.h \
    UI/Common/MainMenu.h \
    UI/Common/BaseWidght.h \
    UI/Common/AppBase.h \
    UI/UIInterface.h \
    UI/UIManager.h \
    UI/Alert/AlertUI.h \
    UI/AudioPassThru/AudioPassThru.h \
    UI/Choiceset/ChoicesetVR.h \
    UI/Choiceset/Choiceset.h \
    UI/Command/Command.h \
    UI/Common/ScrollBar.h \
    UI/Common/Button.h \
    UI/Common/AppItemWidget.h \
    UI/Config/Config.h \
    UI/ScrollableMessage/ScrollMsg.h \
    UI/Show/Show.h \
    UI/Slider/Slider.h \
    UI/Notify/Notify.h \
    UI/Common/Background.h \
    UI/VideoStream/VideoStream.h \
    UI/TextSpeech/textspeech.h \
    UI/AudioTrans/AudioInput.h \
    UI/AudioTrans/MspVRAudio.h \
    UI/TextSpeech/TextToSpeech.h \
    UI/Common/MenuButton.h \
    UI/AppLinkMenu.h \
    UI/Common/CAppButton.h \
    UI/AudioTrans/AudioOutput.h \
    AppManager.h \
    HMI_SDK/AppData/AppCommon.h \
    HMI_SDK/Connect/BasicCommunication.h \
    HMI_SDK/Connect/Buttons.h \
    HMI_SDK/Connect/Navigation.h \
    HMI_SDK/Connect/SocketsToSDL.h \
    HMI_SDK/Connect/TTS.h \
    HMI_SDK/Connect/UI.h \
    HMI_SDK/Connect/VehicleInfo.h \
    HMI_SDK/Connect/VR.h \
    Include/ProtocolDefines.h \
    UI/Common/AppListWidget.h \
    UI/UIInclude.h \
    UI/AppList/AppListUI.h


RESOURCES += \
    UI/images.qrc

OTHER_FILES += \
    UI/LiberationSerif-Regular.ttf

INCLUDEPATH +=  $$PWD/Include/ffmpeg \
                $$PWD/Include/msp

###############################for windows
win32:!wince{
DEFINES +=WIN32
INCLUDEPATH += $$PWD/Include/pthread \
               $$PWD/Include
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

pthread.path=$$OUT_PWD/bin
pthread.files=$$PWD/Library/win32/*.dll
ffmpeg.path=$$OUT_PWD/bin
ffmpeg.files=$$PWD/Library/win32/ffmpeg/*.dll  \
$$PWD/Library/win32/pthread/*.dll
INSTALLS+=pthread
INSTALLS+=ffmpeg
qt_dll.path=$$DESTDIR
qt_dll.files=$$(QT_DIR)/bin/*.dll
INSTALLS +=qt_dll
}


################################for linux
unix:!android:LIBS += -L$$PWD/Library/linux/ffmpeg -lavcodec  -lavformat -lavutil -lswscale

################################for wince
wince{
HEADERS += \
    Include/global_first.h \
    Include/unistd.h \
    Include/stdint.h
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
    Library/android/sdl/config/android.qrc \
    Config/config.qrc
}


!android{
configfile.path=$$OUT_PWD/bin/Config
configfile.files=$$PWD/Config/*
INSTALLS +=configfile
}



#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Library/android/apk

