# jquery.uploadify 3.2 使用 - z69183787的专栏 - CSDN博客
2013年12月10日 13:47:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2907
**[http://blog.csdn.net/z69183787/article/details/8258905](http://blog.csdn.net/z69183787/article/details/8258905) IE9 IE10兼容问题**
```java
$(function() {  
			
			 $("#<%=fileGroupName%>").uploadify({ 
				 //附带值
		        'formData':{
		        	'fileGroup':"<%=fileGroup%>",
		            'fileGroupName':"<%=fileGroupName%>",
		            'fileGroupId':"<%=fileGroupId%>",
		            'attachMemo1':"<%=attachMemo1%>",
		            'attachMemo':$("#attachMemo").val(),
		            'procType': "<%=procType%>",
		            'targetType':"<%=targetType%>",
		            'approve':"<%=approve%>",
		         	'fileCntObjId':"<%=fileCntObjId%>"
		        },
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
                'uploader'         : '<%=path %>/attach/upFileJsp.action;jsessionid=<%=session.getId()%>', 
                'cancelImg'      : '<%=path %>/attach/js/cancel.png', 
		       	'buttonText' : '选择文件',           //按钮名称  
		       	'fileObjName'    :'<%=fileGroupName%>',  
      
                'simUploadLimit' : 1, //一次同步上传的文件数目 
                //'sizeLimit'      : 10000000, //设置单个文件大小限制  单位byte
                'queueSizeLimit' : 1, //队列中同时存在的文件个数限制 
                'fileTypeDesc'       : '支持格式:*.', //如果配置了以下的'fileExt'属性，那么这个属性是必须的 
                'fileTypeExts'        : '*.*',//允许的格式    '*.jpg;*.bmp;*.png;*.gif'
                'fileSizeLimit' : '20000KB' ,    //文件大小   置单个文件大小限制  默认单位byte
        		'queueSizeLimit' : 1,//上传数量
        		'removeTimeout' : '1',//成功后移除 处理框
       			'removeCompleted' : true, 
       			'requeueErrors' : false,
		       'onUploadSuccess' : function(file, data, response){
		    	   //alert(data);
		    	   //window.name = "dialogPage"; 
		    	  
		    	   var attach = eval('(' + data + ')'); 
		    	   $("#fileGroup").val(attach.fileGroup);
		    	   $("#fileGroupName").val(attach.fileGroupName);
		    	   $("#fileGroupId").val(attach.fileGroupId);
		    	   $("#newGroupId").val(attach.newGroupId);
		    	   $("#procType").val(attach.procType);
		    	   $("#targetType").val(attach.targetType);
		    	   $("#fileCntObjId").val(attach.fileCntObjId);
		    	   $("#attachMemo").val(attach.attachMemo);
		    	   $("#approve").val(attach.approve);
		    	   //alert(1);
		    	   $("form").attr("enctype","application/x-www-form-urlencoded").attr("action","<%=path%>/attach/loadFileList.action");
		    	   $("form").submit();
		           // var url = "<%=path%>/attach/loadFileList.action?"+
		            //		"fileGroup="+attach.fileGroup +"&"+
		            	//	"fileGroupName="+attach.fileGroupName +"&"+
		            		///"fileGroupId="+attach.fileGroupId +"&"+
		            		//"newGroupId="+attach.newGroupId +"&"+
		            		//"procType="+attach.prcssocType +"&"+
		            		//"targetType="+attach.targetType +"&"+
		            		//"fileCntObjId="+attach.fileCntObjId +"&"+
		            		//"attachMemo="+attach.attachMemo +"&"+
		            		//"approve="+attach.approve;
		            //window.open(url,"dialogPage");
		            //console.log(url);
		        } ,
		      //选择上传文件后调用
		  		'onSelect' : function(file) {
		  			$(".uploadify").css("float","");
		  			//$(".uploadify").css("display","inline");
					// $(".uploadify-button ").css("display","inline");
					// $(".uploadify-queue").css("display","inline");
		     },
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
			 $(".uploadify").css("float","left");
		 });
```
```java
<input type="button" onclick="$('#<%=fileGroupName%>').uploadify('settings', 'formData', { 'attachMemo' : $('#attachMemo').val() } ); $('#<%=fileGroupName%>').uploadify('upload','*');" name="upBut" value="上传" class="btn" title="附件最大不允许超过20M" />
```
后台：
```java
@Action(value="/upFile")
	public String upFile() {
		//System.out.println("==========FjshAction upFile==================");
		request = ServletActionContext.getRequest();
		this.procParams(request);
		MultiPartRequestWrapper multi = (MultiPartRequestWrapper) request;
		
		String fileGroupId = StringUtil.getNotNullValueString(request.getParameter("fileGroupId"));
		String fileGroupName = StringUtil.getNotNullValueString(request.getParameter("fileGroupName"));
		String uploader = StringUtil.getNotNullValueString(request.getSession().getAttribute("user_name"));
		String uploaderLoginName = StringUtil.getNotNullValueString(request.getSession().getAttribute("login_name"));
		String memo = StringUtil.getNotNullValueString(request.getParameter("attachMemo"));
		//System.out.println("fileGroupId========"+fileGroupId+"|fileGroupName==="+fileGroupName+"|uploader==="+uploader);
		
		//String uploader = "attila";
		String uploadTime = DateUtil.getNowTime();
		String newGroupId = "";
		if (multi.getFiles(fileGroupName) != null&& multi.getFiles(fileGroupName).length > 0) {
			if(StringUtil.isNull(fileGroupId)||fileGroupId.trim().equalsIgnoreCase("undefined")){	//如果取不到文件组编号，说明是首次上传
				System.out.println("fileGroupId is null");
				newGroupId = this.fjshService.uploadNewFiles(null,multi.getFiles(fileGroupName),multi.getFileNames(fileGroupName),uploader,uploaderLoginName,uploadTime,memo);
			}else{	//上传文件至原有目录，并且同名覆盖
				System.out.println("fileGroupId is not null");
				this.fjshService.uploadOverrideFiles(fileGroupId,multi.getFiles(fileGroupName),multi.getFileNames(fileGroupName),uploader,uploaderLoginName,uploadTime,memo);
				newGroupId = fileGroupId;
			}
		}else{
			//System.out.println("===no file======");
		}
		//System.out.println("newGroupId========="+newGroupId);
		try {
			response.getWriter().print("{success:true, msg:true,newGroupId:'"+newGroupId+"'}");// Possible!
			//createJSonData("{\"success\":true,\"msg\":true,\"newGroupId\":" + newGroupId + "}");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return AJAX;
	}
```
