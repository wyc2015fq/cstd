/*
 * <F> CWinDock.cpp   1.0
 *
 * Copyright 2003 Francisco Campos. All rights reserved.
 * BEYONDATA  PROPRIETARY. Use is subject to license terms. </F>
 *
 *       \|||/
 *       |o_o|
 *  ----o00o-------o00o---
 **/


#include "stdafx.h"




DockManager::DockManager()
{
  m_DockWindows.RemoveAll();
  m_Splits.RemoveAll();
  m_Aling = 0;
  m_lastAling = 0;
  m_Visible = TRUE;
  m_MiniDock = NULL;
  m_Splitter = IRECT(0, 0, 0, 0);
  m_OldSplitter = IRECT(0, 0, 0, 0);
  m_pParent = NULL;
  m_NumWins = 0;
  m_NumSep = 0;
  m_SplitMove = 0;
  m_DragSplitter = FALSE;
  m_DragSplitterInside = FALSE;
}

DockManager::~DockManager()
{
  m_DockWindows.RemoveAll();
  m_Splits.RemoveAll();

}

BOOL DockManager::IsFloating()
{
  return m_Aling == BS_ALIGN_FLOAT;
}

BOOL DockManager::IsVertical()
{
  return (m_Aling == BS_ALIGN_LEFT) || (m_Aling == BS_ALIGN_RIGHT);
}

BOOL DockManager::IsHorz()
{
  return m_Aling == BS_ALIGN_BOTTOM;
}
BOOL DockManager::IsLeft()
{
  return m_Aling == BS_ALIGN_LEFT;
}
BOOL DockManager::IsRight()
{
  return m_Aling == BS_ALIGN_RIGHT;
}
/////////////////////////////////////////////////////////////////////////////
// DockManager message handlers


//****************************************************************************
BOOL DockManager::OnEraseBkgnd(HDC hDC)
{
  // TODO: Add your message handler code here and/or call default
  CGDI pDC(hDC);

  if (m_NumWins == 0) {
    COLOR cbr;
    cbr.CreateSysColorBrush(CLR_BTNFACE);
    IRECT m_rect;
    GetClientRect(m_rect);
    pDC.FillRect(m_rect, &cbr);
    cbr.DeleteObject();

  }

  return TRUE;
}


BOOL DockManager::OnPaint(HDC hDC)
{
  IRECT rcClient;
  CPaintDC dc(GetSafeHwnd()); // device context for painting
  COLOR cbr;
  IRECT m_rectDraw;
  cbr = (GetRGBColorFace());
  GetClientRect(rcClient);
  CGDI    MemDC;
  CBitmap   m_BitmapMemDC;
  MemDC.CreateCompatibleDC(dc.m_hDC);
  m_BitmapMemDC.CreateCompatibleBitmap(dc.m_hDC, rcClient.Width(), rcClient.Height());

  CBitmap* m_bitmapOld = new CBitmap(MemDC.SelectObject(&m_BitmapMemDC));
  MemDC.FillRect(&rcClient, &cbr);

  //paint routines

  dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), MemDC.m_hDC,
      rcClient.l, rcClient.t, SRCCOPY);
  MemDC.SelectObject(m_bitmapOld);
  m_BitmapMemDC.DeleteObject();
  MemDC.DeleteDC();
  cbr.DeleteObject();
  m_bitmapOld->DeleteObject();
  delete m_bitmapOld;
  return TRUE;
}



BOOL DockManager::Create(DWORD dwStyle, const RECT& rect, CWin* pParentWnd, UINT nID)
{
  // TODO: Add your specialized code here and/or call the base class
  m_pParent = pParentWnd;
  m_Aling = nID;
  LPCTSTR lpclassname = RegisterDefaultClass(NULL, WS_CHILD | WS_VISIBLE);
  return CWin::Create(lpclassname, _T(""), dwStyle, rect, pParentWnd->GetSafeHwnd(), nID);
}

int DockManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

  // TODO: Add your specialized creation code here
  return 0;
}


BOOL DockManager::OnNcCalcSize(BOOL /*bCalcValidRects*/, LPNCCALCSIZE_PARAMS lpncsp)
{
  // adjust non-client area for border space
  //dependiendo de la alineacion se debe crear un pequeño borde
  switch (m_Aling) {
  case BS_ALIGN_LEFT:
    lpncsp->rgrc[0].r -= 4;
    break;

  case BS_ALIGN_BOTTOM:
    lpncsp->rgrc[0].t += 4;
    break;

  case BS_ALIGN_RIGHT:
    lpncsp->rgrc[0].l += 1;
    break;
  }

  return TRUE;
}

CContainer* DockManager::PointIsInCaption(IPOINT cp)
{
  int iNumW = GetNumVisibleWindows();

  for (int i = 0; i < iNumW; i++) { //wich is the size real of the child windows
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
    IRECT rcW;
    pWin->GetWindowRect(rcW);

    if (pWin->IsWindowVisible()) {
      rcW.b = rcW.t + 18;

      if (rcW.PtInRect(cp)) {
        return (CContainer*)pWin;
      }
    }
  }

  return NULL;

}

