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
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include <atltypes.h>
#include <atlgdi.h>
#include <set>
#include "../include/Worksheet.h"
#include "../include/RgnLight.h"
#include "../include/msg.h"
#include "../include/xmlexcel.h"
#include "../include/XmlMyCell.h"
#include "../include/filedialog.h"
#include "../include/GetBaseName.h"
#include "../include/Text.h"
#include "../include/Workbook.h"
#include "../include/BorderPropertyPage.h"
#include "../include/FontPropertyPage.h"
#include "../include/AlignmentPropertyPage.h"
#include "../include/PatternPropertyPage.h"
#include "../include/PropertySheet.h"
#include "../include/EditImp.h"
namespace mycell{
	class ColumnAutoFitPolicy
	{
		Worksheet* pSheet;
		int col,pos;
	public:
		ColumnAutoFitPolicy(Worksheet* p,int _col):pSheet(p),col(_col)
		{
			pos=p->get_RowHeader().get_rows();
		}
		BOOL GetFirst(LPTSTR text)
		{
			pSheet->get_ColHeader().get_text(col,text);
			return TRUE;
		}
		BOOL GetNext(LPTSTR text)
		{
			if(pos){
				pSheet->GetCellText(CCell(--pos,col),text,EGCVD_RENDER);
				return TRUE;
			}
			return FALSE;
		}
	};
	class RowHeaderAutoFitPolicy
	{
		Worksheet* pSheet;
		int pos;
	public:
		RowHeaderAutoFitPolicy(Worksheet* p):pSheet(p)
		{
			pos=p->get_RowHeader().get_rows();
		}
		BOOL GetFirst(LPTSTR text)
		{
			if(pos){
				RowHeader& rh=pSheet->get_RowHeader();
				rh.get_text(--pos,text);
				return TRUE;
			}
			return FALSE;
		}
		BOOL GetNext(LPTSTR text)
		{
			//RowHeader& rh=pSheet->get_RowHeader();
			//rh.get_text(--pos,text);
			return FALSE;
		}
	};
	template<class AutoFitPolicy>
	int GetAutoFit(AutoFitPolicy* p,HDC hDC,int maxWidth)
	{
		//int nWidth=-1;
		//int nTextLen=0;
		TCHAR text[MAX_CELLTEXT];
		CString strMax;
		for(BOOL b=p->GetFirst(text);b;b=p->GetNext(text))
		{
			int const len=lstrlen(text);
			//if(len<1) continue;
			if(len<strMax.GetLength()){
				continue;
			}
			//nTextLen=len;
			strMax=text;
			/*
			DrawText(hDC,text,len,&rcText,DT_CALCRECT);
			int const rcWidth=rcText.right-rcText.left;
			if(nWidth<rcWidth)
				nWidth=rcWidth;
			if(nWidth>=maxWidth){
				nWidth=maxWidth;
				break;
			}
			*/
		}
		//int nWidth=0;
		if(!strMax.IsEmpty()){
			RECT rcText;
			DrawText(hDC,strMax,strMax.GetLength(),&rcText,DT_CALCRECT);
			return min((rcText.right-rcText.left)+4,maxWidth);
			//if(nWidth>=maxWidth){
			//	nWidth=maxWidth;
			//}
		}
		return 0;//nWidth;
	}
	BOOL Worksheet::IsRowFullVisible(int row)const
	{
		_ASSERT(row>=0);
		const RowHeader& rh=get_RowHeader();
		const int nTopVisScrollRow=rh.get_TopVisScrollRow();
		const int nBottomVisScrollRow=rh.get_BottomVisScrollRow();
		if(row>=nTopVisScrollRow && row<nBottomVisScrollRow){
			return TRUE;
		}
		const int nFreezeBottomRow=rh.get_FreezeBottomRow();
		if(/*rh.IsFreezeRowVisible()&&*/row>=rh.get_FreezeTopRow() && row<nFreezeBottomRow)
			return TRUE;
		const CellAxisInfo& cai=get_CellAxisInfo();
		CRect rcCli;GetClientRect(&rcCli);
		if(nBottomVisScrollRow==row){
			return rh.get_DiffHeights(nTopVisScrollRow,nBottomVisScrollRow+1)+cai.HFreezeAxis<=rcCli.Height();
		}
		if(nFreezeBottomRow==row){
			return cai.HFreezeAxis<=cai.HClientAxis;
		}
		return FALSE;
		/*
		if(IsRowPartialVisible(row)){
			const RowHeader& rh=get_RowHeader();
			int const bottomRow=rh.get_BottomVisScrollRow();
			if(row==bottomRow){
				RECT const rc=raw_GetCellRect(row,HEADER_COL);
				RECT rCli;
				GetClientRect(&rCli);
				return rc.bottom<=rCli.bottom;
			}else
				return TRUE;
		}
		return FALSE;
		*/
	}

	CCell Worksheet::tooltipCell_;
	pair<TOOLINFO,CToolTipCtrl> Worksheet::tooltip_;

	Worksheet::Worksheet(Workbook* pWorkbook):pWorkbook_(pWorkbook),selectionImp_(this)
		,bits_(B_SHOWROWHEADER|B_SHOWCOLHEADER|B_SHOWGRID|B_SHOWSELECTION|B_AllowColumnResize)
		,cellStores_(styleStore_)
		,visMergeCells_(this),asbounds_(ActiveSelectionBounds::SOT_NOOP)
		,styleStore_(pWorkbook->GetStyles())
		//,tooltipCell_(-100,-100)
	{
		//tableDefCellStore_.nStyleID=0;
		const StyleDesc& style=pWorkbook_->GetStyles().get_DefStyle();
		Style_SetTableStyle(style);

		szName_[0]=_T('\0');
		selectionImp_.SetActiveSelection(CCellRange(CCell(0,0)));
	}
	//-------------以下注释摘自Excel---------------------------------------//
	//确认输入的名称不多于31个字符。
	//确认名称中不含下列任一字符：:/?*[或]。
	//确认工作表名称不为空。
	//-----------------------------------------------------------------------------//
	BOOL Worksheet::SetName(LPCTSTR name)
	{
		const int len=lstrlen(name);
		if(len>31 || 0==len)
			return FALSE;
		for(int i=0;i<=len;++i){
			const TCHAR ch=name[i];
			if(_T(':')==ch ||_T('/')==ch||_T('?')==ch||_T('*')==ch||_T('[')==ch||_T(']')==ch)
				return FALSE;
		}
		lstrcpy(szName_,name);
		return TRUE;
	}

	BOOL Worksheet::IsColFullVisible(int col)const
	{
		_ASSERT(col>=0);
		const ColHeader& ch=get_ColHeader();
		const int nLeftVisScrollCol=ch.get_LeftVisScrollCol();
		const int nRightVisScrollCol=ch.get_RightVisScrollCol();
		if(col>=nLeftVisScrollCol && col<nRightVisScrollCol){
			return TRUE;
		}
		const int nFreezeRightCol=ch.get_FreezeRightCol();
		if(/*ch.IsFreezeColVisible()&&*/col>=ch.get_FreezeLeftCol() && col<nFreezeRightCol)
			return TRUE;
		const CellAxisInfo& cai=get_CellAxisInfo();
		CRect rcCli;GetClientRect(&rcCli);
		if(nRightVisScrollCol==col){
			return ch.get_DiffWidths(nLeftVisScrollCol,nRightVisScrollCol+1)+cai.VFreezeAxis<=rcCli.Width();
		}
		if(nFreezeRightCol==col){
			return cai.VFreezeAxis<=cai.VClientAxis;
		}
		return FALSE;
		/*
		if(IsColPartialVisible(col)){
			const ColHeader& ch=get_ColHeader();
			int const rightCol=ch.get_RightVisScrollCol();
			if(col==rightCol){
				RECT const rc=raw_GetCellRect(HEADER_COL,col);
				RECT rCli;
				GetClientRect(&rCli);
				return rc.right<=rCli.right;
			}else
				return TRUE;
		}
		return FALSE;
		*/
	}
//	LRESULT Worksheet::OnPaint(UINT,WPARAM,LPARAM,BOOL&)
//	{
//		//DWORD dwTickCount=GetTickCount();
//		//if(dwTickCount-dwTickCount_<100){
//		//	Sleep(30);
//		//	return 0;
//		//}
//		//dwTickCount_=GetTickCount();
//		//GetUpdateRect(&rcClip_);
//		//AtlTrace(_T("\nGridBase::OnPaint BeginPaint-%d"),GetTickCount());
//		
//		/*
//		//test--
//		CCellRange cr=pSelection_->GetActiveSelection();
//		for(int r=cr.TopRow();r<=cr.BottomRow();++r){
//			RECT rc;
//			if(cr.LeftCol()>0){
//				rc=raw_GetCellRect(r,cr.LeftCol()-1);
//				InvalidateRect(&rc,FALSE);
//			}
//			if(cr.RightCol()<get_ColHeader().get_cols()-1){
//				rc=raw_GetCellRect(r,cr.RightCol()+1);
//				InvalidateRect(&rc,FALSE);
//			}
//		}
//		for(int c=cr.LeftCol();c<=cr.RightCol();++c){
//			RECT rc;
//			if(cr.TopRow()>0){
//				rc=raw_GetCellRect(cr.TopRow()-1,c);
//				InvalidateRect(&rc,FALSE);
//			}
//			if(cr.BottomRow()<get_RowHeader().get_rows()-1){
//				rc=raw_GetCellRect(cr.BottomRow()+1,c);
//				InvalidateRect(&rc,FALSE);
//			}
//		}
//		//:~test--
//		*/
//		
//
//		//RecalcVisibleMergeCells();
//		CRgnLight rgnClip;
//		GetWndUpdateRgn(m_hWnd,rgnClip);
//		rgnClip.Optimize();
//
//		CPaintDC dc(m_hWnd);
//		const RowHeader& rh=get_RowHeader();
//		const ColHeader& ch=get_ColHeader();
//		RECT rcli;
//		GetClientRect(&rcli);
//		//RecalcCellAxisInfo(rcli);
//
//		DrawEnvironment de;
//		/*
//		rh.get_rows()=rh.get_rows();
//		ch.get_cols()=ch.get_cols();
//		rh.get_FreezeBottomRow()=rh.get_FreezeBottomRow();
//		rh.get_FreezeTopRow()=rh.get_FreezeTopRow();
//		ch.get_FreezeRightCol()=ch.get_FreezeRightCol();
//		ch.get_FreezeLeftCol()=ch.get_FreezeLeftCol();
//
//		rh.get_TopVisScrollRow()=rh.get_TopVisScrollRow();
//		rh.get_BottomVisScrollRow()=rh.get_BottomVisScrollRow();
//		ch.get_LeftVisScrollCol()=ch.get_LeftVisScrollCol();
//		ch.get_RightVisScrollCol()=ch.get_RightVisScrollCol();
//
//		get_ShowRowHeader()=get_ShowRowHeader();
//		get_ShowColHeader()=get_ShowColHeader();
//		rh.IsFreezeRowVisible()=rh.IsFreezeRowVisible();//nFreezeBottomRow>=0;
//		ch.IsFreezeColVisible()=ch.IsFreezeColVisible();//nFreezeRightCol>=0;
//		(rh.IsFreezeRowVisible()&&ch.IsFreezeColVisible())=rh.IsFreezeRowVisible()&&ch.IsFreezeColVisible();
//
//		SetRect(&de.rRowHeader,rcli.left,get_ShowColHeader()?(rcli.top+ch.get_height()):rcli.top,rcli.left+(get_ShowRowHeader()?rh.get_width():0),rcli.bottom);
//		SetRect(&de.rColHeader,get_ShowRowHeader()?(rcli.left+rh.get_width()):rcli.left,rcli.top,rcli.right,rcli.top+(get_ShowColHeader()?ch.get_height():0));
//		SetRect(&de.rAlthogonal,rcli.left,rcli.top,de.rRowHeader.right,de.rColHeader.bottom);
//		SetRect(&de.rFreezeAlthogonal,de.rRowHeader.right,de.rColHeader.bottom,de.rRowHeader.right+(ch.IsFreezeColVisible()?ch.get_DiffWidths(ch.get_FreezeLeftCol(),ch.get_FreezeRightCol()+1):0),de.rColHeader.bottom+(rh.IsFreezeRowVisible()?rh.get_DiffHeights(rh.get_FreezeTopRow(),rh.get_FreezeBottomRow()+1):0));
//		SetRect(&de.rFreezeRows,de.rFreezeAlthogonal.right,de.rFreezeAlthogonal.top,rcli.right,de.rFreezeAlthogonal.bottom);
//		SetRect(&de.rFreezeCols,de.rFreezeAlthogonal.left,de.rFreezeAlthogonal.bottom,de.rFreezeAlthogonal.right,rcli.bottom);
//		SetRect(&de.rScroll,de.rFreezeRows.left,de.rFreezeCols.top,rcli.right,rcli.bottom);
//		*/
//		const CellAxisInfo& cai=get_CellAxisInfo();//&rcli);
//
//		render_.draw(this,&cai,dc,&rcli,&rgnClip,de);
//		/*
//#ifdef _DEBUG
//		int i=0;
//#endif
//		for (LPCRECT pRect = rgnClip.GetFirst();pRect; pRect = CRgnLight::GetNext(pRect))
//		{
//			// paint the rectangle piece of your shape
//#ifdef _DEBUG
//			AtlTrace(_T("\nGridBase::OnPaint UpdateRect[%d](%d,%d,%d,%d)"),++i,pRect->left,pRect->top,pRect->right,pRect->bottom);
//#endif
//			//de.lprcClip=pRect;
//			render_.draw(dc,&rcli,pRect,de);
//		}
//		AtlTrace(_T("\nGridBase::OnPaint EndPaint tickcount=%d"),GetTickCount());
//		*/
//		return 0;
//	}

	//void Worksheet::SetCellText(int row,int col,LPCTSTR lpszText,BOOL fRedraw)
	//{
	//	
	//}
	//int Worksheet::GetCellText(int row,int col,LPTSTR buf)
	//{
	//	buf[0]=0;
	//	SendNotifyMessageToListener(row,col,OCN_GETCELLTEXT,(LPARAM)buf);
	//	/*
	//	RowHeader& rh=get_RowHeader();
	//	ColHeader& ch=get_ColHeader();
	//	TCHAR sz[100],sz1[100];
	//	rh.get_text(row,sz);
	//	ch.get_text(col,sz1);
	//	wsprintf(buf,_T("%s%s"),sz1,sz);
	//	*/
	//	for(int i=0;i!=MAX_CELLTEXT && buf[i];++i)
	//		;
	//	return i;
	//}
	//// Sends a message to the listener in the form of a WM_NOTIFY message with
	//// a NM_SHEETVIEW structure attached
	HRESULT Worksheet::SendNotifyMessageToListener(int nRow, int nCol, int nNotifyCode,LPARAM lParam,BOOL* pbHandled) const
	{
		return pWorkbook_->SendNotifyMessageToListener(nRow,nCol,nNotifyCode,lParam,pbHandled);
	}
	//{
	//	if(!m_hWnd)
	//		return 0;
	//	HWND hWndListener=get_listener();
	//	if(!hWndListener)
	//		hWndListener=GetParent();
	//	NM_SHEETVIEW nmgv;
	//	nmgv.hr				= S_OK;
	//	nmgv.bHandled	=FALSE;
	//	if (hWndListener && ::IsWindow(hWndListener) && IsWindow()){
	//		nmgv.row			= nRow;
	//		nmgv.col			= nCol;
	//		nmgv.lParam		= lParam;	
	//		nmgv.hdr.hwndFrom	= m_hWnd;
	//		nmgv.hdr.idFrom	= GetDlgCtrlID();
	//		nmgv.hdr.code	= nNotifyCode;
	//		::SendMessage(hWndListener,WM_NOTIFY,nmgv.hdr.idFrom, (LPARAM)&nmgv);
	//	}
	//	if(pbHandled)
	//		*pbHandled=nmgv.bHandled;
	//	return nmgv.hr;
	//}

