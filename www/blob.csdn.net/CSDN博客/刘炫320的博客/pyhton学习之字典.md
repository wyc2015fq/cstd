# pyhton学习之字典 - 刘炫320的博客 - CSDN博客
2017年03月27日 16:43:19[刘炫320](https://me.csdn.net/qq_35082030)阅读数：313
# 0. 写在前面
同样，在开始今天的新课程之前，我们先回顾一下上一章的内容，我们在继学习了列表、元组后，又学习了字符串这个常用的数据结构。对于字符串的输入输出以及格式有了一定的说明，并且对于字符串常用的函数方法也进行了简要的介绍。这样就把容器的第一部分序列学习完成了，接下来，我们学习容器的第二部分字典。
# 1. 字典定义
对于字典，也许听起来很奇怪，字典是什么？在其他语言中，他可能更多的叫做映射，或者是散列表，或者是键值对，等等，不过他们都具有相同的特性，那就是键-值对应，当然键是唯一的，值不是唯一的。
那么，下面来看一看字典的模样吧。首先我们来建立一个字典，建立子点有两种形式，第一种就是很普通的键值对创建，第二种是使用dict函数，实际上我更认为它是一种类型强制转换，配合其他存储格式来进行创建。
![字典定义](https://img-blog.csdn.net/20170327161738102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到，字典是使用大括号括起来的键值对，这样，我们就见到了三种括号的使用，小括号是元组，元组里的内容不可变。中括号的是列表，有些像数组一样的感觉，那么大括号其实原本是用在集合上的，集合里的元素是互斥的，所以具有唯一性，而字典的键也是唯一的，所以字典其实是集合的一种变种。
# 2. 字典的基本操作
字典的基本操作和序列有些类似，其实说白了就是和数组的方式差不多: 
1.  **len(d)**返回d中键值对的数量 
2. **d[k]**返回关联到键k上的值 
3. **d[k]=v**将值v关联到键k上 
4. **del d[k]**删除键为k的项 
5. **k in d**检查d中是否含有键为k的项。
针对以上基本操作，还有一下几个特性是字典所独有的： 
1. 键类型，字典的键类型只要是不可变的都可以，可以是常见的整形、浮点型，也可以是列表、元组，也可以是自定义类型，当然也可以是字典，等等，这个好像让我想到了什么。
- 
自动添加，即使字典中不存在当前索引的键，它也会自动添加这个键到字典中，而不会像列表一样报索引出界的错。
- 
成员资格，看的是字典的键，而不是值。列表则是查找的是值，不是索引。其实这个不难理解的，如果你看到了字典的本质是集合，你就不会困惑他为什么查的是键，因为键才是他所存储的值啊。
好的，现在我们把字典基本情况弄清楚了，举一个字典的例子来加深印象：
![简单通讯录](https://img-blog.csdn.net/20170327162109871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这就是一个简单的电话簿，可以获取一个人的姓名，电话号码以及家庭住址，接下来我们要介绍如何运行这段代码：
首先要转换到该文件所处的文件夹位置，要引入import os ，然后，再使用os.chdir()函数来改变当前工作路径。
其次，要使用运行文件函数execfile()即可运行指定文件。 
运行结果如下：
![执行结果](https://img-blog.csdn.net/20170327162213810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
是不是很神奇，比使用其他语言要方便快捷的多？（好吧，其实是使用者方便快捷了，对于机器来讲，负担是加重了。）但是，难道你不觉得这像另外一种存储格式吗？JSON，没错，这就跟JSON存储格式一样，难道你就不能再想的远一些么？对，NOSQL的存储也是这样存储的。天哪，这是多么神奇的结构。
# 3. 字典的格式化字符串
那么是不是字典只能这样使用呢？不不不，他可不仅仅只有这般惊艳。下面我们看另外一个例子，是模板搭配着字典和格式化输出形成的绝妙的配合：
![格式化输出](https://img-blog.csdn.net/20170327162601907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
你会看到，就是如此的神奇，当然，如果用到String.Template类上面，那效果更是绝佳，想想，如果有了母版页，又有了数据内容，再使用文件输入输出，那么就会自动输出一个页面，那么就可以组成一个半静态的动态网站，极大的加快网站速度，而且还减轻服务器负担。
# 4. 字典的常用方法
字典的方法也有很多，这里只列举几个常用的方法：
## 4.1 clear()
clear()方法会清楚字典中所有的项，而且是原地操作，即操作就原操作数为中心操作。
![clear()](https://img-blog.csdn.net/20170327163907209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.2 copy()
讲到copy就不得不的讲一下浅克隆与深克隆。这里的copy就是浅客隆，它只是复制了与其父本的数值，也就是说，他并不复制父本的引用，当修改子本引用时，父本引用也会受到影响。而深克隆不仅仅克隆了父本的数值，也克隆了父本的引用，这么讲，深克隆是完整的构造出一个与父本相互独立的对象。而深克隆要使用deepcopy()方法才能实现。下面是2个例子。
![copy()](https://img-blog.csdn.net/20170327163928913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.3 fromkeys
这个方法的目的很简单，就是创建一个词典并且每个键上都有对应的值。
![fromkeys](https://img-blog.csdn.net/20170327163948507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.4 get
这个方法让我想到了get,set接口，没错，这是个标准获取数据的方法，通过它访问字典，即使字典里不存在所要访问的项，它也不会报错，只会给你一个None,当然你也可以指定默认值。
![get](https://img-blog.csdn.net/20170327164009914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面是改进版的电话簿：
![改进版电话簿](https://img-blog.csdn.net/20170327164027148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![结果](https://img-blog.csdn.net/20170327164051758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.5 has_key
这个方法可以检查字典中是否含有特定的键，不过python3中已经移除了这个函数
## 4.6 items和iteritems
items是将字典所有的项以列表的方式返回，列表中每一项都表示为键值对的形式，但是项在返回时并没有遵循特定的次序。
![items](https://img-blog.csdn.net/20170327164108570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Iteriterms和items的功能一样，但是返回的是一个迭代器对象，不过这方面信息需要第九章才能讲到，要说的是通常情况下使用迭代器会更加高效。
## 4.7 keys和iterkeys
keys是返回字典中所有键返回，而iterkeys则是返回迭代器对象。 
![keys](https://img-blog.csdn.net/20170327164124869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.8 pop
这个不陌生吧，就是提取出特定的键的值，并把其键值对移除。 
![pop()](https://img-blog.csdn.net/20170327164139056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.9 popitem
这个是随机移除字典中一个项，因为字典是集合的变种，所以字典也是无序的，如果需要把每一个项都处理了，并移除，这个方法很有效。
## 4.10 setdefault
这个方法和get方法类似，不过有一点要说明的是，get方法当获取的键不在字典中时，它只会返回默认值，而不会修改字典，而setdefault则会修改字典，添加不存在的项。 
![setdefault()](https://img-blog.csdn.net/20170327164155852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.11 update
这个方法会利用一个字典项更新另一个字典，如果原来那个项存在，会被更新，如果不存在，会被添加进原字典。 
![update()](https://img-blog.csdn.net/20170327164212821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 4.12 values和itervalues
好了，这是最后一个常用函数了，既然我们都知道keys和iterkeys，那么这个也应该很熟悉吧，这是获取所有值，返回的也是列表。
# 5. 小结
好了，这一章，我们就讲完了，主要讲了三个部分，一个是字典的概念，一个是利用字典格式化字符串，第三部分就是讲字典的方法。
我们已经把几个常用的数据结构已经讲完了，下一章中，我们会简要的介绍一下集合，然后我们就要开始新的一章了，流程控制语句了。
