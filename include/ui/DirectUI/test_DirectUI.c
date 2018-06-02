

#include <stdio.h>
#include <tchar.h>
#include "debug.h"
#include "DirectUI.h"
#include "UIutil.inl"
#include "Resource.inl"
#include "UIRender.inl"
#include "DirectUI.inl"
#include "LabelUI.inl"
#include "coolscroll.inl"
#include "ScrollbarBase.inl"
#include "ScrollbarUI.inl"
#include "ContainerUI.inl"
#include "GifAnimationUI.inl"
#include "Layout.inl"
#include "CoolEdit.inl"
#include "ListCtrl.inl"
#include "TreeList.inl"
#include "GridCtrl.inl"
//#include "mycell.inl"
#include "cam.inl"
#include "Button.inl"
#include "UIDlgBuilder.inl"

int test_LoadGIF() {
  ImageSet s = {0};
  LoadGIF(&s, "bluemeaniefish.gif");
  ImageSet_Free(&s);
  return 0;
}

int OnFinalMessage( CWindowWnd* ww ) {
  UIResource* res = ww->m_pResource;
  UIResource_Release(res);
  ReleaseWnd(ww);
  return 0;
}
int Notify( CWindowWnd* ww, TNotifyUI* msg ) {
  printf("%10s %10s %15s\n", msg->sType, msg->pSender->m_sName, msg->pSender->vtable->Name);
  return 0;
}

int test_DirectUI() {
  CControlUI* btn;
  //CControlUI* lab;
  CWindowWnd* win;
  UIResource* res;
  res = CreateUIResource();
  win = CreateWnd("test_DirectUI", 0,0,300, 300);
  win->m_pResource = res;
  win->Final = OnFinalMessage;
  RegisterButton();
  //btn = CreateControl("button");
  btn = CreateControl("label");
  UILabel_SetAttribute(res, btn, "text", "ÄãºÃABCabc123");
  UIAttachDialog(win, btn);
  ShowWindow(win->m_hWnd, 1);
  MessageLoop();
  return 0;
}
int a_GetList(void* pListData, int nItem, int nSubItem, CListInfo* pListInfo, CListColumn* pListColumn, CSubItem* pSubItem) {
  if (pListInfo) {
    memset(pListInfo, 0, sizeof(CListInfo));
    pListInfo->nColumnCount = 5;
    pListInfo->nItemCount = 5;
  }
  if (pListColumn) {
    memset(pListColumn, 0, sizeof(CListColumn));
    _snprintf(pListColumn->m_strText, 256, "col %d", nSubItem);
    pListColumn->m_nWidth = 80;
    pListColumn->m_nImage = ITEM_IMAGE_NONE;
  }
  if (pSubItem) {
    memset(pSubItem, 0, sizeof(CSubItem));
    if (nSubItem==1) {
      _snprintf(pSubItem->m_strText, 256, "%d", nItem*10);
      pSubItem->m_nItemFormat = ITEM_FORMAT_PROGRESS;
    } else {
      _snprintf(pSubItem->m_strText, 256, "item %d,%d", nItem, nSubItem);
    }
  }
  return 0;
}
int test_UIListCtrl( CControlUI* ctr ) {
  static UIListCtrlCallBack cb;
  UIListCtrl* list;
  cb.f_GetList = a_GetList;
  list = (UIListCtrl*)FindControlByName(ctr, "list");
  ASSERT(0==strcmp(list->base.base.vtable->Name, "ListCtrl"));
  if (list) {
    list->cb = &cb;
    list->m_nItemHeight = 20;
    list->m_nHeaderHeight = 20;
  }
  return 0;
}
int test_DirectUI2() {
  CControlUI* btn;
  //CControlUI* lab;
  CWindowWnd* win;
  UIResource* res;
  STRINGorID xml;
  xml.s = "test.xml";
  res = CreateUIResource();
  win = CreateWnd("test_DirectUI", 0,0,300, 300);
  win->m_pResource = res;
  RegisterButton();
  win->Final = OnFinalMessage;
  UIAddNotifier(win, Notify);
  //btn = CreateControl("button");
  btn = CreateControlByXml(xml, 0, res);
  test_UIListCtrl(btn);
  UIScrollbar_SetAttribute2(res, btn, "VScrollBar");
  UIAttachDialog(win, btn);
  ShowWindow(win->m_hWnd, 1);
  MessageLoop();
  return 0;
}

int main() {
  _chdir("F:/code/cstd/src/uictr/duilib/bin/test_duilib");
  MEM_CHECK_BEGIN();
  test_DirectUI2();
  MEM_CHECK_END();
  return 0;
}
