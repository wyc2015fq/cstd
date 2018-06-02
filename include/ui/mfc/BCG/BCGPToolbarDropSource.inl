
// CBCGPToolbarDropSource.cpp : implementation file
//


#include "BCGPToolbarDropSource.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolbarDropSource

CBCGPToolbarDropSource::CBCGPToolbarDropSource()
{
	m_bDeleteOnDrop = TRUE;
	m_bEscapePressed = FALSE;
	m_bDragStarted = FALSE;

	m_hcurDelete = NULL;
	m_hcurMove = NULL;
	m_hcurCopy = NULL;
}

CBCGPToolbarDropSource::~CBCGPToolbarDropSource()
{
	if (m_hcurDelete != NULL)
	{
		::DeleteObject (m_hcurDelete);
	}

	if (m_hcurMove != NULL)
	{
		::DeleteObject (m_hcurMove);
	}

	if (m_hcurCopy != NULL)
	{
		::DeleteObject (m_hcurCopy);
	}
}


BEGIN_MESSAGE_MAP(CBCGPToolbarDropSource, COleDropSource)
	//{{AFX_MSG_MAP(CBCGPToolbarDropSource)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPToolbarDropSource message handlers

SCODE CBCGPToolbarDropSource::GiveFeedback(DROPEFFECT dropEffect) 
{
	HCURSOR hcur = NULL;

	switch (dropEffect)
	{
	case DROPEFFECT_MOVE:
		hcur = m_hcurMove;
		break;

	case DROPEFFECT_COPY:
		hcur = m_hcurCopy;
		break;

	default:
		hcur = m_hcurDelete;
		break;
	}

	if (hcur == NULL)
	{
		return COleDropSource::GiveFeedback(dropEffect);
	}

	::SetCursor (hcur);
	return S_OK;
}

SCODE CBCGPToolbarDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState) 
{
	if (m_bDeleteOnDrop && m_hcurDelete != NULL)
	{
		::SetCursor (m_hcurDelete);
	}

	m_bEscapePressed = bEscapePressed;
	return COleDropSource::QueryContinueDrag(bEscapePressed, dwKeyState);
}

BOOL CBCGPToolbarDropSource::OnBeginDrag(CWnd* pWnd) 
{
	if (m_hcurDelete == NULL)
	{
		CBCGPLocalResource locaRes;
		m_hcurDelete = AfxGetApp ()->LoadCursor (IDC_BCGBARRES_DELETE);
		m_hcurMove = AfxGetApp ()->LoadCursor (IDC_BCGBARRES_MOVE);
		m_hcurCopy = AfxGetApp ()->LoadCursor (IDC_BCGBARRES_COPY);
	}

	m_bDragStarted = TRUE;
	return COleDropSource::OnBeginDrag(pWnd);
}
