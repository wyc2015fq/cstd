# PHP相关系列 - 某开源php软件的一个安全漏洞所想 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月23日 14:35:17[initphp](https://me.csdn.net/initphp)阅读数：2255








早上看到某开源php软件爆出这么一个漏洞：

[http://www.xxxx.net//index.php?m=search&c=index&a=public_get_suggest_keyword&url=asdf&q=../../phpsso_server/caches/configs/database.php](http://www.chunbao.net//index.php?m=search&c=index&a=public_get_suggest_keyword&url=asdf&q=../../phpsso_server/caches/configs/database.php)

以上网址为phpcms的应用程序

然后基本你的数据库密码就沦陷了。




看[public_get_suggest_keyword](http://www.chunbao.net//index.php?m=search&c=index&a=public_get_suggest_keyword&url=asdf&q=../../phpsso_server/caches/configs/database.php)
 这个函数：

/phpcms/modules/search/index.php中




```php
public function public_get_suggest_keyword() {
		$url = $_GET['url'].'&q='.$_GET['q'];  
		
		$res = @file_get_contents($url);
		if(CHARSET != 'gbk') {
			$res = iconv('gbk', CHARSET, $res);
		}
		echo $res;
	}
```





```php
$url = $_GET['url'].'&q='.$_GET['q'];
```
url没有任何白名单，没有任何过滤就杀进代码中





```php
$res = @file_get_contents($url);
```
又一个这个函数，如果弄一个大文件，那么直接可以把服务器搞垮



搜索了一圈，只有在一个JS中用到这个代码：



```java
$(document).ready(function() {
        $("#q").suggest("?m=search&c=index&a=public_get_suggest_keyword&url="+encodeURIComponent('http://www.google.cn/complete/search?hl=zh-CN&q='+$("#q").val()), {
            onSelect: function() {
				alert(this.value);
			}
        });

	});
```





**警示：**

**1. 传递进来的参数一定要进行安全过滤**

**2. 如果进行url,路径操作，一定要进行路径过滤**

**3. 对外少用@file_get_contents 怎么死都不知道**

**4. 上面那种情况一定要做域名白名单**





