# jquery 在textarea 指定 位置处 插入内容 - z69183787的专栏 - CSDN博客
2013年12月18日 12:45:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4347
```
<!DOCTYPE html>
<html>
<head>
<meta charset=utf-8 />
<title>Sliding Menu Effect</title>
<link rel="stylesheet" type="text/css" href="style2.css" />
<script type="text/javascript" language="javascript" src="jquery-1.5.1.min.js"></script>
<script type="text/javascript">
	
	(function($) {http://www.css88.com/archives/3627
    $.fn.insertContent = function(myValue, t) {
		var $t = $(this)[0];
		if (document.selection) { //ie
			this.focus();
			var sel = document.selection.createRange();
			sel.text = myValue;
			this.focus();
			sel.moveStart('character', -l);
			var wee = sel.text.length;
			if (arguments.length == 2) {
				var l = $t.value.length;
				sel.moveEnd("character", wee + t);
				t <= 0 ? sel.moveStart("character", wee - 2 * t - myValue.length) : sel.moveStart("character", wee - t - myValue.length);
 
				sel.select();
			}
		} else if ($t.selectionStart || $t.selectionStart == '0') {
			var startPos = $t.selectionStart;
			
			var endPos = $t.selectionEnd;
			var scrollTop = $t.scrollTop;
			$t.value = $t.value.substring(0, startPos) + myValue + $t.value.substring(endPos, $t.value.length);
			this.focus();
			$t.selectionStart = startPos + myValue.length;
			$t.selectionEnd = startPos + myValue.length;
			$t.scrollTop = scrollTop;
			if (arguments.length == 2) {
				$t.setSelectionRange(startPos - t, $t.selectionEnd + t);
				this.focus();
			}
		}
		else {
			this.value += myValue;
			this.focus();
		}       
    };
})(jQuery);
$(function(){
	
	
	$(".cont").click(function(){
		$(".tarea").insertContent("12321321321");
	//	$(".tarea").insertContent("12321321321",3);
	});
});
</script>
</head>
<body><br />
<br />
<div class="cont">点击我</div><br />
<br />
<textarea class="tarea" name="" cols="50" rows="15">
创作、游戏开发、运营管理等领域的精英人才，并拥有国内多家教育科研机构的战略合作资源。
 
　　聚网科技以“为儿童提供真正健康有益的互联网产品”为己任，在产品研发过程中，注重将能力教育融入全新的游戏玩法，不断探索更加受儿童欢迎、更加有益于儿童的多元化娱乐模式，在行业里形成了“锐意进取、不断创新”的良好形象，也使得公司产品一直在行业内保持
</textarea>
</body>
</html>
```
