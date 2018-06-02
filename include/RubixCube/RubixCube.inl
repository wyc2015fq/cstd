
// RubixCube window

struct RubixCube {
  IPOINT m_posML;
  BOOL m_flgML;
  IPOINT m_tchPos;
  BOOL m_runEdit;
  char m_cubesText[256];
  char m_comments[256];

  BYTE m_cubeNo;

  //UINT m_timer;
  // Rubik's Cube moving
  t_Command m_cmd;
  t_Mask m_move;
  int m_iCount;
  char m_hist[256];

  int m_yObs;
  float m_mObs[9];
  float m_mMov[9];

  CCubes m_cubes[1];
  int m_modeDeb; // mode d'affichage de debug
  short m_modeMsg;
  UINT m_wndMsgId; // mode d'affichage de debug

  BYTE m_color, m_ref;
  t_GROUP m_cSides;
  bool m_orEn;
  BYTE m_orients[6];

  // Dialog Data
  int   m_observerBtn;
  int m_btnColorId;
  int m_btnSideId;


  // Dialog Data
  BOOL  m_cmdAll;
  BOOL  m_cmdM;

  RubixCube* m_pView;
  char m_command[256];

  t_Command m_cmds[16];
  char m_code[256];
  char m_save[256];
  char m_work[256];
  char m_textBtnOPTISOLVER[256];
  int m_curPtr;
  BOOL m_continue;
  bool m_help;
  t_Matrix m_orient;

  // Generated message map functions
  char m_msg[256];
  UINT old;
  char m_edit[256];

#if 0

  BOOL Create(void* pParentWnd = NULL);
  void IsSelected();
  void OptiSolverCmd();

  inline bool IsBusy() const {
    return 0;
  }


  BOOL ExeCmd(bool frst);
  void ReversCmd();
  void RotateCmd();
  void SetHelp();
  void SetSelect(DWORD curs);
  char* GetText() {
    return m_edit;
  }

  // Overrides
  // ClassWizard generated function overrides
  BOOL OnInitDialog();

  // Implementation

  // Generated message map functions
  void OnCmdAll();
  void OnRun();
  void OnCmdM();
  void OnCmdhelp();
  void OnSetfocusEdtCmd();
  void OnStep();
  void OnCenters();
  void OnCmdRev();
  void OnCmdInit();
  void OnCmdOrigin();
  void OnClose();
  void OnOptiSolver();
  void OnDestroy();
  void OnCmdRot();

  void OnCmd(UINT id);

  // Construction
  void Update();

  // Overrides
  // ClassWizard generated function overrides
  BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

  COLOR* InitCustColors(void* pData);
  void SetSidesColors(int sideId, int colorId);
  void ObserverOrient();
  // Generated message map functions
  void OnDrawItem(img_t* im, const IRECT* pclip, IRECT rcItem, int nIDCtl, int itemState, const char* msg, const font_t* fo);
  void OnDefaultcolor();
  void OnShowWindow(BOOL bShow, UINT nStatus);
  void OnLButtonUp(UINT nFlags, IPOINT point);
  void OnClickedButton(UINT id);
  void OnObserverBtn(UINT id);
  void OnBtnOrigine();

  void OnDoubleclickedChkcolor(UINT id);


  void WndMsgDump();
  inline CCubes* GetCubes() {
    return &m_cubes;
  }

  void SetMsg(const char* msg, bool init = false);
  void SetMsg6(bool init = false) {
    SetMsg("$6", init);
  }

  void DrawRC(img_t* im, const IRECT* pclip, IRECT rect, const char* msg);

  void OnPaint(img_t* im, const IRECT* pclip, IRECT rc, const font_t* fo);

  // Operations


  CDiagExtr m_diagExtr;

  CStatusBar m_wndStatusBar;
  CToolBar m_wndToolBar;
  // Overrides

  // ClassWizard generated function overrides


  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void OnShowplan();
  void OnFileAcquire();
  void OnCubesDump(UINT id);
  void OnUpdateCubesDump(void* pCmdUI);
  // Operations
  // Overrides
  // ClassWizard generated function overrides

  // Implementation

  BOOL CubeRotate(t_Command cmd, bool save = true, DWORD curs = 0);
  void CubeMove(int pos);
  void InitObs(BYTE or = 1);
  void CubeOrigine();
  BOOL GetCubeText(const char* msg);
  void ColorUpdate();
  void CubeOrientation(const float* mat);
  void CubeRnd(bool pos);
  void Dump(UINT id);

  void OnCubeInit();
  void OnCubeOrigin();

  // Generated message map functions

#endif
};

static COLOR custColors[16];
static BYTE refColors[6];


static const char szFilter[] =         // names of files
    "RubixCube (*.rbx)|*.rbx|"          // internal format of RubixCube
    "Rubik's Cube (*.txt)|*.txt|"       // format  "L:OWGGORWGB F:YROGGOWRR...
    "Rubik Singmaster (*.sgm)|*.sgm|"   // format Singmaster UF UR UB UL DF...
    "Images faces (*.tif)|*.tif|"       // format TIFF : 6 images of 6 faces
    "Facets colors (*.col)|*.col|"      // table of the 6 sides colors
    "All Files (*.*)|*.*||";

// matrix  for the presentation of the Cube
static const float mo[9] = {0.916f, 0.382f, 0.142f, -0.407f, 0.856f, 0.334f, 0.0f, -0.363f, 0.935f};


