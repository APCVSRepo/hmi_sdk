#include "Config.h"

Config::Config()
{

}

Config::~Config()
{

}

void Config::loadResolution(int resolution)
{
    switch(resolution)
    {
        case W800_H480:
        {
            m_i_mainWindowW = 800;
#ifdef ANDROID
            m_i_mainWindowH = 480-60;
#else
            m_i_mainWindowH = 480;
#endif
            m_i_itemW = 600;
            m_i_itemH = 80;

            break;
        }
        case W1024_H600:
            break;
        default:
            break;
    }
    m_i_tcpPort = 10000;
}
