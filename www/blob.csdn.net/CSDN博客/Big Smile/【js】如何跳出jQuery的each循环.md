# 【js】如何跳出jQuery的each循环 - Big Smile - CSDN博客
2018年02月18日 16:41:47[王啸tr1912](https://me.csdn.net/tr1912)阅读数：345标签：[javascript																[循环](https://so.csdn.net/so/search/s.do?q=循环&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/tr1912/article/category/6400625)
        首次碰到这样的问题，是在一个文本框的校验中，由于文本框的数量和有无都是未知可变的，所以在进行页面绘制的时候，所有的input框都有一个一样的name，然后如果存在的话，通过遍历来校验里面是否写入了内容。
        然而在对于数量校验的时候出现了问题，一般的校验代码如下：
```java
if (!valid($("#packUnit").val())) {
    layer.msg('请填写单位！', {
	icon: 2,
        time: 1500, //1s后自动关闭
    });
    return false;
}
```
通过对于id为指定内容的val取值，然后进行函数式的校验，最后判断返回的是true还是false来判断是否填写了内容，如果没有填写，则返回false，然后通过layer进行弹窗提示，并使整个函数结束，方法为return一个false。
但是，如果是一个批量的校验的话，需要用到for，或者foreach进行循环，在里面进行逐个的验证。这个时候，用return false并不能让整个函数结束，那他都做了什么呢？
```java
$("input[name='serviceTimes']").each(function(){
	if (!valid($(this).val())) {
		layer.msg('请填写服务次数！', {
			icon: 2,
			time: 1500, //1s后自动关闭
		});
		return false;
	}
});
```
这样的话，我们在页面可以看到，没有经过验证的输入依然是可以提交的，但是却也弹出了提示。
说明在each里面的return false不是整个函数的return false，而是each的return false。那么，这个return false起到了什么用处呢？
我们可以写一个简单的程序：
```java
$("input[name='serviceTimes']").each(function(){
		 if (!valid($(this).val())) {
			layer.msg('请填写服务次数！', {
				icon: 2,
			    time: 1500, //1s后自动关闭
			 });
			return false;
		}
	});
    alert('结束');
```
依然是上面的代码在each后加入一个alert，我们运行一下就可以看到，在验证到为空的input框的时候，提示之后，就直接运行了alert的内容，显然，这里在each循环中的return false，相当于我们在java或者C#中的break。
那么continue需要怎么操作呢？在网上查询了一下，很简单，就是return true。
综上：
break：    return  false；
continue：  return true;
如此一来，这里的校验，加上一个标志变量，就可以正确跳出了：
```java
var flag=0
	$("input[name='serviceTimes']").each(function(){
		 if (!valid($(this).val())) {
			layer.msg('请填写服务次数！', {
				icon: 2,
			    time: 1500, //1s后自动关闭
			 });
			flag=1;
			return false;
		}
	});
	if(flag==1){
		return false;
	}
```
       上面的flag就是标志变量。
