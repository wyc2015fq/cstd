# XLSTransformer生成excel文件简单示例 - z69183787的专栏 - CSDN博客
2014年12月01日 13:37:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4377
项目结构图：
项目中所用到的jar，可以到[http://www.findjar.com/index.x](http://www.findjar.com/index.x)下载
![](http://hi.csdn.net/attachment/201112/19/0_13242892377kJr.gif)
ExcelUtil类源码：
**[java]**[view plain](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)[copy](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)
- package util;  
- 
- import java.io.IOException;  
- import java.net.URL;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- 
- import net.sf.jxls.exception.ParsePropertyException;  
- import net.sf.jxls.transformer.XLSTransformer;  
- /**
-  * Excel生成类.
-  */
- publicclass ExcelUtil {  
- /**
-      * 根据模板生成Excel文件.
-      * @param templateFileName 模板文件.
-      * @param list 模板中存放的数据.
-      * @param resultFileName 生成的文件.
-      */
- publicvoid createExcel(String templateFileName, List<?> list, String resultFileName){  
- //创建XLSTransformer对象
-         XLSTransformer transformer = new XLSTransformer();  
- //获取java项目编译后根路径
-         URL url = this.getClass().getClassLoader().getResource("");  
- //得到模板文件路径
-         String srcFilePath = url.getPath() + templateFileName;  
-         Map<String,Object> beanParams = new HashMap<String,Object>();  
-         beanParams.put("list", list);  
-         String destFilePath = url.getPath() + resultFileName;  
- try {  
- //生成Excel文件
-             transformer.transformXLS(srcFilePath, beanParams, destFilePath);  
-         } catch (ParsePropertyException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- }  
Test类源码：
**[java]**[view plain](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)[copy](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)
- package test;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import po.Fruit;  
- import util.ExcelUtil;  
- /**
-  * 测试类.
-  */
- publicclass Test {  
- 
- publicstaticvoid main(String[] args) {  
-         List<Fruit> list = new ArrayList<Fruit>();  
-         list.add(new Fruit("苹果",2.01f));  
-         list.add(new Fruit("桔子",2.05f));  
-         String templateFileName = "template/template.xls";  
-         String resultFileName = "result/fruit.xls";  
- new ExcelUtil().createExcel(templateFileName,list,resultFileName);  
- 
-     }  
- 
- }  
template.xls模板文件截图：
![](http://hi.csdn.net/attachment/201112/19/0_1324289488C4cO.gif)
注意：如果你是用的office 2007生成的excel模板，要另存为97-2003版本的。
Fruit类源码：
**[java]**[view plain](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)[copy](http://blog.csdn.net/csdnbenbenchong/article/details/7085124#)
- package po;  
- /**
-  * 水果.
-  */
- publicclass Fruit {  
- /**
-      * 水果名称.
-      */
- private String name;  
- /**
-      * 水果价格.
-      */
- privatefloat price;  
- 
- 
- public Fruit() {  
- super();  
-     }  
- 
- public Fruit(String name, float price) {  
- super();  
- this.name = name;  
- this.price = price;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- publicfloat getPrice() {  
- return price;  
-     }  
- publicvoid setPrice(float price) {  
- this.price = price;  
-     }  
- 
- }  
生成fruit.xls文件截图：
![](http://hi.csdn.net/attachment/201112/19/0_1324289724h6J7.gif)
