// DiagInp.cpp : implementation file
//


#include "RubixCube.h"
#include "ChildView.h"
#include "DiagInp.h"

static const CString sidesId("UDLRFB");
//
// CDiagInp dialog

CDiagInp::CDiagInp(CWnd* pParent)
  : CDialog(CDiagInp::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDiagInp)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_pWnd = pParent;
  m_btnId = 0;
  m_sideId = -1;
  m_autoCompleteEnable = true;
  m_centers = 0;
}

void CDiagInp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDiagInp)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiagInp, CDialog)
  //{{AFX_MSG_MAP(CDiagInp)
  ON_WM_DRAWITEM()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_BN_CLICKED(ID_COLERASE, OnColerase)
  ON_BN_CLICKED(ID_CINIT, OnCinit)
  ON_BN_CLICKED(ID_COLCOPY, OnColcopy)
  ON_WM_CONTEXTMENU()
  //}}AFX_MSG_MAP
  ON_CONTROL_RANGE(BN_CLICKED, ID_FACE_R0, ID_FACE_R3, OnOrientFace)
END_MESSAGE_MAP()

BOOL CDiagInp::OnInitDialog()
{
  CChildView* pView = (CChildView*)m_pWnd;
  CDialog::OnInitDialog();

  CreateTables();
  ColorsErase();

  CubeColorsInit(pView->m_cubes);

  for (UINT id = IDC_CHKCOLOR0; id <= IDC_CHKCOLOR5; id++) {
    GetDlgItem(id)->ModifyStyle(0, BS_OWNERDRAW);
  }

  GetDlgItem(IDOK)->EnableWindow(IsComplete());

  // cree les positions des touches facettes
  IRECT rect;
  GetDlgItem(IDC_RCINP)->GetWindowRect(rect);
  ScreenToClient(rect);
  rect.DeflateRect(5, 10, 5, 5);
  ComputRCSides(rect, facetsRect);

  EnableToolTips(TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
}               // EXCEPTION: OCX Property Pages should return FALSE
//
// ToolTips

int CDiagInp::OnToolHitTest(IPOINT point, TOOLINFO* pTI) const
{
  CWnd* pWnd;

  if (m_hWnd && (pWnd = ChildWindowFromPoint(point, CWP_SKIPINVISIBLE))) {
    UINT id = pWnd->GetDlgCtrlID();

    pTI->hinst = ::AfxGetInstanceHandle();
    pTI->hwnd = m_hWnd;

    if (id != IDC_RCINP) {
      // the tooltips is created if a message exists for this object
      pTI->uFlags = TTF_ALWAYSTIP | TTF_IDISHWND;
      pTI->uId  = (UINT)pWnd->m_hWnd;
      pTI->lpszText = (LPSTR)id;
      return id;

    }
    else {
      // there is no object detected
      for (UINT i = 0; i < 54; i++) {
        if (facetsRect[i].PtInRect(point)) {
          // the point intercepts a facet of the Cube, type selection
          UINT n = i % 9, idt = (n == 4) ? ID_FACETCENT :
              (n & 1) ? ID_FACETEDGE : ID_FACETCORN;

          pTI->uFlags = TTF_ALWAYSTIP;
          pTI->rect = facetsRect[i];
          pTI->uId  = id;
          pTI->lpszText = (LPSTR)idt;
          return idt;
        }
      }
    }
  }

  return -1;
}
//
// tables of cubies ordered by number of visible facets (locations)
static BYTE center[1], noCenters[6], noEdges[12], noCorners[8];
static struct tCubes {
  BYTE* pTb, len;
} typeCubes[4] = {
  {center, 0}, {noCenters, 0}, {noEdges, 0}, {noCorners, 0}
};

void CDiagInp::CreateTables()
{
  // list of edge cubies and corner cubies
  if (typeCubes[0].len == 0) {
    for (BYTE i = 0; i < 27; i++) {
      struct tCubes& tc = typeCubes[SidesCount(GetNoSides(i)) & 3];
      tc.pTb[tc.len++] = i;
    }
  }

  ASSERT(typeCubes[1].len = 6 && typeCubes[2].len == 12 && typeCubes[3].len == 8);
}
//
// copy of the showing cube in the input window

void CDiagInp::CubeColorsInit(const CCubes& cubes)
{

  SetSideText(cubes.GetText(), m_colors, "ORGBWYUDLRFB");
  m_centers = cubes.CenterSidesRotation();
}
//
// Definition of the color facets, center of faces, and the orientation
// of the Rubik's Cube

bool CDiagInp::SetCubeOrientation()
{
  // Search the facets, face center, already filled
  t_GROUP nSides = GetOrientSides(m_colors);

  if (nSides != GROUP_EMPTY) {
    // Extract the corresponding colors
    t_GROUP colors = GetSideColors(nSides, m_colors);
    // Calculates the corresponding orientation of the Rubik's Cube
    t_Matrix or = GetCubeOrientation(RefColors(nSides), colors, 0);

    // t_Matrix or = CMatrix::ComputOrient(RefColors(nSides), colors, 0);
    if ( or == 0) {
      return false;  // error, bad colors !
    }

    // Calculates the colors of the central facets  missing  by
    // positioning a Rubik's Cube with this orientation
    CMatrix m( or);
    BYTE facets[54];
    CCubes cubes(1.f, NULL, NULL, &m);

    cubes.Fixe();
    SetSideText(cubes.GetText(), facets, "ORGBWYUDLRFB");

    // updating the colors of facets, face center, in the color table
    for (int n = 4; n < 54; n += 9) {
      m_colors[n] = facets[n];
    }

    m_orient = or;
  }

  return true;
}
//
// Initializes the color tables

void CDiagInp::ColorsErase()
{
  for (UINT noSide = 0; noSide < 54; noSide++) {
    m_colors[noSide] = 0xFF;
  }

  m_orient = 0;
}

// Clears the facets of the center of the face and removes the orientation of the Cube

void CDiagInp::FCColorsErase()
{
  for (UINT noSide = 4; noSide < 54; noSide += 9) {
    m_colors[noSide] = 0xFF;
  }

  m_orient = 0;
}

// Clears the facet color

void CDiagInp::DelColor(BYTE noSide)
{
  ASSERT(noSide < 54);

  m_colors[noSide] = 0xFF;
}

// Erases the colors of all facets of a cubie

void CDiagInp::DelColors(BYTE noCube, bool dif)
{
  ASSERT(noCube < 27);
  t_GROUP nSides = GetNoSides(noCube);

  if (!dif || GetSideColors(nSides, m_colors) != RefColors(nSides)) {
    DelColors(nSides);
  }
}

// Erases the colors of all facets of a cubie

void CDiagInp::DelColors(t_GROUP nSides)
{
  SetSideColors(nSides, DWORD(-1), m_colors);
}

// Erases the colors of the facets of a set of cubies of a given type

void CDiagInp::DelColorsType(BYTE type, bool dif)
{
  struct tCubes& tc = typeCubes[type & 3];

  for (UINT i = 0; i < tc.len; i++) {
    DelColors(tc.pTb[i], dif);
  }
}
//

bool CDiagInp::IsValidCornerColor(BYTE noSide, BYTE color) const
{
  t_GROUP colors = FillColor(noSide, color, m_colors);
  BYTE cnt = SidesCount(colors);

  if (cnt == 3) {
    // With this new color, all facets of this cubie are updated
    return (CheckSidesColors(colors) >= 0);

  }
  else if (cnt == 2) {
    // With this color, only two facets are updated
    BYTE cvrt = SearchAvailableColors(3) & ~cvrtColor(color);

    // you try the colors still available in the last facet
    while (cvrt) {
      BYTE c = colorCvrt(cvrt);

      if (CheckSidesColors(FillColor(colors, c)) >= 0) {
        return true;  // one color (at least) is available
      }

      cvrt ^= cvrtColor(c);
    }

    return false;
  }
  else {
    // a first color should always
    return true;
  }
}

bool CDiagInp::IsComplete() const
{
  for (int i = 0; i < 54; i++)
    if (m_colors[i] >= 6) {
      return false;
    }

  return true;
}

bool CDiagInp::IsUsed(t_COLORS cvrt) const
{
  struct tCubes& tc = typeCubes[colorsCount(cvrt) & 3];

  for (BYTE noCube = 0; noCube < tc.len; noCube++) {
    t_GROUP nSides = GetNoSides(tc.pTb[noCube]);

    if (cvrt == cvrtColors(GetSideColors(nSides, m_colors))) {
      return true;  // signature color is used
    }
  }

  return false;
}

// Counts the facets using this color based on the types of
// corner or edge cubies. Each uses the same color the maximum 4 times

char CDiagInp::ColorCount(t_COLORS cvrt, BYTE nbc) const
{
  struct tCubes& tc = typeCubes[nbc & 3];
  BYTE count, noCube;

  for (noCube = count = 0; noCube < tc.len; noCube++) {
    DWORD colors = GetSideColors(GetNoSides(tc.pTb[noCube]), m_colors);

    if (cvrtColors(colors) & cvrt) {
      count++;
    }
  }

  return count;  // maxi 4 colors for edge or corner
}

// Tests if the color is still available for this type of facet

bool CDiagInp::IsAvailable(t_COLORS cvrt, BYTE nbc) const
{
  return (ColorCount(cvrt, nbc) < 4);
}

BYTE CDiagInp::SearchAvailableColors(BYTE nbc) const
{
  BYTE out = 0;

  for (t_COLORS cvrt = cvrtColor(5); cvrt != 0; cvrt >>= 1)
    if (IsAvailable(cvrt, nbc)) {
      out |= cvrt;
    }

  return out;
}
//
// Indicates that all the elementary facets of one face of the Rubik's Cube
// are identical to the color of the central facet

bool CDiagInp::IsColorFullSide(BYTE noSide) const
{
  UINT n = (noSide / 9) * 9;       // first facet of the face
  BYTE color = m_colors[n + 4];    // color central facet

  if (color >= 6) {
    return false;  // central facet uninitialized
  }

  for (UINT k = n + 9; n < k; n++)
    if (m_colors[n] != color) {
      return false;
    }

  return true;
}

// erases all the elementary facets of one face of the Rubik's Cube
// or puts the face with the color of the central facet

void CDiagInp::FullSideColor(BYTE noSide, // no facet for this face
    bool del)    // erase or paint
{
  UINT n = (noSide / 9) * 9;    // first facet of the face
  BYTE color = m_colors[n + 4]; // color of the central facet

  if (color < 6) {

    for (UINT k = n + 9, c = n + 4; n < k; n++) {
      if (n == c) {
        continue;
      }

      if (!del) {
        // checks if the facet was not already colored

        m_colors[n] = color;
      }
      else if (m_colors[n] < 6) {
        // erase the facets
        m_colors[n] = 0xFF;
      }
    }
  }
}
//
// Complete the last edge or the last corner

bool CDiagInp::AutoCompletion(BYTE nbc)
{
  BYTE color, save;

  for (save = 0xFF, color = 0; color < 6; color++) {
    if (IsAvailable(cvrtColor(color), nbc)) {
      if (save < 6) {
        return false;
      }
      else {
        save = color;
      }
    }
  }

  if (save < 6) {
    int c = nbc & 1;

    // autocompletion
    for (int n = 0, k; n < 54;) {
      for (k = 0; k < 9; k++, n++) {
        // eliminates the filled facets and the central facet
        if (m_colors[n] < 6 || k == 4 || (k & 1) ==  c) {
          continue;
        }

        m_colors[n] = save;  // update
      }
    }

    return true;
  }

  return false;
}
//
// Search for logical color of a facet
// (Taking into account the color of the other facets)

void CDiagInp::AutoCompletion()
{
  BYTE old;

  do {
    if ((old = ColorsCount(m_colors)) == 54) {
      break;
    }

    for (BYTE noSide = 0; noSide < 54; noSide++) {
      if (m_colors[noSide] < 6 || (noSide % 9) == 4) {
        continue;
      }

      // this facet is not yet completed : it extracts the colors
      // already filled on the cube to which it belongs
      BYTE noCube = side2NoCube(noSide);
      t_GROUP nSides = GetNoSides(noCube);
      BYTE nbc = SidesCount(nSides);
      t_COLORS cvrt = cvrtColors(GetSideColors(nSides, m_colors));

      // if only a color lacks, it attempts the update
      if (cvrt && colorsCount(cvrt) == nbc - 1) {
        m_colors[noSide] = SearchColor(cvrt, noSide, nbc);
      }
    }

    AutoCompletion(2), AutoCompletion(3);
    FreeColorUpdate(2, m_colors), FreeColorUpdate(3, m_colors);

    // we restart a tour if a facet has been updated
  }
  while (old < ColorsCount(m_colors));
}

// Search for the color that may be suitable for this facet

BYTE CDiagInp::SearchColor(t_COLORS cvrt, // A set of colors already filled
    BYTE noSide,   // facet update
    BYTE nbc) const// type (number of visible facets)
{
  struct tCubes& tc = typeCubes[nbc & 3];
  BYTE color, tmp, found, i;

  // research color schemes that may be suitable
  for (found = 0xff, i = 0; i < tc.len; i++) {
    BYTE noCube = tc.pTb[i];
    tmp = cvrtColors(RefColors(GetNoSides(noCube)));

    // tests whether the color is compatible with existing
    if ((tmp & cvrt) == cvrt && !IsUsed(tmp) && IsAvailable(tmp ^ cvrt, nbc)) {
      color = colorCvrt(tmp ^ cvrt);

      if (nbc != 3 || IsValidCornerColor(noSide, color)) {
        // ok, it stores the first opportunity
        if (found == 0xff) {
          found = color;
        }
        else {
          return 0xff;
        }
      }
    }
  }

  return found;
}
//
// update of the color of one facet

void CDiagInp::ColorUpdate(int noSide)       // selected facet number
{
  BYTE color = BYTE(m_btnId - IDC_CHKCOLOR0);// selected color
  t_COLORS cvrt = cvrtColor(color);         // selected color in bit format
  BYTE old = m_colors[noSide];              // old color at this place
  BYTE noCube = side2NoCube(noSide);        // cube corresponding to this place
  t_GROUP nSides = GetNoSides(noCube);      // visible facets of the cube
  BYTE nbc = SidesCount(nSides);            // count of visible facets (1..3)
  // colors for this cube
  t_GROUP colors = GetSideColors(nSides, m_colors);
  t_COLORS icvrt = cvrtColors(colors);      // colors already entered in bit format

  if (nbc == 1) {
    // click on the central facet
    if (old == 0xff) {
      // color of the central facet undefined
      if (m_btnId == 0) {
        MessageBox("No color selected !");
        m_sideId = -1;
        return;
      }

      m_colors[noSide] = color;

      if (!SetCubeOrientation()) {
        MessageBox("Color center faces does not orient the cube\n"
            "with those already entered !");
        FCColorsErase();
      }
    }
    else if (IsColorFullSide(noSide)) {
      // color central facet already defined
      // and the entire face is the color of the central facet
      if (MessageBox("Would you erase the whole face ?",
          NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) {
        FullSideColor(noSide, true), m_autoCompleteEnable = false;
      }
    }
    else {
      // color central facet already defined, we set the whole face
      FullSideColor(noSide, false);
    }

    return;
  }

  if (m_btnId == 0) {
    MessageBox("No color selected !");
    m_sideId = -1;
    return;
  }

  if (ColorCount(cvrt, nbc) >= 4 && (icvrt & cvrt) == 0) {
    MessageBox("No facet of this type available for this color !");
    m_sideId = -1;
    return;
  }

  // check whether modification of a box already filled, it clears
  if (old < 6) {
    if (colorsCount(icvrt) == nbc
        && MessageBox("Faced already completely defined.\n"
            "Would you erase all the faces of\n"
            "this cubie ?",
            NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) {
      DelColors(noCube, false);
    }
    else {
      DelColor(noSide);
    }

    m_autoCompleteEnable = false;
    m_sideId = -1;

    if (old == color) {
      return;  // erase only
    }
  }

  if (icvrt != 0) {
    // one or more colors are already defined for this cube.
    // estimates which facets defined.
    BYTE n = colorsCount(icvrt) + 1;

    if ((icvrt & cvrt) != 0 || (n == nbc &&
        (!IsValid(icvrt | cvrt) || IsUsed(icvrt | cvrt)))) {
      // color already on another facet of this cube
      // or combination of colors incompatible or used for another cube.
      if (MessageBox("This color is not consistent with the\n"
          "other colors of this cubie already\n"
          "defined. Do you want to delete ?",
          NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) {
        m_autoCompleteEnable = false;
        DelColors(noCube, false);
        m_sideId = -1;
      }
      else {
        m_sideId = -1;
        return;
      }
    }

    if (nbc == 3 && !IsValidCornerColor(noSide, color)) {
      MessageBox("This color is not appropriate at this location !");
      m_sideId = -1;
      return;
    }
  }

  // sets the color of this facet
  m_autoCompleteEnable = true;
  m_colors[noSide] = color;
}
//
// Deduces the position of the elementary cubies from the facets colors,
// and updates the Rubik's Cube

bool CDiagInp::CubesUpdate(CCubes& cubes)
{
  CCubes work;
  int er;

  // generates a Rubik's Cube from the facet color table
  if ((er = work.Create(m_colors, true)) != 0) {
    // errors
    Invalidate();
    MessageBox(GetErrorMessage(er) + "Check the data entered !");
    return false;

    // final checks
  }
  else if ((er = (work.ColorsCheck()
      | work.SetCenterSidesRotation(m_centers))) != 0
      && MessageBox(GetErrorMessage(er) +
          "The Cube can be ordered ! Would you correct ?",
          NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) {
    return false;
  }

  // update the Rubik's Cube
  cubes = work;
  cubes.Comput(0);
  return true;
}
//
// CDiagInp message handlers

void CDiagInp::OnDrawItem(int nIDCtl, DRAWITEMSTRUCT* pDis)
{
  if (nIDCtl >= IDC_CHKCOLOR0 && nIDCtl <= IDC_CHKCOLOR5) {
    // provides design and management of the color buttons
    UINT state = DFCS_BUTTONPUSH;
    IRECT r = pDis->rcItem;
    CString msg;
    CWnd* pWnd;

    if (pDis->itemState & ODS_SELECTED) {
      state |= DFCS_PUSHED;

      // shows the new situation and disable the old button
      if (m_btnId != nIDCtl && (pWnd = GetDlgItem(m_btnId))) {
        pWnd->Invalidate();
      }

      m_btnId = nIDCtl;
    }
    else if (m_btnId == nIDCtl) {
      // old select color
      state |= DFCS_PUSHED;
    }

    // draw the button with the text and the right colors
    ::DrawFrameControl(pDis->hDC, &r, DFC_BUTTON, state);

    BYTE color = BYTE(nIDCtl - IDC_CHKCOLOR0);
    BYTE cvrt = cvrtColor(color);
    COLOR save = ::SetTextColor(pDis->hDC, RGBTextColor(color));

    r.DeflateRect(3, 3);
    ::FillRect(pDis->hDC, &r, (HBRUSH)GetBrush(color)->m_hObject);

    ::SetBkMode(pDis->hDC, TRANSPARENT);
    msg.Format("%de + %dc", 4 - ColorCount(cvrt, 2), 4 - ColorCount(cvrt, 3));
    ::DrawText(pDis->hDC, msg, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    ::SetTextColor(pDis->hDC, save);
  }
}

void CDiagInp::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  dc.SetBkMode(TRANSPARENT);
  CFont* pFont = dc.SelectObject(GetFont());

  // draw the buttons facets
  for (int n = 0; n < 54; n++) {
    BYTE color = m_colors[n];
    IRECT r = facetsRect[n];

    if (n % 9 == 4) {
      // central facet
      COLOR textColor = 0;

      if (color < 6) {
        textColor = RGBTextColor(color);
        dc.FillRect(r, GetBrush(color));
      }

      dc.SetTextColor(textColor);
      dc.DrawText(GetMsgCenter(m_centers, sidesId.GetAt(n / 9)),
          r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    }
    else {
      // others facets
      UINT state = (n == m_sideId) ? DFCS_BUTTONPUSH | DFCS_PUSHED
          : DFCS_BUTTONPUSH;

      dc.DrawFrameControl(r, DFC_BUTTON, state);

      if (color < 6) {
        r.DeflateRect(3, 3);
        dc.FillRect(r, GetBrush(color));
      }
    }
  }

  dc.SelectObject(pFont);
}

void CDiagInp::OnLButtonDown(UINT nFlags, IPOINT point)
{
  for (int i = 0; i < 54; i++) {
    if (!facetsRect[i].PtInRect(point)) {
      continue;
    }

    m_sideId = i;

    ColorUpdate(i);

    if (m_autoCompleteEnable) {
      AutoCompletion();
    }

    GetDlgItem(IDOK)->EnableWindow(IsComplete());
    break;
  }

  CDialog::OnLButtonDown(nFlags, point);

  Invalidate();
}

void CDiagInp::OnLButtonUp(UINT nFlags, IPOINT point)
{
  CDialog::OnLButtonUp(nFlags, point);

  if (m_sideId >= 0) {
    m_sideId = -1, Invalidate();
  }
}

void CDiagInp::OnColerase()
{
  ColorsErase();
  GetDlgItem(IDOK)->EnableWindow(FALSE);
  Invalidate();
}

void CDiagInp::OnColcopy()
{
  CChildView* pView = (CChildView*)m_pWnd;
  CubeColorsInit(pView->m_cubes);

  if (IsComplete()) {
    GetDlgItem(IDOK)->EnableWindow();
  }

  Invalidate();
}

void CDiagInp::OnCinit()
{
  for (int i = 0; i < 54; i++) {
    m_colors[i] = defNoColor(BYTE(i));
  }

  m_centers = 0;

  if (IsComplete()) {
    GetDlgItem(IDOK)->EnableWindow();
  }

  Invalidate();
}

void CDiagInp::OnOK()
{
  CChildView* pView = (CChildView*)m_pWnd;

  if (CubesUpdate(pView->m_cubes)) {
    // the update is possible, is terminated
    pView->Invalidate();
    CDialog::OnOK();
  }
}
//
// Management of the orientation of the facets center of face

void CDiagInp::OnContextMenu(CWnd* pWnd, IPOINT point)
{
  IPOINT pt = point;
  int no;                    // face number 0..5

  // search facet corresponding to the cursor position
  ScreenToClient(&pt);

  for (no = 0; !facetsRect[no * 9 + 4].PtInRect(pt); no++)
    if (no == 5) {
      return;  // exit, not found
    }

  m_face = sidesId[no];      // facet corresponding to the cursor position

  // create the orientation menu for this face
  HMENU hMenu = ::CreatePopupMenu();
  ::AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "Orientation face");

  for (int n = 0; n < 4; n++) { // for the 4 orientations
    DWORD all = SetCenters(n, n, n, n, n, n);
    UINT flags = (CenterFaceOrient(m_centers, m_face) == n) ?
        MF_STRING | MF_ENABLED | MF_CHECKED : MF_STRING | MF_ENABLED;

    ::AppendMenu(hMenu, flags, ID_FACE_R0 + n, GetMsgCenter(all, m_face));
  }

  // Display a menu at the cursor position
  ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN,
      point.x, point.y, 0, m_hWnd, NULL);
}

void CDiagInp::OnOrientFace(UINT id)
{
  m_centers = SetCenterFaceOrient(m_centers, m_face, id - ID_FACE_R0);
  Invalidate();
}

//
