# Jxl读/写Excel实例(设置Cell Format格式) - 零度的博客专栏 - CSDN博客
2017年05月25日 11:28:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：3766
                
本篇文章主要讲的是jxl 操作Excel的三个简单的实例，一个是读取Excel表格数据，一个是 写一个新的Excel表格，最后是更新现有的Excel表格。在开始全面学习 jxl 之前，我们先看几个例子，让大家大概整体的了解一下jxl 是怎么操作Excel表的，以使之后更有兴趣，更好地学习jxl。
        读Excel表格数据：
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49478763#)[copy](http://blog.csdn.net/u010657094/article/details/49478763#)
- import java.io.File;  
- import java.io.IOException;  
- import jxl.Cell;  
- import jxl.Sheet;  
- import jxl.Workbook;  
- import jxl.read.biff.BiffException;  
- 
- /**
-  *
-  * @author yrs
-  */
- publicclass TRead {  
- 
- publicstaticvoid read (Workbook book) {  
- int rows;  
- //得到Excel中工作表数
- int sheetNumber = book.getNumberOfSheets();  
- //得到每个表的名称
-         String [] sheetNameList = book.getSheetNames();  
- //得到每个工作表
-         Sheet [] sheetList = book.getSheets();  
- 
- //遍历每个单元格，得到单元格的内容，并输出。注意得到单元格的内容是以字符串形式返回的。
- for(int i = 0;i < sheetNumber;i++) {  
-             System.out.println("############## " + sheetNameList[i] + " ##############");  
- //得到该工作表中数据的行数
-             rows = sheetList[i].getRows();  
- for(int j = 0;j < rows;j++) {  
- //得到每一行的单元格
-                 Cell [] cellList = sheetList[i].getRow(j);  
- for (Cell cell : cellList) {  
-                     System.out.print(cell.getContents() + "  ");  
-                 }  
-                 System.out.println();  
-             }            
-         }        
-     }  
- publicstaticvoid main(String [] args) throws IOException, BiffException  {  
- try {  
- //文件D://JEtest//read.xls 要存在，并且含有数据。
-             File file = new File("D://JEtest//read.xls");  
- //得到工作簿
-             Workbook book = Workbook.getWorkbook(file);   
-             TRead.read(book);  
-             book.close();              
-         }catch (IOException | BiffException e) {  
-             System.out.println("Exception:  " + e);  
- throw e;  
-         }  
-     }    
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://static.blog.csdn.net/images/save_snippets.png)
        读Excel表格的步奏是：工作簿--》工作表--》单元格。 当然有时候对数据经行处理要知道数据的格式，这个要在取得单元格的时候对它进行数据类型的判断，然后再转化成相应的类型，从而得到原数据类型的值。关于数据类型会在之后的文章中介绍。
        生成一个新的Excel表格，并向里面写数据：
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49478763#)[copy](http://blog.csdn.net/u010657094/article/details/49478763#)
- import java.io.File;  
- import java.io.IOException;  
- import jxl.Workbook;  
- import jxl.read.biff.BiffException;  
- import jxl.write.Label;  
- import jxl.write.Number;  
- import jxl.write.Boolean;  
- import jxl.write.DateFormat;  
- import jxl.write.DateTime;  
- import jxl.write.NumberFormat;  
- import jxl.write.WritableCellFormat;  
- import jxl.write.WritableFont;  
- import jxl.write.WritableSheet;  
- import jxl.write.WritableWorkbook;  
- import jxl.write.WriteException;  
- 
- 
- /**
-  *
-  * @author yrs
-  */
- publicclass TWrite {  
- 
- publicstaticvoid  write(WritableWorkbook wb) throws WriteException {  
-         WritableSheet wSheet = wb.createSheet("sheet1", 0);  
- 
- //添加Label对象 
-         Label labelC = new Label(0, 0, "Label cell");   
-         wSheet.addCell(labelC);  
- 
- //添加Number对象 
-         Number numberC = new Number(0, 1, 3.1415926);   
-         wSheet.addCell(numberC);   
- 
- // 3.添加Boolean对象 
-         Boolean booleanC = new Boolean(0, 2, true);   
-         wSheet.addCell(booleanC);   
- 
- // 4.添加DateTime对象 
-         DateTime dateTimeC = new DateTime(0, 3, new java.util.Date());   
-         wSheet.addCell(dateTimeC);   
- 
- // 添加带有字型Formatting的对象 
-         WritableFont wFont = new WritableFont(   
-             WritableFont.TIMES, 18, WritableFont.BOLD, true);   
-         WritableCellFormat wCellFormatC = new WritableCellFormat(wFont);   
-         Label labelCF = new Label(1, 0, "Label Cell", wCellFormatC);   
-         wSheet.addCell(labelCF);   
- 
- // 添加带有formatting的Number对象 
-         NumberFormat nFormat = new NumberFormat("#.##");   
-         WritableCellFormat wCellFormatN = new WritableCellFormat(nFormat);  
-         Number numberNF = new Number(1, 1, 3.1415926, wCellFormatN);   
-         wSheet.addCell(numberNF);   
- 
- // 添加带有formatting的DateFormat对象 
-         DateFormat dFormat = new DateFormat(   
- "yyyy MM dd hh:mm:ss");   
-         WritableCellFormat wCellFormatD = new WritableCellFormat(dFormat);   
-         DateTime dateTimeDF = new DateTime(1, 3, new java.util.Date(), wCellFormatD);   
-         wSheet.addCell(dateTimeDF);  
- 
-     }  
- publicstaticvoid main(String [] args) throws IOException, BiffException, WriteException  {  
- try {  
- 
- //文件D://JEtest//read.xls 不存在，若存在数据将会丢失
-             File file = new File("D://JEtest//write.xls");  
- //创建一个新的可写工作簿
-             WritableWorkbook wb = Workbook.createWorkbook(file);   
-             TWrite.write(wb);  
- 
- // 写入Exel工作表 
-             wb.write();  
- // 关闭Excel工作薄对象 
-             wb.close();              
-         }catch (IOException | WriteException e) {  
-             System.out.println("Exception:  " + e);  
- throw e;  
-         }  
-     }    
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
![](http://static.blog.csdn.net/images/save_snippets.png)
        生成一个新的Excel表格，并向里面写入数据的顺序也是 工作簿 -- 工作表 -- 单元格。但是写入数据的顺序是反过来的，先向单元格中添加数据，然后添加到工作表上，最后写入工作簿中。这里需要注意的是，以 WritableWorkbook wb = Workbook.createWorkbook(file);  方式创建工作簿是创建了一个新的工作簿，如果file文件原来存在，那么它的数据将会失去，下一个例子将会讲解怎样更新一个Excel表格。
 还有就是在工作簿关闭之前要把数据写入工作簿中，也就是 wb.write();。
        更新Excel表格数据：
**[java]**[view
 plain](http://blog.csdn.net/u010657094/article/details/49478763#)[copy](http://blog.csdn.net/u010657094/article/details/49478763#)
- import java.io.File;  
- import java.io.IOException;  
- import jxl.Workbook;  
- import jxl.read.biff.BiffException;  
- import jxl.write.Label;  
- import jxl.write.WritableSheet;  
- import jxl.write.WritableWorkbook;  
- import jxl.write.WriteException;  
- 
- /**
-  *
-  * @author yrs
-  */
- publicclass TUpdate {  
- 
- publicstaticvoid update (WritableWorkbook wb) throws WriteException {  
-         WritableSheet sheet = wb.createSheet("第二页", 1);  
-         sheet.addCell(new Label(0,0,"第二页更新数据"));  
- 
-     }  
- 
- publicstaticvoid main(String [] args) throws IOException, BiffException, WriteException  {  
- try {  
- //文件D://JEtest//read.xls 要存在，并且含有数据。
-             File file = new File("D://JEtest//update.xls");  
- //得到工作簿
-             Workbook book = Workbook.getWorkbook(file);   
- //创建一个可写工作簿
-             WritableWorkbook wb = Workbook.createWorkbook(file, book);  
-             TUpdate.update(wb);  
-             wb.write();  
-             wb.close();  
-             book.close();              
-         }catch (IOException | BiffException | WriteException e) {  
-             System.out.println("Exception:  " + e);  
- throw e;  
-         }  
-     }    
- }  
