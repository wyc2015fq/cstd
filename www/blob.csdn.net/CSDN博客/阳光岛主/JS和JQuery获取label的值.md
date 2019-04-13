
# JS和JQuery获取label的值 - 阳光岛主 - CSDN博客

2014年08月30日 06:19:34[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：31996


**label**
<label name="label_blog" id="label_blog" value="http://blog.ithomer.net">
	http://blog.ithomer.net
</label>
</br>
<input name="input_blog" id="input_blog" value="http://blog.ithomer.net">
</input>
**label 获取值：**
label标签在JS和Jquery中使用不能像其他标签一样用value获取它的值
// input get value
var input = document.getElementById("input_blog");
var value = input.value;						// "http://blog.ithomer.net"
var value2 = $("\#input_blog").val();			// "http://blog.ithomer.net"
// error label get value
var label = document.getElementById("label_blog");
var value = label.value;						// undefined
var value2 = $("\#label_blog").val();			// ""
**1）JS 正确获取：**
// js label get value
var label = document.getElementById("label_blog");
var value = label.innerText.trim();				// "http://blog.ithomer.net"
**2）JQuery 正确获取：**
// jquery label get value
var value = $("\#label_blog").html().trim();		// "http://blog.ithomer.net"
**label 赋值：**
// set label value
var label = document.getElementById("label_blog");
label.innerText="http://proxy.ithomer.net";				// js set value
$("\#label_blog").html("http://proxy.ithomer.net");		// jquery set value
**input 赋值：**
// set input value
var input = document.getElementById("input_blog");
input.value="http://proxy.ithomer.net";					// js set value
$("\#input_blog").val("http://proxy.ithomer.net");		// jquery set value
**完整测试代码：**
<!DOCTYPE html>
<html>
<head>
	<title>demo of label value</title>
</head>
<body>
	<label name="label_blog" id="label_blog" value="http://blog.ithomer.net">
		http://blog.ithomer.net
	</label>
	</br>
	<input name="input_blog" id="input_blog" value="http://blog.ithomer.net">
	</input>
<!--
	<label name="label_blog" id="label_blog"  style="display:none;" value="http://blog.ithomer.net">
		http://blog.ithomer.net
	</label>
-->	
	<script src="http://ajax.aspnetcdn.com/ajax/jQuery/jquery-2.1.1.min.js"></script>
	<script type="text/javascript">
		// input get value
		var input = document.getElementById("input_blog");
		var value = input.value;						// "http://blog.ithomer.net"
		var value2 = $("\#input_blog").val();			// "http://blog.ithomer.net"
		// error label get value
		var label = document.getElementById("label_blog");
		var value = label.value;						// undefined
		var value2 = $("\#label_blog").val();			// ""
		// js label get value
		var label = document.getElementById("label_blog");
		var value = label.innerText.trim();				// "http://blog.ithomer.net"
		// jquery label get value
		var value = $("\#label_blog").html().trim();		// "http://blog.ithomer.net"

		// set input value
		var input = document.getElementById("input_blog");
		input.value="http://proxy.ithomer.net";					// js set value
		$("\#input_blog").val("http://proxy.ithomer.net");		// jquery set value
		// set label value
		var label = document.getElementById("label_blog");
		label.innerText="http://proxy.ithomer.net";				// js set value
		$("\#label_blog").html("http://proxy.ithomer.net");		// jquery set value
	</script>
</body>
</html>

