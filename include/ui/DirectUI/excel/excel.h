

typedef int    row_t;
typedef short  col_t;
typedef UINT   CellType;
typedef UINT   StyleID_t;
typedef UINT   CComVariant;

#ifndef free_s
#define free_s(_p)   ((_p!=NULL)?(free((void*)(_p)), _p=0):(0))
#endif
#define MinMax( a, b, _min, _max) { if(a<b){ _min=a,_max=b;}else{_min=b,_max=a;} }

typedef
enum EXlLineStyle
{  xlLineStyleNone  = 1,
xlContinuous  = xlLineStyleNone + 1,
xlDash  = xlContinuous + 1,
xlDashDot  = xlDash + 1,
xlDashDotDot  = xlDashDot + 1,
xlDot  = xlDashDotDot + 1,
xlDouble  = xlDot + 1,
xlSlantDashDot  = xlDouble + 1
} EXlLineStyle;

typedef 
enum EXlHAlign
{  xlHAlignCenter  = 0,
xlHAlignCenterAcrossSelection  = xlHAlignCenter + 1,
xlHAlignDistributed  = xlHAlignCenterAcrossSelection + 1,
xlHAlignFill  = xlHAlignDistributed + 1,
xlHAlignGeneral  = xlHAlignFill + 1,
xlHAlignJustify  = xlHAlignGeneral + 1,
xlHAlignLeft  = xlHAlignJustify + 1,
xlHAlignRight  = xlHAlignLeft + 1
} EXlHAlign;

typedef 
enum EXlVAlign
{  xlVAlignBottom  = 0,
xlVAlignCenter  = xlVAlignBottom + 1,
xlVAlignDistributed  = xlVAlignCenter + 1,
xlVAlignJustify  = xlVAlignDistributed + 1,
xlVAlignTop  = xlVAlignJustify + 1
} EXlVAlign;

typedef 
enum EXlBordersIndex
{  xlInsideHorizontal  = 1 << 0,
xlInsideVertical  = 1 << 1,
xlDiagonalDown  = 1 << 2,
xlDiagonalUp  = 1 << 3,
xlEdgeBottom  = 1 << 4,
xlEdgeLeft  = 1 << 5,
xlEdgeRight  = 1 << 6,
xlEdgeTop  = 1 << 7
} EXlBordersIndex;

typedef 
enum EFontSetMask
{  EFSM_FONT_NAME  = 1 << 0,
EFSM_FONT_SIZE  = 1 << 1,
EFSM_FONT_COLOR  = 1 << 2,
EFSM_FONT_ITALIC  = 1 << 3,
EFSM_FONT_BOLD  = 1 << 4,
EFSM_FONT_UNDERLINE  = 1 << 5,
EFSM_FONT_STRIKEOUT  = 1 << 6,
EFSM_FONT_ALL  = 0xff
} EFontSetMask;

typedef 
enum EAlignmentSetMask
{  EASM_WRAPTEXT  = 1 << 0,
EASM_HORIZONTAL  = 1 << 1,
EASM_VERTICAL  = 1 << 2
} EAlignmentSetMask;

typedef 
enum ECellStorePosition
{  ESP_STYLE_CELL_OWN  = 0,
ESP_STYLE_MERGE_OWN  = ESP_STYLE_CELL_OWN + 1,
ESP_STYLE_ROW_OWN  = ESP_STYLE_MERGE_OWN + 1,
ESP_STYLE_COL_OWN  = ESP_STYLE_ROW_OWN + 1,
ESP_STYLE_TABLE_OWN  = ESP_STYLE_COL_OWN + 1,
ESP_STYLE_DEFAULT  = ESP_STYLE_TABLE_OWN + 1
} ECellStorePosition;

typedef 
enum ESelectionMode
{  ESM_SINGLECELL  = 0,
ESM_SINGLELINE  = ESM_SINGLECELL + 1,
ESM_MULTILINE  = ESM_SINGLELINE + 1,
ESM_SINGLESELECTION  = ESM_MULTILINE + 1,
ESM_MULTISELECTION  = ESM_SINGLESELECTION + 1
} ESelectionMode;

typedef 
enum ECellType
{  ECT_NONE  = 0,
ECT_TEXT  = ECT_NONE + 1,
ECT_COMBOBOX  = ECT_TEXT + 1,
ECT_CHECKBOX  = ECT_COMBOBOX + 1,
ECT_CALENDAR  = ECT_CHECKBOX + 1,
ECT_BUTTON  = ECT_CALENDAR + 1,
ECT_HYPERLINK  = ECT_BUTTON + 1,
ECT_UINT  = ECT_HYPERLINK + 1,
ECT_INT  = ECT_UINT + 1,
ECT_UDOUBLE  = ECT_INT + 1,
ECT_DOUBLE  = ECT_UDOUBLE + 1,
ECT_LOGICAL  = ECT_DOUBLE + 1,
ECT_DEGREE  = ECT_LOGICAL + 1,
ECT_COLOR  = ECT_DEGREE + 1,
ECT_ROWHEADER  = ECT_COLOR + 1,
ECT_COLHEADER  = ECT_ROWHEADER + 1,
ECT_HEADERCORNER  = ECT_COLHEADER + 1,
ECT_USER  = 200,
ECT_MAX  = 256
} ECellType;

typedef 
enum ESelectionState
{  EST_NOT_IN_SELECTION  = 0,
EST_IN_SELECTION  = EST_NOT_IN_SELECTION + 1,
EST_IN_PROJECTION_OF_SELECTION  = EST_IN_SELECTION + 1,
EST_ACTIVECELL  = EST_IN_PROJECTION_OF_SELECTION + 1
} ESelectionState;

typedef 
enum EGetCellValDistination
{  EGCVD_NORMAL  = 0,
EGCVD_RENDER  = EGCVD_NORMAL + 1,
EGCVD_EDIT  = EGCVD_RENDER + 1,
EGCVD_TOOLTIP  = EGCVD_EDIT + 1
} EGetCellValDistination;

//关于选择集合内绘制单元格背景时是否向内收缩，
//以及在哪个方向收缩的枚举常量
typedef enum _ECellBGShrinkState
{ ECBSS_SHRINK_NONE=0,//无缩进
ECBSS_SHRINK_LEFT=1<<0,//左边收缩
ECBSS_SHRINK_TOP=1<<1,//上边收缩
ECBSS_SHRINK_RIGHT=1<<2,//右边收缩
ECBSS_SHRINK_BOTTOM=1<<3,//下边收缩
ECBSS_SHRINK_LEFTTOP=1<<4,//左上边收缩
ECBSS_SHRINK_RIGHTTOP=1<<5,//右上边收缩
ECBSS_SHRINK_LEFTBOTTOM=1<<6,//左下边收缩
ECBSS_SHRINK_RIGHTBOTTOM=1<<7,//右下边收缩
ECBSS_UNKNOWN=1<<31,//未知
}ECellBGShrinkState;
enum EMouseMode 
{MOUSE_NOTHING, 
MOUSE_SELECT_ALL, 
MOUSE_SELECT_COL, 
MOUSE_SELECT_ROW,
MOUSE_SELECT_CELLS, 
MOUSE_SCROLLING_CELLS,
MOUSE_OVER_ROW_DIVIDE, 
MOUSE_SIZING_ROW, 
MOUSE_OVER_COL_DIVIDE,
MOUSE_SIZING_COL,
MOUSE_OVER_ACTIVESELECTION_DRAG_CORNER,
MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#endif
};
//单元格与选区的位置关系常量
typedef enum _ECellSelectionRelations
{  ECSR_UNKNOWN=0,//未知或未设置状态
ECSR_INNER_LEFT=1<<1,//单元格在选区内并与选区左边框相邻
ECSR_INNER_TOP=1<<2,//选区内靠上
ECSR_INNER_RIGHT=1<<3,//选区内靠右
ECSR_INNER_BOTTOM=1<<4,//选区内靠下
ECSR_OUTER_LEFT=1<<5,//选区外与选区左边框相邻
ECSR_OUTER_TOP=1<<6,//选区外与选区上边框相邻
ECSR_OUTER_RIGHT=1<<7,//选区外与选区右边框相邻
ECSR_OUTER_BOTTOM=1<<8,//选区外与选区下边框相邻
ECSR_OUTER_LEFTTOP=1<<9,//选区外与选区左上角相邻
ECSR_OUTER_RIGHTTOP=1<<10,//选区外与选区右上角相邻
ECSR_OUTER_LEFTBOTTOM=1<<11,//选区外与选区左下角相邻
ECSR_OUTER_RIGHTBOTTOM=1<<12,//选区外与选区右下角相邻
} ECellSelectionRelations;

