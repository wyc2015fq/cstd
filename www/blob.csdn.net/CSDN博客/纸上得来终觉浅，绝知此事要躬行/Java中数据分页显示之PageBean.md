# Java中数据分页显示之PageBean - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年08月31日 17:05:44[boonya](https://me.csdn.net/boonya)阅读数：18011







 1、编写PageBean工具类：

 package com.accp.page.util;

import java.util.ArrayList;

import java.util.List;

public class PageBean<T> {
private int pageIndex = 1;    // 需要显示的页码
private int totalPages = 1;   // 总页数
private int pageSize = 10;    // 每页记录数
private int totalRecords = 0; // 总记录数
private boolean isHavePrePage = false;  // 是否有上一页
private boolean isHaveNextPage = false; // 是否有下一页

private List<T> pageDatas = new ArrayList<T>();

public int getPageIndex() {
return pageIndex;
}

public void setPageIndex(int pageIndex) {
this.pageIndex = pageIndex;
}

public int getPageSize() {
return pageSize;
}

public void setPageSize(int pageSize) {
this.pageSize = pageSize;
}

public int getTotalRecords() {
return totalRecords;
}

public void setTotalRecords(int totalRecords) {
if(totalRecords < 0){
throw new RuntimeException("总记录数不能小于0!");
}
//设置总记录数
this.totalRecords = totalRecords;
//计算总页数
this.totalPages = this.totalRecords/this.pageSize;
if(this.totalRecords%this.pageSize!=0){
this.totalPages++;
}
//计算是否有上一页
if(this.pageIndex>1){
this.isHavePrePage = true;
}else{
this.isHavePrePage = false;
}
//计算是否有下一页
if(this.pageIndex<this.totalPages){
this.isHaveNextPage = true;
}else{
this.isHaveNextPage = false;
}
}

public List<T> getPageDatas() {
return pageDatas;
}

public void setPageDatas(List<T> pageDatas) {
this.pageDatas = pageDatas;
}

public int getTotalPages() {
return totalPages;
}

public boolean getIsHavePrePage() {
return isHavePrePage;
}

public boolean getIsHaveNextPage() {
return isHaveNextPage;
}


}

2、调用方法时的实现（实例）：

 public void getPagePetType(PageBean<PetType> pageBean){
String sql = "(select t.*,rownum as r from pet_type t where rownum <= "
                      +pageBean.getPageIndex()*pageBean.getPageSize()+")";
sql = "select *  from " + sql + " where r > "
                     +(pageBean.getPageIndex()-1)*pageBean.getPageSize();

//计算总记录数
String csql = "select count(*) from pet_type";
int tcount = ((BigDecimal)super.getObject(csql)).intValue();
pageBean.setTotalRecords(tcount);

List<PetType> petTypes = super.query(sql, new ResultSetter(){
              public List<PetType> setResultSetData(ResultSet rs) throws SQLException {
                    List<PetType> pts = new ArrayList<PetType>();
                     while(rs.next()){
                           PetType pt = new PetType();
                           pt.setId(rs.getInt(1));
                           pt.setName(rs.getString(2));
                           pt.setStatus(rs.getInt(3));
                          pts.add(pt);
                   }
                return pts;
              }
});
pageBean.setPageDatas(petTypes);

    }

3、访问数据库基类：

public class BaseDaoImpl<T> {
private Connection conn = null;
private PreparedStatement pst = null;
private ResultSet rs = null;

/**
* 更新(增加到、删除、修改)数据的方法
* 
* @param sql
* @param paras
* @return
*/
public int updateData(String sql, Object[] paras) {
this.createPreparedStatement(sql, paras);
int result = 0;
try {
result = this.pst.executeUpdate();
} catch (SQLException e) {
e.printStackTrace();
} finally {
ConnectionUtils.close(rs, pst, conn);
}
return result;
}

public int updateData(String sql) {
return this.updateData(sql, null);
}

public List<T> query(String sql, ResultSetter rssetter) {
return this.query(sql,null,rssetter);
}

public List<T> query(String sql, Object[] paras, ResultSetter rssetter) {
this.createPreparedStatement(sql, paras);
try {
ResultSet rs = this.pst.executeQuery();
// 将ResultSet中的数据封装到集合中[]
return rssetter.setResultSetData(rs);
} catch (SQLException e) {
e.printStackTrace();
} finally {
ConnectionUtils.close(rs, pst, conn);
}

return null;
}

public Object getObject(String sql){
return this.getObject(sql, null);
}

public Object getObject(String sql, Object[] paras){
Object robj = null;
this.createPreparedStatement(sql, paras);
try {
ResultSet rs = this.pst.executeQuery();
// 将ResultSet中的数据封装到集合中[]
   if(rs.next()){
robj = rs.getObject(1);
   }
} catch (SQLException e) {
e.printStackTrace();
} finally {
ConnectionUtils.close(rs, pst, conn);
}
return robj;
}

/**
* 创建数据操作对象
* 
* @param sql
* @param paras
*/
private void createPreparedStatement(String sql, Object[] paras) {
this.conn = ConnectionUtils.getConnection();
try {
this.pst = this.conn.prepareStatement(sql);
// 绑定动态参数
if (paras != null && paras.length > 0) {
for (int i = 0; i < paras.length; i++) {
this.pst.setObject(i + 1, paras[i]);
}
}
} catch (SQLException e) {
e.printStackTrace();
}
}


}




4、jsp页面使用：


提示：可以用JSTL、OGNL、Struts标签，获取pageBean操作相应的分页逻辑控制。






