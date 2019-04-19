# 关于Ajax使用的总结 - Big Smile - CSDN博客
2016年08月26日 11:40:47[王啸tr1912](https://me.csdn.net/tr1912)阅读数：665
       最近敲手机微信版的订餐，对于Ajax的了解有了新的认识，现在和大家分享一下。
# 一、Ajax的核心XMLHttpRequest
        其实这个是我的上任师傅在我学习Ajax的时候告诉我的，当时没有怎么当一回事，事后在订餐中碰到了，觉得很是不可思议，因为我们在一般处理程序中用到的恰好是XMLHttpRequest的东西，里面巧妙的调用让我感觉很是不错，因为这使整个网页灵活了不少。
        订餐中的例子，其实就是刷卡的时候的核心读卡和切卡，用到这个东西也是经过师哥的修改才用好的，下面是调用的流程图：
![](https://img-blog.csdn.net/20160826105738844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)       首先打卡刷卡的网页，然后通过Js调用的就是一个刷卡的一般处理程序，这个JS就是用的XMLHttpRequest流程写的，也就是Ajax最根本的东西，这样可以调用，也可以接受请求，十分的方便。
         在判断有卡了之后又可以调用另外的一个一般处理程序来判断卡的状态，来扣钱。
         这里为什么要直接用XMLHttpRequest其实我也说不好，但是这里面的灵活性和快速的调用就直接吸引了我的关注。
下面是部分XMLHttpRequest的代码：
```java
function GetXmlHttp() {   //初始化XMLHTTPRequest
    return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
}
function GetRequest() {
    //获取出number 如果<3 handler1  else  handler2
    var url = "handleIsNewCard.ashx";       //调用读卡的一般处理程序
    xmlHttp = GetXmlHttp();
    xmlHttp.onreadystatechange = OnReadyStateChange;
    xmlHttp.open("post", url, false);
    xmlHttp.send("");
}
function OnReadyStateChange() {
    //判断读卡状态，执行相应操作
    if (xmlHttp.readyState == 4) {       //有新卡
        if (xmlHttp.responseText == "True") {
            $.get("Handler1.ashx", {}, function (data) {
                if (data.indexOf('mporaryCharge.aspx') > 0) {
                    window.location.href = "/showfood/" + data;
                    return;
                }
                if (data.indexOf('是否要加餐') > 0) {
                    clearInterval(timer);
                }
                document.getElementById("content").innerHTML = data;
                $("#jiaqian").focus().val('0').select();
            });
        }
    }
}
var xmlHttp;
var interval = 1000;     //开timer刷卡
```
看了这里面的xmlhttp的方法open了，send了，什么的还有要设置url，是不是和Ajax的调用的方法很像？我们在XMLHttpRequest里面的使用其实就是AJAX的基本方式，只不过Ajax给我们封装了起来了，所以知道原理很是重要：
百度百科：[http://baike.baidu.com/link?url=M4vQcUgZ0nhOfADA5g11cVPgH-O_SOJ_YjDfQ1xrC558FKb7HhDTCDccPFRJaBAo8R6u8iCwYHHMn3WuidMrkq](http://baike.baidu.com/link?url=M4vQcUgZ0nhOfADA5g11cVPgH-O_SOJ_YjDfQ1xrC558FKb7HhDTCDccPFRJaBAo8R6u8iCwYHHMn3WuidMrkq)
# 二、Ajax的使用方法（一）
       说道Ajax的使用方法，相信不同的人有不同的理解，但是最最常用的应该就是直接写个function，然后再套一个ajax的方法了吧（用jqurey）：
```java
$.ajax({
     type: 'POST',     //操作类型
     url: url ,       //函数地址
    data: data ,      //传递的数据
    success: success ,    //执行结果
    dataType: dataType     //数据流类型
});
```
        以上就是Ajax常用的书写格式了，其中type可以换为get，一般不需要参数，只要返回值的时候可以使用。值得注意的是，我们写url的时候一定是路径后面加上方法名，并且一般以静态方法为主，还要在方法前加上[WebMethod]才可以调用，并且返回的类型是以datatype为主的，多数情况都是json的类型集合。
# 三、Ajax的使用方法（二）
         还有一种方法是可以调用非静态方法的，就是直接写"$.post"或者“$.get”，这样可以很方便的调用以及传参：
JS中写的方法：
```java
$.post('/mobile/mobileChangePwd.aspx', { oldPWD: $("#txtOldPwd").val(), newPWD: $("#txtNewPwd").val(), conPWD: $("#txtConfirmPwd").val() }, function (data) {
          if (data == "修改密码成功！") {
             document.getElementById("txtOldPwd").value = "";
             document.getElementById("txtNewPwd").value = "";
             document.getElementById("txtConfirmPwd").value = "";
          }
       });
```
后台C#：
```
protected void Page_Load(object sender, EventArgs e)
{
    if (Request["oldPWD"] != null) //判读参数的值是否为空
    {    //传递参数，调用方法          
        string result = confirmabc(Request["oldPWD"].ToString(), Request["newPWD"].ToString(), Request["conPWD"].ToString());
        if (result == "" || result =="true")
        {
            Response.Write("修改密码成功！");  //返回结果
            Response.End();       //截断返回
         }
         else
         {
            Response.Write(result);
            Response.End();
          }
          return;
      }
}
```
        这样写的好处就是可以直接传递参数，并且可以和asp.net的控件混合使用，以达到我们想要的功能，非常的简单，执行起来也非常的有效。
        需要注意的是，如果要返回值的话，一定要写Response.End()方法，这样才会获得需要返回的值，不然他也会把整个页面都返回。
# 四、Ajax模板
        其实如果想用原汁原味的Ajax的话，就需要知道Ajax模板的这个存在，也就是XMLHttpRequest的写作方法，这一点非常有用，我们可以很好的理解Ajax的原理：
```java
<script type="text/javascript">
        var xmlHttp;
        function creatXMLHttpRequest() {
            if (window.ActiveXObject) {
                xmlHttp = new ActiveXObject("Microsoft.XMLHttp");
            }
            else if (window.XMLHttpRequest) {
                xmlHttp = new XMLHttpRequest();
            }
        }
        function startRequest() {
            creatXMLHttpRequest();
            xmlHttp.onreadystatechange = handleStateChange;
            xmlHttp.open("GET", "simpleResponse.xml", true);
            xmlHttp.send(null);
        }
        function handleStateChange() {
            if (xmlHttp.readyState == 4) {
                if (xmlHttp.status == 200) {
                    // document.getElementById("results").innerHTML = xmlHttp.responseText; 
                    // alert("The server replied with:" + xmlHttp.responseText); 
                }
            }
        }
    </script>
```
        其实这样的模板有很多，大家可以去网上 尝试搜索一下。
