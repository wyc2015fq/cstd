# 用Struts2+Dojo实现文件下载 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年11月20日 17:00:00[慢游](https://me.csdn.net/eengel)阅读数：2868








# 介绍

Struts2 + Dojo是目前非常通用的Web应用开发组合。文件下载则是非常老的一个功能了。现在就介绍一下如何用Struts2+ Dojo 1.7.3实现文件下载的功能。



# 前端：初始化Dojo，创建下载按钮，实现点击下载事件

test.jsp



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
@import "../js/dijit/themes/tundra/tundra.css";
@import "../css/main.css";
</style>

</head>
<body class="tundra">
<button id="download_btn" data-dojo-type="dijit.form.Button"
				class="menu_button" type="button">下载文件</button>

<script>
		dojoConfig = {
			isDebug : false,
			parseOnLoad : true,
			async : true
		};
	</script>
	<script type="text/javascript" src="../js/dojo/dojo.js"></script>
	<script>
		require([ "dojo/parser", "dijit/form/Button"]);
	</script>
	<script>
		require(
				[ "dojo/ready", 
				  "dijit/registry",
				  "dojo/dom",
				  "dojo/on",
				  "dojo/io/iframe"
				  ],
				function(ready, registry, dom, on) {
					ready(function() {
						on(registry.byId("download_btn"), "click", function(e){
							var exportIframeName = "downloadPdfIframe"; 
							var iframe = dojo.io.iframe.create(exportIframeName);
							var url = "download.action"; 
							dojo.io.iframe.setSrc(iframe, url, true);
						});
					});
				});
	</script>
</body>
</html>
```


# 后端：配置下载请求



struts.xml

```
<action name="download" class="test.servlet.DownloadAction">
			<result name="success" type="stream">
				<param name="contentType">text/plain</param>
				<param name="inputName">inputStream</param>
				<param name="contentDisposition">
		            attachment;filename="${filename}.xml"
		        </param>
				<param name="bufferSize">2048</param>
			</result>
</action>
```


# 后端：实现下载

DownloadAction.java

```java
package test.servlet;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;

import org.apache.commons.io.FileUtils;

import com.opensymphony.xwork2.ActionSupport;

public class DownloadAction extends ActionSupport {

	private static final long serialVersionUID = 1L;
	
	private String filename;
	private InputStream inputStream;

	public String execute() {
		String xml;
		try {
			xml = FileUtils.readFileToString(new File("c:\test.xml"), "UTF-8");
		} catch (IOException e) {
			return ERROR;
		}
		
		filename = "test";
		try {
			inputStream = new ByteArrayInputStream(xml.getBytes("UTF-8"));
		} catch (UnsupportedEncodingException e) {
			return ERROR;
		}
				
		return SUCCESS;
	}

	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public InputStream getInputStream() {
		return inputStream;
	}

	public void setInputStream(InputStream inputStream) {
		this.inputStream = inputStream;
	}
}
```


# 总结

上面的实现有两个关键点。一个就是用Dojo来实现客户端的文件下载功能。这儿首先要用dojo.io.iframe.create创建一个iframe，然后将这个iframe传给dojo.io.iframe.setSrc，并设置正确的URL。另外一个关键的地方是后台struts.xml和Action.java的配合。struts.xml中两个属性inputName和contentDisposition中的内容必须要和Action.java中的变量对应。而result的type设置为stream。




