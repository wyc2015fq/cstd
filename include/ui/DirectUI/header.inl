
#include "stdafx.h"
#include "header.h"
#include "coleventsimpl.h"
//#include "gridbase.h"
namespace mycell{
  //nScrollHeight=HClientAxis-HFreezeAxis
  int RowHeader::put_TopVisScrollRow(/*Worksheet* pEventsSink,*/int row,int nScrollHeight)//LPCRECT lprcScroll)
  {
    if(rows_<=0)
      return -1;
    if(row<rows_){
      int const nTopScrollableRow=get_TopScrollableRow();
      if(row<nTopScrollableRow)
        row=nTopScrollableRow;
      int const oldTopRow=topVisScrollRow_;
      int const oldBottomRow=bottomVisScrollRow_;
      topVisScrollRow_=row;
      if(!IsTopRowValidate(nScrollHeight/*lprcScroll*/))
        validate_TopVisScrollRow(nScrollHeight/*lprcScroll*/);
      validate_BottomVisScrollRow(nScrollHeight/*lprcScroll*/);
      //if(!(oldTopRow==topVisScrollRow_ && oldBottomRow==bottomVisScrollRow_))
      //  pEventsSink->OnVisibleRowChanged(oldTopRow,oldBottomRow,topVisScrollRow_,bottomVisScrollRow_);
    }
    return topVisScrollRow_;
  }

  //nScrollHeight=HClientAxis-HFreezeAxis
  BOOL RowHeader::IsTopRowValidate(int nScrollHeight/*LPCRECT lprcScroll*/)
  {
    //int const nScrollHeight=lprcScroll->bottom - lprcScroll->top;
    int const rows=get_rows();
    int topRow=get_TopVisScrollRow();
    int height=0;
    for(int i=topRow;i!=rows;++i)
    {
      height+=get_RowHeight(i);
      if(height>=nScrollHeight)
        return TRUE;
    }
    return FALSE;
  }

  /*
  int RowHeader::put_BottomVisScrollRow(int row,int nScrollHeight,BOOL bFullVisible)
  {
  //if(row!=bottomVisScrollRow_)
  {
  int const oldTopRow=topVisScrollRow_;
  int const oldBottomRow=bottomVisScrollRow_;
  int const rows=get_rows();
  if(rows>0){
  _ASSERT(row<rows);
  bottomVisScrollRow_=row;
  int hi=0;
  //const int height=lprcScroll->bottom - lprcScroll->top;
  for(topVisScrollRow_=row;-1!=topVisScrollRow_;--topVisScrollRow_)
  {
  hi+=get_RowHeight(topVisScrollRow_);
  if(hi>nScrollHeight){
  if(bFullVisible){
  ++topVisScrollRow_;
  validate_BottomVisScrollRow(lprcScroll);
  }
  break;
  }
  }
  if(-1==topVisScrollRow_)
  topVisScrollRow_=0;
  }else{
  topVisScrollRow_=bottomVisScrollRow_=-1;
  }
  pEvents_->OnVisibleRowChanged(oldTopRow,oldBottomRow,topVisScrollRow_,bottomVisScrollRow_);
  }
  return topVisScrollRow_;
  }
  */
  /*
  
  //根据跟定的垂直滚动条位置计算处TopVisScrollRow
  
  int RowHeader::calc_TopVisScrollRow(int nScrollPos)const
  {
  int const rows=get_rows();
  int nRow=nScrollPos/get_DefRowHeight();
  int nTopScrollableRow=get_TopScrollableRow();
  if(IsFreezeRowVisible()){
  nRow+=nTopScrollableRow;
  }else
  nTopScrollableRow=0;
  int hi=get_DiffHeights(nTopScrollableRow,nRow);
  if(hi==nScrollPos){
  return min(rows-1,nRow);
  }else if(hi>nScrollPos){
  for(int i=min(rows-1,nRow-1);i!=nTopScrollableRow;--i){
  hi-=get_RowHeight(i);
  if(hi<=nScrollPos)
  return i;
  }
  }else{
  for(;nRow!=rows;++nRow){
  hi+=get_RowHeight(nRow);
  if(hi>=nScrollPos)
  break;
  }
  }
  if(nRow>=rows)
  nRow=rows-1;
  _ASSERT(nRow>=nTopScrollableRow);
  return nRow;
  }
  */
  
