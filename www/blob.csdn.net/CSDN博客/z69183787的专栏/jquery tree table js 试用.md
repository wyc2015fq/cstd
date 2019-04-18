# jquery tree table js 试用 - z69183787的专栏 - CSDN博客
2013年11月18日 19:39:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9233
**用完以后感觉是一个简单的树形显示控件，当然也可以自己改为ajax异步加载类型，**
**由于自身API 有限 ，可用于一些简单功能的展现。**
**官网地址：**
**[http://ludo.cubicphuse.nl/jquery-treetable/#api](http://ludo.cubicphuse.nl/jquery-treetable/#api)**
```
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
<link href="./tree/screen.css" rel="stylesheet"  media="screen" />
	<link href="./tree/jquery.treetable.css" rel="stylesheet" type="text/css">
	<link href="./tree/jquery.treetable.theme.default.css" rel="stylesheet" type="text/css">
<script src="jquery.js" type="text/javascript"> </script> 
<script src="./tree/jquery.treetable.js" type="text/javascript"> </script>
<script type="text/javascript">
        $(function(){
	
	 //$("#treeTable1").treetable({onNodeExpand:function(){alert(1);}});	
          $("#treeTable1").treetable({ expandable: true });	
		  $("#treeTable1").treetable("expandAll");	
						
        });
		function setValue1(){
			
			var nameValue='';
			var idValue='';
			$('#treeTable1').find('input[type="checkbox"]:checked').each(function(){
				nameValue += $(this).parent().text()+',';
				idValue += $(this).val()+',';
			});
			
			$("#a").val(nameValue.substr(0,nameValue.length-1));
			$("#b").val(idValue.substr(0,idValue.length-1));
			//window.close();
		
		}
    </script>
	</head>
	<body>
	<table id="treeTable1" style="width: 100%">
               <tr data-tt-id="0">
			   
          <td><input type="checkbox" value="0">app</td>
        </tr>
        <tr data-tt-id="1" data-tt-parent-id="0">
          <td><input type="checkbox" value="1">controllers</td>
        </tr>
        <tr data-tt-id="5" data-tt-parent-id="1">
          <td><input type="checkbox" value="5">application_controller.rb</td>
        </tr>
        <tr data-tt-id="2" data-tt-parent-id="0">
          <td><input type="checkbox" value="2">helpers</td>
        </tr>
        <tr data-tt-id="3" data-tt-parent-id="0">
          <td><input type="checkbox" value="3">models</td>
        </tr>
        <tr data-tt-id="4" data-tt-parent-id="0">
          <td><input type="checkbox" value="4">views</td>
        </tr>
            </table>
<input type="text" id="a"/><input type="text" id="b"/>
<button onclick="setValue1();" value="选择">
			</body>
```
效果：
![](https://img-blog.csdn.net/20131118193853781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
