# 图片使用base64图片数据格式上传（后台使用springMVC） - 零度的博客专栏 - CSDN博客
2017年09月20日 11:21:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：7738
一、前台JSP页面
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
	<script type="text/javascript">
    //通过FileReader.readAsDataURL获取base64数据
    function uploadFile(file){
        var f = document.getElementById("myImage").files[0];
        var reader = new FileReader(); //新建一个FileReader
        reader.readAsDataURL(f);  // 读取文件base64数据
        reader.onload = function(e){ //数据读取完成产生onload事件
            var data = e.target.result; //获取数据
            if (data.lastIndexOf('data:base64') != -1) {
                  data = data.replace('data:base64', 'data:image/jpeg;base64');
            } else if (data.lastIndexOf('data:,') != -1) {
                  data = data.replace('data:,', 'data:image/jpeg;base64,');
            }
            if(isCanvasSupported()){
                 ajaxUploadBase64File(data);
            }else{
                 alert("您的浏览器不支持");
            }
        };
        reader.onerror = function(){
            console.log("上传失败了 ");
        }
        
    }
    //ajax异步上传
    function ajaxUploadBase64File(base64Data){
        var url = "${ctx}/upload/uploadBase64";
        $.ajax({
            url:url,
            type:"post",
            data:{base64Data:base64Data},
            success:function(data){
                if(data == "success"){
                   alert("上传成功");
                }else{
                    alert("上传失败");
                }
            },
            error:function(){
                alert("上传失败");
            }
        });
    }; 
    //是否支持canvas
    function isCanvasSupported(){
        var elem = document.createElement('canvas');
        return !!(elem.getContext && elem.getContext('2d'));
    };
	   
	</script>
</head>
<body>
    <input type="file" id="myImage" name="myImage"/> 
    <input type="button" onclick="uploadFile();" value="上传">
</body>
</html>
```
二、后台Controller
```
package org.nercita.bcp.upload;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.UUID;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.io.FileUtils;
import org.springframework.stereotype.Controller;
import org.springframework.util.Base64Utils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.commons.CommonsMultipartFile;
@Controller
public class UploadController {	
    @RequestMapping("/upload/uploadImage")
    public String uploadImagePage(){
	return "/upload/uploadImage";
    }
	
	
    @RequestMapping(value="/upload/uploadBase64",method=RequestMethod.POST)
    @ResponseBody
    public String base64UpLoad(@RequestParam String base64Data, HttpServletRequest request, HttpServletResponse response){  	       
        try{  	           
            String dataPrix = "";
            String data = "";	       
            if(base64Data == null || "".equals(base64Data)){
                throw new Exception("上传失败，上传图片数据为空");
            }else{
                String [] d = base64Data.split("base64,");
                if(d != null && d.length == 2){
                    dataPrix = d[0];
                    data = d[1];
                }else{
                    throw new Exception("上传失败，数据不合法");
                }
            }	          
            String suffix = "";
            if("data:image/jpeg;".equalsIgnoreCase(dataPrix)){//data:image/jpeg;base64,base64编码的jpeg图片数据
                suffix = ".jpg";
            } else if("data:image/x-icon;".equalsIgnoreCase(dataPrix)){//data:image/x-icon;base64,base64编码的icon图片数据
                suffix = ".ico";
            } else if("data:image/gif;".equalsIgnoreCase(dataPrix)){//data:image/gif;base64,base64编码的gif图片数据
                suffix = ".gif";
            } else if("data:image/png;".equalsIgnoreCase(dataPrix)){//data:image/png;base64,base64编码的png图片数据
                suffix = ".png";
            }else{
                throw new Exception("上传图片格式不合法");
            }
            String tempFileName = UUID.randomUUID().toString() + suffix;	            
            //因为BASE64Decoder的jar问题，此处使用spring框架提供的工具包
            byte[] bs = Base64Utils.decodeFromString(data);
            try{
                //使用apache提供的工具类操作流
            	
            	System.out.println(request.getServletContext().getRealPath("/upload"));
                FileUtils.writeByteArrayToFile(new File(request.getServletContext().getRealPath("/upload"), tempFileName), bs);  
            }catch(Exception ee){
                throw new Exception("上传失败，写入文件失败，"+ee.getMessage());
            }	           	          
            return "success";	           
        }catch (Exception e) {  	          
            return "error";
        }  	     
    }	
}
```
        本文使用的是FileRead的readAsDataURL获取base64的图片编码数据，通常我们 使用canvas画布在html的dom元素中所绘的图行，也可以使用 canvas的toDataUrl转换成base64编码的图片数据进行上传，具体获取方式如下：
```
var mycanvas = document.getElementById("mycanvas");  
 var imageBase64Data  = mycanvas.toDataURL("image/png");
```