int DockManager::PointInDock(IPOINT cp, IRECT& rc, int& pos)
{
  int iNumW = GetNumVisibleWindows();

  for (int i = 0; i < iNumW; i++) { //wich is the size real of the child windows
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
    IRECT rcW;
    pWin->GetWindowRect(rcW);

    if (pWin->IsWindowVisible()) {
      //primero si es dock superior
      if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {
        IRECT rctop;
        rctop = rcW;
        rctop.t += 18;
        rctop.b = rctop.t + 22;

        if (rctop.PtInRect(cp)) {
          rc = rcW;
          pos = i;
          return 1;
        }

        //si es dock inferior
        rctop = rcW;
        rctop.t = rctop.b - 22;

        if (rctop.PtInRect(cp)) {
          rc = rcW;
          pos = i;
          return 2;
        }

      }
      else {
        IRECT rctop;
        rctop = rcW;
        rctop.r = rctop.l + 22;

        if (rctop.PtInRect(cp)) {
          rc = rcW;
          pos = i;
          return 1;
        }

        //si es dock inferior
        rctop = rcW;
        rctop.l = rctop.r - 22;

        if (rctop.PtInRect(cp)) {
          rc = rcW;
          pos = i;
          return 2;
        }
      }

    }
  }

  return 0;

}
void DockManager::OnNcPaint(HRGN hRgn)
{
  // TODO: Add your message handler code here
  // Do not call CControlBar::OnNcPaint() for painting messages
  // Tomo la misma rutina que se desarrolla para la clase
  // CGuiToolBarWnd.
  IRECT rcWindow;
  IRECT rcClient;
  CGDI  dc(::GetWindowDC(GetSafeHwnd()));
  CGDI      m_dc;   //contexto de dispositivo en memoria
  CBitmap   m_bitmap;
  //la idea es tomar el area de la ventana y area  cliente , luego debemos
  //igualar el area de coordenadas de ventana al cliente
  GetWindowRect(&rcWindow);
  GetClientRect(&rcClient);
  ScreenToClient(rcWindow);


  ::OffsetRect(rcClient, -rcWindow.l, -rcWindow.t);
  ::OffsetRect(rcWindow, -rcWindow.l, -rcWindow.t);

  m_dc.CreateCompatibleDC(dc.m_hDC);
  m_bitmap.CreateCompatibleBitmap(dc.m_hDC, rcWindow.Width(), rcWindow.Height());
  CBitmap m_OldBitmap((HBITMAP)m_dc.SelectObject(&m_bitmap));
  COLOR cb;
  cb = (GetRGBColorFace());
  m_dc.FillRect(rcWindow, &cb);
  dc.IntersectClipRect(rcWindow);
  dc.ExcludeClipRect(rcClient);//asi evitamos el parpadeo
  dc.BitBlt(rcWindow.l, rcWindow.t, rcWindow.Width(), rcWindow.Height(), m_dc.m_hDC, 0, 0, SRCCOPY);
  m_dc.SelectObject(&m_OldBitmap);
  m_bitmap.DeleteObject();
  m_dc.DeleteDC();
  cb.DeleteObject();
  dc.DeleteDC();


}

BOOL DockManager::OnLButtonUp(UINT nFlags, IPOINT point)
{

  if (m_DragSplitter) {
    ReleaseCapture();
    m_DragSplitter = FALSE;
    DrawSppliter(m_Splitter);

    if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {

      if (abs(m_Splitter.l - m_ptStartPos.x) <= 2) {
        return TRUE;
      }
    }
    else {
      if (abs(m_Splitter.t - m_ptStartPos.y) <= 2) {
        return TRUE;
      }
    }

    RecalLayout(TRUE);
    return TRUE;
  }


  if (m_DragSplitterInside) {
    //  ScreenToClient(&point);
    DrawSppliterInside(m_ptOldPos);
    ReleaseCapture();
    m_DragSplitterInside = FALSE;

    if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {

      if (abs(point.y - m_ptStartPos.y) <= 2) {
        return TRUE;
      }
    }
    else {
      if (abs(point.x - m_ptStartPos.x) <= 2) {
        return TRUE;
      }
    }

    ResizeWindow(point);
    //RecalLayout(TRUE);
  }

  return TRUE;
}

void DockManager::OnNcMouseMove(UINT nHitTest, IPOINT point)
{
  Default();
}

