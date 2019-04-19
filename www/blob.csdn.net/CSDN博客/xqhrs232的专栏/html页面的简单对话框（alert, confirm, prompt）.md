# html页面的简单对话框（alert, confirm, prompt） - xqhrs232的专栏 - CSDN博客
2018年11月08日 10:04:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：132
原文地址::[https://blog.csdn.net/ClementAD/article/details/46763139](https://blog.csdn.net/ClementAD/article/details/46763139)
html页面简单的三种对话框如下：
1、alert()，最简单的提示框：
alert("你好！");
2、confirm()，有确认和取消两个按钮：
if(confirm("还有继续吗？")){
    alert("继续");
}else{
    alert("再见");
}
3、prompt()，可以输入信息：
var name = prompt("你的名字是：");
alert("你好，" + name);
完整的html代码：
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="content-type" content="text/html; charset=UTF-8">
    <title>对话框测试</title>
    <script>
        function confirmDialog(){
            if(confirm("还要继续吗？")){
                alert("继续");
            }else{
                alert("再见");
            }
        }
        function promptDialog(){
            var name = prompt("你的名字是：");
            alert("你好，" + name);
        }
    </script>
  </head>
<body>
    对话框测试：
    <div style="margin-top:50px">
        <input type="button" value="alert" onclick="alert('你好！')" />
        <input type="button" value="confirm" onclick="confirmDialog()" />
        <input type="button" value="prompt" onclick="promptDialog()" />
    </div>
</body>
</html>
