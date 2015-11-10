#ifndef CONFIG_H
#define CONFIG_H
#include "../AppInclude.h"
#include "../Singleton.h"

enum
{
    W800_H480=0,
    W1024_H600=1
};

class Config
{
public:
    Config();
    ~Config();

    void loadResolution(int resolution);

    int getItemW(){return m_i_itemW;}
    int getItemH(){return m_i_itemH;}
    int getMainWindowW(){return m_i_mainWindowW;}
    int getMainWindowH(){return m_i_mainWindowH;}

    int getShowListW(){return m_i_showListW;}
    int getShowListH(){return m_i_showListH;}
    int getShowBtnW(){return m_i_showBtnW;}
    int getShowBtnH(){return m_i_showBtnH;}
    int getShowScrollH(){return m_i_showScrollH;}

    int getCommandScrollH(){return m_i_commandScrollH;}

    int getAlertBtnW(){return m_i_alertBtnW;}
    int getAlertBtnH(){return m_i_alertBtnH;}



private:
    int m_i_mainWindowW;
    int m_i_mainWindowH;

    int m_i_itemW;
    int m_i_itemH;

    int m_i_showListW;
    int m_i_showListH;

    int m_i_showBtnW;
    int m_i_showBtnH;

    int m_i_showScrollH;
    int m_i_commandScrollH;

    int m_i_alertBtnW;
    int m_i_alertBtnH;

};
typedef Singleton<Config> ConfigSingle;

#endif // CONFIG_H
