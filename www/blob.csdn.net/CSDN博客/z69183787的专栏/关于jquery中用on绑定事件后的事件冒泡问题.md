# 关于jquery中用on绑定事件后的事件冒泡问题 - z69183787的专栏 - CSDN博客
2014年12月17日 16:14:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13497
用on绑定时，我把子元素的 绑定到 document，而把父元素绑定到上级元素，导致 return false 阻止冒泡无效。
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>事件冒泡</title>
    <script src="jquery-1.7.1.js" type="text/javascript"></script>
    <script language="javascript" type="text/javascript">
        $(function () {
            $(document).on("click","#p1",function(e){
            	
            	console.log(e.target.tagName);
            	console.log("p1被点击了");
                //e.stopPropagation(); //终止冒泡的方法
                return false;
            	})
            	
            	$("#aa").on("click","#td1",function(e){
            		console.log(e.target.tagName);
            	console.log("td1被点击了");
            	})
            	
            	$("#aa").on("click","#tr1",function(e){
            		console.log(e.target.tagName);
            	console.log("tr1被点击了");
            	})
            	
            	$("#aa").on("click","#table1",function(e){
            		console.log(e.target.tagName);
            	console.log("table1被点击了");
            	})
           
        });
    </script>
</head>
<body id="aa">
<table onclick="alert('这是table')">
    <tr onclick="alert('这是tr')">
        <td onclick="alert('这是td')">
            <p onclick="alert('这是p')">段落</p>
        </td>
    </tr>
</table>
<table id="table1">
    <tr id="tr1">
        <td id="td1">
            <p id="p1">你好</p>
        </td>
    </tr>
</table>
</body>
</html>
```
on方法 将click等事件绑定在document对象上，页面上任何元素发生的click事件都冒泡到document对象上得到处理。
增加了绑定效率。当事件冒泡到document对象时，检测事件的target，如果与传入的选择符（这里是button）匹配，就触发事件，否则不触发。
修改为统一绑定对象后即解决，初步认为是因为 on方法的绑定机制问题。
所以return false 无效。子元素和父元素修改为相同 绑定元素后，问题解决
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>事件冒泡</title>
    <script src="jquery-1.7.1.js" type="text/javascript"></script>
    <script language="javascript" type="text/javascript">
        $(function () {
            $("#aa").on("click","#p1",function(e){
            	
            	console.log(e.target.tagName);
            	console.log("p1被点击了");
                //e.stopPropagation(); //终止冒泡的方法
                return false;
            	})
            	
            	$("#aa").on("click","#td1",function(e){
            		console.log(e.target.tagName);
            	console.log("td1被点击了");
            	})
            	
            	$("#aa").on("click","#tr1",function(e){
            		console.log(e.target.tagName);
            	console.log("tr1被点击了");
            	})
            	
            	$("#aa").on("click","#table1",function(e){
            		console.log(e.target.tagName);
            	console.log("table1被点击了");
            	})
           
        });
    </script>
</head>
<body id="aa">
<table onclick="alert('这是table')">
    <tr onclick="alert('这是tr')">
        <td onclick="alert('这是td')">
            <p onclick="alert('这是p')">段落</p>
        </td>
    </tr>
</table>
<table id="table1">
    <tr id="tr1">
        <td id="td1">
            <p id="p1">你好</p>
        </td>
    </tr>
</table>
</body>
</html>
```
