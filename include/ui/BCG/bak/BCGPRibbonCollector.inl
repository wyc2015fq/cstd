//
// BCGPRibbonCollector.cpp: implementation of the CBCGPRibbonCollector class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPRibbonCollector.h"

#include "BCGPRibbonLabel.h"
#include "BCGPRibbonCheckBox.h"
#include "BCGPRibbonButtonsGroup.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonUndoButton.h"
#include "BCGPRibbonColorButton.h"
#include "BCGPRibbonHyperlink.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPRibbonSlider.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPDrawManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

namespace BCGPRibbonCollector
{

class CMemoryDC
{
public:
	CMemoryDC()
		: m_pOldBitmap (NULL)
		, m_Size       (0, 0)
	{
	}

	void CreateDC()
	{
		if (m_DC.GetSafeHdc() != NULL)
		{
			return;
		}

		CDC* hDC = ::GetDC (NULL);

		CDC* hNewDC = ::CreateCompatibleDC (hDC);
		if (hNewDC != NULL)
		{
			m_DC.Attach (hNewDC);
		}

		::ReleaseDC (NULL, hDC);
	}
	void SetSize (const CSize& size)
	{
		if (m_DC.GetSafeHdc() == NULL)
		{
			CreateDC();
		}

		if (m_Bitmap.IsValid())
		{
			if (m_Size.w != size.w || m_Size.h != size.h)
			{
				if (m_pOldBitmap != NULL)
				{
					m_DC.SelectObject (m_pOldBitmap);
				}

				m_Bitmap.DeleteObject();
			}
		}

		m_Size = size;

		if (m_Bitmap.GetSafeHandle() == NULL)
		{
			CBitmap* hbmp = CBCGPDrawManager::CreateBitmap_32 (size, NULL);
			if (hbmp != NULL)
			{
				m_Bitmap.Attach (hbmp);
				m_pOldBitmap = (CBitmap*) m_DC.SelectObject (&m_Bitmap);
			}
		}
	}

	const CSize& GetSize() const
	{
		return m_Size;
	}

	CDC& GetDC()
	{
		return m_DC;
	}
	const CDC& GetDC() const
	{
		return m_DC;
	}

	CBitmap& GetBitmap()
	{
		return m_Bitmap;
	}
	const CBitmap& GetBitmap() const
	{
		return m_Bitmap;
	}

protected:
	CDC			m_DC;
	CBitmap		m_Bitmap;
	CBitmap*	m_pOldBitmap;
	
	CSize		m_Size;
};

static BOOL AddIcon (CBCGPToolBarImages& images, CBitmap* hIcon)
{
	if (hIcon == NULL)
	{
		return FALSE;
	}

	ICONINFO ii;
	::GetIconInfo (hIcon, &ii);

	CSize size;
	{
		BITMAP bmp;
		if (::GetObject (ii.hbmColor, sizeof (BITMAP), &bmp) == 0)
		{
			ASSERT (FALSE);
			return NULL;
		}

		size.w = bmp.bmWidth;
		size.h = bmp.bmHeight;
	}

	CMemoryDC dcColor;
	dcColor.SetSize (size);
	::DrawIconEx (dcColor.GetDC().GetSafeHdc(), 
		0, 0, 
		hIcon, 
		size.w, size.h, 0, NULL,
		DI_NORMAL);

	BITMAP bmpColor;
	dcColor.GetBitmap().GetBitmap (&bmpColor);
	RGBQUAD* pColor = (RGBQUAD*) bmpColor.bmBits;

	BOOL bConvert = TRUE;
	for (int i = 0; i < size.w * size.h; i++)
	{
		if (pColor[i].rgbReserved != 0)
		{
			bConvert = FALSE;
			break;
		}
	}

	if (bConvert)
	{
		CMemoryDC dcMask;
		dcMask.SetSize (size);
		::DrawIconEx (dcMask.GetDC().GetSafeHdc(), 
			0, 0, 
			hIcon, 
			size.w, size.h, 0, NULL,
			DI_MASK);


		BITMAP bmpMask;
		dcMask.GetBitmap().GetBitmap (&bmpMask);
		RGBQUAD* pMask  = (RGBQUAD*) bmpMask.bmBits;

		if (pColor == NULL || pMask == NULL)
		{
			ASSERT (FALSE);
			return NULL;
		}

		// add alpha channel
		for (int i = 0; i < size.w * size.h; i++)
		{
			pColor->rgbReserved = (BYTE) (255 - pMask->rgbRed);
			pColor++;
			pMask++;
		}
	}

	BOOL bRes = FALSE;

	CBitmap* bitmap = CBCGPDrawManager::CreateBitmap_32 (dcColor.GetBitmap());
	if (bitmap != NULL)
	{
		bRes = images.AddImage (bitmap, FALSE) != -1;
		::DeleteObject (bitmap);
	}

	return bRes;
}

};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonCollector::CBCGPRibbonCollector(CBCGPRibbonInfo& info,
	DWORD dwFlags/* = e_CollectUpdatedToolInfo | e_CollectRibbonBarIcons | e_CollectStatusBarIcons | e_CollectGroupImages*/)
	: m_Info    (info)
	, m_dwFlags (dwFlags)
{
}

