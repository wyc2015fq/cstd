# JAVA利用apache的common-lang库转义HTML、SQL、Unicode - z69183787的专栏 - CSDN博客
2016年11月24日 14:32:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：806
转义通常用在需要写入[数据库](http://lib.csdn.net/base/mysql)的时候，经过转义可以避免很多潜在的问题，使系统更健壮。但是[Java](http://lib.csdn.net/base/javaee)并没有自带相关类库。通常我们使用Apache开源的common-lang类库。
Apache common lang最新版下载地址：[点击下载](http://mirrors.cnnic.cn/apache//commons/lang/binaries/commons-lang3-3.4-bin.zip)
**[java]**[view
 plain](http://blog.csdn.net/yxl8359026/article/details/49872585#)[copy](http://blog.csdn.net/yxl8359026/article/details/49872585#)
- import org.apache.commons.lang.StringEscapeUtils;  
- 
- publicclass StringEscapeUtilsTest {  
- 
- publicstaticvoid main(String args[]){  
- 
-         String sql="1' or '1'='1";  
-         System.out.println("防SQL注入:"+StringEscapeUtils.escapeSql(sql)); //防SQL注入
- 
-         System.out.println("转义HTML,注意汉字:"+StringEscapeUtils.escapeHtml3("<font>chen磊  xing</font>"));   //转义HTML,注意汉字
-         System.out.println("反转义HTML:"+StringEscapeUtils.unescapeHtml3("<font>chen磊  xing</font>")); //反转义HTML
- 
-         System.out.println("转成Unicode编码："+StringEscapeUtils.escapeJava("张三"));  //转义成Unicode编码
- 
-         System.out.println("转义XML："+StringEscapeUtils.escapeXml("<name>张三</name>"));    //转义xml
-         System.out.println("反转义XML："+StringEscapeUtils.unescapeXml("<name>张三</name>"));     //转义xml
- 
-     }  
- }  
- 
