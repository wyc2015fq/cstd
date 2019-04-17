# Groovy学习第一篇：什么是Groovy？ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月28日 16:32:11[boonya](https://me.csdn.net/boonya)阅读数：1030标签：[groovy																[面向对象编程																[动态语言																[脚本语言																[jvm](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)
个人分类：[Groovy/Gradle](https://blog.csdn.net/boonya/article/category/3152311)





**From：[Wiki Groovy](http://zh.wikipedia.org/wiki/Groovy)**

**Groovy**是[Java平台](http://zh.wikipedia.org/wiki/Java%E5%B9%B3%E5%8F%B0)上设计的[面向对象编程](http://zh.wikipedia.org/wiki/%E9%9D%A2%E5%90%91%E5%AF%B9%E8%B1%A1%E7%BC%96%E7%A8%8B)语言。这门动态语言拥有类似[Python](http://zh.wikipedia.org/wiki/Python)、[Ruby](http://zh.wikipedia.org/wiki/Ruby)和[Smalltalk](http://zh.wikipedia.org/wiki/Smalltalk)中的一些特性，可以作为Java平台的[脚本语言](http://zh.wikipedia.org/wiki/%E8%84%9A%E6%9C%AC%E8%AF%AD%E8%A8%80)使用。

Groovy的语法与Java非常相似，以至于多数的Java代码也是正确的Groovy代码。Groovy代码动态的被[编译器](http://zh.wikipedia.org/wiki/%E7%BC%96%E8%AF%91%E5%99%A8)转换成Java字节码。由于其运行在JVM上的特性，Groovy可以使用其他Java语言编写的库。

![](https://img-blog.csdn.net/20150428163158184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 程序范例

下面是来自于 [http://Groovy.CodeHaus.org](http://Groovy.CodeHaus.org) 的一个例子程序：


class Foo {
  doSomething(){
    data =["name": "James", "location": "London"]for(e in data){println("entry ${e.key} is ${e.value}")}}
 
  closureExample(collection){
    collection.each{println("value ${it}")}}staticvoid main(args){
    values =[1, 2, 3, "abc"]
    foo =new Foo()
    foo.closureExample(values)
    foo.doSomething()}}


## IDE支持

已经有许多的[集成开发环境](http://zh.wikipedia.org/wiki/%E9%9B%86%E6%88%90%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83)支持了该门语言：
- [Eclipse](http://zh.wikipedia.org/wiki/Eclipse)，插件[Groovy-Eclipse](http://groovy.codehaus.org/Eclipse+Plugin)
- [IntelliJ IDEA](http://zh.wikipedia.org/wiki/IntelliJ_IDEA)，默认就支持Groovy开发。
- [NetBeans](http://zh.wikipedia.org/wiki/NetBeans)

## 外部链接
- [Groovy 官方网站](http://Groovy.CodeHaus.org)
- [Groovy++](http://code.google.com/p/groovypptest/)](https://so.csdn.net/so/search/s.do?q=脚本语言&t=blog)](https://so.csdn.net/so/search/s.do?q=动态语言&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象编程&t=blog)](https://so.csdn.net/so/search/s.do?q=groovy&t=blog)