CBCGPRibbonCollector::~CBCGPRibbonCollector()
{
}

void CBCGPRibbonCollector::CollectRibbonBar(const CBCGPRibbonBar& bar)
{
	CollectRibbonBar (bar, GetInfo().GetRibbonBar());
}

void CBCGPRibbonCollector::CollectRibbonBar(const CBCGPRibbonBar& bar, CBCGPRibbonInfo::XRibbonBar& info)
{
	info.m_bToolTip = bar.IsToolTipEnabled();
	info.m_bToolTipDescr = bar.IsToolTipDescrEnabled();
	info.m_bKeyTips = bar.IsKeyTipEnabled();
	info.m_bPrintPreview = bar.IsPrintPreviewEnabled();

	// main button
	CBCGPRibbonMainButton* pBtnMain = bar.GetMainButton();
	if (pBtnMain != NULL)
	{
		info.m_btnMain = new CBCGPRibbonInfo::XElementButtonMain;
		CollectElement (*pBtnMain, *info.m_btnMain);
	}

	info.m_MainCategory = new CBCGPRibbonInfo::XCategoryMain;
	CollectCategoryMain (bar, *info.m_MainCategory);

	int i = 0;

	// QAT elements
	CollectQATElements (bar, info);

	// tab elements
	CollectElement (bar.m_TabElements, info.m_TabElements);

	int index = info.m_bPrintPreview ? 1 : 0;
	int count = bar.GetCategoryCount();

	for (i = index; i < count; i++)
	{
		CBCGPRibbonCategory* pCategory = bar.GetCategory (i);
		ASSERT_VALID (pCategory);

		if (pCategory->GetContextID() == 0)
		{
			CBCGPRibbonInfo::XCategory* pInfo = new CBCGPRibbonInfo::XCategory;
			CollectCategory (*pCategory, *pInfo);
			info.m_arCategories.Add (pInfo);
		}
	}

	UINT uiContextID = 0;
	CBCGPRibbonInfo::XContext* pInfoContext = NULL;
	for (i = index; i < count; i++)
	{
		CBCGPRibbonCategory* pCategory = bar.GetCategory (i);
		ASSERT_VALID (pCategory);

		UINT uiID = pCategory->GetContextID();
		if (uiID != 0)
		{
			if (uiContextID != uiID)
			{
				uiContextID = uiID;
				pInfoContext = new CBCGPRibbonInfo::XContext;

				CBCGPRibbonContextCaption* pCaption = bar.FindContextCaption (uiContextID);
				ASSERT_VALID (pCaption);

				pInfoContext->m_strText = pCaption->GetText();
				GetID (*pCaption, pInfoContext->m_ID);
				pInfoContext->m_Color = pCaption->GetColor();

				info.m_arContexts.Add (pInfoContext);
			}

			if (pInfoContext != NULL)
			{
				CBCGPRibbonInfo::XCategory* pInfo = new CBCGPRibbonInfo::XCategory;
				CollectCategory (*pCategory, *pInfo);
				pInfoContext->m_arCategories.Add (pInfo);
			}
		}
	}

	// panel images
	GetRibbonBarImages (bar, info);
}

void CBCGPRibbonCollector::CollectStatusBar(const CBCGPRibbonStatusBar& bar)
{
	CollectStatusBar (bar, GetInfo().GetStatusBar());
}

