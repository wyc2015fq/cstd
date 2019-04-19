# jquery上传文件插件-puupload.js - 零度的博客专栏 - CSDN博客
2016年04月25日 15:13:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：1160
前端：
```
<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="tags" tagdir="/WEB-INF/tags" %>
<c:set var="ctx" value="${pageContext.request.contextPath}"/>
<!DOCTYPE html>
<html>
<head>
<title>邮政网点-批量导入</title>
<script src="${ctx}/static/js/widgets/plupload/plupload.full.min.js" type="text/javascript"></script>
</head>
<body>
	<div class="container-fluid">
		<table class="table mytable-bordered table-condensed form-group" style="background-color: #FFFFFF;">
			<thead>
				<tr class="bgcolor">
					<td colspan="2" class="text-center"
						style="border: none; padding: 15px 0;">
						<h4 style="display: inline;"><strong class="text-info">邮政服务网点导入</strong></h4>
						<span style="float: right; margin-right: 10px;">
							<button type="button" class="close" onclick="$('#upload').modal('hide')">×</button>
						</span> 
					</td>
				</tr>
				<tr>
					<td class="text-left" style="width: 50%;">
						<div class="row-fluid">
							<div class="col-xs-6" style="padding: 0;">
									<input name="file" id="fileBtn" type="button" class="btn btn-success" value="选择文件">
									<input type="button" id="uploadBtn" class="btn btn-success" value="上传"/>
							</div>
							<div id="file-list">
							
							</div>
						</div>
					</td>
				</tr>
			</thead>
			<tbody id="infoTr">
			
			</tbody>
			<!-- <tfoot>
				<tr>
					<td colspan="4" class="text-center">
						<input type="button" class="btn btn-success btn-sm" value="关闭" onclick="$('#upload').modal('hide')">
					</td>
				<tr>
			</tfoot> -->
		</table>
		
		<%-- <div class="row-fluid">
			<div class="col-xs-12" style="padding: 0;">			
				<table class="table table-hover" id="dataTable" style="background-color: #FFFFFF;border-bottom: none;">
					<caption style="border: 1px solid #eef1f8;border-bottom: 1px solid #DDDDDD;background-color: #14a4f9;color: #FFFFFF;padding: 4px 0;">
						<span><strong style="line-height: 70%;vertical-align: middle;">  邮政企业列表</strong></span>						
					</caption>
					<thead>
						<tr style="border:none;">
							<td>错误原因</td>							
						</tr>
					</thead>
					<tbody id="datalistArea">
						<c:forEach items="${errorInfoList}" var="info" varStatus="st">
							<tr>
								<td class="text-left">${info}</td>
							</tr>
						</c:forEach>						
					</tbody>					
				</table>
			</div>
		</div> --%>
	</div>
<script type="text/javascript">
var upload =null;
$(function(){
	initUploadModal();
})
function initUploadModal(){
	uploader = new plupload.Uploader({
		chunk_size : '0',				//不使用分片上传
		multi_selection:false,				//是否可以选中多个文件
		multipart:true,
	    browse_button : 'fileBtn', 					//触发文件选择对话框的按钮，为那个元素id
	    file_data_name:'file',
	    filters : {               
	           max_file_size : '10mb',               
	           mime_types: [  
	               {title : "excel files", extensions : "xls"}  
	           ]  
	    },
	    url : '${ctx}/postalNetwork/upload', 		//服务器端的上传页面地址
	    flash_swf_url : '${ctx}/static/js/widgets/plupload/Moxie.swf', 			//swf文件，当需要使用swf方式进行上传时需要配置该参数
	    silverlight_xap_url : '${ctx}/static/js/widgets/plupload/Moxie.xap' 		//silverlight文件，当需要使用silverlight方式进行上传时需要配置该参数
	});
	uploader.init();
	//绑定文件添加进队列事件
	uploader.bind('FilesAdded',function(uploader,files){		
		for(var i = 0, len = files.length; i<len; i++){
			var file_name = files[i].name; //文件名
			//构造html来更新UI
			var html = '<li id="file-' + files[i].id +'"><p class="file-name">' + file_name + '</p>'+
			'<div class="progress"><div class="progress-bar" role="progressbar" aria-valuenow="60" aria-valuemin="0" aria-valuemax="100" style="width: 0%;"><span class="sr-only">40% 完成</span></div></div>'+
			'</li>';
			$('#file-list').html(html);
		}
	});
	uploader.bind('FileUploaded',function(uploader,file,responseObject){
		if(responseObject!=null && responseObject.response!='fail'){
			var info = eval('('+responseObject.response+')');
			if(info.length>0){
				var addHtml ='';
				if(info[0].address==undefined){
					for(var i=0;i<info.length;i++){
						addHtml +='<tr><td>'+info[i]+'</td></tr>';
					}					
				}else{					
					addHtml +='<tr><td>导入成功：插入数据 '+info.length+" 条</td></tr>";
				}
			}
			$('#infoTr').html(addHtml);
		}
		//alert(responseObject);
	});
	uploader.bind('UploadProgress',function(uploader,file){
		$('.progress-bar').css('width',file.percent + '%');//控制进度条
	}); 
	$('#uploadBtn').click(function(){
		uploader.start();
	});
	//绑定文件上传进度事件
	
	/* document.getElementById('uploadBtn').onclick = function(){
        uploader.start(); //调用实例对象的start()方法开始上传文件，当然你也可以在其他地方调用该方法
    } */
}
    
  	
</script>		
</body>
</html>
```
后台：
```
package com.china317.express.manageplatform.entity.bo;
import javax.servlet.http.HttpServletRequest;
import org.springframework.web.multipart.MultipartFile;
/** 
 * Plupload是一个上传插件。 
 * 这是一个bean类,主要存储Plupload插件上传时需要的参数。 
 * 属性名不可随意改动. 
 * 这里主要使用MultipartFile文件上传方法 
 */  
public class Plupload {  
      
    /**文件临时名(打文件被分解时)或原名*/  
    private String name;  
    /**总的块数*/  
    private int chunks = -1;  
    /**当前块数（从0开始计数）*/  
    private int chunk = -1;  
    /**HttpServletRequest对象，不能直接传入进来，需要手动传入*/  
    private HttpServletRequest request;  
    /**保存文件上传信息，不能直接传入进来，需要手动传入*/  
    private MultipartFile multipartFile;  
      
    public String getName() {  
        return name;  
    }  
  
    public void setName(String name) {  
        this.name = name;  
    }  
  
    public int getChunks() {  
        return chunks;  
    }  
  
    public void setChunks(int chunks) {  
        this.chunks = chunks;  
    }  
  
    public int getChunk() {  
        return chunk;  
    }  
  
    public void setChunk(int chunk) {  
        this.chunk = chunk;  
    }  
  
    public HttpServletRequest getRequest() {  
        return request;  
    }  
  
    public void setRequest(HttpServletRequest request) {  
        this.request = request;  
    }  
  
    public MultipartFile getMultipartFile() {  
        return multipartFile;  
    }  
  
    public void setMultipartFile(MultipartFile multipartFile) {  
        this.multipartFile = multipartFile;  
    }  
      
}
```
