#include "Log.h"

void toUnicode(const char *strSrc, unsigned int nCodePage, wchar_string &wstrOut)
{
#if defined(WINCE)
    int nLength = MultiByteToWideChar(nCodePage, 0, strSrc, -1, NULL, 0);
    wstrOut.clear();
    wstrOut.resize(nLength);
    MultiByteToWideChar(nCodePage, 0, strSrc, -1, (wchar_t*)&*wstrOut.begin(), nLength);
    wstrOut.erase(--(wstrOut.end())); // remove last '\0'
#endif
}
int logOut(const char *file, const char *func, const int line, const char *fmt, ...)
{
    char msg[MAX_MSG]={0};
    if(NULL != fmt)
    {
        va_list ap;
        va_start(ap, fmt);
        _vsnprintf(msg, sizeof(msg), fmt, ap);
        va_end(ap);
    }

//   QTime timeNow=QTime::currentTime();

    char filemsg[MAX_MSG]={0};
    sprintf(filemsg,"%s():%d  %s\n", func, line, msg);
//    sprintf(filemsg,"%s %s:%s():%d  %s\n",timeNow.toString().toLatin1().data(), file, func, line, msg);

    qDebug()<<filemsg;

//#if defined(WINCE)
//    FILE *fp = fopen("Storage Card/hmi.log","a+");
//#else
//    FILE *fp = fopen("./hmi.log","a+");
//#endif
//    fwrite(filemsg,1,strlen(filemsg),fp);
//    fclose(fp);

#if defined(WINCE)
    wchar_string outMsg;
    toUnicode(filemsg,CP_ACP,outMsg);
    RETAILMSG(1,(L"%s", outMsg.c_str()));
#endif

    return 0;
}