void DockManager::OnNcLButtonDown(UINT nHitTest, IPOINT point)
{
  if (BS_SPLIT_VERT == nHitTest || nHitTest == BS_SPLIT_HORTZ) {
    if (!m_DragSplitterInside) {
      m_DragSplitterInside = TRUE;
      SetCapture();
      SetFocus();
      m_ptStartPos = point;
      ScreenToClient(&point);
      m_ptOldPos = point;
      DrawSppliterInside(point);
    }

    return;
  }

  if (!m_DragSplitter) {
    m_DragSplitter = TRUE;
    m_ptStartPos = point;
    SetCapture();
    SetFocus();
    DrawSppliter(m_Splitter);
    return;
  }

  //Default();
}


void DockManager::OnNcLButtonUp(UINT nHitTest, IPOINT point)
{
  Default();
}

UINT DockManager::OnNcHitTest(IPOINT cp)
{
  IRECT rcWindow;
  GetWindowRect(rcWindow);

  if (m_Aling == BS_ALIGN_LEFT) {
    rcWindow.l = rcWindow.r - 4;
    m_Splitter = rcWindow;

    if (rcWindow.PtInRect(cp)) {
      return BS_BORDER_RIGHT;
    }
  }
  else if (m_Aling == BS_ALIGN_BOTTOM) {
    rcWindow.b = rcWindow.t + 4;
    m_Splitter = rcWindow;

    if (rcWindow.PtInRect(cp)) {
      return BS_BORDER_TOP;
    }
  }
  else if (m_Aling == BS_ALIGN_RIGHT) {
    rcWindow.r = rcWindow.l + 4;
    m_Splitter = rcWindow;

    if (rcWindow.PtInRect(cp)) {
      return BS_BORDER_LEFT;
    }
  }


  ScreenToClient(&cp);
  cp.y -= 1;
  cp.x -= 1;

  if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {

    for (int i = 0; i < m_NumSep; i++) {

      if (m_Splits[i].PtInRect(cp)) {
        m_Splitter = m_Splits[i];
        m_OldSplitter = m_Splitter;
        return BS_SPLIT_VERT;
      }
    }
  }

  if (m_Aling == BS_ALIGN_BOTTOM) {

    for (int i = 0; i < m_NumSep; i++) {
      if (m_Splits[i].PtInRect(cp)) {
        m_Splitter = m_Splits[i];
        m_OldSplitter = m_Splitter;
        return BS_SPLIT_HORTZ;
      }
    }
  }

  return Default();
}

void DockManager::OnTimer(UINT nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  Default();
}

int  DockManager::GetNumChild()
{
  return m_DockWindows.GetSize();
}

void DockManager::RecalLayout(BOOL bReSize)
{

  // first ajust the docking bars in the frame window
  if (GetNumChild() <= 0) {
    ShowWindow(SW_HIDE);
    return;
  }

  if (bReSize) {

    if (m_Aling == BS_ALIGN_LEFT) {

      IRECT rc, rcw, rcc;
      GetWindowRect(rc);
      GetClientRect(rcc);
      theApp->m_WinMain->GetClientRect(rcw);
      ClientToScreen(rcw);
      ::OffsetRect(rcw, -rc.l, -rc.t);

      rc.r = m_Splitter.r;
      m_size.w = rc.Width();
      m_size.h = rc.Height();
      theApp->m_WinMain->ScreenToClient(rc);
      //SetWindowPos(NULL,rc,SWP_HIDEWINDOW);
      theApp->m_WinMain->UpdateFrame(FALSE);
    }

    if (m_Aling == BS_ALIGN_RIGHT) {
      IRECT rc, rcw, rcc;
      this->GetWindowRect(rc);
      GetClientRect(rcc);
      theApp->m_WinMain->GetClientRect(rcw);

      if (m_Splitter.l < 0) {
        rc.l += m_Splitter.l;
      }
      else {
        rc.l = m_Splitter.l;
      }

      theApp->m_WinMain->ScreenToClient(rc);
      m_size.w = rc.Width();
      m_size.h = rc.Height();
      //SetWindowPos(NULL,rc,SWP_HIDEWINDOW );
      theApp->m_WinMain->UpdateFrame(FALSE);
    }

    if (m_Aling == BS_ALIGN_BOTTOM) {
      IRECT rc, rcw, rcc;
      this->GetWindowRect(rc);
      GetClientRect(rcc);
      theApp->m_WinMain->GetClientRect(rcw);
      ClientToScreen(rcw);
      ::OffsetRect(rcw, -rc.l, -rc.t);

      if (m_Splitter.t < 0) {
        rc.t += m_Splitter.t;
      }
      else {
        rc.t = m_Splitter.t;
      }

      theApp->m_WinMain->ScreenToClient(rc);
      m_size.w = rc.Width();
      m_size.h = rc.Height();
      //SetWindowPos(NULL,rc,SWP_HIDEWINDOW );
      theApp->m_WinMain->UpdateFrame(FALSE);
    }
  }
  else { // now the child windows in the new container windows size
    CalSizeWindow();
  }
}