enum eRectRelationShip
{ RRS_EQUAL,//相等
RRS_DISJOIN,//相离
RRS_CONTAIN,//包含
RRS_INTERNAL,//在内部
RRS_INTERSECT//相交
};
//该结构在OnPaint期间生存，所有内容应在OnPaint
//开始阶段填充并保持在OnPaint结束期间不变，
//在OnPaint结束后失效。
//毁弃的结构。

enum Colors
{ CLR_BKGND=RGB(0xff,0xff,0xff),
CLR_SELECTION=0x00153549,//RGB(182,202,234),
CLR_ACTIVE=0,//0x000f5f59,//RGB(166,160,240),
CLR_GRID=RGB(192,192,192),
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
{ cellNextCol  = 0,
cellNextRow  = 1
} MoveToConstants;

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
#define MIN_ROW 0
#define MIN_COL 0
#define MAX_ROW 0x7FFFF //19位
#define MAX_COL 0x1FFF  //13位
#define IGNOR_ROW -1
#define IGNOR_COL -1
#define HEADER_ROW -1
#define HEADER_COL -1
#define MAX_CELLTEXT 512
#define CHECKBOX_HALF_WIDTH 7
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )


typedef struct Cell { int row, col; } Cell;

typedef struct CellRange { Cell leftTop, rightBottom;} CellRange;

typedef struct HLS { BYTE L, H, S; } HLS;

typedef struct ColorEntry
{
  COLORREF color;
  LPCTSTR lpszName;
  HLS lhs;
} ColorEntry;

typedef struct GetCellTextInfo
{
  VARIANT *lpText;
  EGetCellValDistination gcvDist;
} GetCellTextInfo;

typedef struct _NMHDR
{
  HWND hwndFrom;
  UINT_PTR idFrom;
  UINT code;
} _NMHDR;

typedef struct NM_SHEETVIEW
{
  _NMHDR hdr;
  int row;
  int col;
  LPARAM lParam;
  HRESULT hr;
  BOOL bHandled;
} NM_SHEETVIEW;

typedef struct DrawCellInfo
{
  RECT rcCell;
  StrID_t nData;
  TCHAR text[MAX_CELLTEXT];
  EXlHAlign hAlign;
  EXlVAlign vAlign;
  void *pLogFont;
  COLORREF clrFont;
  COLORREF clrBkgnd;
  ECellType cellType;
  ESelectionState nSelState;
  BYTE bWrapText;
  BYTE bMouseHover;
} DrawCellInfo;

enum 
{
  COLORTABLE_COUNT=48
};


typedef BYTE TIndex;

#include "RGBandHLS.inl"
ColorEntry* ColorTable_Instance() {
  static ColorEntry color_table[COLORTABLE_COUNT]=
  {
    {RGB(0,0,0),_T("黑色")},
    {RGB(0x99,0x33,0x00),_T("褐色")},
    {RGB(0x33,0x33,0),_T("橄榄色")},
    {RGB(0,0x33,0),_T("深绿")},
    {RGB(0,0x33,0x66),_T("深青")},
    {RGB(0,0,0x80),_T("深蓝")},
    {RGB(0x33,0x33,0x99),_T("靛蓝")},
    {RGB(0x33,0x33,0x33),_T("灰色-80%")},
    
    {RGB(0x80,0,0),_T("深红")},
    {RGB(0xff,0x66,0),_T("橙色")},
    {RGB(0x80,0x80,0),_T("深黄")},
    {RGB(0,0x80,0),_T("绿色")},
    {RGB(0,0x80,0x80),_T("青色")},
    {RGB(0,0,0xff),_T("蓝色")},
    {RGB(0x66,0x66,0x99),_T("蓝-灰")},
    {RGB(0x80,0x80,0x80),_T("灰色-50%")},
    
    {RGB(0xff,0,0),_T("红色")},
    {RGB(0xff,0x99,0),_T("浅橙色")},
    {RGB(0x99,0xcc,0),_T("酸橙色")},
    {RGB(0x33,0x99,0x66),_T("海绿")},
    {RGB(0x33,0xcc,0xcc),_T("水绿色")},
    {RGB(0x33,0x66,0xff),_T("浅蓝")},
    {RGB(0x80,0,0x80),_T("紫罗兰")},
    {RGB(0x96,0x96,0x96),_T("灰色-40%")},
    
    {RGB(0xFF,0,0xFF),_T("粉红")},
    {RGB(0xff,0xcc,0),_T("金色")},
    {RGB(0xff,0xff,0),_T("黄色")},
    {RGB(0,0xff,0),_T("鲜绿")},
    {RGB(0,0xff,0xff),_T("青绿")},
    {RGB(0,0xcc,0xff),_T("天蓝")},
    {RGB(0x99,0x33,0x66),_T("梅绿")},
    {RGB(0xc0,0xc0,0xc0),_T("灰色-25%")},
    
    {RGB(0xff,0x99,0xcc),_T("玫瑰红")},
    {RGB(0xff,0xcc,0x99),_T("茶色")},
    {RGB(0xff,0xff,0x99),_T("浅黄")},
    {RGB(0xcc,0xff,0xcc),_T("浅绿")},
    {RGB(0xcc,0xff,0xff),_T("浅青绿")},
    {RGB(0x99,0xcc,0xff),_T("淡蓝")},
    {RGB(0xcc,0x99,0xff),_T("淡紫")},
    {RGB(0xff,0xff,0xff),_T("白色")},
    
    {RGB(0x99,0x99,0xff),_T("海螺")},
    {RGB(0x99,0x33,0x66),_T("梅红")},
    {RGB(0xff,0xff,0xcc),_T("象牙色")},
    {RGB(0xcc,0xff,0xcc),_T("浅青绿")},
    {RGB(0x66,0,0x66),_T("深紫")},
    {RGB(0xff,0x80,0x80),_T("珊瑚红")},
    {RGB(0,0x66,0xcc),_T("海蓝")},
    {RGB(0xcc,0xcc,0xff),_T("冰蓝")},
    
    /*
    {RGB(0,0,0x80),_T("深蓝")},
    {RGB(0xff,0,0xff),_T("粉红")},
    {RGB(0xff,0xff,0),_T("黄色")},
    {RGB(0,0xff,0xff),_T("青绿")},
    {RGB(0x80,0,0x80),_T("紫罗兰")},
    {RGB(0x80,0,0),_T("深红")},
    {RGB(0,0x80,0x80),_T("青色")},
    {RGB(0,0,0xff),_T("蓝色")},
    */
  };
  static int inited=0;
  if (0==inited) {
    size_t i;
    const size_t cnt=COLORTABLE_COUNT;
    inited=1;
    for( i=0;i<cnt;++i){
      color_table[i].lhs=RGBtoHLS(color_table[i].color);
    }
  }
  return color_table;
}

