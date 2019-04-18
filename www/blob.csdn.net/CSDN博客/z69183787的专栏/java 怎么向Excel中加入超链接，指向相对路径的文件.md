# java 怎么向Excel中加入超链接，指向相对路径的文件 - z69183787的专栏 - CSDN博客
2016年10月08日 10:29:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4627
**当希望指向文件时，正常应设置为HSSFHyperlink.LINK_FILE，但指向目录中有中文时，会出现乱码并导致无法超链，****可更改为HSSFHyperlink.LINK_URL解决此问题**如何读取超链接
    HSSFSheet sheet = workbook.getSheetAt(0);
HSSFCell cell = sheet.getRow(0).getCell((short)0);
    HSSFHyperlink link = cell.getHyperlink();
    if(link != null){
        System.out.println(link.getAddress());
    }
如何设置超链接
    HSSFWorkbook wb = new HSSFWorkbook();
//超链接的单元格风格
    //超链接默认的是蓝色底边框
    HSSFCellStyle hlink_style = wb.createCellStyle();
    HSSFFont hlink_font = wb.createFont();
    hlink_font.setUnderline(HSSFFont.U_SINGLE);
    hlink_font.setColor(HSSFColor.BLUE.index); 
    hlink_style.setFont(hlink_font);
HSSFCell cell;
    HSSFSheet sheet = wb.createSheet("Hyperlinks");
//URL
    cell = sheet.createRow(0).createCell((short)0);
    cell.setCellValue("URL Link");
    HSSFHyperlink link = new HSSFHyperlink(HSSFHyperlink.LINK_URL);
    link.setAddress("");
    cell.setHyperlink(link);
    cell.setCellStyle(hlink_style);
//链接到当前路径的一个文件
    cell = sheet.createRow(1).createCell((short)0);
    cell.setCellValue("File Link");
    link = new HSSFHyperlink(HSSFHyperlink.LINK_FILE);
    link.setAddress("link1.xls");
    cell.setHyperlink(link);
    cell.setCellStyle(hlink_style);
//链接到e-mail 
    cell = sheet.createRow(2).createCell((short)0);
    cell.setCellValue("Email Link");
    link = new HSSFHyperlink(HSSFHyperlink.LINK_EMAIL);
    //note, if subject contains white spaces, make sure they are url-encoded
    link.setAddress("mailto:poi@apache.org?subject=Hyperlinks");
    cell.setHyperlink(link);
    cell.setCellStyle(hlink_style);
//链接到 workbook的某个地方
//创建一个目标Sheet和单元格
    HSSFSheet sheet2 = wb.createSheet("Target Sheet");
    sheet2.createRow(0).createCell((short)0).setCellValue("Target Cell");
cell = sheet.createRow(3).createCell((short)0);
    cell.setCellValue("Worksheet Link");
    link = new HSSFHyperlink(HSSFHyperlink.LINK_DOCUMENT);
    link.setAddress("'Target Sheet'!A1");
    cell.setHyperlink(link);
    cell.setCellStyle(hlink_style);
FileOutputStream out = new FileOutputStream("c://hssf-links.xls");
    wb.write(out);
    out.close();
