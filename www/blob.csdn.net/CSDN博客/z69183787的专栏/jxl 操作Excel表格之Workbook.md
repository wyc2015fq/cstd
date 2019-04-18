# jxl 操作Excel表格之Workbook - z69183787的专栏 - CSDN博客
2017年05月05日 11:19:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：571
个人分类：[Excel处理-Jxl & Poi](https://blog.csdn.net/z69183787/article/category/2175693)
核心点：
```java
Workbook wb =null;
            WritableWorkbook wwb=null;
            try{
                wb = Workbook.getWorkbook(file);
                wwb=Workbook.createWorkbook(file,wb);
```
 /**
   * Creates a writable workbook with the given filename as a copy of
   * the workbook passed in.  Once created, the contents of the writable
   * workbook may be modified
   *
   * @param file the output file for the copy
   * @param in the workbook to copy
   * @return a writable workbook
   * @exception IOException
   */
wwb=Workbook.createWorkbook(file,wb); 根据现有workbook获取一份clone，可写
  本篇文章主要讲jxl 中非常重要的一个抽象类Workbook，该类是一切操作Excel表格的开始。可以通过该类得到工作簿，也可以创建一个新的可写工作簿。下面就让我们一起来看看怎样使用该类。
得到已存在Excel表格的工作簿  Workbook.getWorkbook：
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49532429#)[copy](http://blog.csdn.net/u010657094/article/details/49532429#)
- import java.io.*;  
- import jxl.Workbook;  
- import jxl.WorkbookSettings;  
- import jxl.read.biff.BiffException;  
- 
- publicclass Tgetworkbook {  
- publicstaticvoid main(String args[])  {  
- try {  
- //文件已经存在，得到工作簿
-             File file = new File("D://JEtest/测试.xls");  
-             Workbook book1 = Workbook.getWorkbook(file);  
-             System.out.println("sheet's number: " + book1.getNumberOfSheets());  
-             book1.close();  
- 
- //WorkbookSettings 是使应用程序可以使用各种高级工作簿属性设置，若不使用则相关属性会是默认值
-             WorkbookSettings ws = new WorkbookSettings();  
-             Workbook book2 = Workbook.getWorkbook(file,ws);  
-             System.out.println("sheet's number: " + book2.getNumberOfSheets());  
-             book2.close();  
- 
- //以输入流的方式得到工作簿
-             InputStream is = new FileInputStream(file);  
-             Workbook book3 = Workbook.getWorkbook(is);  
-             System.out.println("sheet's number: " + book3.getNumberOfSheets());  
-             book3.close();  
-             is.close();  
- 
- //输入流和WorkbookSettings 相结合
-             InputStream is2 = new FileInputStream(file);  
-             Workbook book4 = Workbook.getWorkbook(is2, ws);  
-             System.out.println("sheet's number: " + book4.getNumberOfSheets());  
-             book4.close();  
-             is2.close();  
- 
- 
-         }catch (IOException | BiffException e) {  
-             System.out.println("Exception: " + e);  
-         }  
-     }  
- 
- }  
        以这种方式得到的工作簿的前题是Excel表格已存在。其中有一个WorkbookSettings 类，该类是设置工作簿相关的高级属性，包括工作簿的字符集、字符编码等属性，有一个方法  public voidsetGCDisabled(boolean disabled)
  在Web相关工作时，可能回影响效率，需要把垃圾回收属性设置为禁止，true 为禁止垃圾回收。有兴趣可以看看API 了解更多设置。
创建可写工作簿 Workbook.createWorkbook :
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49532429#)[copy](http://blog.csdn.net/u010657094/article/details/49532429#)
- import java.io.*;  
- import jxl.*;  
- import jxl.read.biff.BiffException;  
- import jxl.write.*;  
- 
- publicclass TCreateWorkbook2 {  
- publicstaticvoid main(String args[]) throws IOException, WriteException, BiffException {  
- try {  
-             File file = new File("D://JEtest/测试1.xls");  
- //创建一个可写的工作簿
- //若文件 测试1.xls 存在，则以这种方式创建，原文件的数据将会失去。
-             WritableWorkbook wb1 = Workbook.createWorkbook(file);             
-             System.out.println("sheet numbers: " + wb1.getNumberOfSheets());  
-             WritableSheet wsheet1 = wb1.createSheet("sheet1", 0);  
- 
-             System.out.println("sheet numbers: " + wb1.getNumberOfSheets());  
- //注意一定要用write()函数写回去
-             wb1.write();  
-             wb1.close();  
- 
- 
- //文件存在，更新源文件，或者复制源文件到另一个文件中，在进行修改。
- //测试2.xls 文件是存在的。
- //得到的wb2 是对book 的复制
-             File file2 = new File("D://JEtest/测试2.xls");  
- //更新 测试2.xls
-             Workbook book = Workbook.getWorkbook(file2);  
-             WritableWorkbook wb2 = Workbook.createWorkbook(file2, book);  
- 
- //这种方式是把file2 的数据复制到file3里
- //File file3 = new File("D://JEtest/测试3.xls");
- //WritableWorkbook wb3 = Workbook.createWorkbook(file3, book);
- 
-             System.out.println("sheet numbers: " + wb2.getNumberOfSheets());  
- //注意一定要用write()函数写回去
-             wb2.write();        
-             wb2.close();  
- 
- 
- 
- //创建带有WorkbookSettings 高级设置的可写工作簿
-             File file4 = new File("D://JEtest/测试4.xls");  
- 
-             WorkbookSettings ws = new WorkbookSettings();       
-             WritableWorkbook wb4 = Workbook.createWorkbook(file4, ws);  
-             WritableSheet wsheet4 = wb4.createSheet("sheet2", 0);  
-             System.out.println("sheet2 numbers: " + wb4.getNumberOfSheets());  
-             wb4.write();  
-             wb4.close();  
- 
- 
- //public static WritableWorkbook createWorkbook(java.io.File file,Workbook in,WorkbookSettings ws)
-             WritableWorkbook wb5 = Workbook.createWorkbook(file2, book, ws);  
-             wb5.write();  
-             wb5.close();          
- 
-         }catch (IOException | WriteException | BiffException e) {  
-             System.out.println("Exception: " + e);  
- throw e;  
-         }  
-     }  
- 
- }  
也可以以输出流的方式创建：
public static [WritableWorkbook](http://blog.csdn.net/u010657094/article/details/49532429)**createWorkbook**(java.io.OutputStream os)
public static [WritableWorkbook](http://blog.csdn.net/u010657094/article/details/49532429)**createWorkbook**(java.io.OutputStream os, [WorkbookSettings](http://blog.csdn.net/u010657094/article/details/49532429) ws)
public static [WritableWorkbook](http://blog.csdn.net/u010657094/article/details/49532429)**createWorkbook**(java.io.OutputStream os, [Workbook](http://blog.csdn.net/u010657094/article/details/49532429) in)
public static [WritableWorkbook](http://blog.csdn.net/u010657094/article/details/49532429)**createWorkbook**(java.io.OutputStream os, [Workbook](http://blog.csdn.net/u010657094/article/details/49532429) in, [WorkbookSettings](http://blog.csdn.net/u010657094/article/details/49532429) ws)
这里需要注意的是以  public static [WritableWorkbook](http://write.blog.csdn.net/postedit/49532429)**createWorkbook**([Java](http://lib.csdn.net/base/javase).io.OutputStream os, [Workbook](http://write.blog.csdn.net/postedit/49532429) in)
   方式创建可写工作簿，在写入时一直报错，目前没有搞明白是什么原因。希望大神能留言告诉我。
Workbook其他的方法：
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49532429#)[copy](http://blog.csdn.net/u010657094/article/details/49532429#)
- import java.io.File;  
- import java.io.IOException;  
- import java.util.Arrays;  
- import jxl.Workbook;  
- import jxl.Sheet;  
- import jxl.Cell;  
- import jxl.Range;  
- import jxl.read.biff.BiffException;  
- 
- publicclass Tworkbook {  
- publicstaticvoid main(String args[]) throws IOException, BiffException {  
- try {  
- //文件 测试.xls 存在
-             File file = new File("D://JEtest/测试.xls");  
-             Workbook wb = Workbook.getWorkbook(file);  
- 
- //判断表是否受到保护
-             System.out.println("Is the sheet protect?   result: " + wb.isProtected());  
- 
- //在Excel表中可以选定多个单元格，给他们起一个名字，这里的range就代表的是一个你选定命名的那些单元格。
- //！！！！！！！！！！！！！！！！
- //注意不要用wps 操作选定一部分单元格给其命名，这样jxl用该函数会出现错误，返回的结果不正确。
- //！！！！！！！！！！！！！！！！
- //该方法返回所有你命名range的名字的字符串数组
-             String [] rangeNameList = wb.getRangeNames();  
-             System.out.println("the range[]: " + Arrays.toString(rangeNameList));  
- 
- 
- //有参数名，得到range[] 数组。    ？？？？range的名字是唯一的，为什么返回是数组呢？？？？
-             Range[]  ranges;  
-             ranges = wb.findByName("name");  
-             System.out.println("the ranges[] length: " + ranges.length);     
- 
- 
- //该方法返回名为"name" 的cell或range 的左上单元格。
-             Cell cell = wb.findCellByName("name");  
-             System.out.println("cell 或 range的左上单元格的内容： " + cell.getContents());  
- 
- 
- //获取指定单元格， 参数格式为"sheet1!A1"
-             Cell cellA1 = wb.getCell("第一页!A3");  
-             System.out.println("第一页里A1的内容： " + cellA1.getContents());  
- 
- 
- //返回工作簿中工作表个数
- int sheetnumber = wb.getNumberOfSheets();  
-             System.out.println("工作表个数为： " + sheetnumber);  
- 
- //得到指定的表，根据索引值，从零开始。  不要做不必要的调用，因为每次调用都要重读工作表， 
- //此外客户端也不要持有不必要的表的引用，这回阻止垃圾回收器释放内存。
-             Sheet sheet0 = wb.getSheet(0);  
-             System.out.println("第一个表的名字是： " + sheet0.getName());  
- 
- //这种方式是根据工作表的名称来得到工作表的。
-             Sheet sheet1 = wb.getSheet("第一页");  
- 
- 
- //得到工作表数组
-             Sheet [] sheetlist;  
-             sheetlist = wb.getSheets();  
-             System.out.println("工作表个数为： " + sheetlist.length);  
- 
- 
- //得到工作簿中所有的工作表名称，以字符串数组存储。
-             String [] sheetnamelist = wb.getSheetNames();  
-             System.out.println("工作表个数：" + sheetnamelist.length + " 工作表名称： " + Arrays.toString(sheetnamelist));  
- 
- 
- //得到jxl版本号
-             String version = wb.getVersion();  
-             System.out.println(version);  
- 
-             wb.close();  
- 
-         }catch (IOException | BiffException | IndexOutOfBoundsException e) {  
-             System.out.println("Exception: " + e);  
- throw e;  
-         }  
-     }  
- 
- }  
- 