enum NamedColorIndex
{
  WHITE_COLOR=39,
        BLACK_COLOR=0,
};
//给定一个RGB颜色值，返回颜色表的索引值，如果找不到
//相等的颜色，则按最相近的颜色返回

typedef struct HLSDelta
{
  TIndex dh,dl,ds,index;
}HLSDelta;

bool HLSDelta_less(HLSDelta lhs, HLSDelta rhs)
{
  if(lhs.dh<rhs.dh)  
    return true;
  else if(lhs.dh>rhs.dh)
    return false;
  else{
    if(lhs.dl<rhs.dl) 
      return true;
    else if(lhs.dl>rhs.dl)
      return false;
    else{
      return lhs.ds<rhs.ds;
    }
  }
}
//给定一个RGB颜色值，返回颜色表的索引值，如果找不到
//相等的颜色，则按最相近的颜色返回
TIndex find_color(COLORREF clr)
{
  TIndex i;
  HLSDelta minitem;
  HLS lhs0;
  ColorEntry* color_table = ColorTable_Instance();
  const TIndex cnt=COLORTABLE_COUNT;
  for( i=0;i<cnt;++i)
  {
    if(clr==color_table[i].color)
      return i;
  }
  lhs0 =RGBtoHLS(clr);
  minitem.dh=minitem.dl=minitem.ds=minitem.index=255;
  for(i=0;i<cnt;++i)
  {
    HLS lhs;
    HLSDelta item;
    item.index=i;
    lhs =color_table[i].lhs;
    item.dh=lhs.H>lhs0.H?lhs.H-lhs0.H:lhs0.H-lhs.H;
    item.dl=lhs.L>lhs0.L?lhs.L-lhs0.L:lhs0.L-lhs.L;
    item.ds=lhs.S>lhs0.S?lhs.S-lhs0.S:lhs0.S-lhs.S;
    if (HLSDelta_less(item, minitem)) {
      minitem = item;
    }
  }
  return minitem.index;
}
COLORREF get_color(TIndex index)
{
  ColorEntry* color_table = ColorTable_Instance();
  //_ASSERT(index<p.second);
  if(index>=COLORTABLE_COUNT)
        index=0;
  return color_table[index].color;
}


//USHORT clrIndex_:6;//颜色索引最大56
//USHORT lineStyle_:3;//真实类型：EXlLineStyle共8种
//USHORT weight_:2;//Border的宽度【0-3】
//6位color 3位 lineStyle 7位weight
typedef USHORT Border;

enum
{COLOR_MASK=0xFC00,
COLOR_SHIFT=10,
LINESTYLE_MASK=0x380,
LINESTYLE_SHIFT=7,
WEIGHT_MASK=0x7F,
};

COLORREF get_Color(Border data_)
{
  BYTE const clrIdx=(COLOR_MASK & data_)>>COLOR_SHIFT;
  _ASSERT(clrIdx<COLORTABLE_COUNT);
  return get_color(clrIdx);
  //return get_color(clrIndex_);
}
#define Border_put_ColorIndex(data_, idx) { _ASSERT(idx<COLORTABLE_COUNT); if(idx<COLORTABLE_COUNT){ data_=(data_&~COLOR_MASK)|((idx&0x3F)<<COLOR_SHIFT); } }
#define Border_get_LineStyle(data_)  (EXlLineStyle)((data_&LINESTYLE_MASK)>>LINESTYLE_SHIFT)
#define Border_put_LineStyle(data_, ls)  {  _ASSERT(ls<8);  data_=!ls?0:((data_&~LINESTYLE_MASK)|((ls&0x7)<<LINESTYLE_SHIFT));}
#define Border_get_Weight(data_)   (BYTE)(data_ & WEIGHT_MASK)
#define Border_put_Weight(data_, weight)  {_ASSERT(weight<4);  data_=(data_&~WEIGHT_MASK)|(weight&WEIGHT_MASK);}
#define Border_IsNull(data_)   (!Border_get_LineStyle(data_))
#define Border_SetNull(data_)  (data_=0)

typedef struct CellBorder
{
Border left,top,right,bottom,DiagonalRight,DiagonalLeft;
}CellBorder;

#define CellBorder_clear(cb)  memset(&cb, 0, sizeof(cb))

#define CellBorder_empty(cb)   (IsNull(cb.left) && IsNull(cb.top) && IsNull(cb.right) && IsNull(cb.bottom)&&IsNull(cb.DiagonalRight)&&IsNull(cb.DiagonalLeft))

int max_weight(CellBorder cb)
{
  const int max1=max(max(Border_get_Weight(cb.left),Border_get_Weight(cb.right)),max(Border_get_Weight(cb.top),Border_get_Weight(cb.bottom)));
  const int max2=max(Border_get_Weight(cb.DiagonalRight),Border_get_Weight(cb.DiagonalLeft));
  return max(max1,max2);
}

HPEN MyCell_CreatePen(HDC hDC,Border bdr,int nWidth)//HDC hDC,EXlLineStyle ls,int nWidth,COLORREF crColor)
{
  HPEN hPen=NULL;
  LOGBRUSH lbr;
  int logPixelSY;
  EXlLineStyle ls;
  BYTE weight;
  lbr.lbColor=get_Color(bdr);
  lbr.lbStyle=BS_SOLID;
  lbr.lbHatch=0;
  logPixelSY =GetDeviceCaps(hDC, LOGPIXELSY);
  weight =(BYTE)((logPixelSY/72.0f)*nWidth);
  if(nWidth>0 && !weight)
    weight=1;
  ls =Border_get_LineStyle(bdr);
  switch(ls)
  {
  case xlContinuous:
    {
      if(0==weight){
        DWORD style[]=
        {
          MulDiv(1,logPixelSY, 72),
            MulDiv(1,logPixelSY, 72)
        };
        hPen=ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style);
      }else{
        hPen=weight>1?ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_SQUARE|PS_SOLID,weight,&lbr,0,NULL):CreatePen(PS_SOLID,weight,lbr.lbColor);
      }
    }break;
  case xlDash:
    {
      DWORD style[]={3,2};//{9,4};
      if(2==weight){
        style[0]=9;
        style[1]=4;
      }
      hPen=ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style);
    }break;
  case xlDashDot:
    {
      DWORD style[]={3,4,9,4};
      hPen=ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style);
    }break;
  case xlDashDotDot:
    {
      DWORD style[]={3,4,3,4,9,4};
      hPen=ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style);
    }break;
  case xlDot:
    {
      DWORD style[]={1,3};
      hPen=ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style);
    }break;
  case xlDouble:
    {
    }break;
  case xlSlantDashDot:
    {
    }break;
  case xlLineStyleNone:
    {
    }break;
  }
  //  _ASSERT(NULL!=hPen);
  return hPen;
}

