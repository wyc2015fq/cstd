# jquery.uploadify.3.2 试用（同时解决IE9 IE10 按钮无法点击的问题） - z69183787的专栏 - CSDN博客
2012年12月05日 10:49:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15639
以前用的是版本2.1.4,这次看见更新后就尝试了一下，发现有很多改变。
首先引入 js 和 css
```
<link rel="stylesheet" href="uploadify.css" />
 <script src="jquery.uploadify.js"></script>
当然jquery 是必不可少的
```
接下来是代码：
```java
//文件上传
$(function() {
    $("#uploadify").uploadify({
//附带值
       'formData':{
         'userid':'用户id',
         'username':'用户名',
         'rnd':'加密密文'
       },
       'auto' : false,
       'method' : "post",
       'height' : '20',
       'width' : '100',
       'swf' : 'uploadify.swf', 
       'uploader' : '<%=basePath%>/contract/fileUpload.action',
       'fileTypeDesc' : '格式:txt,xls，xlsx,doc,docx',		//描述
       'fileTypeExts' : '*.txt;*.xls;*.xlsx;*.doc;*.docx;*.zip',			//文件类型
       'fileSizeLimit' : '10000KB',			//文件大小
       'buttonText' : '选择文件',			//按钮名称
       'fileObjName'	:'uploadify',
       'successTimeout' : '5',
       'requeueErrors' : false,
       'removeTimeout' : '1',
       'removeCompleted' : true,
       'onUploadSuccess' : function(file, data, response){
       		var attach = eval('(' + data + ')');
       		$("#fileTable").show();
       		var addHtml = "<tr>"+
       						"<td class='t_l'>"+
       							"<a href='<%=basePath%>/attach/downloadAttach.action?attachId="+attach.id+"'>"+attach.filename+"."+attach.fileextname+"</a>"+
       						"</td>"+
       						"<td class='t_r'>"+attach.filesize+"</td>"+
       						"<td class='t_c'>"+attach.uploaddate+"</td>"+
       						"<td class='t_c'><a href='<%=basePath%>/attach/downloadAttach.action?attachId="+attach.id+"' id='"+attach.id+"'>下载</a></td>"+
       						"<td class='t_c'><a href='#' onclick='removeFile(this)' id='"+attach.id+"' name='attach_id'>取消</a></td>"+
       					  "</tr>";
       		$("#fileBody").append(addHtml);
    	}
	});
});
```
3.2最新代码，更新了错误判断
```
<link rel="stylesheet" href="<%=path %>/attach/js/uploadify.css" />
<script src="<%=path %>/receive/js/jquery-1.7.1.js"></script>
<script src="<%=path %>/attach/js/jquery.uploadify.3.2.edited.js"></script>
```
```java
$(function() {  
			 $("#<%=fileGroupName%>").uploadify({ 
             	//开启调试
       		 	'debug' : false,
        		//是否自动上传
        		'auto':false,
                /*注意前面需要书写path的代码*/ 
                //浏览按钮的宽度
        		'width':'100',
        		//浏览按钮的高度
        		'height':'15',
               	'method' : "post",  
                'swf'       : '<%=path %>/attach/js/uploadify.swf', 
                'uploader'         : '<%=path %>/attach/upFileJsp.action', 
                'cancelImg'      : '<%=path %>/attach/js/cancel.png', 
		       	'buttonText' : '选择文件',           //按钮名称  
		       	'fileObjName'    :'<%=fileGroupName%>',  
      
                'simUploadLimit' : 3, //一次同步上传的文件数目 
                //'sizeLimit'      : 10000000, //设置单个文件大小限制  单位byte
                'queueSizeLimit' : 3, //队列中同时存在的文件个数限制 
                'fileTypeDesc'       : '支持格式:*.', //如果配置了以下的'fileExt'属性，那么这个属性是必须的 
                'fileTypeExts'        : '*.*',//允许的格式    '*.jpg;*.bmp;*.png;*.gif'
                'fileSizeLimit' : '20M' ,    //文件大小   置单个文件大小限制  默认单位byte
        		'queueSizeLimit' : 2,//上传数量
        		'removeTimeout' : '1',//成功后移除 处理框
       			'removeCompleted' : true, 
       			'requeueErrors' : false,
		       'onUploadSuccess' : function(file, data, response){  
		            alert(data);
		        } ,
	        
	         //返回一个错误，选择文件的时候触发
	        'onSelectError':function(file, errorCode, errorMsg){
	            switch(errorCode) {
	                case -100:
	                    this.queueData.errorMsg = "上传的文件数量已经超出系统限制的"+$('#<%=fileGroupName%>').uploadify('settings','queueSizeLimit')+"个文件！";
	                    break;
	                case -110:
	                    //alert("文件 ["+file.name+"] 大小超出系统限制的"+$('#<%=fileGroupName%>').uploadify('settings','fileSizeLimit')+"大小！");
	                    this.queueData.errorMsg = "文件 ["+file.name+"] 大小超出系统限制的"+$('#<%=fileGroupName%>').uploadify('settings','fileSizeLimit')+"大小！";
	                    break;
	                case -120:
	                    //alert("文件 ["+file.name+"] 大小异常！");
	                    this.queueData.errorMsg = "文件 ["+file.name+"] 大小异常！";
	                   break;
	                case -130:
	                    //alert("文件 ["+file.name+"] 类型不正确！");
	                    this.queueData.errorMsg = "文件 ["+file.name+"] 类型不正确！";
	                    break;
	            }
	           
	        }
				 }); 
		 });
```
其中 onUploadSuccess为成功上传后的回调函数 file 为上传的文件，可通过file.name 获取文件名 size 可获取大小
data 为后台reponse输出的字符串，上例中输出的是 json 对象，故使用eval 进行转换
response 为 结果 true or false，具体可参考官方文档。
```
<td colspan="3">
	<input type="file" name="uploadify" id="uploadify" />
	<input type="button" value="上传" onclick="$('#uploadify').uploadify('upload','*');">
	<input type="button" value="取消" onclick="$('#uploadify').uploadify('stop');">
	<table style="display: none;" id="fileTable">
		<tbody style="width: 550px;border: solid;border-color: #D0D0D3;" id="fileBody">
			<tr style="border: solid;border: #D0D0D3;">
				<td width="200px;" class="t_c">文件名</td>
				<td width="100px;" class="t_c">大小(k)</td>
				<td width="150px;" class="t_c">上传时间</td>
				<td width="100px;" class="t_c" colspan="2">操作</td>
			</tr>
		</tbody>
	</table>
</td>
```
可以看到初始化中的很多属性都变化了，还包括上传操作的函数名称等等。
其次，还有一个问题，该控件在IE9中 上传文件的按钮会无法点击，初步查看可能是由于flash 的问题 引起，百度后发现 修改源码js 中的 classid即可。
具体可参考：
[http://xuzhihong1987.blog.163.com/blog/static/26731587201222611160906/](http://xuzhihong1987.blog.163.com/blog/static/26731587201222611160906/)
[http://www.cnblogs.com/donhwa/archive/2011/06/23/ie9_swfupload_bug.html](http://www.cnblogs.com/donhwa/archive/2011/06/23/ie9_swfupload_bug.html)
解决此问题后的js替换文件下载
http://files.cnblogs.com/lostboy/jquery.uploadify3.1.fixed.js
或
http://download.csdn.net/detail/chenxiang199055/6003627
**具体修改内容为：**
```java
SWFUpload.prototype.getFlashHTML = function () {
    // Flash Satay object syntax: http://www.alistapart.com/articles/flashsatay
    var obj = ['<object id="', this.movieName, '" type="application/x-shockwave-flash" data="', this.settings.flash_url, '" width="', this.settings.button_width, '" height="', this.settings.button_height, '" class="swfupload">'].join(""),
params = [
'<param name="wmode" value="', this.settings.button_window_mode, '" />',
'<param name="movie" value="', this.settings.flash_url, '" />',
'<param name="quality" value="high" />',
'<param name="menu" value="false" />',
'<param name="allowScriptAccess" value="always" />',
'<param name="flashvars" value="', this.getFlashVars(), '" />'
].join("");
    if (navigator.userAgent.search(/MSIE/) > -1) {
        obj = ['<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" id="', this.movieName, '" width="', this.settings.button_width, '" height="', this.settings.button_height, '" class="swfupload">'].join("");
        params += '<param name="src" value="' + this.settings.flash_url + '" />';
    }
    return [obj, params, '</object>'].join("");
};
```
