// RubiksCubeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "RubiksCubeDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRubiksCubeDlg dialog

CRubiksCubeDlg::CRubiksCubeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRubiksCubeDlg::IDD, pParent)
, m_Yellow(RGB(255,255,0),1)
, m_Blue(RGB(0,0,255),2)
, m_Red(RGB(255,0,0),3)
, m_Orange(RGB(255,128,0),4)
, m_Green(RGB(0,121,0),5)
, m_White(RGB(255,255,255),6)
, m_CurIndex(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRubiksCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_CUBE, m_Cube);
   DDX_Control(pDX, IDC_RED, m_Red);
   DDX_Control(pDX, IDC_BLUE, m_Blue);
   DDX_Control(pDX, IDC_GREEN, m_Green);
   DDX_Control(pDX, IDC_WHITE, m_White);
   DDX_Control(pDX, IDC_YELLOW, m_Yellow);
   DDX_Control(pDX, IDC_ORANGE, m_Orange);
   DDX_Control(pDX, IDC_PROGRESS, m_Progress);
   DDX_Control(pDX, IDC_MOVELIST, m_MoveList);
   DDX_Control(pDX, IDC_MOVETITLE, m_MoveTitle);
   DDX_Control(pDX, IDC_SPEED, m_Speed);
   DDX_Control(pDX, IDC_SPEEDTITLE, m_Animate);
   DDX_Control(pDX, IDC_3d, m_3DCheck);
}

