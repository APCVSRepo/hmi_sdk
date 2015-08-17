#-------------------------------------------------
#
# Project created by QtCreator 2015-04-09T09:25:45
#
#-------------------------------------------------
#QMAKE_CXXFLAGS += -std=c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SDK
TEMPLATE = app

SOURCES += \
    HMISDK/main.cpp\
    HMISDK/threaddlg.cpp \
    HMISDK/connect/buttonsclient.cpp \
    HMISDK/connect/navigationclient.cpp \
    HMISDK/connect/ttsclient.cpp \
    HMISDK/connect/uiclient.cpp \
    HMISDK/connect/vehicleinfoclient.cpp \
    HMISDK/connect/vrclient.cpp \
    HMISDK/connect/basecommunicationclient.cpp \
    HMISDK/utils/stringutil.cpp \
    HMISDK/connect/socketstosdl.cpp \
    HMISDK/connect/Channel.cpp \
    HMISDK/connect/SDLConnector.cpp \
    HMISDK/protocolHandler/MessageCallback.cpp \
    HMISDK/utils/jsoncpp/json_reader.cpp \
    HMISDK/utils/jsoncpp/json_value.cpp \
    HMISDK/utils/jsoncpp/json_writer.cpp

HEADERS  += \
    HMISDK/include/threaddlg.h \
    HMISDK/include/connect/basecommunicationclient.h \
    HMISDK/include/connect/buttonsclient.h \
    HMISDK/include/connect/navigationclient.h \
    HMISDK/include/connect/ttsclient.h \
    HMISDK/include/connect/uiclient.h \
    HMISDK/include/connect/vehicleinfoclient.h \
    HMISDK/include/connect/vrclient.h \
    HMISDK/include/connect/Channel.h \
    HMISDK/include/connect/sockectstosdl.h\
    HMISDK/include/connect/connect.h \
    HMISDK/include/connect/SDLConnector.h \
    HMISDK/include/protocolHandler/IMessageInterface.h \
    HMISDK/include/protocolHandler/MessageCallback.h \
    HMISDK/include/json/assertions.h \
    HMISDK/include/json/autolink.h \
    HMISDK/include/json/config.h \
    HMISDK/include/json/features.h \
    HMISDK/include/json/forwards.h \
    HMISDK/include/json/json.h \
    HMISDK/include/json/reader.h \
    HMISDK/include/json/value.h \
    HMISDK/include/json/version.h \
    HMISDK/include/json/writer.h \
    HMISDK/utils/jsoncpp/json_tool.h \
    HMISDK/include/SDLConnector.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVC2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVC2d

INCLUDEPATH += $$PWD/HMISDK/include
DEPENDPATH += $$PWD/HMISDK/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVC2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVC2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVC2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVC2d.lib

DISTFILES += \
    lib/pthreadVCE2.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVC2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVC2d -lppa

INCLUDEPATH += $$PWD/HMISDK/include
DEPENDPATH += $$PWD/HMISDK/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVC2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVC2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVC2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVC2d.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVCE2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVCE2d

INCLUDEPATH += $$PWD/HMISDK/include
DEPENDPATH += $$PWD/HMISDK/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVCE2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVCE2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVCE2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVCE2d.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVSE2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/HMISDK/lib/ -lpthreadVSE2d

INCLUDEPATH += $$PWD/HMISDK/include
DEPENDPATH += $$PWD/HMISDK/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVSE2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/libpthreadVSE2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVSE2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/HMISDK/lib/pthreadVSE2d.lib
