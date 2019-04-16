# python requests 获取，设置cookie - 这里应该有一个很正经的标题的~~~，但是，怎么没了呢~~~~ - CSDN博客





2018年04月25日 19:41:59[hpulfc](https://me.csdn.net/hpulfc)阅读数：33270








python requests 获取，设置cookie

首先是有一个能返回cookie的url, 然后在获取， 如下：

```python
import requests
url = "https://fanyi.baidu.com"
res = requests.get(url)
print res.cookies
print type(res.cookies)
```

获取cookies是通过response的cookies 属性，可以通过res.cookies["cookie_name"]的方式获取。

并且需要注意的是，这个是一个RequestCookieJar的实例，也就是说，在requests的操作里的cookie一般都是包装到了这个类的实例：

看源码：（**使用方式在后面**）

```python
class RequestsCookieJar(cookielib.CookieJar, collections.MutableMapping):
  
    def get(self, name, default=None, domain=None, path=None):
        try:
            return self._find_no_duplicates(name, domain, path)
        except KeyError:
            return default

    def set(self, name, value, **kwargs):
       
        # support client code that unsets cookies by assignment of a None value:
        if value is None:
            remove_cookie_by_name(self, name, domain=kwargs.get('domain'), path=kwargs.get('path'))
            return

        if isinstance(value, Morsel):
            c = morsel_to_cookie(value)
        else:
            c = create_cookie(name, value, **kwargs)
        self.set_cookie(c)
        return c

    def iterkeys(self):
        
        for cookie in iter(self):
            yield cookie.name

    def keys(self):
        
        return list(self.iterkeys())

    def itervalues(self):
        
        for cookie in iter(self):
            yield cookie.value

    def values(self):
        
        return list(self.itervalues())

    def iteritems(self):
        
        for cookie in iter(self):
            yield cookie.name, cookie.value

    def items(self):
        
        return list(self.iteritems())
```

里面包括了很多的方法，可以根据需要进行使用，通常就是get, set，keys, itemitems等，和字典很像！

**重点**

如果需要在请求中添加cookie，可以实例化一个RequestCookieJar的类，然后把值set进去，最后在get,post方法里面指定cookies参数就行了，如下：

```python
import requests
from requests.cookies import RequestsCookieJar


url = "http://fanyi.baidu.com/v2transapi"

cookie_jar = RequestsCookieJar()
cookie_jar.set("BAIDUID", "B1CCDD4B4BC886BF99364C72C8AE1C01:FG=1", domain="baidu.com")

res = requests.get(url, cookies=cookie_jar)
print res.status_code
```
~~~            


