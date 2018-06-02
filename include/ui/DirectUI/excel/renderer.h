
enum {
  MASK_CELL_INCLIP=1<<16,
        MASK_CSPACK=1<<17,
};
typedef struct _CellItem {
  ECellBGShrinkState eShrink;
  RECT rcCell;          //单元格自身的范围，不考虑合并的情况
  //RECT rcRender;      //用于绘制该单元格的范围，也叫绘制剪裁区
  RECT rcMerge;         //单元格所处合并区的范围
  CellBorder cellBorder;
  UINT nMask;
  CellStorePack csPack;
} CellItem;
#define get_MergeRect(ci)  ci.rcMerge
#define put_MergeRect(ci, rc)  ci.rcMerge=rc
#define SetCSPack(ci, cs)  (ci.csPack=cs, ci.nMask|=MASK_CSPACK)
#define GetCSPack(ci)  ci.csPack
#define IsCSPackValid(ci)  ci.nMask&MASK_CSPACK
#define IsInClip(ci)  ci.nMask&MASK_CELL_INCLIP
#define SetInClip(ci)  ci.nMask|=MASK_CELL_INCLIP
#define SetLeftBorder(ci, bdr)    (ci.nMask|=xlEdgeLeft, ci.cellBorder.left=bdr)
#define SetTopBorder(ci, bdr)     (ci.nMask|=xlEdgeTop, ci.cellBorder.top=bdr)
#define SetRightBorder(ci, bdr)   (ci.nMask|=xlEdgeRight, ci.cellBorder.right=bdr)
#define SetBottomBorder(ci, bdr)  (ci.nMask|=xlEdgeBottom, ci.cellBorder.bottom=bdr)
#define ClearCellItem(ci)         (ci.eShrink=ECBSS_UNKNOWN, ci.rcMerge.left=ci.rcMerge.right=0, ci.nMask=0)
//该结构用在BeginPaint,EndPaint之间，其作用主要是
//用来一次性计算一整块单元格区域中各单元格
//的范围、合并范围、绘制剪裁范围。这样有利于
//减少不必要的重复计算。
#if 0
  pair<CellBorder,UINT> GetBorder()
  {
    return make_pair(cellBorder,nMask);
  }

  struct GridRenderer 
  {
    struct VisibleCellsSession
    {
      void Reset(int rows,int cols,int topRow,int leftCol)
      {
        vec_.clear();
        //vec_.reserve(rows*cols);
        vec_.resize((rows+2)*(cols+2));
        rows_=rows;
        cols_=cols;
        topLeft_.set(topRow,leftCol);
      }
      //void put_cols(int cols)
      //{
      //  cols_=cols;
      //}
      int get_cols()
      {
        return cols_;
      }
      int get_rows()
      {
        //_ASSERT(cols_>=0);
        return rows_;
        //_ASSERT(cols_>=0);
        //if(0==cols_)
        //  return 0;
        //int rows=(int)vec_.size()/cols_;
        //if((rows*cols_)<(int)vec_.size())
        //  ++rows;
        //return rows;
      }
      //void push_back(CellItem const& cell)
      //{
      //  vec_.push_back(cell);
      //}
      //CellItem& operator[](size_t i)
      //{
      //  _ASSERT(i<vec_.size());
      //  return vec_[i];
      //}
      void clear()
      {
        vec_.clear();
      }
      bool empty()
      {
        return vec_.empty();
      }
      Border GetLeftBorder(const Worksheet* pSheet,CCell cell);
      Border GetRightBorder(const Worksheet* pSheet,CCell cell);
      Border GetTopBorder(const Worksheet* pSheet,CCell cell);
      Border GetBottomBorder(const Worksheet* pSheet,CCell cell);
      CellBorder GetCellBorder(const Worksheet* pSheet,CCell cell)
      {
        CellBorder cellBorder;
        cellBorder.left=GetLeftBorder(pSheet,cell);
        cellBorder.right=GetRightBorder(pSheet,cell);
        cellBorder.top=GetTopBorder(pSheet,cell);
        cellBorder.bottom=GetBottomBorder(pSheet,cell);
        return cellBorder;
      }
      CellItem& item(CCell cell)
      {
        _ASSERT(cell.row>=TopRow()-1 && cell.row<=BottomRow()+1);
        _ASSERT(cell.col>=LeftCol()-1 && cell.col<=RightCol()+1);
        const int idx=(cell.row-(TopRow()-1))*(cols_+2)+(cell.col-(LeftCol()-1));
        _ASSERT(idx<int(vec_.size()));
        return vec_[idx];  
      }
      int TopRow()
      {
        return topLeft_.row;
      }
      int BottomRow()
      {
        return topLeft_.row+rows_-1;
      }
      int LeftCol()
      {
        return topLeft_.col;
      }
      int RightCol()
      {
        return topLeft_.col+cols_-1;
      }

    private:
      CCell topLeft_;
      int rows_,cols_;
      vector<CellItem>  vec_;
    };

  public:
    GridRenderer(CellTypeFactory& cellTypeFactory):cellTypeFactory_(cellTypeFactory),pSheet_(NULL)
    {
    }
    void Init(HWND hWndGrid);//Worksheet* pSheet);
    void draw(const Worksheet* pSheet,const CellAxisInfo* pcai,HDC hDC,LPCRECT lprcClient,const CRgnLight* pRgnClip);
    void SetDirty()
    {
      //session_.clear();
    }
  private:
    COLORREF GetBKColor(const Worksheet* pSheet,int row,int col,const CCellRange* pmerge,COLORREF bkColor);//StyleDesc const& style);
    void calc_cells_session(CRgnLight const* pRgnClip,int firstCol,int firstRow,int lastCol,int lastRow,LPCRECT lprcContent,VisibleCellsSession& session);
    ESelectionState SetupRowHeaderDE(const Worksheet* pSheet,const Selections* pCurSelections,int row,COLORREF& backColor);
    ESelectionState SetupColHeaderDE(const Worksheet* pSheet,const Selections* pCurSelections,int col,COLORREF& backColor);
    ECellSelectionRelations GetCellSelectionRelations(const Worksheet* pSheet,const CCellRange& cr,int row,int col);
  private:
    void draw_RowHeader(CDCHandle& dc,LPCRECT lprc,int firstRow,int lastRow,CRgnLight const* pRgnClip);
    void draw_ColHeader(CDCHandle& dc,LPCRECT lprc,int leftCol,int rightCol,CRgnLight const* pRgnClip);
    void draw_session(CDCHandle& dc,VisibleCellsSession& session,EGridPart ePart,const CCellRange& activeSelection);
    void draw_cell(CDCHandle dc,CCell cell,CellItem& item,const CCellRange& activeSelection,const Selections* pSelections,VisibleCellsSession& session);
    void draw_cell_inner(CDCHandle& dc,const CMergeCell* merge,int row,int col,/*DrawCellStruct*/DrawCellInfo& dcs,style::StyleDesc const& style);
    void draw_cell_shrink(const Worksheet* pSheet,int row,int col,const Selections* pCurSelections,CDCHandle& dc,const CCellRange& activeSelection,CellItem& cell,DrawCellInfo/*DrawCellStruct*/& dcs);//,CMergeCell* pmerge);
    void draw_cell_border(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,CellBorder cellBorder,const CMergeCell* pmerge);
    void draw_cell_border_right(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_left(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_bottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_top(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_rightbottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_leftbottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_righttop(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_border_lefttop(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder);
    void draw_cell_DragBounds(const Worksheet* pSheet,CDCHandle& dc,const CCellRange activeSelection,const CMergeCell* merge,RECT rcCell,int row,int col,int rows,int cols,COLORREF bkclr);
  private:
    CellTypeFactory& cellTypeFactory_;
    const Worksheet* pSheet_;
  };
  
#endif
