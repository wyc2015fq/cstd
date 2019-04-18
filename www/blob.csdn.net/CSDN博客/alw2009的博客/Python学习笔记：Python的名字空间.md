# Python学习笔记：Python的名字空间 - alw2009的博客 - CSDN博客
2016年09月25日 16:25:05[traveler_zero](https://me.csdn.net/alw2009)阅读数：148
**基本概念：**
1. Python的世界是名字和对象的世界。
2.名字空间是名字到对象的映射。
3.赋值是名字与对象的绑定， 不拷贝数据。
![](https://img-blog.csdn.net/20160925170853343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**例子说明：**
```python
<pre style="background-color:#2b2b2b;color:#a9b7c6;font-family:'宋体';font-size:9.0pt;"><span style="color:#cc7832;"><strong>def </strong></span><span style="font-weight:bold;">scope_test</span>():
    <span style="color:#cc7832;"><strong>def </strong></span><span style="font-weight:bold;">do_local</span>():
        <span style="color:#808080;"># "local ssam" 对象绑定到 local namespace的spam
</span><span style="color:#808080;">        spam </span>= <span style="color:#008080;">"local spam"
</span><span style="color:#008080;">
</span><span style="color:#008080;">    </span><span style="color:#cc7832;"><strong>def </strong></span><span style="font-weight:bold;">do_nonlocal</span>():
        <span style="color:#808080;">#  声明不要让spam绑定到local namespace
</span><span style="color:#808080;">        </span><span style="color:#cc7832;"><strong>nonlocal </strong></span>spam
        spam = <span style="color:#008080;">"nonlocal spam"
</span><span style="color:#008080;">
</span><span style="color:#008080;">    </span><span style="color:#cc7832;"><strong>def </strong></span><span style="font-weight:bold;">do_global</span>():
        <span style="color:#808080;"># 声明将spam绑定到module 的global namespace
</span><span style="color:#808080;">        </span><span style="color:#cc7832;"><strong>global </strong></span>spam
        spam = <span style="color:#008080;">"global spam"
</span><span style="color:#008080;">
</span><span style="color:#008080;">    </span><span style="color:#808080;"># 在scope_test()的local namespace里将spam与"test spam"绑定
</span><span style="color:#808080;">    </span>spam = <span style="color:#008080;">"test spam"
</span><span style="color:#008080;">    </span><span style="color:#808080;"># 调用do_local(), 会在do_local()的生命周期里将do_local()的local namespace与"local spam"绑定， 调用结束后删除绑定
</span><span style="color:#808080;">    </span>do_local()
    <span style="color:#808080;"># 打印scope_test()的local namespace里的spam
</span><span style="color:#808080;">    </span><span style="color:#8888c6;">print</span>(<span style="color:#008080;">"After local assignment:"</span><span style="color:#cc7832;">, </span>spam)
    <span style="color:#808080;"># 调用do_nolocal(), 在do_nolocal里声明了不要将spam与do_nolacal()的local namespace绑定， python会在外层绑定，
</span><span style="color:#808080;">    # scope_test()的local namespace的spam将会被绑定到"nolocal spam"
</span><span style="color:#808080;">    </span>do_nonlocal()
    <span style="color:#808080;"># 打印scope_test()的local namespace的spam
</span><span style="color:#808080;">    </span><span style="color:#8888c6;">print</span>(<span style="color:#008080;">"After nonlocal assignment:"</span><span style="color:#cc7832;">, </span>spam)
    <span style="color:#808080;"># 调用do_global(), do_global()里将"global spam"于模块的global namespace的spam绑定，
</span><span style="color:#808080;">    </span>do_global()
    <span style="color:#808080;"># 打印scope_test()的local namespace的spam
</span><span style="color:#808080;">    </span><span style="color:#8888c6;">print</span>(<span style="color:#008080;">"After global assignment:"</span><span style="color:#cc7832;">, </span>spam)
scope_test()
<span style="color:#808080;"># 打印模块global namespace的spam
</span><span style="color:#8888c6;">print</span>(<span style="color:#008080;">"In global scope:"</span><span style="color:#cc7832;">, </span>spam)
```
输出结果：
**After local assignment: test spamAfter nonlocal assignment: nonlocal spamAfter global assignment: nonlocal spamIn global scope: global spam**
