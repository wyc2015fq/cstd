
/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer construction/destruction/initialization

int GetString(str_t* str) const
/* ============================================================
  Function :    GetString
  Description : Returns a string representation of the
          virtual paper size
  Access :    Public

  Return :    str_t - Resulting string
  Parameters :  none

  Usage :     Call to get a string representing the paper
          size of the container. The format is
          "paper:x,y;" where "x" and "y" are the
          horisontal and vertical sizes.

   ============================================================*/
{

  str_format(str, _T("paper:%i,%i;"), GetVirtualSize().w, GetVirtualSize().h);
  return 0;

}

BOOL FromString(const char* str)
{

  sscanf(str, "paper:%i,%i;", &m_virtualSize.w, &m_virtualSize.h);
  SetVirtualSize(m_virtualSize);

  return 0;

}

void SetVirtualSize(SIZE size)
{

  m_virtualSize = size;

}

SIZE GetVirtualSize() const
{

  return m_virtualSize;

}


SIZE GetTotalSize()
{
  POINT start = GetStartPoint();
  int width = 0;
  int height = 0;
  int count = 0;

  for (count = 0; count < m_objsNum; ++count) {
    CDiagramEntity* obj = m_objs + count;
    width = MAX(width, obj->m_rc.left);
    width = MAX(width, obj->m_rc.right);
    height = MAX(height, obj->m_rc.top);
    height = MAX(height, obj->m_rc.bottom);

    count++;

  }

  return iSIZE(round(width - start.x), round(height - start.y));

}

POINT GetStartPoint()
{

  int startx = 2000;
  int starty = 2000;

  int count = 0;

  for (count = 0; count < m_objsNum; ++count) {
    CDiagramEntity* obj = m_objs + count;

    startx = MIN(startx, obj->m_rc.left);
    startx = MIN(startx, obj->m_rc.right);
    starty = MIN(starty, obj->m_rc.top);
    starty = MIN(starty, obj->m_rc.bottom);

    count++;

  }

  return cPOINT(round(startx), round(starty));

}

int GetSelectCount() const
{

  int MAX = m_objsNum;
  int count = 0;

  for (int t = 0 ; t < MAX ; t++) {
    CDiagramEntity* obj = m_objs + t;

    if (obj->m_selected) {
      count++;
    }
  }

  return count;

}

void SelectAll()
{
  int MAX = m_objsNum;

  for (int t = 0 ; t < MAX ; t++) {
    CDiagramEntity* obj = m_objs + t;
    obj->m_selected = (TRUE);
  }
}

void UnselectAll()
{
  int MAX = m_objsNum;

  for (int t = 0 ; t < MAX ; t++) {
    CDiagramEntity* obj = m_objs + t;
    obj->m_selected = (FALSE);
  }
}


CDiagramEditor::CDiagramEditor()
{

  m_bkgndCol = _GetSysColor(COLOR_WINDOW);
  m_nonClientBkgndCol = _GetSysColor(COLOR_3DSHADOW);

  m_gridCol = _RGB(192, 192, 192);
  m_isGridVisible = TRUE;
  m_gridSize = iSIZE(8, 8);

  m_isSnapToGrid = FALSE;
  m_bgResize = FALSE;
  m_bgResizeZone = 10;
  m_bgResizeSelected = FALSE;
  m_markerSize = iSIZE(8, 8);

  m_objs = NULL;
  m_multiSelObj = NULL;

  m_restraint = RESTRAINT_NONE;

  m_rcMargin = iRECT(8, 8, 8, 8);
  m_isMarginVisible = FALSE;
  m_marginColor = _RGB(128, 128, 255);

  m_multiDraw = FALSE;

}

void CDiagramEditor::Clear()
{

  // Clearing internal states and vars
  SetRectEmpty(&m_selectionRect);
  m_interactMode = MODE_NONE;
  m_bgResizeSelected = FALSE;
  m_deltaPoint = cPOINT(0, 0);
  m_multiSelObj = NULL;
  m_drawing = FALSE;

}

void RedrawWindow()
{
  return ;
}
void CDiagramEditor::New()
{
  Clear();
  RedrawWindow();
}


// CDiagramEditor painting
void CDiagramEditor::Draw(HDDC dc, RECT rect) const
{

  draw_fillrectR(dc, &rect, m_nonClientBkgndCol);
  draw_fillrectR(dc, &rect, m_bkgndCol);

  if (m_isGridVisible) {
    DrawGrid(dc, rect);
  }

  if (m_isMarginVisible) {
    DrawMargins(dc, rect);
  }

  DrawObjects(dc);

  if (m_bgResize && m_bgResizeSelected) {
    DrawSelectionMarkers(dc, rect);
  }
}

