

// CComboListBox

// CCustomDrawComboBox

CCustomDrawComboBox::CCustomDrawComboBox()
  : m_hComboEdit(NULL)
  , m_hComboList(NULL)
{
}

// CCustomDrawComboBox message handlers

void CCustomDrawComboBox::PreSubclassWindow()
{
  CCustomDrawComboBoxBase::PreSubclassWindow();

  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

  if (pThreadState->m_pWndInit == NULL) {
    OnInitCombo();
  }
}

int CCustomDrawComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CCustomDrawComboBoxBase::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }

  OnInitCombo();
  return 0;
}

BOOL CCustomDrawComboBox::OnInitCombo()
{
  COMBOBOXINFO cbi = {sizeof(COMBOBOXINFO)};

  if (::OGetComboBoxInfo(GetSafeHwnd(), &cbi)) {
    m_hComboEdit = cbi.hwndItem;
    m_hComboList = cbi.hwndList;

#ifndef SUBCLASS_LISTBOX_EDIT_IN_CTLCOLOR
    GetComboListBox().SubclassWindow(m_hComboList);
    GetComboEdit().SubclassWindow(m_hComboEdit);
#endif // SUBCLASS_LISTBOX_EDIT_IN_CTLCOLOR

    //EnableWindowTheme(GetSafeHwnd(), L" ", L" ");
  }

  GetComboListBox().SetBuddyComboBox(this);
  SetExtendedUI(TRUE);  // Pressing the DOWN ARROW key displays the list box
  return TRUE;
}

void CCustomDrawComboBox::OnDestroyCombo()
{
  if (GetComboEdit().GetSafeHwnd()) {
    GetComboEdit().UnsubclassWindow();
  }

  if (GetComboListBox().GetSafeHwnd()) {
    GetComboListBox().UnsubclassWindow();
  }
}

void CCustomDrawComboBox::SetItemCount(int nCount)
{
  if (!GetComboListBox().m_hWnd) {
    return;
  }

  int nCurCount = GetCount();
  int nExtraCount = nCurCount - nCount;

  if (nExtraCount > 0) {
    for (int ii = 0; ii < nExtraCount; ++ii) {
      DeleteString(0);
    }
  }
  else if (nExtraCount < 0) {
    for (int ii = 0; ii > nExtraCount; --ii) {
      AddString(_T(""));
    }
  }

}

void CCustomDrawComboBox::OnDestroy()
{
  //TRACE("CCustomDrawComboBox::OnDestroy()\n");
  OnDestroyCombo();
  CCustomDrawComboBoxBase::OnDestroy();
}

str_t CCustomDrawComboBox::GetItemText(UINT nItem)
{
  if (GetComboListBox().GetSafeHwnd()) {
    return GetComboListBox().GetItemText(nItem);
  }

  return _T("");
}

int CCustomDrawComboBox::GetItemIconIndex(UINT nItem)
{
  if (GetComboListBox().GetSafeHwnd()) {
    return GetComboListBox().GetItemIconIndex(nItem);
  }

  return -1;
}

void CCustomDrawComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
  if (GetComboListBox().GetSafeHwnd()) {
    GetComboListBox().DrawItem(lpDIS);
  }
}

void CCustomDrawComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  if (GetComboListBox().GetSafeHwnd()) {
    GetComboListBox().MeasureItem(lpMeasureItemStruct);
  }
}

int CCustomDrawComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
  if (GetComboListBox().GetSafeHwnd()) {
    return GetComboListBox().CompareItem(lpCompareItemStruct);
  }

  return 0;
}

void CCustomDrawComboBox::SetDroppedVisibleItemCount(int nCount)
{
  if (GetComboListBox().GetSafeHwnd()) {
    SetComboboxDroppedVisibleItemCount(GetSafeHwnd(), nCount);
  }
}

void CCustomDrawComboBox::SetTextOfItem(int nIndex)
{
  if (nIndex >= 0) {
    str_t strText = GetItemText(nIndex);
    SetWindowText(strText);
    GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_EDITCHANGE), (LPARAM)GetSafeHwnd());
  }
}

void CCustomDrawComboBox::PreAddToolTipForItem(int nItem)
{
  // nothing to do
}

void CCustomDrawComboBox::PreShowToolTipForItem(int nItem)
{
  // nothing to do
}

void CCustomDrawComboBox::PrePopToolTip()
{
  // nothing to do
}

LRESULT CCustomDrawComboBox::OnCustomToolTipNotify(WPARAM wParam, LPARAM lParam)
{
  switch (wParam) {
  case CDTOOLTIP_ONBEFORE_SHOW: {
    int nItem = (int)GetCustomDrawToolTips().GetDisplayWParam();
    PreShowToolTipForItem(nItem);
  }
  break;

  case CDTOOLTIP_ONBEFORE_POP:
    PrePopToolTip();
    break;
  }

  return 0;
}
