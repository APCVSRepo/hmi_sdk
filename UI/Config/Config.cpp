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

            m_i_showBtnW = 160;
            m_i_showBtnH = 70;

            m_i_showScrollH = 220;
            m_i_commandScrollH = 280;

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

        m_i_showBtnW = 190;
        m_i_showBtnH = 80;

        m_i_showScrollH = 240;
        m_i_commandScrollH = 350;

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
