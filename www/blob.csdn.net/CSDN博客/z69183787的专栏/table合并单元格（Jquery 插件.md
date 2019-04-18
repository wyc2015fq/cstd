# table合并单元格（Jquery 插件 - z69183787的专栏 - CSDN博客
2013年12月04日 10:58:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1995
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>table合并单元格（Jquery 插件）</title>
<script src="http://code.jquery.com/jquery-latest.js"></script>
<style type="text/css">
	table{ border-collapse:collapse;}
	table th{ background:#ddd; }
	table td,table th{ padding:2px 10px;border:1px solid #999;}
</style>
</head>
<body>
<table id="my-table">
	<thead>
        <tr>
            <th>姓名</th>
            <th>学科</th>
            <th>学分</th>
        </tr>
    </thead>
    <tbody>
    	<tr>
            <td>小铭</td>
            <td>数学</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小铭</td>
            <td>英语</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小铭</td>
            <td>化学</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小铭</td>
            <td>美术</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小红</td>
            <td>数学</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小红</td>
            <td>英语</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小红</td>
            <td>化学</td>
            <td>100</td>
        </tr>
        <tr>
            <td>小红</td>
            <td>美术</td>
            <td>100</td>
        </tr>
    </tbody>
</table>
<script type="text/javascript">
!function($){
	$.fn.rowspan=function(options){
		var defaults = {}
        var options = $.extend(defaults, options);
		this.each(function () {
			
			var tds=$(this).find("tbody td:nth-child("+options.td+")");
			var current_td=tds.eq(0);
			var k=1;
			tds.each(function(index, element) {
				if($(this).text()==current_td.text()&&index!=0){
					k++;
					$(this).remove();
					current_td.attr("rowspan",k);
					current_td.css("vertical-align","middle");
				}else{
					current_td=$(this);
					k=1;
				}
			});
			
		})
	}
}( window.jQuery );
$("#my-table").rowspan({td:1});
</script>
</body>
</html>
```
