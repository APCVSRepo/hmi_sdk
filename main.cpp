#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "HMISDK/AppData/AppList.h"
#include "UI/UIManager.h"
#include "UI/Config/Config.h"

Config g_config;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_config.loadResolution(W800_H480);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

//    //将字体文件名传给addApplicationFont,得到字体的Id
//    int fontId = QFontDatabase::addApplicationFont(":/LiberationSerif-Regular.ttf");
//    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
//    QString msyh = QFontDatabase::applicationFontFamilies ( fontId ).at(0);
//    QFont font(msyh,10);
//    //将此字体设为QApplication的默认字体
//    QApplication::setFont(font);

    AppList m_appList;
    CUIManager m_ui;



    m_appList.setUIManager(&m_ui);
    m_appList.start();

    m_ui.setAppListInterface(&m_appList);
    m_ui.init();

    return a.exec();
}
