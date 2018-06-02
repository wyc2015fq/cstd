
// ctor
CXEditPrompt::CXEditPrompt()
  : m_bShowCueBanner(TRUE)
  , m_strPromptText(_T("<Enter text here>"))
  , m_crPromptColor(_RGB(119, 121, 118)) // RAL 9023 (Pearl dark gray)
  // see http://www.highplains.net/pixelcolor.html
  , m_dwCueBannerAlign(ES_CENTER)
{
  m_crBkColor = _GetSysColor(COLOR_WINDOW);
}


// dtor
CXEditPrompt::~CXEditPrompt()
{
  m_brush.DeleteObject();
  m_robrush.DeleteObject();
}

int CXEditPrompt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CXEditPromptBase::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }

  OnInitEdit();
  return 0;
}


// PreSubclassWindow
void CXEditPrompt::PreSubclassWindow()
{
  CXEditPromptBase::PreSubclassWindow();
  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

  if (pThreadState->m_pWndInit == NULL) {
    OnInitEdit();
  }
}

BOOL CXEditPrompt::OnInitEdit()
{
  m_brush.CreateSolidBrush(_GetSysColor(COLOR_WINDOW));

  m_robrush.CreateSolidBrush(_GetSysColor(COLOR_BTNFACE));
  EnableCueBanner();

  //SetWindowText(m_strPromptText);
  SetSel(-1, 0);    // get rid of standard highlighting
  return TRUE;
}


// Reset
void CXEditPrompt::Reset()
{
  EnableCueBanner();

  SetWindowText(m_strPromptText);
  SetSel(m_strPromptText.GetLength(), m_strPromptText.GetLength());
  RedrawWindow();
}


// SetPromptColor
void CXEditPrompt::SetPromptColor(COLORREF crText)
{
  m_crPromptColor = crText;
  //if (m_bFirstTime)
  //  RedrawWindow();
}


// SetPromptText
void CXEditPrompt::SetPromptText(LPCTSTR lpszPrompt)
{
  m_strPromptText = lpszPrompt;

  if (m_strPromptText.IsEmpty()) {
    EnableCueBanner(FALSE);
  }

  if (m_bShowCueBanner) {
    SetWindowText(m_strPromptText);
  }
}

// CtlColor
HBRUSH CXEditPrompt::CtlColor(HDDC hDC, UINT nCtlColor)
{
  if (!m_bIsWindowEnabled) {
    return NULL;
  }

  if (m_bShowCueBanner) {
    clrText = (m_crPromptColor);
  }

  if (m_dwStyle & ES_READONLY) {
    hDC->SetBkColor(_GetSysColor(COLOR_BTNFACE));
    return m_robrush;
  }

  hDC->SetBkColor(m_crBkColor);
  return m_brush;   // setting text color will have no effect unless
  // we return a valid brush
}

// DefWindowProc
LRESULT CXEditPrompt::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_SETTEXT) {
    TCHAR* cp = (TCHAR*) lParam;

    if (!cp) {
      return TRUE;
    }

    // the control is being initialized, just ignore
    if (m_bShowCueBanner && (cp[0] == _T('\0'))) {
      return TRUE;
    }

    // if this is not prompt string, then no need to prompt
    if (m_strPromptText.Compare(cp) != 0) {
      EnableCueBanner(FALSE);
    }
  }
  else if (message == WM_GETTEXT) {
    if (m_bShowCueBanner) {
      TCHAR* cp = (TCHAR*) lParam;

      if (cp && (wParam != 0)) {
        *cp = _T('\0');
      }

      return 0;
    }
  }
  else if (WM_GETTEXTLENGTH == message) {
    if (m_bShowCueBanner) {
      return 0;
    }
  }

  return CXEditPromptBase::DefWindowProc(message, wParam, lParam);
}

void CXEditPrompt::SetBKColor(COLORREF crBK)
{
  m_crBkColor = crBK;
  RedrawWindow();
}

int CXEditPrompt::GetWindowTextLength() const
{
  if (m_bShowCueBanner) {
    return 0;
  }

  return CXEditPromptBase::GetWindowTextLength();
}

void CXEditPrompt::SetCueBannerAlign(DWORD val)
{
  m_dwCueBannerAlign = val;

  if (m_bShowCueBanner) {
    UpdateStyleForCueBanner();
  }
}

