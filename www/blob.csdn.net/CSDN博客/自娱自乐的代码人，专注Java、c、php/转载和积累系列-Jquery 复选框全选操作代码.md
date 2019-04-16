# 转载和积累系列 - Jquery 复选框全选操作代码 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年08月23日 10:55:12[initphp](https://me.csdn.net/initphp)阅读数：702标签：[jquery																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)








```java
var check_all = function () {
	if ($("#check_all").attr('checked') == true) {
		$('.check_val').each(function (i) {
			$('.check_val').eq(i).attr('checked', true);
		});
	} else {
		$('.check_val').each(function (i) {
			$('.check_val').eq(i).attr('checked', false);
		});
	}
}
$(document).ready(function(){ 
	$("#check_all").bind('click', function () {
		check_all();
	});	
});
```





