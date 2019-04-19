# Pydev Console中文提示乱码的问题 - =朝晖= - 博客园
# [Pydev Console中文提示乱码的问题](https://www.cnblogs.com/dhcn/p/7106451.html)
1、 像这样的规则内容请这样处理"\u305d\u3093\u306a\u306b"style unicode string ：
```
print str.decode("unicode_escape")#decode"\u305d\u3093\u306a\u306b"style unicode string
```
2、Unicode完全乱码症状的解决，主要要把Pydev的默认编码调整成UTF-8,调整方法比较麻烦参看
 http://blog.ianbicking.org/illusive-setdefaultencoding.html
http://stackoverflow.com/questions/3023972/printing-unicode-in-eclipse-pydev-console-and-in-idle

