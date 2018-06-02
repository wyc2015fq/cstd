//
// BCGPRibbonInfo.cpp: implementation of the CBCGPRibbonInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPRibbonInfo.h"
#include "BCGPTagManager.h"

#ifndef BCGP_EXCLUDE_RIBBON

static const DWORD c_dwVersion = MAKELONG (1, 0);

static LPCTSTR s_szLFCR     = _T("\r\n");
static LPCTSTR s_szTab      = _T("\t");
static LPCTSTR s_szLFCRT    = _T("\r\n\t");
static LPCTSTR s_szTrue     = _T("TRUE");
static LPCTSTR s_szFalse    = _T("FALSE");
static LPCTSTR s_szR        = _T("R");
static LPCTSTR s_szG        = _T("G");
static LPCTSTR s_szB        = _T("B");

struct XEntity
{
	LPCTSTR szCode;
	LPCTSTR szSymbol;
};

static XEntity s_Entity [] = 
{
	{_T("&amp;") , _T("&")},
	{_T("&lt;")  , _T("<")},
	{_T("&gt;")  , _T(">")},
	{_T("&quot;"), _T("\"")},
	{_T("&apos;"), _T("\'")},
	{_T("&#09;") , _T("\t")}
};

static CString Entity_ToTag (const CString& value)
{
	CString str (value);

	for (int i = 0; i < sizeof (s_Entity) / sizeof (XEntity); i++)
	{
		if (str.Find (s_Entity[i].szSymbol) != -1)
		{
			str.Replace (s_Entity[i].szSymbol, s_Entity[i].szCode);
		}
	}

	return str;
}

static void Entity_FromTag (CString& value)
{
	for (int i = 0; i < sizeof (s_Entity) / sizeof (XEntity); i++)
	{
		if (value.Find (s_Entity[i].szCode) != -1)
		{
			value.Replace (s_Entity[i].szCode, s_Entity[i].szSymbol);
		}
	}
}

static CString WriteString (const CString& strTag, const CString& value, const CString& valueDefault = CString(), BOOL bEmpty = FALSE)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString str;

	if (bEmpty || !value.IsEmpty())
	{
		str.Format (_T("<%s>%s</%s>"), strTag, value, strTag);
	}

	return str;
}

static CString WriteSize (const CString& strTag, const CSize& value, const CSize& valueDefault)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString strValue;
	strValue.Format (_T("%d, %d"), value.cx, value.cy);

	return WriteString (strTag, strValue);
}

static CString WriteBool (const CString& strTag, BOOL value, BOOL valueDefault)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString strValue;
	if (value)
	{
		strValue = s_szTrue;
	}
	else
	{
		strValue = s_szFalse;
	}

	return WriteString (strTag, strValue);
}

static CString WriteInt (const CString& strTag, int value, int valueDefault)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString strValue;
	strValue.Format (_T("%d"), value);

	return WriteString (strTag, strValue);
}

static CString WriteUInt (const CString& strTag, UINT value, UINT valueDefault)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString strValue;
	strValue.Format (_T("%u"), value);

	return WriteString (strTag, strValue);
}

static CString WriteColor (const CString& strTag, COLORREF value, COLORREF valueDefault)
{
	if (value == valueDefault)
	{
		return CString ();
	}

	CString strValue;
	strValue.Format (_T("%d, %d, %d"), GetRValue (value), GetGValue (value), GetBValue (value));

	return WriteString (strTag, strValue);
}

static void WriteTag (CString& strTag, CString strAddTag)
{
	if (!strAddTag.IsEmpty())
	{
		if (!strTag.IsEmpty())
		{
			strTag += s_szLFCR;
		}

		strTag += strAddTag;
	}
}

static void WriteItem (CString& strTag, const CString& strTagName, CString strAddTag)
{
	if (!strAddTag.IsEmpty ())
	{
		strAddTag = s_szLFCR + strAddTag;
		strAddTag.Replace (s_szLFCR, s_szLFCRT);
		strAddTag += s_szLFCR;

		if (!strTag.IsEmpty ())
		{
			strTag += s_szLFCR;
		}
		strTag += WriteString (strTagName, strAddTag);
	}
}

static CString WriteImage (const CString& strTag, const CBCGPRibbonInfo::XImage& value)
{
	if (!value.m_Image.IsValid ())
	{
		return CString ();
	}

	return WriteString (strTag, value.m_strID);
}

static LPCTSTR s_szTag_Name                 = _T("NAME");
static LPCTSTR s_szTag_Value                = _T("VALUE");
static LPCTSTR s_szTag_Text                 = _T("TEXT");
static LPCTSTR s_szTag_ToolTip              = _T("TOOLTIP");
static LPCTSTR s_szTag_Description          = _T("DESCRIPTION");
static LPCTSTR s_szTag_Keys                 = _T("KEYS");
static LPCTSTR s_szTag_MenuKeys             = _T("KEYS_MENU");
static LPCTSTR s_szTag_ID                   = _T("ID");
static LPCTSTR s_szTag_PaletteTop           = _T("PALETTE_TOP");
static LPCTSTR s_szTag_AlwaysLarge          = _T("ALWAYS_LARGE");
static LPCTSTR s_szTag_AlwaysShowDescription= _T("ALWAYS_DESCRIPTION");

static LPCTSTR s_szTag_Index                = _T("INDEX");
static LPCTSTR s_szTag_IndexSmall           = _T("INDEX_SMALL");
static LPCTSTR s_szTag_IndexLarge           = _T("INDEX_LARGE");
static LPCTSTR s_szTag_DefaultCommand       = _T("DEFAULT_COMMAND");
static LPCTSTR s_szTag_Link                 = _T("LINK");
static LPCTSTR s_szTag_Width                = _T("WIDTH");
static LPCTSTR s_szTag_Height               = _T("HEIGHT");
static LPCTSTR s_szTag_WidthFloaty          = _T("WIDTH_FLOATY");
static LPCTSTR s_szTag_SpinButtons          = _T("SPIN_BUTTONS");
static LPCTSTR s_szTag_Min                  = _T("MIN");
static LPCTSTR s_szTag_Max                  = _T("MAX");

static LPCTSTR s_szTag_EditBox              = _T("EDIT_BOX");
static LPCTSTR s_szTag_DropDownList         = _T("DROPDOWN_LIST");
static LPCTSTR s_szTag_ResizeDropDownList   = _T("DROPDOWN_LIST_RESIZE");

static LPCTSTR s_szTag_FontType             = _T("FONT_TYPE");
static LPCTSTR s_szTag_CharSet              = _T("CHAR_SET");
static LPCTSTR s_szTag_PitchAndFamily       = _T("PITCH_AND_FAMILY");

static LPCTSTR s_szTag_ButtonMode           = _T("BUTTON_MODE");
static LPCTSTR s_szTag_MenuResize           = _T("MENU_RESIZE");
static LPCTSTR s_szTag_MenuResizeVertical   = _T("MENU_RESIZE_VERTICAL");

static LPCTSTR s_szTag_Color                = _T("COLOR");
static LPCTSTR s_szTag_SizeBox              = _T("SIZE_BOX");
static LPCTSTR s_szTag_SimpleButtonLook     = _T("SIMPLE_LOOK");

static LPCTSTR s_szTag_Style                = _T("STYLE");
static LPCTSTR s_szTag_Pos                  = _T("POS");
static LPCTSTR s_szTag_ZoomButtons          = _T("ZOOM_BUTTONS");

static LPCTSTR s_szTag_Horiz                = _T("HORIZ");

static LPCTSTR s_szTag_AlmostLargeText      = _T("ALMOST_LARGE_TEXT");
static LPCTSTR s_szTag_Static               = _T("STATIC");
static LPCTSTR s_szTag_TextAlign            = _T("TEXT_ALIGN");

static LPCTSTR s_szTag_QATTop               = _T("QAT_TOP");
static LPCTSTR s_szTag_JustifyColumns       = _T("JUSTIFY_COLUMNS");
static LPCTSTR s_szTag_CenterColumnVert     = _T("CENTER_COLUMN_VERT");

static LPCTSTR s_szTag_Enable               = _T("ENABLE");
static LPCTSTR s_szTag_EnableToolTips       = _T("ENABLE_TOOLTIPS");
static LPCTSTR s_szTag_EnableToolTipsDescr  = _T("ENABLE_TOOLTIPS_DESCRIPTION");
static LPCTSTR s_szTag_EnableKeys           = _T("ENABLE_KEYS");
static LPCTSTR s_szTag_EnablePrintPreview   = _T("ENABLE_PRINTPREVIEW");