void CBCGPRibbonCollector::CollectStatusBar(const CBCGPRibbonStatusBar& bar, CBCGPRibbonInfo::XStatusBar& info)
{
	// standard SB elements count
	int i = 0;
	int count = bar.GetCount();

	CBCGPRibbonInfo::XStatusBar::XStatusElements& infoElements = info.m_Elements;
	for (i = 0; i < count; i++)
	{
		const CBCGPBaseRibbonElement* pElement = 
			(const_cast<CBCGPRibbonStatusBar&>(bar)).GetElement (i);
		ASSERT_VALID (pElement);

		infoElements.m_arElements.Add (CollectElement (*pElement));
		infoElements.m_arLabels.Add (bar.GetLabel (pElement));
		infoElements.m_arVisible.Add (pElement->IsVisible());
	}

	// extended SB elements count
	count = bar.GetExCount();

	CBCGPRibbonInfo::XStatusBar::XStatusElements& infoExElements = info.m_ExElements;
	for (i = 0; i < count; i++)
	{
		const CBCGPBaseRibbonElement* pElement = 
			(const_cast<CBCGPRibbonStatusBar&>(bar)).GetExElement (i);
		ASSERT_VALID (pElement);

		infoExElements.m_arElements.Add (CollectElement (*pElement));
		infoExElements.m_arLabels.Add (bar.GetLabel (pElement));
		infoExElements.m_arVisible.Add (pElement->IsVisible());
	}

	// SB images
	GetStatusBarImages (bar, info);
}

void CBCGPRibbonCollector::CollectQATElements (const CBCGPRibbonBar& bar, CBCGPRibbonInfo::XRibbonBar& info)
{
	CList<UINT,UINT> lstDefCommands;
	((CBCGPRibbonBar&)bar).m_QAToolbar.GetDefaultCommands (lstDefCommands);

	POSITION pos = lstDefCommands.GetHeadPosition();
	while (pos != NULL)
	{
		UINT nID = lstDefCommands.GetNext (pos);
		if (nID != 0)
		{
			CBCGPRibbonInfo::XQAT::XQATItem item;
			item.m_ID.m_Value = nID;
			item.m_bVisible = TRUE;
			info.m_QAT.m_arItems.Add (item);
		}
	}

	info.m_QAT.m_bOnTop = bar.m_bQuickAccessToolbarOnTop;
}

void CBCGPRibbonCollector::CollectCategoryMain(const CBCGPRibbonBar& bar, CBCGPRibbonInfo::XCategoryMain& info)
{
	CBCGPRibbonCategory* pCategory = bar.GetMainCategory();
	if (pCategory == NULL)
	{
		return;
	}

	info.m_strName = pCategory->GetName();

	if (pCategory->GetPanelCount() > 0)
	{
		CBCGPRibbonMainPanel* pPanel = DYNAMIC_DOWNCAST (CBCGPRibbonMainPanel, pCategory->GetPanel (0));
		if (pPanel == NULL)
		{
			return;
		}

		int i = 0;
		int count = pPanel->GetCount();

		for (i = 0; i < count; i++)
		{
			CBCGPBaseRibbonElement* pElement = pPanel->GetElement (i);
			ASSERT_VALID (pElement);

			info.m_arElements.Add (CollectElement (*pElement));
		}

		if (pPanel->m_pElemOnRight != NULL)
		{
			info.m_bRecentListEnable  = DYNAMIC_DOWNCAST(CBCGPRibbonRecentFilesList, pPanel->m_pElemOnRight) != NULL;
			info.m_strRecentListLabel = pPanel->m_pElemOnRight->GetText();
			info.m_nRecentListWidth   = pPanel->m_nRightPaneWidth;
		}
	}

	GetCategoryImages (*pCategory, info.m_SmallImages, info.m_LargeImages);
}

void CBCGPRibbonCollector::CollectCategory(const CBCGPRibbonCategory& category, CBCGPRibbonInfo::XCategory& info)
{
	info.m_strName = category.GetName();
	info.m_strKeys = category.m_Tab.GetKeys();

	int i = 0;
	int count = category.GetPanelCount();

	for (i = 0; i < count; i++)
	{
		const CBCGPRibbonPanel* pPanel = 
			(const_cast<CBCGPRibbonCategory&>(category)).GetPanel (i);
		ASSERT_VALID (pPanel);

		CBCGPRibbonInfo::XPanel* pInfo = new CBCGPRibbonInfo::XPanel;
		CollectPanel (*pPanel, *pInfo);
		info.m_arPanels.Add (pInfo);
	}

	const CArray <CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arElements = 
		category.m_arElements;

	int countH = (int)arElements.GetSize();
	count = countH;

	for (i = 0; i < countH; i++)
	{
		CBCGPBaseRibbonElement* pElement = arElements[i];
		if (DYNAMIC_DOWNCAST (CBCGPRibbonDefaultPanelButton, pElement) != NULL)
		{
			count--;
		}
	}

	// hidden element count
	for (i = 0; i < countH; i++)
	{
		CBCGPBaseRibbonElement* pElement = arElements[i];
		if (DYNAMIC_DOWNCAST (CBCGPRibbonDefaultPanelButton, pElement) == NULL)
		{
			ASSERT_VALID (pElement);

			info.m_arElements.Add (CollectElement (*pElement));
		}
	}

	GetCategoryImages (category, info);
}

