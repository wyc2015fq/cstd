# js 滚动加载iframe框中内容 - z69183787的专栏 - CSDN博客
2013年08月16日 15:02:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1494
```java
var isIE6 = !!window.ActiveXObject&&!window.XMLHttpRequest;
	//滚动加载
	var scrollLoad =function(){
		$("#content iframe[_src]").each(function(){
				var t = $(this);
				if( t.offset().top<= $(document).scrollTop() + $(window).height()  )
				{
					t.attr( "src",t.attr("_src") ).removeAttr("_src");
				}
		});//each E
	}
	scrollLoad();
	$(window).scroll(function(){ 
		if(isIE6){ btb.css("top", $(document).scrollTop()+30) }
		//scrollLoad();
	});
```
`<iframe allowTransparency="true" scrolling="no" _src="demo/iframe/2.0-focus-iframe.html?&fade&true" frameborder="0"></iframe>`
