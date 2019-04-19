# Drupal7.8的安装注意的问题 - =朝晖= - 博客园
# [Drupal7.8的安装注意的问题](https://www.cnblogs.com/dhcn/p/7105377.html)
      首先推荐在安装之前，首先阅读一下官方的[Installation guide](http://drupal.org/documentation/install) ，说实话话，这玩意安装没有Joomla安装那么平滑，大多数问题，guide都会提到。我是先安装后看的这个guide,比较折腾。
        在安装的过程中一定要在[PHP](http://lib.csdn.net/base/php).ini把xdebug.show_exception_trace特性关闭了，负责的话，在[数据库](http://lib.csdn.net/base/mysql)设置那一步，就会报错：
```
DatabaseConnectionNotDefinedException: The specified database connection is not defined: default
```
        你可能会看到数据库里有31个表，那是不完整的，完整安装有70多个表。
        汉化包用汉化项目组的那个po,安装前放到/profiles/standard/translations目录中即可。国际化安装包的那个po比汉化组的那个po小多了。
        还有一个问题,， 就是php的max_execution_time参数需要调高一点。我刚开始调到100结果导入翻译报错，后来调到800，pass.当然安装完，你可以再调回来。
       参考资料：
[http://drupal.org/node/1074098](http://drupal.org/node/1074098)
[http://drupal.org/node/947616](http://drupal.org/node/947616)