void CBCGPRibbonCollector::CollectPanel(const CBCGPRibbonPanel& panel, CBCGPRibbonInfo::XPanel& info)
{
	info.m_strName = panel.GetName();
	info.m_strKeys = ((CBCGPRibbonPanel&)panel).GetDefaultButton().GetKeys();
	info.m_bJustifyColumns = panel.IsJustifyColumns();
	info.m_bCenterColumnVert = panel.IsCenterColumnVert();

	CollectElement ((const_cast<CBCGPRibbonPanel&>(panel)).GetLaunchButton(), info.m_btnLaunch);

	int i = 0;
	int count = panel.GetCount();

	for (i = 0; i < count; i++)
	{
		CBCGPBaseRibbonElement* pElement = panel.GetElement (i);
		ASSERT_VALID (pElement);

		info.m_arElements.Add (CollectElement (*pElement));
	}
}

CBCGPRibbonInfo::XElement* CBCGPRibbonCollector::CollectElement(const CBCGPBaseRibbonElement& element)
{
	CBCGPRibbonInfo::XElement* info = NULL;
/*
	if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonMainButton)))
	{
		ar << CBCGPRibbonInfo::s_szButton_Main;

		CBCGPRibbonMainButton* pElement = DYNAMIC_DOWNCAST (CBCGPRibbonMainButton, &element);
		ASSERT_VALID (pElement);

		SerializeBase (*pElement, ar, dwVersion);

		CBitmap* bitmap = ((CBCGPRibbonMainButtonF*)pElement)->m_Image.GetImageWell();
		SerializeBitmap (ar, bitmap);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonLaunchButton)))
	{
		ar << CBCGPRibbonInfo::s_szButton_Launch;

		CBCGPRibbonLaunchButton* pElement = (CBCGPRibbonLaunchButton*)&element;
		ASSERT_VALID (pElement);

		SerializeBase (*pElement, ar, dwVersion);
	}
	else */
	if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonButtonsGroup)))
	{
		CBCGPRibbonInfo::XElementGroup* pNewInfo = new CBCGPRibbonInfo::XElementGroup;
		info = pNewInfo;
		
		CollectElement (element, *info);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonLabel)))
	{
		CBCGPRibbonInfo::XElementLabel* pNewInfo = new CBCGPRibbonInfo::XElementLabel;
		info = pNewInfo;

		CollectBaseElement (element, *info);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonFontComboBox)))
	{
		CBCGPRibbonInfo::XElementFontComboBox* pNewInfo = new CBCGPRibbonInfo::XElementFontComboBox;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonFontComboBox* pElement = (const CBCGPRibbonFontComboBox*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_nWidth = pElement->GetWidth (FALSE);
		pNewInfo->m_nWidthFloaty = pElement->GetWidth (TRUE);
		pNewInfo->m_bHasEditBox = pElement->HasEditBox();
		pNewInfo->m_bResizeDropDownList = pElement->IsResizeDropDownList();
		
		pNewInfo->m_nFontType = pElement->GetFontType();
		pNewInfo->m_nCharSet = pElement->GetCharSet();
		pNewInfo->m_nPitchAndFamily = pElement->GetPitchAndFamily();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonComboBox)))
	{
		CBCGPRibbonInfo::XElementComboBox* pNewInfo = new CBCGPRibbonInfo::XElementComboBox;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonComboBox* pElement = (const CBCGPRibbonComboBox*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_nWidth = pElement->GetWidth (FALSE);
		pNewInfo->m_nWidthFloaty = pElement->GetWidth (TRUE);
		pNewInfo->m_bHasEditBox = pElement->HasEditBox();
		pNewInfo->m_bResizeDropDownList = pElement->IsResizeDropDownList();

		int i = 0;
		int count = (int)pElement->GetCount();

		for (i = 0; i < count; i++)
		{
			pNewInfo->m_arItems.Add (pElement->GetItem (i));
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		CBCGPRibbonInfo::XElementEdit* pNewInfo = new CBCGPRibbonInfo::XElementEdit;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonEdit* pElement = (const CBCGPRibbonEdit*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_nWidth = pElement->GetWidth (FALSE);
		pNewInfo->m_nWidthFloaty = pElement->GetWidth (TRUE);
		pNewInfo->m_bHasSpinButtons = pElement->HasSpinButtons();
		pNewInfo->m_nMin = pElement->GetRangeMin();
		pNewInfo->m_nMax = pElement->GetRangeMax();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonUndoButton)))
	{
		CBCGPRibbonInfo::XElementButtonUndo* pNewInfo = new CBCGPRibbonInfo::XElementButtonUndo;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonUndoButton* pElement = (const CBCGPRibbonUndoButton*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_bIsButtonMode = pElement->IsButtonMode();
		pNewInfo->m_bEnableMenuResize = pElement->IsMenuResizeEnabled();
		pNewInfo->m_bMenuResizeVertical = pElement->IsMenuResizeVertical();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonColorButton)))
	{
		CBCGPRibbonInfo::XElementButtonColor* pNewInfo = new CBCGPRibbonInfo::XElementButtonColor;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonColorButton* pElement = (const CBCGPRibbonColorButton*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_bIsButtonMode = pElement->IsButtonMode();
		pNewInfo->m_bEnableMenuResize = pElement->IsMenuResizeEnabled();
		pNewInfo->m_bMenuResizeVertical = pElement->IsMenuResizeVertical();

		pNewInfo->m_Color = pElement->GetColor();
		pNewInfo->m_sizeBox = pElement->GetColorBoxSize();
		pNewInfo->m_bSimpleButtonLook = pElement->IsSimpleButtonLook();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonPaletteButton)))
	{
		CBCGPRibbonInfo::XElementButtonPalette* pNewInfo = new CBCGPRibbonInfo::XElementButtonPalette;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonPaletteButton* pElement = (const CBCGPRibbonPaletteButton*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_bIsButtonMode = pElement->IsButtonMode();
		pNewInfo->m_bEnableMenuResize = pElement->IsMenuResizeEnabled();
		pNewInfo->m_bMenuResizeVertical = pElement->IsMenuResizeVertical();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonHyperlink)))
	{
		CBCGPRibbonInfo::XElementButtonHyperlink* pNewInfo = new CBCGPRibbonInfo::XElementButtonHyperlink;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonHyperlink* pElement = (const CBCGPRibbonHyperlink*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_strLink = pElement->GetLink();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonRadioButton)))
	{
		CBCGPRibbonInfo::XElementButtonRadio* pNewInfo = new CBCGPRibbonInfo::XElementButtonRadio;
		info = pNewInfo;

		CollectBaseElement (element, *info);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonCheckBox)))
	{
		CBCGPRibbonInfo::XElementButtonCheck* pNewInfo = new CBCGPRibbonInfo::XElementButtonCheck;
		info = pNewInfo;

		CollectBaseElement (element, *info);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonStatusBarPane)))
	{
		CBCGPRibbonInfo::XElementStatusPane* pNewInfo = new CBCGPRibbonInfo::XElementStatusPane;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonStatusBarPane* pElement = (const CBCGPRibbonStatusBarPane*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_strAlmostLargeText = pElement->GetAlmostLargeText();
		pNewInfo->m_bIsStatic = pElement->IsStatic();
		pNewInfo->m_nTextAlign = pElement->GetTextAlign();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonMainPanelButton)))
	{
		CBCGPRibbonInfo::XElementButtonMainPanel* pNewInfo = new CBCGPRibbonInfo::XElementButtonMainPanel;
		info = pNewInfo;

		CollectBaseElement (element, *info);
	}	
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonButton)))
	{
		CBCGPRibbonInfo::XElementButton* pNewInfo = new CBCGPRibbonInfo::XElementButton;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonButton* pElement = (const CBCGPRibbonButton*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_bIsAlwaysShowDescription = pElement->IsAlwaysShowDescription();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonSlider)))
	{
		CBCGPRibbonInfo::XElementSlider* pNewInfo = new CBCGPRibbonInfo::XElementSlider;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonSlider* pElement = (const CBCGPRibbonSlider*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_dwStyle = pElement->GetStyle();
		pNewInfo->m_nWidth = pElement->GetWidth();
		pNewInfo->m_bZoomButtons = pElement->HasZoomButtons();
		pNewInfo->m_nMin = pElement->GetRangeMin();
		pNewInfo->m_nMax = pElement->GetRangeMax();
		pNewInfo->m_nPos = pElement->GetPos();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonProgressBar)))
	{
		CBCGPRibbonInfo::XElementProgressBar* pNewInfo = new CBCGPRibbonInfo::XElementProgressBar;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPRibbonProgressBar* pElement = (const CBCGPRibbonProgressBar*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_nWidth = pElement->GetWidth();
		pNewInfo->m_nMin = pElement->GetRangeMin();
		pNewInfo->m_nMax = pElement->GetRangeMax();
		pNewInfo->m_nPos = pElement->GetPos();
		pNewInfo->m_bInfinite = pElement->IsInfiniteMode();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
	{
		CBCGPRibbonInfo::XElementSeparator* pNewInfo = new CBCGPRibbonInfo::XElementSeparator;
		info = pNewInfo;
		
		const CBCGPRibbonSeparator* pElement = (const CBCGPRibbonSeparator*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_bIsHoriz = pElement->IsHorizontal();
	}

	return info;
}

void CBCGPRibbonCollector::CollectElement(const CBCGPBaseRibbonElement& element, CBCGPRibbonInfo::XElement& info)
{
	if (info.GetElementName().Compare (CBCGPRibbonInfo::s_szButton_Main) == 0 &&
		element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonMainButton)))
	{
		CollectBaseElement (element, info);

		CBCGPRibbonInfo::XElementButtonMain& infoElement = 
			(CBCGPRibbonInfo::XElementButtonMain&)info;

		GetElementImages (element, infoElement.m_Image);
	}
	else if (info.GetElementName().Compare (CBCGPRibbonInfo::s_szButton_Launch) == 0 &&
			 element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonLaunchButton)))
	{
		CollectBaseElement (element, info);
	}
	else if (info.GetElementName().Compare (CBCGPRibbonInfo::s_szGroup) == 0 &&
			 element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonButtonsGroup)))
	{
		const CBCGPRibbonButtonsGroup* pElement = (const CBCGPRibbonButtonsGroup*)&element;
		CBCGPRibbonInfo::XElementGroup& infoElement = 
			(CBCGPRibbonInfo::XElementGroup&)info;

		int count = pElement->GetCount();
		if (count > 0)
		{
			int i = 0;

			GetElementImages (*pElement, infoElement.m_Images);

			for (i = 0; i < count; i++)
			{
				infoElement.m_arButtons.Add (CollectElement (*pElement->GetButton (i)));
			}
		}
	}
	else
	{
		ASSERT (FALSE);
	}
}

