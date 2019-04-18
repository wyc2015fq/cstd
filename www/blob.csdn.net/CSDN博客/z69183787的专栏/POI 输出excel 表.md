# POI 输出excel 表 - z69183787的专栏 - CSDN博客
2013年03月01日 14:21:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：697
个人分类：[Excel处理-Jxl & Poi](https://blog.csdn.net/z69183787/article/category/2175693)
```java
```
// Decompiled by DJ v3.7.7.81 Copyright 2004 Atanas Neshkov  Date: 2013/3/1 14:21:24
// Home Page : http://members.fortunecity.com/neshkov/dj.html  - Check often for new version!
// Decompiler options: packimports(3) 
// Source File Name:   ExportExcel.java
package com.wonders.ic.export;
import java.io.IOException;
import java.io.OutputStream;
import java.util.List;
import org.apache.poi.hssf.usermodel.*;
public class ExportExcel
{
    public ExportExcel()
    {
    }
//参数为 1：文件名 2：第一行列名 3：具体数据list<Object[]> 格式  3 4 输出流 5 单元格宽度
    public void exportExcel(String title, String headers[], List dataset, OutputStream out, short width[])
    {
        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet(title);
        sheet.setDefaultColumnWidth((short)15);
        HSSFCellStyle style = workbook.createCellStyle();
        style.setFillForegroundColor((short)43);
        style.setFillPattern((short)1);
        style.setBorderBottom((short)1);
        style.setBorderLeft((short)1);
        style.setBorderRight((short)1);
        style.setBorderTop((short)1);
        style.setAlignment((short)2);
        HSSFFont font = workbook.createFont();
        font.setColor((short)20);
        font.setFontHeightInPoints((short)12);
        font.setBoldweight((short)700);
        style.setFont(font);
        HSSFCellStyle style2 = workbook.createCellStyle();
        style2.setFillForegroundColor((short)9);
        style2.setFillPattern((short)1);
        style2.setBorderBottom((short)1);
        style2.setBorderLeft((short)1);
        style2.setBorderRight((short)1);
        style2.setBorderTop((short)1);
        style2.setAlignment((short)2);
        style2.setVerticalAlignment((short)1);
        HSSFFont font2 = workbook.createFont();
        font2.setBoldweight((short)400);
        style2.setFont(font2);
        HSSFRow row = sheet.createRow(0);
        for(short i = 0; i < headers.length; i++)
        {
            HSSFCell cell = row.createCell(i);
            cell.setCellStyle(style);
            HSSFRichTextString text = new HSSFRichTextString(headers[i]);
            cell.setCellValue(text);
        }
        for(int j = 0; j < dataset.size(); j++)
        {
            row = sheet.createRow(j + 1);
            for(short i = 0; i < ((Object[])dataset.get(j)).length; i++)
            {
                HSSFCell cell = row.createCell(i);
                cell.setCellStyle(style2);
                HSSFRichTextString richString = new HSSFRichTextString(String.valueOf(((Object[])dataset.get(j))[i]).replace("null", ""));
                cell.setCellValue(richString);
            }
        }
        if(width != null)
        {
            for(short i = 0; i < width.length; i++)
                sheet.setColumnWidth(i, width[i]);
        }
        try
        {
            workbook.write(out);
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
}