static LPCTSTR s_szTag_Label                = _T("LABEL");
static LPCTSTR s_szTag_Visible              = _T("VISIBLE");
static LPCTSTR s_szTag_Infinite             = _T("INFINITE");

static LPCTSTR s_szTag_RecentFileList       = _T("RECENT_FILE_LIST");

static LPCTSTR s_szTag_ElementName          = _T("ELEMENT_NAME");
static LPCTSTR s_szTag_Element              = _T("ELEMENT");
static LPCTSTR s_szTag_Elements             = _T("ELEMENTS");
static LPCTSTR s_szTag_ElementsExtended     = _T("ELEMENTS_EXTENDED");
static LPCTSTR s_szTag_Item                 = _T("ITEM");
static LPCTSTR s_szTag_Items                = _T("ITEMS");
static LPCTSTR s_szTag_Panel                = _T("PANEL");
static LPCTSTR s_szTag_Panels               = _T("PANELS");
static LPCTSTR s_szTag_Category             = _T("CATEGORY");
static LPCTSTR s_szTag_Categories           = _T("CATEGORIES");
static LPCTSTR s_szTag_Context              = _T("CONTEXT");
static LPCTSTR s_szTag_Contexts             = _T("CONTEXTS");
static LPCTSTR s_szTag_RibbonBar            = _T("RIBBON_BAR");
static LPCTSTR s_szTag_StatusBar            = _T("STATUS_BAR");
static LPCTSTR s_szTag_Button_Main          = _T("BUTTON_MAIN");
static LPCTSTR s_szTag_QAT_Elements         = _T("QAT_ELEMENTS");
static LPCTSTR s_szTag_Tab_Elements         = _T("TAB_ELEMENTS");
static LPCTSTR s_szTag_Button_Launch        = _T("BUTTON_LAUNCH");
static LPCTSTR s_szTag_CategoryMain         = _T("CATEGORY_MAIN");

static LPCTSTR s_szTag_Body                 = _T("BCGP_RIBBON");
static LPCTSTR s_szTag_Header               = _T("HEADER");
static LPCTSTR s_szTag_Version              = _T("VERSION");
static LPCTSTR s_szTag_Sizes                = _T("SIZES");

static LPCTSTR s_szTag_Image                = _T("IMAGE");
//static LPCTSTR s_szTag_Image_Hot            = _T("IMAGE_HOT");
//static LPCTSTR s_szTag_Image_Disabled       = _T("IMAGE_DISABLED");
static LPCTSTR s_szTag_Image_Small          = _T("IMAGE_SMALL");
static LPCTSTR s_szTag_Image_Large          = _T("IMAGE_LARGE");
static LPCTSTR s_szTag_Image_SBGroup        = _T("IMAGE_SBGROUP");


LPCTSTR CBCGPRibbonInfo::s_szButton           = _T("Button");
LPCTSTR CBCGPRibbonInfo::s_szButton_Check     = _T("Button_Check");
LPCTSTR CBCGPRibbonInfo::s_szButton_Radio     = _T("Button_Radio");
LPCTSTR CBCGPRibbonInfo::s_szButton_Color     = _T("Button_Color");
LPCTSTR CBCGPRibbonInfo::s_szButton_Undo      = _T("Button_Undo");
LPCTSTR CBCGPRibbonInfo::s_szButton_Palette   = _T("Button_Palette");
LPCTSTR CBCGPRibbonInfo::s_szButton_Hyperlink = _T("Button_Hyperlink");
LPCTSTR CBCGPRibbonInfo::s_szButton_Main      = _T("Button_Main");
LPCTSTR CBCGPRibbonInfo::s_szButton_MainPanel = _T("Button_Main_Panel");
LPCTSTR CBCGPRibbonInfo::s_szButton_Launch    = _T("Button_Launch");
LPCTSTR CBCGPRibbonInfo::s_szLabel            = _T("Label");
LPCTSTR CBCGPRibbonInfo::s_szEdit             = _T("Edit");
LPCTSTR CBCGPRibbonInfo::s_szComboBox         = _T("ComboBox");
LPCTSTR CBCGPRibbonInfo::s_szComboBox_Font    = _T("ComboBox_Font");
LPCTSTR CBCGPRibbonInfo::s_szSlider           = _T("Slider");
LPCTSTR CBCGPRibbonInfo::s_szProgress         = _T("Progress");
LPCTSTR CBCGPRibbonInfo::s_szSeparator        = _T("Separator");
LPCTSTR CBCGPRibbonInfo::s_szGroup            = _T("Group");
LPCTSTR CBCGPRibbonInfo::s_szStatusPane       = _T("StatusPane");
LPCTSTR CBCGPRibbonInfo::s_szPanel            = _T("Panel");
LPCTSTR CBCGPRibbonInfo::s_szCategory         = _T("Category");
LPCTSTR CBCGPRibbonInfo::s_szContext          = _T("Context");
LPCTSTR CBCGPRibbonInfo::s_szCategoryMain     = _T("Category_Main");
LPCTSTR CBCGPRibbonInfo::s_szQAT              = _T("QAT");
LPCTSTR CBCGPRibbonInfo::s_szRibbonBar        = _T("RibbonBar");
LPCTSTR CBCGPRibbonInfo::s_szStatusBar        = _T("StatusBar");


CBCGPRibbonInfo::XID::XID ()
	: m_Value (0)
{
}

CBCGPRibbonInfo::XID::~XID ()
{
}


CBCGPRibbonInfo::XImage::XImage ()
{
}
CBCGPRibbonInfo::XImage::~XImage ()
{
}

CBCGPRibbonInfo::XBase::XBase(const CString& strElementName)
	: m_strElementName (strElementName)
{
}

CBCGPRibbonInfo::XBase::~XBase()
{
}

CBCGPRibbonInfo::XElement::XElement(const CString& strElementName)
	: CBCGPRibbonInfo::XBase (strElementName)
	, m_bIsOnPaletteTop      (FALSE)
	, m_bIsAlwaysLarge       (FALSE)
{
}

CBCGPRibbonInfo::XElement::~XElement()
{
}

CBCGPRibbonInfo::XElementSeparator::XElementSeparator()
	: CBCGPRibbonInfo::XElement (CBCGPRibbonInfo::s_szSeparator)
	, m_bIsHoriz                (FALSE)
{
}

CBCGPRibbonInfo::XElementSeparator::~XElementSeparator()
{
}

CBCGPRibbonInfo::XElementGroup::XElementGroup()
	: CBCGPRibbonInfo::XElement (CBCGPRibbonInfo::s_szGroup)
{
}

CBCGPRibbonInfo::XElementGroup::~XElementGroup()
{
	int i;
	for (i = 0; i < m_arButtons.GetSize (); i++)
	{
		if (m_arButtons[i] != NULL)
		{
			delete m_arButtons[i];
		}
	}
}

CBCGPRibbonInfo::XElementButton::XElementButton(const CString& strElementName)
	: CBCGPRibbonInfo::XElement (strElementName)
	, m_nSmallImageIndex        (-1)
	, m_nLargeImageIndex        (-1)
	, m_bIsDefaultCommand       (TRUE)
{
}

CBCGPRibbonInfo::XElementButton::XElementButton()
	: CBCGPRibbonInfo::XElement (CBCGPRibbonInfo::s_szButton)
	, m_nSmallImageIndex        (-1)
	, m_nLargeImageIndex        (-1)
	, m_bIsDefaultCommand       (TRUE)
	, m_bIsAlwaysShowDescription(FALSE)
{
}

CBCGPRibbonInfo::XElementButton::~XElementButton()
{
	int i;
	for (i = 0; i < m_arSubItems.GetSize (); i++)
	{
		if (m_arSubItems[i] != NULL)
		{
			delete m_arSubItems[i];
		}
	}
}

CBCGPRibbonInfo::XElementLabel::XElementLabel()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szLabel)
{
}

CBCGPRibbonInfo::XElementLabel::~XElementLabel()
{
}

CBCGPRibbonInfo::XElementButtonCheck::XElementButtonCheck()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_Check)
{
}

CBCGPRibbonInfo::XElementButtonCheck::~XElementButtonCheck()
{
}

CBCGPRibbonInfo::XElementButtonRadio::XElementButtonRadio()
	: XElementButton (CBCGPRibbonInfo::s_szButton_Radio)
{
}

CBCGPRibbonInfo::XElementButtonRadio::~XElementButtonRadio()
{
}

CBCGPRibbonInfo::XElementButtonHyperlink::XElementButtonHyperlink()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_Hyperlink)
{
}

CBCGPRibbonInfo::XElementButtonHyperlink::~XElementButtonHyperlink()
{
}

