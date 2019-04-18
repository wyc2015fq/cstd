# spring中的spel表达式语言 - z69183787的专栏 - CSDN博客
2016年07月11日 17:40:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2084
spring in action第三版读书笔记
spring3.0引入了spring expression language(spel)语言,通过spel我们可以实现
1.通过bean的id对bean进行引用
2.调用方法以及引用对象中的属性
3.计算表达式的值
4.正则表达式的匹配
5.集合的操作
spel最终的目标是得到表达式计算之后的值，这些表达式可能是列举的一些值，引用对象的某些属性，或者是类中的某些常量,复杂的spel表达式通常都是由一些简单的元素构成的
最简单的仅仅是得到一些给出元素的值例如：
<property name="count" value="the value is #{5}"/>。这种情况貌似很傻，根本就不需要用到spel,但是复杂的表达式都是由简单的构成的
对其他bean的引用
通过spel我们也可以对context中其他的bean进行引用
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <propertyname="instrument"value="#{saxophone}"/>
等同于
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <propertyname="instrument"ref="saxophone"/>
引用另外一个id为saxophone的bean作为instrument的值
对其他bean中某个属性的引用
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <beanid="carl"class="com.springinaction.Instrumentalist">
- <propertyname="song"value="#{kenny.song}"/>
- </bean>
取id为kenny的bean的song字段的作为song的value
对其他bean中某个方法的引用
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <propertyname="song"value="#{songSelector.selectSong().toUpperCase()}"/>
调用id为songSelector的bean的selectSong()方法，使用其返回值作为song的值，这也带来一个问如果selectSong()方法返回一个null，那么会抛出一个空指针异常
<property name="song" value="#{songSelector.selectSong()?.toUpperCase()}"/>,表达式(?.)可以确保在selectSong()返回不为空的情况下调用toUpperCase()方法，如果返回空那么不继续调用后面的方法
对类进行引用
如果某个类是外部类，而不是spring中定义的bean，那么怎么进行引用呢？
使用表达式T(),例如：
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <propertyname="randomNumber"value="#{T(java.lang.Math).random()}"/>
spel计算表达式的值
spel表达式支持各种各样的运算符，我们可以可以运用这些运算符来计算表达式的值
使用spel从集合中筛选元素：
使用spring的util namespace中的元素<util:list>定义一个集合
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <util:listid="cities">
- <beanclass="com.habuma.spel.cities.City"
- p:name="Chicago"p:state="IL"p:population="2853114"/>
- <beanclass="com.habuma.spel.cities.City"
- p:name="Atlanta"p:state="GA"p:population="537958"/>
- <beanclass="com.habuma.spel.cities.City"
- p:name="Dallas"p:state="TX"p:population="1279910"/>
- <beanclass="com.habuma.spel.cities.City"
- p:name="Houston"p:state="TX"p:population="2242193"/>
- </util:list>
使用spel对集合进行筛选
<property name="chosenCity" value="#{cities[2]}"/>，
[]操作符也可以对Map进行筛选，假设citis是一个Map类型<property name="chosenCity" value="#{cities["keyName"]}"/>
[]对Properties类型进行操作
<util:properties id="settings"
location="classpath:settings.properties"/>使用<util:properties>标签读取一个properties文件
**[html]**[view
 plain](http://blog.csdn.net/yangnianbing110/article/details/7614521#)[copy](http://blog.csdn.net/yangnianbing110/article/details/7614521#)
- <propertyname="accessToken"value="#{settings['twitter.accessToken']}"/>
基于某个属性对集合中的元素进行过滤
<property name="bigCitis" value="#{cities.?[population gt 10000]}"/>选中人口大一10000的cities中的元素作为bigCitis的值,同操作符(.?[])类似， 操作符(.^[]选取满足要求的第一个元素， .$[]选取满足要求的最后一个)
选中已有集合中元素的某一个或几个属性作为新的集合
<property name="cityNames" value="#{cities.![name + ", " + state]}"/>
