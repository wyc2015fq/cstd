# 使用flexpaper实现在线预览功能时 ，遇到的flash缓存问题的解放方案 - z69183787的专栏 - CSDN博客
2014年02月07日 18:14:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7324
本项目使用的是本地目录映射为服务器相对路径，故预览页面时读取的flash与项目不再同一盘符中，若更改flash文件后，预览还是只显示之前的旧文件，一番搜索后找到了解决方案，flexpaper 的官方demo中，有一个和java相关的demo，其中给出了一些方法，整理后使用如下：
原本flexpaper调用时：
```java
$('#documentViewer').FlexPaperViewer(
	                { config : {
	                    SWFFile : '<%=request.getContextPath()+"/"+swfFilePath%>',//服务器中映射为本地目录
	                    Scale : 1,
	                    ZoomTransition : 'easeOut',
	                    ZoomTime : 0.5,
	                    ZoomInterval : 0.2,
	                    FitPageOnLoad : true,
	                    FitWidthOnLoad : true,
	                    FullScreenAsMaxWindow : false,
	                    ProgressiveLoading : false,
	                    MinZoomSize : 0.2,
	                    MaxZoomSize : 5,
	                    SearchMatchAll : false,
	                    InitViewMode : 'Portrait',
	                    RenderingOrder : 'flash',
	                    StartAtPage : '',
	                    ViewModeToolsVisible : true,
	                    ZoomToolsVisible : true,
	                    NavToolsVisible : true,
	                    CursorToolsVisible : true,
	                    SearchToolsVisible : true,
	                    WMode : 'window',
	                    localeChain: 'en_US'
	                }}
	        );
```
但一直出现缓存，解决方案：使用另一页面用文件流形式输出flash byte数组，在页面上直接显示，SWFFile 的值设为该页面，查看源码后发现，嵌入该属性时需整个URL编码，不然会无法显示。
```java
function showSwf(swf){
				//console.log('show.jsp?swf='+encodeURI(swf)+'&temp='+Math.random());
		        $('#documentViewer').FlexPaperViewer(
		                { config : {
	
		                    SWFFile : encodeURIComponent('show.jsp?swf='+swf+'&temp='+Math.random()),
	
		                    Scale : 1,
		                    ZoomTransition : 'easeOut',
		                    ZoomTime : 0.5,
		                    ZoomInterval : 0.2,
		                    FitPageOnLoad : false,
		                    FitWidthOnLoad : true,
		                    FullScreenAsMaxWindow : false,
		                    ProgressiveLoading : true,
		                    MinZoomSize : 0.2,
		                    MaxZoomSize : 5,
		                    SearchMatchAll : false,
		                    InitViewMode : 'Portrait',
		                    RenderingOrder : 'flash',
		                    StartAtPage : '',
	
		                    ViewModeToolsVisible : true,
		                    ZoomToolsVisible : true,
		                    NavToolsVisible : true,
		                    CursorToolsVisible : true,
		                    SearchToolsVisible : true,
		                    WMode : 'window',
		                    localeChain: 'en_US'
		                }}
		        );
			}
```
显示页面：
```java
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
    <%@ page import="java.io.*" %>
    <%@ page import="com.wonders.attach.util.*" %>
<%!
public byte[] file_get_contents(String file) {
	byte[] con = {0};
	if(file == null || file == "")
		return con;
	try {
		File f = new File(file);
		if(!f.isFile() || !f.canRead())
			return con;
		FileInputStream fstream = new FileInputStream(file);
		con = new byte[(int) f.length()];
		fstream.read(con);
		fstream.close();
	} catch (Exception e) {
		e.printStackTrace();
	}
	return con;
}
%>
<%
String path = FileUpProperties.getValueByKey("file_path");
String swf = (String)request.getParameter("swf");
if(swf != null && swf.length()>0){
	swf = swf.replace("swf/","");
}
//System.out.println(swf);
 response.reset();//如果在weblogic底下同样要加上此句
BufferedOutputStream outs = new BufferedOutputStream(response.getOutputStream());
response.setContentType("application/x-shockwave-flash");
response.setHeader("Accept-Ranges", "bytes");
byte[] content = file_get_contents(path+swf);
response.setContentLength(content.length);
outs.write(content);
outs.flush();
	outs.close();
//	out.clear();
	//out = pageContext.pushBody();
%>
```
即可正常显示，解决缓存问题。
附上demo中，各文件格式输出方式：
```java
<%@ page import="lib.*,java.io.*" %>
<%
	BufferedOutputStream outs = new BufferedOutputStream(response.getOutputStream());
	common conf 	= new common();
	String doc 		= request.getParameter("doc");
	String pdfdoc	= doc;
	String pages	= request.getParameter("page");
	String format	= request.getParameter("format");
	String resolution	= request.getParameter("resolution");
	String callback = request.getParameter("callback");
	if(doc == null)return;
	if(!doc.endsWith(".pdf"))	{pdfdoc 	= doc + ".pdf";}
	if(pages == null)			{pages = "";}
	if(format == null)			{format="swf";}
	String swfdoc	= pdfdoc + ".swf";
	if("true".equals(conf.getConfig("splitmode", "")))	{	swfdoc 	= pdfdoc + "_" + pages + ".swf";}
	if(callback == null)		{callback = "";}
	String jsondoc	= pdfdoc + ".js";
	if("true".equals(conf.getConfig("splitmode", "")))	{	jsondoc = pdfdoc + "_" + pages + ".js";	}
	String pngdoc 		= pdfdoc + "_" + pages + ".png";
	String jpgcachedoc = pdfdoc + "_" + pages + "_res_" + resolution + ".jpg";
	String messages 	= "";
	String swfFilePath 	= conf.separate(conf.getConfig("path.swf", "")) + swfdoc;
	String pdfFilePath 	= conf.separate(conf.getConfig("path.pdf", "")) + pdfdoc;
	String pngFilePath 	= conf.separate(conf.getConfig("path.swf", "")) + pngdoc;
	String jpgCachePath 	= conf.separate(conf.getConfig("path.swf", "")) + jpgcachedoc;
	String jsonFilePath 	= conf.separate(conf.getConfig("path.swf", "")) + jsondoc;
	boolean validatedConfig = true;
	String error = "";
	if(!conf.is_dir(conf.getConfig("path.swf", ""))){
		error = "Error:Cannot find SWF output directory, please check your configuration file.";
		validatedConfig = false;
	}
	if(!conf.is_dir(conf.getConfig("path.pdf", ""))){
		error += "Error:Cannot find PDF output directory, please check your configuration file.";
		validatedConfig = false;
	}
	if(!validatedConfig){
		error += "Error:Cannot read directories set up in configuration file, please check your configuration.";
	}else if(!conf.validParams(pdfFilePath, pdfdoc, pages)){
		error += "Error:Incorrect file specified, please check your path.";
	}else{
		if("swf".equals(format) || "jpg".equals(format) || "png".equals(format) || "pdf".equals(format)){
			if(!conf.file_exists(swfFilePath)){
				pdf2swf pdfconv = new pdf2swf(request);
				messages = pdfconv.convert(pdfdoc, pages);
			}
			if("png".equals(format)||"jpg".equals(format)){
				if(conf.validParams(swfFilePath, swfdoc, pages)){
					if(!conf.file_exists(pngFilePath)){
						swfrender pngconv = new swfrender();
						pngconv.renderPage(pdfdoc, swfdoc, pages);
					}
					if("true".equals(conf.getConfig("allowcache", ""))){
						conf.setCacheHeaders(response);
					}
					if(conf.getConfig("allowcache") == null || "false".equals(conf.getConfig("allowcache", "")) || ("true".equals(conf.getConfig("allowcache", "")) && conf.endOrRespond(request, response))){
						if(resolution != null){
							response.setContentType("image/jpeg");
							outs.write(conf.file_get_contents(conf.generateImage(pngFilePath, jpgCachePath, resolution, "jpg")));
						}else if("jpg".equals(format)){
						    response.setContentType("image/jpeg");
							outs.write(conf.file_get_contents(conf.generateImage(pngFilePath, jpgCachePath, null, "jpg")));
						}else{
							response.setContentType("image/png");
							outs.write(conf.file_get_contents(pngFilePath));
						}
					}
				}else{
					if(messages.length() == 0 || "[OK]".equals(messages))
						messages = "[Incorrect file specified, please check your path]";
				}
			} else 	if("pdf".equals(format)){
				response.setContentType("application/pdf");
				outs.write(conf.file_get_contents(pdfFilePath));
			}
			if(conf.file_exists(swfFilePath)){
				if("swf".equals(format)){
					if("true".equals(conf.getConfig("allowcache", ""))){
						conf.setCacheHeaders(response);
					}
					if(conf.getConfig("allowcache") == null || "false".equals(conf.getConfig("allowcache", "")) || ("true".equals(conf.getConfig("allowcache", "")) && conf.endOrRespond(request,response))){
						response.setContentType("application/x-shockwave-flash");
						response.setHeader("Accept-Ranges", "bytes");
						byte[] content = conf.file_get_contents(swfFilePath);
						response.setContentLength(content.length);
						outs.write(content);
					}
				}
			}else{
				if(messages.length() == 0)
					messages = "[Cannot find SWF file. Please check your JSP configuration]";
			}
		}else if("json".equals(format) || "jsonp".equals(format)){
			if(!conf.file_exists(jsonFilePath)){
				pdf2json jsonconv = new pdf2json();
				messages = jsonconv.convert(pdfdoc, jsondoc, pages);
			}
			if(conf.file_exists(jsonFilePath)){
				if("true".equals(conf.getConfig("allowcache", ""))){
					conf.setCacheHeaders(response);
				}
				if(conf.getConfig("allowcache") == null || "false".equals(conf.getConfig("allowcache", "")) || ("true".equals(conf.getConfig("allowcache", "")) && conf.endOrRespond(request,response))){
					response.setContentType("text/javascript");
					if("json".equals(format)){
						outs.write(conf.file_get_contents(jsonFilePath));
					}
					if("jsonp".equals(format)){
						outs.write((callback + "(").getBytes());
						outs.write(conf.file_get_contents(jsonFilePath));
						outs.write((");").getBytes());
					}
				}
			}else{
				if(messages.length() == 0)
					messages = "[Cannot find JSON file. Please check your JSP configuration]";
			}
		}
		if(messages.length() > 0 && !"[OK]".equals(messages) && !"[Converted]".equals(messages) && !"png".equals(format)){
			outs.write(("Error:" + messages.substring(1,messages.length()-2)).getBytes());
		}
	}
	if(error.length() > 0)
		outs.write(error.getBytes());
	outs.flush();
	outs.close();
	out.clear();
	out = pageContext.pushBody();
%>
```
