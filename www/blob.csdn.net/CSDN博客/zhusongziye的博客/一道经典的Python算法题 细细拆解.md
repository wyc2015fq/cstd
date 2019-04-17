# 一道经典的Python算法题|细细拆解 - zhusongziye的博客 - CSDN博客





2018年11月24日 09:56:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：397








*新建Python软件开发测试技术交友群QQ:952490269（加群备注software）*

今天来看一道大厂经常面试的算法题Python解法。



**有效的括号**

判断一个字符串中的大，中，小括号是否合法：

有效字符串需满足：
- 
左括号必须用相同类型的右括号闭合。

- 
左括号必须以正确的顺序闭合。


注意空字符串可被认为是有效字符串。比如"( )","( )[ ]","( ( ( [ ] ) ) )"都是合法的，但是"( [ ) ]"就是不合法的。**这道题是非常经典的面试题，据说Facebook,微软，Google，亚马逊都考过这道题**，只是加了一些变化而已。



目前为止最好的解法就是堆栈，比如我们判断"( ( [ ] ) )"。思路就是压栈，然后从栈顶进行匹配，如果匹配成功比如左小括号遇到右小括号，则把压入栈的左小括号出栈，匹配成功,然后继续下一个。



![](https://img-blog.csdnimg.cn/20181124094643122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



如果碰到"( [ ) ]",情况就不一样了，左小括号进栈，左中括号进栈，右小括号和栈顶进行匹对，发现不匹配则失败。

![](https://img-blog.csdnimg.cn/20181124094705275.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



来看一下经典的源码：

![](https://img-blog.csdnimg.cn/20181124094736233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



这段代码非常精炼，首先设计上 mapping 用右括号作为key,这样的好处是当你检查字符串中如果不是右括号(那必然是左括号）直接入栈，这样写非常简洁。

另外直接在elif 里面用stack.pop来循环抛出栈顶进行匹配。最绝是直接not stack返回。如果stack为空则成功，否则失败！

大家可以好好体会一下，有空刷刷leetcode还是蛮好的！

[https://**leetcode**.com/](http://www.baidu.com/link?url=FkrINnXaoQj1a0O9Gd1IOg2UCqUYkZ6nLQRO6u9xgP3)



