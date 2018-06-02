/* -------------------------------------------------------------------------
//MyCell Library - MyCell version 1.0
//
// This file is a part of the MyCell Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
//
// Creator: yanxueming
// Email: xueming.yan@gmail.com
//Yanxm 2007年12月25日 10:14:43
// -----------------------------------------------------------------------*/

#pragma once
#include <vector>
using namespace std;
#include "Worksheet.h"
#include "AtlMsgMacro.h"
#include "CrossCursor.h"
#include "StyleStore.h"
#include "ATLSheetTabWindow.h"
#include "renderer.h"
#include "CellTypeFactory.h"
namespace mycell{
	extern 	bool HasFocus(HWND hWnd);
	struct CursorFactory
	{
		CrossCursor	crossCursor;
		HCURSOR	sizeWECursor;
		HCURSOR	sizeNSCursor;
		HCURSOR	standardCursor;
	public:
		CursorFactory():crossCursor(_Module.GetResourceInstance())
			,standardCursor(NULL)
		{
			sizeWECursor=LoadCursor(NULL,IDC_SIZEWE);
			sizeNSCursor=LoadCursor(NULL,IDC_SIZENS);
		}
	};
	class Workbook;
	class GridWnd : public CWindowImpl<GridWnd>
	{
		friend class Workbook;
		Worksheet* pSheet_;
		GridRenderer gridRender_;

		//CCell tooltipCell_;
		//TCHAR tooltipText_[MAX_CELLTEXT];
		//pair<TOOLINFO,CToolTipCtrl> tooltip_;

		CursorFactory cf_;

		enum {WM_POSTCREATE=WM_USER+100};
	public:
		DECLARE_WND_CLASS_EX(_T("GridWnd"),CS_DBLCLKS,-1)

		DECLARE_MSG_MAP(GridWnd)

		GridWnd(CellTypeFactory& cellTypeFactory):pSheet_(NULL),gridRender_(cellTypeFactory)
		{
		}
		void SetWorksheet(Worksheet* pSheet);
		void ChangeCursor(const CELLHITTESTINFO& chi);
	private:
		HRESULT SendNotifyMessageToListener(int nRow, int nCol, int nNotifyCode,LPARAM lParam,BOOL* pbHandled) const;
		//void TrackDragCorner(const CELLHITTESTINFO& chi);
		//void TrackDragColDivider(const CELLHITTESTINFO& chi);
		//void TrackDragRowDivider(const CELLHITTESTINFO& chi);
		void CreateTT();
	private:
		LRESULT OnCreate(UINT,WPARAM,LPARAM lParam,BOOL& bHandled);
		//LRESULT OnPostCreate(UINT,WPARAM,LPARAM lParam,BOOL& bHandled);
		LRESULT OnSize(UINT,WPARAM,LPARAM lParam,BOOL&);
		LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
		//LRESULT OnMouseEvent(UINT uMsg,WPARAM,LPARAM lParam,BOOL& bHandled);
		LRESULT OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnImeComposition(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//LRESULT OnMouseWheel(UINT,WPARAM,LPARAM,BOOL&);

		LRESULT OnGetDlgCode(UINT,WPARAM,LPARAM lParam,BOOL&)
		{
			return DLGC_WANTALLKEYS;
		}
		LRESULT OnKillFocus(UINT,WPARAM,LPARAM,BOOL&);
		//LRESULT OnShowWindow(UINT,WPARAM,LPARAM,BOOL&);
		LRESULT OnThemeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	};
	//namespace /*Private*/{
	//	class WorkbookCon : public CWindowImpl<WorkbookCon>
	//	{
	//		CContainedWindowT<CScrollBar> wndHScroll_;
	//		CContainedWindowT<CScrollBar> wndVScroll_;
	//	public:
	//	};
	//}//namespace Private
	class Workbook : public CWindowImpl<Workbook>
	{
		typedef CWindowImpl<Workbook> baseWnd;
		friend class GridWnd;

