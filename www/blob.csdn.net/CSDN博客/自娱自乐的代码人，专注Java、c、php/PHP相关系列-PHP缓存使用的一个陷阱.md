# PHP相关系列 - PHP缓存使用的一个陷阱 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月24日 12:37:12[initphp](https://me.csdn.net/initphp)阅读数：988标签：[php																[json																[360																[function																[cache](https://so.csdn.net/so/search/s.do?q=cache&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





先看一段代码：

```php
/**
	 * 获取设置信息
	 */
	public function getCoinSetting() {
		$cache 	= Common::getTair();
		$ckey 	= Common::hashKey("Hello");
		$ret 	= $cache->get($ckey);
		if ($ret) return json_decode($ret, true);
		$taomanyiApiService = $this->_getTmiApiService();
		$result = $taomanyiApiService->getCoinSetting();
		$cache->set($ckey, json_encode($result), 3600);
		return $result;
	}
```



这是一个使用Tair内存缓存的实例，这段代码中，设置了缓存，缓存时间为3600秒。数据是从Api中获取的，如果这么写会出现什么问题呢？假如：

```php
$result = $taomanyiApiService->getCoinSetting();
```



$result获取的数据为空，因为$result数据是从HTTP请求过来的，数据不正常也是比较常见的事情。在这种状况下，HTTP请求失败，那么接口数据就请求不到，接下来的流程是设置缓存

```php
$cache->set($ckey, json_encode($result), 3600);
```



我们会发现，因为一次接口HTTP请求的失败，我们不小心将空数据缓存了起来，缓存时间为3600秒。这样就会出现页面上，例如分类出现了数据的空白，影响了整个业务流程

**我们做以下的优化：**

```php
if ($result) $cache->set($ckey, json_encode($result), 3600);
```](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=360&t=blog)](https://so.csdn.net/so/search/s.do?q=json&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)




