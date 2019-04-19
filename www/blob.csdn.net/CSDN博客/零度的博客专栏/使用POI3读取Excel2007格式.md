# 使用POI3读取Excel2007格式 - 零度的博客专栏 - CSDN博客
2016年05月05日 16:02:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：1877
相关jar包，自行去poi官网下载，其中包含了对于date类型的处理，其中该列必须在excel中设置为日期类型。
解析date 工具类：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/42457295#)[copy](http://blog.csdn.net/z69183787/article/details/42457295#)[print](http://blog.csdn.net/z69183787/article/details/42457295#)[?](http://blog.csdn.net/z69183787/article/details/42457295#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/585605/fork)
- package com.wonders.task.excel.util;  
- 
- import org.apache.poi.ss.usermodel.DateUtil;  
- 
- import java.util.Calendar;  
- 
- /**
-  * Created with IntelliJ IDEA.
-  * User: zhoushun
-  * Date: 2014/12/7
-  * Time: 0:36
-  * To change this template use File | Settings | File Templates.
-  */
- publicclass XSSFDateUtil extends DateUtil {  
- protectedstaticint absoluteDay(Calendar cal, boolean use1904windowing) {  
- return DateUtil.absoluteDay(cal, use1904windowing);  
-     }  
- }  
```java
package com.wonders.task.excel.util;
import org.apache.poi.ss.usermodel.DateUtil;
import java.util.Calendar;
/**
 * Created with IntelliJ IDEA.
 * User: zhoushun
 * Date: 2014/12/7
 * Time: 0:36
 * To change this template use File | Settings | File Templates.
 */
public class XSSFDateUtil extends DateUtil {
    protected static int absoluteDay(Calendar cal, boolean use1904windowing) {
        return DateUtil.absoluteDay(cal, use1904windowing);
    }
}
```

**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/42457295#)[copy](http://blog.csdn.net/z69183787/article/details/42457295#)[print](http://blog.csdn.net/z69183787/article/details/42457295#)[?](http://blog.csdn.net/z69183787/article/details/42457295#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/585605/fork)
- package com.wonders.task.excel.util;  
- 
- import com.wonders.task.excel.model.SendBo;  
- import org.apache.poi.xssf.usermodel.XSSFCell;  
- import org.apache.poi.xssf.usermodel.XSSFRow;  
- import org.apache.poi.xssf.usermodel.XSSFSheet;  
- import org.apache.poi.xssf.usermodel.XSSFWorkbook;  
- 
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.text.SimpleDateFormat;  
- import java.util.ArrayList;  
- import java.util.List;  
- import java.util.regex.Matcher;  
- import java.util.regex.Pattern;  
- 
- /**
-  * Created with IntelliJ IDEA.
-  * User: zhoushun
-  * Date: 2014/12/6
-  * Time: 20:25
-  * To change this template use File | Settings | File Templates.
-  */
- publicclass PoiUtil {  
- privatestatic String excel2007 = "D:\\all.xlsx";  
- 
- privatestaticvoid setCode(SendBo bo){  
-         Pattern pattern = Pattern.compile("(\\S+)\\((\\d+)\\)(\\d+)号");  
- 
-         String sendId = bo.getSendId();  
- if(sendId == null || sendId.length() == 0){  
-             bo.setCode1("");bo.setCode2("");bo.setCode3("");  
-         }else{  
-             Matcher matcher = pattern.matcher(sendId);  
- if(matcher.find()){  
-                 bo.setCode1(matcher.group(1));  
-                 bo.setCode2(matcher.group(2));  
-                 bo.setCode3(matcher.group(3));  
-             }  
-         }  
-     }  
- 
- publicstatic List<SendBo> readExcel2007(){  
-         List<SendBo> list = new ArrayList<SendBo>();  
- try{  
-             File excelFile = new File(excel2007);  
-             FileInputStream is = new FileInputStream(excelFile);// 获取文件输入流
-             XSSFWorkbook workbook2007 = new XSSFWorkbook(is);// 创建Excel2003文件对象
-             XSSFSheet sheet = workbook2007.getSheetAt(0);// 取出第一个工作表，索引是0
- // 开始循环遍历行，表头不处理，从1开始
- for (int i = 1; i <= sheet.getLastRowNum(); i++) {  
-                 XSSFRow row = sheet.getRow(i);// 获取行对象
- if (row == null) {// 如果为空，不处理
- continue;  
-                 }  
-                 SendBo bo = new SendBo();  
- // 循环遍历单元格
- for (int j = 0; j < row.getLastCellNum(); j++) {  
- 
-                     XSSFCell cell = row.getCell(j);// 获取单元格对象
- switch(j){  
- case0 :  
-                             bo.setSendDept(getStringCellValue(cell).trim());  
- break;  
- case1 :  
-                             bo.setSendId(getStringCellValue(cell).trim());  
- break;  
- case2 :  
-                             bo.setSendDate(getStringCellValue(cell).trim());  
-                             setCode(bo);  
- break;  
- case3 :  
-                             bo.setTitle(getStringCellValue(cell).trim());  
- break;  
- case4 :  
-                             bo.setSendMain(getStringCellValue(cell).trim());  
- break;  
- case5 :  
- break;  
- case6 :  
- break;  
- default:  
- break;  
-                     }  
- 
-                 }  
-                 list.add(bo);  
-             }  
- 
- 
-         }catch(Exception e){}  
- 
- 
- 
- return list;  
-     }  
- 
- 
- /**
-      * 获取单元格数据内容为字符串类型的数据
-      *
-      * @param cell Excel单元格
-      * @return String 单元格数据内容
-      */
- privatestatic String getStringCellValue(XSSFCell cell) {  
-         String strCell = "";  
- switch (cell.getCellType()) {  
- case XSSFCell.CELL_TYPE_STRING:  
-                 strCell = cell.getStringCellValue();  
- break;  
- case XSSFCell.CELL_TYPE_NUMERIC:  
- if (XSSFDateUtil.isCellDateFormatted(cell)) {  
- //  如果是date类型则 ，获取该cell的date值
-                     strCell = new SimpleDateFormat("yyyy-MM-dd").format(XSSFDateUtil.getJavaDate(cell.getNumericCellValue()));  
-                 } else { // 纯数字
-                     strCell = String.valueOf(cell.getNumericCellValue());  
-                 }  
- break;  
- case XSSFCell.CELL_TYPE_BOOLEAN:  
-                 strCell = String.valueOf(cell.getBooleanCellValue());  
- break;  
- case XSSFCell.CELL_TYPE_BLANK:  
-                 strCell = "";  
- break;  
- default:  
-                 strCell = "";  
- break;  
-         }  
- if (strCell.equals("") || strCell == null) {  
- return"";  
-         }  
- if (cell == null) {  
- return"";  
-         }  
- return strCell;  
-     }  
- 
- 
- publicstaticvoid main(String[] args){  
-         String s = "沪地铁(2014)22号";  
-         Pattern pattern = Pattern.compile("(\\S+)\\((\\d+)\\)(\\d+)号");  
-         Matcher matcher = pattern.matcher(s);  
- if(matcher.find()){  
- int gc = matcher.groupCount();  
- for(int i = 0; i <= gc; i++)  
-                 System.out.println("group " + i + " :" + matcher.group(i));  
-         }  
- //readExcel2007();
-     }  
- }  
