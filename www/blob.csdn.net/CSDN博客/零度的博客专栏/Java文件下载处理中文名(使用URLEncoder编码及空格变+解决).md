# Java文件下载处理中文名(使用URLEncoder编码及空格变+解决) - 零度的博客专栏 - CSDN博客
2016年04月20日 15:41:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：6069
解决转换空格 
String fileName = StringUtils.trim(file.getName()); 
String formatFileName = encodingFileName(name);//在后面定义方法encodingFileName(String fileName); 
response.setHeader("Content-Disposition", "attachment; filename=" + formatFileName ); 
//处理文件名中出现的空格    
//其中%20是空格在UTF-8下的编码 
public static String encodingFileName(String fileName) { 
        String returnFileName = ""; 
        try { 
            returnFileName = URLEncoder.encode(fileName, "UTF-8"); 
            returnFileName = StringUtils.replace(returnFileName, "+", "%20"); 
            if (returnFileName.length() > 150) { 
                returnFileName = new String(fileName.getBytes("GB2312"), "ISO8859-1"); 
                returnFileName = StringUtils.replace(returnFileName, " ", "%20"); 
            } 
        } catch (UnsupportedEncodingException e) { 
            e.printStackTrace(); 
            if (log.isWarnEnabled()) { 
                log.info("Don't support this encoding ..."); 
            } 
        } 
        return returnFileName; 
    } 
或者：
fileName = URLEncoder.encode(fileName,
 "utf-8")
fileName = fileName.Replace("+", "%20");  encode后替换  解决空格问题
response.addHeader("Content-Disposition",
 "attachment;filename=" + fileName, "utf-8");
