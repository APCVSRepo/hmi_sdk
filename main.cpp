#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "UI/Config/Config.h"
#include <QDialog>
#include "HMI_Initial.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    //将字体文件名传给addApplicationFont,得到字体的Id
    int fontId = QFontDatabase::addApplicationFont(":/LiberationSerif-Regular.ttf");
    LOGI("---fontId=%d",fontId);
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString msyh = QFontDatabase::applicationFontFamilies ( fontId ).at(0);
    QFont font(msyh,10);
    //将此字体设为QApplication的默认字体
    QApplication::setFont(font);


    HMI_Initial appManager;
    QDialog diaStart;
    diaStart.setGeometry(0,0,ui_res_width,ui_res_height);
    diaStart.setStyleSheet("border-image:url(:/images/Screen.png);");
    QObject::connect(&appManager,SIGNAL(finishMainHMI()),&diaStart, SLOT(accept()));
    diaStart.exec();
    appManager.ShowUI();

    return a.exec();
}
