# 正则表达式使用js获取QueryString - z69183787的专栏 - CSDN博客
2017年07月05日 16:50:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：814
[http://www.cnblogs.com/sunnycoder/archive/2010/02/28/1674998.html](http://www.cnblogs.com/sunnycoder/archive/2010/02/28/1674998.html)
 从网上看到一些使用js获取QueryString的方法，但用起来不是很理想，所以决定自己写一个。主要原理是使用正则表达式匹配location.search中的字符串。
三个主要方法：
|**方法**|**说明**|
|----|----|
|getQueryString|获取QueryString的数组。例如路径QueryStringDemo.html?id=5&type=1&flag=0调用后返回["id=5", "type=1", "flag=0"]|
|getQueryStringByName|根据QueryString参数名称获取值|
|getQueryStringByIndex|根据QueryString参数索引获取值|
//获取QueryString的数组
function getQueryString(){
var result = location.search.match(new RegExp("[\?\&][^\?\&]+=[^\?\&]+","g")); 
if(result == null){
return"";
     }
for(var i = 0; i < result.length; i++){
         result[i] = result[i].substring(1);
     }
return result;
}
//根据QueryString参数名称获取值
function getQueryStringByName(name){
var result = location.search.match(new RegExp("[\?\&]" + name+ "=([^\&]+)","i"));
if(result == null || result.length < 1){
return"";
     }
return result[1];
}
//根据QueryString参数索引获取值
function getQueryStringByIndex(index){
if(index == null){
return"";
     }
var queryStringList = getQueryString();
if (index >= queryStringList.length){
return"";
     }
var result = queryStringList[index];
var startIndex = result.indexOf("=") + 1;
     result = result.substring(startIndex);
return result;
}
测试页面路径：QueryStringDemo.html?id=5&type=1&flag=0
页面加载时：
![clip_image001](http://images.cnblogs.com/cnblogs_com/sunnycoder/WindowsLiveWriter/jsQueryString_125D/clip_image001_thumb.jpg)
在QueryString's name后的文本框中输入要获取的QueryString的名称获取相应的值：
![clip_image002](http://images.cnblogs.com/cnblogs_com/sunnycoder/WindowsLiveWriter/jsQueryString_125D/clip_image002_thumb.jpg)
在QueryString's index后的文本框中输入要获取的QueryString的索引获取相应的值（索引从0开始）：
![clip_image003](http://images.cnblogs.com/cnblogs_com/sunnycoder/WindowsLiveWriter/jsQueryString_125D/clip_image003_thumb.jpg)
这样就可以在页面中方便的获取QueryString的值了。最后附上测试页面QueryStringDemo.html的源代码：
<!DOCTYPEhtmlPUBLIC"-//W3C//DTD XHTML 1.0 Transitional//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<htmlxmlns="http://www.w3.org/1999/xhtml">
<head>
<metacontent="text/html; charset=utf-8"http-equiv="Content-Type"/>
<title>QueryString获取</title>
<!--script type="text/javascript" src="CommonFiles/sunnycoder.js"></script-->
<scripttype="text/javascript">
//获取QueryString的数组
function getQueryString(){
var result = location.search.match(new RegExp("[\?\&][^\?\&]+=[^\?\&]+","g"));
for(var i = 0; i < result.length; i++){
              result[i] = result[i].substring(1);
         }
return result;
     }
//根据QueryString参数名称获取值
function getQueryStringByName(name){
var result = location.search.match(new RegExp("[\?\&]" + name+ "=([^\&]+)","i"));
if(result == null || result.length < 1){
return"";
         }
return result[1];
     }
//根据QueryString参数索引获取值
function getQueryStringByIndex(index){
if(index == null){
return"";
         }
var queryStringList = getQueryString();
if (index >= queryStringList.length){
return"";
         }
var result = queryStringList[index];
var startIndex = result.indexOf("=") + 1;
         result = result.substring(startIndex);
return result;
     }
//绑定当控件高亮选中时，点击“回车键”时执行的操作
//control：要绑定事件的控件
//func：要执行的方法
function bindEnterEvent(control, func){
         control.onkeypress = function(){
if (event.keyCode == 13){
                   func();
              }
         }
     }
//根据输入的QueryString名称获取值
function getByName(){
var name = document.getElementById("txtQueryStringName").value;
         document.getElementById("txtResult").innerHTML = getQueryStringByName(name);
     }
//根据输入的QueryString的索引获取值
function getByIndex(){
var index = document.getElementById("txtQueryStringIndex").value;
         document.getElementById("txtResult").innerHTML = getQueryStringByIndex(index);
     }
</script>
</head>
<body>
<div>
<span>QueryString : </span><spanid="queryString"></span>
</div>
<div>
<span>QueryString's name :</span>
<inputid="txtQueryStringName"name="txtQueryStringName"type="text"/>
<inputname="btnGetByName"type="button"value="获取"onclick="getByName()"/>
</div>
<div>
<span>QueryString's index : </span>
<inputid="txtQueryStringIndex"name="txtQueryStringIndex"type="text"/>
<inputname="btnGetByIndex"type="button"value="获取"onclick="getByIndex()"/>
</div>
<div>
<span>结果 ：</span><spanid="txtResult"></span>
</div>
<!--页面加载时执行的操作-->
<scripttype="text/javascript">
//显示所有QueryString
     document.getElementById("queryString").innerHTML = getQueryString();
//为txtQueryStringName绑定回车事件
     bindEnterEvent(txtQueryStringName, getByName);
//为txtQueryStringIndex绑定回车事件
     bindEnterEvent(txtQueryStringIndex, getByIndex);
</script>
</body>
</html>