int  DockManager::GetNumVisibleWindows()
{
  int iNumW = m_DockWindows.GetSize();
  int count = 0;

  for (int i = 0; i < iNumW; i++) {
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;

    if (!((CContainer*)pWin)->IsClosePane()) {
      count++;
    }

  }

  return count;
}

CContainer*  DockManager::GetWichContainerIsTheParent(HWND hWnd)
{
  int iNumW = m_DockWindows.GetSize();
  int count = 0;

  for (int i = 0; i < iNumW; i++) {
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;

    if (((CContainer*)pWin)->GetSafeHwnd() == hWnd) {
      return (CContainer*)pWin;
    }

  }

  return NULL;
}

void DockManager::EveryWindowsInsert()
{
  int iSizeW = 0;
  int iSizeH = 0;
  int iSizeWDock = 0;
  int iSizeHDock = 0;
  int iNumW = GetNumVisibleWindows();
  int FirstVisible = GetFirstVisible();
  int LastVisible = GetLastVisible();

  if (FirstVisible == -1) {
    return;
  }

  m_Splits.RemoveAll();
  m_NumSep = 0;
  IRECT rcSplit;

  if (IsWindowVisible() && iNumW > 0) {
    IRECT rcClient;
    GetClientRect(rcClient); //area client of the docking bar
    iSizeW = rcClient.Width() / iNumW; //size actual
    iSizeH = rcClient.Height() / iNumW;

    if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {
      if (rcClient.Width() < 4) {
        rcClient.r = iSizeW;
      }
    }
    else {
      if (rcClient.Height() < 4) {
        rcClient.r = iSizeH;
      }
    }

    IRECT rcW = IRECT(0, 0, 0, 0);
    int  iContPos = 0;
    HDWP hDWP = ::BeginDeferWindowPos(iNumW);

    //Ok, we going to adjust all child window in the new docking bar size
    for (int i = FirstVisible; i <= LastVisible; i++) {
      CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
      IRECT rcW = IRECT(0, 0, 0, 0);

      if (pWin->IsWindowVisible()) {
        switch (m_Aling) {
        case BS_ALIGN_LEFT:
        case BS_ALIGN_RIGHT:
          pWin->GetWindowRect(rcW);
          ScreenToClient(rcW);

          if (m_Aling == BS_ALIGN_RIGHT) {
            rcW.l = rcClient.l + 2;
          }
          else {
            rcW.l = rcClient.l;
          }

          rcW.r = rcClient.r;

          if (i == 0) { // si es la primera ventana hija donde esta la primera posicion del arriba
            iContPos = rcClient.t;
            rcW.t = rcClient.t;
          }
          else {
            rcW.t = iContPos;  // si no, aqui se inicia la siguiente ventana.
          }

          rcW.b = iSizeH;
          rcSplit = rcW;
          rcSplit.t = rcSplit.b;
          rcSplit.b = rcSplit.t + 2;
          m_NumSep++;
          iContPos += rcW.b + 3;
          ((CDockStruct*)m_DockWindows[i])->m_rectPost = rcW;
          DeferWindow(hDWP, pWin->GetSafeHwnd(), NULL, rcW, SWP_NOZORDER | SWP_NOACTIVATE || SWP_SHOWWINDOW);
          break;

        case BS_ALIGN_BOTTOM:
          pWin->GetWindowRect(rcW);
          ScreenToClient(rcW);
          rcW.t = rcClient.t;
          rcW.b = rcClient.b;

          if (i == 0) {
            iContPos = rcClient.l;
            rcW.l = rcClient.l;
          }
          else {
            rcW.l = iContPos;
          }

          rcW.r = iSizeW;
          rcSplit = rcW;
          rcSplit.l = rcSplit.r;
          rcSplit.r = rcSplit.l + 2;
          m_Splits.InsertAt(m_NumSep, rcSplit);
          m_NumSep++;
          iContPos += rcW.r + 3;
          ((CDockStruct*)m_DockWindows[i])->m_rectPost = rcW;
          DeferWindow(hDWP, pWin->GetSafeHwnd(), NULL, rcW, SWP_NOZORDER | SWP_NOACTIVATE || SWP_SHOWWINDOW);
          break;
        }

      }

    }

    if (hDWP != NULL) {
      ::EndDeferWindowPos(hDWP);
    }
  }

}

int DockManager::GetFirstVisible()
{

  int iNumW = m_DockWindows.GetSize();
  int count = 0;

  for (int i = 0; i < iNumW; i++) {
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;

    if (pWin->IsWindowVisible()) {
      return i;
    }

  }

  return -1;
}

