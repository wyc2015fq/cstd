// CubeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "CubeWnd.h"

// CCubeWnd

IMPLEMENT_DYNAMIC(CCubeWnd, CWnd)
CCubeWnd::CCubeWnd()
: m_Animate(false)
, m_SelectedSide(NULL)
, m_SelectedColor(0)
, m_hRC(NULL)
, m_RotateX(25.0f)
, m_RotateY(-45.0f)
, m_Flat(true)
, m_Speed(0)
{
   m_Colors[0] = RGB(110,110,110);  //no color
   m_Colors[1] = RGB(255,255,0);    //Yellow
   m_Colors[2] = RGB(0,0,255);      //Blue
   m_Colors[3] = RGB(255,0,0);      //Red
   m_Colors[4] = RGB(255,128,0);    //Orange
   m_Colors[5] = RGB(0,121,0);      //Green
   m_Colors[6] = RGB(255,255,255);  //White

   m_SelectedSide = m_Front;
}

CCubeWnd::~CCubeWnd()
{
   wglMakeCurrent(NULL,NULL);
   wglDeleteContext(m_hRC);
}


BEGIN_MESSAGE_MAP(CCubeWnd, CWnd)
   ON_WM_PAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
   ON_WM_ENABLE()
   ON_WM_ERASEBKGND()
   ON_WM_SIZE()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONUP()
   ON_WM_CREATE()
END_MESSAGE_MAP()



// CCubeWnd message handlers

// CCube message handlers
void CCubeWnd::PreSubclassWindow()
{
   CWnd::PreSubclassWindow();
//	InitGL();		// Initialize OpenGL

}

void CCubeWnd::OnEnable(BOOL bEnable)
{
   CWnd::OnEnable(bEnable);
   Invalidate();
   UpdateWindow();
}

void CCubeWnd::OnPaint()
{
   CPaintDC dc(this); // device context for painting
   if (m_Flat)
   {
      DrawFlatCube(&dc);
   }
   else
   {
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	   glLoadIdentity();									// Reset The Current Modelview Matrix
	   glTranslated(0.0,0.0,-7.0);						//The Screen 7.0
      glRotatef(m_RotateX,1,0,0);
      glRotatef(m_RotateY,0,1,0);

      m_3DCube.Render();

      SwapBuffers(m_hDC);
   }
}

void CCubeWnd::DrawFlatCube(CDC *pDC)
{
CDC MemDC;
CBitmap Bmp;
CRect Rect;

   GetClientRect(&Rect);
   MemDC.CreateCompatibleDC(pDC);
   Bmp.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());
   CBitmap *OldBmp = MemDC.SelectObject(&Bmp);

	int X = Rect.Width() / 12;
	int Y = Rect.Height() / 9;
	m_CubeWidth = min(X,Y);


   CBrush Brush(RGB(204,204,204));
   MemDC.FillRect(&Rect,&Brush);
   DrawFlatSide(&MemDC,m_Top,CPoint((m_CubeWidth * 3) + 3,1));
   DrawFlatSide(&MemDC,m_Front,CPoint((m_CubeWidth * 3) + 3,3+(m_CubeWidth * 3)));
   DrawFlatSide(&MemDC,m_Down,CPoint((m_CubeWidth * 3) + 3,5+(m_CubeWidth * 6)));
   DrawFlatSide(&MemDC,m_Left,CPoint(1,3+(m_CubeWidth * 3)));
   DrawFlatSide(&MemDC,m_Right,CPoint((m_CubeWidth * 6)+5,3+(m_CubeWidth * 3)));
   DrawFlatSide(&MemDC,m_Back,CPoint((m_CubeWidth * 9)+7,3+(m_CubeWidth * 3)));

   pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);

   MemDC.SelectObject(OldBmp);
}

void CCubeWnd::DrawFlatSide(CDC *pDC,CSide *pSide,CPoint Start)
{
   CPoint P1(Start);
   CPoint P2(Start.x + m_CubeWidth,Start.y + m_CubeWidth);

   for (int i = 0; i < 3;i++)
   {
      for (int j = 0; j < 3;j++)
      {
         CPen Pen;
         CBrush Brush(m_Colors[pSide->m_Blocks[i][j]]);
         if (pSide == m_SelectedSide)
         {
            Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
         }
         else
         {
            Pen.CreatePen(PS_SOLID,1,RGB(127,127,127));
         }

         CBrush *pOldBrush = pDC->SelectObject(&Brush);
         CPen *pOldPen = pDC->SelectObject(&Pen);

         CRect Rect(P1,P2);
         pDC->Rectangle(&Rect);
         P1.Offset(m_CubeWidth,0);
         P2.Offset(m_CubeWidth,0);

         pDC->SelectObject(pOldBrush);
         pDC->SelectObject(pOldPen);
      }
      P1.Offset(0,m_CubeWidth);
      P2.Offset(0,m_CubeWidth);
      P1.x = Start.x;
      P2.x = Start.x + m_CubeWidth;
   }
   pSide->m_Rect.SetRect(Start,Start);
   pSide->m_Rect.InflateRect(0,0,m_CubeWidth * 3,m_CubeWidth * 3);
}