void Border_LineTo(Border border,HDC hDC,int x2,int y2)
{
  HDC dc=(hDC);
  LOGBRUSH lbr;
  int logPixelSY;
  EXlLineStyle ls;
  BYTE weight;
  lbr.lbColor=get_Color(border);
  lbr.lbStyle=BS_SOLID;
  lbr.lbHatch=0;
  logPixelSY =GetDeviceCaps(hDC, LOGPIXELSY);
  weight =(BYTE)((logPixelSY/72.0f)*Border_get_Weight(border));//MulDiv(border.get_Weight(),logPixelSY, 72);
  if(Border_get_Weight(border)>0 && !weight)
    weight=1;
  //BYTE const weight=border.get_Weight();
  ls=Border_get_LineStyle(border);
  switch(ls)
  {
  case xlContinuous:
    {
      HPEN hOldPen=NULL;
      if(0==weight){
        DWORD style[]= {MulDiv(1,logPixelSY, 72), MulDiv(1,logPixelSY, 72)};
        hOldPen=SelectPen(dc, ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style));
        //hOldPen=SelectPen(dc,ExtCreatePen(PS_COSMETIC|PS_ALTERNATE,1,&lbr,0,NULL));
      }else{
        HPEN hPen=weight>1?ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_SQUARE|PS_SOLID,weight,&lbr,0,NULL):CreatePen(PS_SOLID,weight,lbr.lbColor);
        //HPEN hPen=CreatePen(PS_SOLID,weight,lbr.lbColor);
        hOldPen=SelectPen(dc,hPen);
      }
      //MoveTo(dc,x1,y1);
      LineTo(dc, x2,y2);
      DeleteObject(SelectPen(dc,hOldPen));
    }break;
  case xlDash:
    {
      HPEN hPen;
      DWORD style[]={3,2};//{9,4};
      if(2==weight){
        style[0]=9;
        style[1]=4;
      }
    hPen = SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style));
      //MoveTo(dc,x1,y1);
      LineTo(dc,x2,y2);
      DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlDashDot:
    {
      DWORD style[]={3,4,9,4};
      HPEN hPen=SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style));
      //MoveTo(dc,x1,y1);
      LineTo(dc,x2,y2);
      DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlDashDotDot:
    {
      //DWORD style[]={3,4,3,4,9,4};
      DWORD style[]={2,3,2,3,8,3};
      HPEN hPen=SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style));
      //MoveTo(dc,x1,y1);
      LineTo(dc,x2,y2);
      DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlDot:
    {
      DWORD style[]={1,3};
      HPEN hPen=SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,weight,&lbr,sizeof(style)/sizeof(DWORD),style));
      //MoveTo(dc,x1,y1);
      LineTo(dc,x2,y2);
      DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlDouble:
    {
      MoveTo(dc,x2,y2);
      //HPEN hPen=SelectPen(dc,CreatePen(PS_SOLID,3,lbr.lbColor));
      //LineTo(dc,x2,y2);
      //DeleteObject(SelectPen(dc,hPen));
      //hPen=SelectPen(dc,CreatePen(PS_SOLID,1,RGB(0xff,0xff,0xff)));
      //LineTo(dc,x2,y2);
      //DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlSlantDashDot:
    {
      MoveTo(dc,x2,y2);
      //DWORD style[]={11,2,6,2};
      //HPEN hPen=SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,1,&lbr,sizeof(style)/sizeof(DWORD),style));
      //MoveTo(dc,x1+2,y1);
      //LineTo(dc,x2,y2-1);
      //DeleteObject(SelectPen(dc,hPen));
      
      //DWORD style1[]={10,3,4,3};
      //hPen=SelectPen(dc,ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,1,&lbr,sizeof(style1)/sizeof(DWORD),style1));
      //MoveTo(dc,lprc->left,yCenter);
      //LineTo(dc,lprc->right,yCenter);
      //DeleteObject(SelectPen(dc,hPen));
    }break;
  case xlLineStyleNone:
    {
      MoveTo(dc,x2,y2);
    }break;
  }
}

typedef CellBorder Borders;
//USHORT  cellType;//cellType类型表现ECellType以及用户自定义类型
//说明：bits位描述,位描述从高位到低位,1->8
//位    属性      描述
//[1-3]    Alignment    EXlVAlign
//[4-6]    Alignment    EXlHAlign 
//[7-12]  BackColor    BackColor的索引
//[13]    IsOwnerDraw    是否自绘制
//[14-19]  font      TextColor的索引
//[20-32]    --        保留
typedef struct StyleDesc {
  StrID_t nStyleID;
  UINT    hAlign:3;
  UINT    vAlign:3;
  UINT    bWrapText:1;
  UINT    clrBack:6;
  //UINT    isOwnerDraw:1;
  UINT    TextColorIndex:6;
  UINT    BackColorIndex:6;
  LOGFONT logFont;
  Borders borders;
} StyleDesc;

#define StyleDesc_clear(sd) ZeroMemory(&sd,sizeof(sd))

