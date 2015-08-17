#ifndef CONFIG_H
#define CONFIG_H
#include "../AppInclude.h"

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

    int m_i_mainWindowW;
    int m_i_mainWindowH;

    int m_i_itemW;
    int m_i_itemH;
    int m_i_tcpPort;
private:

};

#endif // CONFIG_H