CBCGPRibbonInfo::XElementEdit::XElementEdit(const CString& strElementName)
	: CBCGPRibbonInfo::XElementButton (strElementName)
	, m_nWidth                        (0)
	, m_nWidthFloaty                  (0)
	, m_bHasSpinButtons               (FALSE)
	, m_nMin                          (INT_MAX)
	, m_nMax                          (INT_MAX)
{
}

CBCGPRibbonInfo::XElementEdit::XElementEdit()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szEdit)
	, m_nWidth                        (0)
	, m_nWidthFloaty                  (0)
	, m_bHasSpinButtons               (FALSE)
	, m_nMin                          (INT_MAX)
	, m_nMax                          (INT_MAX)
{
}

CBCGPRibbonInfo::XElementEdit::~XElementEdit()
{
}

CBCGPRibbonInfo::XElementComboBox::XElementComboBox(const CString& strElementName)
	: CBCGPRibbonInfo::XElementEdit (strElementName)
	, m_bHasEditBox                 (FALSE)
	, m_bHasDropDownList            (TRUE)
	, m_bResizeDropDownList         (TRUE)
{
	m_nWidth = 108;
}

CBCGPRibbonInfo::XElementComboBox::XElementComboBox()
	: CBCGPRibbonInfo::XElementEdit (CBCGPRibbonInfo::s_szComboBox)
	, m_bHasEditBox                 (FALSE)
	, m_bHasDropDownList            (TRUE)
	, m_bResizeDropDownList         (TRUE)
{
	m_nWidth = 108;
}

CBCGPRibbonInfo::XElementComboBox::~XElementComboBox()
{
}

CBCGPRibbonInfo::XElementFontComboBox::XElementFontComboBox()
	: CBCGPRibbonInfo::XElementComboBox (CBCGPRibbonInfo::s_szComboBox_Font)
	, m_nFontType                       (DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE)
	, m_nCharSet                        (DEFAULT_CHARSET)
	, m_nPitchAndFamily                 (DEFAULT_PITCH)
{
	m_bHasEditBox = TRUE;
}

CBCGPRibbonInfo::XElementFontComboBox::~XElementFontComboBox()
{
}

CBCGPRibbonInfo::XElementButtonPalette::XElementButtonPalette(const CString& strElementName)
	: CBCGPRibbonInfo::XElementButton (strElementName)
	, m_bIsButtonMode                 (TRUE)
	, m_bEnableMenuResize             (FALSE)
	, m_bMenuResizeVertical           (FALSE)
{
}

CBCGPRibbonInfo::XElementButtonPalette::XElementButtonPalette()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_Palette)
	, m_bIsButtonMode                 (FALSE)
	, m_bEnableMenuResize             (FALSE)
	, m_bMenuResizeVertical           (FALSE)
{
}

CBCGPRibbonInfo::XElementButtonPalette::~XElementButtonPalette()
{
}

CBCGPRibbonInfo::XElementButtonColor::XElementButtonColor()
	: CBCGPRibbonInfo::XElementButtonPalette (CBCGPRibbonInfo::s_szButton_Color)
	, m_Color                                (RGB (0, 0, 0))
	, m_sizeBox                              (22, 22)
	, m_bSimpleButtonLook                    (FALSE)
{
	m_bIsButtonMode = TRUE;
}

CBCGPRibbonInfo::XElementButtonColor::~XElementButtonColor()
{
}

CBCGPRibbonInfo::XElementButtonUndo::XElementButtonUndo()
	: CBCGPRibbonInfo::XElementButtonPalette (CBCGPRibbonInfo::s_szButton_Undo)
{
}

CBCGPRibbonInfo::XElementButtonUndo::~XElementButtonUndo()
{
}

CBCGPRibbonInfo::XElementButtonLaunch::XElementButtonLaunch()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_Launch)
{
}

CBCGPRibbonInfo::XElementButtonLaunch::~XElementButtonLaunch()
{
}

CBCGPRibbonInfo::XElementButtonMain::XElementButtonMain()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_Main)
{
}

CBCGPRibbonInfo::XElementButtonMain::~XElementButtonMain()
{
}

CBCGPRibbonInfo::XElementButtonMainPanel::XElementButtonMainPanel()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szButton_MainPanel)
{
}

CBCGPRibbonInfo::XElementButtonMainPanel::~XElementButtonMainPanel()
{
}

CBCGPRibbonInfo::XElementSlider::XElementSlider()
	: CBCGPRibbonInfo::XElement (CBCGPRibbonInfo::s_szSlider)
	, m_dwStyle                 (0)
	, m_nWidth                  (100)
	, m_nMin                    (0)
	, m_nMax                    (100)
	, m_nPos                    (0)
	, m_bZoomButtons            (FALSE)
{
}

CBCGPRibbonInfo::XElementSlider::~XElementSlider()
{
}

CBCGPRibbonInfo::XElementProgressBar::XElementProgressBar()
	: CBCGPRibbonInfo::XElement (CBCGPRibbonInfo::s_szProgress)
	, m_nWidth                  (100)
	, m_nHeight                 (22)
	, m_nMin                    (0)
	, m_nMax                    (100)
	, m_nPos                    (0)
	, m_bInfinite               (FALSE)
{
}

CBCGPRibbonInfo::XElementProgressBar::~XElementProgressBar()
{
}

CBCGPRibbonInfo::XElementStatusPane::XElementStatusPane()
	: CBCGPRibbonInfo::XElementButton (CBCGPRibbonInfo::s_szStatusPane)
	, m_bIsStatic                     (TRUE)
	, m_nTextAlign                    (TA_LEFT)
{
}

CBCGPRibbonInfo::XElementStatusPane::~XElementStatusPane()
{
}

CBCGPRibbonInfo::XPanel::XPanel()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szPanel)
	, m_nImageIndex          (-1)
	, m_bJustifyColumns      (FALSE)
	, m_bCenterColumnVert    (FALSE)
{
}

CBCGPRibbonInfo::XPanel::~XPanel()
{
	int i;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		if (m_arElements[i] != NULL)
		{
			delete m_arElements[i];
		}
	}
}

CBCGPRibbonInfo::XCategory::XCategory()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szCategory)
{
}

CBCGPRibbonInfo::XCategory::~XCategory()
{
	int i;
	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		if (m_arPanels[i] != NULL)
		{
			delete m_arPanels[i];
		}
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		if (m_arElements[i] != NULL)
		{
			delete m_arElements[i];
		}
	}
}

CBCGPRibbonInfo::XContext::XContext()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szContext)
	, m_Color                (BCGPCategoryColor_None)
{
}

CBCGPRibbonInfo::XContext::~XContext()
{
	int i;
	for (i = 0; i < m_arCategories.GetSize (); i++)
	{
		if (m_arCategories[i] != NULL)
		{
			delete m_arCategories[i];
		}
	}
}

CBCGPRibbonInfo::XCategoryMain::XCategoryMain()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szCategoryMain)
	, m_bRecentListEnable    (FALSE)
	, m_nRecentListWidth     (300)
{
}

CBCGPRibbonInfo::XCategoryMain::~XCategoryMain()
{
	int i = 0;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		if (m_arElements[i] != NULL)
		{
			delete m_arElements[i];
		}
	}
}

CBCGPRibbonInfo::XQAT::XQATItem::XQATItem()
	: m_bVisible (TRUE)
{
}

CBCGPRibbonInfo::XQAT::XQATItem::~XQATItem()
{
}

CBCGPRibbonInfo::XQAT::XQAT()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szQAT)
	, m_bOnTop               (TRUE)
{
}

CBCGPRibbonInfo::XQAT::~XQAT()
{
	m_arItems.RemoveAll ();
}

CBCGPRibbonInfo::XRibbonBar::XRibbonBar()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szRibbonBar)
	, m_bToolTip             (TRUE)
	, m_bToolTipDescr        (TRUE)
	, m_bKeyTips             (TRUE)
	, m_bPrintPreview        (TRUE)
	, m_btnMain              (NULL)
	, m_MainCategory         (NULL)
{
}

CBCGPRibbonInfo::XRibbonBar::~XRibbonBar()
{
	if (m_btnMain != NULL)
	{
		delete m_btnMain;
	}

	if (m_MainCategory != NULL)
	{
		delete m_MainCategory;
	}

	int i;
	for (i = 0; i < m_arCategories.GetSize (); i++)
	{
		if (m_arCategories[i] != NULL)
		{
			delete m_arCategories[i];
		}
	}

	for (i = 0; i < m_arContexts.GetSize (); i++)
	{
		if (m_arContexts[i] != NULL)
		{
			delete m_arContexts[i];
		}
	}
}

CBCGPRibbonInfo::XStatusBar::XStatusElements::XStatusElements()
{
}

