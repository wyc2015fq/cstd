# JAVA反序列化 - weixin_33985507的博客 - CSDN博客
2017年12月04日 13:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
利用java反序列化实现RCE。
转自：https://security.tencent.com/index.php/blog/msg/97
若java应用没有对传入的序列化数据进行安全性检查，可以将恶意的TransformedMap序列化后，远程提交给Java应用，如果Java应用可以触发变换，即可成功远程命令执行。
如何让Java应用触发Transformer的变换？
在进行反序列化时，会调用ObjectInputStream类的readObject()方法。如果被反序列化的类重写了readObject()，那么该类在进行反序列化时，Java会优先调用重写的readObject()方法。
结合前述Commons Collections的特性，如果某个可序列化的类重写了readObject()方法，并且在readObject()中对Map类型的变量进行了键值修改操作，并且这个Map变量是可控的，就可以实现我们的攻击目标了。
于是找到了这个类：AnnotationInvocationHandler。该类的代码如下：
![5476033-45d0f336aebac517.png](https://upload-images.jianshu.io/upload_images/5476033-45d0f336aebac517.png)
简直完美。它的成员变量memberValue为Map 类型，并且在重写的readObject()方法中有memberValue.setValue()的操作。
我们可以实例化一个AnnotationInvocationHandler类，将其成员变量memberValues赋值为精心构造的恶意TransformedMap对象。然后将其序列化，提交给未做安全检测的Java应用。Java应用在进行反序列化操作时，则会触发TransformedMap的变换函数，执行预设的命令。
