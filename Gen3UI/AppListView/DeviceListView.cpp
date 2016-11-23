#include "DeviceListView.h"

CDeviceListView::CDeviceListView(AppListInterface * pList,QWidget *parent)
    :QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    if (parent){
        setGeometry(0,0,parent->width(),parent->height());
        m_AppWidth = parent->width() / 4;
        m_AppHeight = parent->height() / 2;
    }

    setAutoFillBackground(true);
    QPixmap pixmap(":/images/applinkmain.png");
    pixmap = pixmap.scaled(width(),height(),
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    setPalette(palette);

    m_pList = pList;

    QString appsheet_on[2] = {":/images/phonechild_on.png",
                              ":/images/listchild_on.png"};
    QString appsheet_off[2] = {":/images/phonechild_off.png",
                               ":/images/listchild_off.png"};
    //QString childText[2]={"Find New App","App Setting"};
    /*
    QString childText[2] = {"连接设备","连接移动应用程序"};
    int funcId[2] = {-1,-2};
    for (int i = 0; i < 2; ++i) {
        InsertChildApp(i,funcId[i],childText[i],
                       appsheet_on[i],appsheet_off[i]);
    }*/
}

CDeviceListView::~CDeviceListView()
{
    for (int i = 0; i != m_pDevices.size(); ++i) {
        delete m_pDevices[i];
    }
    m_pDevices.clear();
}

void CDeviceListView::onDeviceSelected(int id)
{
    //m_pList->OnDeviceSelect(id);
}

void CDeviceListView::InsertDevice(int index,int appId,QString text,
                                  QString on,QString off,bool bPaint)
{
    for (int i = index; i < m_pDevices.size(); ++i) {
        CAppButton *button = m_pDevices.at(i);
        int r = (i+1)/4;
        int c = (i+1)%4;
        button->setGeometry(5+m_AppWidth*c,5+m_AppHeight*r,
                            m_AppWidth-10,m_AppHeight-10);
    }
    CAppButton *newbutton = new CAppButton(this);
    int r = index/4;
    int c = index%4;
    newbutton->setGeometry(5 + m_AppWidth*c, 5 + m_AppHeight*r,
                           m_AppWidth - 10, m_AppHeight - 10);
    //newbutton->setStringId(appId);
    newbutton->setIcon(on, off,bPaint);
    newbutton->setText(text);
    newbutton->show();
    connect(newbutton, SIGNAL(clickedWitchFuncId(int)),
            SLOT(onDeviceSelected(int)));
    m_pDevices.insert(index, newbutton);
}

void CDeviceListView::DeleteDevice(int index)
{
    CAppButton *button = m_pDevices.at(index);
    m_pDevices.removeAt(index);
    disconnect(button,SIGNAL(clickedWitchFuncId(int)),
               this,SLOT(onDeviceSelected(int)));
    delete button;
    for (int i = index;i<m_pDevices.size();++i) {
        CAppButton *button = m_pDevices.at(i);
        int r = i/4;
        int c = i%4;
        button->setGeometry(5+m_AppWidth*c,5+m_AppHeight*r,
                            m_AppWidth-10,m_AppHeight-10);
    }
}

void CDeviceListView::showEvent(QShowEvent * e)
{
    std::vector<int> vAppIDs;
    std::vector<std::string> vAppNames;
    std::vector<std::string> vIconPath;
    //m_pList->getDeviceList(vAppIDs, vAppNames,vIconPath);
    int count = m_pDevices.size();
    for (int i = 2; i < count; ++i) {
        DeleteDevice(2);
    }
    if (vAppIDs.size() > 0) {
        for (int i = 0; i < vAppIDs.size(); ++i) {
            InsertDevice(2+i,vAppIDs.at(i),
                           vAppNames.at(i).c_str(),
                           vIconPath.at(i).c_str(),
                           vIconPath.at(i).c_str(),
                           true);
        }
    }
}
