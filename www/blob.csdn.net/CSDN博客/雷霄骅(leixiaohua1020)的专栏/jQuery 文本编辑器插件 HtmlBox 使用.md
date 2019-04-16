# jQuery 文本编辑器插件 HtmlBox 使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月10日 17:12:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27









jQuery 插件 HtmlBox 使用方式如下：

1.引入头文件（注意一定要把jQuery放在前面）：



```
<script src="lib/jquery-1.8.3.min.js"></script>
	<script type="text/javascript" src="lib/htmlbox/htmlbox.colors.js"></script>
	<script type="text/javascript" src="lib/htmlbox/htmlbox.styles.js"></script>
	<script type="text/javascript" src="lib/htmlbox/htmlbox.syntax.js"></script>
	<script type="text/javascript" src="lib/htmlbox/htmlbox.undoredomanager.js"></script>
	<script type="text/javascript" src="lib/htmlbox/htmlbox.min.js"></script>
```


 2.在需要使用HtmlBox的<textarea>后面加入javascript代码：





```
<textarea id="htmlbox_icon_set_green" class="text_area2" cols="32" rows="3" name="content"></textarea>
                  <script language="Javascript" type="text/javascript">
var hb_icon_set_green = $("#htmlbox_icon_set_green").css("height","400").css("width","583").htmlbox({
    toolbars:[
	     ["cut","copy","paste","separator_dots","bold","italic","underline","strike","sub","sup","separator_dots","undo","redo","separator_dots",
		 "left","center","right","justify","separator_dots","ol","ul","indent","outdent","separator_dots","link","unlink","image"],
		 ["code","removeformat","striptags","separator_dots","quote","paragraph","hr","separator_dots",
			 {icon:"new.gif",tooltip:"New",command:function(){hb_icon_set_green.set_text("<p></p>");}},
			 {icon:"open.gif",tooltip:"Open",command:function(){alert('Open')}},
			 {icon:"save.gif",tooltip:"Save",command:function(){alert('Save')}}
		  ]
	],
	icons:"default",
	skin:"green"
});
</script>
```


 原本的<textarea>就会变成一个功能强大的编辑器。



![](https://img-blog.csdn.net/20131010171225671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

htmlbox下载地址：[http://download.csdn.net/detail/leixiaohua1020/6376479](http://download.csdn.net/detail/leixiaohua1020/6376479)