int DockManager::GetLastVisible()
{

  int iNumW = m_DockWindows.GetSize();
  int count = 0;

  for (int i = 0; i < iNumW; i++) {
    CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;

    if (pWin->IsWindowVisible()) {
      count = i;
    }

  }

  return count;
}
void DockManager::CalSizeWindow()
{
  int i;
  int iSizeW = 0;
  int iSizeH = 0;
  int iSizeWDock = 0;
  int iSizeHDock = 0;
  BOOL bGrow = FALSE;
  int  iDif = 0;
  int iNumW = GetNumVisibleWindows();

  if (GetFirstVisible() == -1) {
    return;
  }

  m_Splits.RemoveAll();
  m_Splits.FreeExtra();
  m_NumSep = 0;
  IRECT rcSplit;
  int FirstVisible = GetFirstVisible();
  int LastVisible = GetLastVisible();

  if (IsWindowVisible() && iNumW > 0) {
    IRECT rcClient;
    GetClientRect(rcClient); //area client of the docking bar
    iSizeW = rcClient.Width(); //size actual
    iSizeH = rcClient.Height();
    int iNumSeparador = 0;

    for (int i = FirstVisible; i <= LastVisible; i++) { //wich is the size real of the child windows
      CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
      IRECT rcW;
      pWin->GetWindowRect(rcW);

      if (pWin->IsWindowVisible()) {
        iSizeWDock += rcW.Width();
        iSizeHDock += rcW.Height();
        iNumSeparador++;
      }
    }

    if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {
      if (rcClient.Width() < 4) {
        rcClient.r = iSizeWDock;
      }
    }
    else {
      if (rcClient.Height() < 4) {
        rcClient.r = iSizeHDock;
      }
    }

    if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {

      bGrow = ((iSizeH - ((iNumSeparador - 1) * 3) - 1) - iSizeHDock) < 0 ? FALSE : TRUE;
      iDif = abs((iSizeH - ((iNumSeparador - 1) * 3) - 1) - iSizeHDock);

    }
    else {
      bGrow = ((iSizeW - ((iNumSeparador - 1) * 5) - 1) - iSizeWDock) < 0 ? FALSE : TRUE;
      iDif = abs((iSizeW - ((iNumSeparador - 1) * 5) - 1) - iSizeWDock);

    }

    IRECT rcW = IRECT(0, 0, 0, 0);

    HDWP hDWP = ::BeginDeferWindowPos(GetNumVisibleWindows());

    //Ok, we going to adjust all child window in the new docking bar size
    if (iNumW == 1) { // if it´s contains a only one window
      rcW = rcClient;
      rcW.b -= 1;
      iDif = 0;
      ((CDockStruct*)m_DockWindows[FirstVisible])->m_rectPost = rcW;
    }
    else {
      while (iDif > 0) {
        if (iDif <= 0) {
          break;
        }

        int  iContPos = 0;
        int bFirst = TRUE;

        for (int i = FirstVisible; i <= LastVisible; i++) {
          CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
          IRECT rcW = ((CDockStruct*)m_DockWindows[i])->m_rectPost;

          if (pWin->IsWindowVisible()) {
            switch (m_Aling) {
            case BS_ALIGN_LEFT:
            case BS_ALIGN_RIGHT:
              if (m_Aling == BS_ALIGN_RIGHT) {
                rcW.l = rcClient.l + 2;
              }
              else {
                rcW.l = rcClient.l;
              }

              rcW.r = rcClient.r;

              if (i == 0) { // si es la primera ventana hija donde esta la primera posicion del arriba
                iContPos = rcClient.t;
                rcW.t = rcClient.t;
              }
              else {
                if (iContPos > rcClient.t) {
                  rcW.t = iContPos;  // si no, aqui se inicia la siguiente ventana.
                }
                else {
                  rcW.t = rcClient.t;
                }
              }

              if (iNumW == 1) { // if it´s contains a only one window
                if (bGrow) {
                  rcW.b += iDif;
                }
                else {
                  rcW.b -= iDif;
                }

                iDif = 0;
              }
              else {
                if (bGrow) {
                  rcW.b += 1;
                  iDif--;
                }
                else {
                  rcW.b -= 1;
                  iDif--;
                }
              }


              iContPos += rcW.b + 3;
              ((CDockStruct*)m_DockWindows[i])->m_rectPost = rcW;

              break;

            case BS_ALIGN_BOTTOM:
              rcW.t = rcClient.t;
              rcW.b = rcClient.b;

              if (i == 0) {
                iContPos = rcClient.l;
                rcW.l = rcClient.l;
              }
              else {
                rcW.l = iContPos;
              }

              if (iNumW == 1) { // if it´s contains a only one window
                if (bGrow) {
                  rcW.r += iDif;
                }
                else {
                  rcW.r -= iDif;
                }

                iDif = 0;
              }
              else {
                if (bGrow) {
                  rcW.r += 1;
                  iDif--;
                }
                else {
                  rcW.r -= 1;
                  iDif--;
                }
              }

              iContPos += rcW.r + 5;
              ((CDockStruct*)m_DockWindows[i])->m_rectPost = rcW;
              break;
            }

          }

        }

      }
    }


    for (i = FirstVisible; i <= LastVisible; i++) {
      CWin* pWin = ((CDockStruct*)m_DockWindows[i])->m_WinDock;
      IRECT rcW = ((CDockStruct*)m_DockWindows[i])->m_rectPost;

      if (pWin->IsWindowVisible()) {
        IRECT rcWin;
        pWin->GetWindowRect(rcWin);

        if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {
          if (m_Aling == BS_ALIGN_RIGHT) {
            rcW.l = rcClient.l + 2;
          }
          else {
            rcW.l = rcClient.l;
          }

          rcW.r = rcClient.r;
          rcSplit = rcW;
          rcSplit.t += rcWin.Height() - 2;
          rcSplit.b = rcSplit.t + 3;
          //if (i< LastVisible)
          m_Splits.InsertAt(m_NumSep, rcSplit);
          m_NumSep++;
        }
        else {
          rcW.t = rcClient.t;
          rcW.b = rcClient.b;
          rcSplit = rcW;
          rcSplit.l += rcWin.Width() - 1;
          rcSplit.r = rcSplit.l + 5;
          m_Splits.InsertAt(m_NumSep, rcSplit);
          m_NumSep++;
        }

        ((CDockStruct*)m_DockWindows[i])->m_rectPost = rcW;

        DeferWindow(hDWP, pWin->GetSafeHwnd(), NULL, rcW, SWP_NOZORDER | SWP_NOACTIVATE);

      }
    }

    if (hDWP != NULL) {
      ::EndDeferWindowPos(hDWP);
    }
  }

}

