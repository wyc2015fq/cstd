# 在JavaScript中调用WebService - 左直拳的马桶_日用桶 - CSDN博客
2006年12月26日 17:47:00[左直拳](https://me.csdn.net/leftfist)阅读数：6690
在JavaScript中调用WebService
左直拳
试着在JavaScript中调用了一下WebService。觉得有两个地方要注意。
1、参数传递。
一般调用WebService的方法，都要传参数。怎么传？如下：
【WebService部分】
文件：UserManage/UserInfo.asmx
方法：
[WebMethod]
publicstring GetUserName(string accounts)
【JavaScript部分】
//WebService地址及方法名称。其中GetUserName是要调用的方法
var URL = "UserManage/UserInfo.asmx/GetUserName"; 
var Params = "accounts=leftfist";//传给WebService的参数
var xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
xmlhttp.Open("POST",URL, false);//用POST方法
xmlhttp.SetRequestHeader("Content-Type","application/x-www-form-urlencoded");
xmlhttp.SetRequestHeader ("Content-Length",Params.length);
xmlhttp.send(Params);
if( xmlhttp.Status == 200 ){//200代表成功
var res = xmlhttp.responseXML;//得到WebService传回的结果
return res.childNodes[1].text + "，";
}
return xmlhttp.responseText;
2、配置文件（Web.config）问题。
JavaScript脚本写好以后，在本地上运行并无问题。但上传到服务器却显示：
Request format is unrecognized for URL unexpectedly ending in '/GetUserName'
估计是"UserManage/UserInfo.asmx/GetUserName"这种调用WebService的写法服务器还不认，找了很久才知道，应该在Web.config里加上：
<webServices>
<protocols>
<addname="HttpGet"/>
<addname="HttpPost"/>
</protocols>
</webServices>
