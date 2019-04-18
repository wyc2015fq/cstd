# 使用ajax提交form表单的jquery插件 - z69183787的专栏 - CSDN博客
2012年10月24日 15:58:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1012
个人分类：[Javascript-Jquery插件																[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)](https://blog.csdn.net/z69183787/article/category/2175469)
Note: Aside from the options listed below, you can also pass any of the standard [`$.ajax`](http://docs.jquery.com/Ajax/jQuery.ajax#options) options
 to ajaxForm and ajaxSubmit.
所需JS：
jquery.form.js
设置与jquery中 $.ajax 中类似，提交后参数的形式为 a=1&b=2
option设置提交时的参数
```java
var listOptions = {
	cache:false,
	type:'post',
	callback:null,
	dataType :'json',
	url:contextpath+"/deptConfig/queryList.action",
    success:function(data){
		//var obj = JSON.parse(data);
		if(data){
			
		}
		return false;
    }
};
```
提交： listForm为表单form 的ID
```java
$("#listForm").ajaxSubmit(listOptions);
```
api地址为：
[http://www.malsup.com/jquery/form/#api](http://www.malsup.com/jquery/form/#api)
