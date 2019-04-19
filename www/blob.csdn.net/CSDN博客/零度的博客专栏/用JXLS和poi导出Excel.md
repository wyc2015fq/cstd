# 用JXLS和poi导出Excel - 零度的博客专栏 - CSDN博客
2016年05月05日 14:42:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：6556
apache的poi提供了支持MS Office文档操作的java API。jXLS则是基于poi提供了支持Excel模版操作的API。
jXLS的主页是：http://jxls.sourceforge.net，似乎国内不能访问，需要代理。
下面是一些例子，以及我对导出到多个sheet的一个封装。
先定义一个模版：
![](http://dl2.iteye.com/upload/attachment/0089/7331/695fd5a3-c488-3e13-bf41-794534c9fd85.jpg)
 这里的jxls tag和JSP的tag很像，varStatus是本次循环变量。
jxls提供了一个最简单的接口：
void net.sf.jxls.transformer.XLSTransformer.transformXLS(String srcFilePath,Map
 beanParams, String destFilePath) throwsParsePropertyException,IOException,InvalidFormatException
```
public class VM {
	
	private String id;
	private String name;
	private String scale;
	private Date created;
	private Double price;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getScale() {
		return scale;
	}
	public void setScale(String scale) {
		this.scale = scale;
	}
	public Date getCreated() {
		return created;
	}
	public void setCreated(Date created) {
		this.created = created;
	}
	public Double getPrice() {
		return price;
	}
	public void setPrice(Double price) {
		this.price = price;
	}
	
	
}
```
Java代码 ![复制代码](http://jamie-wang.iteye.com/images/icon_copy.gif)![收藏代码](http://jamie-wang.iteye.com/images/icon_star.png)![](http://jamie-wang.iteye.com/images/spinner.gif)
- String srcFilePath = "E:\\tmp\\template-simple.xlsx";  
- Map<String, Object> beanParams = new HashMap<String, Object>();  
- List<VM> vms = new ArrayList<VM>();  
- VM vm = new VM();  
- vm.setName("我的CENTOS");  
- vm.setPrice(103);  
- vm.setScale("2CPU, 2G MEM, 2T DISK");  
- vm.setCreated(new Date());  
- vms.add(vm);  
- VM vm2 = new VM();  
- vm2.setName("my-ubuntu");  
- vm2.setPrice(200);  
- vm2.setScale("1CPU, 3G MEM, 1T DISK");  
- vm2.setCreated(new Date());  
- vms.add(vm2);  
- beanParams.put("vms", vms);  
- 
- String destFilePath = "E:\\tmp\\simple.xlsx";  
- 
- XLSTransformer transformer = new XLSTransformer();  
- transformer.transformXLS(srcFilePath, beanParams, destFilePath);  
```java
String srcFilePath = "E:\\tmp\\template-simple.xlsx";
        Map<String, Object> beanParams = new HashMap<String, Object>();
        List<VM> vms = new ArrayList<VM>();
        VM vm = new VM();
        vm.setName("我的CENTOS");
        vm.setPrice(103);
        vm.setScale("2CPU, 2G MEM, 2T DISK");
        vm.setCreated(new Date());
        vms.add(vm);
        VM vm2 = new VM();
        vm2.setName("my-ubuntu");
        vm2.setPrice(200);
        vm2.setScale("1CPU, 3G MEM, 1T DISK");
        vm2.setCreated(new Date());
        vms.add(vm2);
        beanParams.put("vms", vms);
        String destFilePath = "E:\\tmp\\simple.xlsx";
        XLSTransformer transformer = new XLSTransformer();
        transformer.transformXLS(srcFilePath, beanParams, destFilePath);
```
执行结果：
![](http://dl2.iteye.com/upload/attachment/0089/7349/4b6ed6d4-2416-3a90-8a81-7a7768ed99f4.jpg)
jXLS提供了很多方法，支持很复杂的表格生成。
但如果要指定在一个sheet最大行数，不是很方便，我包装了一个util方法：
Java代码 ![复制代码](http://jamie-wang.iteye.com/images/icon_copy.gif)![收藏代码](http://jamie-wang.iteye.com/images/icon_star.png)![](http://jamie-wang.iteye.com/images/spinner.gif)
- package org.jamee.demo.poiexcel;  
- 
- import java.io.FileInputStream;  
- import java.io.FileOutputStream;  
- import java.io.InputStream;  
- import java.io.OutputStream;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- 
- import net.sf.jxls.transformer.XLSTransformer;  
- 
- import org.apache.poi.ss.usermodel.Workbook;  
- 
- publicclass ExcelUtil {  
- privatestaticfinal String DEFAULT_SHEET_NAME = "sheet";  
- 
- /**
-      * 导出无动态表头的Excel文件
-      * <p>
-      * 参考重载的有动态表头注释
-      * </p>
-      * @param destOutputStream
-      * @param templateInputStream
-      * @param data
-      * @param dataKey
-      * @param maxRowPerSheet
-      * @throws Exception
-      */
- @SuppressWarnings("rawtypes")  
- publicstaticvoid generateExcelByTemplate(OutputStream destOutputStream,   
-             InputStream templateInputStream,   
-             List data, String dataKey,  
- int maxRowPerSheet) throws Exception {  
-         generateExcelByTemplate(destOutputStream,  
-                 templateInputStream,  
- null, null,  
-                 data, dataKey,  
-                 maxRowPerSheet);  
-     }  
- 
- /**
-      * 通过Excel模版生成Excel文件
-      * <p>
-      * 创建Excel模版，变量类似JSP tag风格。
-      * 例如：
-      * <ul>
-      * <li>无动态表头
-      * <pre>
-      * 序号   名称  规格  创建时间    价格
-      * <jx:forEach items="${vms}" var="vm">
-      * ${vm.id} ${vm.name} ${vm.scale} ${vm.created} ${vm.price}
-      * </jx:forEach>
-      * </pre>
-      * </li>
-      * <li>有动态表头
-      * <pre>
-      * 项目/数量/时间    <jx:forEach items="${dates}" var="date">    ${date} </jx:forEach>
-      * <jx:forEach items="${itemsx}" var="item">            
-      * ${item.name}    <jx:forEach items="${item.counts}" var="count"> ${count}    </jx:forEach>
-      * </jx:forEach>           
-      * </pre>
-      * </li>
-      * </ul>
-      * 调用该方法则生成对应的Excel文件。
-      * </p>
-      * <p>
-      * 注意：dataKey不能是items, items是保留字，如果用items则会提示：Collection is null并抛出NullPointerException
-      * </p>
-      * @param destOutputStream Excel输出流
-      * @param templateInputStream Excel模版输入流
-      * @param header 动态表头
-      * @param headerKey 表头的变量
-      * @param data 数据项
-      * @param dataKey 数据项变量
-      * @param maxRowPerSheet 每个sheet最多行数
-      * @throws Exception
-      */
- @SuppressWarnings("rawtypes")  
- publicstaticvoid generateExcelByTemplate(OutputStream destOutputStream,   
-             InputStream templateInputStream,   
-             List header, String headerKey,  
-             List data, String dataKey,  
- int maxRowPerSheet) throws Exception {  
- 
-         List<List> splitData = null;  
- @SuppressWarnings("unchecked")  
-         Map<String, List> beanMap = new HashMap();  
-         List<String> sheetNames = new ArrayList<String>();  
- if (data.size() > maxRowPerSheet) {  
-             splitData = splitList(data, maxRowPerSheet);  
-             sheetNames = new ArrayList<String>(splitData.size());  
- for (int i = 0; i < splitData.size(); ++i) {  
-                 sheetNames.add(DEFAULT_SHEET_NAME  + i);  
-             }  
-         } else {  
-             splitData = new ArrayList<List>();  
-             sheetNames.add(DEFAULT_SHEET_NAME + 0);  
-             splitData.add(data);  
-         }  
- if (null != header) {  
-             beanMap.put(headerKey, header);  
-         }  
-         XLSTransformer transformer = new XLSTransformer();  
-         Workbook workbook = transformer.transformMultipleSheetsList(  
-                 templateInputStream, splitData, sheetNames, dataKey, beanMap, 0);  
-         workbook.write(destOutputStream);  
-     }  
- 
- 
- /**
-      * 导出无动态表头的Excel文件，目标文件和模版文件均为文件路径
-      * <p>
-      * 参考重载的有动态表头注释
-      * </p>
-      * @param destFilePath
-      * @param templateFilePath
-      * @param data
-      * @param dataKey
-      * @param maxRowPerSheet
-      * @throws Exception
-      */
- @SuppressWarnings("rawtypes")  
- publicstaticvoid generateExcelByTemplate(String destFilePath,   
-             String templateFilePath,   
-             List data, String dataKey, int maxRowPerSheet) throws Exception {  
-         generateExcelByTemplate(destFilePath, templateFilePath, null, null, data, dataKey, maxRowPerSheet);  
-     }  
- 
- /**
-      * 导出有动态表头的Excel文件，目标文件和模版文件均为文件路径
-      * <p>
-      * 参考重载的有动态表头注释
-      * </p>
-      * @param destFilePath
-      * @param templateFilePath
-      * @param header
-      * @param headerKey
-      * @param data
-      * @param dataKey
-      * @param maxRowPerSheet
-      * @throws Exception
-      */
- @SuppressWarnings("rawtypes")  
- publicstaticvoid generateExcelByTemplate(String destFilePath,   
-             String templateFilePath,   
-             List header, String headerKey,  
-             List data, String dataKey, int maxRowPerSheet) throws Exception {  
-         generateExcelByTemplate(new FileOutputStream(destFilePath),  
- new FileInputStream(templateFilePath),  
-                 header, headerKey,  
-                 data, dataKey, maxRowPerSheet);  
-     }  
- 
- @SuppressWarnings({ "rawtypes", "unchecked" })  
- privatestatic List<List> splitList(List data, int maxRowPerSheet) {  
-         List<List> splitData = new ArrayList<List>();  
-         List sdata = null;  
- for (int i = 0; i < data.size(); ++i) {  
- if (0 == i % maxRowPerSheet) {  
- if (null != sdata) {  
-                     splitData.add(sdata);  
-                 }  
-                 sdata = new ArrayList(maxRowPerSheet);  
-             }  
-             sdata.add(data.get(i));  
-         }  
- if (0 != maxRowPerSheet % data.size()) {  
-             splitData.add(sdata);  
-         }  
- 
- return splitData;  
-     }  
- }  
```java
package org.jamee.demo.poiexcel;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import net.sf.jxls.transformer.XLSTransformer;
import org.apache.poi.ss.usermodel.Workbook;
public class ExcelUtil {
    private static final String DEFAULT_SHEET_NAME = "sheet";
    /**
     * 导出无动态表头的Excel文件
     * <p>
     * 参考重载的有动态表头注释
     * </p>
     * @param destOutputStream
     * @param templateInputStream
     * @param data
     * @param dataKey
     * @param maxRowPerSheet
     * @throws Exception
     */
    @SuppressWarnings("rawtypes")
    public static void generateExcelByTemplate(OutputStream destOutputStream, 
            InputStream templateInputStream, 
            List data, String dataKey,
            int maxRowPerSheet) throws Exception {
        generateExcelByTemplate(destOutputStream,
                templateInputStream,
                null, null,
                data, dataKey,
                maxRowPerSheet);
    }
    /**
     * 通过Excel模版生成Excel文件
     * <p>
     * 创建Excel模版，变量类似JSP tag风格。
     * 例如：
     * <ul>
     * <li>无动态表头
     * <pre>
     * 序号   名称  规格  创建时间    价格
     * <jx:forEach items="${vms}" var="vm">
     * ${vm.id} ${vm.name} ${vm.scale} ${vm.created} ${vm.price}
     * </jx:forEach>
     * </pre>
     * </li>
     * <li>有动态表头
     * <pre>
     * 项目/数量/时间    <jx:forEach items="${dates}" var="date">    ${date} </jx:forEach>
     * <jx:forEach items="${itemsx}" var="item">            
     * ${item.name}    <jx:forEach items="${item.counts}" var="count"> ${count}    </jx:forEach>
     * </jx:forEach>           
     * </pre>
     * </li>
     * </ul>
     * 调用该方法则生成对应的Excel文件。
     * </p>
     * <p>
     * 注意：dataKey不能是items, items是保留字，如果用items则会提示：Collection is null并抛出NullPointerException
     * </p>
     * @param destOutputStream Excel输出流
     * @param templateInputStream Excel模版输入流
     * @param header 动态表头
     * @param headerKey 表头的变量
     * @param data 数据项
     * @param dataKey 数据项变量
     * @param maxRowPerSheet 每个sheet最多行数
     * @throws Exception
     */
    @SuppressWarnings("rawtypes")
    public static void generateExcelByTemplate(OutputStream destOutputStream, 
            InputStream templateInputStream, 
            List header, String headerKey,
            List data, String dataKey,
            int maxRowPerSheet) throws Exception {
        List<List> splitData = null;
        @SuppressWarnings("unchecked")
        Map<String, List> beanMap = new HashMap();
        List<String> sheetNames = new ArrayList<String>();
        if (data.size() > maxRowPerSheet) {
            splitData = splitList(data, maxRowPerSheet);
            sheetNames = new ArrayList<String>(splitData.size());
            for (int i = 0; i < splitData.size(); ++i) {
                sheetNames.add(DEFAULT_SHEET_NAME  + i);
            }
        } else {
            splitData = new ArrayList<List>();
            sheetNames.add(DEFAULT_SHEET_NAME + 0);
            splitData.add(data);
        }
        if (null != header) {
            beanMap.put(headerKey, header);
        }
        XLSTransformer transformer = new XLSTransformer();
        Workbook workbook = transformer.transformMultipleSheetsList(
                templateInputStream, splitData, sheetNames, dataKey, beanMap, 0);
        workbook.write(destOutputStream);
    }
    /**
     * 导出无动态表头的Excel文件，目标文件和模版文件均为文件路径
     * <p>
     * 参考重载的有动态表头注释
     * </p>
     * @param destFilePath
     * @param templateFilePath
     * @param data
     * @param dataKey
     * @param maxRowPerSheet
     * @throws Exception
     */
    @SuppressWarnings("rawtypes")
    public static void generateExcelByTemplate(String destFilePath, 
            String templateFilePath, 
            List data, String dataKey, int maxRowPerSheet) throws Exception {
        generateExcelByTemplate(destFilePath, templateFilePath, null, null, data, dataKey, maxRowPerSheet);
    }
    /**
     * 导出有动态表头的Excel文件，目标文件和模版文件均为文件路径
     * <p>
     * 参考重载的有动态表头注释
     * </p>
     * @param destFilePath
     * @param templateFilePath
     * @param header
     * @param headerKey
     * @param data
     * @param dataKey
     * @param maxRowPerSheet
     * @throws Exception
     */
    @SuppressWarnings("rawtypes")
    public static void generateExcelByTemplate(String destFilePath, 
            String templateFilePath, 
            List header, String headerKey,
            List data, String dataKey, int maxRowPerSheet) throws Exception {
        generateExcelByTemplate(new FileOutputStream(destFilePath),
                new FileInputStream(templateFilePath),
                header, headerKey,
                data, dataKey, maxRowPerSheet);
    }
    @SuppressWarnings({ "rawtypes", "unchecked" })
    private static List<List> splitList(List data, int maxRowPerSheet) {
        List<List> splitData = new ArrayList<List>();
        List sdata = null;
        for (int i = 0; i < data.size(); ++i) {
            if (0 == i % maxRowPerSheet) {
                if (null != sdata) {
                    splitData.add(sdata);
                }
                sdata = new ArrayList(maxRowPerSheet);
            }
            sdata.add(data.get(i));
        }
        if (0 != maxRowPerSheet % data.size()) {
            splitData.add(sdata);
        }
        return splitData;
    }
}
```
简单用法：
Java代码 ![复制代码](http://jamie-wang.iteye.com/images/icon_copy.gif)![收藏代码](http://jamie-wang.iteye.com/images/icon_star.png)![](http://jamie-wang.iteye.com/images/spinner.gif)
-         List<VM> vms = new ArrayList<VM>();  
- for (int i = 0; i < 21; ++i) {  
-             VM vma = new VM();  
-             vma.setId(i);  
-             vma.setName("我的CENTOS" + i);  
-             vma.setPrice(103);  
-             vma.setScale("2CPU, 2G MEM, 2T DISK");  
-             vma.setCreated(new Date());  
-             vms.add(vma);  
-         }   
- ExcelUtil.generateExcelByTemplate("E:\\tmp\\ex-sample.xlsx", "E:\\tmp\\template-simple.xlsx", vms, "vms", 10);  
```java
List<VM> vms = new ArrayList<VM>();
        for (int i = 0; i < 21; ++i) {
            VM vma = new VM();
            vma.setId(i);
            vma.setName("我的CENTOS" + i);
            vma.setPrice(103);
            vma.setScale("2CPU, 2G MEM, 2T DISK");
            vma.setCreated(new Date());
            vms.add(vma);
        } 
ExcelUtil.generateExcelByTemplate("E:\\tmp\\ex-sample.xlsx", "E:\\tmp\\template-simple.xlsx", vms, "vms", 10);
```
 运行结果：
![](http://dl2.iteye.com/upload/attachment/0089/7359/2a1a21fa-85f6-34b9-a999-e9714b3b30cc.png)
 可见有3个sheet生成。
接口中的动态表头是指表头是数据项中的字段，例如：日期，项目等。
例如下面这个模版：
![](http://dl2.iteye.com/upload/attachment/0089/7357/6e22a5eb-002c-3644-b4fe-7e753218d897.png)
 调用示例：
Java代码 ![复制代码](http://jamie-wang.iteye.com/images/icon_copy.gif)![收藏代码](http://jamie-wang.iteye.com/images/icon_star.png)![](http://jamie-wang.iteye.com/images/spinner.gif)
-  List<String> dates = new ArrayList<String>();  
- int maxDates = 8;  
- for (int i = 0; i < maxDates ; ++i) {  
-     dates.add("2013-08-1" + i);  
- }  
- List<ItemCount> itemCounts = new ArrayList<ItemCount>();  
- for (int i = 0; i < 82; ++i) {  
-     ItemCount ic = new ItemCount();  
-     List<Integer> counts = new ArrayList<Integer>();  
- for (int j = 0; j < maxDates; ++j) {  
-         counts.add(j);  
-     }  
-     ic.setCounts(counts);  
-     ic.setName("item" + i);  
-     itemCounts.add(ic);  
- }  
- ExcelUtil.generateExcelByTemplate("E:\\tmp\\ex-cpx.xlsx",   
- "E:\\tmp\\template-matrix.xlsx", dates, "dates", itemCounts, "itemsx", 12);  
```java
List<String> dates = new ArrayList<String>();
        int maxDates = 8;
        for (int i = 0; i < maxDates ; ++i) {
            dates.add("2013-08-1" + i);
        }
        List<ItemCount> itemCounts = new ArrayList<ItemCount>();
        for (int i = 0; i < 82; ++i) {
            ItemCount ic = new ItemCount();
            List<Integer> counts = new ArrayList<Integer>();
            for (int j = 0; j < maxDates; ++j) {
                counts.add(j);
            }
            ic.setCounts(counts);
            ic.setName("item" + i);
            itemCounts.add(ic);
        }
        ExcelUtil.generateExcelByTemplate("E:\\tmp\\ex-cpx.xlsx", 
                "E:\\tmp\\template-matrix.xlsx", dates, "dates", itemCounts, "itemsx", 12);
```
 结果：
![](http://dl2.iteye.com/upload/attachment/0089/7369/039a29f2-8fe5-31ab-8e88-22a258d23b58.png)
