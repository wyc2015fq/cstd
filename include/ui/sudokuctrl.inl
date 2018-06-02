#include "sudoku.inl"
#define XSUDOKU_MSG(nType, msg) printf("%d, %s\n", nType, msg)
#define TSetBkColor(a, b) 0
#define TSetTextColor(a, b) _clr=b;
static int g_nGridLineOffset[9] = { 0, 0, 0, 1, 1, 1, 2, 2, 2 };
static char* g_szDigits = ("0123456789");
#define SetBit(x,y) (x|=(1<<(y)))
#define ClearBit(x,y) (x&=~(1<<(y)))
#define TestBit(x,y) (x&(1<<(y)))
#define IDS_COLUMN_LABELS "ABCDEFGHI"
#define IDS_USER_ENTRY "user entry %d at %d%c is "
#define IDS_CORRECT "correct"
#define IDS_INCORRECT "incorrect"
#define IDS_SUDOKU_OK "Sudoku ok, 1 solution found"
#define IDS_SUDOKU_INVALID "Sudoku invalid, more than 1 solution"
#define IDS_SUDOKU_UNSOLVABLE "Sudoku invalid, no solutions"
#define IDS_NO_USER_ENTRY "There are no user entries."
#define IDS_USER_ENTRY_OK "All user entries are correct."
#define IDS_RESET "This operation cannot be undone. \r\n\r\nAre you sure you want to continue?"
#define ACTION_CODE_USER_ENTRY 1 // action_value = 0 to remove user entry
#define ACTION_CODE_USER_ENTRY_REMOVE_ALL 2
#define ACTION_CODE_HINT 3 // action_value: 0 = hide hint; else 1-9
#define ACTION_CODE_HINT_REMOVE_ALL 4
typedef struct sudokuctrl_t {
  ctrl_t c[1];
  COLORREF m_rgbWindowBackground;
  COLORREF m_rgbCellBackground;
  COLORREF m_rgb3x3Gridline;
  COLORREF m_rgbLabels;
  COLORREF m_rgbGivens;
  COLORREF m_rgbSolution;
  COLORREF m_rgbUserEntry;
  COLORREF m_rgbPencilMarks;
  COLORREF m_rgbHighlightNumber;
  COLORREF m_rgbCurCellBorder;
  IPOINT ptoff;
  ISIZE szCell;
  // current row and column number
  int m_nCurRow, m_nCurCol;
  //BOOL m_bPencilMarks;
  BOOL m_bIsValid;
  int m_nGivens[9 * 9];
} sudokuctrl_t;
// ActionCodeToString()
// Purpose: Convert undo action code to string
// Parameters: action_code - undo action code, see definition of UNDO_BLOCK
// Returns: char* - action code string; empty if action code = 0
static char* ActionCodeToString(BYTE action_code)
{
  char* str = ("");
  switch (action_code) {
  case 0: // unused entry
    break;
  case ACTION_CODE_USER_ENTRY:
    str = ("ACTION_CODE_USER_ENTRY");
    break;
  case ACTION_CODE_USER_ENTRY_REMOVE_ALL:
    str = ("ACTION_CODE_USER_ENTRY_REMOVE_ALL");
    break;
  case ACTION_CODE_HINT:
    str = ("ACTION_CODE_HINT");
    break;
  case ACTION_CODE_HINT_REMOVE_ALL:
    str = ("ACTION_CODE_HINT_REMOVE_ALL");
    break;
  default:
    str = ("ERROR - unknown action code");
    break;
  }
  return str;
}
// GetCellRect()
// Purpose: Get rect for a cell
// Parameters: row - row number (0 - 8)
// col - column number (0 - 8)
// Returns: IRECT - rect of the cell
static IRECT GetCellRect(const sudokuctrl_t* s, const IRECT* prc, int row, int col)
{
  IRECT rectCell, rectWnd = *prc;
  rectCell.l = rectWnd.l + s->ptoff.x + col * (s->szCell.w + 1) + g_nGridLineOffset[col];
  rectCell.r = rectCell.l + s->szCell.w;
  rectCell.t = rectWnd.t + s->ptoff.y + row * (s->szCell.h + 1) + g_nGridLineOffset[row];
  rectCell.b = rectCell.t + s->szCell.h;// - 1;
  return rectCell;
}
#define RGBMULI(clr, n) _RGB(MIN(255,(GetRV(clr)*n)>>8), MIN(255,(GetGV(clr)*n)>>8), MIN(255,(GetBV(clr)*n)>>8))
// CellFromPoint()
// Purpose: Get cell row, column for a point
// Parameters: point - point that lies within a cell
// row - returned row number (0 - 8)
// col - returned column number (0 - 8)
// Returns: BOOL - TRUE = point lies within a cell
static int CellFromPoint(sudokuctrl_t* s, const IRECT* prc, int x, int y, IPOINT* out)
{
  int i, j;
  IRECT rectCell;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      rectCell = GetCellRect(s, prc, i, j);
      if (iPtInRect(&rectCell, x, y)) {
        out->y = i;
        out->x = j;
        return 1;
      }
    }
  }
  return 0;
}
// AddUndoAction()
// Purpose: Add action to undo list
// Parameters: row - current row number for action
// col - current column number for action
// action_code - what action
// action_value - value associated with action
// Returns: BOOL - TRUE = action successfully added
static BOOL AddUndoAction(int row, int col, int action_code, int action_value)
{
  BOOL ok = FALSE;
#if 0
  UNDO_BLOCK* pUB = m_Undo + m_nUndoIndex;
  if (!m_bEnableUndo) {
    return ok;
  }
  ASSERT((action_code >= 1) && (action_code <= 4));
  pUB->action_code = (BYTE)action_code;
  pUB->action_value = (BYTE)action_value;
  pUB->row = (BYTE)row;
  pUB->col = (BYTE)col;
  // advance undo pointers
  m_nUndoIndex++;
  // there may be other entries in the undo list after this one -
  // the user may have undone some entries, and then entered new data.
  // m_nUndoLastEntry always points to the last user action
  m_nUndoLastEntry = m_nUndoIndex;
  ASSERT(m_nUndoIndex < m_nUndoSize);
  ok = TRUE;
#endif
  return ok;
}
static LRESULT sudokuctrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  sudokuctrl_t* s = (sudokuctrl_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  IPOINT pt1;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    // Notes: Construction is a two-step process. First, construct the
    // CXSudokuWnd object. Second, call CXSudokuWnd::Create to create
    // the CXSudokuWnd window.
    // int m_bPencilMarks = FALSE;
    s->m_rgbWindowBackground = _RGB(220, 20, 60);
    s->m_rgbCellBackground = _RGB(192, 192, 192);
    s->m_rgbLabels = _RGB(255, 255, 255);
    s->m_rgbGivens = _RGB(220, 20, 60);
    s->m_rgbSolution = _RGB(0, 0, 255);
    s->m_rgbUserEntry = _RGB(0, 0, 0);
    s->m_rgbPencilMarks = _RGB(0, 0, 128);
    s->m_rgbHighlightNumber = _RGB(135, 206, 250);
    s->m_rgbCurCellBorder = _RGB(255, 255, 0);
    s->m_rgb3x3Gridline = _RGB(0, 0, 0);
    s->ptoff.x = 20;
    s->ptoff.y = 20;
    s->szCell.w = 45;
    s->szCell.h = 45;
    s->m_nCurRow = s->m_nCurCol = -1;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = (s->szCell.w) * 9 + 8 + 40;
    mminfo->ptMinTrackSize.y = (s->szCell.h) * 9 + 8 + 40;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (CellFromPoint(s, &rc, x, y, &pt1)) {
      s->m_nCurCol = pt1.x;
      s->m_nCurRow = pt1.y;
      force_redraw(c);
      return 1;
    }
    else {
      s->m_nCurCol = -1;
      s->m_nCurRow = -1;
      force_redraw(c);
      return 1;
    }
    break;
  case WM_KEYUP:
    if ((wParam >= ('0')) && (wParam <= ('9'))) {
      if ((s->m_nCurRow != -1) &&
          (s->m_nCurCol != -1) &&
          (s->m_nGivens[s->m_nCurRow * 9 + s->m_nCurCol] == 0)) {
        // find all possible candidates for this cell
        WORD wCandidates = GetCandidates(s->m_nGivens, s->m_nCurRow, s->m_nCurCol);
        int nDigit = wParam - ('0');
        if (nDigit && TestBit(wCandidates, nDigit)) {
          int nType = 0;
          char str[256] = ("");
          char* strColumns;
          char* strCorrect = ("");
          s->m_nGivens[s->m_nCurRow * 9 + s->m_nCurCol] = nDigit;
          AddUndoAction(s->m_nCurRow, s->m_nCurCol, ACTION_CODE_USER_ENTRY, nDigit);
          strColumns = (IDS_COLUMN_LABELS);
          _snprintf(str, 256, IDS_USER_ENTRY, nDigit, s->m_nCurRow + 1, strColumns[s->m_nCurCol]);
          strcat(str, strCorrect);
          XSUDOKU_MSG(nType, str);
        }
        // NOTE: must set hint to 0 or user entry will not be displayed
        force_redraw(c);
      }
      return TRUE;
    }
    break;
  case WM_KEYDOWN: {
    int nOldRow = s->m_nCurRow;
    int nOldCol = s->m_nCurCol;
    BOOL bUpdate = FALSE;
    switch (wParam) {
    case VK_DOWN:
      if (s->m_nCurRow < 8) {
        s->m_nCurRow++;
        bUpdate = TRUE;
      }
      break;
    case VK_UP:
      if (s->m_nCurRow > 0) {
        s->m_nCurRow--;
        bUpdate = TRUE;
      }
      break;
    case VK_RIGHT:
      if (s->m_nCurCol < 8) {
        s->m_nCurCol++;
        bUpdate = TRUE;
      }
      break;
    case VK_LEFT:
      if (s->m_nCurCol > 0) {
        s->m_nCurCol--;
        bUpdate = TRUE;
      }
      break;
    case VK_HOME:
      if (s->m_nCurCol > 0) {
        s->m_nCurCol = 0;
        bUpdate = TRUE;
      }
      break;
    case VK_END:
      if (s->m_nCurCol < 8) {
        s->m_nCurCol = 8;
        bUpdate = TRUE;
      }
      break;
    case VK_PRIOR:
      if (s->m_nCurRow > 0) {
        s->m_nCurRow = 0;
        bUpdate = TRUE;
      }
      break;
    case VK_NEXT:
      if (s->m_nCurRow < 8) {
        s->m_nCurRow = 8;
        bUpdate = TRUE;
      }
      break;
    }
    if (bUpdate) {
      force_redraw(c);
    }
  }
  break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      // OnPaint()
      // Purpose: The framework calls this member function when Windows or an
      // application makes a request to repaint a portion of the
      // control's window.
      // Notes: OnPaint() always first fills the entire client background with
      // the control's background color, so there is never a need to call
      // force_redraw(c), () with TRUE.
      {
        int i, j;
        DWORD _clr;
        int nHighlightNumber = 0;
        IRECT* prc = &rc;
        IRECT rect = *prc;
        int m_bPencilMarks = 1;
        imdraw_fillrect(im, &rect, rect, s->m_rgbWindowBackground);
        // Draw values in cells
        {
          font_t* fo1 = sys_font_load_win32(s->c->sys, "Arial|30|B");
          if (s->m_nCurRow >= 0 && s->m_nCurCol >= 0) {
            nHighlightNumber = s->m_nGivens[s->m_nCurRow * 9 + s->m_nCurCol];
          }
          for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
              IRECT rectCell = GetCellRect(s, prc, i, j);
              IRECT rectValue = rectCell;
              int nValue = s->m_nGivens[i * 9 + j];
              _clr = s->m_rgbCellBackground;
              if ((nHighlightNumber != 0)) {
                WORD wCandidates = GetCandidates(s->m_nGivens, i, j);
                int nGiven = s->m_nGivens[i * 9 + j];
                if (nGiven) {
                  if (nGiven == nHighlightNumber) {
                    _clr = s->m_rgbHighlightNumber;
                  }
                }
                else if (TestBit(wCandidates, nHighlightNumber)) {
                  _clr = s->m_rgbHighlightNumber;
                }
              }
              imdraw_fillrect(im, &rectCell, rectCell, _clr);
              _clr = s->m_rgbGivens;
              // Paint cell at s->m_nCurRow, s->m_nCurCol
              if ((s->m_nCurRow == i) && (s->m_nCurCol == j)) {
                imdraw_rect(im, &rectCell, rectCell, 0, s->m_rgbCurCellBorder, 3, BDM_ALL);
                _clr = RGBMULI(_clr, 300);
              }
              else if (nHighlightNumber > 0 && nHighlightNumber == nValue) {
                imdraw_rect(im, &rectCell, rectCell, 0, s->m_rgbCurCellBorder, 3, BDM_ALL);
                _clr = RGBMULI(_clr, 300);
              }
              if (nValue && nValue <= 9) {
                imdraw_text(im, &rectValue, rectValue, &g_szDigits[nValue], 1, fo1, _clr, 0, DT_CENTER | DT_VCENTER);
              }
            }
          }
        }
        m_bPencilMarks = 1;
        // Draw pencil marks (aka candidate values) for a cell
        if (m_bPencilMarks) {
          font_t* fo2 = sys_font_load_win32(s->c->sys, ("Verdana|12|B"));
          for (i = 0; i < 9; i++) {
            for (j = 0; j < 9; j++) {
              WORD wCandidates;
              IRECT rectCell, rectPencilMarks, rcs[9];
              int nCandidate, nGiven;
              nGiven = s->m_nGivens[i * 9 + j];
              if (nGiven) {
                continue;
              }
              // find all possible candidates for this cell
              wCandidates = GetCandidates(s->m_nGivens, i, j);
              rectCell = GetCellRect(s, prc, i, j);
              iRectMatrix(rectCell, 3, 3, 0, 0, 9, rcs);
              for (nCandidate = 1; nCandidate <= 9; nCandidate++) {
                if (TestBit(wCandidates, nCandidate)) {
                  rectPencilMarks = rcs[nCandidate - 1];
                  _clr = s->m_rgbPencilMarks;
                  if (nHighlightNumber > 0 && nHighlightNumber == nCandidate) {
                    _clr = RGBMULI(s->m_rgbGivens, 300);
                  }
                  imdraw_text(im, &rectCell, rectPencilMarks, &g_szDigits[nCandidate], 1, fo2, _clr, 0, DT_CENTER | DT_VCENTER);
                }
              }
            }
          }
        }
        // Draw 3x3 grid lines
        {
          int _x0, _y0;
          DWORD _clr = (s->m_rgb3x3Gridline);
          IRECT rectWnd, rectLine;
          rectWnd = *prc;
          rectLine.t = rectWnd.t + s->ptoff.y;
          rectLine.b = rectLine.t + 9 * s->szCell.h + 8 + 2;
          // vertical lines
          rectLine.l = rectWnd.l + s->ptoff.x + 3 * s->szCell.w + 2;
          TMoveTo(im, rectLine.l, rectLine.t);
          _LineTo(im, rectLine.l, rectLine.b);
          TMoveTo(im, rectLine.l + 1, rectLine.t);
          _LineTo(im, rectLine.l + 1, rectLine.b);
          rectLine.l += 2 + 3 * s->szCell.w + 2;
          TMoveTo(im, rectLine.l, rectLine.t);
          _LineTo(im, rectLine.l, rectLine.b);
          TMoveTo(im, rectLine.l + 1, rectLine.t);
          _LineTo(im, rectLine.l + 1, rectLine.b);
          // horizontal lines
          rectLine.l = rectWnd.l + s->ptoff.x;
          rectLine.r = rectLine.l + 9 * s->szCell.w + 8 + 2;
          rectLine.t += 3 * s->szCell.h + 2;
          TMoveTo(im, rectLine.l, rectLine.t);
          _LineTo(im, rectLine.r, rectLine.t);
          TMoveTo(im, rectLine.l, rectLine.t + 1);
          _LineTo(im, rectLine.r, rectLine.t + 1);
          rectLine.t += 3 * s->szCell.h + 2 + 2;
          TMoveTo(im, rectLine.l, rectLine.t);
          _LineTo(im, rectLine.r, rectLine.t);
          TMoveTo(im, rectLine.l, rectLine.t + 1);
          _LineTo(im, rectLine.r, rectLine.t + 1);
        }
        // Draw row and column labels
        if (0) {
          static char* szXLabel = ("ABCDEFGHI");
          IRECT rectWnd, rectLabel;
          font_t* fo2 = sys_font_load_win32(s->c->sys, ("Verdana|12|B"));
          TSetBkColor(im, s->m_rgbWindowBackground);
          TSetTextColor(im, s->m_rgbLabels);
          rectWnd = *prc;
          rectLabel.t = rectWnd.t;
          rectLabel.b = rectLabel.t + s->ptoff.y;
          rectLabel.l = rectWnd.l + s->ptoff.x;
          rectLabel.r = rectLabel.l + s->szCell.w;
          for (i = 0; i < 9; i++) {
            rectLabel.l += g_nGridLineOffset[i];
            imdraw_text(im, &rectWnd, rectLabel, &szXLabel[i], 1, fo2, _clr, 0, DT_CENTER | DT_VCENTER);
            rectLabel.l += s->szCell.w;
            rectLabel.r = rectLabel.l + s->szCell.w;
          }
          rectLabel.t = rectWnd.t + s->ptoff.y;
          rectLabel.b = rectLabel.t + s->szCell.h;
          rectLabel.l = rectWnd.l;
          rectLabel.r = rectLabel.l + s->ptoff.x;
          for (i = 0; i < 9; i++) {
            rectLabel.t += g_nGridLineOffset[i];
            imdraw_text(im, &rectWnd, rectLabel, &g_szDigits[i + 1], 1, fo2, _clr, 0, DT_CENTER | DT_VCENTER);
            rectLabel.t += s->szCell.h;
            rectLabel.b = rectLabel.t + s->szCell.h;
          }
        }
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}
CC_INLINE int sudokuctrl_set(sudokuctrl_t* s, ctrl_t* li, ctrl_t* fa, const char* strSudoku)
{
  ctrl_set(sudokuctrl_proc, s, s->c, li, WT_CHILD, 0);
  sudoku_set_str(s->m_nGivens, strSudoku);
  return 0;
}