  //假定nBottomVisScrollRow为表格的BottomVisScrollRow计算出
  //TopVisScrollRow，bFullVisible表示指定的nBottomVisScrollRow
  //垂直方向是否完全可见
  //参数nScrollHeight为[HFreezeAxis,HClientAxis]范围的距离
  //即nScrollHeight=HClientAxis-HFreezeAxis；
  
  int RowHeader::calc_TopVisScrollRow(int nBottomVisScrollRow,int nScrollHeight,BOOL bFullVisible)const
  {
    const int nTopScrollableRow=get_TopScrollableRow();
    int hi=0,nTopVisScrollRow=nBottomVisScrollRow;
    for(;nTopVisScrollRow>=nTopScrollableRow;--nTopVisScrollRow)
    {
      hi+=get_RowHeight(nTopVisScrollRow);
      if(hi==nScrollHeight)
        return nTopVisScrollRow;
      if(hi>nScrollHeight){
        return bFullVisible?nTopVisScrollRow+1:nTopVisScrollRow;
      }
    }
    //_ASSERT(nTopVisScrollRow>=nTopScrollableRow);
    return max(nTopVisScrollRow,nTopScrollableRow);
  }

  
  //返回最后可见行
  
  int RowHeader::calc_BottomVisScrollRow(int nTopVisScrollRow,int nScrollHeight/*LPCRECT lprcScroll*/)const
  {
    //int const height=lprcScroll->right-lprcScroll->left;
    for(int hi=get_RowHeight(nTopVisScrollRow);nTopVisScrollRow<rows_ && hi<nScrollHeight;)
      hi+=get_RowHeight(++nTopVisScrollRow);
    return min(rows_-1,nTopVisScrollRow);
  }
  void RowHeader::validate_TopVisScrollRow(int nScrollHeight/*LPCRECT lprcScroll*/)
  {
    _ASSERT(rows_>0);
    //int const rcHeight=lprcScroll->bottom - lprcScroll->top;
    int const rows=get_rows();
    int topRow=get_TopVisScrollRow();
    _ASSERT(topRow<rows);
    int height=0;
    for(int i=topRow;i!=rows;++i)
    {
      height+=get_RowHeight(i);
      if(height>=nScrollHeight)
        return;
    }
    if(topVisScrollRow_>get_TopScrollableRow() && nScrollHeight-height>get_RowHeight(topVisScrollRow_-1)){
      --topVisScrollRow_;
      validate_TopVisScrollRow(nScrollHeight);
    }
  }
  void RowHeader::validate_BottomVisScrollRow(int nScrollHeight/*LPCRECT lprcScroll*/)
  {
    _ASSERT(rows_>=0);
    if(topVisScrollRow_<0)
      topVisScrollRow_=0;
    if(0==rows_){
      topVisScrollRow_=bottomVisScrollRow_=-1;
    }else{
      _ASSERT(topVisScrollRow_<rows_);
      int height=0;//,rcHeight=lprcScroll->bottom-lprcScroll->top;
      int const lastRow=rows_-1;
      for(bottomVisScrollRow_=topVisScrollRow_;bottomVisScrollRow_!=lastRow;++bottomVisScrollRow_)
      {
        height+=get_RowHeight(bottomVisScrollRow_);
        if(height>=nScrollHeight){
          break;
        }
      }
      _ASSERT(bottomVisScrollRow_<rows_);
    }
  }
  void RowHeader::delete_rows(StyleStore& ss,int beg,int end,int nScrollHeight/*LPCRECT lprcContent*/)
  {
    _ASSERT(end<rows_);
    _ASSERT(end>=beg);
    //int const cnt=end-beg+1;
    delete_rows(ss,beg,end);
    _ASSERT(rows_>=0);
    if(topVisScrollRow_>=rows_)
      topVisScrollRow_=rows_-1;
    if(rows_>0){
      validate_TopVisScrollRow(nScrollHeight);
      validate_BottomVisScrollRow(nScrollHeight);
    }else{
      topVisScrollRow_=bottomVisScrollRow_=-1;
    }
  }
  void RowHeader::insert_rows(int beg,int nCount,int nScrollHeight)//LPCRECT lprcContent)
  {
    _ASSERT(nCount>0);
    insert_rows(beg,nCount);
    validate_TopVisScrollRow(nScrollHeight);
    validate_BottomVisScrollRow(nScrollHeight);
  }

