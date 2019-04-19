# 使用freemark导出数据到Word(遍历List) - 零度的博客专栏 - CSDN博客
2016年11月22日 12:38:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：8529
1、根据word编辑模板如下：
![](https://img-blog.csdn.net/20161122113531756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、将该模板另存为xml文件，进而重命名为.ftl文件，就得到了freemark的模板，因为是一个列表显示，我们需要所以我们需要根据freemark的标签进行改写，找到word中的每一个表格的行进行改写，改写后的ftl的文件如下：
![](https://img-blog.csdn.net/20161122114601396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、导出工具类
```
package org.nercita.bcp.word.util;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.URLEncoder;
import java.util.Map;
import javax.mail.internet.MimeUtility;
import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.lang.StringUtils;
import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
/**
 * The <code>WordExportUtil</code> class is a util class for word export.
 * 工具类：实现根据freemark 模板导出word文件
 * @author zhangwenchao
 * @since  Sample1.0
 */
public class WordExportUtil {
	
    public static String WORD_2007 = "WORD_2007";
    public static String WORD_2003 = "WORD_2003";
    
    /**
     * 设置下载文件中文件的名称
     * @param filename
     * @param request
     * @return
     */
    public static String encodeFilename(String filename, HttpServletRequest request) {
        String agent = request.getHeader("USER-AGENT");
        try {
            if ((agent != null) && (-1 != agent.indexOf("MSIE"))) {
                String newFileName = URLEncoder.encode(filename, "UTF-8");
                newFileName = StringUtils.replace(newFileName, "+", "%20");
                if (newFileName.length() > 150) {
                    newFileName = new String(filename.getBytes("GB2312"), "ISO8859-1");
                    newFileName = StringUtils.replace(newFileName, " ", "%20");
                }
                return newFileName;
            }
            if ((agent != null) && (-1 != agent.indexOf("Mozilla")))
                return MimeUtility.encodeText(filename, "UTF-8", "B");
            return URLEncoder.encode(filename, "UTF-8");
        } catch (Exception ex) {
            return filename;
        }
    }
    
    /**
     * 
     * @param request  HttpServletRequest
     * @param response HttpServletResponse
     * @param version  Word_2003/Word_2007
     * @param docFileName 生成的doc临时文件名
     * @param templateFile  freemark模板文件名
     * @param beanParams  入参数据:  Map<String, Object>类型
     */
    public static void writeResponse( HttpServletRequest request, HttpServletResponse response, String version, String docFileName,  String templateFile, Map<String, Object> beanParams) {
    	writeResponse(request, response, version, "temp", docFileName, "template", templateFile, beanParams);
    }
    
    
    /**
     * 
     * @param request  HttpServletRequest
     * @param response HttpServletResponse
     * @param version  Word_2003/Word_2007
     * @param docTempDir  生成的doc临时文件目录
     * @param docFileName 生成的doc临时文件名
     * @param templateDir 存放freemark模板的目录
     * @param templateFile freemark模板文件名
     * @param beanParams   入参数据:  Map<String, Object>类型
     */
    public static void writeResponse( HttpServletRequest request, HttpServletResponse response, String version, String docTempDir, String docFileName, String templateDir, String templateFile, Map<String, Object> beanParams) {
	    Configuration config = new Configuration();   
	    ServletContext sc = request.getSession().getServletContext();  
	    InputStream is = null;
	    File previewFile = null;
	    try {
		    config.setDirectoryForTemplateLoading(new File(sc.getRealPath("/"+templateDir)));   
		    config.setObjectWrapper(new DefaultObjectWrapper());   
		    Template template = config.getTemplate(templateFile, "UTF-8");  
	        if (WORD_2007.equals(version)) {
	        	docFileName = encodeFilename(docFileName + ".docx", request);	           
	        } else {
	        	docFileName = encodeFilename(docFileName + ".doc", request);	           
	        }
            String docName = request.getSession().getServletContext().getRealPath("/"+docTempDir)+docFileName;      
	        FileOutputStream fos = new FileOutputStream(docName);   
	        Writer out = new OutputStreamWriter(fos, "UTF-8");   
            template.process(beanParams, out);   
            out.flush();   
            out.close();
            
            
            previewFile = new File(docName);  
    		is = new FileInputStream(previewFile); 
    		response.reset(); 
    		if (WORD_2007.equals(version)) {
	            response.setContentType("application/vnd.openxmlformats-officedocument.wordprocessingml.document;charset=UTF-8");
	        }else{
	        	response.setContentType("application/vnd.ms-word;charset=UTF-8"); 
	        }    		
    		response.addHeader("Content-Disposition", "attachment;filename="+docFileName);    		
    		byte[] b = new byte[1024]; 
    		int len; 
    		while ((len=is.read(b)) >0) { 
    		   response.getOutputStream().write(b,0,len); 
    		}
    		response.getOutputStream().flush(); 
    		response.getOutputStream().close();  
        } catch (Exception e) {   
            e.printStackTrace();   
        }finally{
        	if(is!=null){
    			try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				} 
    		}
            if(previewFile!=null){
        	    previewFile.delete();
        	}
        }
    }
	
}
```
4、控制类
```
@RequestMapping("/test/word/export")
    public void exportWord(HttpServletResponse response,HttpServletRequest request) throws IOException{
    	Map<String, Object> beanParams = new HashMap<String, Object>();	
    	List<User> userList = new ArrayList<User>();
        for(int i=0;i<10;i++){
        	User u = new User();
        	u.setName("ZhangSan"+i);
        	u.setRealName("张三"+i);
        	u.setCompany("xx公司"+i);
        u.setPhone("130xxxxxxxx"+i); 
        userList.add(u);
        }
        beanParams.put("title", "用户信息");  
        beanParams.put("userList", userList);
        beanParams.put("attention", "请注意确保所有信息的正确性"); 
        
        WordExportUtil.writeResponse(request, response, WordExportUtil.WORD_2003, "用户信息列表导出", "templateFile.ftl", beanParams);
    }
```
5、运行结果
![](https://img-blog.csdn.net/20161122123749320?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
