# js模拟showmodaldialog，遮盖层及弹出div - z69183787的专栏 - CSDN博客
2012年11月12日 21:39:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1859
```
<script type="text/javascript" src="jquery-1.4.4.min.js"></script>
<script>
function divPosition(div){
	var top = ($(document).height() - $(div).height())/2;
	var left = ($(document).width() - $(div).width())/2;
	
	$(div).css("left",left+"px");
	$(div).css("top",top+"px");
}
function a(){
	$("#zs").animate({
	"opacity":"show"
	},
	{
	duration:500,
	complete:function(){
		divPosition("#op");
		$("#op").animate({
			"opacity":"show"
		},
		{
		duration:500,
		complete:function(){
		pageWidth = ($.browser.version=="6.0")?$(document).width()-21:$(document).width();
		pageHeight = $(document).height();
		$("#zs").css("height",pageHeight).css("width",pageWidth);
                var offsetHeight = (document.body.clientHeight - $(div).height()) / 2
                if (offsetHeight < 0) offsetHeight = 0;
                ScrollTop = $(div).offset().top - offsetHeight;
                $(document).scrollTop(ScrollTop);
		}
	})
	
}
})
}
</script>
<body>
<div id="zs" style="position:absolute;left:0;top:0;z-index:999;
display:none;filter:alpha(opacity=30);opacity:0.3;-moz-opacity:0.3;width:100%;height:100%;background-color:#000;">
</div>
<div id="op" style="position:absolute;display:none;border:1px yellow solid;z-index:9999">
操作中请稍后
</div>
<input type="button" value="dddddd" onclick="a();">
</body>
```
![](https://img-my.csdn.net/uploads/201211/12/1352728165_6376.jpg)
