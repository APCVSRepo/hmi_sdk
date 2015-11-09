#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "AppData/AppList.h"
#include "UI/UIManager.h"
#include "UI/Config/Config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ConfigSingle::Instance()->loadResolution(W800_H480);
    ConfigSingle::Instance()->loadResolution(W1024_H600);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    AppList m_appList;
    CUIManager m_ui;

    m_ui.setAppListInterface(&m_appList);
    m_ui.init();

    m_appList.setUIManager(&m_ui);
    m_appList.start();

    return a.exec();
}
