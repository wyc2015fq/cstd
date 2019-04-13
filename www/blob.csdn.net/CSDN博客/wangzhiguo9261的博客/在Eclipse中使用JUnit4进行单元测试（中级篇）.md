
# 在Eclipse中使用JUnit4进行单元测试（中级篇） - wangzhiguo9261的博客 - CSDN博客


2018年06月21日 17:12:19[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：30个人分类：[junit](https://blog.csdn.net/wangzhiguo9261/article/category/7748785)


我们继续对初级篇中的例子进行分析。初级篇中我们使用Eclipse自动生成了一个测试框架，在这篇文章中，我们来仔细分析一下这个测试框架中的每一个细节，知其然更要知其所以然，才能更加熟练地应用JUnit4。
一、包含必要地Package
在测试类中用到了JUnit4框架，自然要把相应地Package包含进来。最主要地一个Package就是org.junit.*。把它包含进来之后，绝大部分功能就有了。还有一句话也非常地重要“import static org.junit.Assert.*;”，我们在测试的时候使用的一系列assertEquals方法就来自这个包。大家注意一下，这是一个静态包含（static），是JDK5中新增添的一个功能。也就是说，assertEquals是Assert类中的一系列的静态方法，一般的使用方式是Assert. assertEquals()，但是使用了静态包含后，前面的类名就可以省略了，使用起来更加的方便。
二、测试类的声明
大家注意到，我们的测试类是一个独立的类，没有任何父类。测试类的名字也可以任意命名，没有任何局限性。所以我们不能通过类的声明来判断它是不是一个测试类，它与普通类的区别在于它内部的方法的声明，我们接着会讲到。
三、     创建一个待测试的对象。
你要测试哪个类，那么你首先就要创建一个该类的对象。正如上一篇文章中的代码：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)privatestaticCalculator calculator=newCalculator();

为了测试Calculator类，我们必须创建一个calculator对象。
四、测试方法的声明
在测试类中，并不是每一个方法都是用于测试的，你必须使用“标注”来明确表明哪些是测试方法。“标注”也是JDK5的一个新特性，用在此处非常恰当。我们可以看到，在某些方法的前有@Before、@Test、@Ignore等字样，这些就是标注，以一个“@”作为开头。这些标注都是JUnit4自定义的，熟练掌握这些标注的含义非常重要。
五、编写一个简单的测试方法。
首先，你要在方法的前面使用@Test标注，以表明这是一个测试方法。对于方法的声明也有如下要求：名字可以随便取，没有任何限制，但是返回值必须为void，而且不能有任何参数。如果违反这些规定，会在运行时抛出一个异常。至于方法内该写些什么，那就要看你需要测试些什么了。比如：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)@Test
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicvoidtestAdd(){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)calculator.add(2);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)calculator.add(3);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)assertEquals(5, calculator.getResult());
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

我们想测试一下“加法”功能时候正确，就在测试方法中调用几次add函数，初始值为0，先加2，再加3，我们期待的结果应该是5。如果最终实际结果也是5，则说明add方法是正确的，反之说明它是错的。assertEquals(5, calculator.getResult());就是来判断期待结果和实际结果是否相等，第一个参数填写期待结果，第二个参数填写实际结果，也就是通过计算得到的结果。这样写好之后，JUnit会自动进行测试并把测试结果反馈给用户。
六、忽略测试某些尚未完成的方法。
如果你在写程序前做了很好的规划，那么哪些方法是什么功能都应该实现定下来。因此，即使该方法尚未完成，他的具体功能也是确定的，这也就意味着你可以为他编写测试用例。但是，如果你已经把该方法的测试用例写完，但该方法尚未完成，那么测试的时候一定是“失败”。这种失败和真正的失败是有区别的，因此JUnit提供了一种方法来区别他们，那就是在这种测试函数的前面加上@Ignore标注，这个标注的含义就是“某些方法尚未完成，暂不参与此次测试”。这样的话测试结果就会提示你有几个测试被忽略，而不是失败。一旦你完成了相应函数，只需要把@Ignore标注删去，就可以进行正常的测试。
七、**Fixture（暂且翻译为“固定代码段”）**
Fixture的含义就是“在某些阶段必然被调用的代码”。比如我们上面的测试，由于只声明了一个Calculator对象，他的初始值是0，但是测试完加法操作后，他的值就不是0了；接下来测试减法操作，就必然要考虑上次加法操作的结果。这绝对是一个很糟糕的设计！我们非常希望每一个测试都是独立的，相互之间没有任何耦合度。因此，我们就很有必要在执行每一个测试之前，对Calculator对象进行一个“复原”操作，以消除其他测试造成的影响。因此，“在任何一个测试执行之前必须执行的代码”就是一个Fixture，我们用@Before来标注它，如前面例子所示：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)@Before
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)publicvoidsetUp()throwsException{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)calculator.clear();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

这里不在需要@Test标注，因为这不是一个test，而是一个Fixture。同理，如果“在任何测试执行之后需要进行的收尾工作”也是一个Fixture，使用@After来标注。由于本例比较简单，没有用到此功能。
JUnit4的一些基本知识就介绍到此，还有一些更灵活的用法放在本系列的高级篇中给大家介绍！

