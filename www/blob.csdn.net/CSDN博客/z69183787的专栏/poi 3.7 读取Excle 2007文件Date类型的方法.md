# poi 3.7 读取Excle 2007文件Date类型的方法 - z69183787的专栏 - CSDN博客
2014年12月06日 22:56:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7018
这是个老问题了, 看网上的解决方法都类似于"HSSFDataFormat是HSSF子项目里面定义的一个类。类HSSFDataFormat允许用户新建数据格式类型。HSSFDataFormat类包含静态方法static java.lang.String getBuiltinFormat(short index)，它可以根据编号返回内置数据类型。(摘自[地址](http://javacrazyer.javaeye.com/blog/894758))",代码表示为:
"
**[java]**[view plain](http://blog.csdn.net/maoqb/article/details/6259943#)[copy](http://blog.csdn.net/maoqb/article/details/6259943#)
- case HSSFCell.CELL_TYPE_NUMERIC:      
- if (HSSFDateUtil.isCellDateFormatted(cell)) {      
- double d = cell.getNumericCellValue();      
-         Date date = HSSFDateUtil.getJavaDate(d);      
-     }      
- case HSSFCell.CELL_TYPE_NUMERIC:   
- if (HSSFDateUtil.isCellDateFormatted(cell)) {   
- double d = cell.getNumericCellValue();   
-         Date date = HSSFDateUtil.getJavaDate(d);   
-     }   
"
但是这个方法在读取xlsx格式的文件是不能直接用的,解决方法是找出HSSFDateUtil的源文件,将原代码的类改下就OK了.
附一段我的代码:
**[java]**[view plain](http://blog.csdn.net/maoqb/article/details/6259943#)[copy](http://blog.csdn.net/maoqb/article/details/6259943#)
- switch(c.getCellType()) {  
- case XSSFCell.CELL_TYPE_NUMERIC:      
- if (XSSFDateUtil.isCellDateFormatted((XSSFCell) c)) {      
- double d = c.getNumericCellValue();      
-                         Date date = XSSFDateUtil.getJavaDate(d);    
-                         tdList.add(DateUtil.convertDateToString("yyyy-MM-dd HH:mm:ss", date));  
-                     } else {  
-                         tdList.add(String.valueOf((long)c.getNumericCellValue()));   
-                     }  
- break;  
- default :  
-                     tdList.add(c.toString().trim());  
-                 }  
**[java]**[view plain](http://blog.csdn.net/maoqb/article/details/6259943#)[copy](http://blog.csdn.net/maoqb/article/details/6259943#)
- publicclass XSSFDateUtil extends DateUtil {  
- protectedstaticint absoluteDay(Calendar cal, boolean use1904windowing) {  
- return DateUtil.absoluteDay(cal, use1904windowing);  
-     }  
- }  
