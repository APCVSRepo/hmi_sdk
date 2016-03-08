#include "UILib.h"
#include "Gen3UIManager.h"

CGen3UIManager *g_pUIManager;

UIInterface *UILib_Init(AppListInterface *pList)
{
    g_pUIManager = new CGen3UIManager(pList);
    g_pUIManager->initAppHMI();
    return g_pUIManager;
}

void UILib_Close()
{
    delete g_pUIManager;
}
