# javascript 遍历json对象检查是否不存在某些属性/字段 - 左直拳的马桶_日用桶 - CSDN博客
2019年02月17日 13:10:04[左直拳](https://me.csdn.net/leftfist)阅读数：74
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
javascript函数中，如果参数很多，可以设置一个json参数，将一些可选参数项放到里面去。
这样就有一个问题，既然是可选参数，那么调用的时候，既可以指定，也可以不指定，怎么知道有哪些属性并没有指定？如果不指定，该如何处理？
我的想法是，这个json参数有一个默认值，调用方指定的属性，当然采用调用方的指定；否则采用默认值。具体实现就是将传过来的json参数与默认json参数做个属性一一对照，有则用之，无则补之。
上代码：
```
function (code, url,option) {//code,url是必选参数；option是可选参数
	let _default = {//可选参数的默认值
		name:"未命名",
		width: 500,
		height:400,
		maxmin:true,
	};
	if(typeof option == 'undefined'){//如果option根本没传，简单了，直接全部默认
		option = _default;
	} else {
		for (var k in _default){//否则一一对照
			if(typeof option[k] == 'undefined'){//这里k拿到的是json的属性。
			//如果_default是数组，拿到的就是下标；js中，数组与json的界线有时真的很模糊。
			//I like it
				option[k] = _default[k];
			}
		}
	}
}
```