void DockManager::ResizeWindow(IPOINT point)
{
  ClientToScreen(&point);
  int iNumW = GetNumVisibleWindows();

  if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {
    IRECT rc;
    IRECT rcClient;

    if (abs(m_ptStartPos.y - abs(point.y)) < 1) {
      SetCursorPos(m_ptStartPos.x, m_ptStartPos.y);
      return;
    }

    if (m_ptStartPos.y > point.y) { //la ventana inferior se amplia, la superior o anterior se reduce
      int nDif = 0;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost;

      if (abs(rc.t - point.y) <= 22) {
        nDif = rc.Height() - 23;
        rc.b = rc.t + 23;
      }
      else {
        nDif = rc.b - point.y;
        rc.b = point.y;

      }

      rcClient.b -= nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost = rcClient;
      int NextPos = rcClient.b + 3;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost;
      rcClient.t = NextPos;
      rcClient.b = rc.Height() + nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost = rcClient;
    }
    else {
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost;
      rcClient.b = rc.Height();
      int nDif = 0;

      if (point.y >= rc.b - 22) {
        nDif = rc.Height() - 23;
        rc.t = rc.b - 23;
      }
      else {
        nDif = point.y - rc.t;
        rc.t = point.y;
      }

      rcClient.t += nDif;
      rcClient.b -= nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost = rcClient;

      int NextPos = rcClient.t - 3;

      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost;
      rcClient.b += nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost = rcClient;
    }


  }
  else { //the bottom window
    IRECT rc;
    IRECT rcClient;

    if (abs(m_ptStartPos.y - abs(point.y)) < 1) {
      SetCursorPos(m_ptStartPos.x, m_ptStartPos.y);
      return;
    }

    if (m_ptStartPos.y > point.y) { //la ventana inferior se amplia, la superior o anterior se reduce
      int nDif = 0;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost;

      if (abs(rc.r - point.x) <= 22) {
        nDif = rc.Width() - 23;
        rc.r = rc.l + 23;
      }
      else {
        nDif = rc.r - point.x;
        rc.r = point.x;

      }

      rcClient.r -= nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost = rcClient;
      int NextPos = rcClient.r + 3;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost;
      rcClient.l = NextPos;
      rcClient.r = rc.Width() + nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost = rcClient;
    }
    else {
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost;
      rcClient.r = rc.Width();
      int nDif = 0;

      if (point.x >= rc.r - 22) {
        nDif = rc.Width() - 23;
        rc.l = rc.r - 23;
      }
      else {
        nDif = point.x - rc.l;
        rc.l = point.x;
      }

      rcClient.l += nDif;
      rcClient.r -= nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove + 1])->m_rectPost = rcClient;

      int NextPos = rcClient.l - 3;

      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_WinDock->GetWindowRect(rc);
      rcClient = ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost;
      rcClient.r += nDif;
      ((CDockStruct*)m_DockWindows[m_SplitMove])->m_rectPost = rcClient;
    }

  }

  CalSizeWindow();
  CalSizeWindow();

}


CSize DockManager::Getsize()
{
  return m_size;
}

int DockManager::FindItem(HWND hWnd)
{
  for (int iCont = 0; iCont < m_NumWins; iCont++) {
    CDockStruct* ct = (CDockStruct*) m_DockWindows[iCont];

    if (ct->m_WinDock->GetSafeHwnd() == hWnd) {
      return iCont;
    }
  }

  return -1;

}