BEGIN_MESSAGE_MAP(CRubiksCubeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_SOLVE, OnBnClickedSolve)
   ON_BN_CLICKED(IDC_EXIT, OnBnClickedExit)
   ON_BN_CLICKED(IDC_CLEAR, OnBnClickedClear)
   ON_BN_CLICKED(IDC_ABOUT, OnBnClickedAbout)
   ON_BN_CLICKED(IDC_SHUFFLE, OnBnClickedShuffle)
   ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
   ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
   ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
   ON_LBN_SELCHANGE(IDC_MOVELIST, OnLbnSelchangeMovelist)
   ON_WM_HSCROLL()
   ON_BN_CLICKED(IDC_SPEEDTITLE, OnBnClickedSpeedtitle)
   ON_MESSAGE(WM_COLORSELECTED,OnColorSelected)
   ON_BN_CLICKED(IDC_3d, OnBnClicked3d)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CRubiksCubeDlg)
	EASYSIZE(IDC_CUBE,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_RED,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_GREEN,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BLUE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_YELLOW,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_WHITE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_ORANGE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_SOLUTION_STATIC,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_MOVETITLE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_MOVELIST,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_PROGRESS,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_RESET,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_SHUFFLE,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_CLEAR,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_SOLVE,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_ABOUT,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_EXIT,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_PREV,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_NEXT,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP

// CRubiksCubeDlg message handlers

BOOL CRubiksCubeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	INIT_EASYSIZE;

   CRect Rect(10,10,400,200);
   m_Cube.InitGL();

   // Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

   m_Speed.SetRange(0,180);
   m_Speed.SetPos(30);
   m_Speed.SetPageSize(10);
   m_Speed.SetTicFreq(10);

   SpeedChange();

   m_Animate.SetCheck(BST_CHECKED);
   m_3DCheck.SetCheck(BST_CHECKED);
   OnBnClickedSpeedtitle();
   OnBnClicked3d();
   EnableNextPrev();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRubiksCubeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRubiksCubeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//override to keep Enter and Esc to close dialog
void CRubiksCubeDlg::OnOK()
{
}
void CRubiksCubeDlg::OnCancel()
{
   if (MessageBox("Are you sure you want to exit?","Exit!",MB_YESNO|MB_ICONQUESTION) == IDYES)
   {
      CDialog::OnCancel();
   }
}

//The Exit button was pressed
void CRubiksCubeDlg::OnBnClickedExit()
{
   OnCancel();
}

//bring up the About Dialog
void CRubiksCubeDlg::OnBnClickedAbout()
{
   CAboutDlg   Dlg;
   Dlg.DoModal();
}

//Enable/Disable controls on the Screen
void CRubiksCubeDlg::Enable(BOOL Enable)
{
//   m_Cube.EnableWindow(Enable);
   m_MoveList.EnableWindow(Enable);
   GetDlgItem(IDC_RED)->EnableWindow(Enable);
   GetDlgItem(IDC_BLUE)->EnableWindow(Enable);
   GetDlgItem(IDC_GREEN)->EnableWindow(Enable);
   GetDlgItem(IDC_YELLOW)->EnableWindow(Enable);
   GetDlgItem(IDC_ORANGE)->EnableWindow(Enable);
   GetDlgItem(IDC_WHITE)->EnableWindow(Enable);
   GetDlgItem(IDC_SOLVE)->EnableWindow(Enable);
   GetDlgItem(IDC_CLEAR)->EnableWindow(Enable);
   GetDlgItem(IDC_EXIT)->EnableWindow(Enable);
   GetDlgItem(IDC_SHUFFLE)->EnableWindow(Enable);
   GetDlgItem(IDC_RESET)->EnableWindow(Enable);
   GetDlgItem(IDC_ABOUT)->EnableWindow(Enable);
   if (Enable)
   {
      EnableNextPrev();
   }
   else
   {
      GetDlgItem(IDC_NEXT)->EnableWindow(Enable);
      GetDlgItem(IDC_PREV)->EnableWindow(Enable);
   }
   m_MoveList.SetFocus();
}

void CRubiksCubeDlg::EnableNextPrev()
{
   GetDlgItem(IDC_PREV)->EnableWindow(m_CurIndex > 0);
   GetDlgItem(IDC_NEXT)->EnableWindow(m_CurIndex < m_MoveList.GetCount()-1);
}

//override to get key presses
BOOL CRubiksCubeDlg::PreTranslateMessage(MSG* pMsg)
{
   return CDialog::PreTranslateMessage(pMsg);
}


void CRubiksCubeDlg::ClearMoves()
{
   m_MoveArray.RemoveAll();
   m_MoveList.ResetContent();
   m_MoveTitle.SetWindowText("Moves");
   m_CurIndex = 0;
   EnableNextPrev();
}

//Shuffle/Scramble the cube
void CRubiksCubeDlg::OnBnClickedShuffle()
{
   ClearMoves();
   Enable(FALSE);
   m_Cube.Shuffle(&m_Progress);
   Enable(TRUE);
}

//reset the cube
void CRubiksCubeDlg::OnBnClickedReset()
{
   Enable(FALSE);
   ClearMoves();
   m_Cube.Reset();
   m_Cube.Invalidate();
   Enable(TRUE);
}

//clear the cube
void CRubiksCubeDlg::OnBnClickedClear()
{
   Enable(FALSE);
   ClearMoves();
   m_Cube.Clear();   
   Enable(TRUE);
}

//solve the cube
void CRubiksCubeDlg::OnBnClickedSolve()
{

   ClearMoves();

   m_Progress.SetRange(0,150);
   m_Progress.SetPos(0);
   Enable(FALSE);
   m_Cube.Animate(false);
   int Ret = m_Cube.Solve(&m_Progress);
   OnBnClickedSpeedtitle();
   if (Ret == 1)
   {
      MessageBox("Cube does not contain proper cubelets.  Check configuration and try again.","Configuration Error",MB_OK|MB_ICONWARNING);
   }
   else if (Ret == 2)
   {
      MessageBox("Cube mis-oriented.  If the configuration you have entered is indeed what you have,\n"
                 "then your cube appears to have been disassembled, and re-assembled incorrctly.\n"
                 "Check configuration and try again.","Unable To Solve Cube",MB_OK|MB_ICONHAND);
   }
   else
   {
      m_MoveArray.Add("");
      m_MoveList.AddString("Your Cube!");
      CString Moves = m_Cube.GetMoves();
      CString Move;
      int Start = 0;
      Move = Moves.Mid(Start,3);
      while (!Move.IsEmpty())
      {
         CString Str = "";
         m_MoveArray.Add(Move);
         switch (Move[0])
         {
         case 'T' :  Str = "Top ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Left";
                                 break;
                     case '-' :  Str += "Right";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         case 'F' :  Str = "Front ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Clockwise";
                                 break;
                     case '-' :  Str += "Counter clockwise";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         case 'L' :  Str = "Left ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Down";
                                 break;
                     case '-' :  Str += "Up";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         case 'R' :  Str = "Right ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Up";
                                 break;
                     case '-' :  Str += "Down";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         case 'B' :  Str = "Back ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Clockwise";
                                 break;
                     case '-' :  Str += "Counter clockwise";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         case 'D' :  Str = "Bottom ";
                     switch (Move[1])
                     {
                     case '+' :  Str += "Right";
                                 break;
                     case '-' :  Str += "Left";
                                 break;
                     case '*' :  Str += "180";
                                 break;
                     }
                     break;
         default   : ASSERT(FALSE);
                     break;
         }
         m_MoveList.AddString(Str);
         Start += 3;
         Move = Moves.Mid(Start,3);
      }
      m_MoveList.SetCurSel(0);
      m_CurIndex = m_MoveList.GetCurSel();
      Move.Format("%ld Moves",Start/3);
      m_MoveTitle.SetWindowText(Move);
      DoMove(m_CurIndex);
      EnableNextPrev();
   }
   Enable(TRUE);
}


//Do a Move out of the move list
void CRubiksCubeDlg::DoMove(int Index,bool Undo)
{
   if (Index < 0)
   {
      return;
   }
   CString Text;
   Text = m_MoveArray.GetAt(Index);
   if (Undo)
   {
      m_Cube.UndoMove(Text);
   }
   else
   {
      m_Cube.DoMove(Text);
   }
}

//go to previous move in the move list
void CRubiksCubeDlg::OnBnClickedPrev()
{
   Enable(FALSE);
   int Index = m_MoveList.GetCurSel();
   if (Index <= 0)
   {
      return;
   }
   DoMove(Index,true);
   m_CurIndex = --Index;
   m_MoveList.SetCurSel(m_CurIndex);
   m_Cube.Invalidate();
   m_Cube.UpdateWindow();
   Enable(TRUE);
}

//go to the next move in the move list
void CRubiksCubeDlg::OnBnClickedNext()
{
   Enable(FALSE);
   int Index = m_MoveList.GetCurSel();
   if (Index >= m_MoveList.GetCount()-1)
   {
      return;
   }

   m_CurIndex = ++Index;
   m_MoveList.SetCurSel(m_CurIndex);
   DoMove(Index);
   m_Cube.Invalidate();
   m_Cube.UpdateWindow();
   Enable(TRUE);
}

//jump to a move in the move list
void CRubiksCubeDlg::OnLbnSelchangeMovelist()
{
   Enable(FALSE);
   int Index = m_MoveList.GetCurSel();
   m_MoveList.SetCurSel(m_CurIndex);
   //if we are going forward
   if (Index > m_CurIndex)
   {
      while (m_CurIndex <= Index)
      {
         OnBnClickedNext();
         m_CurIndex++;
      }
      //start with the next move and do the move
//      for (int i = m_CurIndex+1;i <= Index;i++)
//      {
//         DoMove(i);
//      }
   }
   else if (Index < m_CurIndex) //if we are going back
   {
      while (m_CurIndex >= Index)
      {
         OnBnClickedPrev();
         m_CurIndex--;
      }
      //start with the current move and undo the move
      for (int i = m_CurIndex;i > Index;i--)
      {
         DoMove(i,true);
      }
   }
   m_CurIndex = Index;
   m_Cube.Invalidate();
   m_Cube.UpdateWindow();
   Enable(TRUE);
}

void CRubiksCubeDlg::SpeedChange()
{
   m_Cube.SetSpeed(10 * m_Speed.GetPos());
   CString Text;
   double MovePerSecond = (double)m_Speed.GetPos() / 90;
   if (MovePerSecond == 0)
   {
      MovePerSecond = 0.005;
   }
   MovePerSecond = 1 / MovePerSecond;
   Text.Format("Animate - Speed %.2f Moves/s",MovePerSecond);
   GetDlgItem(IDC_SPEEDTITLE)->SetWindowText(Text);
}

//the speed slide bar has been moved
void CRubiksCubeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   if (pScrollBar->m_hWnd == m_Speed.m_hWnd)
   {
      SpeedChange();
   }

   CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRubiksCubeDlg::OnBnClickedSpeedtitle()
{
   bool Animate = m_Animate.GetCheck() == BST_CHECKED;
   m_Cube.Animate(Animate);
   m_Speed.EnableWindow(Animate);
}

LRESULT CRubiksCubeDlg::OnColorSelected(WPARAM wParam,LPARAM lParam)
{
   HWND hWnd = NULL;
   if (!m_Cube.IsFlat())
   {
      MessageBox("Not Yet implemented for 3D view.\nThis feature is only available in 2D Mode!. Please Try again Later","Error",MB_OK|MB_ICONHAND);
   }
   else
   {
      CColorCtrl *pCtrl = reinterpret_cast<CColorCtrl*>(lParam);
      if (pCtrl)
      {
         m_Cube.SetColor(pCtrl->GetColor());
         hWnd = pCtrl->m_hWnd;
      }
   }
   if (hWnd != m_Red.m_hWnd)
   {
      m_Red.SetSelect(false);
   }
   if (hWnd != m_Blue.m_hWnd)
   {
      m_Blue.SetSelect(false);
   }
   if (hWnd != m_Green.m_hWnd)
   {
      m_Green.SetSelect(false);
   }
   if (hWnd != m_White.m_hWnd)
   {
      m_White.SetSelect(false);
   }
   if (hWnd != m_Yellow.m_hWnd)
   {
      m_Yellow.SetSelect(false);
   }
   if (hWnd != m_Orange.m_hWnd)
   {
      m_Orange.SetSelect(false);
   }

   return 1;
}
void CRubiksCubeDlg::OnBnClicked3d()
{
   bool Checked = m_3DCheck.GetCheck() == BST_UNCHECKED;
   if (!Checked)
   {
      OnColorSelected(NULL,NULL);
      m_Cube.SetColor(0);
   }
   m_Cube.SetFlat(Checked);
   m_Cube.Invalidate();
   m_Cube.UpdateWindow();
}


void CRubiksCubeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetWindow(GW_CHILD) != NULL)
	{
   	UPDATE_EASYSIZE;
	}
}