CBCGPRibbonInfo::XStatusBar::XStatusElements::~XStatusElements()
{
	int i;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		if (m_arElements[i] != NULL)
		{
			delete m_arElements[i];
		}
	}
}

CBCGPRibbonInfo::XStatusBar::XStatusBar()
	: CBCGPRibbonInfo::XBase (CBCGPRibbonInfo::s_szStatusBar)
{
}

CBCGPRibbonInfo::XStatusBar::~XStatusBar()
{
}


BOOL CBCGPRibbonInfo::XID::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadString (s_szTag_Name, m_Name);

	int nValue = (int)m_Value;
	tm.ReadInt (s_szTag_Value, nValue);
	m_Value = (UINT)nValue;

	return TRUE;
}

void CBCGPRibbonInfo::XID::ToTag (CString& strTag) const
{
	WriteTag (strTag, WriteString (s_szTag_Name, m_Name));
	WriteTag (strTag, WriteUInt (s_szTag_Value, m_Value, 0));
}


CBCGPRibbonInfo::XBase* CBCGPRibbonInfo::XBase::CreateFromName (const CString& name)
{
	CBCGPRibbonInfo::XBase* base = NULL;

	if (name.Compare (CBCGPRibbonInfo::s_szPanel) == 0)
	{
		base = new CBCGPRibbonInfo::XPanel;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szCategoryMain) == 0)
	{
		base = new CBCGPRibbonInfo::XCategoryMain;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szCategory) == 0)
	{
		base = new CBCGPRibbonInfo::XCategory;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szRibbonBar) == 0)
	{
		base = new CBCGPRibbonInfo::XRibbonBar;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szStatusBar) == 0)
	{
		base = new CBCGPRibbonInfo::XStatusBar;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szQAT) == 0)
	{
		base = new CBCGPRibbonInfo::XQAT;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szContext) == 0)
	{
		base = new CBCGPRibbonInfo::XContext;
	}
	else
	{
		base = CBCGPRibbonInfo::XElement::CreateFromName (name);
	}

	return base;
}

CBCGPRibbonInfo::XBase* CBCGPRibbonInfo::XBase::CreateFromTag (const CString& tag)
{
	CBCGPRibbonInfo::XBase* base = NULL;

	CString strElementName;

	{
		CBCGPTagManager tm (tag);
		tm.ReadString (s_szTag_ElementName, strElementName);
	}

	if (!strElementName.IsEmpty ())
	{
		base = CBCGPRibbonInfo::XBase::CreateFromName (strElementName);
		if (base != NULL)
		{
			base->FromTag (tag);
		}
	}

	return base;
}

void CBCGPRibbonInfo::XBase::ToTag (CString& strTag) const
{
	WriteTag (strTag, WriteString (s_szTag_ElementName, m_strElementName));
}

CBCGPRibbonInfo::XElement* CBCGPRibbonInfo::XElement::CreateFromName (const CString& name)
{
	CBCGPRibbonInfo::XElement* element = NULL;

	if (name.Compare (CBCGPRibbonInfo::s_szButton) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButton;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Check) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonCheck;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Radio) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonRadio;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Color) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonColor;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Undo) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonUndo;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Palette) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonPalette;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Hyperlink) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonHyperlink;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Main) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonMain;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_MainPanel) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonMainPanel;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szButton_Launch) == 0)
	{
		element = new CBCGPRibbonInfo::XElementButtonLaunch;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szLabel) == 0)
	{
		element = new CBCGPRibbonInfo::XElementLabel;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szEdit) == 0)
	{
		element = new CBCGPRibbonInfo::XElementEdit;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szComboBox) == 0)
	{
		element = new CBCGPRibbonInfo::XElementComboBox;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szComboBox_Font) == 0)
	{
		element = new CBCGPRibbonInfo::XElementFontComboBox;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szSlider) == 0)
	{
		element = new CBCGPRibbonInfo::XElementSlider;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szProgress) == 0)
	{
		element = new CBCGPRibbonInfo::XElementProgressBar;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szSeparator) == 0)
	{
		element = new CBCGPRibbonInfo::XElementSeparator;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szGroup) == 0)
	{
		element = new CBCGPRibbonInfo::XElementGroup;
	}
	else if (name.Compare (CBCGPRibbonInfo::s_szStatusPane) == 0)
	{
		element = new CBCGPRibbonInfo::XElementStatusPane;
	}

	return element;
}

BOOL CBCGPRibbonInfo::XElement::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strID;
	if (tm.ExcludeTag (s_szTag_ID, strID))
	{
		m_ID.FromTag (strID);
	}

	tm.ReadString (s_szTag_Text, m_strText);
	Entity_FromTag (m_strText);
	tm.ReadString (s_szTag_ToolTip, m_strToolTip);
	Entity_FromTag (m_strToolTip);
	tm.ReadString (s_szTag_Description, m_strDescription);
	Entity_FromTag (m_strDescription);
	tm.ReadString (s_szTag_Keys, m_strKeys);
	Entity_FromTag (m_strKeys);
	tm.ReadString (s_szTag_MenuKeys, m_strMenuKeys);
	Entity_FromTag (m_strMenuKeys);
	tm.ReadBool (s_szTag_PaletteTop, m_bIsOnPaletteTop);
	tm.ReadBool (s_szTag_AlwaysLarge, m_bIsAlwaysLarge);

	return FALSE;
}

void CBCGPRibbonInfo::XElement::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	CString strID;
	m_ID.ToTag (strID);
	WriteItem (strTag, s_szTag_ID, strID);

	WriteTag (strTag, WriteString (s_szTag_Text, Entity_ToTag (m_strText)));
	WriteTag (strTag, WriteString (s_szTag_ToolTip, Entity_ToTag (m_strToolTip)));
	WriteTag (strTag, WriteString (s_szTag_Description, Entity_ToTag (m_strDescription)));
	WriteTag (strTag, WriteString (s_szTag_Keys, Entity_ToTag (m_strKeys)));
	WriteTag (strTag, WriteString (s_szTag_MenuKeys, Entity_ToTag (m_strMenuKeys)));
	WriteTag (strTag, WriteBool (s_szTag_PaletteTop, m_bIsOnPaletteTop, FALSE));
	WriteTag (strTag, WriteBool (s_szTag_AlwaysLarge, m_bIsAlwaysLarge, FALSE));
}

BOOL CBCGPRibbonInfo::XElementSeparator::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadBool (s_szTag_Horiz, m_bIsHoriz);

	return TRUE;
}

void CBCGPRibbonInfo::XElementSeparator::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteBool (s_szTag_Horiz, m_bIsHoriz, FALSE));
}

BOOL CBCGPRibbonInfo::XElementGroup::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		CBCGPTagManager tmElements (strElements);

		CString strElement;
		while (tmElements.ExcludeTag (s_szTag_Element, strElement))
		{
			XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
			if (pElement != NULL)
			{
				m_arButtons.Add (pElement);
			}
		}
	}

	tm.ReadString (s_szTag_Image, m_Images.m_strID);

	return TRUE;
}

void CBCGPRibbonInfo::XElementGroup::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteImage (s_szTag_Image, m_Images));
	
	CString strElements;

	int i = 0;
	for (i = 0; i < m_arButtons.GetSize (); i++)
	{
		CString strElement;
		m_arButtons[i]->ToTag (strElement);
		WriteItem (strElements, s_szTag_Element, strElement);
	}

	WriteItem (strTag, s_szTag_Elements, strElements);
}

BOOL CBCGPRibbonInfo::XElementButton::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		CBCGPTagManager tmElements (strElements);

		CString strElement;
		while (tmElements.ExcludeTag (s_szTag_Element, strElement))
		{
			XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
			if (pElement != NULL)
			{
				m_arSubItems.Add (pElement);
			}
		}
	}

	tm.ReadInt (s_szTag_IndexSmall, m_nSmallImageIndex);
	tm.ReadInt (s_szTag_IndexLarge, m_nLargeImageIndex);
	tm.ReadBool (s_szTag_DefaultCommand, m_bIsDefaultCommand);
	tm.ReadBool (s_szTag_AlwaysShowDescription, m_bIsAlwaysShowDescription);

	return XElement::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementButton::ToTag (CString& strTag) const
{
	XElement::ToTag (strTag);

	WriteTag (strTag, WriteInt (s_szTag_IndexSmall, m_nSmallImageIndex, -1));
	WriteTag (strTag, WriteInt (s_szTag_IndexLarge, m_nLargeImageIndex, -1));
	WriteTag (strTag, WriteBool (s_szTag_DefaultCommand, m_bIsDefaultCommand, TRUE));

	if (GetElementName ().Compare (s_szButton) == 0)
	{
		WriteTag (strTag, WriteBool (s_szTag_AlwaysShowDescription, m_bIsAlwaysShowDescription, FALSE));
	}

	CString strElements;

	int i = 0;
	for (i = 0; i < m_arSubItems.GetSize (); i++)
	{
		CString strElement;
		m_arSubItems[i]->ToTag (strElement);
		WriteItem (strElements, s_szTag_Element, strElement);
	}

	WriteItem (strTag, s_szTag_Elements, strElements);
}