	/*
	//----------------------------------------------------------------------------//
	//返回客户区坐标，lprc已经包含正确的ClientRect
	//即是说lprc应该在先前由GetClientRect(lprc)得到，
	//通过该方法修改lprc->top,lprc->left左后得到正确的结果。
	//	其bottom和right保持不变
	//----------------------------------------------------------------------------//
	void Worksheet::get_ScrollRect(LPRECT lprc)const
	{
		_ASSERT(lprc->right>=0);
		_ASSERT(lprc->bottom>=0);
		//GetClientRect(lprc);
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		if(get_ShowColHeader())
			lprc->top=ch.get_height();
		if(get_ShowRowHeader())
			lprc->left=rh.get_width();

		int const nFreezeRightCol=ch.get_FreezeRightCol();
		if(nFreezeRightCol>=0){
			lprc->left+=ch.get_DiffWidths(ch.get_FreezeLeftCol(),nFreezeRightCol+1);
		}
		int const nFreezeBottomRow=rh.get_FreezeBottomRow();
		if(nFreezeBottomRow>=0){
			lprc->top+=rh.get_DiffHeights(rh.get_FreezeTopRow(),nFreezeBottomRow+1);
		}
	}
	*/
	////返回客户区坐标
	//RECT Worksheet::get_ScrollRect()
	//{
	//	RECT rc;
	//	GetClientRect(&rc);
	//	get_ScrollRect(&rc);
	//	return rc;
	//}
	void Worksheet::Scroll_ResetHScrollBar(LPCRECT lprcClient)
	{
		_ASSERT(hScrollBar_.IsWindow());

		const BOOL bWindowVis=IsWindowVisible();
#ifdef _DEBUG
		AtlTrace(_T("\nScroll_ResetHScrollBar window visible:"));
		if(bWindowVis)
			AtlTrace(_T("\nScroll_ResetHScrollBar window visible:TRUE"));
		else
			AtlTrace(_T("\nScroll_ResetHScrollBar window visible:FALSE"));
#endif

		CRect rect;
		if(lprcClient)
			rect=*lprcClient;
		else
			GetClientRect(rect);
		RecalcCellAxisInfo(rect,CCAI_CALC_ALL);
		const CellAxisInfo& cai=get_CellAxisInfo();
		rect=cai.get_ScrollRect();
		if (rect.left >= rect.right || rect.top >= rect.bottom)
			return;

		SCROLLINFO si={sizeof(SCROLLINFO),SIF_PAGE | SIF_RANGE | SIF_POS,0};
		const ColHeader& ch=get_ColHeader();
		int const widths=ch.get_widths()-ch.get_DiffWidths(0,ch.get_LeftScrollableCol());
		if(widths>rect.Width()){
			const int cols=ch.get_cols();//-ch.get_LeftScrollableCol();
			const bool bLastColFullVis=ch.get_RightVisScrollCol()==cols-1&&IsColFullVisible(cols-1)?true:false;
			//si.nPage	= (cols-ch.get_LeftScrollableCol())/10;
			si.nMin	= ch.get_LeftScrollableCol();
			si.nMax	= cols;//(bLastColFullVis?cols-1:cols)-si.nPage;
			si.nPos	= ch.get_LeftVisScrollCol();
			si.nPage=MulDiv(rect.Width(),cols-ch.get_LeftScrollableCol(),widths);
			if(bLastColFullVis){
				si.nPage=si.nMax-si.nPos+1;
			}
			//hScrollBar_.EnableScrollBar(ESB_ENABLE_BOTH);
			Scroll_SetHScrollInfo(&si,bWindowVis);
//#ifdef _DEBUG
//			CString str;
//			str.Format(_T("nMax=%d,nPage=%d,nPos=%d"),si.nMax,si.nPage,si.nPos);
//			CWindow parent(::GetParent(GetParent()));
//			parent.SetWindowText(str);
//#endif

		}else{
			//hScrollBar_.EnableScrollBar(ESB_DISABLE_BOTH);
			//si.nMin	= 0;//ch.get_LeftScrollableCol();
			si.nMax	= 10;//cols;//(bLastColFullVis?cols-1:cols)-si.nPage;
			//si.nPos	= 0;//ch.get_LeftVisScrollCol();
			si.nPage=11;//MulDiv(rect.Width(),cols-ch.get_LeftScrollableCol(),widths);
			//if(bLastColFullVis){
			//	si.nPage=si.nMax-si.nPos+1;
			//}
			//hScrollBar_.EnableScrollBar(ESB_ENABLE_BOTH);
			Scroll_SetHScrollInfo(&si,bWindowVis);
		}
	}
	void Worksheet::Scroll_ResetVScrollBar(LPCRECT lprcClient)
	{
		_ASSERT(vScrollBar_.IsWindow());
		CRect rect;
		if(lprcClient)
			rect=*lprcClient;
		else
			GetClientRect(rect);
		RecalcCellAxisInfo(rect,CCAI_CALC_ALL);
		const CellAxisInfo& cai=get_CellAxisInfo();
		rect=cai.get_ScrollRect();
		if (rect.left >= rect.right || rect.top >= rect.bottom)
			return;

		SCROLLINFO si={sizeof(SCROLLINFO),SIF_PAGE | SIF_RANGE | SIF_POS,0};
		const RowHeader& rh=get_RowHeader();
		int const heights=rh.get_heights()-rh.get_DiffHeights(0,rh.get_TopScrollableRow());
		if(heights>rect.Height()){
			const int rows=rh.get_rows();
			const bool bLastRowFullVis=rh.get_BottomVisScrollRow()==rows-1&&IsRowFullVisible(rows-1)?true:false;
			//si.nPage	=	rh.get_BottomVisScrollRow()-rh.get_TopVisScrollRow()+1;//MulDiv(rows,rh.get_DefRowHeight(),rect.Height());
			si.nMin	=	rh.get_TopScrollableRow();
			si.nMax	=	bLastRowFullVis?rows-1:rows;
			si.nPos	=	rh.get_TopVisScrollRow();
			si.nPage=MulDiv(rect.Height(),rows-rh.get_TopScrollableRow(),heights);
			if(bLastRowFullVis){
				si.nPage=si.nMax-si.nPos+1;
			}
			Scroll_SetVScrollInfo(&si);
		}else{
			//vScrollBar_.EnableScrollBar(ESB_DISABLE_BOTH);

			si.nMax	= 10;//cols;//(bLastColFullVis?cols-1:cols)-si.nPage;
			si.nPage=11;//MulDiv(rect.Width(),cols-ch.get_LeftScrollableCol(),widths);
			Scroll_SetVScrollInfo(&si);
		}
	}
	/*
	void Worksheet::Scroll_ResetScrollBars(BOOL bResetHCrollBar,BOOL bResetVScrollBar,LPCRECT lprcClient)//const CellAxisInfo* pcai)
	{
		_ASSERT(hScrollBar_.IsWindow());
		_ASSERT(vScrollBar_.IsWindow());
		CRect rect;
		if(lprcClient)
			rect=*lprcClient;
		else
			GetClientRect(rect);
		RecalcCellAxisInfo(rect,CCAI_CALC_ALL);
		const CellAxisInfo& cai=get_CellAxisInfo();
		rect=cai.get_ScrollRect();
		if (rect.left >= rect.right || rect.top >= rect.bottom)
			return;

		SCROLLINFO si={sizeof(SCROLLINFO),SIF_PAGE | SIF_RANGE | SIF_POS};

		if(bResetVScrollBar)
		{
			const RowHeader& rh=get_RowHeader();
			si.nMin		= 0;
			int const heights=rh.get_heights()-rh.get_DiffHeights(0,rh.get_TopScrollableRow());
			if(heights>rect.Height()){
				si.nPage=rect.Height();
				si.nMax=heights;
				si.nPos=rh.get_DiffHeights(rh.get_TopScrollableRow(),rh.get_TopVisScrollRow());
				Scroll_SetVScrollInfo(&si);
			}else{
				//si.nPage	= 0;
				//si.nPos		= 0;
				//si.nMax		= 0;
				vScrollBar_.EnableScrollBar(ESB_DISABLE_BOTH);
			}
			//vScrollBar_.SetScrollInfo(&si, TRUE);
		}
		if(bResetHCrollBar)
		{
			const ColHeader& ch=get_ColHeader();
			si.nMin = 0;
			int const widths=ch.get_widths()-ch.get_DiffWidths(0,ch.get_LeftScrollableCol());
			if(widths>rect.Width()){
				si.nPage	= rect.Width();
				si.nMax	= widths;
				si.nPos	= ch.get_DiffWidths(ch.get_LeftScrollableCol(),ch.get_LeftVisScrollCol());
				Scroll_SetHScrollInfo(&si);
			}else{
				//si.nPage	= rect.Width();
				//si.nMax	= rect.Width();
				//si.nPos	= 0;
				//Scroll_SetHScrollInfo(&si);
				hScrollBar_.EnableScrollBar(ESB_DISABLE_BOTH);
			}
			//hScrollBar_.SetScrollInfo(&si, TRUE);
		}
	}
	*/
	CCell Worksheet::SetActiveCell(int row,int col,Int2Type<true>)
	{
#ifdef _DEBUG
		_ASSERT(col>=0);
		//_ASSERT(col<cols);
		_ASSERT(col>=0);
		//_ASSERT(row<rows);
#endif
		const int cols=get_ColHeader().get_cols();
		const int rows=get_RowHeader().get_rows();
		CCell const oldCell=ActiveCell_t::GetActiveCell();
		if(row>=rows || col>=cols)
			return oldCell;
		//CCell const oldCell=GetActiveCell();
		if(row!=oldCell.row || col!=oldCell.col){
			//SetActiveCell(row,col);
			const CCell newActiveCell=ActiveCell_t::SetActiveCell(row,col);
			RECT rc=raw_GetCellRect(newActiveCell);//row,col);
			InvalidateRect(&rc,FALSE);
			rc=raw_GetCellRect(oldCell);//.row,oldCell.col);
			InvalidateRect(&rc,FALSE);
			return newActiveCell;
		}
		return oldCell;
	}

	//-----------------------------------------------------------------------------//
	//使单元格可见
	//bFullVisible=TRUE使完全可见否只能保证部分可见
	//-----------------------------------------------------------------------------//
	void Worksheet::EnsureVisible(int row,int col,BOOL bUpdateWindow,BOOL bFullVisible)//,BOOL bPartial,bool bUpdateWindow)
	{
		RowHeader& rh=rh_;
		ColHeader& ch=ch_;
		_ASSERT(row<rh.get_rows());
		int const nTopVisScrollRow=rh.get_TopVisScrollRow();
		int const nLeftVisScrollCol=ch.get_LeftVisScrollCol();
		const CellAxisInfo& cai=get_CellAxisInfo();

		if(row>=rh.get_TopScrollableRow() && row<=rh.get_BottomScrollableRow()){
			if(row<nTopVisScrollRow){
				put_TopVisScrollRow(row);
			}else if(row>=rh.get_BottomVisScrollRow()){
				put_BottomVisScrollRow(row,bFullVisible);
			}
		}
		if(col>=ch.get_LeftScrollableCol() && col<=ch.get_RightScrollableCol()){
			if(col<nLeftVisScrollCol){
				put_LeftVisScrollCol(col,/*si,*/&cai);
			}else if(col>=ch.get_RightVisScrollCol()){
				put_RightVisScrollCol(col,/*si,*/bFullVisible,&cai);
			}
		}
		if(bUpdateWindow && !(nTopVisScrollRow==rh.get_TopVisScrollRow() && nLeftVisScrollCol==ch.get_LeftVisScrollCol())){
			UpdateWindow();
		}
	}
	//------------------------------------------------------------------------------//
	//与RowHeader之put_TopVisScrollRow相比，考虑了滚动窗口和
	//滚动滚动条
	//si.fMask should contain SIF_POS|SIF_RANGE|SIF_PAGE
	//------------------------------------------------------------------------------//
	int Worksheet::put_TopVisScrollRow(int nTopVisScrollRow,const CellAxisInfo* pcai/*,bool bScroll*/)
	{
		if(!pcai)
			pcai=&get_CellAxisInfo();//NULL,CCAI_CALC_HAXIS);
		RowHeader& rh=rh_;
		const int oTopVisScrollRow=rh.get_TopVisScrollRow();
		const int oBottomVisScrollRow=rh.get_BottomVisScrollRow();
		if(nTopVisScrollRow<rh.get_TopScrollableRow())
			nTopVisScrollRow=rh.get_TopScrollableRow();
		if(nTopVisScrollRow>rh.get_BottomScrollableRow())
			nTopVisScrollRow=rh.get_BottomScrollableRow();

		const CRect rcScroll(0,pcai->HFreezeAxis,pcai->VClientAxis,pcai->HClientAxis);
		nTopVisScrollRow=rh.put_TopVisScrollRow(nTopVisScrollRow,rcScroll.Height());
		if(m_hWnd && nTopVisScrollRow!=oTopVisScrollRow)
		{
			int nScrollHeight=0;
			if(nTopVisScrollRow>oTopVisScrollRow&&oTopVisScrollRow>=0){
				nScrollHeight=-rh.get_DiffHeights(oTopVisScrollRow,nTopVisScrollRow);
			}else if(nTopVisScrollRow<oTopVisScrollRow){
				nScrollHeight=rh.get_DiffHeights(max(0,nTopVisScrollRow),oTopVisScrollRow);
			}
			//else
			//	return oTopVisScrollRow;
			
			//SCROLLINFO si={sizeof(SCROLLINFO),SIF_POS|SIF_RANGE|SIF_PAGE};
			//vScrollBar_.GetScrollInfo(&si);
			//int const nPos=IsRowFullVisible(rh.get_rows()-1)?si.nMax-si.nPage:si.nPos-nScrollHeight;
			Scroll_ResetVScrollBar();
			
			//Scroll_ResetScrollBars(FALSE,TRUE);
				ScrollWindow(0,nScrollHeight, rcScroll, rcScroll);
		}
		if(!(oTopVisScrollRow==nTopVisScrollRow && oBottomVisScrollRow==rh.get_BottomVisScrollRow()))
			OnVisibleRowChanged(oTopVisScrollRow,oBottomVisScrollRow);
		return nTopVisScrollRow;
	}
	//------------------------------------------------------------------------------//
	//与ColHeader之put_LeftVisScrollCol相比，考虑了滚动窗口和
	//滚动滚动条
	//si.fMask should contain SIF_POS|SIF_RANGE|SIF_PAGE
	//------------------------------------------------------------------------------//
	int Worksheet::put_LeftVisScrollCol(int nLeftVisScrollCol,const CellAxisInfo* pcai,bool bResetScrollBar)
	{
		if(!pcai)
			pcai=&get_CellAxisInfo();//NULL,CCAI_CALC_VAXIS);
		ColHeader& ch=ch_;
		const int oLeftVisScrollCol=ch.get_LeftVisScrollCol();
		const int oRightVisScrollCol=ch.get_RightVisScrollCol();
		if(nLeftVisScrollCol<ch.get_LeftScrollableCol())
			nLeftVisScrollCol=ch.get_LeftScrollableCol();
		if(nLeftVisScrollCol>ch.get_RightScrollableCol())
			nLeftVisScrollCol=ch.get_RightScrollableCol();
		
		const CRect rcScroll(pcai->VFreezeAxis,0,pcai->VClientAxis,pcai->HClientAxis);
		const int nLeftCol=ch.put_LeftVisScrollCol(nLeftVisScrollCol,rcScroll.Width());
		if(m_hWnd && nLeftCol!=oLeftVisScrollCol){
			int nScrollWidth=0;
			if(nLeftCol>oLeftVisScrollCol){
				nScrollWidth=-ch.get_DiffWidths(oLeftVisScrollCol,nLeftCol);
			}else if(nLeftCol<oLeftVisScrollCol){
				nScrollWidth=ch.get_DiffWidths(nLeftCol,oLeftVisScrollCol);
			}
			//else
			//	return oLeftVisScrollCol;
			if(nScrollWidth){
				//SCROLLINFO si={sizeof(SCROLLINFO),SIF_POS|SIF_RANGE|SIF_PAGE};
				//Scroll_GetHScrollInfo(&si);
				//hScrollBar_.GetScrollInfo(/*SB_HORZ, */&si);
				//int const nPos=IsColFullVisible(ch.get_cols()-1)?si.nMax-si.nPage:si.nPos-nScrollWidth;
				//int const nPos=IsColFullVisible(ch.get_cols()-1)?si.nMax-si.nPage:si.nPos
				//	+(nLeftCol-oLeftVisScrollCol);//nScrollWidth;
				//Scroll_SetHScrollPos32(/*SB_HORZ,*/nPos);
				if(bResetScrollBar)
					Scroll_ResetHScrollBar();
				
				//Scroll_ResetScrollBars(FALSE,TRUE);
				ScrollWindow(nScrollWidth,0, rcScroll, rcScroll);
			}
		}
		if(!(nLeftCol==oLeftVisScrollCol && oRightVisScrollCol==ch.get_RightVisScrollCol()))
		{
			OnVisibleColChanged(oLeftVisScrollCol,oRightVisScrollCol);//,nLeftCol,ch.get_RightVisScrollCol());
		}
		return nLeftCol;
	}
	void Worksheet::put_RowHeight(int row,int newHeight,BOOL fRedrawHeader,bool bUpdateWindow)
	{
		RowHeader& rh=get_RowHeader();
		const int rows=rh.get_rows();
		_ASSERT(row>=0);
		_ASSERT(row<rows);
		if(row<0 || row>=rows)
			return;
		//render_.SetDirty();
		int const oldTopRow=rh.get_TopVisScrollRow();
		int const oldBottomRow=rh.get_BottomVisScrollRow();
		if(m_hWnd && (row>=oldTopRow && row<=oldBottomRow
			|| rh.IsFreezeRowVisible() 
			&& row>=rh.get_FreezeTopRow() && row<=rh.get_FreezeBottomRow()))
		{
			int const oldHeight=rh.get_RowHeight(row);
			int const delta=newHeight-oldHeight;
			RECT rCli;//=get_ScrollRect();
			GetClientRect(&rCli);
			if(!fRedrawHeader && get_ShowRowHeader()){
				//GetClientRect(&rCli);
				rCli.left+=rh.get_width();
			}
			CRect const rcCell=raw_GetCellRect(CCell(row,IGNOR_COL));
			rh.put_RowHeight(row,newHeight);
			//RECT const rScroll=get_CellAxisInfo().get_ScrollRect();
			const CellAxisInfo& cai=get_CellAxisInfo();
			const CRect rScroll(0,cai.HFreezeAxis,cai.VClientAxis,cai.HClientAxis);
			AtlTrace(_T("\nGrid::put_RowHeight cause bottomRow from %d"),rh.get_BottomVisScrollRow());
			rh.validate_BottomVisScrollRow(rScroll.Height());
			AtlTrace(_T(" to %d"),rh.get_BottomVisScrollRow());
			rCli.top=min(rcCell.bottom,rcCell.top+newHeight)+2;
			ScrollWindow(0,delta,&rCli,&rCli);
			//UpdateWindow();

			rCli.top=rcCell.top-2;
			rCli.bottom=rcCell.top+newHeight+2;
			InvalidateRect(&rCli,FALSE);
			VisibleMergeCellMgr& vmc=GetVisibleMergeCellMgr();
			vmc.InvalidRect(&rCli);
			if(bUpdateWindow)
				UpdateWindow();
		}else
			rh.put_RowHeight(row,newHeight);
		if(m_hWnd){
			SendNotifyMessageToListener(0,0,OCN_ROWSHEIGHTSCHANGE,0);
			Scroll_ResetVScrollBar();
		}
	}

	//void Grid::ColumnAutoFit(int col)
	//{
	//	CClientDC dc(m_hWnd);
	//	ColumnAutoFitPolicy afp(this,col);
	//	int const nWidth=GetAutoFit<ColumnAutoFitPolicy>(&afp,dc);
	//	if(nWidth>0){
	//		put_ColWidth(col,nWidth);
	//	}
	//}

