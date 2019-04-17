# 翻版HeadFirstJS例子，日记本的实现 - 长歌行 - CSDN博客





2013年03月24日 23:36:40[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1692








可耻的盗版了。。

HeadfirstJs全书的配套代码可以在csdn等各处下载到。

本处仅仅为手打练习。。用于记载练习进度



```
<!-- 1.使用构造函数创建类的实例 -->
<!-- 2.调用函数遍历展示类的实例 -->
<!-- 3.增加一个全部显示的按钮 -->
<!-- 4.修改了显示风格 -->
<!-- 5.使用Date类进行时间显示,使用类Date，并逐一获取年月日 -->
<!-- 6.增加了按时间排序功能 使用sort函数使用自己的函数指针进行排序，将日期从大到小排列-->

<html>
<head>
	<title>Share Index of BenBen</title>
	<script type = "text/javascript">
	
	function Test(){
	    alert("Hello ,you sucessful open the window!");
	}

	// 构造分享目录类
	function Diary(date, content) {
	    this.date       = date;
	    this.content    = content;
	}

	//构造条目
	var index = [ new Diary(new Date("3/23/2013"), "A good day!"),
	              new Diary(new Date("3/24/2013"), "Want to sleep!"),
	              new Diary(new Date("3/25/2013"), "Work all day!")];

	//显示数据
	function ShowItem(num) {
	    if (!num) {
	        num = index.length;
	    }
	    index.sort(function(diary1, diary2) { return diary2.date - diary1.date; });
	    var i = 0; indexText = "";
	    while (i < index.length && i < num) {
	        if (i % 2 == 0) {
	            indexText += "<p style = 'background-color:#EEEEEE'>";
	        }
	        else
	            indexText += "<p>";
	        indexText += "<strong>"
	        + (index[i].date.getMonth() + 1 )+ "/"
	        + (index[i].date.getDate() + "/")
	        + (index[i].date.getFullYear())
	        + "</strong><br/>" + index[i].content + "</p>";
	        i++;
	    }
	    document.getElementById("Show").innerHTML = indexText;
	}
	</script>
</head>
<body onload = "ShowItem(1);">
<div id = "Show"> </div>
<input type="button" id = "showAll" value = "Show All diary" onclick = "ShowItem();">
</body>
</html>
```





