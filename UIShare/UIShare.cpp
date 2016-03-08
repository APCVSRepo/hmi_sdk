#include "UILib.h"

#include "UIManager.h"

CUIManager *g_pUIManager;

UIInterface *UILib_Init(AppListInterface *pList)
{
    g_pUIManager = new CUIManager(pList);
    g_pUIManager->initAppHMI();
    return g_pUIManager;
}

void UILib_Close()
{
    delete g_pUIManager;
}
