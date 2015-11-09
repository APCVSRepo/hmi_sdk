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

            m_i_showListW = 400;
            m_i_showListH = 150;

            m_i_alertBtnW = 122;
            m_i_alertBtnH = 60;

            break;
        }
        case W1024_H600:
    {
        m_i_mainWindowW = 1024;
#ifdef ANDROID
        m_i_mainWindowH = 480-60;
#else
        m_i_mainWindowH = 600;
#endif
        m_i_itemW = 800;
        m_i_itemH = 100;

        m_i_showListW = 600;
        m_i_showListH = 210;

        m_i_alertBtnW = 170;
        m_i_alertBtnH = 75;

        break;
    }
            break;
        default:
            break;
    }
//    m_i_tcpPort = 10000;
}
