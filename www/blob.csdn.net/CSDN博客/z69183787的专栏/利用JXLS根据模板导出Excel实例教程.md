# 利用JXLS根据模板导出Excel实例教程 - z69183787的专栏 - CSDN博客
2016年05月14日 17:20:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5424
看了几天了，感觉迷迷糊糊的，今天终于搞出来了。
**先做模板，做成想要的格式样子保存，然后通过程序根据模板生成对应样式的Excel文件，代码简单。什么连接数据库查询然后将结果生成Excel文件就不讲了，放入List里面，然后套一下就行了，照老虎花猫。**
准备：
1，相关jar包：
![](https://img-blog.csdn.net/20140317170255968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbzUwNjMy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2，模板文件 ：
![](https://img-blog.csdn.net/20140317170450296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbzUwNjMy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
开始，
1、 先实体类：Staff.java
**[java]**[view
 plain](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/240615/fork)
- package myjxls;  
- /**
-  * 2014-3-17
-  * 8dou
-  * 实体
-  */
- publicclass Staff {  
- 
- /**
-          * 名称
-          */
- private String name;  
- 
- /**
-          * 薪资
-          */
- private Double payment;  
- 
- /**
-          * 年终奖
-          */
- private Double bonus;  
- 
- public String getName() {  
- return name;  
-         }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-         }  
- 
- public Double getPayment() {  
- return payment;  
-         }  
- 
- publicvoid setPayment(Double payment) {  
- this.payment = payment;  
-         }  
- 
- public Double getBonus() {  
- return bonus;  
-         }  
- 
- publicvoid setBonus(Double bonus) {  
- this.bonus = bonus;  
-         }  
- public Staff(String name, Double payment, Double bonus) {  
- super();  
- this.name = name;  
- this.payment = payment;  
- this.bonus = bonus;  
-         }  
- }  
2、测试类 ChartTest.java 
**[java]**[view
 plain](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/240615/fork)
- package myjxls;  
- /**
-  * 2014-3-17
-  * 8dou
-  * 测试JXLS根据模板样式导出Excel
-  */
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- 
- import net.sf.jxls.transformer.XLSTransformer;  
- publicclass ChartTest {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) throws Exception {  
-         List<Staff> staffs = new ArrayList<Staff>();  
- 
-         Staff s1 = new Staff("张三", 6000D, 3000D);  
-         staffs.add(s1);  
- 
-         Staff s2 = new Staff("李四", 5000D, 2000D);  
-         staffs.add(s2);  
- 
-         Staff s3 = new Staff("王五", 4000D, 1000D);  
-         staffs.add(s3);  
- 
-         String srcFilePath = "e:/simple.xlsx";  
-         String destFilePath = "e:/template-simple.xlsx";  
-         Map<String, List<Staff>> beanParams = new HashMap<String, List<Staff>>();  
-         beanParams.put("staffs", staffs);  
- 
-         XLSTransformer former = new XLSTransformer();  
-         former.transformXLS(srcFilePath, beanParams, destFilePath);  
- 
-         System.out.println("the end !!!");  
-     }  
- 
- }  
运行结束后看生成的Excel文件，template-simple.xlsx
![](https://img-blog.csdn.net/20140317170945890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbzUwNjMy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果是Web,需要下载可以看
**[java]**[view
 plain](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/zhao50632/article/details/21397685?utm_source=tuicool&utm_medium=referral#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/240615/fork)
- // 下载
- publicstaticvoid doDownLoad(String path, String name,  
-         HttpServletResponse response) {  
- try {  
-         response.reset();  
-         response.setHeader("Content-disposition",  
- "attachment;success=true;filename ="
-                         + URLEncoder.encode(name, "utf-8"));  
-         BufferedInputStream bis = null;  
-         BufferedOutputStream bos = null;  
-         OutputStream fos = null;  
-         InputStream fis = null;  
-         File uploadFile = new File(path);  
-         fis = new FileInputStream(uploadFile);  
-         bis = new BufferedInputStream(fis);  
-         fos = response.getOutputStream();  
-         bos = new BufferedOutputStream(fos);  
- // 弹出下载对话框
- int bytesRead = 0;  
- byte[] buffer = newbyte[8192];  
- while ((bytesRead = bis.read(buffer, 0, 8192)) != -1) {  
-             bos.write(buffer, 0, bytesRead);  
-         }  
-         bos.flush();  
-         fis.close();  
-         bis.close();  
-         fos.close();  
-         bos.close();  
-     } catch (Exception e) {  
-         e.printStackTrace();  
-     }  
- }  
最后补充下Excel知识：在单元格里面将日期和时间显示在同一个单元格里面，自定义单元格式→yyyy-m-d hh:mm:ss
![](https://img-blog.csdn.net/20140318144014734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbzUwNjMy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
