
typedef int    row_t;
typedef short  col_t;
typedef UINT  CellType;
typedef USHORT  StyleID_t;

#define MinMax(a, b, _min, _max) {if(a<b){_min=a;_max=b;}else{_min=b;_max=a;}}

typedef 
enum EXlLineStyle
{	xlLineStyleNone	= 1,
xlContinuous	= xlLineStyleNone + 1,
xlDash	= xlContinuous + 1,
xlDashDot	= xlDash + 1,
xlDashDotDot	= xlDashDot + 1,
xlDot	= xlDashDotDot + 1,
xlDouble	= xlDot + 1,
xlSlantDashDot	= xlDouble + 1
}	EXlLineStyle;

typedef 
enum EXlHAlign
{	xlHAlignCenter	= 0,
xlHAlignCenterAcrossSelection	= xlHAlignCenter + 1,
xlHAlignDistributed	= xlHAlignCenterAcrossSelection + 1,
xlHAlignFill	= xlHAlignDistributed + 1,
xlHAlignGeneral	= xlHAlignFill + 1,
xlHAlignJustify	= xlHAlignGeneral + 1,
xlHAlignLeft	= xlHAlignJustify + 1,
xlHAlignRight	= xlHAlignLeft + 1
}	EXlHAlign;

typedef struct _id_str_pair {
  int i;
  char* s;
} id_str_pair;
#define IDSTRDEF(ID, STR)  {ID, STR}

typedef enum EXlVAlign
{	xlVAlignBottom	= 0,
xlVAlignCenter	= xlVAlignBottom + 1,
xlVAlignDistributed	= xlVAlignCenter + 1,
xlVAlignJustify	= xlVAlignDistributed + 1,
xlVAlignTop	= xlVAlignJustify + 1
}	EXlVAlign;

id_str_pair EXlHAlignSP[] = {
  IDSTRDEF(xlHAlignCenter,"Center"),// = 0,				//居中				
    IDSTRDEF(xlHAlignCenterAcrossSelection,"CenterAcrossSelection"),//,	//跨列居中
    IDSTRDEF(xlHAlignDistributed,"Justify"),//,			//两端对齐
    IDSTRDEF(xlHAlignFill,"Fill"),//,					//填充
    IDSTRDEF(xlHAlignGeneral,"Top"),//常规
    IDSTRDEF(xlHAlignJustify,"Distributed"),//,				//分散对齐
    IDSTRDEF(xlHAlignLeft,"Left"),//,					//靠左（缩近）
    IDSTRDEF(xlHAlignRight,"Right"),//				//靠右（缩近）
};

id_str_pair EXlVAlignSP[] = {
  IDSTRDEF(xlVAlignBottom, "Bottom"),// = 0,	//靠下
    IDSTRDEF(xlVAlignCenter, "Center"),//,		//居中
    IDSTRDEF(xlVAlignDistributed, "Justify"),//,//两端对齐
    IDSTRDEF(xlVAlignJustify, "Distributed"),//,	//分散对齐
    IDSTRDEF(xlVAlignTop, "Top"),//		    //靠上
};


id_str_pair EXlLineStyleSP[] = {
  IDSTRDEF(xlLineStyleNone, ""),
    IDSTRDEF(xlContinuous, "Continuous"),
    IDSTRDEF(xlDash, "Dash"),
    IDSTRDEF(xlDashDot, "DashDot"),
    IDSTRDEF(xlDashDotDot, "DashDotDot"),
    IDSTRDEF(xlDot, "Dot"),
    IDSTRDEF(xlDouble, "Double"),
    //IDSTRDEF(xlSlantDashDot, "")
};

int id_str_find_id(id_str_pair* p, int n, const char* s, int l) {
  int i;
  for (i=0; i<n; ++i)
    if (0==mystrcmp(p[i].s, (int)strlen(p[i].s), s, l))
      return i;
  return -1;
}

typedef 
enum EXlBordersIndex
{	xlInsideHorizontal	= 1 << 0,
xlInsideVertical	= 1 << 1,
xlDiagonalDown	= 1 << 2,
xlDiagonalUp	= 1 << 3,
xlEdgeBottom	= 1 << 4,
xlEdgeLeft	= 1 << 5,
xlEdgeRight	= 1 << 6,
xlEdgeTop	= 1 << 7
}	EXlBordersIndex;