#define StyleDesc_eq(lhs, rhs)  0==memcmp(&lhs,&rhs,sizeof(lhs))
//----------------------align about--------------
#define Align_GetHorizontal(sd)   (EXlHAlign)(sd).hAlign
#define Align_SetHorizontal(sd, ha)  (sd).hAlign=ha
#define Align_GetVertical(sd)   (EXlVAlign)(sd).vAlign
#define Align_SetVertical(sd, va)  (sd).vAlign=va
#define Align_IsWrapText(sd)   (sd).bWrapText
#define Align_SetWrapText(sd, wrap)  (sd).bWrapText=wrap
#define BKGND_GetColor(sd)  get_color(BKGND_GetColorIndex(sd))
#define BKGND_GetColorIndex(sd)   (BYTE)(sd).BackColorIndex
#define BKGND_SetColorIndex(sd, idx)  ((sd).BackColorIndex=idx, BKGND_HasColor(sd))
#define BKGND_SetColor(sd,clr)  BKGND_SetColorIndex(sd, find_color(clr))
#define BKGND_HasColor(sd)   BKGND_GetColorIndex(sd)<COLORTABLE_COUNT?true:false
#define BKGND_ClearColor(sd)  BKGND_SetColorIndex(sd, 63)
#define Font_GetLogFont(sd)   (sd).logFont
#define Font_IsNull(sd)  (0==(sd).logFont.lfFaceName[0])
HFONT Font_GetHFont_(StyleDesc* sd, HDC hDC)
{
  HFONT hFont;
  LONG lfHeight;
  int mode;
  if(Font_IsNull(*sd))
    return NULL;
  lfHeight =sd->logFont.lfHeight;
  mode=SetMapMode(hDC,MM_TEXT);
  if(MM_TEXT==GetMapMode(hDC))
    ((LOGFONT*)&sd->logFont)->lfHeight=-MulDiv(sd->logFont.lfHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
  hFont =CreateFontIndirect(&sd->logFont);
  if(mode)
    SetMapMode(hDC,mode);
  ((LOGFONT*)&sd->logFont)->lfHeight=lfHeight;
  return hFont;
}
#define Font_GetColorIndex(sd)  (TIndex)(sd).TextColorIndex
#define Font_GetColor(sd)   get_color(Font_GetColorIndex(sd))
#define Font_SetColorIndex(sd, idx)  (sd).TextColorIndex=idx
#define Font_SetColor(sd, clr)  Font_SetColorIndex(find_color(clr));
#define Font_IsBold(sd)   (FW_BOLD==(sd).logFont.lfWeight)
#define Font_SetBold(sd, bold)  (sd).logFont.lfWeight=bold?FW_BOLD:FW_NORMAL
#define GetBorders()   &(sd).borders

void Style_CopyPatternProperties(StyleDesc* t,const StyleDesc* f,UINT nPatternMask/*=0*/)
{
  if(!BKGND_HasColor(*f))
    BKGND_ClearColor(*t);
  else
    BKGND_SetColorIndex(*t, BKGND_GetColorIndex(*f));
}
void Style_CopyAlignProperties(StyleDesc* t,const StyleDesc* f,UINT nAlignmentSetMask)
{
  if(EASM_WRAPTEXT&nAlignmentSetMask)
    Align_SetWrapText(*t, Align_IsWrapText(*f));
  if(EASM_HORIZONTAL&nAlignmentSetMask)
    Align_SetHorizontal(*t, Align_GetHorizontal(*f));
  if(EASM_VERTICAL&nAlignmentSetMask)
    Align_SetVertical(*t, Align_GetVertical(*f));
}
void Style_CopyFontProperties(StyleDesc* t,const StyleDesc* f,UINT nFontSetMask)
{
  //t.Font_GetLogFont()=f.Font_GetLogFont();
  if(EFSM_FONT_NAME&nFontSetMask)
    lstrcpy(Font_GetLogFont(*t).lfFaceName,Font_GetLogFont(*f).lfFaceName);
  
  if(EFSM_FONT_SIZE&nFontSetMask)
    Font_GetLogFont(*t).lfHeight=Font_GetLogFont(*f).lfHeight;
  
  if(EFSM_FONT_COLOR&nFontSetMask)
    Font_SetColorIndex(*t, Font_GetColorIndex(*f));
  
  if(EFSM_FONT_ITALIC&nFontSetMask)
    Font_GetLogFont(*t).lfItalic=Font_GetLogFont(*f).lfItalic;
  
  if(EFSM_FONT_BOLD&nFontSetMask)
    Font_SetBold(*t, Font_IsBold(*f));
  
  if(EFSM_FONT_UNDERLINE&nFontSetMask)
    Font_GetLogFont(*t).lfUnderline=Font_GetLogFont(*f).lfUnderline;
  
  if(EFSM_FONT_STRIKEOUT&nFontSetMask)
    Font_GetLogFont(*t).lfStrikeOut=Font_GetLogFont(*f).lfStrikeOut;
}

enum ECalcCellAxisInfoMask
{
  CCAI_CALC_HAXIS=1<<0,
    CCAI_CALC_VAXIS=1<<1,
    CCAI_CALC_HREMAINAXIS=1<<2,
    CCAI_CALC_VREMAINAXIS=1<<3,
    CCAI_CALC_ALL=0xFF,
};

//参见doc/MyCell坐标轴及区间图.png
typedef struct CellAxisInfo
{
  short VHeaderAxis;
  short VFreezeAxis;
  short VRemainAxis;
  short VClientAxis;
  short HHeaderAxis;
  short HFreezeAxis;
  short HRemainAxis;
  short HClientAxis;
}CellAxisInfo;
//包括1区的客户区坐标
#define get_HeaderAlthonogalRect(rc)    (SetRect(&rc,0,0,VHeaderAxis,HHeaderAxis), rc)
//包括2区的客户区坐标
#define get_HeaderHFreezeRect(rc)    (SetRect(&rc,VHeaderAxis,0,VFreezeAxis,HHeaderAxis), rc)
//包括3区的客户区坐标
#define get_HeaderHScrollRect(rc)    (SetRect(&rc,VFreezeAxis,0,VRemainAxis,HHeaderAxis), rc)
//包括5区的客户区坐标
#define get_HeaderVFreezeRect(rc)    (SetRect(&rc,0,HHeaderAxis,VHeaderAxis,HFreezeAxis), rc)
//包括9区的客户区坐标
#define get_HeaderVScrollRect(rc)    (SetRect(&rc,0,HFreezeAxis,VHeaderAxis,HRemainAxis), rc)
//包括6区的客户区坐标
#define get_FreezeAlthonogalRect(rc)    (SetRect(&rc,VHeaderAxis,HHeaderAxis,VFreezeAxis,HFreezeAxis), rc)
//包括7，8区的客户区坐标
#define get_FreezeHScrollRect(rc)    (SetRect(&rc,VFreezeAxis,HHeaderAxis,VClientAxis,HFreezeAxis), rc)
//包括10，14区的客户区坐标
#define get_FreezeVScrollRect(rc)    (SetRect(&rc,VHeaderAxis,HFreezeAxis,VFreezeAxis,HClientAxis), rc)

//返回客户区坐标，
//  其bottom和right与客户区的bottom和right相同。
//包括11，12，15，16区的客户区坐标
#define get_ScrollRect(rc)    (SetRect(&rc,VFreezeAxis,HFreezeAxis,VClientAxis,HClientAxis), rc)
//将其构造函数保护起来;将Worksheet设为friend class
//该对象由Worksheet负责生成，a Worksheet has a CellAxisInfo
//构造函数不做任何操作，正确的结构内容通过
//Worksheet::RecalcCellAxisInfo方法获得。

typedef enum ePos 
{ CRP_LEFTTOP,
CRP_RIGHTTOP,
CRP_RIGHTBOTTOM,
CRP_LEFTBOTTOM,
}ePos;

#define CellRange_IsNormalize(cr) ((cr).leftTop.row<=(cr).rightBottom.row && (cr).leftTop.col<=(cr).rightBottom.col)
#define CellRange_cols(cr) (_ASSERT(CellRange_IsNormalize(cr)), rightBottom.col-leftTop.col+1)
#define CellRange_rows(cr) (_ASSERT(CellRange_IsNormalize(cr)), rightBottom.row-leftTop.row+1)
ePos GetFisrtCellPos(CellRange cr)
{
  ePos ep;
  if(cr.leftTop.col<=cr.rightBottom.col){
    ep=cr.leftTop.row<=cr.rightBottom.row?CRP_LEFTTOP:CRP_LEFTBOTTOM;
  }else{
    ep=cr.leftTop.row<=cr.rightBottom.row?CRP_RIGHTTOP:CRP_RIGHTBOTTOM;
  }
  return ep;
}
#define SetCellRange(cr, firstRow, firstCol, lastRow, lastCol)  ( (cr).leftTop.row=firstRow,(cr).leftTop.col=firstCol,(cr).rightBottom.row=lastRow,(cr).rightBottom.col=lastCol)
CellRange CCellRange(Cell topLeft,Cell rightBottom)//:leftTop(topLeft),rightBottom(rightBottom)
{
  CellRange cr;
  cr.leftTop=topLeft;
  cr.rightBottom=rightBottom;
  return cr;
}
CellRange CCellRange_RECT(RECT rc)
{
  CellRange cr;
  SetCellRange(cr, rc.top,rc.left,rc.bottom,rc.right);
  return cr;
}
#define CellRange_Inflate(cr, dx,dy)  (_ASSERT(CellRange_IsNormalize(*pcr)), SetCellRange(&cr, cr.leftTop.row-dy,leftTop.col-dx,cr.rightBottom.row+dy,cr.rightBottom.col+dx))
#define CellRange_SetRowPair(cr, topRow, bottomRow)  (cr.leftTop.row=topRow, cr.rightBottom.row=bottomRow)
#define CellRange_SetColPair(cr, leftCol,rightCol)   (cr.leftTop.col=leftCol,cr.rightBottom.col=rightCol)
#define CellRange_IsNull(cr)  cr.leftTop.row==-10 && cr.leftTop.col==-10 && cr.rightBottom.row==-10 && cr.rightBottom.col==-10;
#define CellRange_SetNull(cr)  SetCellRange(cr, -10,-10,-10,-10)
#define CellRange_LeftCol(cr)  cr.leftTop.col
#define CellRange_RightCol(cr)  cr.rightBottom.col
#define CellRange_TopRow(cr)  cr.leftTop.row
#define CellRange_BottomRow(cr)  cr.rightBottom.row
#define CellRange_TopLeft(cr)  cr.leftTop
#define CellRange_RightBottom(cr)  cr.rightBottom
//修改左上角的值，并返回规则化后
//新的左上角
#define CellRange_SetTopLeft(cr, row,col)  (cr.leftTop.row=row,cr.leftTop.col=col,cr.leftTop)
//修改左上角的值，并返回规则化后
//新的左上角
#define CellRange_SetTopLeft2(cr,topLeft)  (cr.leftTop=topLeft)
#define CellRange_OffsetRow(cr, nDelta)              (cr.leftTop.row+=nDelta,cr.rightBottom.row+=nDelta,cr)
#define CellRange_OffsetCol(cr, nDelta)              (cr.leftTop.col+=nDelta,cr.rightBottom.col+=nDelta,cr)

//修改右下角的值，并返回规则化后
//新的右下角
#define CellRange_SetRightBottom(cr,row,col)  (cr.rightBottom.row=row,cr.rightBottom.col=col,cr)
//修改右下角的值，并返回规则化后
//新的右下角
#define CellRange_SetRightBottom2(cr, rightBottom)  (cr.rightBottom=rightBottom)
//修改左边列，并返回规则化后的左边列
#define CellRange_SetLeftCol(cr, col)  (cr.leftTop.col=col)
//修改右边列，并返回规则化后的右边列
#define CellRange_SetRightCol(cr, col)  (cr.rightBottom.col=col)
//修改上边行，并返回规则化后的上边行
#define CellRange_SetTopRow(cr, row)    cr.leftTop.row=row
//修改下边行，并返回规则化后的下边行
#define CellRange_SetBottomRow(cr, row) cr.rightBottom.row=row
#define CellRange_Normalize(cr)   { int minR,maxR,minC,maxC; \
  MinMax(leftTop.row,rightBottom.row,minR,maxR); \
  MinMax(leftTop.col,rightBottom.col,minC,maxC); \
  SetCellRange(cr, minR,minC,maxR,maxC); }

