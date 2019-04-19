# 利用jquery将页面中所有目标为“#‘的链接改为其他URL - 左直拳的马桶_日用桶 - CSDN博客
2014年01月28日 13:54:33[左直拳](https://me.csdn.net/leftfist)阅读数：1524
有一张页面草稿，开始时为了方便，里面差不多所有的<a>都指向了"#"。现在要修改为另一个地址，难道要全部修改吗？用jquery 改改就好。代码如下：
```java
<script type="text/javascript">
	    $(function () {
	        $("[href = '#']").attr("href", "/show/");
	    });
	</script>
```