	void Worksheet::put_RowsHeight(int beg,int end,int newHeight,bool bRedrawWindow)
	{
		//render_.SetDirty();
		RowHeader& rh=get_RowHeader();
		for(int row=max(0,beg);row<end;++row){
			rh.put_RowHeight(row,newHeight);
		}
		SendNotifyMessageToListener(0,0,OCN_ROWSHEIGHTSCHANGE,0);
		Scroll_ResetVScrollBar();//TRUE,FALSE);

		//RECT const rScroll=get_CellAxisInfo().get_ScrollRect();
		const CellAxisInfo& cai=get_CellAxisInfo();
		const CRect rScroll(0,cai.HFreezeAxis,cai.VClientAxis,cai.HClientAxis);
		rh.validate_BottomVisScrollRow(rScroll.Height());

		if(bRedrawWindow)
			RedrawWindow();
	}
	//[beg,end)
	void Worksheet::put_ColsWidth(int beg,int end,int newWidth,bool bRedrawWindow)
	{
		//render_.SetDirty();
		ColHeader& ch=get_ColHeader();
		for(int col=max(0,beg);col<end;++col){
			ch.put_ColWidth(col,newWidth);
		}

		SendNotifyMessageToListener(0,0,OCN_COLSWIDTHSCHANGE,0);
		Scroll_ResetHScrollBar();//TRUE,FALSE);

		CRect const rScroll=get_CellAxisInfo().get_ScrollRect();
		ch.validate_RightVisScrollCol(rScroll.Width());

		if(bRedrawWindow)
			RedrawWindow();
	}
	void Worksheet::put_ColWidth(int col,int newWidth,BOOL fRedrawHeader,bool bUpdateWindow)
	{
		ColHeader& ch=get_ColHeader();
		const int cols=ch.get_cols();
		_ASSERT(col>=0);
		_ASSERT(col<cols);
		if(col<0 || col>=cols)
			return;
		//render_.SetDirty();
		int const oldLeftCol=ch.get_LeftVisScrollCol();
		int const oldRightCol=ch.get_RightVisScrollCol();
		if((col>=oldLeftCol && col<=oldRightCol
			|| ch.IsFreezeColVisible()
			&& col>=ch.get_FreezeLeftCol()&&col<=ch.get_FreezeRightCol())
			&& m_hWnd)
		{
			int const oldWidth=ch.get_ColWidth(col);
			int const delta=newWidth-oldWidth;
			RECT rCli;//=get_ScrollRect();//;
			GetClientRect(&rCli);
			if(!fRedrawHeader && get_ShowColHeader()){
				rCli.top+=ch.get_height();
				//GetClientRect(&rCli);
			}
			CRect const rcCell=raw_GetCellRect(CCell(IGNOR_ROW,col));
			ch.put_ColWidth(col,newWidth);
			//if(fRedraw){
				CRect const rScroll=get_CellAxisInfo().get_ScrollRect();
				AtlTrace(_T("\nGrid::put_ColWidth cause rightCol from %d"),ch.get_RightVisScrollCol());
				ch.validate_RightVisScrollCol(rScroll.Width());
				AtlTrace(_T(" to %d"),ch.get_RightVisScrollCol());
				rCli.left=min(rcCell.right,rcCell.left+newWidth)+2;
				ScrollWindow(delta,0,&rCli,&rCli);
				//UpdateWindow();

				rCli.left=rcCell.left-2;
				rCli.right=rcCell.left+newWidth+2;
				InvalidateRect(&rCli,FALSE);

				VisibleMergeCellMgr& vmc=GetVisibleMergeCellMgr();
				vmc.InvalidRect(&rCli);
				if(bUpdateWindow)
					UpdateWindow();
		}else
			ch.put_ColWidth(col,newWidth);
		if(m_hWnd){
			SendNotifyMessageToListener(0,0,OCN_COLSWIDTHSCHANGE,0);
			Scroll_ResetHScrollBar();
		}
	}
	void Worksheet::CopyToClipboard(int y1,int x1,int y2,int x2)
	{
		CWaitCursor wc;
		/*
#ifdef _DEBUG
		{
			if(::OpenClipboard(NULL)){
				EmptyClipboard();
				
				LPTSTR  lptstrCopy; 
				HGLOBAL hglbCopy; 
				//LPTSTR  lptstrCopy; 
				string vecstr("123");
				typedef string::value_type char_t;
				HGLOBAL hglb=GlobalAlloc(GMEM_MOVEABLE,(vecstr.size()+1)*sizeof(char_t));
				if (hglb) 
				{ 
					char_t* lpstr=(char_t*)GlobalLock(hglb);

					//wcscpy(lpstr,vecstr.c_str());
					memcpy(lpstr,vecstr.c_str(),sizeof(char_t)*(vecstr.size()+1));
					//lpstr[vecstr.size()]=0;
					GlobalUnlock(hglb);
					SetClipboardData(CF_TEXT,hglb);
				}
				CloseClipboard();
				//if(hglb)
				//	GlobalFree(hglb);
			}else{
				FormatMessage(m_hWnd);
			}
			return ;
		}
#endif
		*/
//#ifdef UNICODE
//		std::wstring vecstr;
//#else
//		std::string vecstr;
//#endif
		//vector<wchar_t> vecstr;
		//vecstr.resize((y2-y1+1)*(x2-x1+1));
		wstring vecstr;
//#ifndef UNICODE
//		WCHAR wtext[MAX_CELLTEXT];
//#endif
		TCHAR text[MAX_CELLTEXT];
		_ASSERT(y1<=y2);
		_ASSERT(x1<=x2);
		if(y1<0)
			y1=0;
		if(x1<0)
			x1=0;
		RowHeader const& rh=get_RowHeader();
		ColHeader const& ch=get_ColHeader();
		for(;y1<=y2;++y1)
		{
			if(rh.get_RowHeight(y1)<=1) continue;
			for(int x=x1;x<=x2;++x)
			{
				if(ch.get_ColWidth(x)<=1) continue;
				const CCell cell(y1,x);
				int const nCount=GetCellText(cell,text,EGCVD_RENDER);
				if(nCount>0){
					const BOOL bHasReturnChar=Text_HasReturnChar(text,nCount);
#ifdef UNICODE
					if(bHasReturnChar){
						vecstr+=L"\"";
						vecstr+=text;
						vecstr+=L"\"";
					}else
						vecstr+=text;
#else
					WCHAR wtext[MAX_CELLTEXT];
					wtext[0]='\0';
					int lRet=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,text,nCount,wtext,MAX_CELLTEXT);
					if(lRet>0){
						if(lRet>=MAX_CELLTEXT)
							lRet=MAX_CELLTEXT-1;
						wstring wstrTmp;
						//wtext[lRet]='\0';
						for(int i=0;i!=lRet;++i)
						{
							if(wtext[i]=='"'){
								wstrTmp+=bHasReturnChar?L"\"\"":L"\"";
							}else if(wtext[i]=='\t')
								wstrTmp+=' ';
							else
								wstrTmp+=wtext[i];
								//wtext[i]=' ';
						}
						//wstrTmp+=L'\0';
						if(bHasReturnChar){
							vecstr+=L"\"";
							vecstr+=wstrTmp;//wtext;
							vecstr+=L"\"";
						}else{
							vecstr+=wstrTmp;//wtext;
						}
					}
#endif
				}
				vecstr+=0x09;
			}
			vecstr[vecstr.size()-1]=0x0D;
			vecstr+=0x0A;
		}
		if(!vecstr.empty()){
			if(::OpenClipboard(0)){
				EmptyClipboard();
//#ifdef UNICODE
				HGLOBAL hglb=GlobalAlloc(GMEM_MOVEABLE,(vecstr.size()+1)*sizeof(wchar_t));
				if (hglb) 
				{ 
					wchar_t* lpstr=(wchar_t*)GlobalLock(hglb);
					wcscpy(lpstr,vecstr.c_str());
					GlobalUnlock(hglb);
					SetClipboardData(CF_UNICODETEXT,hglb);
					//GlobalFree(hglb); do't delete this comment
//#else
//				HGLOBAL hglb=GlobalAlloc(GMEM_MOVEABLE,(vecstr.size()+1)*sizeof(char));
//				char* lpstr=(char*)GlobalLock(hglb);
//				strcpy(lpstr,vecstr.c_str());
//				GlobalUnlock(hglb);
//				SetClipboardData(CF_TEXT,hglb);
//#endif
				}
				CloseClipboard();
			}else{
				FormatMessage(m_hWnd);
			}
		}
	}
	void Worksheet::RowHeaderAutoFit()
	{
		//render_.SetDirty();
		CClientDC dc(m_hWnd);
		CRect const rcContent=get_CellAxisInfo().get_ScrollRect();
		RowHeaderAutoFitPolicy afp(this);
		int nWidth=GetAutoFit<RowHeaderAutoFitPolicy>(&afp,dc,rcContent.Width()-1);
		if(nWidth>0){
			RowHeader& rh=get_RowHeader();
			int const oldWidth=rh.get_width();
			nWidth=max(nWidth,rh.get_MinWidth());
			int const xDelta=nWidth-oldWidth;
			if(xDelta){
				rh.put_width(nWidth);
				if(IsWindow()){
					RECT rcContent=get_CellAxisInfo().get_ScrollRect();
					rcContent.top=0;
					ScrollWindow(xDelta,0,&rcContent,&rcContent);
					rcContent.right=rcContent.left;
					rcContent.left=0;
					InvalidateRect(&rcContent,FALSE);
					UpdateWindow();
				}
			}
		}
	}
	//delete range [row,end]
	BOOL Worksheet::delete_cols(int beg,int end,bool bFireMsg)
	{
		const int cols=ch_.get_cols();
		_ASSERT(beg>=0);
		_ASSERT(end<cols);
		_ASSERT(end>=beg);
		if(beg<0)beg=0;
		if(end>=cols)end=cols-1;
		if(beg>end)
			return FALSE;
		if(bFireMsg){
			BOOL bAllowDelete=TRUE;
			SendNotifyMessageToListener(beg,end,OCN_DELETECOLS,(LPARAM)&bAllowDelete);
			if(!bAllowDelete)
				return FALSE;
		}
		BOOL const isWindow=IsWindow();
		vector<CCellRange> vecCrs;
		MergeCellMgr::OnColDelete(beg,end,vecCrs);
		//CellTypeMgr_t::OnColDelete(beg,end);
		//GridBorders::OnColDelete(beg,end);
		//FontsMgr::OnColDelete(beg,end);
		cellStores_.OnColDelete(beg,end);
		selectionImp_.OnColDelete(beg,end);
		if(isWindow){
			for(vector<CCellRange>::const_iterator it=vecCrs.begin();it!=vecCrs.end();++it)
				InvalidateCellRange(&*it);
		}
		CRect rcContent;
		if(isWindow)
			rcContent=get_CellAxisInfo().get_ScrollRect();
		ColHeader& ch=get_ColHeader();
		int const leftCol=ch.get_LeftVisScrollCol();
		int const rightCol=ch.get_RightVisScrollCol();
		int const colWidth=ch.get_DiffWidths(beg,end+1);
		ch.delete_cols(styleStore_,beg,end,rcContent.Width());//isWindow?&rcContent:NULL);
		_ASSERT(ch.get_cols()>=0);
		if(isWindow){
			RECT rc;
			GetClientRect(&rc);
			RecalcCellAxisInfo(rc);
		}
		if(0==ch.get_cols()){
			//CCellRange const cr=get_Selection();
			////selection_.SetActiveSelection(0,cr.TopRow(),0,cr.BottomRow());
			//selections_.SetActiveSelection(0,cr.TopRow(),0,cr.BottomRow());
			RECT const rc={rcContent.left,0,rcContent.right,rcContent.bottom};
			if(isWindow){
				InvalidateRect(&rc,FALSE);
				Scroll_ResetHScrollBar();//TRUE,FALSE);
			}
			return TRUE;
		}
		if(isWindow){
			if(beg>=rightCol || end<leftCol){
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask  = SIF_RANGE;
				hScrollBar_.GetScrollInfo(&si);
				si.nMax  -= ch.get_DiffWidths(beg,end+1);
				Scroll_SetHScrollInfo(/*SB_HORZ, */&si);
			}else{
				int const newLeftCol=ch.get_LeftVisScrollCol();
				int const bottom=rcContent.top;
				RECT rcScroll={rcContent.left+ch.get_DiffWidths(leftCol,beg),0,rcContent.right,rcContent.bottom};
				ScrollWindow(-colWidth,0,&rcScroll,&rcScroll);
				if(leftCol!=newLeftCol){
					rcScroll.top=rcContent.top;
					int const xDelta=ch.get_DiffWidths(newLeftCol,leftCol);
					ScrollWindow(xDelta,0,&rcScroll,&rcScroll);
				}
				Scroll_ResetHScrollBar();//TRUE,FALSE);

				rcScroll.bottom=bottom;
				InvalidateRect(&rcScroll,FALSE);
			}
		}
		//CCellRange const oldSelection=get_Selection();
		int const newCols=ch.get_cols();
		int const newRightCol=ch.get_RightVisScrollCol();
		//CCell const activeCell=selection_.GetActiveCell();
		CCell const activeCell=GetActiveCell();
		if(activeCell.col>=newCols)
			//selection_.SetActiveCell(activeCell.row,newCols-1);
			SetActiveCell<false>(activeCell.row,newCols-1);
		RecalcVisibleMergeCells();
		if(isWindow){
			/*
			if(oldSelection.RightCol()>=newCols){
				RECT rc=rcContent;
				rc.top=0;
				InvalidateRect(&rc,FALSE);
			}else if(IsCellFullVisible(rh_.get_TopVisScrollRow(),newRightCol)){
				RECT rc=rcContent;
				RECT rc1=raw_GetCellRect(HEADER_ROW,newRightCol);
				rc.left=rc1.right;
				rc.top=0;
				InvalidateRect(&rc,FALSE);
			}
			CCellRange const cr=get_Selection();
			if(cr.RightCol()>=beg){
				RECT rc=get_RangeRect(&cr);
				InvalidateRect(&rc,FALSE);
			}
			UpdateWindow();
			*/
			Scroll_ResetHScrollBar();
			RedrawWindow();
		}
		return TRUE;
	}
	//insert front
	void Worksheet::insert_rows(int beg,int nCount,bool bFireMsg)
	{
		_ASSERT(beg>=0);
		if(beg<0)
			return;
		_ASSERT(nCount>0);
		if(nCount<=0)
			return;
		if(bFireMsg){
			BOOL bAllowInsert=TRUE;
			SendNotifyMessageToListener(beg,nCount,OCN_ROWSINSERTFRONT,(LPARAM)&bAllowInsert);
			if(!bAllowInsert) 
				return;
		}

		BOOL const isWindow=IsWindow();

		vector<CCellRange> vecCrs;
		MergeCellMgr::OnRowInsertFront(beg,nCount,vecCrs);
		if(isWindow){
			for(vector<CCellRange>::const_iterator it=vecCrs.begin();it!=vecCrs.end();++it)
				InvalidateCellRange(&*it);
		}
		//CellTypeMgr_t::OnRowInsertFront(beg,nCount);
		//GridBorders::OnRowInsertFront(beg,nCount);
		//FontsMgr::OnRowInsertFront(beg,nCount);
		selectionImp_.OnRowInsertFront(beg,nCount);

		cellStores_.OnRowInsertFront(beg,nCount);

		RowHeader& rh=get_RowHeader();
		int const topRow=rh.get_TopVisScrollRow();
		int const bottomRow=rh.get_BottomVisScrollRow();
		int const rows=rh.get_rows();
		_ASSERT(rows>=0);
		RECT const rcCell=raw_GetCellRect(CCell(min(beg,rows-1),get_ColHeader().get_LeftVisScrollCol()));
		//RECT rcContent;
		//if(isWindow)
		//	rcContent=get_CellAxisInfo().get_ScrollRect();
		const CellAxisInfo& cai=get_CellAxisInfo();
		rh.insert_rows(beg,nCount,cai.HClientAxis-cai.HFreezeAxis);
		if(isWindow){
			RECT rc;
			GetClientRect(&rc);
			RecalcCellAxisInfo(rc);
		}
		RecalcVisibleMergeCells();
		if(isWindow){
			RowHeaderAutoFit();
			RedrawWindow();
			/*
			if(!(beg>bottomRow || beg<=topRow)){
				int const yDiff=rh.get_DiffHeights(beg-nCount,beg);
				rcContent.left=0;
				rcContent.top=rcCell.top;
				ScrollWindow(0,yDiff,&rcContent,&rcContent);
			}else if(beg>=rows && bottomRow==rows-1){
				rcContent.left=0;
				rcContent.top=rcCell.bottom;
				InvalidateRect(&rcContent,FALSE);
			}
			if(get_AutoAppendRow() && rows-1<=rh.get_BottomVisScrollRow()){
				InvalidateCell(rows-1,HEADER_COL);
			}
			RowHeaderAutoFit();
			UpdateWindow();
			*/
		}
		if(m_hWnd)
			Scroll_ResetVScrollBar();//FALSE,TRUE);
	}
	//insert front
	void Worksheet::insert_cols(int beg,int nCount,bool bFireMsg)
	{
		_ASSERT(beg>=0);
		if(beg<0)
			return;
		_ASSERT(nCount>0);
		if(nCount<=0)
			return;
		if(bFireMsg){
			BOOL bAllowInsert=TRUE;
			SendNotifyMessageToListener(beg,nCount,OCN_COLSINSERTFRONT,(LPARAM)&bAllowInsert);
			if(!bAllowInsert) 
				return;
		}

		BOOL const isWindow=IsWindow();

		vector<CCellRange> vecCrs;
		MergeCellMgr::OnColInsertFront(beg,nCount,vecCrs);
		if(isWindow){
			for(vector<CCellRange>::const_iterator it=vecCrs.begin();it!=vecCrs.end();++it)
				InvalidateCellRange(&*it);
		}
		//CellTypeMgr_t::OnColInsertFront(beg,nCount);
		//GridBorders::OnColInsertFront(beg,nCount);
		//FontsMgr::OnColInsertFront(beg,nCount);
		cellStores_.OnColInsertFront(beg,nCount);

		ColHeader& ch=get_ColHeader();
		int const leftCol=ch.get_LeftVisScrollCol();
		int const rightCol=ch.get_RightVisScrollCol();
		int const cols=ch.get_cols();
		_ASSERT(cols>=0);
		RECT const rcCell=raw_GetCellRect(CCell(get_RowHeader().get_TopVisScrollRow(),min(beg,cols-1)));
		CRect rcContent;
		if(isWindow)
			rcContent=get_CellAxisInfo().get_ScrollRect();
		ch.insert_cols(beg,nCount,rcContent.Width());//isWindow?&rcContent:NULL);
		RecalcVisibleMergeCells();
		if(isWindow){
			RedrawWindow();
			/*
			if(!(beg>rightCol || beg<=leftCol)){
				int const xDiff=ch.get_DefColWidth()*nCount;//ch.get_DiffWidths(beg-nCount,beg);
				RECT rcScroll={rcCell.right,0,rcContent.right,rcContent.bottom};
				ScrollWindow(xDiff,0,&rcScroll,&rcScroll);
				rcScroll.right=rcScroll.left;
				rcScroll.left=rcCell.left;
				InvalidateRect(&rcScroll,FALSE);
				UpdateWindow();
			}else if(beg>=cols && rightCol==cols-1){
				rcContent.top=0;
				rcContent.left=rcCell.right;
				InvalidateRect(&rcContent,FALSE);
				UpdateWindow();
			}
			*/
			Scroll_ResetHScrollBar();//TRUE,FALSE);
		}
	}
	
	//BOOL Worksheet::draw_HeaderCell(HDC hDC,int col,/*LPCTSTR lpText,int nCount,*/LPCRECT lprc/*,DrawEnvironment& de*/)
	//{
	//	CDCHandle dc(hDC);
	//	ColHeader& ch=get_ColHeader();
	//	TCHAR lpText[MAX_CELLTEXT];
	//	int const nCount=ch.get_text(col,lpText);
	//	DrawEnvironment de;
	//	selection_.SetupColHeaderDE(col,de);
	//	CRect rc=*lprc;
	//	CBrush solidBrush;
	//	solidBrush.Attach(::CreateSolidBrush(de.backColor));
	//	dc.FillRect(&rc,solidBrush);
	//	//画线
	//	dc.MoveTo(rc.left,rc.bottom-1);
	//	dc.LineTo(rc.right-1,rc.bottom-1);
	//	dc.LineTo(rc.right-1,rc.top-1);
	//	if(lpText){
	//		long nFormat=DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	//		::DrawText(hDC,lpText,nCount, rc, nFormat);
	//	}
	//	return TRUE;
	//}

	//delete range [row,end]
	BOOL Worksheet::delete_rows(int beg,int end,bool bFireMsg)
	{
		const int rows=rh_.get_rows();
		_ASSERT(beg>=0);
		_ASSERT(end<rows);
		_ASSERT(end>=beg);
		if(beg<0) beg=0;
		if(end>=rows) end=rows-1;
		if(beg>end)return FALSE;
		if(bFireMsg){
			BOOL bAllowDelete=TRUE;
			SendNotifyMessageToListener(beg,end,OCN_DELETEROWS,(LPARAM)&bAllowDelete);
			if(!bAllowDelete)
				return FALSE;
		}
		BOOL const isWindow=IsWindow();

		vector<CCellRange> vecCrs;
		MergeCellMgr::OnRowDelete(beg,end,vecCrs);
		//CellTypeMgr_t::OnRowDelete(beg,end);
		//GridBorders::OnRowDelete(beg,end);
		//FontsMgr::OnRowDelete(beg,end);
		selectionImp_.OnRowDelete(beg,end);
		cellStores_.OnRowDelete(beg,end);

		const CellAxisInfo& cai=get_CellAxisInfo();
		const RECT rcClient={0,0,cai.VClientAxis,cai.HClientAxis};
		RECT rcContent={0,cai.HFreezeAxis,cai.VClientAxis,cai.HClientAxis};
		if(isWindow){
			for(vector<CCellRange>::const_iterator it=vecCrs.begin();it!=vecCrs.end();++it)
				InvalidateCellRange(&*it);
			//rcContent=get_CellAxisInfo().get_ScrollRect();
		}
		RowHeader& rh=get_RowHeader();
		int const topRow=rh.get_TopVisScrollRow();
		int const bottomRow=rh.get_BottomVisScrollRow();
		int const rowHeight=rh.get_DiffHeights(beg,end+1);
		rh.delete_rows(styleStore_,beg,end,cai.HClientAxis-cai.HFreezeAxis);
		_ASSERT(rh.get_rows()>=0);
		if(0==rh.get_rows()){
			//CCellRange const cr=get_Selection();
			////selection_.SetActiveSelection(cr.LeftCol(),0,cr.RightCol(),0);
			//selections_.SetActiveSelection(cr.LeftCol(),0,cr.RightCol(),0);
			RECT const rc={0,cai.HFreezeAxis,cai.VClientAxis,cai.HClientAxis};//rcContent.top,rcContent.right,rcContent.bottom};
			if(isWindow){
				InvalidateRect(&rc,FALSE);
				Scroll_ResetVScrollBar();//FALSE,TRUE);
			}
			return TRUE;
		}
		if(isWindow){
			if(beg>=bottomRow || end<topRow){
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask  = SIF_RANGE;
				Scroll_GetVScrollInfo(/*SB_VERT,*/&si);
				si.nMax  -= rh.get_DiffHeights(beg,end+1);
				Scroll_SetVScrollInfo(/*SB_VERT,*/ &si);
			}else{
				int const newTopRow=rh.get_TopVisScrollRow();
				int const right=rcContent.left;
				RECT rcScroll={0,rcContent.top+rh.get_DiffHeights(topRow,beg),rcContent.right,rcContent.bottom};
				ScrollWindow(0,-rowHeight,&rcScroll,&rcScroll);
				if(topRow!=newTopRow){
					rcScroll.top=rcContent.top;
					int const yDelta=rh.get_DiffHeights(newTopRow,topRow);
					ScrollWindow(0,yDelta,&rcScroll,&rcScroll);
				}
				Scroll_ResetVScrollBar();//FALSE,TRUE);
				rcScroll.right=right;
				InvalidateRect(&rcScroll,FALSE);
			}
		}

		//CCellRange const oldSelection=get_Selection();
		int const newRows=rh.get_rows();
		int const newBottomRow=rh.get_BottomVisScrollRow();

		//CCell const activeCell=selection_.GetActiveCell();
		CCell const activeCell=GetActiveCell();
		if(activeCell.row>=newRows)
			//selection_.SetActiveCell(newRows-1,activeCell.col);
			SetActiveCell<false>(newRows-1,activeCell.col);

		/*
		if(oldSelection.BottomRow()>=newRows){
			CCellRange newSel=oldSelection;
			newSel.SetBottomRow(newRows-1);
			//if(bottomRow<newSel.TopRow()){
			//	newSel.SetTopRow(bottomRow);
			//}
			//selection_.SetActiveSelection(newSel.LeftCol(),newSel.TopRow(),newSel.RightCol(),newSel.BottomRow());
			selections_.SetActiveSelection(newSel.LeftCol(),newSel.TopRow(),newSel.RightCol(),newSel.BottomRow());
			RECT rc=rcContent;
			rc.left=0;
			if(isWindow)
				InvalidateRect(&rc,FALSE);
		}else if(IsCellFullVisible(newBottomRow,ch_.get_LeftVisScrollCol())){
			RECT rc=rcContent;
			RECT rc1=raw_GetCellRect(newBottomRow,HEADER_COL);
			rc.top=rc1.bottom;
			rc.left=0;
			if(isWindow)
				InvalidateRect(&rc,FALSE);
		}

		CCellRange cr=get_Selection();
		*/
		RecalcVisibleMergeCells();
		if(isWindow){
			/*
			if(cr.BottomRow()>=beg){
				RECT rc=get_RangeRect(&cr);
				InvalidateRect(&rc,FALSE);
			}
			if(get_AutoAppendRow() && newBottomRow==newRows-1){
				InvalidateCell(newBottomRow,HEADER_COL);
			}
			*/
			RowHeaderAutoFit();
			//Scroll_ResetHScrollBar(&rcClient);
			Scroll_ResetVScrollBar(&rcClient);
			//UpdateWindow();
			RedrawWindow();
		}
		return TRUE;
	}
	void Worksheet::put_rows(int rows)
	{
		_ASSERT(rows>=0);
		RowHeader& rh=get_RowHeader();
		int const oldRows=rh.get_rows();
		BOOL bAllow=TRUE;
		SendNotifyMessageToListener(oldRows,rows,OCN_SETROWS,(LPARAM)&bAllow);
		if(bAllow){
			if(rows>oldRows){
				int const nCount=rows-oldRows;
				insert_rows(oldRows,nCount,false);
			}else if(rows<oldRows){
				int const end=oldRows-1;
				delete_rows(rows,end,false);
				selectionImp_.OnRowDelete(rows,end);
			}
			RecalcVisibleMergeCells();
		}
	}
	void Worksheet::put_cols(int cols)
	{
		_ASSERT(cols>=0);
		ColHeader& ch=get_ColHeader();
		int const oldCols=ch.get_cols();
		BOOL bAllow=TRUE;
		SendNotifyMessageToListener(oldCols,cols,OCN_SETCOLS,(LPARAM)&bAllow);
		if(bAllow){
			if(cols>oldCols){
				int const nCount=cols-oldCols;
				insert_cols(oldCols,nCount,false);
			}else if(cols<oldCols){
				int const end=oldCols-1;
				delete_cols(cols,end,false);
				selectionImp_.OnColDelete(cols,end);
			}
			RecalcVisibleMergeCells();
		}
	}
	//nMask为ECellStoreMask的位组合
	CellStorePack Worksheet::GetCellStore(CCell cell,UINT nMask,const CMergeCell* pMerge)const
	{
		_ASSERT(cell.row>=0);
		_ASSERT(cell.col>=0);
		const CellStore* pcs=NULL;
		if(cell.col>=ch_.get_cols() || cell.row>=rh_.get_rows())
			return CellStorePack();
		ECellStorePosition eStorePos=ESP_STYLE_TABLE_OWN;
		//const CMergeCell* pMerge=GetMergeCells(cell);
		if(pMerge){
			eStorePos=ESP_STYLE_MERGE_OWN;
			pcs=&pMerge->GetCellStore();
		}else{
			if(pcs=cellStores_.GetCellStore(cell))
				eStorePos=ESP_STYLE_CELL_OWN;
		}
		if(!pcs){//
			if(pcs=rh_.GetRowStore(cell.row))
				eStorePos=ESP_STYLE_ROW_OWN;
		}
		if(!pcs){//
			if(pcs=ch_.GetColStore(cell.col))
				eStorePos=ESP_STYLE_COL_OWN;
		}
		if(!pcs){
			pcs=&tableDefCellStore_;
			eStorePos=ESP_STYLE_TABLE_OWN;
		}
		return CellStorePack(this,pMerge,cell,*pcs,eStorePos,nMask);//tableDefCellStore_;
	}

	BOOL Worksheet::GetCellText(CCell cell,VARIANT* pval,EGetCellValDistination bRender)const
	{
		const CMergeCell* pMerge=GetMergeCells(cell);
		if(pMerge){
			cell=pMerge->TopLeft();
		}
		if(pWorkbook_->IsOwnerData()){
			//TCHAR buf[MAX_CELLTEXT]={0};
			GetCellTextInfo gcti={pval,bRender};
			HRESULT hr=SendNotifyMessageToListener(cell.row,cell.col,OCN_GETCELLTEXT,(LPARAM)&gcti);
			return SUCCEEDED(hr);
			//int i=0;
			//for(;i!=MAX_CELLTEXT && buf[i];++i)
			//	;
			//if(buf[0]){
			//	val=buf;
			//	return TRUE;
			//}
		}else{
			const CComVariant* pvData=GetCellStore(cell,ECSM_CELLDATA_VALID).GetCellValPack().pVal;
			if(pvData){
				CComVariant v(*pvData);
				v.Detach(pval);
				return VT_EMPTY!=pval->vt;
			}
		}
		return FALSE;
	}

	HRESULT Worksheet::SetCellText(CCell cell,const VARIANT& lpszText,BOOL fRedraw)
	{
		HRESULT hr=S_OK;
		//CCell cell(row,col);
		CMergeCell* pMerge=GetMergeCells(cell);
		if(pMerge){
			cell=pMerge->TopLeft();
		}
		if(pWorkbook_->IsOwnerData()){
			hr=SendNotifyMessageToListener(cell.row,cell.col,OCN_SETCELLTEXT,(LPARAM)&lpszText);
			if FAILED(hr) return hr;
		}else{
			if(pMerge){
				pMerge->SetData(lpszText);
			}else
				cellStores_.SetCellText(cell,lpszText);
		}
		if(fRedraw){
			RECT const rc=pMerge?get_RangeRect(*pMerge):raw_GetCellRect(cell);
			InvalidateRect(&rc,FALSE);
			UpdateWindow();
		}
		return hr;
	}
	void Worksheet::Range_ClearStyles(const CCellRange& cr)
	{
		for(int row=cr.TopRow();row<=cr.BottomRow();++row){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				const CCell cell(row,col);
				CMergeCell* pMerge=GetMergeCells(cell);
				if(pMerge){
					pMerge->ClearStyle(styleStore_);
				}else
					cellStores_.Style_ClearCellStyle(cell);
			}
		}
		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			ch_.style_ClearStyles(styleStore_);
			rh_.style_ClearStyles(styleStore_);
			tableDefCellStore_.ClearStyle(styleStore_);
			_ASSERT(styleStore_.IsNoStyleHasReferenced());
		}else if(HEADER_ROW==cr.TopRow()){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				ch_.Style_ClearColStyle(styleStore_,col);
			}
		}else if(HEADER_COL==cr.LeftCol()){
			for(int row=cr.TopRow();row<=cr.BottomRow();++row)
				rh_.style_ClearRowStyle(styleStore_,row);
		}
	}
	void Worksheet::Range_ClearContents(const CCellRange& cr)
	{
		const CComVariant vEmpty;
		for(int row=cr.TopRow();row<=cr.BottomRow();++row){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				const CCell cell(row,col);
				CMergeCell* pMerge=GetMergeCells(cell);
				if(pMerge){
					pMerge->SetData(vEmpty);
				}else
					cellStores_.SetCellText(cell,vEmpty);
			}
		}
		/*
		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			ch_.ClearContents();
			rh_.ClearContents();
		}else if(HEADER_ROW==cr.TopRow()){
		}else if(HEADER_COL==cr.LeftCol()){
		}
		*/
	}
	void Worksheet::Range_Clear(const CCellRange& cr)
	{
		for(int row=cr.TopRow();row<=cr.BottomRow();++row){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				const CCell cell(row,col);
				CMergeCell* pMerge=GetMergeCells(cell);
				if(pMerge){
					pMerge->Clear(styleStore_);
				}else
					cellStores_.ClearCellStore(cell);
			}
		}
		Range_ClearStyles(cr);
	}

	void Worksheet::Range_SetPattern(const CCellRange& cr,StyleDesc _style,UINT nPatternMask)
	{
		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			StyleDesc style=Style_GetTableStyle();
			Style_CopyPatternProperties(style,_style,nPatternMask);
			Style_SetTableStyle(style);

			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID()){
					AtlTrace(_T("\nCell(%d,%d)"),it->first.row,it->first.col);
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID()){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
			for(int row=0;row<rh_.get_rows();++row){
				if(const StyleID_t sid=rh_.Style_GetRowStyleID(row)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetRowStyle(row,style);
				}
			}
			for(int col=0;col<ch_.get_cols();++col){
				if(const StyleID_t sid=ch_.Style_GetColStyleID(col)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetColStyle(col,style);
				}
			}
		}else if(HEADER_ROW==cr.TopRow()){
			StyleDesc style;
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				style=Style_GetStyleByID(ch_.Style_GetColStyleID(col));
				Style_CopyPatternProperties(style,_style,nPatternMask);
				ch_.style_SetColStyle(styleStore_,col,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.ColInside(it->first.col)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}

		}else if(HEADER_COL==cr.LeftCol()){
			StyleDesc style;
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				style=Style_GetStyleByID(rh_.Style_GetRowStyleID(row));
				Style_CopyPatternProperties(style,_style,nPatternMask);
				rh_.style_SetRowStyle(styleStore_,row,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.RowInside(it->first.row)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
		}else{
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				for(int col=cr.LeftCol();col<=cr.RightCol();++col){
					const CCell cell(row,col);
					StyleDesc style=Style_GetCellStyle(cell);
					Style_CopyPatternProperties(style,_style,nPatternMask);
					Style_SetCellStyle(cell,style);
				}
			}
		}
	}
	void Worksheet::Range_SetAlignment(const CCellRange& cr,StyleDesc _style,UINT nAlignmentSetMask)
	{
		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			StyleDesc style=Style_GetTableStyle();
			Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
			Style_SetTableStyle(style);

			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID()){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID()){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
			for(int row=0;row<rh_.get_rows();++row){
				if(const StyleID_t sid=rh_.Style_GetRowStyleID(row)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetRowStyle(row,style);
				}
			}
			for(int col=0;col<ch_.get_cols();++col){
				if(const StyleID_t sid=ch_.Style_GetColStyleID(col)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetColStyle(col,style);
				}
			}
		}else if(HEADER_ROW==cr.TopRow()){
			StyleDesc style;
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				style=Style_GetStyleByID(ch_.Style_GetColStyleID(col));
				Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
				ch_.style_SetColStyle(styleStore_,col,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.ColInside(it->first.col)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}

		}else if(HEADER_COL==cr.LeftCol()){
			StyleDesc style;
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				style=Style_GetStyleByID(rh_.Style_GetRowStyleID(row));
				Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
				rh_.style_SetRowStyle(styleStore_,row,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.RowInside(it->first.row)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
		}else{
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				for(int col=cr.LeftCol();col<=cr.RightCol();++col){
					const CCell cell(row,col);
					StyleDesc style=Style_GetCellStyle(cell);
					Style_CopyAlignProperties(style,_style,nAlignmentSetMask);
					Style_SetCellStyle(cell,style);
				}
			}
		}
	}
	void Worksheet::Range_SetFont(const CCellRange& cr,const LOGFONT& lf,ColorTable::TIndex clrIdx,UINT nFontSetMask)
	{
		StyleDesc st;st.Font_GetLogFont()=lf;
		st.Font_SetColorIndex(clrIdx);

		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			StyleDesc style=Style_GetTableStyle();
			Style_CopyFontProperties(style,st,nFontSetMask);
			Style_SetTableStyle(style);

			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID()){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID()){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
			for(int row=0;row<rh_.get_rows();++row){
				if(const StyleID_t sid=rh_.Style_GetRowStyleID(row)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetRowStyle(row,style);
				}
			}
			for(int col=0;col<ch_.get_cols();++col){
				if(const StyleID_t sid=ch_.Style_GetColStyleID(col)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetColStyle(col,style);
				}
			}
		}else if(HEADER_ROW==cr.TopRow()){
			StyleDesc style;
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				style=Style_GetStyleByID(ch_.Style_GetColStyleID(col));
				Style_CopyFontProperties(style,st,nFontSetMask);
				ch_.style_SetColStyle(styleStore_,col,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.ColInside(it->first.col)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}

		}else if(HEADER_COL==cr.LeftCol()){
			StyleDesc style;
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				style=Style_GetStyleByID(rh_.Style_GetRowStyleID(row));
				Style_CopyFontProperties(style,st,nFontSetMask);
				rh_.style_SetRowStyle(styleStore_,row,style);
			}
			map<CCell,CellStore> maps;cellStores_.EnumCells(maps);
			for(map<CCell,CellStore>::const_iterator it=maps.begin();it!=maps.end();++it){
				if(const StyleID_t sid=it->second.GetStyleID() && cr.RowInside(it->first.row)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->first,style);
				}
			}
			const vector<CMergeCell>& vMerge=GetMergeCells();
			for(vector<CMergeCell>::const_iterator it=vMerge.begin();it!=vMerge.end();++it){
				if(const StyleID_t sid=it->GetCellStore().GetStyleID() && cr.Inside(*it)){
					style=Style_GetStyleByID(sid);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(it->TopLeft(),style);
				}
			}
		}else{
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				for(int col=cr.LeftCol();col<=cr.RightCol();++col){
					const CCell cell(row,col);
					StyleDesc style=Style_GetCellStyle(cell);
					Style_CopyFontProperties(style,st,nFontSetMask);
					Style_SetCellStyle(cell,style);
				}
			}
		}
	}
	void Worksheet::Range_SetBorder(const CCellRange& cr,CellBorder cellbdr,Border InsideHorizontal,Border InsideVertical,UINT nMask)
	{
		if(HEADER_ROW==cr.TopRow() && HEADER_COL==cr.LeftCol()){
			StyleDesc style=Style_GetTableStyle();
			Borders* pbdrs=style.GetBorders();
			if(xlInsideHorizontal&nMask)
				pbdrs->bottom=pbdrs->top=InsideHorizontal;
			if(xlInsideVertical&nMask)
				pbdrs->right=pbdrs->left=InsideVertical;
			if(xlDiagonalDown&nMask)
				pbdrs->DiagonalLeft=cellbdr.DiagonalLeft;
			if(xlDiagonalUp&nMask)
				pbdrs->DiagonalRight=cellbdr.DiagonalRight;
			if(*pbdrs!=*Style_GetTableStyle().GetBorders()){
				Style_SetTableStyle(style);
			}
		}else if(HEADER_ROW==cr.TopRow()){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				StyleDesc style=Style_GetStyleByID(ch_.Style_GetColStyleID(col));
				Borders* pbdrs=style.GetBorders();
				const Borders bdrs=*pbdrs;
				if((xlEdgeLeft&nMask) && col==cr.LeftCol()){
					pbdrs->left=cellbdr.left;
				}
				if(xlInsideHorizontal&nMask){
					pbdrs->top=pbdrs->bottom=InsideHorizontal;
				}
				if((xlInsideVertical&nMask)&&col>cr.LeftCol()&&col<cr.RightCol()){
					pbdrs->left=pbdrs->right=InsideVertical;
				}
				if((xlEdgeRight&nMask) && col==cr.RightCol())
					pbdrs->right=cellbdr.right;
				//if(xlEdgeBottom&nMask)
				//	pbdrs->bottom=cellbdr.bottom;
				if(xlDiagonalDown&nMask)
					pbdrs->DiagonalLeft=cellbdr.DiagonalLeft;
				if(xlDiagonalUp&nMask)
					pbdrs->DiagonalRight=cellbdr.DiagonalRight;
				if(*pbdrs!=bdrs){
					ch_.style_SetColStyle(styleStore_,col,style);
				}
			}
		}else if(HEADER_COL==cr.LeftCol()){
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				StyleDesc style=Style_GetStyleByID(rh_.Style_GetRowStyleID(row));
				Borders* pbdrs=style.GetBorders();
				const Borders bdrs=*pbdrs;
				if(xlInsideVertical&nMask)
					pbdrs->right=pbdrs->left=InsideVertical;
				if((xlEdgeTop&nMask) && row==cr.TopRow())
					pbdrs->top=cellbdr.top;
				if((xlEdgeBottom&nMask)&&row==cr.BottomRow())
					pbdrs->bottom=cellbdr.bottom;
				if((xlInsideHorizontal&nMask)&&row>cr.TopRow()&&row<cr.BottomRow())
					pbdrs->bottom=pbdrs->top=InsideHorizontal;
				
				if(xlDiagonalDown&nMask)
					pbdrs->DiagonalLeft=cellbdr.DiagonalLeft;
				if(xlDiagonalUp&nMask)
					pbdrs->DiagonalRight=cellbdr.DiagonalRight;
				if(*pbdrs!=bdrs){
					rh_.style_SetRowStyle(styleStore_,row,style);
				}
			}
		}else{
			if(xlEdgeLeft&nMask)
				Range_SetBorderLeft(cr,cellbdr.left);
			if(xlEdgeTop&nMask)
				Range_SetBorderTop(cr,cellbdr.top);
			if(xlEdgeRight&nMask)
				Range_SetBorderRight(cr,cellbdr.right);
			if(xlEdgeBottom&nMask)
				Range_SetBorderBottom(cr,cellbdr.bottom);

			if(xlInsideHorizontal&nMask)
				Range_SetBorderInsideHorizontal(cr,InsideHorizontal);
			if(xlInsideVertical&nMask)
				Range_SetBorderInsideVertical(cr,InsideVertical);
			if(xlDiagonalDown&nMask)
				Range_SetBorderDiagonalDown(cr,cellbdr.DiagonalLeft);
			if(xlDiagonalUp&nMask)
				Range_SetBorderDiagonalUp(cr,cellbdr.DiagonalRight);
		}
	}
	void Worksheet::Range_SetBorderLeft(const CCellRange& cr,Border bdr)
	{
		StyleDesc style;
		for(int row=cr.TopRow();row<=cr.BottomRow();++row){
			const CCell cell(row,cr.LeftCol());
			const CMergeCell* pMerge=GetMergeCells(cell);
			if(pMerge && cell!=pMerge->TopLeft())
				continue;
			if(cell.col>0){
				CellStorePack csp=GetCellStore(cell.LeftCell(),ECSM_STYLE_VALID,pMerge);
				if(ESP_STYLE_CELL_OWN==csp.GetStylePack().eStorePos){
					StyleDesc sd=csp.GetStyle(this);
					if(!sd.GetBorders()->right.IsNull()){
						sd.GetBorders()->right.SetNull();
						Style_SetCellStyle(cell.LeftCell(),sd);
					}
				}
			}
			style=Style_GetCellStyle(cell);
			Borders bdrs=style.GetBorders();
			bdrs.left=bdr;
			if(bdrs!=*style.GetBorders()){
				*style.GetBorders()=bdrs;
				Style_SetCellStyle(cell,style);
			}
		}
	}
	void Worksheet::Range_SetBorderTop(const CCellRange& cr,Border bdr)
	{
		StyleDesc style;
		for(int col=cr.LeftCol();col<=cr.RightCol();++col){
			const CCell cell(cr.TopRow(),col);
			const CMergeCell* pMerge=GetMergeCells(cell);
			if(pMerge && cell!=pMerge->TopLeft())
				continue;
			if(cell.row>0){
				CellStorePack csp=GetCellStore(cell.TopCell(),ECSM_STYLE_VALID,pMerge);
				if(ESP_STYLE_CELL_OWN==csp.GetStylePack().eStorePos){
					StyleDesc sd=csp.GetStyle(this);
					if(!sd.GetBorders()->bottom.IsNull()){
						sd.GetBorders()->bottom.SetNull();
						Style_SetCellStyle(cell.TopCell(),sd);
					}
				}
			}

			style=Style_GetCellStyle(cell);
			Borders bdrs=style.GetBorders();
			bdrs.top=bdr;
			if(bdrs!=*style.GetBorders()){
				*style.GetBorders()=bdrs;
				Style_SetCellStyle(cell,style);
			}
		}
	}
	void Worksheet::Range_SetBorderRight(const CCellRange& cr,Border bdr)
	{
		const int cols=ch_.get_cols();
		StyleDesc style;
		for(int row=cr.TopRow();row<=cr.BottomRow();++row){
			const CCell cell(row,cr.RightCol());
			const CMergeCell* pMerge=GetMergeCells(cell);
			//if(pMerge && cell!=pMerge->TopLeft())
			//	continue;
			if(cell.col<cols-1){
				CellStorePack csp=GetCellStore(cell.RightCell(),ECSM_STYLE_VALID,pMerge);
				if(ESP_STYLE_CELL_OWN==csp.GetStylePack().eStorePos){
					StyleDesc sd=csp.GetStyle(this);
					if(!sd.GetBorders()->left.IsNull()){
						sd.GetBorders()->left.SetNull();
						Style_SetCellStyle(cell.RightCell(),sd);
					}
				}
			}
			style=Style_GetCellStyle(cell);
			Borders bdrs=style.GetBorders();
			bdrs.right=bdr;
			if(bdrs!=*style.GetBorders()){
				*style.GetBorders()=bdrs;
				Style_SetCellStyle(cell,style);
			}
		}
	}
	void Worksheet::Range_SetBorderBottom(const CCellRange& cr,Border bdr)
	{
		const int rows=rh_.get_rows();
		StyleDesc style;
		for(int col=cr.LeftCol();col<=cr.RightCol();++col){
			const CCell cell(cr.BottomRow(),col);
			const CMergeCell* pMerge=GetMergeCells(cell);
			//if(pMerge && cell!=pMerge->TopLeft())
			//	continue;
			if(cell.row<rows-1){
				CellStorePack csp=GetCellStore(cell.BottomCell(),ECSM_STYLE_VALID,pMerge);
				if(ESP_STYLE_CELL_OWN==csp.GetStylePack().eStorePos){
					StyleDesc sd=csp.GetStyle(this);
					if(!sd.GetBorders()->top.IsNull()){
						sd.GetBorders()->top.SetNull();
						Style_SetCellStyle(cell.BottomCell(),sd);
					}
				}
			}
			style=Style_GetCellStyle(cell);
			Borders bdrs=style.GetBorders();
			bdrs.bottom=bdr;
			if(bdrs!=*style.GetBorders()){
				*style.GetBorders()=bdrs;
				Style_SetCellStyle(cell,style);
			}
		}
	}
	void Worksheet::Range_SetBorderInsideHorizontal(const CCellRange& cr,Border bdr)
	{
		StyleDesc style;
		for(int row=cr.TopRow();row<cr.BottomRow();++row){
			for(int col=cr.LeftCol();col<=cr.RightCol();++col){
				const CCell cell(row,col);
				//const CCellRange* pMerge=GetMergeCells(cell);
				//if(pMerge && cell!=pMerge->TopLeft())
				//	continue;
				style=Style_GetCellStyle(cell);
				Borders bdrs=style.GetBorders();
				bdrs.bottom=bdr;
				if(bdrs!=*style.GetBorders()){
					*style.GetBorders()=bdrs;
					Style_SetCellStyle(cell,style);
				}
			}
		}
	}
	void Worksheet::Range_SetBorderInsideVertical(const CCellRange& cr,Border bdr)
	{
		StyleDesc style;
		for(int col=cr.LeftCol();col<cr.RightCol();++col){
			for(int row=cr.TopRow();row<=cr.BottomRow();++row){
				const CCell cell(row,col);
				//const CCellRange* pMerge=GetMergeCells(cell);
				//if(pMerge && cell!=pMerge->TopLeft())
				//	continue;
				style=Style_GetCellStyle(cell);
				Borders bdrs=style.GetBorders();
				bdrs.right=bdr;
				if(bdrs!=*style.GetBorders()){
					*style.GetBorders()=bdrs;
					Style_SetCellStyle(cell,style);
				}
			}
		}
	}
	void Worksheet::Range_SetBorderDiagonalDown(const CCellRange& cr,Border bdr)
	{
	}
	void Worksheet::Range_SetBorderDiagonalUp(const CCellRange& cr,Border bdr)
	{
	}
	CMergeCell* Worksheet::Merge(const int minR,const int minC,const int maxR,const int maxC)
	{
		//render_.SetDirty();
		RowHeader& rh=get_RowHeader();
		ColHeader& ch=get_ColHeader();
		int const rows=rh.get_rows();
		int const cols=ch.get_cols();
		CCellRange crBeMerge(max(0,minR),max(0,minC),min(maxR,rows-1),min(maxC,cols-1));
		MergeCellMgr::AdjustSelection(crBeMerge);
		_ASSERT(crBeMerge.IsNormalize());
		if(crBeMerge.TopLeft()==crBeMerge.RightBottom())
			return NULL;
		const USHORT cellType=GetCellType(crBeMerge.TopLeft()).cellType;
		CComVariant vText;
		const BOOL bHasdata=Range_GetFirstValidData(crBeMerge,vText);

		//清理风格和数据
		//CComVariant* pdata=cellStores_.GetCellData(pMerge->TopLeft());
		StyleDesc style=Style_GetCellStyle(crBeMerge.TopLeft());
		Borders bdrs;
		Border bdr;
		if(Range_GetLeftBorder(crBeMerge,bdr))
			bdrs.left=bdr;
		if(Range_GetTopBorder(crBeMerge,bdr))
			bdrs.top=bdr;
		if(Range_GetRightBorder(crBeMerge,bdr))
			bdrs.right=bdr;
		if(Range_GetBottomBorder(crBeMerge,bdr))
			bdrs.bottom=bdr;
		*style.GetBorders()=bdrs;

		Range_Clear(crBeMerge);

		CMergeCell* pMerge=MergeCellMgr::Merge(crBeMerge.TopRow(),crBeMerge.LeftCol(),crBeMerge.BottomRow(),crBeMerge.RightCol());
		if(!pMerge)
			return NULL;
		_ASSERT(pMerge->IsNormalize());
		pMerge->SetData(vText);
		if(!style.IsNull())
			pMerge->SetStyle(styleStore_,style);
			//pMerge->cellStore.nStyleID=styleStore_.add_style(style);
		//pMerge->cellStore.cellType=cellType;
		pMerge->SetCellType((ECellType)cellType);
		//:~清理风格和数据

		RecalcVisibleMergeCells();
		if(IsWindow()){
			InvalidateCellRange(pMerge);
		}
		if(m_hWnd){
			const Selections oss=selectionImp_;
			selectionImp_.OnMergeCells(pMerge->TopRow(),pMerge->LeftCol(),pMerge->BottomRow(),pMerge->RightCol());
			selectionImp_.InvalidateContent(GetActiveCell(),oss);
			UpdateWindow();
		}else
			selectionImp_.OnMergeCells(pMerge->TopRow(),pMerge->LeftCol(),pMerge->BottomRow(),pMerge->RightCol());
		return pMerge;
	}

	CCellRange Worksheet::Unmerge(int minR,int minC,int maxR,int maxC)
	{
		//render_.SetDirty();
		//CCellRange const oldSel=get_Selection();
		vector<CMergeCell> vecUnmerged;
		CCellRange cr=MergeCellMgr::Unmerge(minR,minC,maxR,maxC,vecUnmerged);
		_ASSERT(cr.IsNormalize());
		for(vector<CMergeCell>::iterator it=vecUnmerged.begin();it!=vecUnmerged.end();++it){
			if(VT_EMPTY!=it->GetCellStore().GetData().vt){
				CComVariant val;
				it->DetachData(&val);
				cellStores_.AttachCellData(it->TopLeft(),val);
			}
			//将左上角的风格扩散开来
			const CMergeCell& merge=*it;
			_ASSERT(merge.IsNormalize());
			const StyleID_t sid=merge.GetCellStore().GetStyleID();//cellStores_.Style_GetCellStyleID(_cr.TopLeft());
			if(sid){
				StyleDesc style=Style_GetStyleByID(sid);
				const Borders bdrs=style.GetBorders();
				style.GetBorders()->clear();
				for(int row=merge.TopRow();row<=merge.BottomRow();++row){
					for(int col=merge.LeftCol();col<=merge.RightCol();++col){
						const CCell cell(row,col);
						_ASSERT(row>=0);
						_ASSERT(col>=0);
						Borders* pbdrs=style.GetBorders();
						if(col==merge.LeftCol()){
							pbdrs->left=bdrs.left;
						}
						if(col==merge.RightCol())
							pbdrs->right=bdrs.right;
						if(row==merge.TopRow())
							pbdrs->top=bdrs.top;
						if(row==merge.BottomRow())
							pbdrs->bottom=bdrs.bottom;
						if(!pbdrs->empty()){
							Style_SetCellStyle(cell,style);
							pbdrs->clear();
						}else{
							cellStores_.Style_SetCellStyle(cell,style);
						}
					}
				}
			}
			//:~将左上角的风格扩散开来
		}
		if(cr.Inside(GetActiveCell()))
			SetActiveCell<false>(cr.TopLeft());
		RecalcVisibleMergeCells();
		//RECT rc=get_RangeRect(&cr);
		//InvalidateRect(&rc);
		if(m_hWnd){
			InvalidateCellRange(&cr);
			UpdateWindow();
		}
		/*
		if(oldSel.Inside(cr.TopRow(),cr.LeftCol())
			&& oldSel.Inside(cr.BottomRow(),cr.RightCol()))
		{
			SetActiveCell(oldSel.TopRow(),oldSel.LeftCol());
			//selection_.SetActiveCell(oldSel.TopRow(),oldSel.LeftCol());
			put_Selection(oldSel.LeftCol(),oldSel.TopRow(),oldSel.RightCol(),oldSel.BottomRow());
			RecalcVisibleMergeCells();
			//UpdateWindow();
		}else{
			RecalcVisibleMergeCells();
			if(IsWindow()){
				RECT const rc=get_RangeRect(cr.TopRow(),cr.LeftCol(),cr.BottomRow(),cr.RightCol());
				InvalidateRect(&rc,FALSE);
				RECT const rContent=get_ScrollRect();
				RECT rc1;
				if(get_ShowColHeader()){
					SetRect(&rc1,rc.left,0,rc.right,rContent.top);
					InvalidateRect(&rc1,FALSE);
				}
				if(get_ShowRowHeader()){
					SetRect(&rc1,0,rc.top,rContent.left,rc.bottom);
					InvalidateRect(&rc1,FALSE);
				}
				UpdateWindow();
			}
		}
		*/
		return cr;
	}
	HRESULT Workbook::ImportFromXml(EXmlType nType,LPCTSTR Filename,LPTSTR outFileName,LPARAM lParam)
	{
		HRESULT hr=S_OK;
		CString strFile(Filename);
		if(strFile.IsEmpty()){
			xm::ui::CxmFileDialog dlg(TRUE,_T("xml文件(*.xml)\0*.xml\0\0"));
			if(IDOK==dlg.DoModal()){
				GetFileBaseName(dlg.m_ofn.lpstrFile);
				lstrcat(dlg.m_ofn.lpstrFile,_T(".xml"));
				if(outFileName)
					lstrcpy(outFileName,dlg.m_ofn.lpstrFile);
				strFile=dlg.m_ofn.lpstrFile;
				//CWaitCursor wc;
				//return XmlMyCell().load(this,dlg.m_ofn.lpstrFile);
			}else{
				return S_OK;
			}
		}else{
			if(outFileName)
				lstrcpy(outFileName,Filename);
		}
		CWaitCursor wc;
		CComPtr<MSXML2::IXMLDOMDocument>	   pXmlDoc;
		if FAILED(hr=pXmlDoc.CoCreateInstance(MSXML2::CLSID_DOMDocument))
			return hr;
		if(EXT_UNKNOWN==nType){
			VARIANT_BOOL bLoad;
			if FAILED(hr=pXmlDoc->load(CComVariant(strFile),&bLoad))
				return hr;
			if(!bLoad)
				return AtlReportError(CLSID_NULL,L"载入文件失败！",GUID_NULL,E_FAIL);
			BOOL bValidateXml=FALSE;
			CComPtr<MSXML2::IXMLDOMNode> pN;
			pXmlDoc->selectSingleNode(L"mycell/DocumentProperties/Version",&pN);
			if(pN){
				nType=EXT_MYCELL1_0;
				//return XmlMyCell().load(this,strFile);
			}else{
				pXmlDoc->selectSingleNode(L"Workbook/DocumentProperties/Version",&pN);
				if(pN){
					nType=EXT_EXCEL;
					//return XmlExcel().load(this,strFile);
				}
			}
		}
		switch(nType)
		{
		case EXT_MYCELL1_0:_ASSERT(FALSE);//return XmlMyCell().load(pCurSheet_,strFile);
		case EXT_EXCEL:return XmlExcel().load(this,strFile);//,(int)lParam);
		}
		return AtlReportError(CLSID_NULL,L"无法读取的文件格式！",GUID_NULL,E_FAIL);
	}
	HRESULT Workbook::ExportToXml(EXmlType nType,LPCTSTR Filename,LPTSTR outFileName)const
	{
		if(outFileName){
			outFileName[0]=0;
		}
		HRESULT hr=S_OK;
		//RowHeader const& rh=get_RowHeader();
		//int const rows=rh.get_rows();
		//if(rows>65534){
		//	return AtlReportError(CLSID_NULL,L"不能导出，记录太长。");
		//}
		CString strFile(Filename);
		if(strFile.IsEmpty()){
			CString strFilter(_T("MyCell文件(*.xml)\0*.xml\0\0"));
			switch(nType)
			{
				case EXT_EXCEL:strFilter=_T("Excel文件(*.xml)\0*.xml\0\0");break;
			}
			xm::ui::CxmFileDialog dlg(FALSE,strFilter);
			if(IDOK==dlg.DoModal()){
				GetFileBaseName(dlg.m_ofn.lpstrFile);
				lstrcat(dlg.m_ofn.lpstrFile,_T(".xml"));
				//if(outFileName)
				//	lstrcpy(outFileName,dlg.m_ofn.lpstrFile);
				strFile=dlg.m_ofn.lpstrFile;
				//CWaitCursor wc;
				//return XmlMyCell().save(this,dlg.m_ofn.lpstrFile);
			}else{
				return S_OK;
			}
		}/*else{
			if(outFileName)
				lstrcpy(outFileName,Filename);
			CWaitCursor wc;
			return XmlMyCell().save(this,Filename);
		}
		*/
		if(outFileName)
			lstrcpy(outFileName,strFile);
		CWaitCursor wc;
		switch(nType)
		{
		case EXT_EXCEL:return XmlExcel().save(this,strFile);
		}
		_ASSERT(FALSE);
		return E_NOTIMPL;
		//return XmlMyCell().save(this,strFile);
	}
	/*
	HRESULT Worksheet::ImportFromExcel(LPCTSTR Filename,LPTSTR outFileName)
	{
		if(Filename==NULL){
			xm::ui::CxmFileDialog dlg(FALSE,_T("Excel文件(*.xml)\0*.xml\0\0"));
			if(IDOK==dlg.DoModal()){
				GetFileBaseName(dlg.m_ofn.lpstrFile);
				lstrcat(dlg.m_ofn.lpstrFile,_T(".xml"));
				if(outFileName)
					lstrcpy(outFileName,dlg.m_ofn.lpstrFile);
				CWaitCursor wc;
				return XmlExcel().load(this,dlg.m_ofn.lpstrFile);
			}else{
				return S_OK;
			}
		}else{
			if(outFileName)
				lstrcpy(outFileName,Filename);
			CWaitCursor wc;
			return XmlExcel().load(this,Filename);
		}

	}
	*/
	/*
	HRESULT Worksheet::ExportToExcel(LPCTSTR Filename,LPTSTR outFileName)const
	{
		if(outFileName){
			outFileName[0]=0;
		}
		HRESULT hr=S_OK;
		RowHeader const& rh=get_RowHeader();
		int const rows=rh.get_rows();
		if(rows>65534){
			return AtlReportError(CLSID_NULL,L"不能导出，记录太长。");
		}
		if(Filename==NULL){
			xm::ui::CxmFileDialog dlg(FALSE,_T("Excel文件(*.xml)\0*.xml\0\0"));
			if(IDOK==dlg.DoModal()){
				GetFileBaseName(dlg.m_ofn.lpstrFile);
				lstrcat(dlg.m_ofn.lpstrFile,_T(".xml"));
				if(outFileName)
					lstrcpy(outFileName,dlg.m_ofn.lpstrFile);
				CWaitCursor wc;
				return XmlExcel().save(this,dlg.m_ofn.lpstrFile);
			}else{
				return S_OK;
			}
		}else{
			if(outFileName)
				lstrcpy(outFileName,Filename);
			CWaitCursor wc;
			return XmlExcel().save(this,Filename);
		}
	}
	*/
	//vector<CCell>::iterator FindCell(vector<CCell>& vec,CCell cell)
	//{
	//	for(vector<CCell>::iterator it=vec.begin();it!=vec.end();++it)
	//	{
	//		if(it->row==cell.row && it->col==cell.col)
	//			return it;
	//	}
	//	return vec.end();
	//}
	
	void GetDirtySelectionBound(Worksheet* pSheet,CCellRange cr,set<CCell>& vec)
	{
		_ASSERT(cr.IsNormalize());
		const ESelectionMode esm=pSheet->GetWorkbook()->get_SelectionMode();
		switch(esm)
		{
		case ESM_SINGLECELL:
		case ESM_SINGLELINE:
		case ESM_MULTILINE:
			return;//这几种选择方式无选择框或则选择框在单元格内部
			//故无需添加附加的无效区
		}
		const RowHeader& rh=pSheet->get_RowHeader();
		const ColHeader& ch=pSheet->get_ColHeader();
		int const leftCol=ch.get_LeftVisScrollCol();
		int const rightCol=ch.get_RightVisScrollCol();
		int const topRow=rh.get_TopVisScrollRow();
		int const bottomRow=rh.get_BottomVisScrollRow();

		CCell cell;
		for(int r=max(topRow,cr.TopRow()-1);r<=min(cr.BottomRow()+1,bottomRow);++r){
			if(cr.LeftCol()>0){
				cell.set(r,cr.LeftCol()-1);
				vec.insert(cell);
			}
			{
				cell.set(r,cr.LeftCol());
				vec.insert(cell);
			}
			if(cr.RightCol()<rightCol){
				cell.set(r,cr.RightCol()+1);
				vec.insert(cell);
			}
			if(cr.RightCol()<=rightCol){
				cell.set(r,cr.RightCol());
				vec.insert(cell);
			}
		}
		for(int c=max(leftCol,cr.LeftCol());c<=min(cr.RightCol(),rightCol);++c){
			if(cr.TopRow()>0){
				cell.set(cr.TopRow()-1,c);
				vec.insert(cell);
			}
			{
				cell.set(cr.TopRow(),c);
				vec.insert(cell);
			}
			if(cr.BottomRow()<rh.get_rows()-1){
				cell.set(cr.BottomRow()+1,c);
				vec.insert(cell);
			}
			if(cr.BottomRow()<rh.get_rows()){
				cell.set(cr.BottomRow(),c);
				vec.insert(cell);
			}
		}
	}
		/*
	//扩大脏区域
	void Worksheet::ExpandDirtyActiveSelection(const CCellRange* poldSelection,const CCellRange* pnewSelection)
	{
		const ESelectionMode esm=get_SelectionMode();
		if(!(ESM_MULTISELECTION==esm || ESM_SINGLESELECTION==esm))
			return ;
		//if(ESM_MULTISELECTION==esm)
		//	return;
		_ASSERT(poldSelection!=NULL);
		if(poldSelection==NULL)
			return;
		 CCellRange oldSelection=*poldSelection;
		 CCellRange newSelection=*pnewSelection;
		EMouseMode const mouseMode=get_MouseMode();
		if(MOUSE_NOTHING==mouseMode && oldSelection==newSelection)
			return;
		//oldSelection.Normalize();
		//newSelection.Normalize();
		_ASSERT(oldSelection.IsNormalize());
		_ASSERT(newSelection.IsNormalize());
		AdjustSelection(oldSelection);
		AdjustSelection(newSelection);
		set<CCell> vec;
		GetDirtySelectionBound(this,oldSelection,vec);
		GetDirtySelectionBound(this,newSelection,vec);

		RowHeader& rh=get_RowHeader();
		ColHeader& ch=get_ColHeader();
		int const topRow=rh.get_TopVisScrollRow();
		int const bottomRow=rh.get_BottomVisScrollRow();
		int const leftCol=ch.get_LeftVisScrollCol();
		int const rightCol=ch.get_RightVisScrollCol();

		RECT rc;
		for(set<CCell>::const_iterator it=vec.begin();it!=vec.end();++it){

			//左上外角
			if(it->row==oldSelection.TopRow()-1 
				&& it->row==newSelection.TopRow()-1
				&& it->col>=oldSelection.LeftCol() && it->col<=oldSelection.RightCol()
				&& it->col>=newSelection.LeftCol() && it->col<=newSelection.RightCol())
				continue;

			//左内外边
			if(
				oldSelection.LeftCol()==newSelection.LeftCol()
				&& it->row>oldSelection.TopRow() && it->row<oldSelection.BottomRow()
				&& it->row>newSelection.TopRow() && it->row<newSelection.BottomRow()
				&& it->col!=min(oldSelection.RightCol(),newSelection.RightCol())
				&&(
					it->col==oldSelection.LeftCol()-1
					||it->col==oldSelection.LeftCol()
				)
				&& !(it->col==leftCol && (it->col==oldSelection.LeftCol()||it->col==oldSelection.LeftCol()-1))
				&& !(it->col==leftCol+1 && it->col==oldSelection.LeftCol())
				)
				continue;

			if(oldSelection.LeftCol()==newSelection.LeftCol()
				&& oldSelection.RightCol()==newSelection.RightCol()
				&& (oldSelection.TopRow()==newSelection.TopRow() && it->row <min(oldSelection.BottomRow(),newSelection.BottomRow())
				|| oldSelection.BottomRow()==newSelection.BottomRow() && it->row >max(oldSelection.TopRow(),newSelection.TopRow()))
				)
				continue;


			if(oldSelection.LeftCol()==newSelection.LeftCol()
				&& oldSelection.TopRow()==newSelection.TopRow()
				&& it->row==oldSelection.TopRow()
				&& (it->col==oldSelection.LeftCol()-1 || it->col==oldSelection.LeftCol())
				&& it->row<oldSelection.BottomRow()
				&& it->col!=min(oldSelection.RightCol(),newSelection.RightCol())
				&& it->row!=min(oldSelection.BottomRow(),newSelection.BottomRow())
				)
				continue;

			if(it->row==oldSelection.TopRow()-1
				&& it->col==oldSelection.LeftCol()-1
				&& oldSelection.TopRow()==newSelection.TopRow()
				&& oldSelection.LeftCol()==newSelection.LeftCol())
				continue;


			//下内外边
			if(
				oldSelection.BottomRow()==newSelection.BottomRow()
				&& it->col>oldSelection.LeftCol() && it->col<oldSelection.RightCol()
				&& it->col>newSelection.LeftCol() && it->col<newSelection.RightCol()
				&& (it->row==oldSelection.BottomRow()+1||it->row==oldSelection.BottomRow())
				&& it->row!=min(oldSelection.BottomRow(),newSelection.BottomRow())
				)
				continue;

			if(oldSelection.TopRow()==newSelection.TopRow()
				&& oldSelection.RightCol()==newSelection.RightCol()
				&& it->row==oldSelection.TopRow()-1
				&& it->col==oldSelection.RightCol()+1
				)
				continue;

			//右内外边
			if(oldSelection.RightCol()==newSelection.RightCol()
				&& it->row>oldSelection.TopRow() && it->row<oldSelection.BottomRow()
				&& it->row>newSelection.TopRow() && it->row<newSelection.BottomRow()
				&& it->col!=oldSelection.LeftCol()
				&& it->col!=newSelection.LeftCol()
				&&(it->col==oldSelection.RightCol()+1||it->col==oldSelection.RightCol())
				)
				continue;
			rc=raw_GetCellRect(it->row,it->col);
			InvalidateRect(&rc,FALSE);
		}
		//:~test--扩大脏区域
	}
		*/

	//----------------------------2007-4-13
	void Worksheet::ColumnAutoFit(int col)
	{
		CWaitCursor wc;
		CClientDC dc(m_hWnd);
		RECT const rcContent=get_CellAxisInfo().get_ScrollRect();
		ColumnAutoFitPolicy afp(this,col);
		int const nWidth=GetAutoFit<ColumnAutoFitPolicy>(&afp,dc,rcContent.right-rcContent.left-1);
		if(nWidth>0){
			ColHeader& ch=get_ColHeader();
			int const oldWidth=ch.get_ColWidth(col);
			put_ColWidth(col,nWidth,TRUE);
		}
	}
	//ECellState Worksheet::get_CellState(int row,int col)const
	//{
	//	//todo...
	//	return ECS_OUTOF_SELECTION;
	//}

	//--------------------------------------------------------------//
	//return 行号小于row的第一个行高大于0的行号
	//如果无满足条件的行则返回row本身。
	//--------------------------------------------------------------//
	int Worksheet::GetPreRow(int row)const
	{
		const RowHeader& rh=get_RowHeader();
		for(int i=row-1;i>=0;--i){
			if(rh.get_RowHeight(i)>0) return i;
		}
		return row;
	}

	//--------------------------------------------------------------//
	//return 行号大于row的第一个行高大于0的行号
	//如果无满足条件的行则返回row本身。
	//--------------------------------------------------------------//
	int Worksheet::GetNextRow(int row)const
	{
		const RowHeader& rh=get_RowHeader();
		int const rows=rh.get_rows();
		for(int i=row+1;i<rows;++i){
			if(rh.get_RowHeight(i)>0) return i;
		}
		return row;
	}

	//--------------------------------------------------------------//
	//return 列号小于col的第一个列宽大于0的列号
	//如果无满足条件的列则返回col本身。
	//--------------------------------------------------------------//
	int Worksheet::GetPreCol(int col)const
	{
		//return col>0?--col:0;
		const ColHeader& ch=get_ColHeader();
		for(int i=col-1;i>=0;--i){
			if(ch.get_ColWidth(i)>0) return i;
		}
		return col;
	}

	//--------------------------------------------------------------//
	//return 列号大于col的第一个列宽大于0的列号
	//如果无满足条件的列则返回col本身。
	//--------------------------------------------------------------//
	int Worksheet::GetNextCol(int col)const
	{
		//return col<cols?++col:cols;
		const ColHeader& ch=get_ColHeader();
		int const cols=ch.get_cols();
		for(int i=col+1;i<cols;++i){
			if(ch.get_ColWidth(i)>0) return i;
		}
		return col;
	}

	//--------------------------------------------------------------//
	//return col前一个可以滚动到的列
	//遍历中应跳过列宽<=0的列
	//如果无满足条件的列则返回col本身。
	//--------------------------------------------------------------//
	int Worksheet::GetPreScrollableCol(int col)const 
	{
		const ColHeader& ch=get_ColHeader();
		int nLeftScrollableCol=ch.get_LeftScrollableCol();
		for(int i=col-1;i>=nLeftScrollableCol;--i){
			if(ch.get_ColWidth(i)>0) return i;
		}
		return col;
	}
	//--------------------------------------------------------------//
	//return row前一个可以滚动到的行
	//遍历中应跳过行高<=0的行
	//如果无满足条件的行则返回row本身。
	//--------------------------------------------------------------//
	int Worksheet::GetPreScrollableRow(int row)const 
	{
		const RowHeader& rh=get_RowHeader();
		const int nTopScrolllableRow=rh.get_TopScrollableRow();
		for(int i=row-1;i>=nTopScrolllableRow;--i){
			if(rh.get_RowHeight(i)>0) return i;
		}
		return row;
	}
	EGridPart Worksheet::QueryPart(int row,int col)const
	{
		if(HEADER_ROW==row){
			return HEADER_COL==col?EGP_HEADER_ALTHOGONAL:EGP_HEADER_COLS;
		}else if(HEADER_COL==col){
			return EGP_HEADER_ROWS;
		}else{
			const RowHeader& rh=get_RowHeader();
			const ColHeader& ch	=get_ColHeader();
			const bool bInRowFreeze=(row>=rh.get_TopScrollableRow() && row<=rh.get_BottomVisScrollRow())?true:false;
			const bool bInColFreeze=(col>=ch.get_FreezeLeftCol()&& col<=ch.get_FreezeRightCol())?true:false;
			if(bInRowFreeze && bInColFreeze){
				return EGP_FREEZE_ALTHOGONAL;
			}
			const bool bInRowScroll=(row>=rh.get_TopVisScrollRow()&&row<=rh.get_BottomVisScrollRow())?true:false;
			const bool bInColScroll=(col>=ch.get_LeftVisScrollCol()&&col<=ch.get_RightVisScrollCol())?true:false;
			if(bInRowScroll&&bInColScroll){
				return EGP_SCROLLABLE;
			}else if(bInRowFreeze && bInColScroll){
				return EGP_FREEZE_ROWS;
			}else if(bInColFreeze && bInRowScroll){
				return EGP_FREEZE_COLS;
			}else
				return EGP_UNKNOWN;
		}
	}
	/*
	CCellRange Worksheet::get_Selection()const
	{
		CCellRange cr;
		vector<CCellRange> vec;
		pSelection_->get_Range(vec);
		if(!vec.empty()){
			cr=vec[0];
		}
		AdjustSelection(cr);
		return cr;
	}
	*/
	void Worksheet::RecalcVisibleMergeCells()
	{
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		if(rh.get_TopVisScrollRow()>rh.get_BottomVisScrollRow()
			||ch.get_LeftVisScrollCol()>ch.get_RightVisScrollCol())
		{
			visMergeCells_.clear();
			return;
		}
		const int rows=rh.get_rows();
		const int cols=ch.get_cols();
		const int nLeftVisScrollCol=ch.get_LeftVisScrollCol();
		const int nTopVisScrollRow=rh.get_TopVisScrollRow();
		const int nBottomVisScrollRow=rh.get_BottomVisScrollRow();
		const int nRightVisScrollCol=ch.get_RightVisScrollCol();
		const int nLeftScrollableCol=ch.get_LeftScrollableCol();
		const int nTopScrollableRow=rh.get_TopScrollableRow();
		const int nFreezeBottomRow=rh.get_FreezeBottomRow();
		const int nFreezeTopRow=rh.get_FreezeTopRow();
		const int nFreezeLeftCol=ch.get_FreezeLeftCol();
		const int nFreezeRightCol=ch.get_FreezeRightCol();
		CCellRange cr;
		cr.set(max(nTopVisScrollRow-1,nTopScrollableRow),
			max(nLeftVisScrollCol-1,nLeftScrollableCol),
			min(nBottomVisScrollRow+1,rows-1),
			min(nRightVisScrollCol+1,cols-1));
		cr.Normalize();
		visMergeCells_.ReCalculate(cr);

		VisibleMergeCellMgr vmc(this);
		BOOL const bShowFreezeRows=rh.IsFreezeRowVisible();//nFreezeBottomRow>=0;
		BOOL const bShowFreezeCols=ch.IsFreezeColVisible();//nFreezeRightCol>=0;
		if(bShowFreezeRows && bShowFreezeCols){
			cr.set(nFreezeTopRow,nFreezeLeftCol,nFreezeBottomRow,nFreezeRightCol);
			vmc.ReCalculate(cr);			
			visMergeCells_.merge(vmc);
		}
		if(bShowFreezeRows){
			cr.set(nFreezeTopRow,max(nLeftVisScrollCol-1,nLeftScrollableCol),rh.get_FreezeBottomRow(),min(nRightVisScrollCol+1,cols-1));
			if(cr.IsNormalize()){
				vmc.ReCalculate(cr);			
				visMergeCells_.merge(vmc);
			}
		}
		if(bShowFreezeCols){
			cr.set(max(nTopVisScrollRow-1,nTopScrollableRow),nFreezeLeftCol,min(nBottomVisScrollRow+1,rows-1),nFreezeRightCol);
			if(cr.IsNormalize()){
				vmc.ReCalculate(cr);			
				visMergeCells_.merge(vmc);
			}
		}
	}
	void Worksheet::Freeze(int nFreezeTopRow,int nFreezeBottomRow,int nFreezeLeftCol,int nFreezeRightCol)
	{
		RowHeader& rh=get_RowHeader();
		ColHeader& ch=get_ColHeader();
		//if(nFreezeBottomRow<=0 && nFreezeRightCol<=0)
		//	return;
		if(rh.get_rows()>1){
			rh.put_FreezeTopRow(nFreezeTopRow);
			rh.put_FreezeBottomRow(nFreezeBottomRow);
		}
		if(ch.get_cols()>1){
			ch.put_FreezeLeftCol(nFreezeLeftCol);
			ch.put_FreezeRightCol(nFreezeRightCol);
		}
		{
			RECT rcCli;GetClientRect(&rcCli);
			RecalcCellAxisInfo(rcCli,CCAI_CALC_ALL);
		}
		//const CRect rc=get_CellAxisInfo().get_ScrollRect();
		const CellAxisInfo& cai=get_CellAxisInfo();
		//CRect rc(0,cai.HFreezeAxis,cai.VClientAxis,cai.HClientAxis);
		//if(rh.get_TopVisScrollRow()<=nFreezeBottomRow)
		{
			const int oTopVisScrollRow=nFreezeBottomRow+1;
			const int oBottomVisScrollRow=rh.get_BottomVisScrollRow();
			rh.put_TopVisScrollRow(oTopVisScrollRow,cai.HClientAxis-cai.HFreezeAxis);
			OnVisibleRowChanged(oTopVisScrollRow,oBottomVisScrollRow);//,nTopVisScrollRow,rh.get_BottomVisScrollRow());
		}
		//if(ch.get_LeftVisScrollCol()<=nFreezeRightCol)
		{
			const int oLeftVisScrollCol=nFreezeRightCol+1;
			const int oRightVisScrollCol=ch.get_RightVisScrollCol();
			ch.put_LeftVisScrollCol(oLeftVisScrollCol,cai.VClientAxis-cai.VFreezeAxis);//&rc);
			OnVisibleColChanged(oLeftVisScrollCol,oRightVisScrollCol);//,nLeftVisScrollCol,ch.get_RightVisScrollCol());
		}
		if(IsWindow()){
			Scroll_ResetHScrollBar();//TRUE,TRUE);
			Scroll_ResetVScrollBar();
			RedrawWindow();
		}
	}
	void Worksheet::UnFreeze()
	{
		if(IsFreezed()){
			RowHeader& rh=get_RowHeader();
			ColHeader& ch=get_ColHeader();
			const bool bFreezeRowVisible=rh.IsFreezeRowVisible();
			const bool bFreezeColVisible=ch.IsFreezeColVisible();
			int newVisTopRow=rh.get_TopVisScrollRow();
			int newVisLeftCol=ch.get_LeftVisScrollCol();
			if(bFreezeRowVisible){
				newVisTopRow=rh.get_FreezeTopRow();
				rh.put_FreezeTopRow(-1);
				rh.put_FreezeBottomRow(-1);
			}
			if(bFreezeColVisible){
				newVisLeftCol=ch.get_FreezeLeftCol();
				ch.put_FreezeLeftCol(-1);
				ch.put_FreezeRightCol(-1);
			}
			{
				RECT rcCli;GetClientRect(&rcCli);
				RecalcCellAxisInfo(rcCli,CCAI_CALC_ALL);
			}
			//const CRect rc=get_CellAxisInfo().get_ScrollRect();
			const CellAxisInfo& cai=get_CellAxisInfo();
			//rh.put_TopVisScrollRow(newVisTopRow,cai.HClientAxis-cai.HFreezeAxis);
			//ch.put_LeftVisScrollCol(newVisLeftCol,cai.VClientAxis-cai.VFreezeAxis);//&rc);
			const int oTopVisScrollRow=newVisTopRow;
			const int oBottomVisScrollRow=rh.get_BottomVisScrollRow();
			rh.put_TopVisScrollRow(oTopVisScrollRow,cai.HClientAxis-cai.HFreezeAxis);
			OnVisibleRowChanged(oTopVisScrollRow,oBottomVisScrollRow);//,nTopVisScrollRow,rh.get_BottomVisScrollRow());
			const int oLeftVisScrollCol=newVisLeftCol;
			const int oRightVisScrollCol=ch.get_RightVisScrollCol();
			ch.put_LeftVisScrollCol(oLeftVisScrollCol,cai.VClientAxis-cai.VFreezeAxis);//&rc);
			OnVisibleColChanged(oLeftVisScrollCol,oRightVisScrollCol);//,nLeftVisScrollCol,ch.get_RightVisScrollCol());

			if(IsWindow()){
				Scroll_ResetHScrollBar();//TRUE,TRUE);
				Scroll_ResetVScrollBar();
				RedrawWindow();
			}
		}
	}
	bool Worksheet::GetFreezeAlthogonalPart(CCellRange& cr)const
	{
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		if(rh.IsFreezeRowVisible() && ch.IsFreezeColVisible()){
			cr.SetTopLeft(rh.get_FreezeTopRow(),ch.get_FreezeLeftCol());
			cr.SetRightBottom(rh.get_FreezeBottomRow(),ch.get_FreezeRightCol());
			return true;
		}
		cr.SetNull();
		return false;
	}
	bool Worksheet::GetFreezeRowsPart(CCellRange& cr)const
	{
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		if(rh.IsFreezeRowVisible()){
			cr.SetTopLeft(rh.get_FreezeTopRow(),ch.get_LeftVisScrollCol());
			cr.SetRightBottom(rh.get_FreezeBottomRow(),ch.get_RightVisScrollCol());
			return true;
		}
		cr.SetNull();
		return false;
	}
	bool Worksheet::GetFreezeColsPart(CCellRange& cr)const
	{
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		if(ch.IsFreezeColVisible()){
			cr.SetTopLeft(rh.get_TopVisScrollRow(),ch.get_FreezeLeftCol());
			cr.SetRightBottom(rh.get_BottomVisScrollRow(),ch.get_FreezeRightCol());
			return true;
		}
		cr.SetNull();
		return false;
	}
	bool Worksheet::GetScrollPart(CCellRange& cr)const
	{
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		int const topVisScrollRow=rh.get_TopVisScrollRow();
		int const leftVisScrollCol=ch.get_LeftVisScrollCol();
		int const bottomVisScrollRow=rh.get_BottomVisScrollRow();
		int const rightVisScrollCol=ch.get_RightVisScrollCol();
		if(topVisScrollRow<=bottomVisScrollRow && leftVisScrollCol<=rightVisScrollCol){
			cr.SetTopLeft(topVisScrollRow,leftVisScrollCol);
			cr.SetRightBottom(bottomVisScrollRow,rightVisScrollCol);
			return true;
		}
		cr.SetNull();
		return false;
	}
	//pcr可以跨冻结线的四个区
	RECT Worksheet::get_RangeRect(CCellRange const& pcr)const
	{
		_ASSERT(pcr.IsNormalize());
		CCellRange cr,cr1,cr2,cr3,cr4;
		GetFreezeAlthogonalPart(cr1);
		GetFreezeRowsPart(cr2);
		GetFreezeColsPart(cr3);
		GetScrollPart(cr4);
		/*
		if(!cr1.IsNull())
			cr1.Inflate(1,1);
		if(!cr2.IsNull())
			cr2.Inflate(1,1);
		if(!cr3.IsNull())
			cr3.Inflate(1,1);
		if(!cr4.IsNull())
			cr4.Inflate(1,1);
			*/
		pcr.GetInterset(cr1,cr1);
		pcr.GetInterset(cr2,cr2);
		pcr.GetInterset(cr3,cr3);
		pcr.GetInterset(cr4,cr4);
		RECT rc,rc1,rc2;
		if(!cr1.IsNull() && !cr4.IsNull()){
			rc1=raw_GetCellRect(cr1.TopLeft());
			rc2=raw_GetCellRect(cr4.RightBottom());
			SetRect(&rc,rc1.left,rc1.top,rc2.right,rc2.bottom);
			return rc;
		}else if(!cr1.IsNull() && !cr2.IsNull()){
			rc1=raw_GetCellRect(cr1.TopLeft());
			rc2=raw_GetCellRect(cr2.RightBottom());
			SetRect(&rc,rc1.left,rc1.top,rc2.right,rc2.bottom);
			return rc;
		}else if(!cr1.IsNull() && !cr3.IsNull()){
			rc1=raw_GetCellRect(cr1.TopLeft());
			rc2=raw_GetCellRect(cr3.RightBottom());
			SetRect(&rc,rc1.left,rc1.top,rc2.right,rc2.bottom);
			return rc;
		}else if(!cr2.IsNull() && !cr4.IsNull()){
			rc1=raw_GetCellRect(cr2.TopLeft());
			rc2=raw_GetCellRect(cr4.RightBottom());
			SetRect(&rc,rc1.left,rc1.top,rc2.right,rc2.bottom);
			return rc;
		}else if(!cr3.IsNull() && !cr4.IsNull()){
			rc1=raw_GetCellRect(cr3.TopLeft());
			rc2=raw_GetCellRect(cr4.RightBottom());
			SetRect(&rc,rc1.left,rc1.top,rc2.right,rc2.bottom);
			return rc;
		}else if(!cr1.IsNull()){
			return get_RangeRectPlain(cr1);
		}else if(!cr2.IsNull()){
			return get_RangeRectPlain(cr2);
		}else if(!cr3.IsNull()){
			return get_RangeRectPlain(cr3);
		}else if(!cr4.IsNull()){
			return get_RangeRectPlain(cr4);
		}
		::SetRectEmpty(&rc);
		return rc;
	}
	//--------------------------------------------------------------------------------//
	//自动剪裁，即你可以传一个很大的范围，方法
	//会自动和由冻结线分割的四个可见区间剪裁
	//仅使边框部分无效
	//--------------------------------------------------------------------------------//
	void Worksheet::InvalidateCellRangeBorder(CCellRange const* pcr)
	{
		RECT const rc=get_RangeRect(*pcr);//pcr->first.row,pcr->first.col,pcr->second.row,pcr->second.col);
		//InvalidateRect(&rc,FALSE);
		RECT _rc={rc.left,rc.top,rc.left+4,rc.bottom};
		InvalidateRect(&_rc,FALSE);
		SetRect(&_rc,rc.right-4,rc.top,rc.right,rc.bottom);
		InvalidateRect(&_rc,FALSE);
		SetRect(&_rc,rc.left,rc.top,rc.right,rc.top+4);
		InvalidateRect(&_rc,FALSE);
		SetRect(&_rc,rc.left,rc.bottom-4,rc.right,rc.bottom);
		InvalidateRect(&_rc,FALSE);
	}

	//----------------------------------------------------------------------//
	//nCalcMask为如下掩码位的组合
	//	CCAI_CALC_HAXIS=1<<0,					计算垂直轴线的掩码位
	//CCAI_CALC_VAXIS=1<<1,					计算水平轴线的掩码位
	//CCAI_CALC_HREMAINAXIS=1<<2,	计算HRemainAxis轴线的掩码位
	//CCAI_CALC_VREMAINAXIS=1<<3,	计算VRemainAxis轴线的掩码位
	//----------------------------------------------------------------------//
	const CellAxisInfo& Worksheet::RecalcCellAxisInfo(const RECT& rcli,UINT nCalcMask)//bool bCalcVAxis,bool bCalcHAxis,bool bCalcRemainAxis)
	{
		AtlTrace(_T("\n"));
		AtlTrace(_T("Worksheet::RecalcCellAxisInfo %d"),GetTickCount());
		const RowHeader& rh=get_RowHeader();
		const ColHeader& ch=get_ColHeader();
		const int rows=rh.get_rows();
		const int cols=ch.get_cols();
		if(CCAI_CALC_VAXIS & nCalcMask){
			cellAxisInfo_.VHeaderAxis	=short(rcli.left+(get_ShowRowHeader()?rh.get_width():0));
			cellAxisInfo_.VFreezeAxis	=short(ch.IsFreezeColVisible()?(cellAxisInfo_.VHeaderAxis+ch.get_DiffWidths(ch.get_FreezeLeftCol(),ch.get_FreezeRightCol()+1)):cellAxisInfo_.VHeaderAxis);
			cellAxisInfo_.VClientAxis	=short(rcli.right);
		}
		if(CCAI_CALC_VREMAINAXIS&nCalcMask){
			//如果最后一列不完全可见，则VRemainAxis>VClientAxis，但其值不准确
			if(ch.get_RightVisScrollCol()==cols-1){
				//cellAxisInfo_.VRemainAxis=short(min(rcli.right,cellAxisInfo_.VFreezeAxis+ch.get_DiffWidths(max(0,ch.get_LeftVisScrollCol()),ch.get_RightVisScrollCol()+1)));
				cellAxisInfo_.VRemainAxis=short(min(rcli.right-rcli.left+1,cellAxisInfo_.VFreezeAxis+ch.get_DiffWidths(max(0,ch.get_LeftVisScrollCol()),ch.get_RightVisScrollCol()+1)));
			}else
				cellAxisInfo_.VRemainAxis =short(rcli.right+1);
		}
		
		if(CCAI_CALC_HAXIS&nCalcMask){
			cellAxisInfo_.HHeaderAxis	=short(rcli.top+(get_ShowColHeader()?ch.get_height():0));
			cellAxisInfo_.HFreezeAxis	=short(rh.IsFreezeRowVisible()?(cellAxisInfo_.HHeaderAxis+rh.get_DiffHeights(rh.get_FreezeTopRow(),rh.get_FreezeBottomRow()+1)):cellAxisInfo_.HHeaderAxis);
			cellAxisInfo_.HClientAxis		=short(rcli.bottom);
		}
		if(CCAI_CALC_HREMAINAXIS&nCalcMask){
			//如果最后一行不完全可见，则HRemainAxis>HClientAxis，但其值不准确
			if(rh.get_BottomVisScrollRow()==rows-1)
				cellAxisInfo_.HRemainAxis=short(min(rcli.bottom-rcli.top+1,cellAxisInfo_.HFreezeAxis+rh.get_DiffHeights(max(0,rh.get_TopVisScrollRow()),rh.get_BottomVisScrollRow()+1)));
			else
				cellAxisInfo_.HRemainAxis=short(rcli.bottom+1);
		}
		_ASSERT(cellAxisInfo_.HRemainAxis>=0);
		_ASSERT(cellAxisInfo_.VRemainAxis>=0);
		return cellAxisInfo_;
	}
	void Worksheet::clear()
	{
		ActiveCell<Worksheet>::SetActiveCell(-1,-1);
		selectionImp_.clear();
		cellStores_.clear();
		ClearMergeCells(styleStore_);
		get_RowHeader().clear(styleStore_);
		get_ColHeader().clear(styleStore_);
		visMergeCells_.clear();
		put_AutoAppendRow(FALSE);
		tableDefCellStore_.ClearStyle(styleStore_);
		_ASSERT(styleStore_.IsNoStyleHasReferenced());
	}
	/*
	//如果cell在合并单元格中
	pair<StyleID_t,ECellStorePosition> Worksheet::Style_GetCellStyleID(CCell cell)const
	{
		const CellStoreDef cs=GetCellStore(cell);
		return cs.CellStore()
		//const CCellRange* pMerge=GetMergeCells(cell);
		//if(pMerge)
		//	cell=pMerge->TopLeft();

		pair<StyleID_t,ECellStorePosition> pr;
		pr.first=cellStores_.Style_GetCellStyleID(cell);
		if(pr.first){
			pr.second=ESP_STYLE_CELL_OWN;
			return pr;
		}
		const RowHeader& rh=get_RowHeader();
		pr.first=rh.Style_GetRowStyleID(cell.row);
		if(pr.first){
			pr.second=ESP_STYLE_ROW_OWN;
			return pr;
		}
		const ColHeader& ch=get_ColHeader();
		pr.first=ch.Style_GetColStyleID(cell.col);
		if(pr.first){
			pr.second=ESP_STYLE_COL_OWN;
			return pr;
		}
		pr.first=tableDefCellStore_.nStyleID;
		if(pr.first){
			pr.second=ESP_STYLE_TABLE_OWN;
			return pr;
		}
		pr.second=ESP_STYLE_DEFAULT;
		return pr;
	}
	*/
	int Worksheet::ShowSetupDialog(HWND hWndParent,int flag)
	{
		BorderPP::BorderPropertyPage page;
		FontPropertyPage fontPage;
		AlignmentPropertyPage	alignPage;
		PatternPropertyPage		patternPage;
		mycell::CPropertySheet dlg;
		dlg.put_Title(_T("单元格格式"));
		dlg.AddPage(&page);
		dlg.AddPage(&fontPage);
		dlg.AddPage(&alignPage);
		dlg.AddPage(&patternPage);
		VARIANT_BOOL ok;
		dlg.EditProperties(hWndParent,this,&ok);
		return ok?IDOK:IDCANCEL;
	}
	void Worksheet::ClearCellTypes()
	{
		get_RowHeader().ClearCellTypes();
		get_ColHeader().ClearCellTypes();
		MergeCellClearCellTypes();
		cellStores_.ClearCellTypes();
		tableDefCellStore_.SetCellType(ECT_NONE);
	}
		void Worksheet::Style_ClearStyles()
		{
			//StyleStore& ss=pWorkbook_->GetStyles();
			ClearMergeCellStyles(styleStore_);
			cellStores_.Style_ClearStyles();
			get_RowHeader().style_ClearStyles(styleStore_);
			get_ColHeader().style_ClearStyles(styleStore_);
			styleStore_.ResetDefStyle();
			tableDefCellStore_.ClearStyle(styleStore_);
			_ASSERT(styleStore_.IsNoStyleHasReferenced());
		}
	LRESULT Worksheet::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//if(!pSheet_)
		//	return 0;
		CELLHITTESTINFO chi;
		chi.pt.x=GET_X_LPARAM(lParam);
		chi.pt.y=GET_Y_LPARAM(lParam);
		HitTest(chi);

		const CCellRange* pMerge=GetMergeCells(chi.cell);
		if(pMerge){
			chi.cell=pMerge->TopLeft();
		}

		//if(GetFocus()==m_hWnd)
		pWorkbook_->GetSheetWnd().ChangeCursor(chi);
		if(WM_RBUTTONDOWN==uMsg){
			selectionImp_.OnRButtonDown(chi,wParam);
		}
		const int nNotifyCode=OCN_BASE+uMsg;
		SendNotifyMessageToListener(chi.cell.row,chi.cell.col,nNotifyCode,(LPARAM)&chi,&bHandled);
		if(bHandled)
			return 0;
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			if(CHTC_ON_DRAGCORNER&chi.flags){
				TrackDragCorner(chi);
			}else if(CHTC_ON_DRAGBOUNDS&chi.flags){
			}else if(CHTC_ON_COLDIVIDER&chi.flags){
				TrackDragColDivider(chi);
			}else if(CHTC_ON_ROWDIVIDER&chi.flags){
				TrackDragRowDivider(chi);
			}else{
				const ECellType ctype=(ECellType)GetCellType(chi.cell).cellType;
				CComPtr<ICellEditor> pEditor=pWorkbook_->GetTypeFactory().GetEditor(ctype);
				if(pEditor){
					CComVariant vText;
					GetCellText(chi.cell,&vText,EGCVD_EDIT);
					pEditor->OnLButtonDown(chi.pt.x,chi.pt.y,chi.cell,&vText,chi.flags,(VARIANT_BOOL*)&bHandled);
				}
				if(!bHandled)
					selectionImp_.OnLButtonDown(chi,wParam);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				const ECellType ctype=(ECellType)GetCellType(chi.cell).cellType;
				CComPtr<ICellEditor> pEditor=pWorkbook_->GetTypeFactory().GetEditor(ctype);
				if(pEditor){
					CComVariant vText;
					GetCellText(chi.cell,&vText,EGCVD_EDIT);
					pEditor->OnLButtonDblClick(chi.pt.x,chi.pt.y,chi.cell,&vText,chi.flags);
				}
			}break;
		case WM_MOUSEMOVE:
			if(HasFocus(m_hWnd))
				Tooltip_ChangeCellToolTip(chi);
			break;
		}
		return 0;
	}
	void Worksheet::Refresh()
	{
		RECT rc;
		GetClientRect(&rc);
		if(rc.left>=rc.right || rc.top>=rc.bottom)
			return;
		RecalcCellAxisInfo(rc,CCAI_CALC_ALL);
		const CellAxisInfo& cai=get_CellAxisInfo();
		put_LeftVisScrollCol(get_ColHeader().get_LeftVisScrollCol(),&cai);
		put_TopVisScrollRow(get_RowHeader().get_TopVisScrollRow(),&cai);
		Scroll_ResetHScrollBar(&rc);
		Scroll_ResetVScrollBar(&rc);
		RedrawWindow();
	}
	//void Worksheet::Draw(HDC hDC,	GridRenderer& render,const CRgnLight& rgnClip)const
	//{
	//	const RowHeader& rh=get_RowHeader();
	//	const ColHeader& ch=get_ColHeader();
	//	RECT rcli;
	//	GetClientRect(&rcli);
	//	DrawEnvironment de;
	//	const CellAxisInfo& cai=get_CellAxisInfo();
	//	render.draw(this,&cai,hDC,&rcli,&rgnClip,de);
	//}
	//StyleID_t Worksheet::Style_SetRowStyle(int row,const StyleDesc& style)
	//{
	//	RowHeader& rh=get_RowHeader();
	//	if(Style_GetTableStyle()==style){
	//		rh.style_ClearRowStyle(styleStore_,row);
	//		return 0;
	//	}else{
	//		/*
	//		if(row>0){
	//			StyleID_t sid=rh.Style_GetRowStyleID(row-1);
	//			if(!sid)
	//				sid=tableDefCellStore_.GetStyleID();
	//			StyleDesc sd=styleStore_.get_Style(sid);
	//			if(!sd.GetBorders()->bottom.IsNull() && sd.GetBorders()->bottom!=style.GetBorders()->top){
	//				sd.GetBorders()->bottom=style.GetBorders()->top;
	//				rh.style_SetRowStyle(styleStore_,row-1,sd);
	//			}
	//		}
	//		const int rows=rh.get_rows();
	//		if(row<rows-1){
	//			StyleID_t sid=rh.Style_GetRowStyleID(row+1);
	//			if(!sid)
	//				sid=tableDefCellStore_.GetStyleID();
	//			StyleDesc sd=styleStore_.get_Style(sid);
	//			if(!sd.GetBorders()->top.IsNull() && sd.GetBorders()->top!=style.GetBorders()->bottom){
	//				sd.GetBorders()->top=style.GetBorders()->bottom;
	//				rh.style_SetRowStyle(styleStore_,row+1,sd);
	//			}
	//		}
	//		*/
	//		return rh.style_SetRowStyle(styleStore_,row,style);
	//		//return get_RowHeader().style_SetRowStyle(styleStore_,row,&style);
	//	}
	//}

	StyleID_t Worksheet::Style_SetColStyle(int col,const StyleDesc& style)
	{
		ColHeader& ch=get_ColHeader();
		if(Style_GetTableStyle()==style){
			ch.Style_ClearColStyle(styleStore_,col);
			return 0;
		}else{
			/*
			if(col>0){
				StyleID_t sid=ch.Style_GetColStyleID(col-1);
				if(!sid)
					sid=tableDefCellStore_.GetStyleID();
				StyleDesc sd=styleStore_.get_Style(sid);
				if(!sd.GetBorders()->right.IsNull() && sd.GetBorders()->right!=style.GetBorders()->left){
					sd.GetBorders()->right=style.GetBorders()->left;
					ch.style_SetColStyle(styleStore_,col-1,&sd);
				}
			}
			const int cols=ch.get_cols();
			if(col<cols-1){
				StyleID_t sid=ch.Style_GetColStyleID(col+1);
				if(!sid)
					sid=tableDefCellStore_.GetStyleID();
				StyleDesc sd=styleStore_.get_Style(sid);
				if(!sd.GetBorders()->left.IsNull() && sd.GetBorders()->left!=style.GetBorders()->right){
					sd.GetBorders()->left=style.GetBorders()->right;
					ch.style_SetColStyle(styleStore_,col+1,&sd);
				}
			}
			*/
			return ch.style_SetColStyle(styleStore_,col,style);
		}
	}

	//如果cell在合并单元格中
	StyleID_t Worksheet::Style_SetCellStyle(CCell cell,const StyleDesc& style)
	{
		CMergeCell* pMerge=GetMergeCells(cell);
		if(pMerge){
			cell=pMerge->TopLeft();
			//styleStore_.DecrementRefCount(pMerge->cellStore.nStyleID);
		}
		StyleID_t sid=get_RowHeader().Style_GetRowStyleID(cell.row);
		if(!sid)
			sid=get_ColHeader().Style_GetColStyleID(cell.col);
		if(!sid)
			sid=tableDefCellStore_.GetStyleID();
		if(Style_GetStyleByID(sid)==style){
			if(pMerge){
				pMerge->ClearStyle(styleStore_);
			}else
				Style_ClearCellStyle(cell);
			return 0;
		}
		if(pMerge){
			//return pMerge->cellStore.nStyleID=styleStore_.add_style(style);
			return pMerge->SetStyle(styleStore_,style);
		}

		/*
		const StyleDesc& oldStyle=Style_GetCellStyle(cell);
		const Borders* pbdrsN=style.GetBorders();
		const Borders* pbdrsO=oldStyle.GetBorders();
		if(pbdrsN->left!=pbdrsO->left && cell.col>0){
			const bottomRow=pMerge?pMerge->BottomRow():cell.row;
			for(int row=cell.row;row<=bottomRow;++row){
				const CCell cll(row,cell.col-1);
				StyleDesc sd=Style_GetCellStyle(cll);
				if(!sd.GetBorders()->right.IsNull() && sd.GetBorders()->right!=pbdrsN->left){
					sd.GetBorders()->right.SetNull();
					cellStores_.Style_SetCellStyle(cll,sd);
				}
			}
		}
		if(pbdrsN->top!=pbdrsO->top && cell.row>0){
			const int rightCol=pMerge?pMerge->RightCol():cell.col;
			for(int col=cell.col;col<=rightCol;++col){
				const CCell cll(cell.row-1,col);
				StyleDesc sd=Style_GetCellStyle(cll);
				if(!sd.GetBorders()->bottom.IsNull() && sd.GetBorders()->bottom!=pbdrsN->top){
					sd.GetBorders()->bottom.SetNull();
					cellStores_.Style_SetCellStyle(cll,sd);
				}
			}
		}
		if(pbdrsN->right!=pbdrsO->right){
			const int col=pMerge?pMerge->RightCol()+1:cell.col+1;
			if(col<get_ColHeader().get_cols()-1){
				const bottomRow=pMerge?pMerge->BottomRow():cell.row;
				for(int row=cell.row;row<=bottomRow;++row){
					const CCell cll(row,col);
					StyleDesc sd=Style_GetCellStyle(cll);
					if(!sd.GetBorders()->left.IsNull()&&sd.GetBorders()->left!=pbdrsN->right){
						sd.GetBorders()->left.SetNull();
						cellStores_.Style_SetCellStyle(cll,sd);
					}
				}
			}
		}
		if(pbdrsN->bottom!=pbdrsO->bottom){
			const int row=pMerge?pMerge->BottomRow()+1:cell.row+1;
			if(row<get_RowHeader().get_rows()-1){
				const int rightCol=pMerge?pMerge->RightCol():cell.col;
				for(int col=cell.col;col<=rightCol;++col){
					const CCell cll(row,col);
					StyleDesc sd=Style_GetCellStyle(cll);
					if(!sd.GetBorders()->top.IsNull() && sd.GetBorders()->top!=pbdrsN->bottom){
						sd.GetBorders()->top.SetNull();
						cellStores_.Style_SetCellStyle(cll,sd);
					}
				}
			}
		}
		*/
		return cellStores_.Style_SetCellStyle(cell,style);
	}
	//枚举存储有CellStore的单元格，pClipRange为剪裁范围
	//pClipRange可以为NULL。
	void Worksheet::EnumCells(vector<pair<CCell,const CellStore*> >& vec,const CCellRange* pClipRange)const
	{
		map<CCell,CellStore> mapCells;cellStores_.EnumCells(mapCells);
		if(!pClipRange){
			for(map<CCell,CellStore>::const_iterator i=mapCells.begin();i!=mapCells.end();++i){
				vec.push_back(make_pair(i->first,&i->second));
			}
		}else{
			for(map<CCell,CellStore>::const_iterator i=mapCells.begin();i!=mapCells.end();++i){
				if(pClipRange->Inside(i->first))
					vec.push_back(make_pair(i->first,&i->second));
			}
		}
	}
	//-------------------------------------------------------------------//
	//取单元格cell的上Border
	//1,如果本格上Border有效，则返回有效Border
	//2,如果上格下Border有效，则返回有效Border
	//3,如果在合并单元格又不在上端则没有上Border,直接返回
	//4,如果在合并单元格的上端又不是左上角，则
	//取左上角的上Border，如果有效则返回
	//5,如果上格在合并单元格中，则取上格合并单元格
	//的左上角的下Border返回
	//-------------------------------------------------------------------//
	//Border Worksheet::Style_GetTopBorder(CCell cell)const
	//{
	//	typedef GetBordersPolicyNormal<Worksheet> TPolicy;
	//	TPolicy policy(this);
	//	BorderGetter<TPolicy> bg(policy);
	//	return bg.GetTopBorder(this,cell);
	//}
	/*
	Border Worksheet::Style_GetBottomBorder(CCell cell)const
	{
		Border bdr=Style_GetCellStyle(cell).GetBorders()->bottom;
		if(!bdr.IsNull())
			return bdr;
		if(cell.row<get_RowHeader().get_rows()-1){
			bdr=Style_GetCellStyle(CCell(cell.row+1,cell.col)).GetBorders()->top;
			if(!bdr.IsNull())
				return bdr;
		}
		const CCellRange* pMerge=GetMergeCells(cell);
		//if(pMerge && cell.row<pMerge->BottomRow())
		//	return bdr;
		if(pMerge && cell.row==pMerge->BottomRow()){
			bdr=Style_GetCellStyle(pMerge->TopLeft()).GetBorders()->bottom;
		}
		return bdr;
	}
*/
	const CComVariant* Worksheet::Range_GetFirstValidData(const CCellRange& cr)const
	{
		_ASSERT(cr.IsNormalize());
		const int leftCol=max(0,cr.LeftCol());
		for(int row=max(0,cr.TopRow());row<=cr.BottomRow();++row){
			for(int col=leftCol;col<=cr.RightCol();++col){
				const CCell cell(row,col);
				const CellStorePack cs=GetCellStore(cell,ECSM_CELLDATA_VALID);
				if(VT_EMPTY!=cs.GetCellValPack().pVal->vt){
					return cs.GetCellValPack().pVal;
				}
			}
		}
		return NULL;
	}
	BOOL Worksheet::Range_GetFirstValidData(const CCellRange& cr,CComVariant& val)
	{
		_ASSERT(cr.IsNormalize());
		const int leftCol=max(0,cr.LeftCol());
		for(int row=max(0,cr.TopRow());row<=cr.BottomRow();++row){
			for(int col=leftCol;col<=cr.RightCol();++col){
				const CCell cell(row,col);
				CMergeCell* pMerge=GetMergeCells(cell);
				const CComVariant* pData=pMerge?&pMerge->GetCellStore().GetData():cellStores_.GetCellData(cell);
				if(pData && VT_EMPTY!=pData->vt){
					val=*pData;
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	//取范围cr的上Border，如果范围内的上Border不一致则返回FALSE
	BOOL Worksheet::Range_GetTopBorder(const CCellRange& cr,Border& bdr)const
	{
		BOOL bFind=FALSE;
		const int nTopRow=max(0,cr.TopRow());
		for(int col=cr.LeftCol();col<=cr.RightCol();++col){
			const CCell cell(nTopRow,col);
			/*
			const CCellRange* pMerge=GetMergeCells(cell);
			if(pMerge){
				if(cell.row>0){
					const Border _bdr=Style_GetBottomBorder(cell,pMerge);
					if(_bdr.IsNull()){
						if(!bFind){
							bdr=_bdr;
							bFind=TRUE;
						}else{
							if(_bdr!=bdr)
								return FALSE;
						}
						continue;
					}
				}

			}
			*/
			const Border _bdr=Style_GetTopBorder(cell);
			if(!bFind){
				bdr=_bdr;
				bFind=TRUE;
			}else{
				if(_bdr!=bdr)
					return FALSE;
			}
		}
		return bFind;
	}
	//取范围cr的下Border，如果范围内的下Border不一致则返回FALSE
	BOOL Worksheet::Range_GetBottomBorder(const CCellRange& cr,Border& bdr)const
	{
		BOOL bFind=FALSE;
		const int nBottomRow=max(0,cr.BottomRow());
		for(int col=cr.LeftCol();col<=cr.RightCol();++col){
			CCell cell(nBottomRow,col);
			//const CCellRange* pMerge=GetMergeCells(cell);
			//if(pMerge && col!=pMerge->LeftCol()){
			//	col=pMerge->RightCol()+1;
			//	continue;
			//}
			const Border _bdr=Style_GetBottomBorder(cell);
			if(!bFind){
				bdr=_bdr;
				bFind=TRUE;
			}else{
				if(_bdr!=bdr)
					return FALSE;
			}
		}
		return bFind;
	}
	//取范围cr的左Border，如果范围内的左Border不一致则返回FALSE
	BOOL Worksheet::Range_GetLeftBorder(const CCellRange& cr,Border& bdr)const
	{
		BOOL bFind=FALSE;
		const int nLeftCol=max(0,cr.LeftCol());
		for(int row=max(0,cr.TopRow());row<=cr.BottomRow();++row){
			const CCell cell(row,nLeftCol);
			//const CCellRange* pMerge=GetMergeCells(cell);
			//if(pMerge && row!=pMerge->TopRow()){
			//	row=pMerge->BottomRow()+1;
			//	continue;
			//}
			const Border _bdr=Style_GetLeftBorder(cell);
			if(!bFind){
				bdr=_bdr;
				bFind=TRUE;
			}else{
				if(_bdr!=bdr)
					return FALSE;
			}
			;
		}
		return bFind;
	}
	//取范围cr的右Border，如果范围内的右Border不一致则返回FALSE
	BOOL Worksheet::Range_GetRightBorder(const CCellRange& cr,Border& bdr)const
	{
		BOOL bFind=FALSE;
		for(int row=max(0,cr.TopRow());row<=cr.BottomRow();++row){
			CCell cell(row,cr.RightCol());
			//const CCellRange* pMerge=GetMergeCells(cell);
			//if(pMerge && row!=pMerge->TopRow()){
			//	row=pMerge->BottomRow()+1;
			//	continue;
			//}
			const Border _bdr=Style_GetRightBorder(cell);
			if(!bFind){
				bdr=_bdr;
				bFind=TRUE;
			}else{
				if(_bdr!=bdr)
					return FALSE;
			}
		}
		return bFind;
	}
	BOOL Worksheet::Range_GetInsideHorizontalBorder(const CCellRange& cr,Border& bdr)const
	{
		bool bFirst=true;
		Border _bdr;
		for(int row=cr.TopRow();row<cr.BottomRow();++row){
			const CCellRange _cr(row,cr.LeftCol(),row,cr.RightCol());
			if(!Range_GetBottomBorder(_cr,bdr))
				return FALSE;
			if(bFirst){
				_bdr=bdr;
				bFirst=false;
			}else if(bdr!=_bdr)
				return FALSE;
		}
		return TRUE;
	}
	BOOL Worksheet::Range_GetInsideVerticalBorder(const CCellRange& cr,Border& bdr)const
	{
		bool bFirst=true;
		Border _bdr;
		for(int col=cr.LeftCol();col<cr.RightCol();++col){
			const CCellRange _cr(cr.TopRow(),col,cr.BottomRow(),col);
			if(!Range_GetRightBorder(_cr,bdr))
				return FALSE;
			if(bFirst){
				_bdr=bdr;
				bFirst=false;
			}else if(bdr!=_bdr)
				return FALSE;
		}
		return TRUE;
	}

	Border Worksheet::Style_GetTopBorder(CCell cell)const
	{
		Border bdr;
		const CellStorePack csPack=GetCellStore(cell,ECSM_STYLE_VALID);
		const CMergeCell* pMerge=csPack.GetMerge();//GetMergeCells(cell);
		//const CMergeCell* pMerge=GetMergeCells(cell);
		if(pMerge){
			cell.row=pMerge->TopRow();
		}
		const CCell _cell(GetPreRow(cell.row),cell.col);
		if(_cell.row==cell.row){
			bdr=csPack.GetStyle(this).GetBorders()->top;
		}else{
			const CellStorePack csPackTop=GetCellStore(_cell,ECSM_STYLE_VALID);
			const StylePack sp=csPack.GetStylePack();
			const StylePack spTop=csPackTop.GetStylePack();
			if(sp.eStorePos>spTop.eStorePos){
				bdr=Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
			}else{
				bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
			}
		}
		return bdr;
		//typedef GetBordersPolicyNormal<Worksheet> TPolicy;
		//TPolicy policy(this);
		//BorderGetter<TPolicy> bg(policy);
		//return bg.GetTopBorder(this,cell);
	}
	Border Worksheet::Style_GetLeftBorder(CCell cell)const
	{
		_ASSERT(cell.row>=0);
		_ASSERT(cell.col>=0);
		Border bdr;
		const CellStorePack csPack=GetCellStore(cell,ECSM_STYLE_VALID);
		const CMergeCell* pMerge=csPack.GetMerge();//GetMergeCells(cell);
		if(pMerge)
			cell.col=pMerge->LeftCol();
		const CCell _cell(cell.row,GetPreCol(cell.col));
		if(_cell.col==cell.col){
			bdr=csPack.GetStyle(this).GetBorders()->left;
		}else{
			const CellStorePack csPackLeft=GetCellStore(_cell,ECSM_STYLE_VALID);
			const StylePack sp=csPack.GetStylePack();
			const StylePack spLeft=csPackLeft.GetStylePack();
			if(sp.eStorePos>spLeft.eStorePos){
				bdr=Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
			}else{
				bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
			}
		}
		return bdr;
	}
	Border Worksheet::Style_GetRightBorder(CCell cell)const
	{
		Border bdr;
		const CellStorePack csPack=GetCellStore(cell,ECSM_STYLE_VALID);
		const CMergeCell* pMerge=csPack.GetMerge();
		if(pMerge)
			cell.col=pMerge->RightCol();
		const CCell _cell(cell.row,GetNextCol(cell.col));
		if(_cell.col==cell.col){
			bdr=csPack.GetStyle(this).GetBorders()->right;
		}else{
			const CellStorePack csPackRight=GetCellStore(_cell,ECSM_STYLE_VALID);
			const StylePack sp=csPack.GetStylePack();
			StylePack spRight=csPackRight.GetStylePack();
			if(sp.eStorePos>spRight.eStorePos){
				bdr=Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
				if(bdr.IsNull()){
					bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
				}
			}else{
				bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
			}
		}
		return bdr;
	}
	Border Worksheet::Style_GetBottomBorder(CCell cell)const
	{
		Border bdr;
		const CellStorePack csPack=GetCellStore(cell,ECSM_STYLE_VALID);
		const CMergeCell* pMerge=csPack.GetMerge();
		if(pMerge)
			cell.row=pMerge->BottomRow();
		const CCell _cell(GetNextRow(cell.row),cell.col);
		if(_cell.row==cell.row){
			bdr=csPack.GetStyle(this).GetBorders()->bottom;
		}else{
			const CellStorePack csPackBottom=GetCellStore(_cell,ECSM_STYLE_VALID);
			const StylePack sp=csPack.GetStylePack();
			const StylePack spBottom=csPackBottom.GetStylePack();
			if(sp.eStorePos>spBottom.eStorePos){
				bdr=Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
				if(bdr.IsNull()){
					bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
				}
			}else{
				bdr=Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
				if(bdr.IsNull())
					bdr=Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
			}
		}
		return bdr;
	}
	/*
	//--------------------------------------------------------------------------------------//
	//在已知单元格cell的存储位置且该位置没有类型的情况下求该单
	//元格的展现类型。所谓展现类型即：按照ECellStorePosition常量从
	//小到大的优先级顺序依次追溯而得到的最终展现给用户的表现
	//类型。
	//--------------------------------------------------------------------------------------//
	pair<ECellType,ECellStorePosition> 
		Worksheet::GetCellType(CCell cell,ECellStorePosition eStartPos)const
	{
		switch(eStartPos)
		{
		case ESP_STYLE_CELL_OWN:
			{
				const CellStore* pcs=rh_.GetRowStore(cell.row);
				if(pcs && pcs->cellType){
					return make_pair(static_cast<ECellType>(pcs->cellType),ESP_STYLE_ROW_OWN);
				}else{
					UINT nRet=ch_.GetColStore(cell.col).cellType;
					if(nRet){
						return make_pair(static_cast<ECellType>(nRet),ESP_STYLE_COL_OWN);
					}else{
						return make_pair(static_cast<ECellType>(tableDefCellStore_.cellType),ESP_STYLE_TABLE_OWN);
					}
				}
			}break;
		case ESP_STYLE_MERGE_OWN://属于合并单元格的风格
			{
				return GetCellType(GetMergeCells(cell)->TopLeft(),ESP_STYLE_CELL_OWN);
			}break;
		case ESP_STYLE_ROW_OWN://属于行的风格
			{
				UINT nRet=ch_.GetColStore(cell.col).cellType;
				if(nRet){
					return make_pair(static_cast<ECellType>(nRet),ESP_STYLE_COL_OWN);
				}else{
					return make_pair(static_cast<ECellType>(tableDefCellStore_.cellType),ESP_STYLE_TABLE_OWN);
				}
			}break;
		case ESP_STYLE_COL_OWN://属于列的风格
			{
				return make_pair(static_cast<ECellType>(tableDefCellStore_.cellType),ESP_STYLE_TABLE_OWN);
			}break;
				//ESP_STYLE_TABLE_OWN,//属于Worksheet的风格
				//ESP_STYLE_DEFAULT,//属于缺省风格
		}
		return make_pair(ECT_NONE,ESP_STYLE_DEFAULT);
	}
	//--------------------------------------------------------------------------------------//
	//在已知单元格cell的存储位置的情况下求该单元格的展现风格ID。
	//所谓展现风格即：按照ECellStorePosition常量从小到大的优先级顺
	//序依次追溯而得到的最终展现给用户的表现风格。
	//--------------------------------------------------------------------------------------//
	pair<StyleID_t,ECellStorePosition> Worksheet::Style_GetCellStyleID(CCell cell,ECellStorePosition eStorePos)const
	{
		switch(eStorePos)
		{
		case ESP_STYLE_CELL_OWN:
			{
				const CellStore* pcs=rh_.GetRowStore(cell.row);
				if(pcs && pcs->nStyleID){
					return make_pair(pcs->nStyleID,ESP_STYLE_ROW_OWN);
				}else{
					StyleID_t sid=ch_.GetColStore(cell.col).nStyleID;
					if(sid){
						return make_pair(sid,ESP_STYLE_COL_OWN);
					}else{
						sid=tableDefCellStore_.nStyleID;
						return make_pair(sid,ESP_STYLE_TABLE_OWN);
					}
				}
			}break;
		case ESP_STYLE_MERGE_OWN://属于合并单元格的风格
			return Style_GetCellStyleID(GetMergeCells(cell)->TopLeft(),ESP_STYLE_CELL_OWN);
		case ESP_STYLE_ROW_OWN://属于行的风格
			{
				StyleID_t sid=ch_.GetColStore(cell.col).nStyleID;
				if(sid){
					return make_pair(sid,ESP_STYLE_COL_OWN);
				}else{
					sid=tableDefCellStore_.nStyleID;
					return make_pair(sid,ESP_STYLE_TABLE_OWN);
				}
			}break;
		case ESP_STYLE_COL_OWN://属于列的风格
			{
				return make_pair(tableDefCellStore_.nStyleID,ESP_STYLE_TABLE_OWN);
			}break;
				//ESP_STYLE_TABLE_OWN,//属于Worksheet的风格
				//ESP_STYLE_DEFAULT,//属于缺省风格
		}
		_ASSERT(FALSE);
		return make_pair(0,ESP_STYLE_DEFAULT);;
	}
	*/
	////自动剪裁，即你可以传一个很大的范围，方法
	////会自动和由冻结线分割的四个可见区间剪裁
	//void InvalidateCellRange(CCellRange const* pcr)
	//{
	//	//_ASSERT(FALSE);
	//	RECT const rc=get_RangeRect(*pcr);//pcr->first.row,pcr->first.col,pcr->second.row,pcr->second.col);
	//	InvalidateRect(&rc,FALSE);
	//}

}//namespace mycell