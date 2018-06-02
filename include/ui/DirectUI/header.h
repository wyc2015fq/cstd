
#pragma once
#include <map>
#include <vector>
//#include <atlstr.h>
#include "base.h"
#include "StylesMgr.h"
//#include "Cell2MapEventsImpl.h"
#include "coleventsimpl.h"
using namespace std;

  //class VaryRowHeightProcy
  //{
  //  enum eBits
  //  {
  //    B_AllowRowResize=1<<0,
  //  };
  //protected:
  //  VaryRowHeightProcy():rows_(100),defHeight_(19),bits_(B_AllowRowResize)
  //  {}
  //public:
  //  void put_DefRowHeight(int hi)
  //  {
  //    defHeight_=hi;
  //  }
  //  int get_DefRowHeight()const
  //  {
  //    return defHeight_;
  //  }
  //  //template<class T>
  //  int get_heights(/*int rows*/)const
  //  {
  //    int heights=(rows_ - int(mapIrregHeights_.size()))*get_DefRowHeight();
  //    for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it)
  //    {
  //      heights+=it->second;
  //    }
  //    return heights;
  //  }
  //  //[row1,row2)
  //  int get_DiffHeights(int row1,int row2)const
  //  {
  //    _ASSERT(row1>=0);
  //    if(row1>=row2) return 0;
  //    int heights=(row2-row1)*get_DefRowHeight();
  //    for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it)
  //    {
  //      if(it->first>=row1 && it->first<row2){
  //        heights+=it->second-get_DefRowHeight();
  //      }
  //    }
  //    return heights;
  //  }

  //  void put_RowHeight(int row,int hi)
  //  {
  //    _ASSERT(row>=0);
  //    _ASSERT(row<rows_);
  //    if(hi==defHeight_){
  //      map<row_t,short>::iterator it=mapIrregHeights_.find(row);
  //      if(it!=mapIrregHeights_.end())
  //        mapIrregHeights_.erase(it);
  //    }else{
  //      mapIrregHeights_[row]=hi;
  //    }
  //  }
  //  int get_RowHeight(int row)const
  //  {
  //    map<row_t,short>::const_iterator it=mapIrregHeights_.find(row);
  //    return it==mapIrregHeights_.end()?defHeight_:it->second;
  //  }
  //  BOOL get_AllowRowResize()const
  //  {
  //    return bits_&B_AllowRowResize?TRUE:FALSE;
  //  }
  //  void put_AllowRowResize(BOOL bAllow)
  //  {
  //    bAllow?bits_|=B_AllowRowResize:bits_&=~B_AllowRowResize;   
  //  }

  //  //返回不规则行的行号和行高，所谓不规则行指行高
  //  //不等于默认行高的行

  //  void GetAbnormalRowsInfo(vector<pair<row_t,short> >& vec)const
  //  {
  //    for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it){
  //      vec.push_back(*it);
  //    }
  //  }
  //protected:
  //  //delete range[begin,end];
  //  void delete_rows(int beg,int end)
  //  {
  //    int const cnt=end-beg+1;
  //    rows_-=cnt;
  //    mycell::delete_cols<row_t,short>(mapIrregHeights_,beg,end);
  //    _ASSERT((int)mapIrregHeights_.size()<=rows_);
  //  }
  //  void put_rows(int rows)
  //  {
  //    for(map<row_t,short>::iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();)
  //    {
  //      if(it->first>=rows){
  //        it=mapIrregHeights_.erase(it);
  //        continue;
  //      }
  //      ++it;
  //    }
  //  }
  //  //在beg前插入nCount行
  //  void insert_rows(int beg,int nCount)
  //  {
  //    rows_+=nCount;
  //    mycell::insert_cols<row_t,short>(mapIrregHeights_,beg,nCount);
  //    _ASSERT((int)mapIrregHeights_.size()<=rows_);
  //  }
  //protected:
  //  void clear()
  //  {
  //    rows_=0;
  //    mapIrregHeights_.clear();
  //  }
  //private:
  //  map<row_t,short> mapIrregHeights_;
  //  int  rows_;
  //  short    defHeight_,bits_;
  //};
  /*
  class FixedRowHeightPolicy
  {
    int rowHeight_;
  public:     
    FixedRowHeightPolicy():rowHeight_(19)
    {}
    int get_heights(int rows)const
    {
      return rows*rowHeight_;
    }
    //[row1,row2)
    int get_DiffHeights(int row1,int row2)const
    {
      return (row2-row1)*rowHeight_;
    }
    
    //与方法get_DiffHeights一样
    
    int get_DiffHeightsPlain(int row1,int row2)const
    {
      return get_DiffHeights(row1,row2);
    }

    void put_RowHeight(int row,int hi)
    {
    }
    int get_RowHeight(int row)const
    {       
      return rowHeight_;
    }
    int get_DefRowHeight()const
    {
      return rowHeight_;
    }
    void put_DefRowHeight(int hi)
    {
       rowHeight_=hi;
    }
    BOOL get_AllowRowResize()const
    {
      return FALSE;
    }
    void put_AllowRowResize(BOOL)
    {
    }
  protected:
    //void delete_row(int row)
    void delete_rows(int beg,int end)
    {
    }
    void put_rows(int rows)
    {
    }
    void insert_rows(int beg,int nCount)
    {
    }
  };
  */
  /*
  template<class T,class RowHeightProcy=VaryRowHeightProcy>
  class RowHeightMgr : public RowHeightProcy
  {
  protected:
    RowHeightMgr(){}
  public:
    int get_heights()const
    {
      T const* pT=static_cast<T const*>(this);
      return RowHeightProcy::get_heights(pT->get_rows());
    }
    //[row1,row2)
    int get_DiffHeights(int row1,int row2)const
    {
      return RowHeightProcy::get_DiffHeights(row1,row2);
    }
    int get_RowHeight(int row)const
    {       
      return RowHeightProcy::get_RowHeight(row);
    }
    int get_DefRowHeight()const
    {
      return RowHeightProcy::get_DefRowHeight();
    }
    void put_DefRowHeight(int hi)
    {
      RowHeightProcy::put_DefRowHeight(hi);
    }
  protected:
    void put_RowHeight(int row,int hi)
    {
      RowHeightProcy::put_RowHeight(row,hi);
    }
  };
  */
  //class RowHeader;
  //typedef RowHeightMgr<RowHeader> VaryRowHeightMgr;
  //typedef RowHeightMgr<RowHeader,FixedRowHeightPolicy> FixedRowHeightMgr;
