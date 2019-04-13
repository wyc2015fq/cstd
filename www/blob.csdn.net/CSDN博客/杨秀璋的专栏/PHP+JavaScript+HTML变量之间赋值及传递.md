
# PHP+JavaScript+HTML变量之间赋值及传递 - 杨秀璋的专栏 - CSDN博客

2015年02月20日 16:23:12[Eastmount](https://me.csdn.net/Eastmount)阅读数：30784标签：[PHP																](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)[JavaScript																](https://so.csdn.net/so/search/s.do?q=JavaScript&t=blog)[Html																](https://so.csdn.net/so/search/s.do?q=Html&t=blog)[变量赋值																](https://so.csdn.net/so/search/s.do?q=变量赋值&t=blog)[Ajax																](https://so.csdn.net/so/search/s.do?q=Ajax&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=变量赋值&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Html&t=blog)个人分类：[PHP基础知识																](https://blog.csdn.net/Eastmount/article/category/1644197)
[
																					](https://so.csdn.net/so/search/s.do?q=Html&t=blog)所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)[
							](https://so.csdn.net/so/search/s.do?q=Html&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=JavaScript&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)

本文是最近做WAMP网站的学习知识，这做这个网站过程中需要通过新闻通告的超链接显示相应的具体内容，所以就涉及到一些相关变量赋值传递的内容，包括：HTML超链接传递值通过JavaScript显示、JavaScript变量转换成PHP变量、超链接实现传递给PHP连接数据库(重点)、PHP输出JavaScript内容。

## 一. HTML超链接传递值
首先讲述通过HTML超链接<A href=></A>实现跳转，再通过JavaScript实现获取传递的值。代码如下：其中testA.php是超链接，testB.php是获取的传递值。

```python
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>值传递</title>
</head>
<body>
<?php
	$result=10001;
?>
<A href="testB.php?newid=<?php echo $result; ?>" >跳转链接</A>
</body>
</html>
```
testB.php代码如下：

```python
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>µȂ¼½脦</title>
</head>
<body>
<input id="new_id" type="hidden">
<script language=javascript runat="server">
	var src = document.getElementById("new_id").value = location.href;
	//alert(src);
	var params = src.split('?');
	//输出newid=10001中的id
	if(params[1]) {
		var idparams = params[1].split('=');
	}
</script>
<P>输出src完整路径：<script>document.write(src);</script>
<P>输出params[1]获取参数：<script>document.write(params[1]);</script>
<P>输出id值：<script>document.write(idparams[1]);</script>
</body>
</html>
```
其中核心代码是：
<A href="testB.php?newid=<?php echo $result; ?>" >跳转链接</A>
设置URL链接，需要注意的是HTML中嵌套PHP的变量，同时可以与PHP中SQL查询赋值比较下：
$result=$hgi->doquery('2',"select * from Course_News where CN_Nid='".$_GET['newid']."';");
然后在testB.php中通过JavaScript显示其值：
var src = document.getElementById("new_id").value = location.href;
<script>document.write(src);</script>
![](https://img-blog.csdn.net/20150220140101221)

## 二. JavaScript变量转换PHP值
此时已经通过获取了var idparams[1] = 10001,但是我想实现的效果是通过id数据库主键，获取数据库的内容，而数据库的那部分代码如下：
$result=$hgi->doquery('2',"select * from Course_News where CN_Nid='".$id."';");
所以只需要把$id赋值为JavaScript的var型变量即可。引用mingfish2 发表的贴“怎么将js变量赋值给php?”
![](https://img-blog.csdn.net/20150220150350640)
由于PHP或Ajax学习较少，所以我没有实现，仅引用别人的方法和思想。通常采用的方法如下：
1.通过Ajax实现js变量赋值给PHP(引用：[http://www.xgllseo.com/?p=3241](http://www.xgllseo.com/?p=3241))
```python
JS部分代码
$('button').click(function(){
var js2_101=30;
$.ajax({
type:'GET',
url:"3.php",
data:{text:js2_101},
success: function(data){
alert(data)
}
}); 
return false;
});
PHP部分代码
$m2_10=$_GET['text'];
$php2=10;
echo $m2_10+$php2."<---JS赋值给php，可以在PHP中进行编程";
```
此处我更想分享的是它的一些原理知识：
JS变量赋值给PHP时，你需要知道JS是客户端语言可以直接在浏览器上运行，PHP是服务器语言在后台运行，所以JS变量不能直接赋值给PHP。那么怎么样知道什么是服务器语言什么是客户端语言呢？其实查看网页的源代码我们会发现可以直接查看到JS的源代码而PHP的不能查看源代码只能把结果输出到页面上。为了实现这效果，这里使用ajax，定义一个JS变量，将变量赋值给data，再将data的参数赋值给php变量。
而PHP能赋值给JS是因为PHP是在服务器端被解析。服务器端不会解析JS代码，JS代码和HTML代码在服务器端都是原样输出的。只有标注php能识别的位置，PHP才会去进行解析并执行。而JS则是到了客户端才被客户的浏览器解析。
如果需要把JS的变量“赋值”给PHP，这就涉及交互了。就是所谓的提交，因为但凡你在浏览器看到的页面，无论是PHP,ASP,JSP等等，都是HTML文档，本地需要修改服务器端的数据，当然是要跟服务器进行交互。而进行交互的中间人就是JS，JS搭起客户与服务器交互的桥梁。
2.通过表单GET或POST方法获取值
(参考：[http://jingyan.baidu.com/article/fd8044faf3cda05030137a47.html](http://jingyan.baidu.com/article/fd8044faf3cda05030137a47.html))
通过Javascript将客户端要传递的数据先交给表单中的一个隐匿的标签中，然后以提交表单的形式，PPHP通过POST方法获取该数据。
```python
1)mainfunc.js代码：
function func(){
var a = "Javascript变量";
document.getElementById("test").value = a;
}
2)test.php代码。
<?php 
if(isset($_POST['sub'])){
$test = $_POST['test'];
echo "<br>".$test."----php变量显示";
}
?>
<form action="" method="post">
<input type="hidden" id="test" name="test"/>
<input type="submit" id="sub" value="变量转换" />
</form>
```

## 三. 超链接实现传递给PHP连接数据库
其实上面“一二”讲述的就是如何实现过程如下：
HTML=>超链接=>JavaScript=>PHP=>数据库
由于Ajax没有实现，所以我后来发现一个更简单的方法，就是在超链接设置参数的时候，其实就可以直接获取相关参数，并不需要JavaScript去处理字符串获取相应的值。代码如下：
//testA.php设置超链接
<A href="main_right_yk4-1-content.php?newid=<?php echo $id; ?>">超链接</A>
//testB.php中获取数据库内容
$hgi=new HttpPostInf();
$result=$hgi->doquery('2',"select * from Course_News whereCN_Nid='".$_GET['newid']."';");
//解析json
$dj=json_decode($result,true);
后面的文章我会讲述PHP和数据库的相关知识，此处你只需要知道如何获取连接即可。由于项目是我和另一个同学完成，他把后台数据库操作都用PHP类封装长好了（采用新浪SAE），所以我前端调用非常方便（采用apache）。该部分可以实现的初步效果如下图所示：
![](https://img-blog.csdn.net/20150220160707455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150220160627251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 四. PHP输出JavaScript内容

该部分比较简单，将PHP变量放在HTML的隐匿类型的标签中，然后Javascript通过document.getElementById("").value来获取标签的值，即可得到PHP的变量值。也可以直接进行php与js变量的赋值。代码如下：
```python
<script>                 //PHP变量能赋值给JS编程运算
<?php $m=10; ?>          //php的变量$m
var js = 20;             //js变量js
var n=<?php echo $m; ?>; //php值赋值给js变量n
document.write(js+n);    //结果：php的值能赋值给JS变量，并且可以在JS程序中运用
</script>
```
该代码输出的结果为30。另一种代码如下：
```python
<?php
   $test = "It's a test!"；
?>
<input type="hidden" id="test" name="test" value="<?=$test;?>"  />
<script>
   var test_js = document.getElementById("test").value;
</script>
<?php
    //其它处理
?>
```
最后希望文章对大家有所帮助，下一篇想写写WAMP关于数据库方面的知识（Window+Apache+MySQL+PHP）。
(By:Eastmount 2015-2-20 下午4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