void RubixCube_Init(RubixCube* s)
{
  s->m_yObs = (800);
  s->m_flgML = FALSE;

  //m_timer = 0;
  s->m_cubeNo = 0xFF;
  s->m_cmd = 0;
  s->m_move = 0;
  s->m_iCount = 0;

  s->m_modeDeb = -1;
  s->m_modeMsg = 0;

  M33_Set(s->m_mObs, 1.f);
  M33_Set(s->m_mMov, 1.f);
  M33_Cpy(s->m_mObs, mo);
  CCubes_Set(s->m_cubes, 100.f, &s->m_yObs, s->m_mObs, s->m_mMov);
  s->m_wndMsgId = 0;
  // reading the color of the faces of the Cube from the Register
  InitColors();
  // create the rotation table
  {
    utime_start(_start_time);
    InitRotateTable();
    printf("%f\n", utime_elapsed(_start_time));
  }
  // InitColorsRotateTable();

  {
    s->old = 0xa5a5a5a;
    s->m_modeMsg = 0;
  }
  {
    s->m_observerBtn = -1;

    s->m_btnColorId = 0;
    s->m_btnSideId = 0;
    s->m_color = s->m_ref = 0xFF;
    s->m_cSides = GROUP_EMPTY;
    s->m_orEn = true;

    s->m_orients[0] = 4;  // gauche
    s->m_orients[1] = 23; // dessus
    s->m_orients[2] = 1;  // face
    s->m_orients[3] = 17; // dessous
    s->m_orients[4] = 2;  // droite
    s->m_orients[5] = 3;  // derriere
  }
  {
    memset(s->m_code, (' '), 16);
    s->m_code[16] = 0;
    s->m_cmdAll = FALSE;
    s->m_cmdM = FALSE;
    s->m_curPtr = 0;
    s->m_help = false;
    s->m_orient = 1;
  }
#if 0
  s->Create(s);
  s->m_diagExtr.Create(s);
#endif
}

// RubixCube
void RubixCube_SetMsg(RubixCube* s, const char* msg, bool init)
{
  if (init) {

    if (msg == "$$") {

      s->m_msg[0] = 0;
      s->old = 0;
    }
    else if (0 == strcmp(msg, s->m_msg) || (0 == strcmp(msg, "$6") && s->old != CCubes_GetHash(s->m_cubes))) {
      IRECT rdim = {0, 0, 0, 0};

      strcpy(s->m_msg, msg);

      if (strlen(s->m_msg) >= 70 && s->m_msg[1] == ':') {
        rdim = iRECT(0, 0, 184, 138);
      }
      else if (s->m_msg != "$6") {
        //dc.DrawText(s->m_msg, rdim, DT_CALCRECT);
        s->old = 0;
      }
      else {
        s->old = CCubes_GetHash(s->m_cubes);
        rdim = CCubes_Comput2D(s->m_cubes, 15, iPOINT(5, 5));
        rdim.l = rdim.t = 0;
      }

      //SetWindowPos(NULL, 10, 50, rdim.Width() + 10, rdim.Height() + 10, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
    }
  }
}

void RubixCube_SetMsg6(RubixCube* s, bool init = false)
{
  RubixCube_SetMsg(s, "$6", init);
}
void RubixCube_WndMsgDump(RubixCube* s)
{
  char tmp[256] = {0};

  switch (s->m_wndMsgId) {
  case ID_CUBES_DUMP_00 :
  case ID_CUBES_DUMP_01 :
  case ID_CUBES_DUMP_02 :
  case ID_CUBES_DUMP_03 :
  case ID_CUBES_DUMP_04 :
  case ID_CUBES_DUMP_05 :
  case ID_CUBES_DUMP_06 :
  case ID_CUBES_DUMP_07 : // 00 to 07
    CCubes_Dump(s->m_cubes, tmp, 256, s->m_wndMsgId - ID_CUBES_DUMP_00);
    RubixCube_SetMsg(s, tmp, true);
    break;

  case ID_CUBES_DUMP_08 :
    RubixCube_SetMsg(s, s->m_cubesText, true);
    break;

  case ID_CUBES_DUMP_09 :
    CmdConvert(tmp, 256, s->m_hist, true);
    RubixCube_SetMsg(s, tmp, true);
    break;
  }
}
void RubixCube_OnPaint(RubixCube* s, img_t* im, const IRECT* pclip, IRECT rc, const font_t* fo, int m_init)
{
  const char* msg = "Left click : rotate the cube\n"
      "Right-click : the faces change\n"
      "Double Click : resolution";

  IRECT clientRect, wrkRect;
  clientRect = rc;
  wrkRect = rc;

  if (m_init) {
    CCubes_SetScreen(s->m_cubes, clientRect.r, clientRect.b);
    CCubes_Comput(s->m_cubes, s->m_move);
    CCubes_Comput2D(s->m_cubes, 15, iPOINT(5, 5));
  }

  imdraw_rect(im, pclip, wrkRect, _RGB(225, 225, 225), 0, 0, 0);
  imdraw_text(im, pclip, rc, msg, -1, fo, Color_Black, 0, DT_RIGHT);
  CCubes_Draw(s->m_cubes, im, pclip);
  // Dump();
  RubixCube_SetMsg6(s);
  RubixCube_WndMsgDump(s);
}

BOOL RubixCube_OnToolTipNotify(RubixCube* s, UINT id, NMHDR* pNMH, LRESULT* pResult)
{
  TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMH;
  CSide* side;

  if ((pTTT->uFlags & TTF_IDISHWND) == 0 &&
      (side = CCubes_GetSide(s->m_cubes, pNMH->idFrom - 1000))) {
    pTTT->hinst = NULL;
    //pTTT->lpszText = side->GetMsg(s->m_modeMsg, s->m_cubes);
    return TRUE;
  }

  return FALSE;
}

