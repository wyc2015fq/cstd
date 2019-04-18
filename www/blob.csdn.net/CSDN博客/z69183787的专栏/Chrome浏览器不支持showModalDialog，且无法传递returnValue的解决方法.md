# Chrome浏览器不支持showModalDialog，且无法传递returnValue的解决方法 - z69183787的专栏 - CSDN博客
2012年11月19日 13:44:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5286
在调试的时候发现有2种情况
1、当前页面 弹出div操作层中 使用showModalDialog
2、当前页面弹出showModalDialog对话框中，使用showModalDialog
大致解决方法：父窗口中，若浏览器为chrome，则设置window.opener.returnValue 传递returnValue（即父窗口的returnValue属性）
同时
modal窗口中，增加一个方法，监测窗口关闭。
用来监测窗口关闭事件；
**在情况1下：chrome下showModalDialog的result将为undefined**
```java
$(function(){
$(window).unload(function(){
var ua = navigator.userAgent.toLowerCase(); 
if(ua.match(/chrome\/([\d.]+)/)){
	var result=new Array;    
	result[0]=document.getElementById('fileGroup').value;
        result[1]=document.getElementById('fileGroupId').value;
	result[2]=document.getElementById('fileGroupName').value;
	result[3]=document.getElementById('procType').value;
	result[4]=document.getElementById('targetType').value;
	result[5]=document.getElementById('fileType').value;
	result[6]=document.getElementById('fileCntObjId').value;
	result[7]=document.getElementById('attachMemo1').value;
	result[8]=document.getElementById('count').value;
	window.opener.returnValue=result;
	}else{
		btnClose(2);
	}			
	})
	window.onbeforeunload = function() { 
		var ua = navigator.userAgent.toLowerCase(); 
		if(ua.match(/chrome\/([\d.]+)/)){
		var result=new Array;    
		result[0]=document.getElementById('fileGroup').value;
		result[1]=document.getElementById('fileGroupId').value;
		result[2]=document.getElementById('fileGroupName').value;
		result[3]=document.getElementById('procType').value;
		result[4]=document.getElementById('targetType').value;
		result[5]=document.getElementById('fileType').value;
		result[6]=document.getElementById('fileCntObjId').value;
		result[7]=document.getElementById('attachMemo1').value;
		result[8]=document.getElementById('count').value;
		window.opener.returnValue=result;
		}else{
			btnClose(2);
		}
	}
			
})
```
```java
function btnClose(num) {
		
			var result=new Array;    
	        result[0]=document.getElementById('fileGroup').value;
	        result[1]=document.getElementById('fileGroupId').value;
	        result[2]=document.getElementById('fileGroupName').value;
	        result[3]=document.getElementById('procType').value;
	        result[4]=document.getElementById('targetType').value;
	        result[5]=document.getElementById('fileType').value;
	        result[6]=document.getElementById('fileCntObjId').value;
	        result[7]=document.getElementById('attachMemo1').value;
	        result[8]=document.getElementById('count').value;
	        
	        window.returnValue=result;
	        
	        var ua = navigator.userAgent.toLowerCase(); 
			if(ua.match(/chrome\/([\d.]+)/)){
				window.opener.returnValue=result;
			}
			if(num==1){
	        	window.close();
	        }
		}
```
父窗口：判断浏览器，若为chome且result为undefined，则取 window.returnValue（即子窗口中设置的）
```java
var result = window.showModalDialog(url, window, 'dialogWidth:600px;dialogHeight:300px;');
	//alert(window.returnValue);
	if (typeof (result) != "undefined") {
		var result_fileGroup = result[0];
		var result_fileGroupId = result[1];
		var result_fileGroupName = result[2];
		var result_procType = result[3];
		var result_targetType = result[4];
		var result_fileType = result[5];
		var result_fileCntObjId = result[6];
		var result_attachMemo1 = result[7];
		var result_count = result[8];
		$("#" + fileGroup).val(result_fileGroupId);
		$("#" + fileCntObjId).html(result_count);
		// $("."+fileCntObjId).html(result_count);
	}else{
		result = window.returnValue;
		var result_fileGroup = result[0];
		var result_fileGroupId = result[1];
		var result_fileGroupName = result[2];
		var result_procType = result[3];
		var result_targetType = result[4];
		var result_fileType = result[5];
		var result_fileCntObjId = result[6];
		var result_attachMemo1 = result[7];
		var result_count = result[8];
		$("#" + fileGroup).val(result_fileGroupId);
		$("#" + fileCntObjId).html(result_count);
	}
```
在情况2下：chrome下showModalDialog的result 有值，但若ModalDialog提供上传文件功能，ModalDialog作为form提交一次后，
即原来 a=1， form提交后 a=2，但result中只能接受到1，父窗口中解决方法如下：
```java
var result = window.showModalDialog(url, window, 'dialogWidth:650px;dialogHeight:350px;');
		var ua = navigator.userAgent.toLowerCase(); 
		if(ua.match(/chrome\/([\d.]+)/)){
			if (result != undefined) {
			    result = window.returnValue;
			}
		}
		if (typeof(result) != "undefined") { 
			var result_fileGroup=result[0];
			var result_fileGroupId=result[1]; 
			var result_fileGroupName=result[2]; 
			var result_procType=result[3]; 
			var result_targetType=result[4]; 
			var result_fileType=result[5]; 
			var result_fileCntObjId=result[6]; 
			var result_attachMemo1=result[7]; 
			var result_count=result[8]; 
			$("#"+fileGroup).val(result_fileGroupId);
			$("#"+fileCntObjId).html(result_count);
			$("."+fileCntObjId).html(result_count);
		}
```
上面2个是我在项目中遇到的不同情况，一开始还以为是理解有问题，后来发现div层中弹出dialog，和window窗口弹出dialog不一样。
在父窗口中需要做不同的判断。
