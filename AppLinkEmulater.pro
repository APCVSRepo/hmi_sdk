#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T16:04:10
#
#-------------------------------------------------
QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:CONFIG += qaxcontainer

TARGET = AppLinkEmulater
TEMPLATE = app

SOURCES += \
    main.cpp \
    HMISDK/connect/vrclient.cpp \
    HMISDK/connect/vehicleinfoclient.cpp \
    HMISDK/connect/uiclient.cpp \
    HMISDK/connect/ttsclient.cpp \
    HMISDK/connect/socketstosdl.cpp \
    HMISDK/connect/SDLConnector.cpp \
    HMISDK/connect/navigationclient.cpp \
    HMISDK/connect/Channel.cpp \
    HMISDK/connect/buttonsclient.cpp \
    HMISDK/connect/basecommunicationclient.cpp \
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
    HMISDK/utils/jsoncpp/json_writer.cpp \
    HMISDK/utils/jsoncpp/json_valueiterator.inl \
    HMISDK/utils/jsoncpp/json_value.cpp \
    HMISDK/utils/jsoncpp/json_reader.cpp \
    UI/Notify/Notify.cpp \
    UI/Common/Background.cpp \
#    UI/VideoStream/VideoStream.cpp \
    UI/TextSpeech/textspeech.cpp \
    Log.cpp \
    HMISDK/AppData/AppData.cpp \
    HMISDK/AppData/AppList.cpp


HEADERS  += \
    HMISDK/include/connect/vrclient.h \
    HMISDK/include/connect/vehicleinfoclient.h \
    HMISDK/include/connect/uiclient.h \
    HMISDK/include/connect/ttsclient.h \
    HMISDK/include/connect/sockectstosdl.h \
    HMISDK/include/connect/SDLConnector.h \
    HMISDK/include/connect/navigationclient.h \
    HMISDK/include/connect/connect.h \
    HMISDK/include/connect/Channel.h \
    HMISDK/include/connect/buttonsclient.h \
    HMISDK/include/connect/basecommunicationclient.h \
    HMISDK/include/protocolHandler/IMessageInterface.h \
    UIcallBack.h \
    UI/AppLink.h \
    UI/AppInclude.h \
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
    UI/UIManager.h \
    UI/Slider/Slider.h \
    HMISDK/utils/jsoncpp/version.h.in \
    HMISDK/utils/jsoncpp/json_tool.h \
    HMISDK/include/json/writer.h \
    HMISDK/include/json/version.h \
    HMISDK/include/json/value.h \
    HMISDK/include/json/reader.h \
    HMISDK/include/json/json.h \
    HMISDK/include/json/forwards.h \
    HMISDK/include/json/features.h \
    HMISDK/include/json/config.h \
    HMISDK/include/json/autolink.h \
    HMISDK/include/json/assertions.h \
    UI/Notify/Notify.h \
    UI/Common/Background.h \
    HMISDK/include/SDLConnector.h \
    HMISDK/include/IMessageInterface.h \
    HMISDK/include/connect/ISocketManager.h \
    UI/UIInterface.h \
#    UI/VideoStream/VideoStream.h \
    UI/TextSpeech/textspeech.h \
    Log.h \
    HMISDK/AppData/AppData.h \
    HMISDK/AppData/AppDataInterface.h \
    HMISDK/AppData/AppList.h \
    HMISDK/AppData/AppListInterface.h



RESOURCES += \
    UI/images.qrc

OTHER_FILES += \
    UI/LiberationSerif-Regular.ttf

#INCLUDEPATH +=  $$PWD/ffmpeg/include

###############################for windows
INCLUDEPATH += $$PWD/HMISDK/include/pthread
win32: LIBS +=  $$PWD/HMISDK/lib/pthreadVC2.lib
win32: LIBS +=  $$PWD/HMISDK/lib/pthreadVCE2.lib
win32: LIBS +=  $$PWD/HMISDK/lib/pthreadVSE2.lib
win32: LIBS +=  $$PWD/HMISDK/lib/WS2_32.Lib
#win32: LIBS += -L$$PWD/ffmpeg/lib -lavcodec -lavfilter -lavformat -lavutil -lswscale


################################for linux
#LIBS += -L$$PWD/ffmpeg/so -lavcodec -lavfilter -lavformat -lavutil -lswscale


################################for wince
#INCLUDEPATH +=HMISDK/include
#HEADERS += \
#    HMISDK/include/global_first.h \
#    HMISDK/include/unistd.h \
#    HMISDK/include/stdint.h
#LIBS +=  $$PWD/HMISDK/lib/wince/pthread.lib
##LIBS += -L$$PWD/ffmpeg/ce/ -lavcodec -lavfilter -lavformat -lavutil -lswscale
#LIBS += $$PWD/ffmpeg/ce/libavcodec.dll.a  $$PWD/ffmpeg/ce/libavfilter.dll.a  $$PWD/ffmpeg/ce/libavformat.dll.a  $$PWD/ffmpeg/ce/libswscale.dll.a  $$PWD/ffmpeg/ce/libavutil.dll.a



################################for android
#LIBS += -L$$PWD/ffmpeg/android -lffmpeg

