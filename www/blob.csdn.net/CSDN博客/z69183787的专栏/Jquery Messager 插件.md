# Jquery Messager 插件 - z69183787的专栏 - CSDN博客
2012年11月05日 15:04:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1321
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>MESSAGER插件</title>
<script src="jquery-1.2.6.pack.js"></script>
<script src="jquery.messager.js"></script>
<script>
$(document).ready(function(){
	$.messager.show(0,'送你一个Jquery Messager消息弹出插件！');
	$("#showMessager300x200").click(function(){
		$.messager.lays(300, 200);
		$.messager.show(0, '300x200的消息');
	});
	$("#showMessagerFadeIn").click(function(){
		$.messager.anim('fade', 2000);
		$.messager.show(0, 'fadeIn动画消息');
	});
	$("#showMessagerShow").click(function(){
		$.messager.anim('show', 1000);
		$.messager.show(0, 'show动画消息');
	});
	$("#showMessagerDim").click(function(){
		$.messager.show('<font color=red>自定义标题</font>', '<font color=green style="font-size:14px;font-weight:bold;">自定义内容</font>');
	});
	$("#showMessagerSec").click(function(){
		$.messager.show(0, '一秒钟关闭消息', 1000);
	});
	$("#showMessagerNoClose").click(function(){
		$.messager.show('不会关闭的消息', '要自己点关闭的X才可以哦！', 0);
	});
});
</script>
<style type="text/css">
<!--
body,td,th { font-size: 12px; }
body { background-color: #fefefe; }
p	{ width:80%; height:auto; padding:10px; background-color:#D6F097; border:solid 1px #FF9900; color:#333; margin-left:auto; margin-right:auto;}
input	{ background-color:#F5D99E; color:#333; border:solid 1px #993300; font-size:12px;}
p.new input	{ background-color:#FF0000; color:#fff;}
-->
</style></head>
<body>
<h2>JQUEYR PLUGIN - Messager v1.5版权所有 © <a href="http://www.corrie.net.cn" target="_blank">www.corrie.net.cn</a>赵戈</h2>
<p>
<b>版本1.5更新：</b><Br />
1、修正设置不自动关闭时，鼠标over后直接关闭的bug<br />
2、修正页面有滚动条时，弹出位置不在最下方的bug<br />
3、增加了随屏移动效果<br />
<b>版本1.4更新：</b><Br />1、支持鼠标over时不关闭消息提示框<br />
<b>版本1.3更新：</b><br />
1、可以设置不自动关闭，停留在固定位置。<br />
2、增加了对UA的判断能力和效果解析<br />
3、针对chrome下的错位给予了一定的修补，目前效果还算可以<br />
4、针对代码段做了调整优化<br /></p>
<p><input type="button" id="showMessager300x200" value="显示一个300x200的消息" /></p>
<p><input type="button" id="showMessagerFadeIn" value="显示一个fadeIn动画消息" /></p>
<p><input type="button" id="showMessagerShow" value="显示一个show动画消息" /></p>
<p><input type="button" id="showMessagerDim" value="显示定义内容和标题消息" /></p>
<p><input type="button" id="showMessagerSec" value="一秒钟关闭消息" /></p>
<p class="new"><input type="button" id="showMessagerNoClose" value="不自动关闭消息" /></p>
<div><a href="./jquery.messager.js">JS文件下载</a> | <a href="./jquery.messager.rar">完整包下载</a><br />
讨论技术和有任何问题请发邮件，非诚勿扰！corrie#sina.com</div>
<div align="center">
<script type="text/javascript" src="http://js.tongji.linezing.com/724202/tongji.js"></script><noscript><a href="http://www.linezing.com"><img src="http://img.tongji.linezing.com/724202/tongji.gif"/></a></noscript>
 </div>
<p style="height:500px; background:none; border:none;"> </p>
</body>
</html>
```
``
效果为窗口右下角弹出 提示信息
