# 大数据导出POI之SXSSFWorkbook - z69183787的专栏 - CSDN博客
2018年06月14日 18:31:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：332
[https://www.jianshu.com/p/df05c7fd6fa8](https://www.jianshu.com/p/df05c7fd6fa8)
POI还有[SXSSFWorkbook](https://link.jianshu.com/?t=http%3A%2F%2Fpoi.apache.org%2Fapidocs%2Forg%2Fapache%2Fpoi%2Fxssf%2Fstreaming%2FSXSSFWorkbook.html)专门处理大数据
```java
OutputStream os = null;  
try {  
    HttpServletResponse response = super.getResponse();  
    response.setContentType("application/force-download"); // 设置下载类型  
    String filename ="risk_event.xlsx";  
    response.setHeader("Content-Disposition","attachment;filename=" + filename); // 设置文件的名称  
    os = response.getOutputStream(); // 输出流  
    SXSSFWorkbook wb = new SXSSFWorkbook(1000);//内存中保留 1000 条数据，以免内存溢出，其余写入 硬盘  
       //获得该工作区的第一个sheet     
    Sheet sheet1 = wb.createSheet("sheet1");   
       int excelRow = 0;  
       //标题行  
       Row titleRow = (Row) sheet1.createRow(excelRow++);  
    for (int i = 0; i < columnList.size(); i++) {  
        Cell cell = titleRow.createCell(i);    
              cell.setCellValue(columnList.get(i));  
    }  
      
    for (int m = 0; m < cycleCount; m++) {  
        List<List<String>> eventStrList = this.convertPageModelStrList();  
        if (eventStrList!= null && eventStrList.size() > 0) {  
            for (int i = 0; i < eventStrList.size(); i++) {  
                //明细行  
                Row contentRow = (Row) sheet1.createRow(excelRow++);  
                List<String> reParam = (List<String>) eventStrList.get(i);  
                for (int j = 0; j < reParam.size(); j++) {  
                    Cell cell = contentRow.createCell(j);    
                    cell.setCellValue(reParam.get(j));  
                }  
            }  
        }  
    }  
    wb.write(os);  
} catch (Exception e) {  
    e.printStackTrace();  
} finally {  
    try {  
        if (os != null) {  
            os.close();  
        }  
    } catch (IOException e) {  
        e.printStackTrace();  
    } // 关闭输出流  
}
```