BOOL CBCGPRibbonInfo::XElementLabel::FromTag (const CString& strTag)
{
	return XElementButton::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementLabel::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);
}

BOOL CBCGPRibbonInfo::XElementButtonCheck::FromTag (const CString& strTag)
{
	return XElementButton::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementButtonCheck::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);
}

BOOL CBCGPRibbonInfo::XElementButtonRadio::FromTag (const CString& strTag)
{
	return XElementButton::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementButtonRadio::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);
}

BOOL CBCGPRibbonInfo::XElementButtonHyperlink::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadString (s_szTag_Link, m_strLink);

	return XElementButton::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementButtonHyperlink::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);

	WriteTag (strTag, WriteString (s_szTag_Link, m_strLink));
}

BOOL CBCGPRibbonInfo::XElementEdit::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadInt (s_szTag_Width, m_nWidth);
	tm.ReadInt (s_szTag_WidthFloaty, m_nWidthFloaty);
	tm.ReadBool (s_szTag_SpinButtons, m_bHasSpinButtons);
	tm.ReadInt (s_szTag_Min, m_nMin);
	tm.ReadInt (s_szTag_Max, m_nMax);

	return XElementButton::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementEdit::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);

	WriteTag (strTag, WriteInt (s_szTag_Width, m_nWidth, 0));
	WriteTag (strTag, WriteInt (s_szTag_WidthFloaty, m_nWidthFloaty, 0));
	WriteTag (strTag, WriteBool (s_szTag_SpinButtons, m_bHasSpinButtons, FALSE));
	WriteTag (strTag, WriteInt (s_szTag_Min, m_nMin, INT_MAX));
	WriteTag (strTag, WriteInt (s_szTag_Max, m_nMax, INT_MAX));
}

BOOL CBCGPRibbonInfo::XElementComboBox::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strItems;
	if (tm.ExcludeTag (s_szTag_Items, strItems))
	{
		CBCGPTagManager tmItem (strItems);

		CString strItem;
		while (tmItem.ExcludeTag (s_szTag_Item, strItem))
		{
			Entity_FromTag (strItem);
			m_arItems.Add (strItem);
		}
	}

	tm.ReadBool (s_szTag_EditBox, m_bHasEditBox);
	tm.ReadBool (s_szTag_DropDownList, m_bHasDropDownList);
	tm.ReadBool (s_szTag_ResizeDropDownList, m_bResizeDropDownList);

	return XElementEdit::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementComboBox::ToTag (CString& strTag) const
{
	XElementEdit::ToTag (strTag);

	WriteTag (strTag, WriteBool (s_szTag_EditBox, m_bHasEditBox, FALSE));
	WriteTag (strTag, WriteBool (s_szTag_DropDownList, m_bHasDropDownList, TRUE));
	WriteTag (strTag, WriteBool (s_szTag_ResizeDropDownList, m_bResizeDropDownList, TRUE));

	CString strItems;

	int i = 0;
	for (i = 0; i < m_arItems.GetSize (); i++)
	{
		WriteTag (strItems, WriteString (s_szTag_Item, Entity_ToTag (m_arItems[i]), m_arItems[i] + _T("_")));
	}

	WriteItem (strTag, s_szTag_Items, strItems);
}

BOOL CBCGPRibbonInfo::XElementFontComboBox::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadInt (s_szTag_FontType, m_nFontType);

	int nValue = m_nCharSet;
	tm.ReadInt (s_szTag_CharSet, nValue);
	m_nCharSet = (BYTE)nValue;

	nValue = m_nPitchAndFamily;
	tm.ReadInt (s_szTag_PitchAndFamily, nValue);
	m_nPitchAndFamily = (BYTE)nValue;

	return XElementComboBox::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementFontComboBox::ToTag (CString& strTag) const
{
	XElementComboBox::ToTag (strTag);

	WriteTag (strTag, WriteInt (s_szTag_FontType, m_nFontType, DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE));
	WriteTag (strTag, WriteInt (s_szTag_CharSet, m_nCharSet, DEFAULT_CHARSET));
	WriteTag (strTag, WriteInt (s_szTag_PitchAndFamily, m_nPitchAndFamily, DEFAULT_PITCH));
}

BOOL CBCGPRibbonInfo::XElementButtonPalette::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadBool (s_szTag_ButtonMode, m_bIsButtonMode);
	tm.ReadBool (s_szTag_MenuResize, m_bEnableMenuResize);
	tm.ReadBool (s_szTag_MenuResizeVertical, m_bMenuResizeVertical);

	return XElementButton::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementButtonPalette::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);

	WriteTag (strTag, WriteBool (s_szTag_ButtonMode, m_bIsButtonMode, TRUE));
	WriteTag (strTag, WriteBool (s_szTag_MenuResize, m_bEnableMenuResize, FALSE));
	WriteTag (strTag, WriteBool (s_szTag_MenuResizeVertical, m_bMenuResizeVertical, FALSE));
}

BOOL CBCGPRibbonInfo::XElementButtonColor::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadColor (s_szTag_Color, m_Color);
	tm.ReadSize (s_szTag_SizeBox, m_sizeBox);
	tm.ReadBool (s_szTag_SimpleButtonLook, m_bSimpleButtonLook);

	return XElementButtonPalette::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementButtonColor::ToTag (CString& strTag) const
{
	XElementButtonPalette::ToTag (strTag);

	WriteTag (strTag, WriteColor (s_szTag_Color, m_Color, RGB (0, 0, 0)));
	WriteTag (strTag, WriteSize (s_szTag_SizeBox, m_sizeBox, CSize (22, 22)));
	WriteTag (strTag, WriteBool (s_szTag_SimpleButtonLook, m_bSimpleButtonLook, FALSE));
}

BOOL CBCGPRibbonInfo::XElementButtonUndo::FromTag (const CString& strTag)
{
	return XElementButtonPalette::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementButtonUndo::ToTag (CString& strTag) const
{
	XElementButtonPalette::ToTag (strTag);
}

BOOL CBCGPRibbonInfo::XElementButtonLaunch::FromTag (const CString& strTag)
{
	return XElementButton::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementButtonLaunch::ToTag (CString& strTag) const
{
	if (m_ID.m_Value != 0)
	{
		XElementButton::ToTag (strTag);
	}
}

BOOL CBCGPRibbonInfo::XElementButtonMain::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadString (s_szTag_Image, m_Image.m_strID);

	return XElementButton::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementButtonMain::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);

	WriteTag (strTag, WriteImage (s_szTag_Image, m_Image));
}

BOOL CBCGPRibbonInfo::XElementButtonMainPanel::FromTag (const CString& strTag)
{
	return XElementButton::FromTag (strTag);
}

void CBCGPRibbonInfo::XElementButtonMainPanel::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);
}

BOOL CBCGPRibbonInfo::XElementSlider::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	int nValue = (int)m_dwStyle;
	tm.ReadInt (s_szTag_Style, nValue);
	m_dwStyle = (DWORD)nValue;

	tm.ReadInt (s_szTag_Width, m_nWidth);
	tm.ReadInt (s_szTag_Min, m_nMin);
	tm.ReadInt (s_szTag_Min, m_nMax);
	tm.ReadInt (s_szTag_Pos, m_nPos);
	tm.ReadBool (s_szTag_ZoomButtons, m_bZoomButtons);

	return XElement::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementSlider::ToTag (CString& strTag) const
{
	XElement::ToTag (strTag);

	WriteTag (strTag, WriteUInt (s_szTag_Style, m_dwStyle, 0));
	WriteTag (strTag, WriteInt (s_szTag_Width, m_nWidth, 100));
	WriteTag (strTag, WriteInt (s_szTag_Min, m_nMin, 0));
	WriteTag (strTag, WriteInt (s_szTag_Min, m_nMax, 100));
	WriteTag (strTag, WriteInt (s_szTag_Pos, m_nPos, 0));
	WriteTag (strTag, WriteBool (s_szTag_ZoomButtons, m_bZoomButtons, FALSE));
}

