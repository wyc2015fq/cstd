# Apache POI （JAVA处理Office文档的类库） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月08日 23:34:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：22
个人分类：[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)










Apache POI是Apache软件基金会的开放源码函式库，POI提供API给Java程序对Microsoft Office格式档案读和写的功能。
 结构：

> HSSF － 提供读写Microsoft Excel格式档案的功能。
 XSSF － 提供读写Microsoft Excel OOXML格式档案的功能。
 HWPF － 提供读写Microsoft Word格式档案的功能。
 HSLF － 提供读写Microsoft PowerPoint格式档案的功能。
 HDGF － 提供读写Microsoft Visio格式档案的功能。

**创建Excel 文档**
 示例1将演示如何利用Jakarta POI API 创建Excel 文档。
 示例1程序如下：



```cpp
public class CreateXL {
/** Excel 文件要存放的位置，假定在D盘下*/
public static String outputFile="D:\\test.xls";
public static void main(String argv[]){
try{
// 创建新的Excel 工作簿
HSSFWorkbook workbook = new HSSFWorkbook();
// 在Excel工作簿中建一工作表，其名为缺省值
// 如要新建一名为"效益指标"的工作表，其语句为：
// HSSFSheet sheet = workbook.createSheet("效益指标");
HSSFSheet sheet = workbook.createSheet();
// 在索引0的位置创建行（最顶端的行）
HSSFRow row = sheet.createRow((short)0);
//在索引0的位置创建单元格（左上端）
HSSFCell cell = row.createCell((short) 0);
// 定义单元格为字符串类型
cell.setCellType(HSSFCell.CELL_TYPE_STRING);
// 在单元格中输入一些内容
cell.setCellValue("增加值");
// 新建一输出文件流
FileOutputStream fOut = new FileOutputStream(outputFile);
// 把相应的Excel 工作簿存盘
workbook.write(fOut);
fOut.flush();
// 操作结束，关闭文件
fOut.close();
System.out.println("文件生成...");
}catch(Exception e) {
System.out.println("已运行 xlCreate() : " + e );
}
}
```


**读取Excel文档中的数据**
 示例2将演示如何读取Excel文档中的数据。假定在D盘JTest目录下有一个文件名为test1.xls的Excel文件。
 示例2程序如下：





```java
public class ReadXL {
/** Excel文件的存放位置。注意是正斜线*/
public static String fileToBeRead="D:\\test1.xls";
public static void main(String argv[]){
try{
// 创建对Excel工作簿文件的引用
HSSFWorkbook workbook = new HSSFWorkbook(new FileInputStream(fileToBeRead));
// 创建对工作表的引用。
// 本例是按名引用（让我们假定那张表有着缺省名"Sheet1"）
HSSFSheet sheet = workbook.getSheet("Sheet1");
// 也可用getSheetAt(int index)按索引引用，
// 在Excel文档中，第一张工作表的缺省索引是0，
// 其语句为：HSSFSheet sheet = workbook.getSheetAt(0);
// 读取左上端单元
HSSFRow row = sheet.getRow(0);
HSSFCell cell = row.getCell((short)0);
// 输出单元内容，cell.getStringCellValue()就是取所在单元的值
System.out.println("左上端单元是： " + cell.getStringCellValue());
}catch(Exception e) {
System.out.println("已运行xlRead() : " + e );
}
}
```


 详细使用方法见《Busy Developers' Guide to HSSF and XSSF Features》[http://poi.apache.org/spreadsheet/quick-guide.html](http://poi.apache.org/spreadsheet/quick-guide.html)



**处理WORD文档**



```cpp
public class TestPoi {
public TestPoi() {
}
public static void main(String args[]) throws Exception
{
FileInputStream in = new FileInputStream ("D:\\a.doc");
WordExtractor extractor = new WordExtractor();
String str = extractor.extractText(in);
//System.out.println("the result length is"+str.length());
System.out.println(str);
}
}
```