void CBCGPRibbonCollector::CollectBaseElement (const CBCGPBaseRibbonElement& element, CBCGPRibbonInfo::XElement& info)
{
	info.m_strText = element.GetText();
	if ((GetFlags() & e_CollectUpdatedToolInfo) == e_CollectUpdatedToolInfo)
	{
		((CBCGPBaseRibbonElement&)element).UpdateTooltipInfo();
	}
	info.m_strToolTip = element.GetToolTip();
	info.m_strDescription = element.GetDescription();
	info.m_strKeys = element.GetKeys();
	info.m_strMenuKeys = element.GetMenuKeys();

	GetID (element, info.m_ID);

	const CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST (CBCGPRibbonButton, &element);
	if (pButton != NULL)
	{
		CBCGPRibbonInfo::XElementButton& infoButton = 
			(CBCGPRibbonInfo::XElementButton&)info;

		infoButton.m_nSmallImageIndex = pButton->GetImageIndex (FALSE);
		infoButton.m_nLargeImageIndex = pButton->GetImageIndex (TRUE);
		infoButton.m_bIsAlwaysLarge = pButton->IsAlwaysLargeImage();
		infoButton.m_bIsDefaultCommand = pButton->IsDefaultCommand();

		const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& subAr = 
			pButton->GetSubItems();

		int i = 0;
		int count = (int)subAr.GetSize();

		for (i = 0; i < count; i++)
		{
			CBCGPBaseRibbonElement* pSubItem = (CBCGPBaseRibbonElement*)subAr[i];
			ASSERT_VALID (pSubItem);

			CBCGPRibbonInfo::XElement* pInfo = CollectElement (*pSubItem);
			pInfo->m_bIsOnPaletteTop = pSubItem->IsOnPaletteTop();
			infoButton.m_arSubItems.Add (pInfo);
		}
	}
}