BOOL CBCGPRibbonInfo::XElementProgressBar::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadInt (s_szTag_Width, m_nWidth);
	tm.ReadInt (s_szTag_Height, m_nHeight);
	tm.ReadInt (s_szTag_Min, m_nMin);
	tm.ReadInt (s_szTag_Min, m_nMax);
	tm.ReadInt (s_szTag_Pos, m_nPos);
	tm.ReadBool (s_szTag_Infinite, m_bInfinite);

	return XElement::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementProgressBar::ToTag (CString& strTag) const
{
	XElement::ToTag (strTag);

	WriteTag (strTag, WriteInt (s_szTag_Width, m_nWidth, 100));
	WriteTag (strTag, WriteInt (s_szTag_Height, m_nHeight, 22));
	WriteTag (strTag, WriteInt (s_szTag_Min, m_nMin, 0));
	WriteTag (strTag, WriteInt (s_szTag_Min, m_nMax, 100));
	WriteTag (strTag, WriteInt (s_szTag_Pos, m_nPos, 0));
	WriteTag (strTag, WriteBool (s_szTag_Infinite, m_bInfinite, FALSE));
}

BOOL CBCGPRibbonInfo::XElementStatusPane::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	tm.ReadString (s_szTag_AlmostLargeText, m_strAlmostLargeText);
	Entity_FromTag (m_strAlmostLargeText);
	tm.ReadBool (s_szTag_Static, m_bIsStatic);
	tm.ReadInt (s_szTag_TextAlign, m_nTextAlign);

	return XElementButton::FromTag (tm.GetBuffer ());
}

void CBCGPRibbonInfo::XElementStatusPane::ToTag (CString& strTag) const
{
	XElementButton::ToTag (strTag);

	WriteTag (strTag, WriteString (s_szTag_AlmostLargeText, Entity_ToTag (m_strAlmostLargeText)));
	WriteTag (strTag, WriteBool (s_szTag_Static, m_bIsStatic, TRUE));
	WriteTag (strTag, WriteInt (s_szTag_TextAlign, m_nTextAlign, TA_LEFT));
}

BOOL CBCGPRibbonInfo::XPanel::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		CBCGPTagManager tmElements (strElements);

		CString strElement;
		while (tmElements.ExcludeTag (s_szTag_Element, strElement))
		{
			XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
			if (pElement != NULL)
			{
				m_arElements.Add (pElement);
			}
		}
	}

	CString strButton;
	if (tm.ExcludeTag (s_szTag_Button_Launch, strButton))
	{
		m_btnLaunch.FromTag (strButton);
	}

	tm.ReadBool (s_szTag_JustifyColumns, m_bJustifyColumns);
	tm.ReadBool (s_szTag_CenterColumnVert, m_bCenterColumnVert);
	tm.ReadInt (s_szTag_Index, m_nImageIndex);

	tm.ReadString (s_szTag_Name, m_strName);
	Entity_FromTag (m_strName);
	tm.ReadString (s_szTag_Keys, m_strKeys);
	Entity_FromTag (m_strKeys);

	return TRUE;
}

void CBCGPRibbonInfo::XPanel::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteString (s_szTag_Name, Entity_ToTag (m_strName)));
	WriteTag (strTag, WriteString (s_szTag_Keys, Entity_ToTag (m_strKeys)));
	WriteTag (strTag, WriteInt (s_szTag_Index, m_nImageIndex, -1));
	WriteTag (strTag, WriteBool (s_szTag_JustifyColumns, m_bJustifyColumns, FALSE));
	WriteTag (strTag, WriteBool (s_szTag_CenterColumnVert, m_bCenterColumnVert, FALSE));

	CString strButton;
	m_btnLaunch.ToTag (strButton);
	WriteItem (strTag, s_szTag_Button_Launch, strButton);

	CString strElements;

	int i = 0;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CString strElement;
		m_arElements[i]->ToTag (strElement);
		WriteItem (strElements, s_szTag_Element, strElement);
	}

	WriteItem (strTag, s_szTag_Elements, strElements);
}

BOOL CBCGPRibbonInfo::XCategory::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strPanels;
	if (tm.ExcludeTag (s_szTag_Panels, strPanels))
	{
		CBCGPTagManager tmPanel (strPanels);

		CString strPanel;
		while (tmPanel.ExcludeTag (s_szTag_Panel, strPanel))
		{
			XPanel* pPanel = (XPanel*)CBCGPRibbonInfo::XBase::CreateFromTag (strPanel);
			if (pPanel != NULL)
			{
				m_arPanels.Add (pPanel);
			}
		}
	}

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		CBCGPTagManager tmElements (strElements);

		CString strElement;
		while (tmElements.ExcludeTag (s_szTag_Element, strElement))
		{
			XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
			if (pElement != NULL)
			{
				m_arElements.Add (pElement);
			}
		}
	}

	tm.ReadString (s_szTag_Name, m_strName);
	Entity_FromTag (m_strName);
	tm.ReadString (s_szTag_Keys, m_strKeys);
	Entity_FromTag (m_strKeys);
	tm.ReadString (s_szTag_Image_Small, m_SmallImages.m_strID);
	tm.ReadString (s_szTag_Image_Large, m_LargeImages.m_strID);

	return TRUE;
}

void CBCGPRibbonInfo::XCategory::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteString (s_szTag_Name, Entity_ToTag (m_strName)));
	WriteTag (strTag, WriteString (s_szTag_Keys, Entity_ToTag (m_strKeys)));
	WriteTag (strTag, WriteImage (s_szTag_Image_Small, m_SmallImages));
	WriteTag (strTag, WriteImage (s_szTag_Image_Large, m_LargeImages));

	CString strPanels;

	int i = 0;
	for (i = 0; i < m_arPanels.GetSize (); i++)
	{
		CString strPanel;
		m_arPanels[i]->ToTag (strPanel);
		WriteItem (strPanels, s_szTag_Panel, strPanel);
	}

	WriteItem (strTag, s_szTag_Panels, strPanels);

	CString strElements;

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CString strElement;
		m_arElements[i]->ToTag (strElement);
		WriteItem (strElements, s_szTag_Element, strElement);
	}

	WriteItem (strTag, s_szTag_Elements, strElements);
}

BOOL CBCGPRibbonInfo::XContext::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strCategories;
	if (tm.ExcludeTag (s_szTag_Categories, strCategories))
	{
		CBCGPTagManager tmElements (strCategories);

		CString strCategory;
		while (tmElements.ExcludeTag (s_szTag_Category, strCategory))
		{
			XCategory* pCategory = (XCategory*)CBCGPRibbonInfo::XBase::CreateFromTag (strCategory);
			if (pCategory != NULL)
			{
				m_arCategories.Add (pCategory);
			}
		}
	}

	CString strID;
	if (tm.ExcludeTag (s_szTag_ID, strID))
	{
		m_ID.FromTag (strID);
	}

	tm.ReadString (s_szTag_Text, m_strText);
	Entity_FromTag (m_strText);

	int color = (int)BCGPCategoryColor_None;
	tm.ReadInt (s_szTag_Color, color);
	m_Color = (BCGPRibbonCategoryColor)color;

	return FALSE;
}

void CBCGPRibbonInfo::XContext::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	CString strID;
	m_ID.ToTag (strID);
	WriteItem (strTag, s_szTag_ID, strID);

	WriteTag (strTag, WriteString (s_szTag_Text, Entity_ToTag (m_strText)));
	WriteTag (strTag, WriteInt (s_szTag_Color, (int)m_Color, (int)BCGPCategoryColor_None));

	CString strCategories;
	for (int i = 0; i < m_arCategories.GetSize (); i++)
	{
		CString strCategoryTag;
		m_arCategories[i]->ToTag (strCategoryTag);
		WriteItem (strCategories, s_szTag_Category, strCategoryTag);
	}
	WriteItem (strTag, s_szTag_Categories, strCategories);
}

BOOL CBCGPRibbonInfo::XCategoryMain::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		CBCGPTagManager tmElements (strElements);

		CString strElement;
		while (tmElements.ExcludeTag (s_szTag_Element, strElement))
		{
			XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
			if (pElement != NULL)
			{
				m_arElements.Add (pElement);
			}
		}
	}

	CString strRecent;
	if (tm.ExcludeTag (s_szTag_RecentFileList, strRecent))
	{
		CBCGPTagManager tmRecent (strRecent);

		tmRecent.ReadBool (s_szTag_Enable, m_bRecentListEnable);
		tmRecent.ReadString (s_szTag_Label, m_strRecentListLabel);
		Entity_FromTag (m_strRecentListLabel);
		tmRecent.ReadInt (s_szTag_Width, m_nRecentListWidth);
	}

	tm.ReadString (s_szTag_Name, m_strName);
	Entity_FromTag (m_strName);
	tm.ReadString (s_szTag_Image_Small, m_SmallImages.m_strID);
	tm.ReadString (s_szTag_Image_Large, m_LargeImages.m_strID);

	return TRUE;
}