  //nScrollWidth=VClientAxis-VFreezeAxis
  int ColHeader::put_LeftVisScrollCol(int col,int nScrollWidth)
  {
    if(col<cols_){
      int const nLeftScrollableCol=get_LeftScrollableCol();
      int const oldLeflCol=leftVisScrollCol_;
      int const oldRightCol=rightVisScrollCol_;
      leftVisScrollCol_=col;
      validate_LeftVisScrollCol(nScrollWidth);
      rightVisScrollCol_=calc_RightVisScrollCol(leftVisScrollCol_,nScrollWidth);
    }
    return leftVisScrollCol_;
  }
  //nScrollWidth=VClientAxis-VFreezeAxis
  int ColHeader::put_RightVisScrollCol(/*Worksheet* pEventsSink,*/int col,int nScrollWidth/*LPCRECT lprcScroll*/,BOOL bFullVisible)
  {
    int const cols=get_cols();
    _ASSERT(col<cols);
    int const oldLeflCol=leftVisScrollCol_;
    int const oldRightCol=rightVisScrollCol_;

    rightVisScrollCol_=col;
    int wi=0;
    //int const width=lprcScroll->right - lprcScroll->left;
    for(leftVisScrollCol_=col;leftVisScrollCol_!=-1 ;--leftVisScrollCol_)
    {
      wi+=get_ColWidth(leftVisScrollCol_);
      if(wi>nScrollWidth){
        if(bFullVisible){
          ++leftVisScrollCol_;
          validate_RightVisScrollCol(nScrollWidth/*lprcScroll*/);
        }
        break;
      }
    }
    if(-1==leftVisScrollCol_)
      leftVisScrollCol_=0;
    _ASSERT(leftVisScrollCol_>=0);
    //pEvents_->OnVisibleColChanged(oldLeflCol,oldRightCol,leftVisScrollCol_,rightVisScrollCol_);
    //if(!(oldLeflCol==leftVisScrollCol_ && oldRightCol==rightVisScrollCol_))//2007-9-18
    //  pEventsSink->OnVisibleColChanged(oldLeflCol,oldRightCol,leftVisScrollCol_,rightVisScrollCol_);

    return leftVisScrollCol_;
  }
  //毁弃的方法，该方法因get_ColWidth的调用使得效率
  //不及get_DiffWidths

  ////类似与get_DiffWidths(int col1,int col2)方法，但使用简单累加，
  ////对于较少汇总列的情况下使用效率较高
  ////区间:[col1,col2)

  //int ColHeader::get_DiffWidthsPlain(int col1,int col2)const
  //{
  //  int wi=0;
  //  for(int i=max(0,col1);i<col2;++i){
  //    wi+=get_ColWidth(i);
  //  }
  //  return wi;
  //}
  //[col1,col2)
  int ColHeader::get_DiffWidths(int col1,int col2)const
  {
    //_ASSERT(col1>=0);
    if(col1>=col2) return 0;
    int widths=(col2-col1)*get_DefColWidth();
    for(map<col_t,short>::const_iterator it=mapIrregWidths_.begin();it!=mapIrregWidths_.end();++it)
    {
      if(it->first>=col1 && it->first<col2){
        //widths-=get_DefColWidth();
        widths+=it->second-get_DefColWidth();
      }
    }
    return widths;
  }
  
  //假定nRightVisScrollCol为表格的RightVisScrollCol计算出
  //LeftVisScrollCol，bFullVisible表示指定的nRightVisScrollCol
  //水平方向是否完全可见
  //参数nScrollWidth为[VFreezeAxis,VClientAxis]范围的距离
  //即nScrollWidth=VClientAxis-VFreezeAxis；
  
