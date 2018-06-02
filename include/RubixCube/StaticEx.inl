
#include "StaticEx.h"

//
// CStaticEx

CStaticEx::CStaticEx()
{
  chkHistory = FALSE;
  acq = false;
}

// CStaticEx message handlers

void CStaticEx::OnPaint()
{
  CPaintDC dc(this); // device context for painting
  IRECT rect, r;

  GetClientRect(rect);
  dc.FillSolidRect(rect, _RGB(240, 240, 245));

  CFont* pFont = dc.SelectObject(&m_font1);

  m_extract.Analyse();

  rect.DeflateRect(10, 10);

  if (chkHistory) {
    // debug mode with history
    r = m_extract.DrawImage(&dc, IPOINT(rect.l, rect.t), rect.Width(), acq);

    rect.t = r.b + 5;
    r.t = r.b + 5, r.b = rect.b;

    if (acq) {
      m_extract.DrawDebugMsg(&dc, r);
    }

    m_extract.DrawHist(&dc, r);
  }
  else {
    // only the faces pictures
    r = m_extract.DrawImage(&dc, rect, acq);
  }

  dc.SelectObject(pFont);
}

void CStaticEx::PreSubclassWindow()
{
  CClientDC dc(this);
  int nHeight4 = -(dc.GetDeviceCaps(LOGPIXELSY) * 16);

  m_font1.CreateFont(nHeight4 / 144, 0, 0, 0, FW_HEAVY, 0, 0, 0,
      DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
  m_font2.CreateFont(nHeight4 / 144, 0, 0, 0, FW_NORMAL, 0, 0, 0,
      DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

  CStatic::PreSubclassWindow();
}