void CBCGPRibbonInfo::XCategoryMain::ToTag (CString& strTag) const
{
	if (m_arElements.GetSize () == 0 && !m_bRecentListEnable)
	{
		return;
	}

	XBase::ToTag (strTag);

	WriteTag (strTag, WriteString (s_szTag_Name, Entity_ToTag (m_strName)));
	WriteTag (strTag, WriteImage (s_szTag_Image_Small, m_SmallImages));
	WriteTag (strTag, WriteImage (s_szTag_Image_Large, m_LargeImages));

	CString strElements;

	int i = 0;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CString strElement;
		m_arElements[i]->ToTag (strElement);
		WriteItem (strElements, s_szTag_Element, strElement);
	}

	WriteItem (strTag, s_szTag_Elements, strElements);

	CString strRecent;
	WriteTag (strRecent, WriteBool (s_szTag_Enable, m_bRecentListEnable, FALSE));
	WriteTag (strRecent, WriteString (s_szTag_Label, Entity_ToTag (m_strRecentListLabel)));
	WriteTag (strRecent, WriteInt (s_szTag_Width, m_nRecentListWidth, 300));

	WriteItem (strTag, s_szTag_RecentFileList, strRecent);
}

BOOL CBCGPRibbonInfo::XQAT::XQATItem::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strID;
	if (tm.ExcludeTag (s_szTag_ID, strID))
	{
		if (m_ID.FromTag (strID))
		{
			tm.ReadBool (s_szTag_Visible, m_bVisible);
			return TRUE;
		}
	}

	return FALSE;
}

void CBCGPRibbonInfo::XQAT::XQATItem::ToTag (CString& strTag) const
{
	CString strID;
	m_ID.ToTag (strID);
	WriteItem (strTag, s_szTag_ID, strID);

	WriteTag (strTag, WriteBool (s_szTag_Visible, m_bVisible, TRUE));
}

BOOL CBCGPRibbonInfo::XQAT::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strItem;
	while (tm.ExcludeTag (s_szTag_Item, strItem))
	{
		XQATItem item;
		if (item.FromTag (strItem))
		{
			m_arItems.Add (item);
		}
	}

	tm.ReadBool (s_szTag_QATTop, m_bOnTop);

	return TRUE;
}

void CBCGPRibbonInfo::XQAT::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteBool (s_szTag_QATTop, m_bOnTop, TRUE));

	CString strItems;
	for (int i = 0; i < m_arItems.GetSize (); i++)
	{
		CString strItem;
		m_arItems[i].ToTag (strItem);

		WriteItem (strItems, s_szTag_Item, strItem);
	}

	WriteItem (strTag, s_szTag_Items, strItems);
}

BOOL CBCGPRibbonInfo::XRibbonBar::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strButton;
	if (tm.ExcludeTag (s_szTag_Button_Main, strButton))
	{
		m_btnMain = new XElementButtonMain;
		m_btnMain->FromTag (strButton);
	}

	CString strMainCategory;
	if (tm.ExcludeTag (s_szTag_CategoryMain, strMainCategory))
	{
		m_MainCategory = new XCategoryMain;
		m_MainCategory->FromTag (strMainCategory);
	}

	CString strQATElements;
	if (tm.ExcludeTag (s_szTag_QAT_Elements, strQATElements))
	{
		m_QAT.FromTag (strQATElements);
	}

	CString strTabElements;
	if (tm.ExcludeTag (s_szTag_Tab_Elements, strTabElements))
	{
		m_TabElements.FromTag (strTabElements);
	}

	CString strContexts;
	if (tm.ExcludeTag (s_szTag_Contexts, strContexts))
	{
		CBCGPTagManager tmElements (strContexts);

		CString strContext;
		while (tmElements.ExcludeTag (s_szTag_Context, strContext))
		{
			XContext* pContext = (XContext*)CBCGPRibbonInfo::XBase::CreateFromTag (strContext);
			if (pContext != NULL)
			{
				m_arContexts.Add (pContext);
			}
		}
	}

	CString strCategories;
	if (tm.ExcludeTag (s_szTag_Categories, strCategories))
	{
		CBCGPTagManager tmElements (strCategories);

		CString strCategory;
		while (tmElements.ExcludeTag (s_szTag_Category, strCategory))
		{
			XCategory* pCategory = (XCategory*)CBCGPRibbonInfo::XBase::CreateFromTag (strCategory);
			if (pCategory != NULL)
			{
				m_arCategories.Add (pCategory);
			}
		}
	}

	tm.ReadBool (s_szTag_EnableToolTips, m_bToolTip);
	tm.ReadBool (s_szTag_EnableToolTipsDescr, m_bToolTipDescr);
	tm.ReadBool (s_szTag_EnableKeys, m_bKeyTips);
	tm.ReadBool (s_szTag_EnablePrintPreview, m_bPrintPreview);
	tm.ReadString (s_szTag_Image, m_Images.m_strID);

	return TRUE;
}

void CBCGPRibbonInfo::XRibbonBar::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteBool (s_szTag_EnableToolTips, m_bToolTip, TRUE));
	WriteTag (strTag, WriteBool (s_szTag_EnableToolTipsDescr, m_bToolTipDescr, TRUE));
	WriteTag (strTag, WriteBool (s_szTag_EnableKeys, m_bKeyTips, TRUE));
	WriteTag (strTag, WriteBool (s_szTag_EnablePrintPreview, m_bPrintPreview, TRUE));
	WriteTag (strTag, WriteImage (s_szTag_Image, m_Images));

	if (m_btnMain != NULL)
	{
		CString strButton;
		m_btnMain->ToTag (strButton);
		WriteItem (strTag, s_szTag_Button_Main, strButton);
	}

	if (m_MainCategory != NULL)
	{
		CString strMainCategory;
		m_MainCategory->ToTag (strMainCategory);
		WriteItem (strTag, s_szTag_CategoryMain, strMainCategory);
	}

	int i = 0;

	CString strQATElements;
	m_QAT.ToTag (strQATElements);
	WriteItem (strTag, s_szTag_QAT_Elements, strQATElements);

	CString strTabElements;
	m_TabElements.ToTag (strTabElements);
	WriteItem (strTag, s_szTag_Tab_Elements, strTabElements);

	CString strCategories;
	for (i = 0; i < m_arCategories.GetSize (); i++)
	{
		CString strCategory;
		m_arCategories[i]->ToTag (strCategory);
		WriteItem (strCategories, s_szTag_Category, strCategory);
	}
	WriteItem (strTag, s_szTag_Categories, strCategories);

	CString strContexts;
	for (i = 0; i < m_arContexts.GetSize (); i++)
	{
		CString strContext;
		m_arContexts[i]->ToTag (strContext);
		WriteItem (strContexts, s_szTag_Context, strContext);
	}
	WriteItem (strTag, s_szTag_Contexts, strContexts);
}

BOOL CBCGPRibbonInfo::XStatusBar::XStatusElements::FromTag (const CString& strTag)
{
	CBCGPTagManager tmElements (strTag);

	CString strElement;
	while (tmElements.ExcludeTag (s_szTag_Element, strElement))
	{
		XElement* pElement = (XElement*)CBCGPRibbonInfo::XBase::CreateFromTag (strElement);
		if (pElement != NULL)
		{
			m_arElements.Add (pElement);

			CBCGPTagManager tmElement (strElement);

			CString strLabel;
			tmElement.ReadString (s_szTag_Label, strLabel);
			Entity_FromTag (strLabel);
			m_arLabels.Add (strLabel);

			BOOL bVisible = TRUE;
			tmElement.ReadBool (s_szTag_Visible, bVisible);
			m_arVisible.Add (bVisible);
		}
	}

	return TRUE;
}

void CBCGPRibbonInfo::XStatusBar::XStatusElements::ToTag (CString& strTag) const
{
	int i = 0;
	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CString strElement;
		m_arElements[i]->ToTag (strElement);

		WriteTag (strElement, WriteString (s_szTag_Label, Entity_ToTag (m_arLabels[i])));
		WriteTag (strElement, WriteBool (s_szTag_Visible, m_arVisible[i], TRUE));

		WriteItem (strTag, s_szTag_Element, strElement);
	}
}

BOOL CBCGPRibbonInfo::XStatusBar::FromTag (const CString& strTag)
{
	CBCGPTagManager tm (strTag);

	CString strExElements;
	if (tm.ExcludeTag (s_szTag_ElementsExtended, strExElements))
	{
		m_ExElements.FromTag (strExElements);
	}

	CString strElements;
	if (tm.ExcludeTag (s_szTag_Elements, strElements))
	{
		m_Elements.FromTag (strElements);
	}

	tm.ReadString (s_szTag_Image, m_Images.m_strID);

	return TRUE;
}

