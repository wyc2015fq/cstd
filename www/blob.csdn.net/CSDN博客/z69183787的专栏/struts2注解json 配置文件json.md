# struts2注解json 配置文件json - z69183787的专栏 - CSDN博客
2015年06月27日 10:28:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3373
**result 中一定要 有name 属性和方法中的return对应**
**1.注解Action中写法：**
package cn.damai.subscription.action; 
@ParentPackage("json-default") 
public class TestAction extends BaseAction{
@Action(value="/faq/queryjsonList",results=@Result(type="json",name="jsonlist",params={"includeProperties","rows.*"})) 
public String queryjsonList(){ 
    this.rows = infoService.selectByTitleAndTypeForNormal(title, type); 
    return "jsonlist"; 
}
}
**2.在配置文件中实现设置的写法：**
<struts>
<package name="qp" extends="json-default">
<!-- 通知单受理 -->
<action name="noticebill_*" class="noticebillAction" method="{1}">
<!-- 根据手机查询客户 -->
<result name="findCustomerByTelephoneSUCCESS" type="json">
<param name="root">result</param>
</result>
<!-- 业务受理 -->
<result name="saveSUCCESS" >/WEB-INF/pages/qupai/noticebill_add.jsp</result>
<!-- 人工调度，查询操作 -->
<result name="findnoassociationsSUCCESS" type="json"> 
     //action中对应的方法
               public String findnoassociations(){
                     List<NoticeBill> noticeBills = noticeBillService.findnoassociations();
                     ActionContext.getContext().put("noticeBills", noticeBills);
                      return "findnoassociationsSUCCESS";
               }
      <param name="root">noticeBills</param>
       <param name="includeProperties">
              \[\d+\]\.id,
              \[\d+\]\.delegater,
              \[\d+\]\.telephone,
              \[\d+\]\.pickaddress,
              \[\d+\]\.product,
             \[\d+\]\.pickdate
       </param>
</result>
      <result name="pageQuerySUCCESS" type="json">
             <param name="root">pageResponseBean</param>
             <param name="includeProperties">
                      total,
                      rows\[\d+\]\.id,
                      rows\[\d+\]\.arrivecity,
                      rows\[\d+\]\.product,
                      rows\[\d+\]\.num,
                      rows\[\d+\]\.weight,
                      rows\[\d+\]\.floadreqr
             </param>
    </result>
</action>
</package>
</struts>
对应的类文件：
public class PageResponseBean {
private long total ; // 总记录数
private List rows ; // 当页的数据记录
public long getTotal() {
return total;
}
public void setTotal(long total) {
this.total = total;
}
public List getRows() {
return rows;
}
public void setRows(List rows) {
this.rows = rows;
}
}
