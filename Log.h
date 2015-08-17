#ifndef LOG_H
#define LOG_H


#include <stdlib.h>
#include <time.h>
#include <QTime>
#include <QString>
#include <QDebug>
#include <windows.h>
#include <string>
typedef std::basic_string<wchar_t> wchar_string;

#define MAX_MSG     10000
#define DBG(fmt,...)  logOut(__FILE__,__FUNCTION__,__LINE__,fmt"",##__VA_ARGS__)

void toUnicode(const char *strSrc, unsigned int nCodePage, wchar_string &wstrOut);
int logOut(const char *file, const char *func, const int line, const char *fmt, ...);

#endif // LOG_H