void CBCGPRibbonInfo::XStatusBar::ToTag (CString& strTag) const
{
	XBase::ToTag (strTag);

	WriteTag (strTag, WriteImage (s_szTag_Image, m_Images));

	CString strElements;
	m_Elements.ToTag (strElements);
	WriteItem (strTag, s_szTag_Elements, strElements);

	CString strExElements;
	m_ExElements.ToTag (strExElements);
	WriteItem (strTag, s_szTag_ElementsExtended, strExElements);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPRibbonInfo::CBCGPRibbonInfo()
{
	m_dwVersion                  = c_dwVersion;
	m_sizeImage[e_ImagesSmall]   = CSize(16, 16);
	m_sizeImage[e_ImagesLarge]   = CSize(32, 32);
	m_sizeImage[e_ImagesSBGroup] = CSize(14, 14);
	
	m_RibbonBar.m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);
	m_StatusBar.m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);
}

CBCGPRibbonInfo::~CBCGPRibbonInfo()
{
}

BOOL CBCGPRibbonInfo::FromTag (const CString& strTag, DWORD use/* = e_UseRibbon | e_UseStatus*/)
{
	if ((use & (e_UseRibbon | e_UseStatus)) == 0)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	CBCGPTagManager tm (strTag);

	CString strHeader;
	if (tm.ExcludeTag (s_szTag_Header, strHeader))
	{
		CBCGPTagManager tmHeader (strHeader);

		int nValue = (int)m_dwVersion;
		tmHeader.ReadInt (s_szTag_Version, nValue);
		m_dwVersion = (DWORD)nValue;

		CString strSizes;
		if (tmHeader.ExcludeTag (s_szTag_Sizes, strSizes))
		{
			CBCGPTagManager tmSizes (strSizes);

			tmSizes.ReadSize (s_szTag_Image_Small, m_sizeImage[e_ImagesSmall]);
			tmSizes.ReadSize (s_szTag_Image_Large, m_sizeImage[e_ImagesLarge]);
			tmSizes.ReadSize (s_szTag_Image_SBGroup, m_sizeImage[e_ImagesSBGroup]);
		}
	}
	else
	{
		return FALSE;
	}

	BOOL bRibbon = FALSE;
	if ((use & e_UseRibbon) == e_UseRibbon)
	{
		CString strRibbonBar;
		if (tm.ExcludeTag (s_szTag_RibbonBar, strRibbonBar))
		{
			bRibbon = m_RibbonBar.FromTag (strRibbonBar);
		}
	}

	BOOL bStatus = FALSE;
	if ((use & e_UseStatus) == e_UseStatus)
	{
		CString strStatusBar;
		if (tm.ExcludeTag (s_szTag_StatusBar, strStatusBar))
		{
			bStatus = m_StatusBar.FromTag (strStatusBar);
		}
	}

	m_RibbonBar.m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);

	if (m_RibbonBar.m_MainCategory != NULL)
	{
		m_RibbonBar.m_MainCategory->m_SmallImages.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);
		m_RibbonBar.m_MainCategory->m_LargeImages.m_Image.SetImageSize (m_sizeImage [e_ImagesLarge]);
	}

	int i = 0;
	for (i = 0; i < m_RibbonBar.m_arCategories.GetSize (); i++)
	{
		m_RibbonBar.m_arCategories[i]->m_SmallImages.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);
		m_RibbonBar.m_arCategories[i]->m_LargeImages.m_Image.SetImageSize (m_sizeImage [e_ImagesLarge]);
	}

	for (i = 0; i < m_RibbonBar.m_arContexts.GetSize (); i++)
	{
		XContext* context = m_RibbonBar.m_arContexts[i];
		for (int j = 0; j < context->m_arCategories.GetSize (); j++)
		{
			context->m_arCategories[j]->m_SmallImages.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);
			context->m_arCategories[j]->m_LargeImages.m_Image.SetImageSize (m_sizeImage [e_ImagesLarge]);
		}
	}

	m_StatusBar.m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSmall]);

	for (i = 0; i < m_StatusBar.m_Elements.m_arElements.GetSize (); i++)
	{
		if (m_StatusBar.m_Elements.m_arElements[i]->GetElementName ().Compare (s_szGroup) == 0)
		{
			((XElementGroup*)m_StatusBar.m_Elements.m_arElements[i])->m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSBGroup]);
		}
	}
	for (i = 0; i < m_StatusBar.m_ExElements.m_arElements.GetSize (); i++)
	{
		if (m_StatusBar.m_ExElements.m_arElements[i]->GetElementName ().Compare (s_szGroup) == 0)
		{
			((XElementGroup*)m_StatusBar.m_ExElements.m_arElements[i])->m_Images.m_Image.SetImageSize (m_sizeImage [e_ImagesSBGroup]);
		}
	}

	return TRUE;
}

void CBCGPRibbonInfo::ToTag (CString& strTag, DWORD use/* = e_UseRibbon | e_UseStatus*/) const
{
	if ((use & (e_UseRibbon | e_UseStatus)) == 0)
	{
		ASSERT (FALSE);
		return;
	}

	CString strData;

	CString strSizes;
	WriteTag (strSizes, WriteSize (s_szTag_Image_Small, m_sizeImage[e_ImagesSmall], CSize (16, 16)));
	WriteTag (strSizes, WriteSize (s_szTag_Image_Large, m_sizeImage[e_ImagesLarge], CSize (32, 32)));
	WriteTag (strSizes, WriteSize (s_szTag_Image_SBGroup, m_sizeImage[e_ImagesSBGroup], CSize (14, 14)));

	CString strHeader;
	WriteTag (strHeader, WriteUInt (s_szTag_Version, m_dwVersion, 0));
	WriteItem (strHeader, s_szTag_Sizes, strSizes);

	WriteItem (strData, s_szTag_Header, strHeader);

	if ((use & e_UseRibbon) == e_UseRibbon)
	{
		CString strRibbonBar;
		m_RibbonBar.ToTag (strRibbonBar);
		WriteItem (strData, s_szTag_RibbonBar, strRibbonBar);
	}

	if ((use & e_UseStatus) == e_UseStatus)
	{
		CString strStatusBar;
		m_StatusBar.ToTag (strStatusBar);
		WriteItem (strData, s_szTag_StatusBar, strStatusBar);
	}

	WriteItem (strTag, s_szTag_Body, strData);
}

void CBCGPRibbonInfo::GetArrayImages (XArrayImages& images)
{
	images.RemoveAll ();

	images.Add (&m_RibbonBar.m_Images);
	if (m_RibbonBar.m_btnMain != NULL)
	{
		images.Add (&m_RibbonBar.m_btnMain->m_Image);
	}
	if (m_RibbonBar.m_MainCategory != NULL)
	{
		images.Add (&m_RibbonBar.m_MainCategory->m_SmallImages);
		images.Add (&m_RibbonBar.m_MainCategory->m_LargeImages);
	}

	int i = 0;
	for (i = 0; i < m_RibbonBar.m_arCategories.GetSize (); i++)
	{
		images.Add (&m_RibbonBar.m_arCategories[i]->m_SmallImages);
		images.Add (&m_RibbonBar.m_arCategories[i]->m_LargeImages);
	}

	for (i = 0; i < m_RibbonBar.m_arContexts.GetSize (); i++)
	{
		XContext* context = m_RibbonBar.m_arContexts[i];
		for (int j = 0; j < context->m_arCategories.GetSize (); j++)
		{
			images.Add (&context->m_arCategories[j]->m_SmallImages);
			images.Add (&context->m_arCategories[j]->m_LargeImages);
		}
	}

	images.Add (&m_StatusBar.m_Images);

	for (i = 0; i < m_StatusBar.m_Elements.m_arElements.GetSize (); i++)
	{
		if (m_StatusBar.m_Elements.m_arElements[i]->GetElementName ().Compare (s_szGroup) == 0)
		{
			images.Add (&((XElementGroup*)m_StatusBar.m_Elements.m_arElements[i])->m_Images);
		}
	}
	for (i = 0; i < m_StatusBar.m_ExElements.m_arElements.GetSize (); i++)
	{
		if (m_StatusBar.m_ExElements.m_arElements[i]->GetElementName ().Compare (s_szGroup) == 0)
		{
			images.Add (&((XElementGroup*)m_StatusBar.m_ExElements.m_arElements[i])->m_Images);
		}
	}
}

#endif // BCGP_EXCLUDE_RIBBON
