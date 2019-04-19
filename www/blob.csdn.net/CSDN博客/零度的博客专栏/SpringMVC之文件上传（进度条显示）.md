# SpringMVC之文件上传（进度条显示） - 零度的博客专栏 - CSDN博客
2016年06月02日 12:48:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：6915
#### 1、实现org.apache.commons.fileupload.ProgressListener接口
```
import javax.servlet.http.HttpSession;
import org.apache.commons.fileupload.ProgressListener;
import org.springframework.stereotype.Component;
import com.chinalife.cip.web.multipart.model.Progress;
@Component
public class FileUploadProgressListener implements ProgressListener {
	private HttpSession session;
	public void setSession(HttpSession session){
		this.session=session;
		Progress status = new Progress();
		session.setAttribute("status", status);
	}
	/*
	 * pBytesRead 到目前为止读取文件的比特数 pContentLength 文件总大小 pItems 目前正在读取第几个文件
	 */
	public void update(long pBytesRead, long pContentLength, int pItems) {
		Progress status = (Progress) session.getAttribute("status");
		status.setpBytesRead(pBytesRead);
		status.setpContentLength(pContentLength);
		status.setpItems(pItems);
	}
}
```
#### 2、扩展org.springframework.web.multipart.commons.CommonsMultipartResolver类
      重写public MultipartParsingResult parseRequest(HttpServletRequest request) throws MultipartException方法
```
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUpload;
import org.apache.commons.fileupload.FileUploadBase;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.multipart.MaxUploadSizeExceededException;
import org.springframework.web.multipart.MultipartException;
import org.springframework.web.multipart.commons.CommonsMultipartResolver;
import com.chinalife.cip.web.multipart.listener.FileUploadProgressListener;
public class CustomMultipartResolver extends CommonsMultipartResolver {
	@Autowired
	private FileUploadProgressListener progressListener;
	
	public void setFileUploadProgressListener(FileUploadProgressListener progressListener){
		this.progressListener=progressListener;
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public MultipartParsingResult parseRequest(HttpServletRequest request)
			throws MultipartException {
		String encoding = determineEncoding(request);
		FileUpload fileUpload = prepareFileUpload(encoding);
		progressListener.setSession(request.getSession());
		fileUpload.setProgressListener(progressListener);
		try {
			List<FileItem> fileItems = ((ServletFileUpload) fileUpload).parseRequest(request);
			return parseFileItems(fileItems, encoding);
		}
		catch (FileUploadBase.SizeLimitExceededException ex) {
			throw new MaxUploadSizeExceededException(fileUpload.getSizeMax(), ex);
		}
		catch (FileUploadException ex) {
			throw new MultipartException("Could not parse multipart servlet request", ex);
		}
	}
}
```
#### 3、mvc命名空间声明
```
<bean id="multipartResolver" class="com.chinalife.cip.web.multipart.CustomMultipartResolver">
	<property name="maxUploadSize" value="50000000" />
</bean>
```
#### 4、获取文件controller编写
```
@RequestMapping(...)
public String uploadFile(@RequestParam(value = "file") MultipartFile... files) throws IOException {
	for (MultipartFile f : files) {
		if (f.getSize() > 0) {
			File targetFile = new File("目标文件路径及文件名");
			f.transferTo(targetFile);//写入目标文件
		}
	}
	return "...";
}
```
#### 5、文件进度controller编写
```
import java.util.Map;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.SessionAttributes;
import com.chinalife.cip.web.multipart.model.Progress;
@Controller
@SessionAttributes("status")
public class ProgressController {
	@RequestMapping(value = "/upfile/progress", method = RequestMethod.POST )
	@ResponseBody
	public String initCreateInfo(Map<String, Object> model) {
		Progress status = (Progress) model.get("status");
		if(status==null){
			return "{}";
		}
		return status.toString();
	}
}
```
#### 6、最后
       html及js脚本就不在此赘述了，网上有很多例子，给出一个示例。
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<c:set var="ctx" value="${pageContext.request.contextPath}" />
<!DOCTYPE html>
<html> 
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
	<title>文件上传</title>
	<script type="text/javascript"  src="${ctx}/js/jquery-2.1.4/jquery-2.1.4.js"></script>
	<script type="text/javascript" src="${ctx}/js/bootstrap-3.3.5/js/bootstrap.min.js"></script>
    <link rel="stylesheet" href="${ctx}/js/bootstrap-3.3.5/css/bootstrap.min.css" />
	<style type="text/css">
		.file-box {
			position: relative;
			width: 340px
		}
		
		.txt {
			height: 22px;
			border: 1px solid #cdcdcd;
			width: 180px;
			vertical-align: middle;
			margin: 0;
			padding: 0
		}
		
		.btn {
			border: 1px solid #CDCDCD;
			height: 24px;
			width: 70px;
			vertical-align: middle;
			margin: 0;
			padding: 0
		}
		
		.file {
			position: absolute;
			top: 0;
			right: 80px;
			height: 24px;
			filter: alpha(opacity :   0);
			opacity: 0;
			width: 260px;
			vertical-align: middle;
			margin: 0;
			padding: 0
		}
	</style>
	<script type="text/javascript">
	    <%-- 
	    function myInterval()
	       {
	    	   $("#progress").html("");
	    	   $.ajax({
	    		   type: "POST",
	    		   url: "<%=basePath%>UserControllers/getSessions",
				data : "1=1",
				dataType : "text",
				success : function(msg) {
					var data = msg;
					console.log(data);
					$("#pdiv").css("width", data + "%");
					$("#progress").html(data + "%");
				}
			});
		}
		function autTime() {
			setInterval("myInterval()", 200);//1000为1秒钟
		} 
		--%>
		
		function UpladFile() {
			var fileObj = document.getElementById("file").files[0]; // js 获取文件对象
			var FileController = "${ctx}/upload/progress"; // 接收上传文件的后台地址 
			// FormData 对象
			var form = new FormData();
			form.append("author", "hooyes"); // 可以增加表单数据
			form.append("file", fileObj); // 文件对象
			// XMLHttpRequest 对象
			var xhr = new XMLHttpRequest();
			xhr.open("post", FileController, true);
			xhr.onload = function() {
				alert("上传完成!");
				//$('#myModal').modal('hide');
			};
			xhr.upload.addEventListener("progress", progressFunction, false);
			xhr.send(form);
		}
		
		function progressFunction(evt) {
	
			var progressBar = document.getElementById("progressBar");
	
			var percentageDiv = document.getElementById("percentage");
	
			if (evt.lengthComputable) {
	
				progressBar.max = evt.total;
	
				progressBar.value = evt.loaded;
	
				percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100)+ "%";
	
			}
	
		}
	</script>