void CBCGPRibbonCollector::GetID (const CBCGPBaseRibbonElement& element, CBCGPRibbonInfo::XID& info)
{
	info.m_Value = element.GetID();

	if (info.m_Value == -1 || info.m_Value == 0)
	{
		const CBCGPRibbonContextCaption* pCaption = DYNAMIC_DOWNCAST(CBCGPRibbonContextCaption, &element);
		if (pCaption != NULL)
		{
			info.m_Value = pCaption->GetContextID();
		}
	}

	if (info.m_Value == -1)
	{
		info.m_Value = 0;
	}
}

void CBCGPRibbonCollector::GetRibbonBarImages(const CBCGPRibbonBar& bar, CBCGPRibbonInfo::XRibbonBar& info)
{
	if ((GetFlags() & e_CollectRibbonBarIcons) == 0)
	{
		return;
	}

	CBCGPToolBarImages images;
	images.SetImageSize (GetInfo().GetImageSize (CBCGPRibbonInfo::e_ImagesSmall));
	images.SetTransparentColor ((COLORREF)-1);
	images.SetPreMultiplyAutoCheck();

	int nImageIndex = 0;

	CBCGPRibbonButtonsGroup& group = (CBCGPRibbonButtonsGroup&)(bar.m_TabElements);

	int count = group.GetCount();
	int i = 0;

	for (i = 0; i < count; i++)
	{
		CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton, group.GetButton (i));
		if (pButton != NULL)
		{
			CBitmap* hIcon = pButton->GetIcon();
			if (hIcon != NULL)
			{
				if (BCGPRibbonCollector::AddIcon (images, hIcon))
				{
					((CBCGPRibbonInfo::XElementButton*)info.m_TabElements.m_arButtons[i])->m_nSmallImageIndex = nImageIndex;
					nImageIndex++;
				}
			}
		}
	}

	int index = info.m_bPrintPreview ? 1 : 0;
	int nCategory = 0;
	count = bar.GetCategoryCount();

	for (i = index; i < count; i++)
	{
		CBCGPRibbonCategory* pCategory = bar.GetCategory (i);
		ASSERT_VALID(pCategory);

		if (pCategory->GetContextID() != 0)
		{
			continue;
		}

		for (int j = 0; j < pCategory->GetPanelCount(); j++)
		{
			CBCGPRibbonPanel* pPanel = pCategory->GetPanel (j);
			ASSERT_VALID(pPanel);

			CBitmap* hIcon = ((CBCGPRibbonDefaultPanelButton&)pPanel->GetDefaultButton()).m_hIcon;
			if (hIcon != NULL)
			{
				if (BCGPRibbonCollector::AddIcon (images, hIcon))
				{
					info.m_arCategories[nCategory]->m_arPanels[j]->m_nImageIndex = nImageIndex;
					nImageIndex++;
				}
			}
		}

		nCategory++;
	}

	int nContext = -1;
	UINT nContextID = 0;
	nCategory = 0;
	for (i = index; i < count; i++)
	{
		CBCGPRibbonCategory* pCategory = bar.GetCategory (i);

		UINT nID = pCategory->GetContextID();
		if (nID == 0)
		{
			continue;
		}

		if (nID != nContextID)
		{
			nContextID = nID;
			nContext++;
			nCategory = 0;
		}

		for (int j = 0; j < pCategory->GetPanelCount(); j++)
		{
			CBCGPRibbonPanel* pPanel = pCategory->GetPanel (j);
			ASSERT_VALID(pPanel);

			CBitmap* hIcon = ((CBCGPRibbonDefaultPanelButton&)pPanel->GetDefaultButton()).m_hIcon;
			if (hIcon != NULL)
			{
				if (BCGPRibbonCollector::AddIcon (images, hIcon))
				{
					info.m_arContexts[nContext]->m_arCategories[nCategory]->m_arPanels[j]->m_nImageIndex = nImageIndex;
					nImageIndex++;
				}
			}
		}

		nCategory++;
	}

	if (images.IsValid() && images.GetCount() > 0)
	{
		images.CopyTo (info.m_Images.m_Image);
	}
}

