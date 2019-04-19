# java使用jxl与poi操作excel文件 - 零度的博客专栏 - CSDN博客
2016年04月20日 14:29:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：469
个人分类：[POI/JXLS/JXL																[Java](https://blog.csdn.net/zmx729618/article/category/3108443)](https://blog.csdn.net/zmx729618/article/category/6193394)
```
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import jxl.Sheet;
import jxl.Workbook;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
public class Test {
 /**
  * @param path
  */
 public static void readExcelByJXL(String path) {
  Workbook book = null;
  InputStream input = null;
  try {
   input = new FileInputStream(path);
   book = Workbook.getWorkbook(input);
   Sheet sheet = book.getSheet(0);
   int columns = sheet.getColumns();
   for (int i = 0; i < sheet.getRows(); i++) {
    for (int j = 0; j < columns; j++)
     System.out.print(sheet.getCell(j, i).getContents()
       + "\t\t\t\t");
    System.out.println();
   }
  } catch (Exception ex) {
   ex.printStackTrace();
  } finally {
   if (book != null)
    book.close();
   if (input != null)
    try {
     input.close();
    } catch (IOException e) {
     e.printStackTrace();
    }
  }
 }
 /**
  * @param path
  */
 public static void readExcelByPOI(String path) {
  HSSFWorkbook book = null;
  InputStream input = null;
  try {
   input = new FileInputStream(path);
   book = new HSSFWorkbook(input);
   HSSFSheet sheet = book.getSheetAt(0);
   int rows = sheet.getPhysicalNumberOfRows();
   int cells = 0;
   HSSFRow row = null;
   HSSFCell cell = null;
   // for (Iterator iterator = sheet.rowIterator();
   // iterator.hasNext();) {
   // row = (HSSFRow) iterator.next();
   for (int i = 0; i < rows; i++) {
    row = sheet.getRow(i);
    cells = row.getPhysicalNumberOfCells();
    // for (Iterator iterator2 = row.cellIterator(); iterator2
    // .hasNext();) {
    // cell = (HSSFCell) iterator2.next();
    for (short j = 0; j < cells; j++) {
     cell = row.getCell(j);
     switch (cell.getCellType()) {
     case HSSFCell.CELL_TYPE_BLANK:
      System.out.print("");
      break;
     case HSSFCell.CELL_TYPE_BOOLEAN:
      System.out.print(cell.getBooleanCellValue());
      break;
     case HSSFCell.CELL_TYPE_ERROR:
      System.out.print(cell.getErrorCellValue());
      break;
     case HSSFCell.CELL_TYPE_FORMULA:
      System.out.print(cell.getCellFormula());
      break;
     case HSSFCell.CELL_TYPE_NUMERIC:
      System.out.print(cell.getNumericCellValue());
      break;
     case HSSFCell.CELL_TYPE_STRING:
      System.out.print(cell.getStringCellValue());
      break;
     }
     System.out.print("\t\t\t\t");
    }
    System.out.println();
   }
  } catch (Exception ex) {
   ex.printStackTrace();
  } finally {
   if (input != null)
    try {
     input.close();
    } catch (IOException e) {
     e.printStackTrace();
    }
  }
 }
 /**
  * @param args
  */
 public static void main(String[] args) {
//测试JXL比POI速度快
  long start = System.currentTimeMillis();
  readExcelByJXL("c:/questions.xls");
  System.out.println("===================AAAAAAAAAAAAAAAa============");
  System.out.println(System.currentTimeMillis() - start);
 }
}
```