</head>
<body style="width: 80%;height: 80%;margin: 0px auto;">
	<div class="row bootstrap-admin-no-edges-padding">
		<div class="col-md-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					<div class="text-muted bootstrap-admin-box-title">文件管理</div>
				</div>
				<div class="bootstrap-admin-panel-content">
					<button class="btn btn-primary btn-lg" data-toggle="modal"
						data-target="#myModal">上传</button>
					<input type="text" id="test">
				</div>
			</div>
		</div>
	</div>
	<!-- 模态框（Modal） -->
	<div class="modal fade" id="myModal" tabindex="-1" role="dialog"
		aria-labelledby="myModalLabel" aria-hidden="true">
		<div class="modal-dialog">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"
						aria-hidden="true">×</button>
					<h4 class="modal-title" id="myModalLabel">文件上传进度</h4>
				</div>
				<div class="modal-body">
					<progress id="progressBar" value="0" max="100"
						style="width: 100%;height: 20px; "> </progress>
					<span id="percentage" style="color:blue;"></span> <br>
					<br>
					<div class="file-box">
						<input type='text' name='textfield' id='textfield' class='txt' />
						<input type='button' class='btn' value='浏览...' /> <input
							type="file" name="file" class="file" id="file" size="28"
							onchange="document.getElementById('textfield').value=this.value" />
						<input type="submit" name="submit" class="btn" value="上传"
							onclick="UpladFile()" />
						
					</div>
				</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-default" data-dismiss="modal">关闭
					</button>
				</div>
			</div>
			<!-- /.modal-content -->
		</div>
		<!-- /.modal -->
	</div>
</body>
</html>
```