void CDiagramEditor::DrawGrid(HDDC dc, RECT rect) const
{
  COLORREF gridcol = m_gridCol;
  DWORD _clr = gridcol;
  int _x0, _y0;
  int width = RCW(&rect);
  int height = RCH(&rect);

  int stepx = width / m_gridSize.w;
  int stepy = height / m_gridSize.h;

  // ...instead we calculate the position of each line.
  for (int x = 0 ; x <= stepx ; x++) {
    TMoveTo(dc, round((m_gridSize.w * x)), 0);
    _LineTo(dc, round((m_gridSize.w * x)), height);
  }

  for (int y = 0; y <= stepy ; y++) {
    TMoveTo(dc, 0, round((m_gridSize.h * y)));
    _LineTo(dc, width, round((m_gridSize.h * y)));
  }

}

void CDiagramEditor::DrawMargins(HDDC dc, RECT rect) const
{
  DWORD _clr = m_marginColor;
  int _x0, _y0;
  POINT leftTop = cPOINT(rect.left + round((m_rcMargin.left)), rect.top + round((m_rcMargin.top)));
  POINT leftBottom = cPOINT(rect.left + round((m_rcMargin.left)), rect.bottom - round((m_rcMargin.bottom)) - 1);
  POINT rightTop = cPOINT(rect.right - round((m_rcMargin.right)) - 1, rect.top + round((m_rcMargin.top)));
  POINT rightBottom = cPOINT(rect.right - round((m_rcMargin.right)) - 1, rect.bottom - round((m_rcMargin.bottom)) - 1);

  if (m_rcMargin.left) {
    TMoveToPt(dc, leftTop);
    _LineToPt(dc, leftBottom);
  }

  if (m_rcMargin.right) {
    TMoveToPt(dc, rightTop);
    _LineToPt(dc, rightBottom);
  }

  if (m_rcMargin.top) {
    TMoveToPt(dc, leftTop);
    _LineToPt(dc, rightTop);
  }

  if (m_rcMargin.bottom) {
    TMoveToPt(dc, leftBottom);
    _LineToPt(dc, rightBottom);
  }

}

void CDiagramEditor::DrawObjects(HDDC dc) const
{

  if (m_objs) {
    int count = 0;
    //CDiagramEntity* obj;
    //while( ( obj = m_objs->GetAt( count++ ) ) )
    //  obj->DrawObject( dc );
  }

}

void CDiagramEditor::DrawSelectionMarkers(HDDC dc, RECT rect) const
{

  // Draw selection markers
  RECT rectSelect;
  DWORD _clr = _RGB(0, 0, 0);

  rectSelect = GetSelectionMarkerRect(DEHT_TOPLEFT, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_TOPMIDDLE, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_TOPRIGHT, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_BOTTOMLEFT, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_BOTTOMMIDDLE, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_BOTTOMRIGHT, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_RIGHTMIDDLE, rect);
  draw_fillrectR(dc, &rectSelect, _clr);

  rectSelect = GetSelectionMarkerRect(DEHT_LEFTMIDDLE, rect);
  draw_fillrectR(dc, &rectSelect, _clr);
}

RECT CDiagramEditor::GetSelectionMarkerRect(UINT marker, RECT rect) const
{
  RECT rectMarker;
  int horz = m_markerSize.w / 2;
  int vert = m_markerSize.h / 2;

  switch (marker) {
  case DEHT_TOPLEFT:
    SetRect(&rectMarker, rect.left - horz,
        rect.top - vert,
        rect.left + horz,
        rect.top + vert);
    break;

  case DEHT_TOPMIDDLE:
    SetRect(&rectMarker, rect.left + (RCW(&rect) / 2) - horz,
        rect.top - vert,
        rect.left + (RCW(&rect) / 2) + horz,
        rect.top + vert);
    break;

  case DEHT_TOPRIGHT:
    SetRect(&rectMarker, rect.right - horz,
        rect.top - vert,
        rect.right + horz,
        rect.top + vert);
    break;

  case DEHT_BOTTOMLEFT:
    SetRect(&rectMarker, rect.left - horz,
        rect.bottom - vert,
        rect.left + horz,
        rect.bottom + vert);
    break;

  case DEHT_BOTTOMMIDDLE:
    SetRect(&rectMarker, rect.left + (RCW(&rect) / 2) - horz,
        rect.bottom - vert,
        rect.left + (RCW(&rect) / 2) + horz,
        rect.bottom + vert);
    break;

  case DEHT_BOTTOMRIGHT:
    SetRect(&rectMarker, rect.right - horz,
        rect.bottom - vert,
        rect.right + horz,
        rect.bottom + vert);
    break;

  case DEHT_LEFTMIDDLE:
    SetRect(&rectMarker, rect.left - horz,
        rect.top + (RCH(&rect) / 2) - vert,
        rect.left + horz,
        rect.top + (RCH(&rect) / 2) + vert);
    break;

  case DEHT_RIGHTMIDDLE:
    SetRect(&rectMarker, rect.right - horz,
        rect.top + (RCH(&rect) / 2) - vert,
        rect.right + horz,
        rect.top + (RCH(&rect) / 2) + vert);
    break;
  }

  return rectMarker;

}
