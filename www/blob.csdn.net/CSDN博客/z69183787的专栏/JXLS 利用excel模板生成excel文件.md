# JXLS 利用excel模板生成excel文件 - z69183787的专栏 - CSDN博客
2016年10月19日 15:44:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1002
maven：
```
<dependency>
            <groupId>net.sf.jxls</groupId>
            <artifactId>jxls-core</artifactId>
            <version>1.0.3</version>
        </dependency>
```
Java：
```java
String templatePath = Thread.currentThread().getContextClassLoader().getResource("jxls/moduleClickTemplate.xlsx").getPath();
                InputStream inputStream = new BufferedInputStream(new FileInputStream(templatePath));
                Map beans = new HashMap();
                beans.put("topicIdList", topicIdList);
                beans.put("topicModuleMap", topicModuleMap);
                beans.put("result", result);
                XLSTransformer xlsTransformer = new XLSTransformer();
                org.apache.poi.ss.usermodel.Workbook workbook = xlsTransformer.transformXLS(inputStream, beans);
                response.setHeader("Content-Disposition", "attachment;filename=" + new String("模块点击数据".getBytes(),"ISO8859-1")+".xlsx");
                response.setContentType("application/vnd.ms-excel");
                OutputStream outputStream = response.getOutputStream();
                workbook.write(outputStream);
                outputStream.flush();
                inputStream.close();
                outputStream.close();
```
excel：
![](https://img-blog.csdn.net/20161019154337376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