  int ColHeader::calc_LeftVisScrollCol(int nRightVisScrollCol,int nScrollWidth,BOOL bFullVisible)const
  {
    int const nLeftScrollableCol=get_LeftScrollableCol();
    int wi=0,nLeftVisScrollCol=nRightVisScrollCol;
    for(;nLeftVisScrollCol>=nLeftScrollableCol;--nLeftVisScrollCol)
    {
      wi+=get_ColWidth(nLeftVisScrollCol);
      if(wi==nScrollWidth)
        return nLeftVisScrollCol;
      if(wi>nScrollWidth){
        return bFullVisible?nLeftVisScrollCol+1:nLeftVisScrollCol;
      }
    }
    //_ASSERT(nLeftVisScrollCol>=nLeftScrollableCol);
    return max(nLeftVisScrollCol,nLeftScrollableCol);
  }
  /*
  
  //根据水平滚动条的位置计算LeftVisScrollCol
  
  int ColHeader::calc_LeftVisScrollCol(int nScrollPos)const
  {
  int const cols=get_cols();
  int const nDefColWidth=get_DefColWidth();
  int nCol=nScrollPos/nDefColWidth;
  int nLeftScrollableCol=get_LeftScrollableCol();
  if(IsFreezeColVisible()){
  nCol+=nLeftScrollableCol;
  //nCol=nDefColWidth+get_DiffWidths(0,nLeftScrollableCol)/nDefColWidth;
  }else
  nLeftScrollableCol=0;
  int wi=get_DiffWidths(nLeftScrollableCol,nCol);
  if(wi==nScrollPos){
  //return nCol;
  }else if(wi>nScrollPos){
  for(int i=nCol-1;i!=nLeftScrollableCol;--i){
  wi-=get_ColWidth(i);
  if(wi<=nScrollPos){
  nCol=i+1;
  break;
  }
  }
  }else{
  //for(int i=nCol;i!=cols;++i){
  for(;nCol!=cols;++nCol){
  wi+=get_ColWidth(nCol);
  if(wi>=nScrollPos){
  //nCol=i;
  break;
  //return i;
  }

  }
  }
  if(nCol>=cols)
  nCol=cols-1;
  //_ASSERT(nCol<cols);
  return nCol;
  }
  */
  
  //根据指定的LeftVisScrollCol计算处最右可见列
  //参数nScrollWidth为[VFreezeAxis,VClientAxis]范围的距离
  //即nScrollWidth=VClientAxis-VFreezeAxis；
  
  int ColHeader::calc_RightVisScrollCol(int nLeftVisScrollCol,int nScrollWidth)const
  {
    //int const width=lprcScroll->right-lprcScroll->left;
    for(int wi=get_ColWidth(nLeftVisScrollCol);nLeftVisScrollCol<cols_ && wi<nScrollWidth;)
    {
      wi+=get_ColWidth(++nLeftVisScrollCol);
      //++nLeftVisScrollCol;
    }
    return min(cols_-1,nLeftVisScrollCol);
  }
  
  //使leftVisScrollCol_处于有效值；如果最后一列可见则
  //最后一列最右边到窗口客户区部分的宽度不应大于
  //LeftScrollableCol-1的宽度。
  //nScrollWidth=VClientAxis-VFreezeAxis
  
