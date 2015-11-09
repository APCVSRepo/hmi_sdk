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



private:
    int m_i_mainWindowW;
    int m_i_mainWindowH;

    int m_i_itemW;
    int m_i_itemH;
//    int m_i_tcpPort;

};
typedef Singleton<Config> ConfigSingle;

#endif // CONFIG_H
