# mybatis xml中是sql语句报错： Error creating document instance. Cause: org.xml.sax.SAXPa - Ms.Fox的专栏 - CSDN博客
2018年11月02日 11:34:11[lulu-lu](https://me.csdn.net/smbluesky)阅读数：46
今天遇到了这个问题，感觉很奇怪，没有什么问题，但是还是会报错。
在网上找了半天，用第二种方法解决了这个问题。第一种方法没有尝试。以此来记录下。
感觉这个问题出现概率很大。要记着呢。
1、使用转移字符替代
|<|<|
|----|----|
|>|>|
|&|&|
|'|‘|
|"|“|
2、使用<![CDATA[      sql     ]]>标记
    例如： select * from table where 
        <![CDATA[
             id <= 3
          ]]>
mybatis中不能出现小写
# ibatis动态sql配置启动时提示：The content of elements must consist of well-formed character data
把下面这个表达式反过来写就可以了。如图：![](https://gss0.baidu.com/-fo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=69e55e9e2e1f95caa6a09ab0f9275306/77094b36acaf2edd5d231209861001e938019304.jpg)