typedef 
enum EFontSetMask
{	EFSM_FONT_NAME	= 1 << 0,
EFSM_FONT_SIZE	= 1 << 1,
EFSM_FONT_COLOR	= 1 << 2,
EFSM_FONT_ITALIC	= 1 << 3,
EFSM_FONT_BOLD	= 1 << 4,
EFSM_FONT_UNDERLINE	= 1 << 5,
EFSM_FONT_STRIKEOUT	= 1 << 6,
EFSM_FONT_ALL	= 0xff
}	EFontSetMask;

typedef 
enum EAlignmentSetMask
{	EASM_WRAPTEXT	= 1 << 0,
EASM_HORIZONTAL	= 1 << 1,
EASM_VERTICAL	= 1 << 2
}	EAlignmentSetMask;

typedef 
enum ECellStorePosition
{	ESP_STYLE_CELL_OWN	= 0,
ESP_STYLE_MERGE_OWN	= ESP_STYLE_CELL_OWN + 1,
ESP_STYLE_ROW_OWN	= ESP_STYLE_MERGE_OWN + 1,
ESP_STYLE_COL_OWN	= ESP_STYLE_ROW_OWN + 1,
ESP_STYLE_TABLE_OWN	= ESP_STYLE_COL_OWN + 1,
ESP_STYLE_DEFAULT	= ESP_STYLE_TABLE_OWN + 1
}	ECellStorePosition;

typedef 
enum ESelectionMode
{	ESM_SINGLECELL	= 0,
ESM_SINGLELINE	= ESM_SINGLECELL + 1,
ESM_MULTILINE	= ESM_SINGLELINE + 1,
ESM_SINGLESELECTION	= ESM_MULTILINE + 1,
ESM_MULTISELECTION	= ESM_SINGLESELECTION + 1
}	ESelectionMode;

typedef 
enum ECellType
{	ECT_NONE	= 0,
ECT_TEXT	= ECT_NONE + 1,
ECT_COMBOBOX	= ECT_TEXT + 1,
ECT_CHECKBOX	= ECT_COMBOBOX + 1,
ECT_CALENDAR	= ECT_CHECKBOX + 1,
ECT_BUTTON	= ECT_CALENDAR + 1,
ECT_HYPERLINK	= ECT_BUTTON + 1,
ECT_UINT	= ECT_HYPERLINK + 1,
ECT_INT	= ECT_UINT + 1,
ECT_UDOUBLE	= ECT_INT + 1,
ECT_DOUBLE	= ECT_UDOUBLE + 1,
ECT_LOGICAL	= ECT_DOUBLE + 1,
ECT_DEGREE	= ECT_LOGICAL + 1,
ECT_COLOR	= ECT_DEGREE + 1,
ECT_ROWHEADER	= ECT_COLOR + 1,
ECT_COLHEADER	= ECT_ROWHEADER + 1,
ECT_HEADERCORNER	= ECT_COLHEADER + 1,
ECT_USER	= 200,
ECT_MAX	= 256
}	ECellType;

typedef 
enum ESelectionState
{	EST_NOT_IN_SELECTION	= 0,
EST_IN_SELECTION	= EST_NOT_IN_SELECTION + 1,
EST_IN_PROJECTION_OF_SELECTION	= EST_IN_SELECTION + 1,
EST_ACTIVECELL	= EST_IN_PROJECTION_OF_SELECTION + 1
}	ESelectionState;

typedef 
enum EGetCellValDistination
{	EGCVD_NORMAL	= 0,
EGCVD_RENDER	= EGCVD_NORMAL + 1,
EGCVD_EDIT	= EGCVD_RENDER + 1,
EGCVD_TOOLTIP	= EGCVD_EDIT + 1
}	EGetCellValDistination;

typedef struct Cell
{
  int row;
  int col;
}	Cell;

typedef struct CellRange
{
  Cell leftTop;
  Cell rightBottom;
}	CellRange;

typedef struct HLS
{
  BYTE L;
  BYTE H;
  BYTE S;
}	HLS;

typedef struct ColorEntry
{
  COLORREF color;
  LPCTSTR lpszName;
  HLS lhs;
}	ColorEntry;

typedef struct GetCellTextInfo
{
  VARIANT __RPC_FAR *lpText;
  EGetCellValDistination gcvDist;
}	GetCellTextInfo;

typedef struct __NMHDR
{
  HWND hwndFrom;
  UINT_PTR idFrom;
  UINT code;
}	_NMHDR;

typedef struct _NM_SHEETVIEW
{
  _NMHDR hdr;
  int row;
  int col;
  LPARAM lParam;
  HRESULT hr;
  BOOL bHandled;
}	NM_SHEETVIEW;

