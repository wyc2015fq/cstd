#pragma once


struct LISTCTRLEXDATA
{
	LISTCTRLEXDATA()
	{
		m_crText        = ::GetSysColor(COLOR_WINDOWTEXT);
		m_crBackground  = ::GetSysColor(COLOR_WINDOW);
	}
	CString  m_strText;
	COLORREF m_crText;
	COLORREF m_crBackground;
	CString  m_strImage;
	DWORD    m_dwData;  
};

class CSkinList : public CListCtrl
{
	DECLARE_DYNAMIC(CSkinList)
public:
	CSkinList();
	virtual ~CSkinList();
public:
	void SetItemHeight(int nItemHeight); 
	void SetItemWidth(int nItemHeight);  
	void SetStyle( DWORD style ){ m_dwStyle = style ;}
	DWORD GetStyle() const{ return m_dwStyle ;} ;
	void SetBkColor( COLORREF color );  
	void SetTextColor( COLORREF color ); 
	void SetHighLightColor( COLORREF color ); 
	void SetHighLightTextColor( COLORREF color ); 
	int  InsertItem( int nItem, LPCTSTR lpszItem  ); 
	BOOL SetItemText( int nItem, int nSubItem,LPCTSTR lpszText );
	BOOL SetItemData( int nItem, DWORD dwData  );
	DWORD GetItemData(int nItem) const;
	BOOL DeleteItem( int nItem );
	BOOL DeleteAllItems();
	BOOL SetItemState( int nItem,UINT nState,UINT nMask ); 
	int GetColumnCount() ;
public:
	virtual void PreSubclassWindow();
protected:
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnLButtonDown( NMHDR * pNotifyStruct, LRESULT* result  );
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
	void DrawReport( int nItem, HDC hdc );
	void UpdateWindow();
private:
	int m_nItemHeight;
	int m_nItemWidth;
	COLORREF m_crText;
	COLORREF m_crBackground;
	COLORREF m_crHighLight;
	COLORREF m_crHighLightText;
	DWORD m_dwStyle;
};


