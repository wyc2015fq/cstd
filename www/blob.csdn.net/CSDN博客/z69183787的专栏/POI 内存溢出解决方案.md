# POI 内存溢出解决方案 - z69183787的专栏 - CSDN博客
2017年04月25日 11:52:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1538
POI之前的版本不支持大数据量处理，如果数据过多则经常报OOM错误，有时候调整JVM大小效果也不是太好。3.8版本的POI新出来了SXSSFWorkbook,可以支持大数据量的操作，只是SXSSFWorkbook只支持.xlsx格式，不支持.xls格式。
3.8版本的POI对excel的导出操作，一般只使用HSSFWorkbook以及SXSSFWorkbook，HSSFWorkbook用来处理较少的数据量，SXSSFWorkbook用来处理大数据量以及超大数据量的导出。
HSSFWorkbook的使用方法和之前的版本的使用方法一致，这里就不在陈述使用方法了
SXSSFWorkbook的使用例子如下：
```java
import junit.framework.Assert;
import org.apache.poi.ss.usermodel.Cell;  
import org.apache.poi.ss.usermodel.Row;  
import org.apache.poi.ss.usermodel.Sheet;  
import org.apache.poi.ss.usermodel.Workbook;  
import org.apache.poi.ss.util.CellReference;  
import org.apache.poi.xssf.streaming.SXSSFWorkbook;   
public static void main(String[] args) throws Throwable {  
 Workbook wb = new SXSSFWorkbook(100); // keep 100 rows in memory, exceeding rows will be flushed to disk  
 Sheet sh = wb.createSheet();  
 for(int rownum = 0; rownum < 100000; rownum++){ 
 Row row = sh.createRow(rownum);  
 for(int cellnum = 0; cellnum < 10; cellnum++){  
 Cell cell = row.createCell(cellnum);  
 String address = new CellReference(cell).formatAsString();  
 cell.setCellValue(address); }  
 } 
FileOutputStream out = new FileOutputStream("/temp/sxssf.xlsx");  
 wb.write(out);  
 out.close(); 
}
```
以上是转载的**值得注意的是SXSSFWorkbook只能写不能读。**但是往往我们需要向一个Excel模版里导出数据，这样才更好提前定义里面的格式和vba代码。这里就需要使用SXSSFWorkbook的另外一个构造函数：[**SXSSFWorkbook**](http://poi.apache.org/apidocs/org/apache/poi/xssf/streaming/SXSSFWorkbook.html#SXSSFWorkbook(org.apache.poi.xssf.usermodel.XSSFWorkbook))([XSSFWorkbook](http://poi.apache.org/apidocs/org/apache/poi/xssf/usermodel/XSSFWorkbook.html) workbook)
Construct a workbook from a template.通过[XSSFWorkbook](http://poi.apache.org/apidocs/org/apache/poi/xssf/usermodel/XSSFWorkbook.html)来读取模版，然后用SXSSFWorkbook来设置样式和写数据，详细使用就参考API吧。[http://poi.apache.org/apidocs/org/apache/poi/xssf/streaming/SXSSFWorkbook.html](http://poi.apache.org/apidocs/org/apache/poi/xssf/streaming/SXSSFWorkbook.html)
