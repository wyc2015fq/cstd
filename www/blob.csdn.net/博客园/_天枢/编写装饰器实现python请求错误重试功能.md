# 编写装饰器实现python请求错误重试功能 - _天枢 - 博客园
## [编写装饰器实现python请求错误重试功能](https://www.cnblogs.com/yhleng/p/9273075.html)
2018-07-06 13:19 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9273075)
**在做接口自动化测试的时候，总会遇到，因连接超时等错误导致，接口脚本失败。**
官方给出的方法：
**max_retries=5 出错重试5次**
**注意的是，这个只对DNS，连接错误进行重试。**
```
from requests.adapters import HTTPAdapter
    s = requests.Session()
    s.mount('http://',HTTPAdapter(max_retries=5))
    s.mount('https://',HTTPAdapter(max_retries=5))
    s.get('https://www.baidu.com')
```
**自编写装饰器一**
```
from requests.exceptions import ConnectionError
import requests
def retry(**kw):
    def war(func):
        def w(*args,**kwargs):
            try:
                ret = func(*args,**kwargs)
            except ConnectionError:
                kw['reNum'] = int(kw['reNum']) - 1
                if kw['reNum'] >=0:
                    print kw['reNum']
                    ret = w(*args,**kwargs)
                else:
                    ret = ConnectionError
            return ret
        return w
    return war
```
**自编写装饰器二**
```
from requests.exceptions import ConnectionError
def retry(**kw):
    def wrapper(func):
        def _wrapper(*args,**kwargs):
            raise_ex = None
            for _ in range(kw['reNum']):
                print _
                try:
                    return func(*args,**kwargs)
                except ConnectionError as ex:
                    raise_ex = ex
            #raise raise_ex
        return _wrapper
    return wrapper
```
**使用方法：reNum = 5 代表，出现ConnectionError时最多可重试5次。**
```
@retry(reNum=5)
def demo():
    raise ConnectionError
```
**总结：**
1.编写装饰器，其实没有那么难，只要掌握方法。 这个可以参考，我之前写的关于装饰器的文章
2.装饰器的通熟解释，就是在函数之前后之后做点什么。通过这个我们可以做很多。
3.关于请求连接错误，重试，装饰器；原理就是做一个循环，只要捕获到有ConnectionError 错误，就进入下一次循环
调用；只要有正确的时候，直接返回函数。
**qq技术交流群，期待你的加入：**
**python｜测试｜技术交流群：563227894**
**python｜测试｜技术交流群：563227894**
**python｜测试｜技术交流群：563227894**