void CCubeWnd::SetSquareUnderPoint(CPoint Point,char Color)
{
   CSide *pSide = GetSideUnderPoint(Point);

   if (pSide)
   {
      CRect Rect = pSide->m_Rect;
      Rect.bottom = Rect.top + m_CubeWidth;
      Rect.right = Rect.left + m_CubeWidth;
      int i;
      int j;
      BOOL Found = FALSE;
      for (i = 0;i < 3 && !Found;i++)
      {
         for (j = 0;j < 3 && !Found;j++)
         {
            if (Rect.PtInRect(Point))
            {
               Found = TRUE;
            }
            Rect.OffsetRect(m_CubeWidth,0);
         }
         Rect.left = pSide->m_Rect.left;
         Rect.right = Rect.left + m_CubeWidth;
         Rect.OffsetRect(0,m_CubeWidth);
      }
      if (Found)
      {
         pSide->m_Blocks[--i][--j] = Color;
         m_3DCube.SetColor(pSide->m_Side,i,j,Color);
         Invalidate();
         UpdateWindow();
      }
   }
}

CSide *CCubeWnd::GetSideUnderPoint(CPoint Point)
{
   CSide *Side = NULL;
   if (m_Top->m_Rect.PtInRect(Point))
   {
      Side = m_Top;
   }
   else if (m_Front->m_Rect.PtInRect(Point))
   {
      Side = m_Front;
   }
   else if (m_Down->m_Rect.PtInRect(Point))
   {
      Side = m_Down;
   }
   else if (m_Left->m_Rect.PtInRect(Point))
   {
      Side = m_Left;
   }
   else if (m_Right->m_Rect.PtInRect(Point))
   {
      Side = m_Right;
   }
   else if (m_Back->m_Rect.PtInRect(Point))
   {
      Side = m_Back;
   }
   return Side;
}



BOOL CCubeWnd::IsPointGoodForDrop(CPoint Point)
{
   CSide *pSide = GetSideUnderPoint(Point);

   if (pSide)
   {
      return TRUE;
   }
   return FALSE;
}

void CCubeWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   m_SelectedSide = GetSideUnderPoint(point);

   if (nFlags & MK_CONTROL)
   {
      RotateCCW(m_SelectedSide);
   }
   else if (nFlags & MK_SHIFT)
   {
      RotateCW(m_SelectedSide);
   }

   Invalidate();
   UpdateWindow();

   CWnd::OnLButtonDblClk(nFlags, point);
}


void CCubeWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
   if (nFlags & MK_CONTROL)
   {
//      RotateCubeCCW();
   }
   else if (nFlags & MK_SHIFT)
   {
//      RotateCubeCW();
   }

   Invalidate();
   UpdateWindow();
   CWnd::OnRButtonDown(nFlags, point);
}

void CCubeWnd::UpdateDisplay(bool Force)
{
   CCubeSolver::UpdateDisplay();
   if (m_Animate || Force)
   {
      Invalidate();
      UpdateWindow();
   }
}

void CCubeWnd::YieldToOthers()
{
   CCubeSolver::YieldToOthers();

   DWORD Interval = m_Speed;
   if (!m_Flat)
   {
      Interval = Interval / ((m_Speed/ 10)+1);
   }
   DWORD Start = GetTickCount();
   DWORD Current = 0;   //so we do the loop at least once
   MSG Msg;
   while (Current < Start + Interval)
   {
      while (PeekMessage(&Msg,NULL,0,0,PM_REMOVE))
      {
         if (!PreTranslateMessage(&Msg))
         {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
         }
      }
      Current = GetTickCount();
      if (!m_Animate)
      {
         return;
      }
   }
}

BOOL CCubeWnd::OnEraseBkgnd(CDC* pDC)
{
   return TRUE;
}

void CCubeWnd::OnSize(UINT nType, int cx, int cy)
{
   CWnd::OnSize(nType, cx, cy);
   ReSizeGLScene(cx,cy);
}

void CCubeWnd::SetFlat(bool Flat)
{
   m_Flat = Flat;
   Invalidate();
   UpdateWindow();
}



void CCubeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
   m_LastPoint = point;
   SetCapture();

   if (m_Flat)
   {
      m_SelectedSide = GetSideUnderPoint(point);

      if (nFlags & MK_CONTROL)
      {
         RotateCCW(m_SelectedSide);
      }
      else if (nFlags & MK_SHIFT)
      {
         RotateCW(m_SelectedSide);
      }
      else if (m_SelectedColor > 0)
      {
         SetSquareUnderPoint(point,m_SelectedColor);
      }
   }
   Invalidate();
   UpdateWindow();

   CWnd::OnLButtonDown(nFlags, point);
}

void CCubeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
   ReleaseCapture();
   CWnd::OnLButtonUp(nFlags, point);
}

