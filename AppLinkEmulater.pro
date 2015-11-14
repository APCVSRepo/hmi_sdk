#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T16:04:10
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:CONFIG += qaxcontainer

#CONFIG  += wince  ##非wince 平台需要屏�?

TARGET = AppLinkDevice
TEMPLATE = app

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin

target.path=$$OUT_PWD/bin
INSTALLS+=target

qtdll.path=$$OUT_PWD/bin
qtdll.files=$$QMAKE_LIBDIR/*.dll
INSTALLS +=qtdll



SOURCES += \
    main.cpp \
    HMI_SDK/Connect/vrclient.cpp \
    HMI_SDK/Connect/vehicleinfoclient.cpp \
    HMI_SDK/Connect/uiclient.cpp \
    HMI_SDK/Connect/ttsclient.cpp \
    HMI_SDK/Connect/socketstosdl.cpp \
    HMI_SDK/Connect/SDLConnector.cpp \
    HMI_SDK/Connect/navigationclient.cpp \
    HMI_SDK/Connect/Channel.cpp \
    HMI_SDK/Connect/buttonsclient.cpp \
    HMI_SDK/Connect/basecommunicationclient.cpp \
    Tools/json/json_writer.cpp \
    Tools/json/json_valueiterator.inl \
    Tools/json/json_value.cpp \
    Tools/json/json_reader.cpp \
    HMI_SDK/AppData/AppList.cpp \
    HMI_SDK/AppData/AppData.cpp \
    UI/AppLink.cpp \
    UI/Alert/AlertUI.cpp \
    UI/AudioPassThru/AudioPassThru.cpp \
    UI/Choiceset/ChoicesetVR.cpp \
    UI/Choiceset/Choiceset.cpp \
    UI/Command/Command.cpp \
    UI/Common/ScrollBar.cpp \
    UI/Common/PopBase.cpp \
    UI/Common/Button.cpp \
    UI/Common/AppItemWidget.cpp \
    UI/Common/AppBase.cpp \
    UI/Config/Config.cpp \
    UI/ScrollableMessage/ScrollMsg.cpp \
    UI/Show/Show.cpp \
    UI/UIManager.cpp \
    UI/Slider/Slider.cpp \  
    UI/Notify/Notify.cpp \
    UI/Common/Background.cpp \
    UI/VideoStream/VideoStream.cpp \
    UI/TextSpeech/textspeech.cpp

INCLUDEPATH += $$PWD/   \
              $$PWD/HMI_SDK  \
              $$PWD/Tools

HEADERS  += HMI_SDK/AppData/AppListInterface.h \
    HMI_SDK/AppData/AppDataInterface.h \
    HMI_SDK/AppData/AppData.h \
    HMI_SDK/Connect/vrclient.h \
    HMI_SDK/Connect/vehicleinfoclient.h \
    HMI_SDK/Connect/uiclient.h \
    HMI_SDK/Connect/ttsclient.h \
    HMI_SDK/Connect/sockectstosdl.h \
    HMI_SDK/Connect/SDLConnector.h \
    HMI_SDK/Connect/navigationclient.h \
    HMI_SDK/Connect/connect.h \
    HMI_SDK/Connect/Channel.h \
    HMI_SDK/Connect/buttonsclient.h \
    HMI_SDK/Connect/basecommunicationclient.h \
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
    UI/AppLink.h \
    UI/AppInclude.h \
    UI/UIInterface.h \
    UI/UIManager.h \
    UI/Alert/AlertUI.h \
    UI/AudioPassThru/AudioPassThru.h \
    UI/Choiceset/ChoicesetVR.h \
    UI/Choiceset/Choiceset.h \
    UI/Command/Command.h \
    UI/Common/ScrollBar.h \
    UI/Common/PopBase.h \
    UI/Common/Button.h \
    UI/Common/AppItemWidget.h \
    UI/Common/AppBase.h \
    UI/Config/Config.h \
    UI/ScrollableMessage/ScrollMsg.h \
    UI/Show/Show.h \
    UI/Slider/Slider.h \
    UI/Notify/Notify.h \
    UI/Common/Background.h \
    UI/VideoStream/VideoStream.h \
    UI/TextSpeech/textspeech.h


RESOURCES += \
    UI/images.qrc

OTHER_FILES += \
    UI/LiberationSerif-Regular.ttf

INCLUDEPATH +=  $$PWD/Include/ffmpeg

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

pthread.path=$$DESTDIR
pthread.files=$$PWD/Library/win32/*.dll
ffmpeg.path=$$DESTDIR
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
#LIBS += -L$$PWD/lib/ce/ffmpeg -lavcodec -lavfilter -lavformat -lavutil -lswscale
#LIBS += $$PWD/ffmpeg/ce/libavcodec.dll.a  $$PWD/ffmpeg/ce/libavfilter.dll.a  $$PWD/ffmpeg/ce/libavformat.dll.a  $$PWD/ffmpeg/ce/libswscale.dll.a  $$PWD/ffmpeg/ce/libavutil.dll.a
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
DEFINES +=ANDROID
LIBS += -L$$PWD/Library/android/ffmpeg -lffmpeg
LIBS += -L$$PWD/Library/android/sdl -lsmartDeviceLinkCore
#sdlcfg.path=$$OUT_PWD/android_build/assets
#sdlcfg.files=$$PWD/Library/android/sdl/config/*
#INSTALLS +=sdlcfg
}
!android{
configfile.path=$$DESTDIR/Config
configfile.files=$$PWD/Config/*
INSTALLS +=configfile
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/Library/android/ffmpeg/libffmpeg.so \
        $$PWD/Library/android/sdl/libsmartDeviceLinkCore.so
}

#DISTFILES += \
#    Library/android/apk/gradle/wrapper/gradle-wrapper.jar \
#    Library/android/apk/AndroidManifest.xml \
#    Library/android/apk/res/values/libs.xml \
#    Library/android/apk/build.gradle \
#    Library/android/apk/gradle/wrapper/gradle-wrapper.properties \
#    Library/android/apk/gradlew \
#    Library/android/apk/gradlew.bat

#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Library/android/apk
DISTFILES +=android/assets/*