CWin*  DockManager::GetNumWnd(int m_numWin)
{
  if (m_numWin > m_NumWins) {
    return NULL;
  }
  else {
    if (m_DockWindows.GetSize() < 1) {
      return NULL;
    }

    CWin* pw = ((CDockStruct*) m_DockWindows [m_numWin])->m_WinDock;

    if (HIWORD(pw) == NULL) {
      return NULL;
    }
    else {
      return ((CDockStruct*) m_DockWindows [m_numWin])->m_WinDock;
    }
  }
}



void DockManager::AddWindow(CWin* pParent, int nPos)
{
  if (!HIWORD(pParent)) {
    return;
  }

  if (nPos == -1) {
    m_DockWindows.SetAtGrow(m_NumWins, new CDockStruct(pParent));
  }
  else {
    m_DockWindows.InsertAt(nPos, new CDockStruct(pParent));

  }

  pParent->ShowWindow(SW_SHOW);
  ((CContainer*)pParent)->m_DOCKFLOAT = ((CContainer*)pParent)->m_DOCKSITE =
      GetWindowLong(GetSafeHwnd(), GWL_ID);
  m_NumWins++;
  EveryWindowsInsert();
  RecalLayout(FALSE);
}

UINT DockManager::AddWindow(CDockPanelBase* pParent, UINT dlgCtrl, int nPos)
{

  if (dlgCtrl == NULL) {
    CContainer* ctn = new CContainer;

    if (!ctn->Create(WS_CHILD | WS_VISIBLE, IRECT(0, 0, 0, 0), this, pParent->GetDlgCtrlID() + 1)) {
      return NULL;
    }

    ctn->Addtab(pParent);
    pParent->SetParent(ctn);


    ctn->m_DOCKFLOAT = !BS_FLOATING;
    ctn->m_DOCKSITE = GetWindowLong(GetSafeHwnd(), GWL_ID);
    pParent->uAling = GetWindowLong(GetSafeHwnd(), GWL_ID);
    pParent->iLasPosition = m_NumWins;
    pParent->bIsPartOfTabbed = FALSE;
    pParent->bIsShow = TRUE;
    pParent->m_IdCtrlParent = NULL;

    if (!HIWORD(pParent)) {
      return NULL;
    }

    if (nPos == -1) {
      m_DockWindows.SetAtGrow(m_NumWins, new CDockStruct(ctn));
      m_NumWins++;
    }
    else {
      m_DockWindows.InsertAt(nPos, new CDockStruct(ctn));

    }
  }
  else {
    for (int i = 0; i < m_DockWindows.GetSize(); i++) {
      CDockStruct* pTab = (CDockStruct*)m_DockWindows.GetAt(i);
      CTab*  ctab = ((CContainer*)pTab->m_WinDock)->GetTab(0);

      if ((UINT)ctab->pParent->GetDlgCtrlID() == dlgCtrl) {
        CContainer* pct = (CContainer*)pTab->m_WinDock;
        pParent->SetParent(pct);
        pParent->uAling = NULL;
        pParent->iLasPosition = m_NumWins;
        pParent->bIsPartOfTabbed = TRUE;
        pParent->bIsShow = TRUE;
        pParent->m_IdCtrlParent = dlgCtrl;
        pct->Addtab(pParent);
        break;
      }
    }

  }

  pParent->ShowWindow(SW_SHOW);
  //((CContainer*)pParent)->m_DOCKFLOAT=((CContainer*)pParent)->m_DOCKSITE=
  //GetWindowLong(GetSafeHwnd(),GWL_ID);

  EveryWindowsInsert();
  RecalLayout(FALSE);
  return pParent->GetDlgCtrlID() + 1;
}



void DockManager::DeleteWindow(int m_NumW)
{
  if (m_NumW < 0) {
    return;
  }

  if (m_NumW < m_NumWins) {
    CDockStruct* ctn = (CDockStruct*) m_DockWindows[m_NumW];

    if (ctn) {
      delete ctn;
    }

    m_NumWins--;
    m_DockWindows.RemoveAt(m_NumW, 1);
    EveryWindowsInsert();
  }

}

BOOL DockManager::OnSize(UINT nType, int cx, int cy)
{
  //if (m_NumWins > 0)
  RecalLayout(FALSE);
  Invalidate();
  UpdateWindow();
  return TRUE;
}





