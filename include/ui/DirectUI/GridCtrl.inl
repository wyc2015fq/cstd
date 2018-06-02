
#define GetTextClr( pCell )   pCell->m_crFgClr
#define GetBackClr( pCell )   pCell->m_crBkClr
#define SetTextClr( pCell, clr )   pCell->m_crFgClr = clr
#define SetBackClr( pCell, clr )   pCell->m_crBkClr = clr

#define GetGridBkColor()   m_crGridBkColour
  
// default Grid cells. Use these for setting default values such as colors and fonts
CGridCell* GetDefaultCell( BOOL bFixedRow, BOOL bFixedCol ) {
  if ( bFixedRow && bFixedCol )
    return ( CGridCell* ) & m_cellFixedRowColDef;
  if ( bFixedRow )
    return ( CGridCell* ) & m_cellFixedRowDef;
  if ( bFixedCol )
    return ( CGridCell* ) & m_cellFixedColDef;
  return ( CGridCell* ) & m_cellDefault;
}
void OnSysColorChange() {
  //CWnd::OnSysColorChange();
  SetTextClr( GetDefaultCell( FALSE, FALSE ), GetSysColor( COLOR_WINDOWTEXT ) );      // set to new system colour
  SetBackClr( GetDefaultCell( FALSE, FALSE ), GetSysColor( COLOR_WINDOW ) );
  SetTextClr( GetDefaultCell( TRUE, FALSE ) , GetSysColor( COLOR_WINDOWTEXT ) );      // set to new system colour
  SetBackClr( GetDefaultCell( TRUE, FALSE ) , GetSysColor( COLOR_WINDOW ) );
  SetTextClr( GetDefaultCell( FALSE, TRUE ) , GetSysColor( COLOR_WINDOWTEXT ) );      // set to new system colour
  SetBackClr( GetDefaultCell( FALSE, TRUE ) , GetSysColor( COLOR_WINDOW ) );
  SetTextClr( GetDefaultCell( TRUE, TRUE )  , GetSysColor( COLOR_WINDOWTEXT ) );      // set to new system colour
  SetBackClr( GetDefaultCell( TRUE, TRUE )  , GetSysColor( COLOR_WINDOW ) );
  m_crGridBkColour = ( GetSysColor( COLOR_3DSHADOW ) );
  m_crWindowText = GetSysColor( COLOR_WINDOWTEXT );
  m_crWindowColour = GetSysColor( COLOR_WINDOW );
  m_cr3DFace = GetSysColor( COLOR_3DFACE );
  m_crShadow = GetSysColor( COLOR_3DSHADOW );
}
// UIGridCtrl cellrange functions
// Gets the first non-fixed cell ID
CCellID GetTopleftNonFixedCell( CControlUI* ctr, BOOL bForceRecalculation /*=FALSE*/ ) {
  int nRight, nTop;
  SIZE szScroll;
  // Used cached value if possible
  if ( m_idTopLeftCell.row>=0 && m_idTopLeftCell.col>=0 && !bForceRecalculation )
    return m_idTopLeftCell;
  szScroll = CContainerUI_GetScrollPos( ctr );
  m_idTopLeftCell.col = m_nFixedCols;
  nRight = 0;
  while ( nRight < szScroll.cx && m_idTopLeftCell.col < ( GetColumnCount() - 1 ) )
    nRight += GetColumnWidth( m_idTopLeftCell.col++ );
  m_idTopLeftCell.row = m_nFixedRows;
  nTop = 0;
  while ( nTop < szScroll.cy && m_idTopLeftCell.row < ( GetRowCount() - 1 ) )
    nTop += GetRowHeight( m_idTopLeftCell.row++ );
  //TRACE2("TopLeft cell is row %d, col %d\n",m_idTopLeftCell.row, m_idTopLeftCell.col);
  return m_idTopLeftCell;
}
int GetFixedRowHeight(CControlUI* ctr) {
  int i, nHeight = 0;
  for ( i = 0; i < m_nFixedRows; i++ )
    nHeight += GetRowHeight( i );
  return nHeight;
}
int GetFixedColumnWidth(CControlUI* ctr) {
  int i, nWidth = 0;
  for ( i = 0; i < m_nFixedCols; i++ )
    nWidth += GetColumnWidth( i );
  return nWidth;
}
CCellRange cCellRange( int minRow, int minCol, int maxRow, int maxCol ) {
  CCellRange cr;
  cr.m_nMinRow = minRow, cr.m_nMinCol = minCol;
  cr.m_nMaxRow = maxRow, cr.m_nMaxCol = maxCol;
  return cr;
}
// This gets even partially visible cells
CCellRange GetVisibleNonFixedCellRange( CControlUI* ctr, RECT rect, LPRECT pRect /*=NULL*/,
                                       BOOL bForceRecalculation /*=FALSE*/ ) {
  int i, maxVisibleRow, maxVisibleCol, right;
  CCellID idTopLeft = GetTopleftNonFixedCell( ctr, bForceRecalculation );
  // calc bottom
  int bottom = GetFixedRowHeight(ctr);
  for ( i = idTopLeft.row; i < GetRowCount(); i++ ) {
    bottom += GetRowHeight( i );
    if ( bottom >= rect.bottom ) {
      bottom = rect.bottom;
      break;
    }
  }
  maxVisibleRow = min( i, GetRowCount() - 1 );
  // calc right
  right = GetFixedColumnWidth(ctr);
  for ( i = idTopLeft.col; i < GetColumnCount(); i++ ) {
    right += GetColumnWidth( i );
    if ( right >= rect.right ) {
      right = rect.right;
      break;
    }
  }
  maxVisibleCol = min( i, GetColumnCount() - 1 );
  if ( pRect ) {
    pRect->left = pRect->top = 0;
    pRect->right = right;
    pRect->bottom = bottom;
  }
  return cCellRange( idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol );
}
// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void EraseBkgnd( CControlUI* ctr, RECT rect, HDC hDC ) {
  HDC pDC = hDC;
  RECT VisRect, ClipRect, rc;
  HBRUSH FixedRowColBack, FixedRowBack, FixedColBack, TextBack, Back;
  int nFixedColumnWidth, nFixedRowHeight;
  FixedRowColBack=CreateSolidBrush( GetBackClr(GetDefaultCell( TRUE, TRUE )) );
  FixedRowBack=CreateSolidBrush( GetBackClr(GetDefaultCell( TRUE, FALSE )) );
  FixedColBack=CreateSolidBrush( GetBackClr(GetDefaultCell( FALSE, TRUE )) );
  TextBack=CreateSolidBrush( GetBackClr(GetDefaultCell( FALSE, FALSE )) );
  Back=CreateSolidBrush( GetGridBkColor() );
  //CBrush Back(GetTextBkColor());
  if ( GetClipBox( pDC, &ClipRect ) == ERROR )
    return ;
  GetVisibleNonFixedCellRange( ctr, rect, &VisRect, FALSE );
  nFixedColumnWidth = GetFixedColumnWidth(ctr);
  nFixedRowHeight = GetFixedRowHeight(ctr);
  // Draw Fixed row/column background
  if ( ClipRect.left < nFixedColumnWidth && ClipRect.top < nFixedRowHeight ) {
    rc = iRECT( ClipRect.left, ClipRect.top, nFixedColumnWidth, nFixedRowHeight );
    FillRect( pDC, &rc, FixedRowColBack );
  }
  // Draw Fixed columns background
  if ( ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom ) {
    rc = iRECT( ClipRect.left, ClipRect.top, nFixedColumnWidth, VisRect.bottom );
    FillRect( pDC, &rc, FixedColBack );
  }
  // Draw Fixed rows background
  if ( ClipRect.top < nFixedRowHeight && ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right ) {
    rc = iRECT( nFixedColumnWidth - 1, ClipRect.top, VisRect.right, nFixedRowHeight );
    FillRect( pDC, &rc, FixedRowBack );
  }
  // Draw non-fixed cell background
  if ( IntersectRect( &rect, &VisRect, &ClipRect ) ) {
    RECT CellRect = iRECT( __max( nFixedColumnWidth, rect.left ), __max( nFixedRowHeight, rect.top ), rect.right, rect.bottom );
    FillRect(pDC, &CellRect, TextBack );
  }
  // Draw right hand side of window outside grid
  if ( VisRect.right < ClipRect.right ) {
    rc = iRECT( VisRect.right, ClipRect.top, ClipRect.right, ClipRect.bottom );
    FillRect(pDC, &rc, Back );
  }
  // Draw bottom of window below grid
  if ( VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right ) {
    rc = iRECT( ClipRect.left, VisRect.bottom, VisRect.right, ClipRect.bottom );
    FillRect(pDC, &rc, Back );
  }
}