typedef struct _DrawCellInfo
{
  const CellRange __RPC_FAR *pMerge;
  RECT rcCell;
  LPCTSTR pdata;
  EXlHAlign hAlign;
  EXlVAlign vAlign;
  void __RPC_FAR *pLogFont;
  COLORREF clrFont;
  COLORREF clrBkgnd;
  ECellType cellType;
  ESelectionState nSelState;
  BYTE bWrapText;
  BYTE bMouseHover;
}	DrawCellInfo;


enum eRectRelationShip
{
  RRS_EQUAL,//相等
    RRS_DISJOIN,//相离
    RRS_CONTAIN,//包含
    RRS_INTERNAL,//在内部
    RRS_INTERSECT//相交
};

//该结构在OnPaint期间生存，所有内容应在OnPaint
//开始阶段填充并保持在OnPaint结束期间不变，
//在OnPaint结束后失效。
//毁弃的结构。

enum Colors {
  CLR_BKGND=RGB(0xff,0xff,0xff),
    CLR_SELECTION=0x00153549,//RGB(182,202,234),
    CLR_ACTIVE=0,//0x000f5f59,//RGB(166,160,240),
    CLR_GRID=RGB(192,192,192),
};
enum HeaderColors {
  HCLR_BKGND=RGB(239,235,222),//RGB(200,200,200),
    HCLR_SELECTION=RGB(255,192,111),
    HCLR_ACTIVE=RGB(0,0,0xff),
    HCLR_GRID=RGB(126,125,104),
};
enum ECellHitTestConstants
{ CHTC_UNKNOWN=0,
CHTC_LEFT=1<<0,
CHTC_TOP=1<<1,
CHTC_RIGHT=1<<2,
CHTC_BOTTOM=1<<3,
CHTC_ON_DRAGCORNER=1<<4,
CHTC_ON_DRAGBOUNDS=1<<5,
CHTC_IN_HREMAIN_PART=1<<6,//
CHTC_IN_VREMAIN_PART=1<<7,
CHTC_IN_REMAIN_PART=1<<8,
CHTC_ON_VREMAIN_PART_LEFT=1<<9,
CHTC_ON_HREMAIN_PART_TOP=1<<10,
CHTC_INROWHEADER=1<<11,
CHTC_INCOLHEADER=1<<12,
CHTC_ON_COLDIVIDER=1<<13,
CHTC_ON_ROWDIVIDER=1<<14,
};
typedef enum MoveToConstants
{  
  cellNextCol  = 0,
    cellNextRow  = 1
}   MoveToConstants;

enum EGridPart
{ EGP_UNKNOWN=0,
EGP_HEADER_ALTHOGONAL,//表格头正交区
EGP_HEADER_ROWS,//行表头
EGP_HEADER_COLS,//列表头
EGP_HEADER_FREEZE_ROWS,//行表头冻结部分，包含在EGP_HEADER_ROWS但比其更细化
EGP_HEADER_SCOLL_ROWS,//行表头滚动区部分，包含在EGP_HEADER_ROWS但比其更细化
EGP_HEADER_FREEZE_COLS,//列表头冻结部分，包含在EGP_HEADER_COLS但比其更细化
EGP_HEADER_SCROLL_COLS,//列表头滚动区部分，包含在EGP_HEADER_COLS但比其更细化
EGP_FREEZE_ALTHOGONAL,//冻结正交区
EGP_FREEZE_ROWS,//冻结行部分但不包含EGP_FREEZE_ALTHOGONAL部分
EGP_FREEZE_COLS,//冻结列部分但不包含EGP_FREEZE_ALTHOGONAL部分
EGP_SCROLLABLE,//滚动区部分
EGP_REMAIN_PARTS,//其他背景部分，包含从最后一行到水平滚动条部分加
//最后一列到垂直滚动条部分
EGP_VISIBLE_PART,//可见区间，初略的划分
};
enum EXmlType
{ EXT_UNKNOWN=0,
EXT_MYCELL1_0=1,//
EXT_EXCEL,
};
//enum {ResizeCaptureRange=4};
enum
{ INVALIDATE_ROW=-2,
INVALIDATE_COL=-2,
};
static const MIN_ROW=0;
static const MIN_COL=0;
static const MAX_ROW=0x7FFFF;//19位
static const MAX_COL=0x1FFF;//13位
static const IGNOR_ROW=-1;
static const IGNOR_COL=-1;
static const HEADER_ROW=-1;
static const HEADER_COL=-1;
static const MAX_CELLTEXT=512;
static const CHECKBOX_HALF_WIDTH=7;
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