void CBCGPRibbonCollector::GetStatusBarImages(const CBCGPRibbonStatusBar& bar, CBCGPRibbonInfo::XStatusBar& info)
{
	if ((GetFlags() & e_CollectStatusBarIcons) == 0)
	{
		return;
	}

	CBCGPToolBarImages images;
	images.SetImageSize (GetInfo().GetImageSize (CBCGPRibbonInfo::e_ImagesSmall));
	images.SetTransparentColor ((COLORREF)-1);
	images.SetPreMultiplyAutoCheck();

	int nImageIndex = 0;
	int count = bar.GetCount();

	for (int i = 0; i < count; i++)
	{
		CBCGPRibbonStatusBarPane* pPane = 
			DYNAMIC_DOWNCAST(CBCGPRibbonStatusBarPane, ((CBCGPRibbonStatusBar&)bar).GetElement (i));
		if (pPane == NULL)
		{
			continue;
		}

		CBitmap* hIcon = pPane->GetIcon (TRUE);
		if (hIcon != NULL)
		{
			if (BCGPRibbonCollector::AddIcon (images, hIcon))
			{
				((CBCGPRibbonInfo::XElementStatusPane*)info.m_Elements.m_arElements[i])->m_nSmallImageIndex = nImageIndex;
				nImageIndex++;
			}
		}
	}

	if (images.IsValid() && images.GetCount() > 0)
	{
		images.CopyTo (info.m_Images.m_Image);
	}
}