//是否完全在内部，即在内部但不等于边界
#define CellRange_FullInside(cr, row, col)   (_ASSERT(CellRange_IsNormalize(cr)), row>cr.leftTop.row && row<cr.rightBottom.row && col>cr.leftTop.col && col<cr.rightBottom.col)
#define CellRange_Inside(cr, row, col)       (_ASSERT(CellRange_IsNormalize(cr)), row>=cr.leftTop.row && row<=cr.rightBottom.row && col>=cr.leftTop.col && col<=cr.rightBottom.col)
#define CellRange_Inside2(cr, cell)   CellRange_Inside(cr, cell.row,cell.col)
#define CellRange_Inside3(me, cr)  (_ASSERT(CellRange_IsNormalize(me)),_ASSERT(CellRange_IsNormalize(cr)),CellRange_Inside(me,CellRange_TopRow(cr),CellRange_LeftCol(cr)) && CellRange_Inside(CellRange_BottomRow(cr),CellRange_RightCol(cr))
#define CellRange_ColInside(cr, col)  (_ASSERT(CellRange_IsNormalize(cr)), col>=cr.leftTop.col && col<=cr.rightBottom.col)
#define CellRange_ColFullInside(cr, col)  (col>cr.leftTop.col && col<cr.rightBottom.col)
#define CellRange_RowFullInside(cr, row)  (_ASSERT(CellRange_IsNormalize(cr)), row>cr.leftTop.row && row<cr.rightBottom.row)
#define CellRange_RowInside(cr, row)     (_ASSERT(CellRange_IsNormalize(cr)), row>=cr.leftTop.row && row<=cr.rightBottom.row)
//取该范围的边界集合，见doc/图解CCellRange的
//边界集合.png
//void GetEdges(vector<CCellRange>& vecEdges);
#define CellRange_IsIntersect(cr, rhs)   (_ASSERT(CellRange_IsNormalize(cr)),_ASSERT(CellRange_IsNormalize(rhs)), \
(cr.rightBottom.row<rhs.leftTop.row || cr.leftTop.row>rhs.rightBottom.row || cr.rightBottom.col<rhs.leftTop.col || cr.leftTop.col>rhs.rightBottom.col)?FALSE:TRUE)

//最后改变cr的值，因此rhs和cr可以是同一个对象
#define CellRange_Xor(lhs,rhs,vec)    (CellRange_Subtract(lhs,rhs,vec),CellRange_Subtract(rhs,lhs,vec))
#define CellRange_Xor(lhs,rhs,vec)    (CellRange_Subtract(lhs,rhs,vec),CellRange_Subtract(rhs,lhs,vec))
#define CellRange_CopyTo(cr, rc)     (rc.left=cr.leftTop.col, rc.top=cr.leftTop.row,rc.right=cr.rightBottom.col,rc.bottom=cr.rightBottom.row)

//最后改变cr的值，因此rhs和cr可以是同一个对象
BOOL CellRange_GetInterset(CellRange* this,CellRange* rhs,CellRange* cr)
{
  int minR=this->leftTop.row,maxR=this->rightBottom.row,minC=this->leftTop.col,maxC=this->rightBottom.col;
  int minR1=rhs->leftTop.row,maxR1=rhs->rightBottom.row,minC1=rhs->leftTop.col,maxC1=rhs->rightBottom.col;
  BOOL const bRet=(maxR<minR1 || minR>maxR1 || maxC<minC1 || minC>maxC1)?FALSE:TRUE;
  _ASSERT(CellRange_IsNormalize(*this));
  _ASSERT(CellRange_IsNormalize(*rhs));
  if(bRet)
    SetCellRange(*cr, max(minR,minR1),max(minC,minC1),min(maxR,maxR1),min(maxC,maxC1));
  else
    CellRange_SetNull(*cr);
  return bRet;
}

