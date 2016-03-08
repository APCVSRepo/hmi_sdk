#ifndef APPINCLUDE_H
#define APPINCLUDE_H

#include <QDebug>
#include <QString>
#include <QFile>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QTextStream>

#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>

#include <QBitmap>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QSlider>

#include <QVector>
#include <QTableWidget>
#include <QScrollBar>
#include <QLineEdit>
#include "UI/Config/Config.h"

#include <stdio.h>
//#define _D(msg, args...) fprintf(stderr,"%s-%s():%d: ", __FILE__, __FUNCTION__, __LINE__, ##args)
//#define _D   printf
#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

#endif // APPINCLUDE_H