// CGridCellBase Operations
// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell() and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL CGridCellBase_Draw( CControlUI* ctr, CGridCell* pCell, HDC pDC, int nRow, int nCol, RECT rect, BOOL bEraseBkgnd /*=TRUE*/ ) {
  UIGridCtrl* pGrid = (UIGridCtrl*)ctr;
  int nSavedDC;
  CGridCell *pDefaultCell;
  COLORREF TextClr, TextBkClr;
  ASSERT( pGrid );
  if ( !pGrid || !pDC )
    return FALSE;
  if ( rcWidth(rect) <= 0 || rcHeight(rect) <= 0 )         // prevents imagelist item from drawing even
    return FALSE;                             //  though cell is hidden
  //TRACE3("Drawing %scell %d, %d\n", IsFixed()? _T("Fixed ") : _T(""), nRow, nCol);
  nSavedDC = SaveDC(pDC);
  SetBkMode( pDC, TRANSPARENT );
  // Get the default cell implementation for this kind of cell. We use it if this cell
  // has anything marked as "default"
  pDefaultCell = CellGetDefaultCell(pCell);
  if ( !pDefaultCell )
    return FALSE;
  // Set up text and background colours
  TextClr = ( GetTextClr(pCell) == CLR_DEFAULT ) ? GetTextClr(pDefaultCell) : GetTextClr(pCell);
  if ( GetBackClr(pCell) == CLR_DEFAULT ) {
    TextBkClr = GetBackClr(pDefaultCell);
  } else {
    bEraseBkgnd = TRUE;
    TextBkClr = GetBackClr(pCell);
  }
  // Draw cell background and highlighting (if necessary)
  if ( CellIsFocused(pCell) || CellIsDropHighlighted(pCell) ) {
    // Always draw even in list mode so that we can tell where the
    // cursor is at.  Use the highlight colors though.
    if ( GetState(pCell) & GVIS_SELECTED ) {
      TextBkClr = GetSysColor( COLOR_HIGHLIGHT );
      TextClr = GetSysColor( COLOR_HIGHLIGHTTEXT );
      bEraseBkgnd = TRUE;
    }
    rect.right++;
    rect.bottom++;    // FillRect doesn't draw RHS or bottom
    if ( bEraseBkgnd ) {
      HBRUSH brush = CreateSolidBrush( TextBkClr );
      FillRect( pDC, &rect, brush );
      DeleteObject(brush);
    }
    // Don't adjust frame rect if no grid lines so that the
    // whole cell is enclosed.
    if ( GetGridLines(pGrid) != GVL_NONE ) {
      rect.right--;
      rect.bottom--;
    }
    if ( GetFrameFocusCell(pGrid) ) {
      // Use same color as text to outline the cell so that it shows
      // up if the background is black.
      HBRUSH brush = CreateSolidBrush( TextClr );
      FrameRect( pDC, &rect, brush );
      DeleteObject(brush);
    }
    SetTextColor( pDC, TextClr );
    // Adjust rect after frame draw if no grid lines
    if ( GetGridLines(pGrid) == GVL_NONE ) {
      rect.right--;
      rect.bottom--;
    }
    //rect.DeflateRect(0,1,1,1);  - Removed by Yogurt
  } else if ( ( GetState(pCell) & GVIS_SELECTED ) ) {
    rect.right++;
    rect.bottom++;    // FillRect doesn't draw RHS or bottom
    FillSolidRect2( pDC, &rect, GetSysColor( COLOR_HIGHLIGHT ) );
    rect.right--;
    rect.bottom--;
    SetTextColor( pDC, GetSysColor( COLOR_HIGHLIGHTTEXT ) );
  } else {
    if ( bEraseBkgnd ) {
      HBRUSH brush = CreateSolidBrush( TextBkClr );
      rect.right++;
      rect.bottom++;    // FillRect doesn't draw RHS or bottom
      FillRect( pDC, &rect, brush );
      rect.right--;
      rect.bottom--;
      DeleteObject(brush);
    }
    SetTextColor( pDC, TextClr );
  }
  // Draw lines only when wanted
  if ( CellIsFixed(pCell) && GetGridLines(pGrid) != GVL_NONE ) {
    CCellID FocusCell = GetFocusCell(pGrid);
    // As above, always show current location even in list mode so
    // that we know where the cursor is at.
    BOOL bHiliteFixed = GetTrackFocusCell(pGrid) && GridIsValid( pGrid, FocusCell.row, FocusCell.col ) &&
                        ( FocusCell.row == nRow || FocusCell.col == nCol );
    // If this fixed cell is on the same row/col as the focus cell,
    // highlight it.
    if ( bHiliteFixed ) {
      rect.right++;
      rect.bottom++;
      DrawEdge( pDC, &rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT );
      DeflateRect( &rect, 1, 1 );
    } else {
      HPEN lightpen = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DHIGHLIGHT ) );
      HPEN darkpen = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DDKSHADOW ) );
      HPEN pOldPen = GetCurrentPen(pDC);
      SelectObject( pDC, lightpen );
      MoveTo( pDC, rect.right, rect.top );
      LineTo( pDC, rect.left, rect.top );
      LineTo( pDC, rect.left, rect.bottom );
      SelectObject( pDC, darkpen );
      MoveTo( pDC, rect.right, rect.top );
      LineTo( pDC, rect.right, rect.bottom );
      LineTo( pDC, rect.left, rect.bottom );
      SelectObject( pDC, pOldPen );
      DeflateRect( &rect, 1, 1 );
      DeleteObject(darkpen);
      DeleteObject(lightpen);
    }
  }
  // Draw Text and image
  //rect.DeflateRect(GetMargin(), 0); - changed by Yogurt
  DeflateRect( &rect, CellGetMargin(pCell), CellGetMargin(pCell) );
  rect.right++;
  rect.bottom++;
  if ( GetImageList(pGrid) && CellGetImage(pCell) >= 0 ) {
    IMAGEINFO Info;
    if ( GetImageInfo( GetImageList(pGrid), CellGetImage(pCell), &Info ) ) {
      //  would like to use a clipping region but seems to have issue
      //  working with CMemDC directly.  Instead, don't display image
      //  if any part of it cut-off

      // CRgn rgn;
      // rgn.CreateRectRgnIndirect(rect);
      // pDC.SelectClipRgn(&rgn);
      // rgn.DeleteObject();
      /*
      // removed by Yogurt
      int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
      int nImageHeight = Info.rcImage.bottom-Info.rcImage.top+1;
      if( nImageWidth + rect.left <= rect.right + (int)(2*GetMargin())
          && nImageHeight + rect.top <= rect.bottom + (int)(2*GetMargin())  )
      {
          pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
      }
      */ 
      // Added by Yogurt
      int nImageWidth = Info.rcImage.right - Info.rcImage.left;
      int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;
      if ( ( nImageWidth + rect.left <= rect.right ) && ( nImageHeight + rect.top <= rect.bottom ) )
        CImageList_Draw( GetImageList(pGrid), pDC, CellGetImage(pCell), TopLeft(rect), ILD_NORMAL );
      //rect.left += nImageWidth+GetMargin();
    }
  }
  // Draw sort arrow
  if ( GetSortColumn(pGrid) == nCol && nRow == 0 ) {
    BOOL bVertical;
    SIZE size = GetTextExtent( pDC, _T( "M" ), 1 );
    int nOffset = 2;
    // Base the size of the triangle on the smaller of the column
    // height or text height with a slight offset top and bottom.
    // Otherwise, it can get drawn outside the bounds of the cell.
    size.cy -= ( nOffset * 2 );
    if ( size.cy >= rcHeight(rect) )
      size.cy = rcHeight(rect) - ( nOffset * 2 );
    size.cx = size.cy;      // Make the dimensions square
    // Kludge for vertical text
    bVertical = ( CellGetFont(pCell) ->lfEscapement == 900 );
    // Only draw if it'll fit!
    //if (size.cx + rect.left < rect.right + (int)(2*GetMargin())) - changed / Yogurt
    if ( size.cx + rect.left < rect.right ) {
      HPEN penShadow, penLight;
      int nTriangleBase = rect.bottom - nOffset - size.cy;    // Triangle bottom right
      //int nTriangleBase = (rect.top + rect.bottom - size.cy)/2; // Triangle middle right
      //int nTriangleBase = rect.top + nOffset;                 // Triangle top right
      //int nTriangleLeft = rect.right - size.cx;                 // Triangle RHS
      //int nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
      //int nTriangleLeft = rect.left;                            // Triangle LHS
      int nTriangleLeft;
      if ( bVertical )
        nTriangleLeft = ( rect.right + rect.left - size.cx ) / 2; // Triangle middle
      else
        nTriangleLeft = rect.right - size.cx;               // Triangle RHS
      penShadow = CreatePen( PS_SOLID, 0, GetSysColor( COLOR_3DSHADOW ) );
      penLight = CreatePen( PS_SOLID, 0, GetSysColor( COLOR_3DHILIGHT ) );
      if ( GetSortAscending(pGrid) ) {
        // Draw triangle pointing upwards
        HPEN pOldPen = SelectObject( pDC, penLight );
        MoveTo( pDC, nTriangleLeft + 1, nTriangleBase + size.cy + 1 );
        LineTo( pDC, nTriangleLeft + ( size.cx / 2 ) + 1, nTriangleBase + 1 );
        LineTo( pDC, nTriangleLeft + size.cx + 1, nTriangleBase + size.cy + 1 );
        LineTo( pDC, nTriangleLeft + 1, nTriangleBase + size.cy + 1 );
        SelectObject( pDC, penShadow );
        MoveTo( pDC, nTriangleLeft, nTriangleBase + size.cy );
        LineTo( pDC, nTriangleLeft + ( size.cx / 2 ), nTriangleBase );
        LineTo( pDC, nTriangleLeft + size.cx, nTriangleBase + size.cy );
        LineTo( pDC, nTriangleLeft, nTriangleBase + size.cy );
        SelectObject( pDC, pOldPen );
      } else {
        // Draw triangle pointing downwards
        HPEN pOldPen = SelectObject( pDC, penLight );
        MoveTo( pDC, nTriangleLeft + 1, nTriangleBase + 1 );
        LineTo( pDC, nTriangleLeft + ( size.cx / 2 ) + 1, nTriangleBase + size.cy + 1 );
        LineTo( pDC, nTriangleLeft + size.cx + 1, nTriangleBase + 1 );
        LineTo( pDC, nTriangleLeft + 1, nTriangleBase + 1 );
        SelectObject( pDC, penShadow );
        MoveTo( pDC, nTriangleLeft, nTriangleBase );
        LineTo( pDC, nTriangleLeft + ( size.cx / 2 ), nTriangleBase + size.cy );
        LineTo( pDC, nTriangleLeft + size.cx, nTriangleBase );
        LineTo( pDC, nTriangleLeft, nTriangleBase );
        SelectObject( pDC, pOldPen );
      }
      if ( !bVertical ) {
        rect.right -= size.cy;
      }
      DeleteObject(penShadow);
      DeleteObject(penLight);
    }
  }
  // We want to see '&' characters so use DT_NOPREFIX
  CellGetTextRect( ctr, pCell, &rect );
  rect.right++;
  rect.bottom++;
  DrawText( pDC, CellGetText(pCell), -1, &rect, CellGetFormat(pCell) | DT_NOPREFIX );
  RestoreDC( pDC, nSavedDC );
  return TRUE;
}