void CBCGPRibbonCollector::GetCategoryImages(const CBCGPRibbonCategory& category, CBCGPRibbonInfo::XCategory& info)
{
	GetCategoryImages (category, info.m_SmallImages, info.m_LargeImages);

	if ((GetFlags() & e_CollectGroupImages) == 0)
	{
		return;
	}

	CBCGPRibbonCategory* pCategory = (CBCGPRibbonCategory*)&category;
	CBCGPToolBarImages& infoSmall = info.m_SmallImages.m_Image;

	if (!infoSmall.IsValid() || infoSmall.GetCount() == 0)
	{
		infoSmall.SetImageSize (GetInfo().GetImageSize (CBCGPRibbonInfo::e_ImagesSmall));
		infoSmall.SetTransparentColor ((COLORREF)-1);
		infoSmall.SetPreMultiplyAutoCheck();
	}

	int nImageIndex = infoSmall.GetCount();
	for (int i = 0; i < pCategory->GetPanelCount(); i++)
	{
		CBCGPRibbonPanel* pPanel = pCategory->GetPanel (i);

		for (int j = 0; j < pPanel->GetCount(); j++)
		{
			CBCGPRibbonButtonsGroup* pGroup = DYNAMIC_DOWNCAST(CBCGPRibbonButtonsGroup, pPanel->GetElement (j));
			if (pGroup == NULL)
			{
				continue;
			}

			CBCGPToolBarImages& images = pGroup->GetImages();
			if (!images.IsValid() || images.GetCount() == 0)
			{
				continue;
			}

			CBCGPRibbonInfo::XElementGroup* pElementGroup = 
				(CBCGPRibbonInfo::XElementGroup*)(info.m_arPanels[i]->m_arElements[j]);

			for (int k = 0; k < pGroup->GetCount(); k++)
			{
				CBCGPRibbonButton* pButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton, pGroup->GetButton (k));
				if (pButton == NULL)
				{
					continue;
				}

				int nImage = pButton->GetImageIndex (FALSE);
				if (nImage == -1)
				{
					continue;
				}

				((CBCGPRibbonInfo::XElementButton*)pElementGroup->m_arButtons[k])->m_nSmallImageIndex = nImageIndex;

				infoSmall.AddImage (images, nImage);
				nImageIndex++;
			}
/*
			int nImageIndex = infoSmall.m_Image.GetCount();
			for (int k = 0; k < images.GetCount(); k++)
			{
				infoSmall.m_Image.AddImage (images, k);
			}
*/
		}
	}

	if (infoSmall.GetCount() == 0)
	{
		infoSmall.Clear();
	}
}

void CBCGPRibbonCollector::GetCategoryImages(const CBCGPRibbonCategory& category, CBCGPRibbonInfo::XImage& infoSmall, CBCGPRibbonInfo::XImage& infoLarge)
{
	CBCGPRibbonCategory* pCategory = (CBCGPRibbonCategory*)&category;

	pCategory->GetLargeImages().CopyTo (infoLarge.m_Image);
	pCategory->GetSmallImages().CopyTo (infoSmall.m_Image);
}

void CBCGPRibbonCollector::GetElementImages(const CBCGPBaseRibbonElement& element, CBCGPRibbonInfo::XImage& info)
{
	if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonMainButton)))
	{
		CBCGPRibbonMainButton* pElement = (CBCGPRibbonMainButton*)&element;

		pElement->GetImage().CopyTo (info.m_Image);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPRibbonButtonsGroup)))
	{
		CBCGPRibbonButtonsGroup* pElement = (CBCGPRibbonButtonsGroup*)&element;

		pElement->GetImages().CopyTo (info.m_Image);
	}
}

#endif // BCGP_EXCLUDE_RIBBON