enum NamedColorIndex
{ WHITE_COLOR=39,
BLACK_COLOR=0,
};
typedef BYTE TIndex;
enum { COLORTABLE_COUNT=48 };

#include "RGBandHLS.inl"
ColorEntry* GetColorTable() {
  static ColorEntry color_table[COLORTABLE_COUNT];
  static int inited=0;
  if (0==inited) {
    size_t i;
    const size_t cnt=COLORTABLE_COUNT;
    for(i=0;i<cnt;++i){
      color_table[i].lhs=RGBtoHLS(color_table[i].color);
    }
    inited = 1;
  }
  return color_table;
}
//给定一个RGB颜色值，返回颜色表的索引值，如果找不到
//相等的颜色，则按最相近的颜色返回
//TIndex find_color(COLORREF clr);
#define GET_COLOR(index)  GetColorTable()[index].color

enum
{ COLOR_MASK=0xFC00,
COLOR_SHIFT=10,
LINESTYLE_MASK=0x380,
LINESTYLE_SHIFT=7,
WEIGHT_MASK=0x7F,
};

//6位color 3位 lineStyle 7位 weight
typedef struct _Border {
  DWORD Color;
  int LineStyle;
  int Weight;
}Border;

#if 0
COLORREF get_Color(Border data_)
{
  BYTE const clrIdx=(COLOR_MASK & data_)>>COLOR_SHIFT;
  ASSERT(clrIdx<COLORTABLE_COUNT);
  return GET_COLOR(clrIdx);
}
#define put_ColorIndex(data_, idx) { _ASSERT(idx<COLORTABLE_COUNT),	data_=(data_&~COLOR_MASK)|((idx&0x3F)<<COLOR_SHIFT); }
EXlLineStyle get_LineStyle(Border data_) {
  return (EXlLineStyle)((data_&LINESTYLE_MASK)>>LINESTYLE_SHIFT);
}
#define put_LineStyle(data_, ls) { _ASSERT(ls<8); data_= !ls?0:((data_&~LINESTYLE_MASK)|((ls&0x7)<<LINESTYLE_SHIFT)); }
BYTE get_Weight(Border data_) { return data_ & WEIGHT_MASK; }
#define put_Weight(data_, weight) { _ASSERT(weight<4); data_=(data_&~WEIGHT_MASK)|(weight&WEIGHT_MASK); }

#define IsNull(data_)  !get_LineStyle(data_)
#define SetNull(data_)  (data_=0)
#endif

typedef struct _CellBorder {
  Border Left,Top,Right,Bottom,DiagonalRight,DiagonalLeft;
} CellBorder;
typedef CellBorder Borders;

//left(prhs->left),top(prhs->top),right(prhs->right) ,bottom(prhs->bottom),DiagonalRight(prhs->DiagonalRight),DiagonalLeft(prhs->DiagonalLeft)
#define CellBorder_eq(lhs, rhs) ( lhs.left==rhs.left && lhs.top==rhs.top && lhs.right==rhs.right && lhs.bottom==rhs.bottom && lhs.DiagonalRight==rhs.DiagonalRight && lhs.DiagonalLeft==rhs.DiagonalLeft )
#if 0
int max_weight(CellBorder border)
{
  const int max1=MAX(MAX(get_Weight(border.left),get_Weight(border.right)),MAX(get_Weight(border.top),get_Weight(border.bottom)));
  const int max2=MAX(get_Weight(border.DiagonalRight),get_Weight(border.DiagonalLeft));
  return MAX(max1,max2);
}
#endif
//说明：bits位描述,位描述从高位到低位,1->8
//  位      属性         描述
//[ 1- 3]   Alignment    EXlVAlign
//[ 4- 6]   Alignment    EXlHAlign 
//[ 7-12]   BackColor    BackColor的索引
//[   13]   IsOwnerDraw  是否自绘制
//[14-19]   font         TextColor的索引
//[20-32]   --           保留
typedef struct _StyleDesc {
  UINT    hAlign:3;
  UINT    vAlign:3;
  UINT    bWrapText:1;
  //UINT    clrBack:6;
  //UINT    isOwnerDraw:1;
  //UINT    TextColorIndex:6;
  //UINT    BackColorIndex:6;
  COLORREF crText, crBack;
  LOGFONT  logFont;
  Borders  borders;
  UINT ID;
  //USHORT  cellType;
  //cellType类型表现ECellType以及用户自定义类型
} StyleDesc;