// Returns the dimensions and placement of the checkbox in client coords.
RECT GetCheckPlacement(CGridCell* pCell) {
  int nWidth = GetSystemMetrics( SM_CXHSCROLL );
  RECT place = pCell->m_Rect;
  OffsetRect( &place, CellGetMargin(pCell), CellGetMargin(pCell) );
  place.right = place.left + nWidth;
  place.bottom = place.top + nWidth;
  if ( rcHeight(pCell->m_Rect) < nWidth + 2 * (int) ( CellGetMargin(pCell) ) ) {
    place.top = pCell->m_Rect.top + ( rcHeight(pCell->m_Rect) - nWidth ) / 2;
    place.bottom = place.top + nWidth;
  }
  return place;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCheck_Draw( CControlUI* ctr, CGridCell* pCell, HDC pDC, int nRow, int nCol, RECT rect, BOOL bEraseBkgnd /*=TRUE*/ ) {
  BOOL bResult = CGridCellBase_Draw( ctr, pCell, pDC, nRow, nCol, rect, bEraseBkgnd );
  // Store the cell's dimensions for later
  RECT CheckRect = GetCheckPlacement(pCell);
  pCell->m_Rect = rect;
  rect.left = CheckRect.right;
  // enough room to draw?
  // if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {
  // Do the draw
  rect = GetCheckPlacement(pCell);
  DrawFrameControl( pDC, &rect, DFC_BUTTON,
    ( pCell->m_bChecked ) ? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK );
  // }
  return bResult;
}
// UIGridCtrl implementation functions
void UIGridCtrl_DoPaint( CControlUI* ctr, HDC pDC, RECT rcPaint ) {
  RECT clipRect, rect, VisRect;
  int row, col, nFixedRowHeight, nFixedColWidth;
  int minVisibleRow, minVisibleCol;
  int maxVisibleRow, maxVisibleCol;
  HPEN pen;
  CCellID idTopLeft;
  CGridCell* pCell;
  CCellRange VisCellRange;
  GetClientRect( rect );
  if ( !m_bAllowDraw )
    return ;
  if ( GetClipBox(pDC, &clipRect ) == ERROR )
    return ;
  EraseBkgnd( ctr, rect, pDC ); // OnEraseBkgnd does nothing, so erase bkgnd here.
  // This necessary since we may be using a Memory DC.
  nFixedRowHeight = GetFixedRowHeight(ctr);
  nFixedColWidth = GetFixedColumnWidth(ctr);
  idTopLeft = GetTopleftNonFixedCell(ctr, FALSE);
  minVisibleRow = idTopLeft.row, minVisibleCol = idTopLeft.col;
  VisCellRange = GetVisibleNonFixedCellRange( ctr, rect, &VisRect, FALSE );
  maxVisibleRow = VisCellRange.m_nMaxRow, maxVisibleCol = VisCellRange.m_nMaxCol;
  // draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
  rect.bottom = -1;
  for ( row = 0; row < m_nFixedRows; row++ ) {
    if ( GetRowHeight( row ) <= 0 )
      continue;
    rect.top = rect.bottom + 1;
    rect.bottom = rect.top + GetRowHeight( row ) - 1;
    rect.right = -1;
    for ( col = 0; col < m_nFixedCols; col++ ) {
      if ( GetColumnWidth( col ) <= 0 )
        continue;
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( col ) - 1;
      GetCell( pCell, row, col );
      if ( pCell ) {
        SetCoords( pCell, row, col );
        //pCell->Draw( pDC, row, col, rect, FALSE );
      }
    }
  }
  // draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
  rect.bottom = nFixedRowHeight - 1;
  for ( row = minVisibleRow; row <= maxVisibleRow; row++ ) {
    if ( GetRowHeight( row ) <= 0 )
      continue;
    rect.top = rect.bottom + 1;
    rect.bottom = rect.top + GetRowHeight( row ) - 1;
    // rect.bottom = bottom pixel of previous row
    if ( rect.top > clipRect.bottom )
      break;    // Gone past cliprect
    if ( rect.bottom < clipRect.top )
      continue;             // Reached cliprect yet?
    rect.right = -1;
    for ( col = 0; col < m_nFixedCols; col++ ) {
      if ( GetColumnWidth( col ) <= 0 )
        continue;
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( col ) - 1;
      if ( rect.left > clipRect.right )
        break;            // gone past cliprect
      if ( rect.right < clipRect.left )
        continue;         // Reached cliprect yet?
      GetCell( pCell, row, col );
      if ( pCell ) {
        SetCoords( pCell, row, col );
        //pCell->Draw( pDC, row, col, rect, FALSE );
      }
    }
  }
  // draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
  rect.bottom = -1;
  for ( row = 0; row < m_nFixedRows; row++ ) {
    if ( GetRowHeight( row ) <= 0 )
      continue;
    rect.top = rect.bottom + 1;
    rect.bottom = rect.top + GetRowHeight( row ) - 1;
    // rect.bottom = bottom pixel of previous row
    if ( rect.top > clipRect.bottom )
      break;                // Gone past cliprect
    if ( rect.bottom < clipRect.top )
      continue;             // Reached cliprect yet?
    rect.right = nFixedColWidth - 1;
    for ( col = minVisibleCol; col <= maxVisibleCol; col++ ) {
      if ( GetColumnWidth( col ) <= 0 )
        continue;
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( col ) - 1;
      if ( rect.left > clipRect.right )
        break;        // gone past cliprect
      if ( rect.right < clipRect.left )
        continue;     // Reached cliprect yet?
      GetCell( pCell, row, col );
      if ( pCell ) {
        SetCoords( pCell, row, col );
        //pCell->Draw( pDC, row, col, rect, FALSE );
      }
    }
  }
  // draw rest of non-fixed cells
  rect.bottom = nFixedRowHeight - 1;
  for ( row = minVisibleRow; row <= maxVisibleRow; row++ ) {
    if ( GetRowHeight( row ) <= 0 )
      continue;
    rect.top = rect.bottom + 1;
    rect.bottom = rect.top + GetRowHeight( row ) - 1;
    // rect.bottom = bottom pixel of previous row
    if ( rect.top > clipRect.bottom )
      break;                // Gone past cliprect
    if ( rect.bottom < clipRect.top )
      continue;             // Reached cliprect yet?
    rect.right = nFixedColWidth - 1;
    for ( col = minVisibleCol; col <= maxVisibleCol; col++ ) {
      if ( GetColumnWidth( col ) <= 0 )
        continue;
      rect.left = rect.right + 1;
      rect.right = rect.left + GetColumnWidth( col ) - 1;
      if ( rect.left > clipRect.right )
        break;        // gone past cliprect
      if ( rect.right < clipRect.left )
        continue;     // Reached cliprect yet?
      GetCell( pCell, row, col );
      // TRACE(_T("Cell %d,%d type: %s\n"), row, col, pCell->GetRuntimeClass()->m_lpszClassName);
      if ( pCell ) {
        SetCoords( pCell, row, col );
        //pCell->Draw( pDC, row, col, rect, FALSE );
      }
    }
  }
  pen=CreatePen( PS_SOLID, 0, m_crGridLineColour );
  SelectObject( pDC, pen );
  // draw vertical lines (drawn at ends of cells)
  if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT ) {
    int x = nFixedColWidth;
    for ( col = minVisibleCol; col <= maxVisibleCol; col++ ) {
      if ( GetColumnWidth( col ) <= 0 )
        continue;
      x += GetColumnWidth( col );
      MoveTo( pDC, x - 1, nFixedRowHeight );
      LineTo( pDC, x - 1, VisRect.bottom );
    }
  }
  // draw horizontal lines (drawn at bottom of each cell)
  if ( m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ ) {
    int y = nFixedRowHeight;
    for ( row = minVisibleRow; row <= maxVisibleRow; row++ ) {
      if ( GetRowHeight( row ) <= 0 )
        continue;
      y += GetRowHeight( row );
      MoveTo( pDC, nFixedColWidth, y - 1 );
      LineTo( pDC, VisRect.right, y - 1 );
    }
  }
  SelectStockObject( pDC, NULL_PEN );
  // Let parent know it can discard it's data if it needs to.
  //if ( GetVirtualMode() ) SendCacheHintToParent( CCellRange( -1, -1, -1, -1 ) );
  DeleteObject(pen);
}
