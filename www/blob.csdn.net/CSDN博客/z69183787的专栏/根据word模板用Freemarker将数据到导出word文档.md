# 根据word模板用Freemarker将数据到导出word文档 - z69183787的专栏 - CSDN博客
2014年01月23日 19:59:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2009
在struts框架中应用：
1.用word写一个需要导出的模板，将需要动态修改的内容替换成***。然后另存为testxml.xml格式，后缀名修改为testxml.ftl。
![根据word模板用Freemarker将数据到导出word文档](http://s5.sinaimg.cn/mw690/c6c3646atdcd0371ebe94&690)
![根据word模板用Freemarker将数据到导出word文档](http://s4.sinaimg.cn/mw690/c6c3646at7c7b37fc2ab3&690)
注意：
（1）编写模板时应使用Microsoft Office Word 2003，以避免出现导出的word文档不能用Word 2003打开的问题。
（2）同时要注意word格式问题。如果在word导出成功之后发现格式不协调等问题，就只能从新来过了。
2.用Firstobject free XML editor打开ftl文件，选择Tools下的Indent【或者按快捷键F8】格式化文件内容。左边是ftl的文档结构，右边是文档内容。
![根据word模板用Freemarker将数据到导出word文档](http://s3.sinaimg.cn/mw690/c6c3646atdcd057e4d2e2&690)
3.将testxml.ftl文件中需要动态修改内容的地方，换成freemarker的标识。其实就是Map<String, Object>中key，如${name},如果需要循环，则使用： <#list grade.list as student>...</#list>
![根据word模板用Freemarker将数据到导出word文档](http://s16.sinaimg.cn/mw690/c6c3646atdcd0796c26cf&690)
![根据word模板用Freemarker将数据到导出word文档](http://s1.sinaimg.cn/mw690/c6c3646atdcd079b99140&690)
4.标识替换完之后，模板就弄完了。
特别注意：一定不要用word打开ftl模板文件，否则xml内容会发生变化，导致前面的工作白做了。
5.将testxml.ftl文件放到项目中：
![根据word模板用Freemarker将数据到导出word文档](http://s1.sinaimg.cn/mw690/c6c3646atdcd09a01f070&690)
1》TheFreemarker.java
package com.cstp.free;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import freemarker.template.Configuration;
import freemarker.template.Template;
import freemarker.template.TemplateException;
public class TheFreemarker {
 private Configuration configuration=null;
 public TheFreemarker(){
  configuration=new Configuration();
  configuration.setDefaultEncoding("utf-8");
 }
 public void createDoc(){
  Map dateMap=new HashMap();
  getData(dateMap);
// 模板放在com.canyou.template包下面，通过classpath装载
  configuration.setClassForTemplateLoading(this.getClass(), "/com/cstp/free"); 
 Template t=null;
  try {
   t=configuration.getTemplate("testxml.ftl");// 设置要装载的模板
  } catch (IOException e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
  File outFile=new File("D:/outFileDoc.doc");
  Writer out=null;
  try {
   out=new BufferedWriter(new OutputStreamWriter(new FileOutputStream(outFile),"utf-8"));
  } catch (UnsupportedEncodingException e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  } catch (FileNotFoundException e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
  try {
   t.process(dateMap, out);
   out.close();
  } catch (TemplateException e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  } catch (IOException e) {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
 }
 private void getData(Map dataMap){
  dataMap.put("name", "经理");
  //dataMap.put("Tdate", "2013-05-15");
  List list=new ArrayList();
  for(int i=0;i<5;i++){
   Table2 tt=new Table2();
   tt.setCode("1");
   tt.setUsername("小明");
   tt.setDate("2013-15-13");
   tt.setQujian("aaa");
   list.add(tt);
  }
  dataMap.put("table2", list);
 }
 public static void main(String[] args) {
  TheFreemarker th=new TheFreemarker();
  th.createDoc();
 }
}
2》Table2.java
![根据word模板用Freemarker将数据到导出word文档](http://s11.sinaimg.cn/mw690/c6c3646atdcd0a71b02da&690)