#if 0
Cell CCell(int r, int c) {Cell ce; ce.row=r,ce.col=c;return ce;}
void CCellRange_GetInterset(CellRange* rhs,vector<CCell>& vec)
{
  _ASSERT(this->IsNormalize());
  _ASSERT(rhs->IsNormalize());
  for(int r=leftTop.row;r<=rightBottom.row;++r){
    for(int c=leftTop.col;c<=rightBottom.col;++c){
      if(rhs.Inside(r,c))
        vec.push_back(CCell(r,c));
    }
  }
}
void CCellRange::Subtract(CCellRange const& rhs,vector<CCell>& vec)const
{
  _ASSERT(rhs.IsNormalize());
  _ASSERT(IsNormalize());
  if(!IsNull()){
    for(int r=leftTop.row;r<=rightBottom.row;++r){
      for(int c=leftTop.col;c<=rightBottom.col;++c){
        if(!rhs.Inside(r,c)){
          vec.push_back(CCell(r,c));
        }
      }
    }
  }
}
CCellRange& CCellRange::Union(CCellRange const& rhs)
{
  _ASSERT(IsNormalize());
  _ASSERT(rhs.IsNormalize());
  
  if(rhs.IsNull()){
    return *this;
  }else if(IsNull()){
    *this=rhs;
  }else{
    if(leftTop.row>rhs.leftTop.row)
      leftTop.row=rhs.leftTop.row;
    if(rightBottom.row<rhs.rightBottom.row)
      rightBottom.row=rhs.rightBottom.row;
    if(leftTop.col>rhs.leftTop.col)
      leftTop.col=rhs.leftTop.col;
    if(rightBottom.col<rhs.rightBottom.col)
      rightBottom.col=rhs.rightBottom.col;
  }
  return *this;
}
CCellRange& CCellRange::Union(int row,int col)
{
  _ASSERT(IsNormalize());
  if(leftTop.row<=rightBottom.row){
    if(row<leftTop.row)
      leftTop.row=row;
    else if(row>rightBottom.row)
      rightBottom.row=row;
  }else{
    if(row>leftTop.row)
      leftTop.row=row;
    else if(row<rightBottom.row)
      rightBottom.row=row;
  }
  if(leftTop.col<=rightBottom.col){
    if(col<leftTop.col)
      leftTop.col=col;
    else if(col>rightBottom.col)
      rightBottom.col=col;
  }else{
    if(col>leftTop.col)
      leftTop.col=col;
    else if(col<rightBottom.col)
      rightBottom.col=col;
  }
  return *this;
}
void CCellRange::Union(CCellRange const& rhs,vector<CCell>& vec)const
{
  _ASSERT(IsNormalize());
  _ASSERT(rhs.IsNormalize());
  CCell cell;
  for(int r=leftTop.row;r<=rightBottom.row;++r){
    for(int c=leftTop.col;c<=rightBottom.col;++c){
      cell.set(r,c);
      vec.push_back(cell);
    }
  }
  for(int r=rhs.leftTop.row;r<=rhs.rightBottom.row;++r){
    for(int c=rhs.leftTop.col;c<=rhs.rightBottom.col;++c){
      //CCell cell={r,c};
      cell.set(r,c);
      if(vec.end()==find(vec.begin(),vec.end(),cell)){
        vec.push_back(cell);
      }
    }
  }
}

void CCellRange::Extract(vector<CCell>& vec)const
{
  _ASSERT(IsNormalize());
  for(int r=TopRow();r<=BottomRow();++r){
    for(int c=LeftCol();c<=RightCol();++c){
      vec.push_back(CCell(r,c));
    }
  }
}
void CCellRange::Subtract(CCellRange const& rhs,vector<CCellRange>& vec)const
{
  _ASSERT(IsNormalize());
  _ASSERT(rhs.IsNormalize());
  if(IsNull())
    return;
  if(rhs.IsNull()){
    vec.push_back(*this);
    return;
  }
  if(IsIntersect(rhs)){
    CCellRange cr=*this;
    if (cr.leftTop.row < rhs.leftTop.row)
    {
      vec.push_back(CCellRange(cr.leftTop.row, cr.leftTop.col, rhs.leftTop.row-1, cr.rightBottom.col));
      cr.leftTop.row=rhs.leftTop.row;
    }
    if (cr.rightBottom.row > rhs.rightBottom.row)
    {
      vec.push_back(CCellRange(rhs.rightBottom.row+1,cr.leftTop.col, cr.rightBottom.row,  cr.rightBottom.col));
      cr.rightBottom.row=rhs.rightBottom.row;
    }
    if (cr.leftTop.col < rhs.leftTop.col)
    {
      vec.push_back(CCellRange(cr.leftTop.row,cr.leftTop.col, cr.rightBottom.row,  rhs.leftTop.col-1));
      cr.leftTop.col=rhs.leftTop.col;
    }
    if (cr.rightBottom.col > rhs.rightBottom.col)
    {
      vec.push_back(CCellRange(cr.leftTop.row,rhs.rightBottom.col+1, cr.rightBottom.row,  cr.rightBottom.col));
      cr.rightBottom.col=rhs.rightBottom.col;
    }
  }else{
    vec.push_back(*this);
  }
}
struct Ranges
{
  vector<CCellRange> vec_;
  BOOL Inside(CCell cell)
  {
    for(vector<CCellRange>::const_iterator it=vec_.begin();it!=vec_.end();++it){
      if(it->Inside(cell))
        return TRUE;
    }
    return FALSE;
  }
};
//typedef CCellRange CMergeCell;
class CMergeCell : public CCellRange
{
  CellStore cellStore;
  
  CMergeCell()
  {
  }
  CMergeCell& operator=(const CMergeCell& mc)
  {
    SetTopLeft(mc.TopLeft());
    SetRightBottom(mc.RightBottom());
    cellStore=mc.cellStore;
    return *this;
  }
  void Attach(CMergeCell& mc)
  {
    SetTopLeft(mc.TopLeft());
    SetRightBottom(mc.RightBottom());
    cellStore.Attach(mc.cellStore);
  }
  const CellStore& GetCellStore()
  {
    return cellStore;
  }
  void SetCellType(ECellType cellType)
  {
    _ASSERT(cellType<ECT_MAX);
    cellStore.SetCellType(cellType);
  }
  StyleID_t SetStyle(StyleStore& ss,StyleDesc style)
  {
    return cellStore.SetStyle(ss,style);
  }
  void ClearStyle(StyleStore& ss)
  {
    cellStore.ClearStyle(ss);
  }
  void SetData(VARIANT val)
  {
    cellStore.SetData(val);
  }
  void AttachData(VARIANT& val)
  {
    cellStore.GetData().Attach(&val);
  }
  void DetachData(VARIANT* pval)
  {
    cellStore.GetData().Detach(pval);
  }
  void Clear(StyleStore& ss)
  {
    ClearStyle(ss);
    cellStore.clear();
  }
};
#endif