void RubixCube_OnTooltipsMode(RubixCube* s)
{
  s->m_modeMsg = (s->m_modeMsg + 1) % MODE_TOOLTIP;
}

void RubixCube_ObserverOrient(RubixCube* s)
{
  BYTE or = BYTE(M33_Pack(s->m_mObs));
  int i;

  for (i = 0; i < 6; i++) {
    if (s->m_orients[i] == or) {
      break;
    }
  }

  s->m_observerBtn = (i < 6) ? i : -1;
}

void RubixCube_Update(RubixCube* s)
{
  if (s->m_orEn) {
    s->m_cSides = CCubes_SidesColors(s->m_cubes, false);
  }
  else if (s->m_cSides == CCubes_SidesColors(s->m_cubes, false)) {
    s->m_orEn = true;
  }

  RubixCube_ObserverOrient(s);
}
void RubixCube_ColorUpdate(RubixCube* s)
{
  CCubes_Comput2D(s->m_cubes);
  CCubes_Comput(s->m_cubes, 0);
  RubixCube_Update(s);
}

// Menu commands
void RubixCube_OnCubeInit(RubixCube* s)
{
  if ((s->m_move | s->m_cmd) == 0) {
    if (MessageBox(NULL, "Would you Reset the Rubik's Cube ?",
        NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) {

      M33_Cpy(s->m_mObs, mo);
      M33_Set(s->m_mMov, 1.f);
      CCubes_CmdAddRevers(s->m_cubes, 4, s->m_hist, 256, -1);
      CCubes_Init(s->m_cubes);
      CCubes_Comput(s->m_cubes, 0);
      CCubes_Comput2D(s->m_cubes, 15, iPOINT(5, 5));
      s->m_comments[0] = 0;
      RubixCube_ColorUpdate(s);
    }
  }
}

void RubixCube_InitObs(RubixCube* s, BYTE or)
{
  float mmo[9] = {0};
  M33_Cpy(mmo, mo);
  M33_M33(s->m_mObs, or , mmo, 1.f);
  CCubes_Comput(s->m_cubes, 0);
}

// Order of rotation or deformation
BOOL RubixCube_CubeRotate(RubixCube* s, t_Command cmd, bool save, DWORD curs)
{
  if ((s->m_move | s->m_cmd) == 0 && cmd != 0) {

    if (save) {
      CCubes_CmdAddRevers(s->m_cubes, cmd, s->m_hist, curs, -1);
    }

    // reduced orders for multiple rotation
    s->m_cmd = cmd, cmd = CmdTranslate(s->m_cmd);

    if ((s->m_move = CmdCubeSet(cmd)) != 0) {
      s->m_iCount = CmdCubeCount(cmd) * 50;
      //m_timer = SetTimer(IDD_TIMER, 5, NULL);
    }

    return TRUE;

  }
  else {
    return FALSE;
  }
}

void RubixCube_CubeOrigine(RubixCube* s)
{
  if ((s->m_move | s->m_cmd) == 0) {
    float tmp[9] = {0};
    t_Matrix or = CCubes_Orientation(s->m_cubes);
    M33_M33(tmp, or , 1.f);
    RubixCube_CubeRotate(s, CmdGetRotation(tmp, true), true, 0);
  }
}

void RubixCube_OnCubeOrigin(RubixCube* s)
{
  RubixCube_InitObs(s, 1);
  RubixCube_Update(s);
  RubixCube_CubeOrigine(s);
}

void RubixCube_OnRButtonDown(RubixCube* s, UINT nFlags, IPOINT point)
{
  s->m_cubeNo = CCubes_GetCubeNo(s->m_cubes, point);
  s->m_tchPos = point;
}

void RubixCube_OnRButtonUp(RubixCube* s, UINT nFlags, IPOINT point)
{
  if (s->m_cubeNo < 27 && (s->m_move | s->m_cmd) == 0) {
    IPOINT p;
    PTSUB(p, point, s->m_tchPos);

    RubixCube_CubeRotate(s, CCubes_GetMove(s->m_cubes, p.x, p.y, s->m_cubeNo), true, 0);
  }
}

void RubixCube_OnLButtonDown(RubixCube* s, UINT nFlags, IPOINT point)
{
  s->m_posML = point;
  s->m_flgML = TRUE;
}

void RubixCube_OnLButtonUp(RubixCube* s, UINT nFlags, IPOINT point)
{
  s->m_flgML = FALSE;
  RubixCube_Update(s);
}

void RubixCube_OnMouseMove(RubixCube* s, UINT nFlags, IPOINT point)
{
  if (s->m_flgML) {
    IPOINT mv;
    PTSUB(mv, s->m_posML, point);
    M33_PosRotate(s->m_mObs, mv.y, mv.x);
    CCubes_Comput(s->m_cubes, s->m_move);
  }

  s->m_posML = point;
}

void RubixCube_SetSelect(RubixCube* s, DWORD curs)
{
#if 0
  int start = WORD(curs), end = start + BYTE(curs >> 16), last;
  t_Command cmd = t_Command(curs >> 24);
  char text[256];
  char tmp[256];

  s->m_edit.GetWindowText(text);
  t_Command sc = CmdExtractOne(text, start, last);

  if (curs == 0 || end != last || sc != cmd) {
    s->m_edit.SetSel(0), s->m_curPtr = 0;
  }
  else {
    s->m_edit.SetSel(start, end), s->m_curPtr = last;
  }

#endif
}

void RubixCube_OnUndo(RubixCube* s)
{
  if ((s->m_move | s->m_cmd) == 0) {
    DWORD curs = DWORD(-1);
    t_Command cmd = CCubes_CmdExtract(s->m_cubes, s->m_hist, &curs);

    if (cmd != 0) {
      RubixCube_CubeRotate(s, cmd, false, 0);
    }
    else {
      CCubes_Comput2D(s->m_cubes);
      CCubes_Comput(s->m_cubes, 0);
      RubixCube_ColorUpdate(s);
    }

    if (curs != DWORD(-1)) {
      RubixCube_SetSelect(s, curs);
    }
  }
}

BOOL RubixCube_ExeCmd(RubixCube* s, bool frst)
{
#if 0

  if (frst || s->m_continue) {
    int len, last, start, end;
    DWORD curs = 0;
    t_Command cmd;
    char* text = s->m_edit;
    len = strlen(text);

    // s->old selection for undo function
    s->m_edit.GetSel(start, end);

    if (start < end
        && (cmd = CmdExtractOne(text, start, last)) != 0
        && last == end)
      // si la selection correspond a une commande...
    {
      curs = start | ((end - start) << 16) | cmd << 24;
    }

    // recherche la prochaine commande
    while ((start = s->m_curPtr) < len) {
      if ((cmd = CmdExtractOne(text, start, s->m_curPtr)) != 0) {

        // trace la commande en cours d'execution (selection)
        s->m_edit.SetSel(start, s->m_curPtr);

        // lance l'execution de la commande
        s->RubixCube_CubeRotate(s, cmd, true, curs);
        return TRUE;
      }
    }
  }
  else {
    return FALSE;  // mode pas a pas
  }

  // fin de la liste des commandes
  s->m_edit.SetSel(0);
  s->m_curPtr = 0;
#endif
  return FALSE;
}

void RubixCube_OnTimer(RubixCube* s, UINT nIDEvent)
{
  if (s->m_move != 0) {
    if (s->m_iCount > 0) {
      s->m_iCount--;

      if (s->m_move & 0x7F00) {
        M33_RotateCmd(s->m_mMov, s->m_move, 20);
      }
      else {
        M33_RotateCmd(s->m_mObs, s->m_move, 20);
      }

      CCubes_Comput(s->m_cubes, s->m_move);
    }
    else {
      t_Command cmd;

      CCubes_Fixe(s->m_cubes, s->m_move, true), s->m_move = 0;

      if (cmd = CmdTranslate(s->m_cmd)) {

        // Following the orders of multiple rotation
        s->m_move = CmdCubeSet(cmd);
        s->m_iCount = CmdCubeCount(cmd) * 50;
      }
      else if (RubixCube_ExeCmd(s, false) == FALSE) {
        // complete the request
        //KillTimer(nIDEvent);
      }

      RubixCube_ColorUpdate(s);
    }
  }

}

// others subroutines
void RubixCube_CubeMove(RubixCube* s, int pos)
{
  s->m_yObs = pos;
  CCubes_Comput(s->m_cubes, s->m_move);
}

void RubixCube_CubeOrientation(RubixCube* s, const float* mat)
{
  float om[9] = {0};
  float tmp[9] = {0};
  M33_M33(om, CCubes_Orientation(s->m_cubes), 1.f);
  M33_Invert(om);
  M33_M33(tmp, mat, om);
  RubixCube_CubeRotate(s, CmdGetRotation(tmp, false), true, 0);
}

void RubixCube_CubeRnd(RubixCube* s, bool pos)
{
  char cmdStr[256];
  char tmp[256];
  CmdRandom(cmdStr, 256, pos, 40, -1);

  s->m_comments[0] = 0;
  s->m_hist[0] = 0;
  _snprintf(s->m_cubesText, 256, "Mixing sequence :\n");
  CmdConvert(tmp, 256, cmdStr, true);
  CCubes_CmdApply(s->m_cubes, cmdStr);
  RubixCube_ColorUpdate(s);
}

// Read write

void RubixCube_OnFileOpen(RubixCube* s)
{
#if 0
  // Open file to reading
  CFileDialogSa dlg(TRUE, szFilter, "RubixCube");

  if (dlg.DoModal() == IDOK) {
    Title(), s->m_comments[0] = 0;

    if (dlg.GetFileExt().CompareNoCase("tif") == 0) {
      CDiagExtr(dlg.GetPathName(), s).DoModal();
    }
    else {
      if (s->m_cubes.Read(dlg.GetPathName(), &s->m_comments)) {
      }
    }

    RubixCube_ColorUpdate(s);
  }

#endif
}

void RubixCube_OnFileSaveAs(RubixCube* s)
{
  // Open file to writing
#if 0
  if (1) {
    // Creating a TIFF file containing the 6 images of the faces of the Rubik's Cube
    BYTE sides[6] = {5, 0, 3, 4, 1, 2}; // order of faces in a file
    float mv[9] = {0};
    CCubes cubes(1.f, NULL, NULL, &mv);
    CExtract extract;

    cubes = s->m_cubes;
    // provides a 180 degrees rotation of the bottom face
    mv.Rotate90Req(0x0A);

    dc.CreateCompatibleDC(NULL);
    bm.CreateCompatibleBitmap(&pdc, 99, 99);
    CBitmap* pBmOld = dc.SelectObject(&bm);

    // draws a black background between the facets
    dc.FillSolidRect(0, 0, 99, 99, COLOR(0));

    // simulates the sequence of rotations showing the six faces
    for (int i = 0; i < 6; i++) {
      // positions the cube according to the matrix
      cubes.Fixe();
      // draws and saves the bottom face
      cubes.DrawSide(&dc, 5, 99);
      extract.AddImage(&bm);
      // rotated for the new face
      mv.Rotate90Req((i & 1) ? 3 : 7);
    }

    // saves the TIFF file with s six images
    extract.FileWrite("asdf");
  }
  else {
    // saves others file type (*.rbx, *.txt, *.sgm, *.col)
    CCubes_Write(s->m_cubes, "asdfasdf.cub", s->m_comments);
    Title(0);
  }

#endif
}

BOOL RubixCube_GetCubeText(RubixCube* s, const char* msg)
{
  // This function is also called when you press OK on CDiagExtract
  BYTE facets[54];
  CCubes cubes;
  int er;
  char tmp[256];
  CCubes_Set(&cubes, 1.f, NULL, NULL, NULL);

  STRcpy(s->m_cubesText, 256, msg, -1);

  if ((er = SetSideText(msg, facets, "ORGBWYUDLRFB")) != 0
      || (er = CCubes_Create(&cubes, facets, false)) != 0) {
    GetErrorMessage(tmp, 256, er, NULL);
    STRcat(tmp, 256, "Impossible construction !", -1);
    MessageBox(NULL, NULL, tmp, 0);
    return FALSE;
    // test if all sides are legal (allowing a cube ordered)
  }
  else if ((er = CCubes_ColorsCheck(&cubes)) != 0) {
    GetErrorMessage(tmp, 256, er, NULL);
    STRcat(tmp, 256, "The Cube can be ordered ! Would you give up ?", -1);

    if (MessageBox(NULL, tmp, NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) {
      return FALSE;
    }
  }

  // update of the Rubik's Cube displayed
  *s->m_cubes = cubes;
  RubixCube_ColorUpdate(s);
  s->m_hist[0] = 0;
  return TRUE;
}
//



// Test, dump
void RubixCube_OnCubesDump(RubixCube* s, UINT id)
{
  if (id == s->m_wndMsgId) {
    s->m_wndMsgId = 0;
    //s->ShowWindow(SW_HIDE);
  }
  else {
    s->m_wndMsgId = id;
    //s->ShowWindow(SW_SHOW);
    RubixCube_WndMsgDump(s);
  }
}

void RubixCube_OnUpdateCubesDump(RubixCube* s, void* pCmdUI)
{
  if (s->m_wndMsgId != 0) {
    s->m_wndMsgId = 0;
  }
}

void RubixCube_OnShowplan(RubixCube* s)
{
  RubixCube_SetMsg6(s, true);
}

// Task and associated mechanism for solving the Rubik's Cube
void RubixCube_OnOptiSolver(RubixCube* s)
{
  //s->OptiSolverCmd();
  if (s) {
    CCubes_GetText(s->m_cubes, s->m_command, 256);
    s->m_orient = CCubes_Orientation(s->m_cubes);

    // Start the solving task
    s->m_help = false;
    //((CButton*)GetDlgItem(IDC_OPTISOLVER))->SetWindowText("Stop");
  }
  else {
    // Stop scan
  }
}

void RubixCube_OnFileAcquire(RubixCube* s)
{
#if 0

  if (!m_diagExtr.IsBegin()) {
    m_diagExtr.DeleteImages();
  }

  m_diagExtr.ShowWindow(SW_SHOW);
  m_diagExtr.Acquire(6);
#endif
}

void RubixCube_DrawRC(RubixCube* s, img_t* im, const IRECT* pclip, IRECT rect, const char* msg)
{
  BYTE facets[54];
  SetSideText(msg, facets, "ORGBWYUDLRFB");
  DrawRCSides(im, pclip, rect, facets);
}
//
// RubixCube message handlers

void RubixCube_OnPaint(RubixCube* s, img_t* im, const IRECT* pclip, IRECT rc, const font_t* fo)
{
  IRECT r = rc;

  imdraw_rect(im, pclip, r, _RGB(255, 255, 230), 0, 0, 0);

  if (strlen(s->m_msg) >= 70 && s->m_msg[1] == ':') {
    // trace le deroule a plat
    RubixCube_DrawRC(s, im, pclip, r, s->m_msg);
  }
  else if (0 != STR_cmp(s->m_msg, -1, "$6", -1)) {
    // imprime le texte
    r.l = 3, r.t = 3;
    imdraw_DrawText(im, pclip, s->m_msg, r, 0, fo, Color_Black);
  }
  else {
    CCubes_Draw2D(s->m_cubes, im, pclip);
  }
}

COLOR* RubixCube_InitCustColors(RubixCube* s, void* pData)
{
  UINT size = 0;

  //::AfxGetApp()->GetProfileBinary("Settings", "CustColors", &pData, &size);

  for (UINT i = 0; i < 16; i++) {
    custColors[i] = _RGB(255, 255, 255);
  }

  if (pData) {
    if (size > sizeof(custColors)) {
      size = sizeof(custColors);
    }

    size /= 4;

    for (UINT i = 0; i < size; i++) {
      custColors[i] = PDWORD(pData)[i];
    }
  }

  return custColors;
}

//
// RubixCube message handlers

BOOL RubixCube_OnNotify(RubixCube* s, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#if 0

  if (s) {
    int pos = 300000 / (1000 - m_slidDistance.GetPos());
    s->CubeMove(pos);
  }

#endif

  return 0;
}

void RubixCube_OnShowWindow(RubixCube* s, BOOL bShow, UINT nStatus)
{
  if (s && bShow) {
    s->m_cSides = CCubes_SidesColors(s->m_cubes, false);
    s->m_orEn = true;

    RubixCube_ObserverOrient(s);

    //m_slidDistance.SetRange(80, 960);
    //m_slidDistance.SetPos(1000 - 300000 / s->s->m_yObs);
  }
}

void RubixCube_SetSidesColors(RubixCube* s, int sideId, int colorId)
{
  if (s->m_ref < 6) {
    t_GROUP nSides  = GROUP(s->m_ref * 9 + 4, BYTE(sideId * 9 + 4));
    t_GROUP colors = GROUP(s->m_color, BYTE(colorId));
    // Calcule l'orientation correspondante du Rubik's Cube
    t_Matrix or = GetCubeOrientation(RefColors(nSides), colors, 0);
    // t_Matrix or = CMatrix::ComputOrient(RefColors(nSides), colors, 0);

    if ( or == 0) {
      MessageBox(NULL, "Choisir deux faces ou deux couleurs non\nopposées pour orienter le Rubik's Cube", NULL, 0);
    }
    else {
      // Calcule les couleurs des facettes centrales manquantes
      // en positionnant un Rubik's Cube avec cette orientation
      float mn[9] = {0};
      float tmp[9] = {0};
      M33_M33(mn, or , 1.f);
      CCubes cubes;
      CCubes_Set(&cubes, 1.f, NULL, NULL, mn);

      // oriente le Rubik's Cube en fonction de la demande
      CCubes_Fixe(&cubes, 0, true);
      // extrait les couleurs des faces centrales
      s->m_cSides = CCubes_SidesColors(&cubes, false);
      s->m_ref = s->m_color = 0xFF;

      // oriente le Cube affiche
      s->m_orEn = false;
      M33_M33(tmp, or , 1.f);
      RubixCube_CubeOrientation(s, tmp);
    }
  }
  else {

    // selection premiere couleur
    s->m_ref = BYTE(sideId), s->m_color = BYTE(colorId);
    s->m_cSides = GROUP_EMPTY;
  }
}
void RubixCube_OnDrawItem(RubixCube* s, img_t* im, const IRECT* pclip, IRECT rcItem, int nIDCtl, int itemState, const char* msg, const font_t* fo)
{
  if (nIDCtl >= IDC_CHKCOLOR0 && nIDCtl <= IDC_BTN_B) {
    // ensure the drawings and management of color buttons
    UINT state = DFCS_BUTTONPUSH;
    //IRECT r = rcItem;
    BYTE color = 0xFF;

    TRACE("RubixCube_OnDrawItem id=%d itemState=%08x\n", nIDCtl, itemState);

    if (nIDCtl <= IDC_CHKCOLOR5) {

      // color buttons
      if (itemState & ODS_SELECTED) {

        state |= DFCS_PUSHED;

        if (s->m_btnColorId == nIDCtl) {

          s->m_btnColorId = 0;
        }
        else if (s->m_btnColorId > 0) {

          // swap colors
          ColorsExchange(BYTE(s->m_btnColorId - IDC_CHKCOLOR0), BYTE(nIDCtl - IDC_CHKCOLOR0));

          // display the new colors
          s->m_btnColorId = 0;
        }
        else if (s->m_btnSideId > 0) {

          // change the Rubik's Cube orientation
          int sideId = s->m_btnSideId - IDC_BTN_U;
          int colorId = nIDCtl - IDC_CHKCOLOR0;

          s->m_btnSideId = 0;
          s->m_btnColorId = nIDCtl;

          RubixCube_SetSidesColors(s, sideId, colorId);

          // display the new position
          RubixCube_ColorUpdate(s);
        }
        else {

          s->m_btnColorId = nIDCtl;
        }
      }
      else if (s->m_btnColorId == nIDCtl) {

        // s->old selected color
        state |= DFCS_PUSHED;
      }

      color = BYTE((nIDCtl - IDC_CHKCOLOR0) % 6);
    }
    else {

      if (itemState & ODS_SELECTED) {
        state |= DFCS_PUSHED;

        if (s->m_btnSideId == nIDCtl) {
          s->m_btnSideId = 0;
        }
        else if (s->m_btnSideId > 0) {
          // erase the s->old button
          // display the new
          s->m_btnSideId = nIDCtl;
        }
        else if (s->m_btnColorId > 0) {

          int colorId = s->m_btnColorId - IDC_CHKCOLOR0;
          int sideId  = nIDCtl - IDC_BTN_U;

          s->m_btnSideId = nIDCtl;
          s->m_btnColorId = 0;

          RubixCube_SetSidesColors(s, sideId, colorId);
        }
        else {
          s->m_btnSideId = nIDCtl;
        }
      }
      else if (s->m_btnSideId == nIDCtl) {

        // s->old selected colors
        state |= DFCS_PUSHED;
      }

      BYTE noSide = BYTE(nIDCtl - IDC_BTN_U);

      if (s->m_ref >= 6) {
        color = ExtractSidesColors(noSide, s->m_cSides);
      }
      else if (s->m_ref == noSide) {
        color = s->m_color;
      }
    }

#if 0
    // draw the button withs his text and his color
    DrawFrameControl(pDis->hDC, &r, DFC_BUTTON, state);

    r.DeflateRect(3, 3);

    if ((IDC_BTN_U + (s->m_ref ^ 1)) == nIDCtl
        || (IDC_CHKCOLOR0 + s->m_color) == nIDCtl
        || (IDC_CHKCOLOR0 + (s->m_color ^ 1)) == nIDCtl) {
      ::FillRect(pDis->hDC, &r, (HBRUSH)GetStockObject(GRAY_BRUSH));
    }
    else if (color < 6) {
      ::FillRect(pDis->hDC, &r, (HBRUSH)GetBrush(color)->m_hObject);
    }

    COLOR save = ::SetTextColor(pDis->hDC, RGBTextColor(color));
    ::SetBkMode(pDis->hDC, TRANSPARENT);
    ::GetWindowText(pDis->hwndItem, msg, 40);
    ::DrawText(pDis->hDC, msg, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    ::SetTextColor(pDis->hDC, save);
#endif
  }
}

void RubixCube_OnDoubleclickedChkcolor(RubixCube* s, UINT id)
{
#if 0
  // modification of one of the color facet
  BYTE color = BYTE(id - IDC_CHKCOLOR0);
  CColorDialog dlg(RGBDrawColor(color));

  dlg.m_cc.lpCustColors = InitCustColors();

  if (dlg.DoModal() == IDOK) {
    SetBrush(color, dlg.GetColor());
    // save of new colors in the registry
    SaveRegColors();
    SaveCustColors();
  }

  s->m_btnColorId = 0;
  GetDlgItem(id)->Invalidate();
  s->RubixCube_ColorUpdate(s);
#endif
}

void RubixCube_OnClickedButton(RubixCube* s, UINT id)
{
  RubixCube_ColorUpdate(s);
}

void RubixCube_OnObserverBtn(RubixCube* s, UINT id)
{
  TRACE("RubixCube_OnObserverBtn %d or=%d\n", s->m_observerBtn, M33_Pack(s->m_mObs));

  if (s->m_observerBtn >= 0 && s->m_observerBtn < 6) {
    RubixCube_InitObs(s, s->m_orients[s->m_observerBtn]);
  }
}

void RubixCube_OnDefaultcolor(RubixCube* s)
{
  SetDefaultColors();
  SaveRegColors();

  s->m_btnColorId = 0;

  for (int id = IDC_CHKCOLOR0; id <= IDC_CHKCOLOR5; id++) {
    //GetDlgItem(id)->Invalidate();
  }

  RubixCube_ColorUpdate(s);
}

void RubixCube_OnBtnOrigine(RubixCube* s)
{
  RubixCube_CubeOrigine(s);
  RubixCube_Update(s);
}

void RubixCube_SetHelp(RubixCube* s)
{
  if (s->m_help = (s->m_help == false)) {
    static const char* help = TEXT(
        "/ The commands shown here changes the Rubik's Cube.\r\n"
        "/\r\n"
        "/   L, l : left = turn left face,  R, r : right = turn right face,\r\n"
        "/   F, f : front = front face,  B, b : back = back face,\r\n"
        "/   U, u : up = up face,  D, d : down = down face,\r\n"
        "/\r\n"
        "/   Uppercase letter CW rotation, lowercase letter CCW rotation.\r\n"
        "/   The char '2' last for 180°, the char (\') last reverse the rotation.\r\n"
        "/   Begin with :  * : whole cube,  > : the slice of the middle.\r\n"
        "/\r\n"
        "/ Commands can be edited in s window instead of s text.\r\n"
        "/ The char '/' starts a comment until the end of the line.\r\n"
        "/\r\n"
        "U B2 F2 D' R L U' R L    / Example : press \'Run\' or \'Step\' to start.\r\n"
        "D B' F' U' R L B F \r\n"
        "/\r\n"
        "/ The \'Solve\' button provides the commands that unscrambles the Cube.\r\n"
        "/ The \'Centers\', provides the command that unscrambles center of faces.\r\n"
        "/ The \'Reverse\', translates commands for regenerating the initial state.\r\n"
        "/ The \'Rotate\', Compensates the commands after a rotation of the Cube.\r\n"
        );

    //s->m_edit.GetWindowText(m_save);
    //s->m_edit.SetWindowText(help);
  }
  else {
    //s->m_edit.SetWindowText(m_save);
  }

}

BOOL RubixCube_OnInitDialog(RubixCube* s)
{
  // decoding of the buttons
  s->m_cmds[IDC_CMD_R - IDC_CMD_L] = Move_cR;  // right
  s->m_cmds[IDC_CMD_RR - IDC_CMD_L] = Move_cRi;

  s->m_cmds[IDC_CMD_L - IDC_CMD_L] = Move_cL;  // left
  s->m_cmds[IDC_CMD_LR - IDC_CMD_L] = Move_cLi;

  s->m_cmds[IDC_CMD_B - IDC_CMD_L] = Move_cB;  // back
  s->m_cmds[IDC_CMD_BR - IDC_CMD_L] = Move_cBi;

  s->m_cmds[IDC_CMD_F - IDC_CMD_L] = Move_cF;  // front
  s->m_cmds[IDC_CMD_FR - IDC_CMD_L] = Move_cFi;

  s->m_cmds[IDC_CMD_U - IDC_CMD_L] = Move_cU;  // up
  s->m_cmds[IDC_CMD_UR - IDC_CMD_L] = Move_cUi;

  s->m_cmds[IDC_CMD_D - IDC_CMD_L] = Move_cD;  // down
  s->m_cmds[IDC_CMD_DR - IDC_CMD_L] = Move_cDi;

  // help text
  RubixCube_SetHelp(s);

  //((CButton*)GetDlgItem(IDC_OPTISOLVER))->GetWindowText(m_textBtnOPTISOLVER);
  return TRUE;  // return TRUE unless you set the focus to a control
}               // EXCEPTION: OCX Property Pages should return FALSE


void RubixCube_IsSelected(RubixCube* s)
{
#if 0
  int nStartChr = -1, nEndChr = -1;

  s->m_edit.GetSel(nStartChr, nEndChr);

  if (nStartChr == nEndChr) {
    s->m_curPtr = 0;  // unselected, reset
  }
  else if (s->m_curPtr != nEndChr) {
    s->m_curPtr = nStartChr;  // new selection
  }

#endif
}

void RubixCube_ReversCmd(RubixCube* s)
{
  char cmd[256];
  char* text = s->m_edit;
  char tmp[256];
  char tmp2[256];

  s->m_curPtr = 0;

  CmdExtract(cmd, 256, text, 0, NULL);
  CmdReverse(cmd);
  InvalidCommands(text, tmp, 256);
  CmdConvert(tmp2, 256, cmd, false);
  STRcat2(text, 256, tmp, -1, tmp2, -1);

  //s->m_edit.SetWindowText(text);
  // to be sure to display the last line with the new commands string
  int len = strlen(text);
  //s->m_edit.SetSel(len, len);
}

void RubixCube_RotateCmd(RubixCube* s)
{
  t_Matrix orient = CCubes_Orientation(s->m_cubes);
  char cmd[256];
  char tmp[256];
  char tmp2[256];
  char* text = s->m_edit;

  s->m_curPtr = 0;
  // Determines the change in the orientation of the Cube
  t_Matrix or = MatrixProduct(MatrixInvert(orient), s->m_orient);

  text[0] = 0;

  if ( or > 1) {
    CmdExtract(cmd, 256, text, 0, NULL);
    // Modifies the string of commands accordingly
    CmdRotate(cmd, 256, cmd, or);
    InvalidCommands(text, tmp, 256);
    CmdConvert(tmp2, 256, cmd, false);
    STRcat2(text, 256, tmp, -1, tmp2, -1);

    s->m_orient = orient;
  }
  else {
    STRcat(text, 256, "/ No rotation of the Cube\r\n", -1);
  }

  // to be sure to display the last line with the new commands string
}

inline void RubixCube_SolverAbort(RubixCube* s)
{
}

void RubixCube_OptiSolverCmd(RubixCube* s)
{
  RubixCube_SolverAbort(s);
  RubixCube_OnOptiSolver(s);
}

// RubixCube message handlers

void RubixCube_OnCmd(RubixCube* s, UINT id)
{
  TRACE("RubixCube_OnCmd %d\n", id);

  if (s) {
    t_Command cmd = s->m_cmds[id - IDC_CMD_L];

    if (s->m_cmdAll) {
      cmd |= 0x40;
    }
    else if (s->m_cmdM) {
      cmd |= 0x20;
    }

    RubixCube_CubeRotate(s, cmd, true, 0);
  }

  s->m_cmdAll = s->m_cmdM = FALSE;
}

void RubixCube_OnCmdAll(RubixCube* s)
{
  if (s->m_cmdAll && s->m_cmdM) {
    s->m_cmdM = FALSE;
  }
}

void RubixCube_OnRun(RubixCube* s)
{
  RubixCube_SolverAbort(s);
  RubixCube_IsSelected(s);
  s->m_continue = TRUE;
}

void RubixCube_OnStep(RubixCube* s)
{
  RubixCube_SolverAbort(s);
  RubixCube_IsSelected(s);
  s->m_continue = FALSE;

}

void RubixCube_OnCmdM(RubixCube* s)
{
  if (s->m_cmdAll && s->m_cmdM) {
    s->m_cmdAll = FALSE;
  }
}

void RubixCube_OnCmdhelp(RubixCube* s)
{
  RubixCube_SetHelp(s);
}

void RubixCube_OnCmdRev(RubixCube* s)
{
  RubixCube_SolverAbort(s);
  RubixCube_ReversCmd(s);
}

void RubixCube_OnCmdRot(RubixCube* s)
{
  RubixCube_SolverAbort(s);
  RubixCube_RotateCmd(s);
}

void RubixCube_OnSetfocusEdtCmd(RubixCube* s)
{
  if (s->m_help) {
    // Clicking on the text of the help, which clears
    s->m_help = false;
    //s->m_edit.SetWindowText(m_save);
  }
}

void RubixCube_OnCmdInit(RubixCube* s)
{
  RubixCube_OnCubeInit(s);
  s->m_orient = 0;
}

void RubixCube_OnCmdOrigin(RubixCube* s)
{
  if (s) {
    RubixCube_OnCubeOrigin(s);
    s->m_orient = 0;
  }
}


void RubixCube_OnCenters(RubixCube* s)
{
  RubixCube_SolverAbort(s);

  BYTE state = CCubes_GetState(s->m_cubes);
  char* text = s->m_edit;
  char text1[256] = {0};
  char text2[256] = {0};
  char tmp[256] = {0};

  // s->m_help = false;
  if (state == 0) {
    STRcpy(text2, 256, "/ Centers already ordered\r\n", -1);
  }
  else if (state == 2) {
    char cmd[256];
    DWORD centers = CCubes_CenterSidesRotation(s->m_cubes);
    int cc = ResolveCenters(centers, cmd, 256);

    if (cc == 0) {
      CmdConvert(text1, 256, cmd, true);
    }
    else {
      GetErrorMessage(tmp, 256, cc, "\r\n");
      _snprintf(text1, 256, "/ Error : %s", tmp);
    }

    GetMsgCenter(tmp, 256, centers);
    _snprintf(text2, 256, "/ Centers : %s\r\n", tmp);
  }
  else if (state == 3) {
    STRcpy(text2, 256, "/ Rubik's Cube must be ordered before launching s operation\r\n", -1);
  }

  // s->m_edit.GetWindowText(text);
  InvalidCommands(text, tmp, 256);
  STRcat(text, 256, tmp, -1);
  STRcat(text, 256, text2, -1);
  STRcat(text, 256, text1, -1);
  s->m_help = false;
  s->m_curPtr = 0;
}

void RubixCube_OnClose(RubixCube* s)
{
  TRACE("RubixCube_OnClose\n");
  RubixCube_SolverAbort(s);
}

void RubixCube_OnDestroy(RubixCube* s)
{
  TRACE("RubixCube_OnDestroy\n");
  RubixCube_SolverAbort(s);
}