		enum eBits
		{
			B_OWNERDATA=1<<0,
			B_SHOWHSCROLLBAR=1<<1,
			B_SHOWVSCROLLBAR=1<<2,
			B_ScreenUpdating=1<<3,
		};
		enum 
		{
			WM_POSTCREATE=WM_USER+100,
		};
		DWORD bits_;//位组合
		//DWORD selectionMode_:3;//ESelectionMode
		/*
		BOOL bOwnerData_:1;
		BOOL bShowHScrollBar_:1;
		BOOL bShowVScrollBar_:1;
		*/

		ESelectionMode selectionMode_;

		vector<Worksheet*> vecWorksheet_;
		Worksheet* pCurSheet_;
		HWND hWndListener_;
		//GridWnd eventImpl_;


		//GridRenderer render_;
		StyleStore styleStore_;

		//CContainedWindowT<CScrollBar> wndHScroll_;
		CContainedWindowT<CScrollBar> wndVScroll_;
	    CSheetTabWindow wndTabHScroll_;

		CScrollBar wndBottomRightCorner_;
		GridWnd wndSheet_;

		CellTypeFactory cellTypeFactory_;
	public:
		static TCHAR tooltipText_[MAX_CELLTEXT];
		DECLARE_WND_CLASS_EX(_T("Workbook"),CS_DBLCLKS,-1)

		DECLARE_MSG_MAP(Workbook)

		Workbook();
		~Workbook();
		void Clear();

		CellTypeFactory& GetTypeFactory()
		{
			return cellTypeFactory_;
		}
		const CellTypeFactory& GetTypeFactory()const
		{
			return cellTypeFactory_;
		}

		BOOL IsOwnerData()const
		{
			return bits_&B_OWNERDATA;
			//return bOwnerData_;
		}
		void SetOwnerData(BOOL bOwnerData)
		{
			bOwnerData?bits_|=B_OWNERDATA:bits_&=~B_OWNERDATA;
			//bOwnerData_=bOwnerData;
		}
		void SetShowScrollBars(BOOL bShowHScrollbar,BOOL bShowVScrollbar)
		{
			bShowHScrollbar?bits_|=B_SHOWHSCROLLBAR:bits_&=~B_SHOWHSCROLLBAR;
			bShowVScrollbar?bits_|=B_SHOWVSCROLLBAR:bits_&=~B_SHOWVSCROLLBAR;
			//bShowHScrollBar_=bShowHScrollbar;
			//bShowVScrollBar_=bShowVScrollBar;
		}
		BOOL IsShowHScrollBar()const
		{
			return bits_&B_SHOWHSCROLLBAR;
		}
		BOOL IsShowVScrollBar()const
		{
			return bits_&B_SHOWVSCROLLBAR;
		}
		CComPtr<ICellSymbol> RegisterSymbol(ECellType cellType,ICellSymbol* pSymbol)
		{
			return cellTypeFactory_.SetSymbol(cellType,pSymbol);
			//wndSheet_.gridRender_.RegisterSymbol(cellType,pSymbol);
		}
		ESelectionMode get_SelectionMode()const
		{
			return (ESelectionMode)selectionMode_;
		}
		void put_SelectionMode(ESelectionMode esm)
		{
			selectionMode_=esm;
		}

