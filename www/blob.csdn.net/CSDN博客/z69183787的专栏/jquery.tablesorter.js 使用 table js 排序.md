# jquery.tablesorter.js 使用 table js 排序 - z69183787的专栏 - CSDN博客
2013年11月08日 13:59:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3233
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
<script type="text/javascript" src="jquery-1.7.1.js"></script>
<script type="text/javascript" src="jquery.tablesorter.js"></script>
<script>
	$(document).ready(function(){
		//header headerSortUp   headerSortDown   默认点击更改class  配置即可
		
			//$("#mytable").tablesorter();
			//$("#mytable").tablesorter({headers:{5:{sorter:false}}});
			//$('#mytable').tablesorter({ sortList:[[0,0],[1,0]] });. 初始化排序
			$('#mytable').tablesorter({ sortList:[[0,0]] });
		})
	
</script>
<style>
.headerSortDown {
    background: url("http://w1.goalhi.com.cn/images/gamespace/headerSortDown.gif") no-repeat scroll center center transparent;
    color: #FFFFFF;
    cursor: pointer;
    line-height: 30px;
}
.headerSortUp {
    background: url("http://w1.goalhi.com.cn/images/gamespace/headerSortUp.gif") no-repeat scroll center center transparent;
    color: #FFFFFF;
    cursor: pointer;
    line-height: 30px;
}
</style>
</head>
<body>
	
	<table id="mytable" border="1">
<thead>
<tr><th>编号</th><th>姓名</th><th>性别</th><th>年龄</th><th>年龄等级</th><th>地址</th><th>电话</th><th>金钱</th></tr>
</thead>
<tbody>
<tr><td>1</td><td>a sdf</td><td>男</td><td>22</td><td>青年</td><td>啊北京</td><td>010123456</td><td>￥2,000</td></tr>
<tr><td>2</td><td>d wr</td><td>男</td><td>10</td><td>少年</td><td>不广州</td><td>020123456</td><td>￥5,000</td></tr>
<tr><td>3</td><td>e vcv</td><td>女</td><td>35</td><td>中年</td><td>才上海</td><td>021123456</td><td>￥1,030</td></tr>
<tr><td>11</td><td>b sdewr</td><td>女</td><td>67</td><td>老年</td><td>的天津</td><td>022123456</td><td>￥1,000.1</td></tr>
<tr><td>5</td><td>c sdfd</td><td>男</td><td>40</td><td>中年</td><td>额重庆</td><td>023123456</td><td>￥3,000</td></tr>
</tbody>
</table>
</body>
</html>
```