  void ColHeader::validate_LeftVisScrollCol(int nScrollWidth)
  {
    int const nLeftScrollableCol=get_LeftScrollableCol();
    if(leftVisScrollCol_<nLeftScrollableCol){
      leftVisScrollCol_=nLeftScrollableCol;
    }else if(leftVisScrollCol_>nLeftScrollableCol){
      const int width=get_DiffWidths(get_LeftVisScrollCol(),get_cols());
      if(nScrollWidth-width>get_ColWidth(leftVisScrollCol_-1)){
        --leftVisScrollCol_;
        if(leftVisScrollCol_>nLeftScrollableCol)
          validate_LeftVisScrollCol(nScrollWidth);
      }
    }
  }
  /*
  
  //保持不变使rightVisScrollCol_处于有效值，当范围
  //[leftVisScrollCol_,rightVisScrollCol_]之间的列宽度发生变化
  //(leftVisScrollCol_,rightVisScrollCol_]之间增加或删除列时
  //可调用该方法
  
  void ColHeader::validate_RightVisScrollCol(LPCRECT lprcScroll)
  {
  _ASSERT(cols_>=0);
  if(leftVisScrollCol_<0)
  leftVisScrollCol_=0;
  if(0==cols_){
  leftVisScrollCol_=rightVisScrollCol_=-1;
  }else{
  _ASSERT(leftVisScrollCol_<cols_);
  int width=0,rcWidth=lprcScroll->right - lprcScroll->left;
  for(rightVisScrollCol_=leftVisScrollCol_;rightVisScrollCol_!=cols_-1;++rightVisScrollCol_)
  {
  width+=get_ColWidth(rightVisScrollCol_);
  if(width>=rcWidth){
  break;
  }
  }
  _ASSERT(rightVisScrollCol_<cols_);
  }
  }
  */
  //void ColHeader::put_cols(int cols)
  //{
  //  _ASSERT(cols>=0);
  //  cols_=cols;
  //  vecLabel_.resize(cols_);
  //  for(map<col_t,short>::iterator it=mapIrregWidths_.begin();it!=mapIrregWidths_.end();++it)
  //  {
  //    if(it->first>=cols){
  //      it=mapIrregWidths_.erase(it);
  //    }
  //  }
  //}
  //delete range[begin,end];
  //lprcContent can NULL,
  void ColHeader::delete_cols(StyleStore& ss,int beg,int end,int nScrollWidth)//,LPCRECT lprcContent)
  {
    _ASSERT(end<cols_);
    _ASSERT(end>=beg);
    int const cnt=end-beg+1;
    cols_-=cnt;
    _ASSERT(cols_>=0);
    mycell::delete_cols<col_t,short>(mapIrregWidths_,beg,end/*,lprcContent*/);

    /*
    for(map<col_t,StyleID_t>::iterator it=mapStyleID_.begin();it!=mapStyleID_.end();++it){
      const int nk=it->first;
      if(nk>=beg && nk<=end && it->second){
        ss.DecrementRefCount(it->second);
      }
    }
    mycell::delete_cols<col_t,StyleID_t>(mapStyleID_,beg,end);
    */
    _ASSERT((int)mapIrregWidths_.size()<=cols_);
    int const iVecSize=static_cast<int>(vecColumn_.size());
    if(beg<iVecSize){
      const vector<pair<CString,CellStore> >::iterator _beg=vecColumn_.begin()+beg;
      const vector<pair<CString,CellStore> >::iterator _end=vecColumn_.begin()+min(end+1,iVecSize);
      for(vector<pair<CString,CellStore> >::iterator it=_beg;it!=_end;++it){
        it->second.ClearStyle(ss);
      }
      vecColumn_.erase(_beg,_end);
    }

    if(leftVisScrollCol_>=cols_)
      leftVisScrollCol_=cols_-1;
    if(cols_>0){
      validate_LeftVisScrollCol(nScrollWidth);
      validate_RightVisScrollCol(nScrollWidth);
    }else{
      leftVisScrollCol_=rightVisScrollCol_=-1;
    }
  }

  void ColHeader::insert_cols(int beg,int nCount,int nScrollWidth)//,LPCRECT lprcContent)
  {
    _ASSERT(nCount>0);
    cols_+=nCount;
    _ASSERT(cols_<MAX_COL);
    int const end=beg+nCount;
    mycell::insert_cols<col_t,short>(mapIrregWidths_,beg,nCount/*,lprcContent*/);
    //mycell::insert_cols<col_t,StyleID_t>(mapStyleID_,beg,nCount/*,lprcContent*/);
    //mycell::insert_cols<StyleID_t>(mapColType_,beg,nCount,lprcContent);
    //{//block
    //  vector<pair<int,int> > vec;
    //  for(map<col_t,short>::iterator it=mapIrregWidths_.begin();it!=mapIrregWidths_.end();)
    //  {
    //    if(it->first>=beg){
    //      vec.push_back(make_pair(it->first+nCount,it->second));
    //      it=mapIrregWidths_.erase(it);
    //      continue;
    //    }
    //    ++it;
    //  }
    //  for(vector<pair<int,int> >::const_iterator it=vec.begin();it!=vec.end();++it)
    //  {
    //    mapIrregWidths_.insert(*it);
    //  }
    //}
    //{//block
    //  vector<pair<col_t,ECellType> > vec;
    //  for(map<col_t,ECellType>::iterator it=mapColType_.begin();it!=mapColType_.end();)
    //  {
    //    if(it->first>=beg){
    //      vec.push_back(make_pair(it->first+nCount,it->second));
    //      it=mapColType_.erase(it);
    //      continue;
    //    }
    //    ++it;
    //  }
    //  for(vector<pair<col_t,ECellType> >::const_iterator it=vec.begin();it!=vec.end();++it)
    //  {
    //    mapColType_.insert(*it);
    //  }
    //}
    {//block
      //vector<CString>
      if(beg < static_cast<int>(vecColumn_.size())){
        pair<CString,CellStore> pr;
        vecColumn_.insert(vecColumn_.begin()+beg,nCount,pr);
      }else{
        vecColumn_.resize(vecColumn_.size()+nCount);
      }
    }
    //if(lprcContent){
    validate_LeftVisScrollCol(nScrollWidth/*lprcContent*/);
    validate_RightVisScrollCol(nScrollWidth/*lprcContent*/);
    //}
  }
  void ColHeader::clear(StyleStore& ss)
  {
    leftVisScrollCol_=rightVisScrollCol_=-1;
    cols_=0;
    freezeLeftCol_=freezeRightCol_=-1;
    mapIrregWidths_.clear();
    style_ClearStyles(ss);
    vecColumn_.clear();;
  }
  //StyleID_t ColHeader::style_SetColStyle(StyleStore& ss,int _col,const StyleDesc& style)
  //{
  //  const col_t col=static_cast<col_t>(_col);
  //  //const StyleID_t sid=Style_GetColStyleID(col);
  //  //if(sid)
  //  //  ss.DecrementRefCount(sid);
  //  //if(!pstyle)
  //  //return mapStyleID_[col]=ss.add_style(*pstyle);

