# Python中urllib.urlencode中文字符的一个问题 - =朝晖= - 博客园
# [Python中urllib.urlencode中文字符的一个问题](https://www.cnblogs.com/dhcn/p/7105614.html)
   Django项目在访问Restful service时调用urllib.urlencode编码中文字符串时碰到下面这个错误、
```
v = quote_plus(str(v))  
UnicodeEncodeError: 'ascii' codec can't encode characters in position...........
```
     这个错误在windows7的开发环境下没有发生，在Linux生产环境下发生了，比较郁闷，刚上线以后的Bug.Google以后，在一份中文资料(链接见参考资料)中看到URL字符串不能是Unicode字符串，如果时，得编码成GBK或者UTF-8，在Django环境中，这个的确比较讨厌，所幸及时发现这个资料，对那段中文字符串采用了encode('utf-8')的方法做了编码转换:
```
utfstr =  unicodeStr.encode('utf-8')
```
      把那个utfstr传给相应的url编码函数即可。
参考资料：[http://lijiang.iteye.com/blog/312247](http://lijiang.iteye.com/blog/312247)