void CCubeWnd::OnMouseMove(UINT nFlags, CPoint point)
{
   if (!m_Flat)
   {
      if (nFlags & MK_LBUTTON)
      {
         m_RotateX += (point.y - m_LastPoint.y);
         int TempRotate = (int)m_RotateX;
         if (TempRotate< 0)
         {
            TempRotate += 360;
         }
         TempRotate = TempRotate % 360;
         if (TempRotate > 90 && TempRotate < 270)
         {
            m_RotateY -= (point.x - m_LastPoint.x);
         }
         else
         {
            m_RotateY += (point.x - m_LastPoint.x);
         }

         m_LastPoint = point;
         Invalidate();
         UpdateWindow();
      }
   }
   CWnd::OnMouseMove(nFlags, point);
}

int CCubeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   InitGL();
   return 0;
}



//open gl functions
void CCubeWnd::ReSizeGLScene( int width, int height)	// Resize And Initialize The GL Window
{
	// from NeHe's Tutorial 3
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0,(float)width/(float)height,0.1,100.0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}

int CCubeWnd::InitGL()							// Intitialize OpenGl				// All Setup For OpenGL Goes Here
{
   m_hDC = ::GetDC(this->m_hWnd);
   if(!SetPixelformat(m_hDC))	// set pixel format
   {
      ::MessageBox(::GetFocus(),"SetPixelformat Failed!","Error",MB_OK);
   }

   m_hRC = wglCreateContext(m_hDC);	// Create an OpenGL rendering context 
   int i= wglMakeCurrent(m_hDC,m_hRC);	// Make rendering context current

	// from NeHe's Tutorial 3
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.8f, 0.8f, 0.8f, 0.5f);					// Black Background
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

//   GLfloat light_ambient[]  = { 0.15f, 0.15f, 0.15f, 0.15f }; // faint white light
   GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f }; // faint white light
   GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0 }; // white diffuse
   GLfloat light_specular[] = { 0.35f, 0.35f, 0.35f, 1.0 }; // highlights
   GLfloat light_position[] = { 0.0, 10.0, 20.0, 1.0 };

//   glClearColor(0.5, 0, 0.8, 0);
   glShadeModel(GL_SMOOTH);

   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);
   
   CRect Rect;
   GetClientRect(&Rect);
   ReSizeGLScene(Rect.Width(),Rect.Height());
	return TRUE;										// Initialization Went OK
}

BOOL CCubeWnd::SetPixelformat(HDC hdc)
{

    PIXELFORMATDESCRIPTOR *ppfd; 
    int pixelformat; 
 
    PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_GENERIC_FORMAT |
    PFD_DOUBLEBUFFER,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 32-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // no alpha buffer 
    0,                     // shift bit ignored 
    8,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    64,                    // 64-bit z-buffer	 
    8,                     // stencil buffer 
    8,                     // auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 

   
    ppfd = &pfd;

 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        ::MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        ::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 

}

void CCubeWnd::RotateCW(CSide *pSide)
{
   CCubeSolver::RotateCW(pSide);
   Rotate3DCW(pSide);
}

void CCubeWnd::RotateCCW(CSide *pSide)
{
   CCubeSolver::RotateCCW(pSide);
   Rotate3DCCW(pSide);
}

void CCubeWnd::Rotate3DCW(CSide *pSide)
{
   if (!pSide)
   {
      return;
   }
   int NumTurns = (m_Speed / 40);
   if (NumTurns < 2)
   {
      NumTurns = 2;
   }
   while (90 % NumTurns)
   {
      NumTurns++;
   }
   int Degrees =  90 / NumTurns;
   ASSERT(90 % Degrees == 0);
   switch (pSide->m_Side)
   {
   case 'F' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateZ(2, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'B' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateZ(0, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'R' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateX(2, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'L' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateX(0, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'T' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateY(2, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'D' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateY(0, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   }
}

void CCubeWnd::Rotate3DCCW(CSide *pSide)
{
   if (!pSide)
   {
      return;
   }
   int NumTurns = (m_Speed / 40);
   if (NumTurns < 2)
   {
      NumTurns = 2;
   }
   while (90 % NumTurns)
   {
      NumTurns++;
   }
   int Degrees =  90 / NumTurns;
   ASSERT(90 % Degrees == 0);
   switch (pSide->m_Side)
   {
   case 'F' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateZ(2, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'B' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateZ(0, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'R' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateX(2, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'L' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateX(0, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'T' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateY(2, Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   case 'D' :  for (int i = 0; i < NumTurns;i++)
               {
                  m_3DCube.RotateY(0, -Degrees);
                  if (m_Animate && !m_Flat)
                  {
                     UpdateDisplay();
                     YieldToOthers();
                  }
               }
               break;
   }
}

void CCubeWnd::Reset()
{
   CCubeSolver::Reset();
   m_3DCube.Reset();
}


void CCubeWnd::SaveCube()
{
   CCubeSolver::SaveCube();
   m_3DCube.Save();
}

void CCubeWnd::RestoreCube()
{
   CCubeSolver::RestoreCube();
   m_3DCube.Restore();
}
