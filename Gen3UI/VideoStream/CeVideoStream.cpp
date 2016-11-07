#include "CeVideoStream.h"
#include "main.h"

CeVideoStream::CeVideoStream(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    if (parent) {
        setGeometry(0,0,parent->width(),parent->height());
    }

#ifdef SDL_CALL_BACK
    sdl_set_videostream_callback(callBack_send_data);
#endif
}

CeVideoStream::~CeVideoStream()
{
}

void CeVideoStream::startStream()
{
#ifdef TEST_FILE
    fp = fopen("./VideoFile.mp4", "ab+");
#endif
}

void CeVideoStream::stopStream()
{
#ifdef TEST_FILE
    fclose(fp);
#endif
}

#ifdef SDL_CALL_BACK
#define TMP_BUF_LEN 100
static uchar m_tmpBuf[TMP_BUF_LEN + 2048] = {0};
static int offset = 0;
void CeVideoStream::callBack_send_data(const char *data, int size)
{
    static bool bb = false;
    if(!bb){
        printf("wince videostream callback\n");
        bb = true;
    }
}
#endif

void CeVideoStream::onRawData(void *p, int iLength)
{
    static bool bb = false;
    if(!bb){
        printf("wince videostream socket\n");
        bb = true;
    }
#ifdef TEST_FILE
    fwrite(p,iLength,1,fp);
#endif
}
