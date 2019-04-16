# 转载和积累系列 - Nodejs 中GET方法实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月10日 09:14:28[initphp](https://me.csdn.net/initphp)阅读数：1676








```java
//GET方法，获取URL中的GET提交数据
	this.get = function (name) {
		var getQuery= url.parse(_req.url).query;
		var getData = querystring.parse(getQuery); //getData数据
		if (typeof(name) == 'object') { //数组形式传递进来
			var temp = {};
			for (var i = 0; i < name.length; i++) {
				if (getData[name[i]]) {
					temp[name[i]] = getData[name[i]];
				} else {
					temp[name[i]] = '';
				}
			}
			return temp;
		} else {
			if (getData[name]) {
				return getData[name];
			} else {
				return '';
			}
		}
	}
```



使用方法：

```java
//获取单个
get('username');
//获取多个返回对象
get(['username', 'hello'])
```