//#ifdef MYCELL_USE_VARYROWHEADER
  //typedef VaryRowHeightProcy RowHeightMgrT;
//#else
//  typedef FixedRowHeightMgr RowHeightMgrT;
//#endif
  //struct RowHeaderEvents
  //{
  //  virtual void OnVisibleRowChanged(int oldTopRow,int oldBottomRow,int newTopRow,int newBottomRow)=0;
  //};

  class Worksheet;
  //class Grid;

  
  //本类中nScrollHeight的概念如无特别说明均
  //表示HClientAxis-HFreezeAxis
  
  class RowHeader// : public RowHeightMgrT
  {
    //typedef RowHeightMgrT baseClass;
    enum eBits
    {
      B_AllowRowResize=1<<0,
    };
    friend class Worksheet;
    //friend class Grid;
    friend class XmlExcel;
    //friend class HeaderDragImpl;
  public:
    RowHeader():width_(30),topVisScrollRow_(0),bottomVisScrollRow_(-1)
      ,freezeTopRow_(-1),freezeBottomRow_(-1),rows_(1),defHeight_(19)
      ,bits_(B_AllowRowResize)
    {
      vecRowStore_.resize(rows_);
    }
    int get_rows()const
    {
      return rows_;
    }

    void put_DefRowHeight(int hi)
    {
      defHeight_=hi;
    }
    int get_DefRowHeight()const
    {
      return defHeight_;
    }
    int get_heights()const;

    //[row1,row2)
    int get_DiffHeights(int row1,int row2)const;
    void put_RowHeight(int row,int hi);
    int get_RowHeight(int row)const
    {
      map<row_t,short>::const_iterator it=mapIrregHeights_.find(row);
      return it==mapIrregHeights_.end()?defHeight_:it->second;
    }
    BOOL get_AllowRowResize()const
    {
      return bits_&B_AllowRowResize?TRUE:FALSE;
    }
    void put_AllowRowResize(BOOL bAllow)
    {
      bAllow?bits_|=B_AllowRowResize:bits_&=~B_AllowRowResize;   
    }
    
    //返回不规则行的行号和行高，所谓不规则行指行高
    //不等于默认行高的行
    
    void GetAbnormalRowsInfo(vector<pair<row_t,short> >& vec)const
    {
      for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it){
        vec.push_back(*it);
      }
    }

    //取滚动区第一个可见行
    int get_TopVisScrollRow()const
    {
      return topVisScrollRow_;
    }
    int get_FreezeTopRow()const
    {
      return freezeTopRow_;
    }
    int get_FreezeBottomRow()const
    {
      return freezeBottomRow_;
    }
    bool IsFreezeRowVisible()const
    {
      return -1!=freezeBottomRow_;//freezeBottomRow_>=freezeTopRow_ && freezeBottomRow_>HEADER_ROW;
    }

    //冻结区freezeTopRow_发生改变将导致垂直滚动条发生改变，
    //topVisScrollRow_,bottomVisScrollRow_也可能发生改变，
    //该方法作为底层方法，此处不维护这些改变，
    //其改变的维护工作交由Worksheet负责。
    void put_FreezeTopRow(int nFreezeTopRow)
    {
      freezeTopRow_=nFreezeTopRow;
    }
    //注解与put_FreezeTopRow类似
    void put_FreezeBottomRow(int nFreezeBottomRow)
    {
      freezeBottomRow_=nFreezeBottomRow;
    }

    //取可滚动的最顶行
    int get_TopScrollableRow()const
    {
      return freezeBottomRow_+1;
    }
    int get_BottomScrollableRow()const
    {
      return rows_-1;
    }

    //int calc_TopVisScrollRow(int nScrollPos)const;
    int calc_TopVisScrollRow(int nBottomVisScrollRow,int nScrollHeight,BOOL bFullVisible)const;
    int calc_BottomVisScrollRow(int nTopVisScrollRow,int nScrollHeight/*LPCRECT lprcScroll*/)const;

    //取滚动区最后一个可见行
    int get_BottomVisScrollRow()const
    {
      //_ASSERT(-1!=bottomVisScrollRow_);
      return bottomVisScrollRow_;
    }
    void put_width(int wi)
    {
      width_=wi;
    }
    int get_MinWidth()const
    {
      return 30;
    }
    int get_width()const
    {
      return width_;
    }
    int get_text(int row,LPTSTR buf)const;
    void clear(StyleStore& ss);
    void ClearCellTypes()
    {
      for(vector<CellStore>::iterator it=vecRowStore_.begin();it!=vecRowStore_.end();++it){
        it->SetCellType(ECT_NONE);
      }
    }
    void ClearContents()
    {
      /*
      for(map<row_t,CellStore>::iterator it=mapCellStore_.begin();it!=mapCellStore_.end();){
        it->second.GetData().Clear();
        if(it->second.IsEmpty())
          it=mapCellStore_.erase(it);
        else
          ++it;
      }
      */
      for(vector<CellStore>::iterator it=vecRowStore_.begin();it!=vecRowStore_.end();++it){
        it->GetData().Clear();
      }
    }
    //枚举具有风格的列
    void Style_EnumRows(vector<pair<row_t,StyleID_t> >& vec)const
    {
      for(size_t i=0;i<vecRowStore_.size();++i){
        if(const StyleID_t sid=vecRowStore_[i].GetStyleID())
          vec.push_back(make_pair((row_t)i,sid));
      }
      //for(map<row_t,CellStore>::const_iterator it=mapCellStore_.begin();it!=mapCellStore_.end();++it){
      //  if(const StyleID_t sid=it->second.GetStyleID())
      //    vec.push_back(make_pair(it->first,sid));
      //}
    }
    const CellStore* GetRowStore(int row)const
    {
      _ASSERT(row<(int)vecRowStore_.size());
      _ASSERT(row>=0);
      const CellStore& cs=vecRowStore_[row];
      return cs.IsEmpty()?NULL:&cs;
      /*
      map<row_t,CellStore>::const_iterator it=mapCellStore_.find(static_cast<row_t>(row));
      if(mapCellStore_.end()!=it)
        return &it->second;
      return NULL;
      */
    }
    void put_RowType(int row,ECellType cellType)
    {
      _ASSERT(row<(int)vecRowStore_.size());
      _ASSERT(row>=0);
      CellStore& cs=vecRowStore_[row];
      cs.SetCellType(cellType);
    }
    //row style id
    StyleID_t Style_GetRowStyleID(int row)const
    {
      _ASSERT(row<(int)vecRowStore_.size());
      _ASSERT(row>=0);
      return row>=0?vecRowStore_[row].GetStyleID():NULL;
      //map<row_t,CellStore>::const_iterator it=mapCellStore_.find(static_cast<row_t>(row));
      //if(mapCellStore_.end()!=it)
      //  return it->second.GetStyleID();
      //return NULL;
    }
    void style_ClearRowStyle(StyleStore& ss,int row)
    {
      _ASSERT(row<(int)vecRowStore_.size());
      _ASSERT(row>=0);
      vecRowStore_[row].ClearStyle(ss);
      /*
      map<row_t,CellStore>::iterator it=mapCellStore_.find(row);
      if(it!=mapCellStore_.end()){
        it->second.ClearStyle(ss);
        if(it->second.IsEmpty())
          mapCellStore_.erase(it);
      }
      */
    }
    void style_ClearStyles(StyleStore& ss)
    {
      /*
      for(map<row_t,CellStore>::iterator it=mapCellStore_.begin();it!=mapCellStore_.end();){
        it->second.ClearStyle(ss);
        if(it->second.IsEmpty()){
          it=mapCellStore_.erase(it);
        }else
          ++it;
      }
      //mapCellStore_.clear();
      */
      for(vector<CellStore>::iterator it=vecRowStore_.begin();it!=vecRowStore_.end();++it)
        it->ClearStyle(ss);
    }
  private:
    //delete range[begin,end];
    void delete_rows(StyleStore& ss,int beg,int end);
    void put_rows(StyleStore& ss,int rows);
    //在beg前插入nCount行
    void insert_rows(int beg,int nCount);
  private:
    StyleID_t style_SetRowStyle(StyleStore& ss,int row,const StyleDesc& style)
    {
      _ASSERT(row>=0);
      _ASSERT(row<(int)vecRowStore_.size());
      return vecRowStore_[row].SetStyle(ss,style);
    }
    //return validate toprow;
    int put_TopVisScrollRow(/*Worksheet* pEventsSink,*/int row,int nScrollHeight/*LPCRECT lprcScroll*/);
    //lprcContent can NULL,
    void insert_rows(int beg,int nCount,int nScrollHeight/*LPCRECT lprcContent*/);
    //delete range[begin,end];
    //lprcContent can NULL,
    void delete_rows(StyleStore& ss,int beg,int end,int nScrollHeight/*LPCRECT lprcContent*/);
    BOOL IsTopRowValidate(int nScrollHeight/*LPCRECT lprcScroll*/);
    void validate_TopVisScrollRow(int nScrollHeight/*LPCRECT lprcScroll*/);
    void validate_BottomVisScrollRow(int nScrollHeight/*LPCRECT lprcScroll*/);
  private:
    map<row_t,short> mapIrregHeights_;
    //map<row_t,StyleID_t> mapStyleID_;
    //map<row_t,CellStore> mapCellStore_;
    vector<CellStore> vecRowStore_;
    int  rows_;
    int  width_;//width of row header
    //滚动区第一可见行和最后可见行
    int topVisScrollRow_,bottomVisScrollRow_;
    //冻结区第一行后最后一行
    int freezeTopRow_,freezeBottomRow_;
    short    defHeight_,bits_;
  };
  //struct ColumnInfo
  //{
  //  CString label;
  //  ECellType cellType;
  //};
  //struct ColHeaderEvents
  //{
  //  virtual void OnVisibleColChanged(int oldLeftCol,int oldRightCol,int newLeftCol,int newRightCol)=0;
  //};
  //class ColStylesMgr
  //{
  //  StyleStore& styleStore_;
  //  //ECellType defColType_;
  //protected:
  //  map<col_t,StyleID_t> mapColType_;//<col,ECellType>
  //  ColStylesMgr(StyleStore& styleStore):styleStore_(styleStore)//,defColType_(ECT_NONE)
  //  {}
  //  BOOL GetColStyle(int col,StyleDesc& styleDesc)const
  //  {
  //    map<col_t,StyleID_t>::const_iterator it=mapColType_.find((col_t)col);
  //    if(it!=mapColType_.end()){
  //      return styleStore_.get_Style(it->second,styleDesc);
  //    }else{
  //      styleDesc.SetEmptyStyle();
  //      return FALSE;
  //    }
  //  }
  //public:
  //  //void put_DefColType(ECellType cellType)
  //  //{
  //  //  defColType_=cellType;
  //  //}
  //  //ECellType get_DefColType()const
  //  //{
  //  //  return defColType_;
  //  //}
  //  /*
  //  ECellType get_ColType(int col)const
  //  {
  //    map<col_t,StyleID_t>::const_iterator it=mapColType_.find(col);
  //    //return it==mapColType_.end()?defColType_:it->second;
  //    if(it!=mapColType_.end()){
  //      StyleDesc style;
  //      if(styleStore_.get_Style(it->second,style) && style.cellType)
  //        return style.cellType;
  //    }
  //    return defColType_;
  //  }
  //  */
  //  void put_ColType(int col,ECellType type)
  //  {
  //    StyleDesc style;
  //    map<col_t,StyleID_t>::iterator it=mapColType_.find((col_t)col);
  //    if(it!=mapColType_.end()){
  //      styleStore_.get_Style(it->second,style);
  //    }
  //    if(type!=style.cellType){
  //      style.cellType=type;
  //      if(style.IsEmptyStyle())
  //        mapColType_.erase(it);
  //      else{
  //        StyleID_t const styleID=styleStore_.add_style(style);
  //        mapColType_[(col_t)col]=styleID;
  //      }
  //    }
  //  }
  //};
  class ColHeader //: public ColStylesMgr
  {
    friend class Worksheet;
    //friend class Grid;
    friend class XmlExcel;
    //friend class HeaderDragImpl;
    ColHeader(ColHeader const& rhs);
    ColHeader& operator=(ColHeader const& rhs);
  public:     
    explicit ColHeader(/*Worksheet* p*/):cols_(10),defWidth_(80),height_(20),leftVisScrollCol_(0),rightVisScrollCol_(-1)
      //,pEvents_(p)
      ,freezeLeftCol_(-1),freezeRightCol_(-1)
    {
      vecColumn_.resize(cols_);
    }
    //void SetListener(/*ColHeaderEvents*/Worksheet* p)
    //{
    //  pEvents_=p;
    //}
    int get_cols()const
    {
      return cols_;
    }
    int get_LeftVisScrollCol()const
    {
      return leftVisScrollCol_;
    }
    int get_RightVisScrollCol()const
    {
      //_ASSERT(-1!=rightVisScrollCol_);
      _ASSERT(rightVisScrollCol_<cols_);
      return rightVisScrollCol_;
    }
    int get_FreezeLeftCol()const
    {
      return freezeLeftCol_;
    }
    int get_FreezeRightCol()const
    {
      return freezeRightCol_;
    }
    bool IsFreezeColVisible()const
    {
      return -1!=freezeRightCol_;//freezeRightCol_>=freezeLeftCol_ && freezeRightCol_>HEADER_COL;
    }
    //冻结区freezeTopRow_发生改变将导致垂直滚动条发生改变，
    //topVisScrollRow_,bottomVisScrollRow_也可能发生改变，
    //该方法作为底层方法，此处不维护这些改变，
    //其改变的维护工作交由Worksheet负责。
    void put_FreezeLeftCol(int nFreezeLeftCol)
    {
      freezeLeftCol_=nFreezeLeftCol;
    }
    //注解与put_FreezeLeftCol类似
    void put_FreezeRightCol(int nFreezeRightCol)
    {
      freezeRightCol_=nFreezeRightCol;
    }
    //return 可滚动到最左端的列
    int get_LeftScrollableCol()const
    {
      return freezeRightCol_+1;
    }
    //return 可滚动到最右边的列
    int get_RightScrollableCol()const
    {
      return cols_-1;
    }
    //logPixelsX:Number of pixels per logical inch along the screen width
    //wi 设备像素
    void put_DefColWidth(int wi)
    {
      //wi/logPixelsX*254
      //defWidth_=MulDiv(wi,254,logPixelsX);
      defWidth_=wi;
    }
    int get_DefColWidth()const
    {
      return defWidth_;
    }
    //[col1,col2)
    int get_DiffWidths(int col1,int col2)const;
    ////[col1,col2)
    //int get_DiffWidthsPlain(int col1,int col2)const;
    int get_widths()const
    {
      const int nAbnormalCols=int(mapIrregWidths_.size());
      _ASSERT(nAbnormalCols<=cols_);
      int widths=(cols_-nAbnormalCols)*get_DefColWidth();
      for(map<col_t,short>::const_iterator it=mapIrregWidths_.begin();it!=mapIrregWidths_.end();++it)
        widths+=it->second;
      return widths;
    }
    //logPixelsX:Number of pixels per logical inch along the screen width
    int get_ColWidth(/*int logPixelsX,*/int col)const
    {
      map<col_t,short>::const_iterator it=mapIrregWidths_.find(col);
      int width=it==mapIrregWidths_.end()?defWidth_:it->second;//微米
      //下面将微米换算成设备像素
      //(width/254.0f)*logPixelsX
      //width=MulDiv(width,logPixelsX,254);
      return width;
    }
    void put_height(int hi)
    {
      height_=hi;
    }
    int get_height()const
    {
      return height_;
    }
    int get_text(int col,LPTSTR buf)const
    {
      _ASSERT(col>=0);
      _ASSERT(col<cols_);
      if(col<0 || col>=static_cast<int>(vecColumn_.size())){
        buf[0]=_T('\0');
        return 0;
      }
      lstrcpy(buf,vecColumn_[col].first);
      //lstrcpy(buf,vecColInfo_[col].label);
      /*
      ++col;
      if(col<27){          //A~Z
        buf[0]=(col+0x40);
        buf[1]=0;
      }else if(col<703){        //AA~ZZ
        buf[1]=(col-1)%26+'A';//0x41;
        buf[0]=(col-1)/26-1+'A';//0x40;
        buf[2]=0;
      }
      */
      int i=0;
      for(;i!=MAX_CELLTEXT && buf[i];++i)
        ;
      return i;
    }
    int calc_LeftVisScrollCol(int nRightVisScrollCol,int nScrollWidth,BOOL bFullVisible=TRUE)const;
    
    //根据水平滚动条的位置计算LeftVisScrollCol
    
    //int calc_LeftVisScrollCol(int nScrollPos)const;
    int calc_RightVisScrollCol(int nLeftVisScrollCol,int nScrollWidth/*LPCRECT lprcScroll*/)const;
    void put_ColLabel(int col,LPCTSTR label)
    {
      _ASSERT(col<cols_);
      _ASSERT(col>=0);
      if(col>=static_cast<int>(vecColumn_.size()))
        vecColumn_.resize(col+1);
      vecColumn_[col].first=label;
      //vecColInfo_[col].label=label;
    }
    void put_ColType(int col,ECellType cellType)
    {
      _ASSERT(col<cols_);
      _ASSERT(col>=0);
      vecColumn_[col].second.SetCellType(cellType);
    }
    CString const& getref_ColLabel(int col)
    {
      _ASSERT(col<cols_);
      _ASSERT(col>=0);
      if(col<static_cast<int>(vecColumn_.size()))
        return vecColumn_[col].first;
      else{
        static CString empty(_T(""));
        return empty;
      }
      //return vecColInfo_[col].label;
    }
    const CellStore* GetColStore(int col)const
    {
      _ASSERT(col<(int)vecColumn_.size());
      const CellStore& cs=vecColumn_[col].second;
      return cs.IsEmpty()?NULL:&cs;
    }
    //ECellType GetCellType(int col)
    //{
    //  return cellEdit;
    //}
    //delete range[begin,end];
    //lprcContent can NULL,
    void delete_cols(StyleStore& ss,int beg,int end,int nScrollWidth);//LPCRECT lprcContent);
    void insert_cols(int beg,int nCount,int nScrollWidth);//LPCRECT lprcContent);

    //枚举具有风格的列
    void Style_EnumCols(vector<pair<col_t,StyleID_t> > vec)const
    {
      for(vector<pair<CString,CellStore> >::const_iterator it=vecColumn_.begin();it!=vecColumn_.end();++it){
        if(const StyleID_t sid=it->second.GetStyleID())
          vec.push_back(make_pair(col_t(it-vecColumn_.begin()),sid));
      }
    }
    //col style id
    StyleID_t Style_GetColStyleID(int col)const
    {
      _ASSERT(col>=0);
      _ASSERT(col<(int)vecColumn_.size());
      return col>=0?vecColumn_[col].second.GetStyleID():NULL;
      /*
      map<col_t,CellStore>::const_iterator it=mapColStore_.find(static_cast<col_t>(col));
      if(mapColStore_.end()!=it)
        return it->second.nStyleID;
      return NULL;
      */
    }
    void Style_ClearColStyle(StyleStore& ss,int col)
    {
      _ASSERT(col>=0);
      _ASSERT(col<(int)vecColumn_.size());
      if(col<(int)vecColumn_.size())
        vecColumn_[col].second.ClearStyle(ss);
    }
    void style_ClearStyles(StyleStore& ss)
    {
      for(vector<pair<CString,CellStore> >::iterator it=vecColumn_.begin();it!=vecColumn_.end();++it){
        it->second.ClearStyle(ss);
      }
      //for(map<col_t,CellStore>::iterator it=mapColStore_.begin();it!=mapColStore_.end();){
      //  if(it->second.nStyleID>0){
      //    ss.DecrementRefCount(it->second.nStyleID);
      //    it=mapColStore_.erase(it);
      //  }else
      //    ++it;
      //}
      //mapColStore_.clear();
    }
    void clear(StyleStore& ss);
    void ClearContents()
    {
      for(vector<pair<CString,CellStore> >::iterator it=vecColumn_.begin();it!=vecColumn_.end();++it){
        it->first.Empty();
      }
    }
    void ClearCellTypes()
    {
      for(vector<pair<CString,CellStore> >::iterator it=vecColumn_.begin();it!=vecColumn_.end();++it){
        it->second.SetCellType(ECT_NONE);
      }
    }
    void SetColData(int col,VARIANT val)
    {
      _ASSERT(col>=0);
      _ASSERT(col<(int)vecColumn_.size());
      vecColumn_[col].second.SetData(val);
    }
    void GetColData(int col,VARIANT* pVal)
    {
      _ASSERT(col>=0);
      _ASSERT(col<(int)vecColumn_.size());
      *pVal=vecColumn_[col].second.GetData();
    }
  private:
    StyleID_t style_SetColStyle(StyleStore& ss,int col,const StyleDesc& style)
    {
      _ASSERT(col<(int)vecColumn_.size());
      //ss.DecrementRefCount(vecColumn_[col].second.nStyleID);
      //vecColumn_[col].second.nStyleID=bNullStyle?0:ss.add_style(*pstyle);
      return vecColumn_[col].second.SetStyle(ss,style);
    }
    
    //return new leftcol
    
    int put_LeftVisScrollCol(/*Worksheet* pEventsSink,*/int col,int nScrollWidth);//LPCRECT lprcScroll);

    
    //return new leftcol
    
    int put_RightVisScrollCol(/*Worksheet* pEventsSink,*/int col,int nScrollWidth/*LPCRECT lprcScroll*/,BOOL bFullVisible=FALSE);
    //void put_cols(int cols)
    //{
    //  _ASSERT(cols>=0);
    //  cols_=cols;
    //  vecColumn_.resize(cols);
    //}
  public:
    //logPixelsX:Number of pixels per logical inch along the screen width
    //wi 设备像素
    void put_ColWidth(/*int logPixelsX,*/int col,int wi/*,LPCRECT lprcClient*/)
    {
      _ASSERT(col>=0);
      _ASSERT(col<cols_);
      //wi=MulDiv(wi,254,logPixelsX);
      if(wi==defWidth_){
        map<col_t,short>::iterator it=mapIrregWidths_.find(col);
        if(it!=mapIrregWidths_.end())
          mapIrregWidths_.erase(it);
      }else{
        mapIrregWidths_[col]=wi;
      }
#ifdef _DEBUG
      const int nAbnormalCols=int(mapIrregWidths_.size());
      _ASSERT(nAbnormalCols<=cols_);
#endif
    }
    void validate_LeftVisScrollCol(int nScrollWidth);//LPCRECT lprcScroll);
    
    //保持不变使rightVisScrollCol_处于有效值，当范围
    //[leftVisScrollCol_,rightVisScrollCol_]之间的列宽度发生变化
    //(leftVisScrollCol_,rightVisScrollCol_]之间增加或删除列时
    //可调用该方法
    //nScrollWidth=VClientAxis-VFreezeAxis
    
    void validate_RightVisScrollCol(int nScrollWidth/*LPCRECT lprcScroll*/)
    {
      rightVisScrollCol_=calc_RightVisScrollCol(leftVisScrollCol_,nScrollWidth);
    }
  private:
    int  cols_;
    int  defWidth_;
    int  height_;//height of header column

    //滚动区第一可见列和左后可见列
    int leftVisScrollCol_,rightVisScrollCol_;

    //冻结区第一列和左后一列
    int freezeLeftCol_,freezeRightCol_;

    map<col_t,short> mapIrregWidths_;  //<col,width>
    //map<col_t,StyleID_t> mapStyleID_;//<col,ECellType>
    //map<col_t,CellStore> mapColStore_;
    vector<pair<CString,CellStore> > vecColumn_;//first存储Label,second.cellVal存储列的附加数据
  };
