# java使用poi解析或处理excel的时候，如何防止数字变成科学计数法的形式 - z69183787的专栏 - CSDN博客
2015年08月24日 15:41:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4697
个人分类：[Excel处理-Jxl & Poi](https://blog.csdn.net/z69183787/article/category/2175693)
当使用POI处理excel的时候，遇到了比较长的数字，虽然excel里面设置该单元格是文本类型的，但是POI的cell的类型就会变成数字类型。 
而且无论数字是否小数，使用cell.getNumbericCellValue() 去获取值的时候，会得到一个double，而且当长度大一点的时候会变成科学计数法形式。 
那么获取这个单元格的原始的数据，就其实是一个double怎么转换成整数的问题了。 
使用DecimalFormat对这个double进行了格式话，随后使用format方法获得的String就是你想要的值了。 
Java代码  ![收藏代码](http://liuqing9382.iteye.com/images/icon_star.png)
- DecimalFormat df = new DecimalFormat("0");  
- 
- String whatYourWant = df.format(cell.getNumericCellValue());  
- 
Example：
![](https://img-blog.csdn.net/20170724174230043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```java
public static void main(String[] args) throws IOException {
        File excelFile = new File(path);
       FileInputStream is = new FileInputStream(excelFile);// 获取文件输入流
      XSSFWorkbook workbook2007 = new XSSFWorkbook(is);
       XSSFCell cell = workbook2007.getSheetAt(0).getRow(1).getCell(2);
       // cell.setCellType(XSSFCell.CELL_TYPE_STRING);
        DecimalFormat df = new DecimalFormat("0");
        String a =   String.valueOf(df.format(cell.getNumericCellValue()));
        System.out.println(a);
//
//        write();
    }
```
![](https://img-blog.csdn.net/20170724174309563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