  //  //StyleID_t sidRet=0;
  //  //const BOOL bNullStyle=!pstyle || pstyle->IsNull();
  //  _ASSERT(_col<(int)vecColumn_.size());
  //  //ss.DecrementRefCount(vecColumn_[col].second.nStyleID);
  //  //vecColumn_[col].second.nStyleID=bNullStyle?0:ss.add_style(*pstyle);
  //  return vecColumn_[col].second.SetStyle(ss,style);
  //  /*
  //  map<col_t,StyleID_t>::iterator it=mapStyleID_.find(col);
  //  if(mapStyleID_.end()!=it){
  //    if(it->second){
  //      ss.DecrementRefCount(it->second);
  //    }
  //    if(bNullStyle){
  //      mapStyleID_.erase(it);
  //    }else{
  //      sidRet=it->second=ss.add_style(*pstyle);
  //    }
  //  }else{
  //    if(!bNullStyle){
  //      sidRet=mapStyleID_[col]=ss.add_style(*pstyle);
  //    }
  //  }
  //  */
  //  //if(!bNullStyle){
  //  //  const Borders* pbdrs=pstyle->GetBorders();
  //  //  if(col>0){
  //  //    //const StyleID_t sid=Style_GetColStyleID(col-1);
  //  //    const StyleDesc& sd=ss.get_Style(Style_GetColStyleID(col-1));
  //  //    if(!sd.GetBorders()->right.IsNull()&&sd.GetBorders()->right!=pbdrs->left){
  //  //      StyleDesc rs=sd;
  //  //      rs.GetBorders()->right=pbdrs->left;
  //  //      style_SetColStyle(ss,col-1,&rs);
  //  //      //it->second=ss.add_style(rs);
  //  //      //mapStyleID_[col-1]=ss.add_style(rs);
  //  //    }
  //  //    /*
  //  //    it=mapStyleID_.find(col-1);
  //  //    if(it!=mapStyleID_.end() && it->second){
  //  //      const StyleDesc& sd=ss.get_Style(it->second);
  //  //      if(!sd.GetBorders()->right.IsNull()&&sd.GetBorders()->right!=pbdrs->left){
  //  //        StyleDesc rs=sd;
  //  //        rs.GetBorders()->right=pbdrs->left;
  //  //        it->second=ss.add_style(rs);
  //  //      }
  //  //    }
  //  //    */
  //  //  }
  //  //  if(col<cols_-1){
  //  //    const StyleDesc& sd=ss.get_Style(Style_GetColStyleID(col+1));
  //  //    if(!sd.GetBorders()->left.IsNull() && sd.GetBorders()->left!=pbdrs->right){
  //  //      StyleDesc rs=sd;
  //  //      rs.GetBorders()->left=pbdrs->right;
  //  //      style_SetColStyle(ss,col+1,&rs);
  //  //      //it->second=ss.add_style(rs);
  //  //    }
  //  //    /*
  //  //    it=mapStyleID_.find(col+1);
  //  //    if(it!=mapStyleID_.end() && it->second){
  //  //      const StyleDesc& sd=ss.get_Style(it->second);
  //  //      if(!sd.GetBorders()->left.IsNull() && sd.GetBorders()->left!=pbdrs->right){
  //  //        StyleDesc rs=sd;
  //  //        rs.GetBorders()->left=pbdrs->right;
  //  //        it->second=ss.add_style(rs);
  //  //      }
  //  //    }
  //  //    */
  //  //  }
  //  //}
  //  //return sidRet;
  //}

