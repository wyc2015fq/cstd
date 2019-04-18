# java使用jxls导出excel功能 - z69183787的专栏 - CSDN博客
2016年05月14日 17:27:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2085
个人分类：[Excel处理-导出](https://blog.csdn.net/z69183787/article/category/2748995)
在java的开发里,导出excel功能会经常遇到的,而jxls框架提供了很大的方便.
jxls官网:http://jxls.sourceforge.net/,官网的下载包里面也提供详细的文档说明.
下文提供思路,主要以web项目导出excel来举例jxls用法,文末也贴一下java项目导出excel功能(这个简单一点)
直接贴关键代码(其它关于怎么用java config方式的spring mvc不做说明)
1.引用jxls依赖
**[html]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/38906683#)[copy](http://blog.csdn.net/xiejx618/article/details/38906683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1555473/fork)
- <dependency>
- <groupId>net.sf.jxls</groupId>
- <artifactId>jxls-core</artifactId>
- <version>1.0.5</version>
- </dependency>
2.EmployeeController.java
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/38906683#)[copy](http://blog.csdn.net/xiejx618/article/details/38906683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1555473/fork)
- package com.exam.web;  
- import com.exam.domain.Employee;  
- import net.sf.jxls.transformer.XLSTransformer;  
- import org.apache.poi.openxml4j.exceptions.InvalidFormatException;  
- import org.apache.poi.ss.usermodel.Workbook;  
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import java.io.*;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- @Controller
- @RequestMapping("/employee")  
- publicclass EmployeeController {  
- /**
-      * 这里接管了response,我们自己把response flush出去,所以返回类型是void(不要用什么String,否则会报响应已提交过的异常哦)
-      */
- @RequestMapping("/export")  
- publicvoid export(HttpServletRequest request,HttpServletResponse response){  
-         String templateFileName= request.getServletContext().getRealPath("/") + "/resources/templateFileName.xls";  
-         String destFileName= "destFileName.xls";  
- //模拟数据
-         List<Employee> staff = new ArrayList<Employee>();  
-         staff.add(new Employee("Derek", 35, 3000, 0.30));  
-         staff.add(new Employee("Elsa", 28, 1500, 0.15));  
-         staff.add(new Employee("Oleg", 32, 2300, 0.25));  
-         Map<String,Object> beans = new HashMap<String,Object>();  
-         beans.put("employees", staff);  
-         XLSTransformer transformer = new XLSTransformer();  
-         InputStream in=null;  
-         OutputStream out=null;  
- //设置响应
-         response.setHeader("Content-Disposition", "attachment;filename=" + destFileName);  
-         response.setContentType("application/vnd.ms-excel");  
- try {  
-             in=new BufferedInputStream(new FileInputStream(templateFileName));  
-             Workbook workbook=transformer.transformXLS(in, beans);  
-             out=response.getOutputStream();  
- //将内容写入输出流并把缓存的内容全部发出去
-             workbook.write(out);  
-             out.flush();  
-         } catch (InvalidFormatException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } finally {  
- if (in!=null){try {in.close();} catch (IOException e) {}}  
- if (out!=null){try {out.close();} catch (IOException e) {}}  
-         }  
-     }  
- }  
3.templateFileName.xls(内容是二进制,不容易贴出来)
![](https://img-blog.csdn.net/20140828224751345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGllang2MTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个JEXL语法和JSTL语法十分相似.
这里我们自己处理了Response,没必要交给spring的视图解析器.在返回响应前我们设置了响应的Header和ContentType以便客户端的浏览器解析为弹出下载.这里调用了transformer.transformXLS(in, beans)方法得到一个Workbook,这方法会把模板(至于模板怎么写,详细看官方文档)先读入到输入流再处理,再把Workbook的内容写入到输出流发送出去.
上文源码:[http://download.csdn.net/detail/xiejx618/7836191](http://download.csdn.net/detail/xiejx618/7836191)
如果不想使用上面的模板，还有一种更灵活自定义的方法(比如编码定义excel样式,动态增加多个sheet等等),也可以在Controller方法直接返回AbstractXlsView.下面是一个例子:
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/38906683#)[copy](http://blog.csdn.net/xiejx618/article/details/38906683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1555473/fork)
- @RequestMapping(value = "listXls",method = RequestMethod.GET)  
- public View listXls() {  
-         List<User> list = getUsers();  
- returnnew AbstractXlsView() {  
- @Override
- protectedvoid buildExcelDocument(Map<String, Object> model, Workbook workbook, HttpServletRequest request, HttpServletResponse response) throws Exception {  
-                 addSheet(workbook.createSheet("UserStatistics1"), list.subList(0, 50));//不包括第50个
-                 addSheet(workbook.createSheet("UserStatistics2"), list.subList(50,list.size()));  
-             }  
-         };  
-     }  
- 
- private List<User> getUsers() {  
-         List<User> list = new ArrayList<>();  
- for (int i = 0; i < 103; i++) {  
-             User user = new User();  
-             user.setUsername("name" + i);  
-             user.setPassword("pwd" + i);  
-             user.setId((long) i);  
-             list.add(user);  
-         }  
- return list;  
-     }  
- 
- privatevoid addSheet(Sheet sheet, List<User> list) {  
- int rowNum=0;  
-         Row row0 = sheet.createRow(rowNum++);  
-         row0.createCell(0).setCellValue("ID");  
-         row0.createCell(1).setCellValue("Username");  
-         row0.createCell(2).setCellValue("Password");  
-         row0.createCell(3).setCellValue("Height");  
- for (User user : list) {  
-             Row row = sheet.createRow(rowNum++);  
-             row.createCell(0).setCellValue(user.getId());  
-             row.createCell(1).setCellValue(user.getUsername());  
-             row.createCell(2).setCellValue(user.getPassword());  
-             row.createCell(3).setCellValue(user.getHeight());  
-         }  
-     }  
顺便贴一下java项目(不是web项目)的例子源码:[http://download.csdn.net/detail/xiejx618/7836227](http://download.csdn.net/detail/xiejx618/7836227)