	public:
		const StyleStore& GetStyles()const
		{
			return styleStore_;
		}
		StyleStore& GetStyles()
		{
			return styleStore_;
		}
		void EnableDrawing(BOOL bEnable=TRUE)
		{
			if(pCurSheet_){
				if(bEnable){
					pCurSheet_->m_hWnd=wndSheet_.m_hWnd;
				}else{
					pCurSheet_->m_hWnd=NULL;
				}
			}
		}
		BOOL IsDrawingEnabled()const
		{
			return pCurSheet_ && pCurSheet_->m_hWnd;
		}
		void SetHScrollTabVisible(BOOL bVis)
		{
			wndTabHScroll_.SetHScrollTabVisible(bVis);
		}
	public:
		const Worksheet* Worksheet_GetActiveSheet()const
		{
			return pCurSheet_;
		}
		Worksheet* Worksheet_GetActiveSheet()
		{
			return pCurSheet_;
		}
		BOOL Worksheet_SetSheetName(size_t idx,LPCTSTR lpszName)
		{
			BOOL bRet=FALSE;
			_ASSERT(idx<vecWorksheet_.size());
			Worksheet* pSheet=Worksheet_GetSheet(idx);
			Worksheet* pFind=Worksheet_FindByName(lpszName);
			if(pFind!=pSheet){
				bRet= pSheet->SetName(lpszName);
			}else
				bRet=TRUE;
			if(bRet)
				wndTabHScroll_.GetFlatTabCtrl()->SetTabItem((int)idx,lpszName);
			return bRet;
		}
		Worksheet* Worksheet_GetSheet(size_t idx)
		{
			_ASSERT(idx<vecWorksheet_.size());
			return vecWorksheet_[idx];
		}
		const Worksheet* Worksheet_GetSheet(size_t idx)const
		{
			_ASSERT(idx<vecWorksheet_.size());
			return vecWorksheet_[idx];
		}
		void Worksheet_swap(size_t nItem1,size_t nItem2)
		{
			_ASSERT(nItem1<vecWorksheet_.size());
			_ASSERT(nItem2<vecWorksheet_.size());
			Worksheet* pSheet=vecWorksheet_[nItem1];
			vecWorksheet_[nItem1]=vecWorksheet_[nItem2];
			vecWorksheet_[nItem2]=pSheet;
		}
		Worksheet* Worksheet_SetActiveSheet(size_t idx);
		size_t Worksheet_GetSheetCount()const
		{
			return vecWorksheet_.size();
		}
		Worksheet* Worksheet_AddNewSheet();
		void Worksheet_DeleteSheet(size_t idx);
		Worksheet* Worksheet_FindByName(LPCTSTR lpszSheetName)const;
		int Worksheet_FindPosByName(LPCTSTR lpszSheetName)const;
	public:
		GridWnd& GetSheetWnd()
		{
			return wndSheet_;
		}
		const GridWnd& GetSheetWnd()const
		{
			return wndSheet_;
		}
	public:
		HRESULT ImportFromXml(EXmlType nType=EXT_UNKNOWN,LPCTSTR Filename=NULL,LPTSTR outFileName=NULL,LPARAM lParam=0);
		HRESULT ExportToXml(EXmlType nType,LPCTSTR Filename=NULL,LPTSTR outFileName=NULL)const;
	public:
		HWND put_listener(HWND hWnd)
		{
			HWND hWndListenerOld=hWndListener_;
			hWndListener_=hWnd;
			return hWndListenerOld; 
		}
		HWND get_listener()const
		{
			return hWndListener_;
		}
		void OnActivateApp(BOOL bActivated);
	public:
		BOOL SubclassWindow(HWND hWnd);
		HWND UnsubclassWindow()
		{
			return baseWnd::UnsubclassWindow();
		}
		HRESULT SendNotifyMessageToListener(int nRow, int nCol, int nNotifyCode,LPARAM lParam,BOOL* pbHandled) const;
	private:
		void CreateChildren();
		void UpdateControlLayout(WORD cx, WORD cy, bool bMouseMove = false);
		LPTSTR GetNewSheetName(TCHAR buf[32]);
		void InitTypeFactory();
private:
		LRESULT OnCreate(UINT,WPARAM,LPARAM lParam,BOOL& bHandled);
		LRESULT OnPostCreate(UINT,WPARAM,LPARAM lParam,BOOL& bHandled);
		LRESULT OnSize(UINT,WPARAM,LPARAM lParam,BOOL&);
		LRESULT OnTabItemChange(int,LPNMHDR pNMHDR, BOOL& bHandled);
		LRESULT OnThemeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
		{
			cellTypeFactory_.OnThemeChanged(wndSheet_.m_hWnd);
			return 0;
		}
	};
}//namespace mycell