void DockManager::OnMouseMove(UINT nHitTest, IPOINT point)
{
  IPOINT cp;
  GetCursorPos(&cp);

  if (GetCapture() != GetSafeHwnd()) {
    m_DragSplitter = FALSE;
  }

  if (m_DragSplitter) {
    DrawSppliter(m_Splitter);

    //nuevos tamaños de la ventana
    if (IsVertical()) {
      //ClientToScreen(&cp);

      m_Splitter.l = cp.x;
      m_Splitter.r = m_Splitter.l + 4;

    }
    else {

      m_Splitter.t = cp.y;
      m_Splitter.b = m_Splitter.t + 4;
    }

    DrawSppliter(m_Splitter);
    return;
  }

  ScreenToClient(&cp);

  if (m_DragSplitterInside) {

    DrawSppliterInside(m_ptOldPos);

    if (IsVertical()) {
      //ClientToScreen(&cp);

      m_Splitter.l = cp.x;
      m_Splitter.r = m_Splitter.l + 4;

    }
    else {

      m_Splitter.t = cp.y;
      m_Splitter.b = m_Splitter.t + 4;
    }

    DrawSppliterInside(cp);
    m_ptOldPos = point;


  }

  //Default();
}

BOOL DockManager::OnLButtonDown(UINT nFlags, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default


  Default();
  return TRUE;

}

BOOL DockManager::OnRButtonDown(UINT nFlags, IPOINT point)
{
  Default();

  return TRUE;

}


int DockManager::OnDestroy()
{
  // Libera la memoria
  for (int i = 0; i < m_NumWins; i++) {
    CDockStruct* pTab = (CDockStruct*)m_DockWindows.GetAt(i);

    if (pTab) {
      delete pTab;
    }
  }

  return TRUE;

  // TODO: Add your message handler code here
}



void DockManager::DrawSppliter(IRECT rc)
{

  IRECT rcWindow;
  theApp->m_WinMain->GetWindowRect(rcWindow);
  CGDI pDC(theApp->m_WinMain->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE));
  COLOR* pBrush = CGDI::HalftGray();
  COLOR pBrushOld(pDC.SelectObject(pBrush));
  ::OffsetRect(rc, -rcWindow.l, -rcWindow.t);
  //theApp->m_WinMain->GetClientRect(rcWindow);
  IRECT rc1;
  GetClientRect(rc1);
  //::OffsetRect(rcWindow, -rcWindow.l,-rcWindow.t);

  //rc.OffsetRect(-rcWindow.TopLeft());
  if (IsVertical()) {
    pDC.PatBlt(rc.l, rc.t, rc.Width(), rc1.Height(), PATINVERT);
  }
  else {
    pDC.PatBlt(rc.l, rc.t, rc.Width(), rc.Height(), PATINVERT);
  }

  pDC.SelectObject(&pBrushOld);
  theApp->m_WinMain->ReleaseDC(pDC.m_hDC);
  pBrush->DeleteObject();
  pBrushOld.DeleteObject();
  delete pBrush;

}

void DockManager::DrawSppliterInside(IPOINT cp)
{
  IRECT rc;
  GetClientRect(rc);
  CGDI pDC(this->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE));
  COLOR* pBrush = CGDI::HalftGray();
  COLOR pBrushOld(pDC.SelectObject(pBrush));

  if (IsVertical()) {
    rc.t = cp.y;
    rc.b = cp.y + 3;
    pDC.PatBlt(rc.l, rc.t, rc.Width(), rc.Height(), PATINVERT);
  }
  else {
    rc.l = cp.x;
    rc.r = cp.x + 5;
    pDC.PatBlt(rc.l, rc.t, rc.Width(), rc.Height(), PATINVERT);
  }

  pDC.SelectObject(&pBrushOld);
  theApp->m_WinMain->ReleaseDC(pDC.m_hDC);
  pBrush->DeleteObject();
  pBrushOld.DeleteObject();
  delete pBrush;

}

BOOL DockManager::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
  IPOINT cp;
  GetCursorPos(&cp);
  IRECT rcWindow;
  GetWindowRect(rcWindow);
  m_SplitMove = 0;

  if (m_Aling == BS_ALIGN_LEFT) {
    rcWindow.l = rcWindow.r - 2;

    if (rcWindow.PtInRect(cp)) {
      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
      return TRUE;
    }

  }
  else if (m_Aling == BS_ALIGN_BOTTOM) {
    rcWindow.b = rcWindow.t + 2;

    if (rcWindow.PtInRect(cp)) {
      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
      return TRUE;
    }


  }
  else if (m_Aling == BS_ALIGN_RIGHT) {

    rcWindow.r = rcWindow.l + 2;

    if (rcWindow.PtInRect(cp)) {
      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
      return TRUE;
    }


  }

  ScreenToClient(&cp);
  cp.y -= 1;
  cp.x -= 1;

  if (m_Aling == BS_ALIGN_LEFT || m_Aling == BS_ALIGN_RIGHT) {

    for (int i = 0; i < m_NumSep; i++) {

      if (m_Splits[i].PtInRect(cp)) {
        m_SplitMove = i;
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
        return TRUE;
      }
    }
  }

  if (m_Aling == BS_ALIGN_BOTTOM) {

    for (int i = 0; i < m_NumSep; i++) {
      if (m_Splits[i].PtInRect(cp)) {
        m_SplitMove = i;
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
        return TRUE;
      }
    }
  }

  SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
  return TRUE;

}