  //StyleID_t ColHeader::style_SetColStyle(StyleStore& ss,int col,const StyleDesc& style)
  //{
  //  StyleID_t sid=style_GetColStyleID(col);
  //  sid=sid?ss.ChangeStyle(sid,style):ss.add_style(style);
  //  return mapStyleID_[static_cast<col_t>(col)]=sid;
  //  //map<col_t,StyleID_t>::iterator it=mapStyleID_.find(static_cast<col_t>(col));
  //  //if(mapStyleID_.end()!=it){
  //  //  sid=ss.ChangeStyle(it->second,style);
  //  //  it->second=sid;
  //  //}else{
  //  //  sid=ss.add_style(style);
  //  //  mapStyleID_.insert(make_pair(static_cast<col_t>(col),sid));
  //  //}
  //  //return sid;
  //}
  //StyleID_t RowHeader::style_SetRowStyle(StyleStore& ss,int row,const StyleDesc& style)
  //{
  //  StyleID_t sid=NULL;
  //  map<row_t,StyleID_t>::iterator it=mapStyleID_.find(row);
  //  if(mapStyleID_.end()!=it){
  //    sid=ss.ChangeStyle(it->second,style);
  //    it->second=sid;
  //  }else{
  //    sid=ss.add_style(style);
  //    mapStyleID_.insert(make_pair(row,sid));
  //  }
  //  return sid;
  //}
  void RowHeader::put_rows(StyleStore& ss,int rows)
  {
    _ASSERT(rows>=0);
    _ASSERT(rows<MAX_ROW);
    rows_=rows;
    for(map<row_t,short>::iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();)
    {
      if(it->first>=rows){
        it=mapIrregHeights_.erase(it);
        continue;
      }
      ++it;
    }
    if((int)vecRowStore_.size()>rows){
      for(size_t i=(size_t)rows;i<vecRowStore_.size();++i){
        vecRowStore_[i].ClearStyle(ss);
      }
    }
    vecRowStore_.resize(rows);
    /*
    for(map<row_t,CellStore>::iterator it=mapCellStore_.begin();it!=mapCellStore_.end();)
    {
      if(it->first>=rows){
        it->second.ClearStyle(ss);
        it=mapCellStore_.erase(it);
        continue;
      }
      ++it;
    }
    */
  }
  int RowHeader::get_heights()const
  {
    int heights=(rows_ - int(mapIrregHeights_.size()))*get_DefRowHeight();
    for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it)
    {
      heights+=it->second;
    }
    return heights;
  }
  //[row1,row2)
  int RowHeader::get_DiffHeights(int row1,int row2)const
  {
    _ASSERT(row1>=0);
    if(row1>=row2) return 0;
    int heights=(row2-row1)*get_DefRowHeight();
    for(map<row_t,short>::const_iterator it=mapIrregHeights_.begin();it!=mapIrregHeights_.end();++it)
    {
      if(it->first>=row1 && it->first<row2){
        heights+=it->second-get_DefRowHeight();
      }
    }
    return heights;
  }
  void RowHeader::put_RowHeight(int row,int hi)
  {
    _ASSERT(row>=0);
    _ASSERT(row<rows_);
    if(hi==defHeight_){
      map<row_t,short>::iterator it=mapIrregHeights_.find(row);
      if(it!=mapIrregHeights_.end())
        mapIrregHeights_.erase(it);
    }else{
      mapIrregHeights_[row]=hi;
    }
  }
  //delete range[begin,end];
  void RowHeader::delete_rows(StyleStore& ss,int beg,int end)
  {
    _ASSERT(beg>=0);
    _ASSERT(end>=beg);
    int const cnt=end-beg+1;
    rows_-=cnt;
    mycell::delete_cols<row_t,short>(mapIrregHeights_,beg,end);
    for(size_t i=(size_t)beg;i<=(size_t)end;++i)
      vecRowStore_[i].ClearStyle(ss);
    vecRowStore_.erase(vecRowStore_.begin()+beg,vecRowStore_.begin()+end);
    /*
    for(map<row_t,CellStore>::iterator it=mapCellStore_.begin();it!=mapCellStore_.end();){
      const int nk=it->first;
      if(nk>=beg && nk<=end){
        it->second.ClearStyle(ss);
        //ss.DecrementRefCount(it->second.nStyleID);
        it=mapCellStore_.erase(it);
      }else
        ++it;
    }
    //mycell::delete_cols<row_t,StyleID_t>(mapStyleID_,beg,end);
    _ASSERT((int)mapIrregHeights_.size()<=rows_);
    */
  }
  void RowHeader::insert_rows(int beg,int nCount)
  {
    rows_+=nCount;
    _ASSERT(rows_<MAX_ROW);
    mycell::insert_cols<row_t,short>(mapIrregHeights_,beg,nCount);
    if(beg < static_cast<int>(vecRowStore_.size())){
      vecRowStore_.insert(vecRowStore_.begin()+beg,nCount,CellStore());
    }else{
      vecRowStore_.resize(vecRowStore_.size()+nCount);
    }
    //vecRowStore_.insert(vecRowStore_.begin(),nCount,
    //mycell::insert_cols<row_t,CellStore>(mapCellStore_,beg,nCount);
    //_ASSERT((int)mapIrregHeights_.size()<=rows_);
  }
  int RowHeader::get_text(int row,LPTSTR buf)const
  {
    wsprintf(buf,_T("%d"),row+1);
    int i=0;
    for(;i!=MAX_CELLTEXT && buf[i];++i)
      ;
    return i;
  }
  void RowHeader::clear(StyleStore& ss)
  {
    //baseClass::clear();
    rows_=0;
    mapIrregHeights_.clear();
    style_ClearStyles(ss);
    //mapStyleID_.clear();
    topVisScrollRow_=bottomVisScrollRow_=-1;
    freezeTopRow_=freezeBottomRow_=-1;
  }

  //StyleID_t RowHeader::style_SetRowStyle(StyleStore& ss,int row,const StyleDesc& style)
  //{
  //  /*
  //  const StyleID_t sid=Style_GetRowStyleID(row);
  //  if(sid){
  //  ss.DecrementRefCount(sid);
  //  }
  //  return mapStyleID_[row]=ss.add_style(style);
  //  */
  //  _ASSERT(row>=0);
  //  _ASSERT(row<(int)vecRowStore_.size());
  //  return vecRowStore_[row].SetStyle(ss,style);
  //  /*
  //  map<row_t,CellStore>::iterator it=mapCellStore_.find(static_cast<row_t>(row));
  //  if(mapCellStore_.end()!=it){
  //    //ss.DecrementRefCount(it->second.nStyleID);
  //    if(bNullStyle){
  //      it->second.ClearStyle(ss);
  //      //it->second.nStyleID=0;
  //      if(it->second.IsEmpty())
  //        mapCellStore_.erase(it);
  //    }else{
  //      sid=it->second.SetStyle(ss,*pstyle);
  //    }
  //  }else{
  //    if(!bNullStyle){
  //      mapCellStore_[row].SetStyle(ss,*pstyle);
  //    }
  //  }
  //  */
  //  /*
  //  if(!bNullStyle){
  //    const Borders* pbdrs=pstyle->GetBorders();
  //    if(row>0){
  //      it=mapStyleID_.find(static_cast<row_t>(row-1));
  //      if(it!=mapStyleID_.end() && it->second){
  //        const StyleDesc& sd=ss.get_Style(it->second);
  //        if(!sd.GetBorders()->bottom.IsNull()&&sd.GetBorders()->bottom!=pbdrs->top){
  //          StyleDesc rs=sd;
  //          rs.GetBorders()->bottom=pbdrs->top;
  //          it->second=ss.add_style(rs);
  //        }
  //      }
  //    }
  //    if(row<rows_-1){
  //      it=mapStyleID_.find(static_cast<row_t>(row+1));
  //      if(it!=mapStyleID_.end() && it->second){
  //        const StyleDesc& sd=ss.get_Style(it->second);
  //        if(!sd.GetBorders()->top.IsNull() && sd.GetBorders()->top!=pbdrs->bottom){
  //          StyleDesc rs=sd;
  //          rs.GetBorders()->top=pbdrs->bottom;
  //          it->second=ss.add_style(rs);
  //        }
  //      }
  //    }
  //  }
  //  return sid;
  //  */
  //}
}//namespace mycell