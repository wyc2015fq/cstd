# Springmvc 使用 AbstractExcelView 导出excel - z69183787的专栏 - CSDN博客
2014年12月04日 15:41:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：19304
页面调用：
```java
$("#exportBtn").click(function () {
            location.href = "${pageContext.request.contextPath}/report/export?" + $("form").serialize()
        });
```
参考了网上资料，实现Spring mvc 下Excel导出 ，如有更好方法请大家拍砖
1.新建类继承AbstractExcelView
Java代码  ![收藏代码](http://zwtlong.iteye.com/images/icon_star.png)
- import java.io.OutputStream;  
- import java.util.Map;  
- import javacommon.util.MyUtils;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import org.apache.poi.hssf.usermodel.HSSFCell;  
- import org.apache.poi.hssf.usermodel.HSSFCellStyle;  
- import org.apache.poi.hssf.usermodel.HSSFRow;  
- import org.apache.poi.hssf.usermodel.HSSFSheet;  
- import org.apache.poi.hssf.usermodel.HSSFWorkbook;  
- import org.springframework.web.servlet.view.document.AbstractExcelView;  
- publicclass ViewExcel extends AbstractExcelView {  
- @Override
- protectedvoid buildExcelDocument(Map<String, Object> obj,  
-             HSSFWorkbook workbook, HttpServletRequest request, HttpServletResponse response)  
- throws Exception {  
-         HSSFSheet sheet = workbook.createSheet("list");    
-         sheet.setDefaultColumnWidth((short) 12);    
-         HSSFCell cell = getCell(sheet, 0, 0);    
-         setText(cell, "Spring Excel test");    
-         HSSFCellStyle dateStyle = workbook.createCellStyle();    
- //dateStyle.setDataFormat(HSSFDataFormat.getBuiltinFormat("mm/dd/yyyy"));  
-         cell = getCell(sheet, 1, 0);    
-         cell.setCellValue("日期：2008-10-23");    
- //cell.setCellStyle(dateStyle);  
-         getCell(sheet, 2, 0).setCellValue("测试1");    
-         getCell(sheet, 2, 1).setCellValue("测试2");  
-         HSSFRow sheetRow = sheet.createRow(3);    
- for (short i = 0; i < 10; i++) {    
-             sheetRow.createCell(i).setCellValue(i * 10);    
-         }    
-         String filename = "测试.xls";//设置下载时客户端Excel的名称   
-         filename = MyUtils.encodeFilename(filename, request);//处理中文文件名
-         response.setContentType("application/vnd.ms-excel");     
-         response.setHeader("Content-disposition", "attachment;filename=" + filename);     
-         OutputStream ouputStream = response.getOutputStream();     
-         workbook.write(ouputStream);     
-         ouputStream.flush();     
-         ouputStream.close();     
-     }  
- }  
处理中文文件名的方法：
Java代码  ![收藏代码](http://zwtlong.iteye.com/images/icon_star.png)
- /** 
-      * 设置下载文件中文件的名称 
-      *  
-      * @param filename 
-      * @param request 
-      * @return 
-      */
- publicstatic String encodeFilename(String filename, HttpServletRequest request) {    
- /** 
-        * 获取客户端浏览器和操作系统信息 
-        * 在IE浏览器中得到的是：User-Agent=Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Maxthon; Alexa Toolbar) 
-        * 在Firefox中得到的是：User-Agent=Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.7.10) Gecko/20050717 Firefox/1.0.6 
-        */
-       String agent = request.getHeader("USER-AGENT");    
- try {    
- if ((agent != null) && (-1 != agent.indexOf("MSIE"))) {    
-           String newFileName = URLEncoder.encode(filename, "UTF-8");    
-           newFileName = StringUtils.replace(newFileName, "+", "%20");    
- if (newFileName.length() > 150) {    
-             newFileName = new String(filename.getBytes("GB2312"), "ISO8859-1");    
-             newFileName = StringUtils.replace(newFileName, " ", "%20");    
-           }    
- return newFileName;    
-         }    
- if ((agent != null) && (-1 != agent.indexOf("Mozilla")))    
- return MimeUtility.encodeText(filename, "UTF-8", "B");    
- 
- return filename;    
-       } catch (Exception ex) {    
- return filename;    
-       }    
-     }   
2接下来在你的下载 Controller里面返回ModelAndView 即可
Java代码  ![收藏代码](http://zwtlong.iteye.com/images/icon_star.png)
- /**
-     * 导出Excel
-     * @param model
-     * @param projectId
-     * @param request
-     * @return
-     */
- @RequestMapping(value="/dcExcel",method=RequestMethod.GET)  
- public ModelAndView toDcExcel(ModelMap model, HttpServletRequest request){  
-        List list = new ArrayList();  //测试数据没有用到
-        Map map = new HashMap();    
-        list.add("test1");    
-        list.add("test2");    
-        map.put("list", list);    
-        ViewExcel viewExcel = new ViewExcel();    
- returnnew ModelAndView(viewExcel, model);   
-    }  
代码很简单，只实现下载，关于Excel 格式数据方面请参考 poi 框架
