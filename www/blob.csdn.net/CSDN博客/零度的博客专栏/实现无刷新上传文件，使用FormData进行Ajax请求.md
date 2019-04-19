# 实现无刷新上传文件，使用FormData进行Ajax请求 - 零度的博客专栏 - CSDN博客
2017年01月12日 13:13:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：1494
  通过传统的form表单提交的方式上传文件：
Html代码  ![收藏代码](http://yunzhu.iteye.com/images/icon_star.png)
- <formid= "uploadForm"action= "http://localhost:8080/cfJAX_RS/rest/file/upload"method= "post"enctype ="multipart/form-data">
- <h1>测试通过Rest接口上传文件 </h1>
- <p>指定文件名： <inputtype ="text"name="filename"/></p>
- <p>上传文件： <inputtype ="file"name="file"/></p>
- <p>关键字1： <inputtype ="text"name="keyword"/></p>
- <p>关键字2： <inputtype ="text"name="keyword"/></p>
- <p>关键字3： <inputtype ="text"name="keyword"/></p>
- <inputtype ="submit"value="上传"/>
- </form>
![](http://static.blog.csdn.net/images/save_snippets.png)
不过传统的form表单提交会导致页面刷新，但是在有些情况下，我们不希望页面被刷新，这种时候我们都是使用Ajax的方式进行请求的：
Js代码  ![收藏代码](http://yunzhu.iteye.com/images/icon_star.png)
- $.ajax({  
-      url : "http://localhost:8080/STS/rest/user",  
-      type : "POST",  
-      data : $( '#postForm').serialize(),  
-      success : function(data) {  
-           $( '#serverResponse').html(data);  
-      },  
-      error : function(data) {  
-           $( '#serverResponse').html(data.status + " : " + data.statusText + " : " + data.responseText);  
-      }  
- });  
![](http://static.blog.csdn.net/images/save_snippets.png)
如上，通过$('#postForm').serialize()可以对form表单进行序列化，从而将form表单中的所有参数传递到服务端。
但是上述方式，只能传递一般的参数，上传文件的文件流是无法被序列化并传递的。
不过如今主流浏览器都开始支持一个叫做FormData的对象，有了这个FormData，我们就可以轻松地使用Ajax方式进行文件上传了。
 FormData是什么呢？我们来看看Mozilla上的介绍。XMLHttpRequest Level 2添加了一个新的接口`FormData`.利用`FormData对象`,我们可以通过JavaScript用一些键值对来模拟一系列表单控件,我们还可以使用XMLHttpRequest的[`send()`](https://developer.mozilla.org/zh-CN/docs/DOM/XMLHttpRequest#send%28%29)方法来异步的提交这个"表单".比起普通的ajax,使用`FormData`的最大优点就是我们可以异步上传一个二进制文件。所有主流浏览器的较新版本都已经支持这个对象了，比如Chrome
 7+、Firefox 4+、IE 10+、Opera 12+、Safari 5+。
这里只展示一个通过from表单来初始化FormData的方式：
<formenctype=*"multipart/form-data"*method=*"post"*name=*"fileinfo"*>
Js代码  ![收藏代码](http://yunzhu.iteye.com/images/icon_star.png)
- var oData = new FormData(document.forms.namedItem("fileinfo" ));  
- oData.append( "CustomField", "This is some extra data" );  
- var oReq = new XMLHttpRequest();  
- oReq.open( "POST", "stash.php" , true );  
- oReq.onload = function(oEvent) {  
- if (oReq.status == 200) {  
-           oOutput.innerHTML = "Uploaded!" ;  
-      } else {  
-           oOutput.innerHTML = "Error " + oReq.status + " occurred uploading your file.<br \/>";  
-      }  
- };  
- oReq.send(oData);  
![](http://static.blog.csdn.net/images/save_snippets.png)
**使用FormData，进行Ajax请求并上传文件**：
Html代码  ![收藏代码](http://yunzhu.iteye.com/images/icon_star.png)
- <formid= "uploadForm">
- <p>指定文件名： <inputtype="text"name="filename"value= ""/></p>
- <p>上传文件： <inputtype="file"name="file"/></p>
- <inputtype="button"value="上传"onclick="doUpload()"/>
- </form>
![](http://static.blog.csdn.net/images/save_snippets.png)
Js代码  ![收藏代码](http://yunzhu.iteye.com/images/icon_star.png)
- function doUpload() {  
- var formData = new FormData($( "#uploadForm" )[0]);  
-      $.ajax({  
-           url: 'http://localhost:8080/cfJAX_RS/rest/file/upload' ,  
-           type: 'POST',  
-           data: formData,  
-           async: false,  
-           cache: false,  
-           contentType: false,  
-           processData: false,  
-           success: function (returndata) {  
-               alert(returndata);  
-           },  
-           error: function (returndata) {  
-               alert(returndata);  
-           }  
-      });  
- }  