#define get_bit(bits, bit)       (((bits) & (bit))?TRUE:FALSE)
#define set_bit(bits, bit, b)    ((b)?(bits)|=(bit):(bits)&=~(bit))
enum eCellBits {B_SELECTION=1<<0};
typedef struct CellStore {
  StrID_t nStyleID;   // 单元格的风格ID
  StrID_t cellVal;    // 单元格的值
  UINT cellType;      // cellType类型表现ECellType以及用户自定义类型
  int nIndex;
  int nMergeDown;
  int nMergeAcross;
  UINT bits;
} CellStore;
typedef struct _StylePack {
  StyleID_t nStyleID;
  ECellStorePosition eStorePos;
}StylePack;
typedef struct _CellTypePack {
		BYTE cellType;//ECellType
    ECellStorePosition eStorePos;
}CellTypePack;
typedef struct _CellValPack {
  const CComVariant* pVal;
  ECellStorePosition eStorePos;
}CellValPack;
typedef enum ECellStoreMask
{ ECSM_CELLDATA_VALID=0,
ECSM_STYLE_VALID=1<<0,
ECSM_CELLTYPE_VALID=1<<1
} ECellStoreMask;
typedef struct _CellStorePack {
  StylePack		stylePack;
  CellTypePack	cellTypePack;
  CellValPack	cellValPack;
  //CMergeCell* pMerge;
}CellStorePack;
typedef struct _RowHeader {
  int nIndex;
  StrID_t nStyleID;
  BOOL bAutoFitHeight;
  int nHeight;
  UINT bits;
} RowHeader;
typedef struct _ColHeader {
  StrID_t  nStyleID;  //单元格的风格ID
  int nWidth;
  int nSpan;
  int nIndex;
  UINT bits;
} ColHeader;
enum eBits
{B_SHOWROWHEADER=1<<0,
B_SHOWCOLHEADER=1<<1,
B_SHOWGRID=1<<2,
B_AllowColumnResize=1<<3,
B_HandleTabKey=1<<4,
B_AutoAppendRow=1<<5,
//B_AllowColumEdit=1<<6,
B_SHOWSELECTION=1<<6,
//B_ALLOWMULTISELECTION=1<<7,
};
//typedef CellTypeMgr<Worksheet>  CellTypeMgr_t;
//typedef ActiveCell<Worksheet> ActiveCell_t;
typedef struct _Worksheet {
  DWORD bits;//位组合
  int nrh_, nch_, nCells;
  RowHeader* rh_;
  ColHeader* ch_;
  CellStore* Cells;
  int* pCell;
  BOOL bShowGrid;
  Cell activeCell_;
  int defHeight;
  int defWidth;
  int nRowCount;
  int nColCount;
  int nFullRows;
  int nFullColumns;
  UINT nStyleID;
  StrID_t nName;
} Worksheet;

typedef struct _Workbook {
  StyleDesc* Styles;
  int nStyles;
  Worksheet* Sheets;
  int nSheets;
  int nWindowHeight;
  int nWindowWidth;
  int nWindowTopX;
  int nWindowTopY;
  BOOL bProtectStructure;
  BOOL bProtectWindows;
} Workbook;

int Free_Workbook(Workbook* workbook) {
  int i;
  for (i=0; i<workbook->nSheets; ++i) {
    Worksheet* pSheet = workbook->Sheets+i;
    free_s(pSheet->rh_);
    free_s(pSheet->ch_);
    free_s(pSheet->Cells);
    free_s(pSheet->pCell);
  }
  free_s(workbook->Sheets);
  free_s(workbook->Styles);
  return 0;
}

#define put_ShowGrid(bShow)  (bShow?pSheet->bits|=B_SHOWGRID:pSheet->bits&=~B_SHOWGRID)
#define get_ShowGrid(pSheet)       (pSheet->bits&B_SHOWGRID?TRUE:FALSE)
#define put_ShowSelection(pSheet, bShow)    (bShow?bits|=B_SHOWSELECTION:pSheet->bits&=~B_SHOWSELECTION)
#define get_ShowSelection(pSheet)   (pSheet->bits&B_SHOWSELECTION?TRUE:FALSE)
#define get_AutoAppendRow(pSheet)     get_bit(pSheet->bits, B_AutoAppendRow)
#define put_AutoAppendRow(pSheet, b)  set_bit(pSheet->bits, B_AutoAppendRow, b)

#define get_RowHeight(pSheet, row)  pSheet->rh_[row].nHeight
//位于row行的所有列是否被选中
#define IsFullColSelectedAtRow(pSheet, row) get_bit(pSheet->rh_[row].bits, B_SELECTION)
#define IsFullRowSelectedAtCol(pSheet, col) get_bit(pSheet->ch_[col].bits, B_SELECTION)

#define CellInSelections(cell)    get_bit((cell).bits, B_SELECTION)
BOOL IsSelectAll(const Worksheet* pSheet) {
  int i, n = pSheet->nCells;
  for( i=0;i<n;++i) {
    if (!get_bit(pSheet->Cells[i].bits, B_SELECTION)) {
      return false;
    }
  }
  return true;
}
BOOL RowInSelections(const Worksheet* pSheet, int row) {
  int i, n = pSheet->nColCount;
  int* pCell = pSheet->pCell+row * n;
  for( i=0;i<n;++i) {
    if (!get_bit(pSheet->Cells[pCell[i]].bits, B_SELECTION)) {
      return false;
    }
  }
  return true;
}

CellStore* GetCell1(CellStore* Cells, int nCells, int col) {
  int i;
  for (i=0; i<nCells; ++i) {
    if (Cells[i].nIndex<=col && Cells[i].nIndex+Cells[i].nMergeAcross>=col) {
      return Cells+i;
    }
  }
  return NULL;
}

#define GetCellRow(RorH, col) GetCell1(RorH->Cells, RorH->nCells)

ColHeader* GetColHeader(const Worksheet* pSheet, int col) {
  int i;
  if (pSheet->ch_) {
    for (i=0; i<pSheet->nch_; ++i) {
      int scol=pSheet->ch_[i].nIndex, ecol=pSheet->ch_[i].nIndex+pSheet->ch_[i].nSpan;
      if (scol<=col && ecol>=col) {
        return pSheet->ch_+i;
      }
    }
  }
  return NULL;
}
int get_ColWidth(const Worksheet* pSheet, int col) {
  ColHeader* colh = GetColHeader(pSheet, col);
  return colh ? colh->nWidth : pSheet->defWidth;
}

#define GetCell(pSheet, row, col)  ((row>0 && col>0 && row<pSheet->nColCount && col<pSheet->nColCount) ? pSheet->Cells + pSheet->pCell[row * pSheet->nColCount + col] : NULL)

BOOL ColInSelections(const Worksheet* pSheet, int col) {
  int i;
  CellStore* pCell;
  for( i=0;i<pSheet->nrh_;++i) {
    pCell=GetCell(pSheet, i, col);
    if (pCell==NULL || !get_bit(pCell[0].bits, B_SELECTION)) {
      return false;
    }
  }
  return true;
}

int get_RowHeaderText(const Worksheet* pSheet, int row, LPTSTR buf, int maxlen)
{
  int i=0;
  _snprintf(buf, maxlen, _T("%d"), row+1);
  for(;i!=MAX_CELLTEXT && buf[i];++i)
    ;
  return i;
}

int GetCellText(const CellStore* pCell, LPTSTR buf, int maxlen)
{
  int i=0;
  _snprintf(buf, maxlen, _T("%s"), idtostr(pCell->cellVal));
  for(;i!=MAX_CELLTEXT && buf[i];++i)
    ;
  return i;
}

int get_CowHeaderText(const Worksheet* pSheet, int col, LPTSTR buf, int maxlen)
{
  if(col<27){          //A~Z
    buf[0]=(col+0x40);
    buf[1]=0;
    return 1;
  } else if(col<703){        //AA~ZZ
    buf[1]=(col-1)%26+'A';  //0x41;
    buf[0]=(col-1)/26-1+'A';//0x40;
    buf[2]=0;
    return 2;
  }
  return 0;
}

//#define GetWorkbook(ws)   ws.pWorkbook_
//#define GetName(ws)   ws.szName_