enum ECalcCellAxisInfoMask
{ CCAI_CALC_HAXIS=1<<0,
CCAI_CALC_VAXIS=1<<1,
CCAI_CALC_HREMAINAXIS=1<<2,
CCAI_CALC_VREMAINAXIS=1<<3,
CCAI_CALC_ALL=0xFF,
};

//参见doc/MyCell坐标轴及区间图.png
typedef struct _CellAxisInfo {
  short VHeaderAxis;
  short VFreezeAxis;
  short VRemainAxis;
  short VClientAxis;
  short HHeaderAxis;
  short HFreezeAxis;
  short HRemainAxis;
  short HClientAxis;
}CellAxisInfo;

typedef struct _CRgnLight {
  int asdf;
}CRgnLight;
typedef struct _VisibleCellsSession {
  int asdf;
}VisibleCellsSession;
typedef struct _CellStore {
  int Index;
  int MergeAcross;
  int Type;
  int StyleID;  //单元格的风格ID
  //USHORT  cellType;    //cellType类型表现ECellType以及用户自定义类型
  // CComVariant cellVal;    //单元格的值
  STRING data;
}CellStore;

typedef struct _StrCellPair {
  char* strKey;
  CellStore CellVal;
}StrCellPair;
typedef struct _RowHeader {
  float Height;
  int StyleID;
  //int AutoFitHeight;
  int Index;
  CellStore* ce;
  int celen;
}RowHeader;

typedef struct _ColHeader {
  float Width;
  int StyleID;
  int Index;
} ColHeader;

typedef struct _Worksheet {
  //CString name_;
  TCHAR szName_[32];
  int ExpandedColumnCount;
  int ExpandedRowCount;
  int FullColumns;
  int FullRows;
  int StyleID;
  int DefaultColumnWidth;
  int DefaultRowHeight;
  
  DWORD bits_;//位组合
  RowHeader* rh;
  ColHeader* ch;
  int  chlen;
  int  rhlen;
  CellStore tableDefCellStore_;

}Worksheet;

typedef struct _Workbook {
  Worksheet vecWorksheet_[1];
  int sheetlen;
  StyleDesc* styleStore_;
  int stylelen;
}Workbook;

typedef struct _mycellinfo {
  int rows;
  int cols;
  int nFreezeBottomRow;
  int nFreezeTopRow;
  int nFreezeRightCol;
  int nFreezeLeftCol;
  int nTopVisScrollRow;
  int nBottomVisScrollRow;
  int nLeftVisScrollCol;
  int nRightVisScrollCol;
  BOOL bShowRowHeader;
  BOOL bShowColHeader;
  BOOL IsFreezeRowVisible;
  BOOL IsFreezeColVisible;
}mycellinfo;


//strColor like #808080
COLORREF RGBFromHtmlString(LPCTSTR strColor)
{
  char szR[3],szG[3],szB[3];
	//USES_CONVERSION;
	//const char* string=OLE2CA(strColor);
	if(strlen(strColor)<7) return _RGB(0,0,0);
	memcpy(szR,&strColor[1],2);
	memcpy(szG,&strColor[3],2);
  memcpy(szB,&strColor[5],2);
  szR[2]=szG[2]=szB[2]='\0';
  
  {
    char* stopstring=NULL;
    BYTE r=(BYTE)strtol(szR,&stopstring,16);
    BYTE g=(BYTE)strtol(szG,&stopstring,16);
    BYTE b=(BYTE)strtol(szB,&stopstring,16);
    
    return _RGB(r,g,b);
  }
}

//strColor like #808080
//在atlutil.h中有一个函数RGBToHtml与此类似
int RGBToHtmlString(char* buf, int buflen, COLORREF color)
{
	_snprintf(buf, buflen, _T("#%0.2x%0.2x%0.2x"),
		GetRValue(color), GetGValue(color), GetBValue(color));
  return 0;
}

enum CellColorType{
  CELL_CLR_BKGND=RGB(0xff,0xff,0xff),
    CELL_CLR_SELECTION=0x00153549,//RGB(182,202,234),
    CELL_CLR_ACTIVE=0,//0x000f5f59,//RGB(166,160,240),
    CELL_CLR_GRID=RGB(192,192,192),
};


enum HeaderColorType{
  HEADER_CLR_BKGND=RGB(239,235,222),//RGB(200,200,200),
    HEADER_CLR_SELECTION=RGB(255,192,111),
    HEADER_CLR_ACTIVE=RGB(0,0,0xff),
    HEADER_CLR_GRID=RGB(126,125,104),
};

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )
