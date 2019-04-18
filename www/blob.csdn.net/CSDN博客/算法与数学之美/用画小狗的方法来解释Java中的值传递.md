# 用画小狗的方法来解释Java中的值传递 - 算法与数学之美 - CSDN博客
2017年11月20日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：359
在开始看我画小狗之前，咱们先来看道很简单的题目：
下面程序的输出是什么？
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fia5qa79SVHrBVwxdtzSVqmW4hOpCD8ic0N0ClDOUhlrgKkyqwqmzMF9Rw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
如果你的回答是“**小强**”，好，恭喜你答对了。下面我们改一下代码：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaT7SzMtDccQyibq7kOewv4tvkvgLEe87RnHe9JtzBqfKA6UlGDxq41sQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
是的，我只是在changeName方法里面加了一句代码
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaMOqUlVe3ibiaLkWXhbaeZVljjjahrPIVgQpG9EKcICaAQS6uCeqvaIXA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
这一次的输出又是什么呢？
A、旺财 
B、小强
答案是A、旺财，changeName方法并没有把myDog的名称改了。如果你答错了，没关系，我要开始画小狗了，画完你就明白了；如果你答对了，但不太明白其中的原因，那我画的小狗也肯定能帮到你。
**myDog是什么**
首先你要搞懂，代码里的变量myDog是什么？**myDog真的就是一只狗吗？不！不是！myDog只是一条遛狗用的狗绳！**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/1hReHaqafadYppLylufOEF6ga71OD6fiaoJON4mLnAME56ZfE2AHRr1XPqsvAEnlksk2iaTjze0I6DtEq4QPwBLA/640?wx_fmt=jpeg)
换句话说说，myDog并不是new出来的放在堆中的对象（object）！**myDog只是一个指向这个对象实例的引用（reference）！**
如果你对Java的运行时数据区域足够了解，应该知道，这个引用是放在虚拟机栈上的。
**参数传递**
现在你知道了，myDog只是一条绳子，但这似乎并不能解释为什么changeName方法没有把myDog的名称改为“小强”，因为按照现有的理解，dog = new Dog()，就是把我的狗绳绑到另一只小狗身上，然后给这只小狗起名为“小强”，就像这样：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/1hReHaqafadYppLylufOEF6ga71OD6fiaeXcjnmc80CYgLDvrhW0iaBZsQ0H6puyia77BwSZNYmoh0tOIqIJ1KbTQ/640?wx_fmt=jpeg)
可事实是，myDog还是叫旺财，这是为什么？ 
**问题就出在方法调用上。**
当我执行changeName(myDog)这一行代码时，myDog这条狗绳，被复制了一份，而传入到changeName方法里的那条狗绳（dog），就是复制出来的那一条，就像这样：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/1hReHaqafadYppLylufOEF6ga71OD6fiaoZzw3nO5DuU9VBKKynCZAV4icialx23XhzNGJ2effKLDico5hr7cB05kA/640?wx_fmt=jpeg)
接着执行dog= new Dog()，这一行代码，就是把复制出来的那一条狗绳，从myDog解绑，重新绑到new出来的那只小狗上，也就是后来被起名为“小强”的小狗：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/1hReHaqafadYppLylufOEF6ga71OD6fiamdDWlwgZeIbtYYDdpkh0XMuJooHRkeAZSWyt4wor0RApF76q7jIv9w/640?wx_fmt=jpeg)
而myDog还是绑在旺财身上，这也就解释了，为什么执行完方法出来，myDog.getName()还是旺财。而在第一段代码里面，我们没有执行dog= new Dog()，也就没有改变dog所绑的小狗，dog还是绑在旺财身上，因此dog.setName(“小强”) 就把旺财的名字改成小强了。
**string的例子**
我们再来看一个例子：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaPzibWrOqmqVfEaDPrTsKSWzs70FJ3038NfKsFZo5auvvtyp05z8ic3zQ/640?wx_fmt=png)
如果你弄懂了上面那个例子，那么这里应该不难理解，changeString方法里，只是将新复制出来的引用str，指向另外一个字符串常量对象“bbb”，方法体外面的str并不受影响，还是指向字符串常量“aaa”，因此最终打印的还是aaa。
**int的例子**
上面提到的都是对象，下面看一个**基本数据类型**的例子
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaoC44J7bSPO4zHE6R2CQXJbmdzOG0oibHzCia9rHNQia0YoYINnM6Qbhsg/640?wx_fmt=png)
对于基本数据类型，他们没有引用，但是不要忘了，调用函数时，复制的动作还是会做的，执行changeInt(i)时，会将 i 复制到一个新的int上，传给changeInt方法，因此不管changeInt内部对入参做了什么，外面的 i 都不会受影响。最后打印出来的还是1。
**值传递和引用传递**
上面提到的参数传递过程中的复制操作，说白了，就是 **= 操作**。把上面那个int例子，做一下方法内联，其实就是这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiahlHyXyicicwZRXndUZ5IUciaeibWeLvxbibCliaN66XlMibAL9rUXdM5Zichxw/640?wx_fmt=png)
对于**基本数据类型**，= 操作将右边的变量（R_VALUE）完整的复制给左边的变量（L_VALUE），而对于对象，准确的说，应该是指向对象的引用（就像上面说的myDog），**= 操作同样也是将右边的引用完整的复制给左边的引用，两者指向同一个对象实例**。 
这个 **= 操作，是值传递和引用传递的根本差别**，这也导致了值传递和引用传递有以下直观上的**差别**：
> 
如果参数是值传递，那么调用者（方法体外部）和被调用者（方法体内部）用的是两个不同的变量，方法体里面对变量的改动不会影响方法体外面的变量。而之所以在Java可以在方法体内部改变方法体外部的对象，是因为方法体内部拿到了对象的引用，但是这个引用是和方法体外部的引用属于两个不同的引用的，方法体内部的引用指向别的对象，不会导致方法体外部的引用也指向别的对象。
如果参数是引用传递，那么调用者（方法体外部）和被调用者（方法体内部）用的是两个相同的变量，方法体里面对变量的改动会影响方法体外面的变量。
**Java的变量都不是对象**
通过上面的讲解，你也知道了一个很重要的点：Java里面的变量，要么是基本数据类型，要么是指向对象实例的引用类型（狗绳），绝对不会是一个对象（狗）。
**狗绳和垃圾回收**
弄懂了myDog只是一条狗绳（引用），也有助于我们理解Java的**垃圾回收机制**，**一旦JVM发现一个对象跟GC Roots不可达时，这个对象就会被回收掉**。
看一下下面这段代码：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaeXf0lribSa0sOrDEzMAYEribsN87gpgI7uDEOtlvfVCGpZ3lx2gicwgtg/640?wx_fmt=png)
现在我们知道，dog=null就等于是把狗绳给咔嚓减掉了，这样狗就跑了，变成流浪狗了，就像Java中的对象被当做垃圾回收了一样：
接着再来看一下**交叉引用**的例子：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaDo8UQLjvNFOlsnABRerBWicibhEU5eSMJVlhF5ibkVXQkHpgLtk47iciasQ/640?wx_fmt=png)
如果JVM采用的是引用计数法，那么狗2原先被dog2和dog1.son两个变量引用这，执行完dog2 = null之后，还被dog1.son引用，狗2是不会被回收的。 
但是如果使用可达性分析法，我们就会发现，这两只狗和这个世界已经没有关联了，尽管他们俩还是父子关系，JVM对于这种互相引用，但是和GC ROOTS已经没有关联的对象，照样会进行回收。
**引用传递的替代方法**
引用传递有两个好处：
> 
引用传递可以**避免调用方法时进行拷贝**，尤其是当方法的入参是个大对象时，拷贝会耗费大量的时间和空间，当然，这一点Java已经巧妙地解决了，因为对于对象，拷贝的只是它的引用而已；
引用传递可以对外面的对象进行修改，这也是很多语言支持引用传递的原因。
那么，在Java，要怎么实现“对外面的对象进行修改”类似的功能呢？ 
答案是**使用返回值**，类似这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaCBhP8ibeckgmABiczxIWbkyVicqEg2n6DXEAnBsX09oWibn3ZY3ZD3RSkA/640?wx_fmt=png)
当然，如果你只是对一个对象进行修改，然后返回这个对象的新的版本，那么可以考虑把这个方法挪到这个对象里面去，就像这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1hReHaqafadYppLylufOEF6ga71OD6fiaeeIGx2ClHXLRDrd8EuiaoGU8Yb3v8RFtSZz6FHNm3UH0tyTTqj2cE1g/640?wx_fmt=png)
还有，如果你是需要返回多个值，不使用引用传递，要如何实现？ 
答案是**返回一个对象**，比如你想修改一个地方的经度和纬度，那么与其传入log和lat两个变量，不如把他们封装到Point对象里面去。
以上，希望对你有所帮助。
来源：程序人生
**---****------**
**等的就是你，真的超有趣！高能金融抱团群发车啦~******加我拉你进群呦****
**![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyJCDnjGtGFUOmOAE2SZY1zvIueyhic76aNgYN38xwV6XE9y6tSzCTFYianCf1YgCrfeNqconWV5RhQ/640?wx_fmt=png)**
**算法数学之美微信公众号欢迎赐稿**
**稿件涉及数学、物理、算法、计算机、编程等相关领域。**
**稿件一经采用，我们将奉上稿酬。**
**投稿邮箱：math_alg@163.com商务合作：联系微信号hengzi5809**
