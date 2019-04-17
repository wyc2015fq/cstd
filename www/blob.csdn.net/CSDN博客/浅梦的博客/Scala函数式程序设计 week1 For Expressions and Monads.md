# Scala函数式程序设计 week1 For Expressions and Monads - 浅梦的博客 - CSDN博客





2017年09月20日 11:32:37[浅梦s](https://me.csdn.net/u012151283)阅读数：163








# For表达式

## For表达式的翻译

Scala编译器将`for`表达式翻译成`map`,`faltMap`,`filter`的懒变形。 

以下函数都可以用for表达式的形式定义 
![这里写图片描述](https://img-blog.csdn.net/20170920120149082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```php
for(x <- e1) yield e2
//翻译成
e1.map(x=>e2)
//-----------------------------------
for(x <- e1 if f; s) yield e2//f是一个filters是一个可能为空的生成器或filter序列，翻译成
for(x<-e1.withFilter(x=>f);s) yield e2
//翻译会在新的表达上继续进行

`withFilter`是`filter`的一种变种，不产生中间列表。
//-----------------------------------
for(x <- e1; y<- e2;s) yield e3
//翻译成
e1.flatMap(x => for (y <- e2;s) yield e3)
```

## For表达式中模式匹配的翻译

![这里写图片描述](https://img-blog.csdn.net/20170920115438860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Monads

具有`map`和`flatMap`的数据结构很常见。 

事实上有一个名字描述这种带有一些代数法则的数据结构的类，叫做monada. 
![这里写图片描述](https://img-blog.csdn.net/20170920120640835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170920120921235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## monads 和map

map可以在每个monad中定义成flatMap和unit的组合

```
m map f == m flatMap (x => unit(f(x)))
        == m flatMap (f andThen unit)
```

## monad法则

要证明一个monad，一个类型必须满足三个条件
- 结合性 
`m flatMap f flatMap g == m flatMap (x => f(x) flatMap g)`- Left unit
`unit(x) flatMap f == f(x)`- Right unit
`m flatMap unit == m`




