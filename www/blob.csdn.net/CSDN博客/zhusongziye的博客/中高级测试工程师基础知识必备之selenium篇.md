# 中高级测试工程师基础知识必备之selenium篇 - zhusongziye的博客 - CSDN博客





2018年11月11日 13:14:06[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：155








**part 1:**

**引言：自动化永远是避不开的，反正你入职的岗位要不要用自动化，你必须得会一点，加分项。这一块包括，自动化一些理念和自动化的工具使用。**



**1.selenium中如何判断元素是否存在？**

答： isElementPresent

**2、selenium中hidden或者是display ＝ none的元素是否可以定位到？**

答：不能

**3、selenium中如何保证操作元素的成功率？也就是说如何保证我点击的元素一定是可以点击的？**

答：- 添加元素智能等待时间 driver.implicitly_wait(30)

- try 方式进行 id,name,clas,x path, css selector 不同方式进行定位，如果第一种失败可以自动尝试第二种

-Selenium保证元素成功率是通过元素的定位，当然它的定位方法很多，一定能有合适的。但是在自动化工程的实施过程中，高质量的自动化测试不是只有测试人员保证的。需要开发人员规范开发习惯，如给页面元素加上唯一的name,id等，这样就能大大地提高元素定位的准确性。当然如果开发人员开发不规范，我们在定位元素的时候尽量使用相对地址定位，这样能减少元素定位受页面变化的影响。只要我们元素定位准确，就能保证我的每一个操作符合我的预期

**4、如何提高selenium脚本的执行速度？**

答： Selenium脚本的执行速度受多方面因素的影响，如网速，操作步骤的繁琐程度，页面加载的速度，以及我们在脚本中设置的等待时间，运行脚本的线程数等。所以不能单方面追求运行速度的，要确保稳定性，能稳定地实现回归测试才是关键。

我们可以从以下几个方面来提高速度：

一，减少操作步骤，如经过三四步才能打开我们要测试的页面的话，我们就可以直接通过网址来打开，减少不必要的操作。

二，中断页面加载，如果页面加载的内容过多，我们可以查看一下加载慢的原因，如果加载的内容不影响我们测试，就设置超时时间，中断页面加载。

三，在设置等待时间的时候，可以sleep固定的时间，也可以检测某个元素出现后中断等待也可以提高速度。

四，配置testNG实现多线程。在编写测试用例的时候，一定要实现松耦合，然后在服务器允许的情况下，尽量设置多线程运行，提高执行速度。

**5、用例在运行过程中经常会出现不稳定的情况，也就是说这次可以通**过，下次就没办法通过了，如何去提升用例的稳定性？

答：

- time.sleep( )

- driver.implicitly_wait(30)

- 多用 try 捕捉，处理异常

-此时我们要分析出不稳定的原因，然后有针对性的去解决问题。主要有以下几个方面 ：

一，网速问题：有的时候网页加载的比较慢，在程序执行的时候要操作的元素没有显示出来。这种情况比较常见，运行一次网速好的时候通过了，再运行一次，页面没有打开，就不通过了。为了提高稳定性，我们只能牺牲运行时间了，在经常检测失败的元素前加上等待时间，等要操作的元素出现之后再执行下面的操作。

二，Selelnium的原因：Selenium1.0和2.0还是有区别的，有些儿函数在2.0下运行确实有时而有效，时面无效。如果mouseover()函数，就是这种情况， 我们需要避免使用这类的函数。

三，多线程的时候，测试用例间相互影响。虽然多线程的时候运行速度比较快，但是如果用例之间的耦合性没有设计好，也会影响的，如果用例A先于用例B执行的时候，就会影响到用例B；反之则没有问题。这种情况，如果你的自动化测试工程打算多线程的时候，提前就要把测试用例测试的耦合度比较松，尽量没有任何关系，因为多线程的执行顺序是不受控制的。



**6、你的自动化用例的执行策略是什么？**

答：

- 自动化测试用例的执行策略是要看自动化测试的目的，通常有如下几种策略：

一，自动化测试用例是用来监控的，在此目的下，我们就把自动化测试用例设置成定时执行的，如果每五分钟或是一个小时执行一次，在jenkins上创建一个定时任务即可。

二，必须回归的用例。有些儿测试用例，如BVT测试用例，我们在公司产品任何变动上线之前都需要回归执行。那我们就把测试用例设置成触发式执行，在jenkins上将我们的自动化测试任务绑定到开发的build任务上。当开发人员在仿真环境上部代码的时候，我们的自动化测试用例就会被触发执行。

三，不需要经常执行的测试用例。像全量测试用例，我们没有必要一直回归执行，必竟还是有时间消耗的，有些非主要业务线也不需要时时回归。这类测试用例我们就采用人工执行，在jenkins创建一个任务，需要执行的时候人工去构建即可。



**7、什么是持续集成？**

答：

- 持续集成源于极限编程(XP)，是一种软件实践，软件开发过程中集成步骤是一个漫长并且无法预测的过程。集成过程中可能会爆发大量的问题，因此集成过程需要尽可能小而多，实际上持续集成讲的是不断的去做软件的集成工作。持续集成，最简单的形式是包括一个监控版本控制（SVN等等）变化的工具。当变化被发觉时，这个工具可以自动的编译并测试你的应用。



**8、自动化测试的时候是不是需要连接数据库做数据校验？**

答：

- UI自动化不需要

- 接口测试会需要



**9、id,name,class,xpath, css selector这些属性，你最偏爱哪一种，为什么？**

答：- css 、xpath 几乎所有的元素都可以定位到



**10、如何去定位页面上动态加载的元素？**

答：- 触发动态加载元素的事件，直至动态元素出现，进行定位



**11、如何去定位属性动态变化的元素？**

答：- xpath或者css通过同级、父级、子级进行定位



**12、点击链接以后，selenium是否会自动等待该页面加载完毕？**

答：- 会的

**Part 2:**

**1.webdriver的协议是什么？**

答：The WebDriver Wire Protocol



**2.启动浏览器的时候用到的是哪个webdriver协议？**

答：http



**3.怎样去选择一个下拉框中的value＝xx的option？**

答：二次定位

**![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)**



**4.如何在定位元素后高亮元素（以调试为目的）？**

答：重置元素属性，给定位的元素加背景、边框



**5.什么是断言？**

答：

1.断言的英文是assertion，断言检查的英文是assertion checking。

2.断言是指定一个程序必须已经存在的状态的一个逻辑表达式，或者一组程序变量在程序执行期间的某个点上必须满足的条件。



**6.如果你进行自动化测试方案的选型，你会选择哪种语言，java，js，python还是ruby？**

答：使用自己熟悉的语言

![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)**![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)**



**7.page object设置模式中，是否需要在page里定位的方法中加上断言？**

答：不需要



**8.page object设计模式中，如何实现页面的跳转？**

答：get、click （可能有坑）



**9.自动化测试用例从哪里来？**

答：

1.手工用例中抽取

2.可以参考自动化用例的执行策略

![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**10.你觉得自动化测试最大的缺陷是什么？**

答：

1.不稳定

2.可靠性

3.不易维护

4.成本与收益



**11.webdriver可以用来做接口测试吗？**

答：有难度，不推荐



**12.get和post 的区别？（感觉可能答案不对）**

答：

-因为使用GET请求不会产生什么动作。不会产生动作意味着GET的HTTP请求不会在服务器上产生任何结果。但是安全方法并不是什么动作都不产生，这里的安全方法仅仅指不会修改信息。POST可能会修改服务器上的资源的请求。比如CSDN的博客，用户提交一篇文章或者一个读者提交评论是通过POST请求来实现的，因为再提交文章或者评论提交后资源（即某个页面）不同了，或者说资源被修改了。



两种请求方式的区别：

1.GET请求，请求的数据会附加在URL之后，以?分割URL和传输数据，多个参数用&连接。URL的编码格式采用的是ASCII编码，而不是uniclde，即是说所有的非ASCII字符都要编码之后再传输。

POST请求：POST请求会把请求的数据放置在HTTP请求包的包体中。上面的item=bandsaw就是实际的传输数据。因此，GET请求的数据会暴露在地址栏中，而POST请求则不会。



2.传输数据的大小

在HTTP规范中，没有对URL的长度和传输的数据大小进行限制。但是在实际开发过程中，对于GET，特定的浏览器和服务器对URL的长度有限制。因此，在使用GET请求时，传输数据会受到URL长度的限制。

对于POST，由于不是URL传值，理论上是不会受限制的，但是实际上各个服务器会规定对POST提交数据大小进行限制，Apache、IIS都有各自的配置。

3.安全性

POST的安全性比GET的高

![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**13.公司内一直在使用的测试系统（B/S架构）突然不能访问了，需要你进行排查并恢复，说出你的检查方法**

答：

一.网站输入域名直接无法访问，网站之前还正常，突然就无法访问

1. 测试FTP是否正常可以登录，不能登录的直接问空间商那是空间商的问题直接联系他们。

2. 空间赠送的三级域名是否能够访问网站打开网站（空间都赠送三级域名），如果也不能访问应该是空间问题。

3. 在电脑的开始菜单运行中输入cmd，在弹出的黑框中输入：ping 你的域名；然后回车，如果看不到IP或IP地址与你的主机地址不符，则说明域名解析有误,是域名的问题得重新解析域名。

二.访问报404错误（无法找到该页）。说明是网站内容都正常是程序出现问题，看看程序是否完整。

三.访问网站出现MySQL Server Error 这个是数据库链接错误，查看数据库连接文件和数据库是不是错误。

四.访问网站出现500错误。

1. 请登录FTP查看是否多了异常文件或丢失文件，说明网站被侵入了，马上联系网站制作进行进行排查故障。

2. 如果空间且FTP程序目录没有缺失文件或刚刚安装就出现500错误，请确认空间已开启scandir()函数，查看是不是禁止了这个函数。



**14.webdriver client的原理是什么？**

答：

 [Webdriver实现原理](http://www.cnblogs.com/timsheng/archive/2012/06/12/2546957.html)

通过研究selenium-webdriver的源码，笔者发现其实webdriver的实现原理并不高深莫测无法揣度。在这里以webdriver ruby binding的firefox-webdriver实现为例，简单介绍一下webdriver的工作原理。

当测试脚本启动firefox的时候，selenium-webdriver 会首先在新线程中启动firefox浏览器。如果测试脚本指定了firefox的profile，那么就以该profile启动，否则的话就新启1个profile，并启动firefox；firefox一般是以-no-remote的方法启动，启动后selenium-webdriver会将firefox绑定到特定的端口，绑定完成后该firefox实例便作为webdriver的remote server存在；客户端(也就是测试脚本)创建1个session，在该session中通过http请求向remote server发送restful的请求，remote server解析请求，完成相应操作并返回response；客户端接受response，并分析其返回值以决定是转到第3步还是结束脚本；这就是webdriver的工作流程，看起来很复杂实际上当了解了webdriver的实现原理后，理解上述问题应该比较简单。

webdriver是按照server – client的经典设计模式设计的。server端就是remote server，可以是任意的浏览器。当我们的脚本启动浏览器后，该浏览器就是remote server，它的职责就是等待client发送请求并做出相应；client端简单说来就是我们的测试代码，我们测试代码中的一些行为，比如打开浏览器，转跳到特定的url等操作是以http请求的方式发送给被 测试浏览器，也就是remote server；remote server接受请求，并执行相应操作，并在response中返回执行状态、返回值等信息；



**15.什么是page object设计模式？**

答：

-http://www.cnblogs.com/tsbc/p/4080301.html

 相似功能地方，代码基本都是一样的，界面元素换个查找方式，把原来的使用 xpath方式，改为使用 id 查找，需要对每个用例脚本都要改，虽然几个用例看不出什么工作量，但是重复findElement的代码，已经让我们感到了代码的笨重。如果某些定位发生了改变，我们就得贯穿整个测试代码进行调整元素定位，这样就会导致我们的脚本在后期，难以维护。因此通过Page Object Model 我们可以创建更加健壮代码，并减少或者消除重复的测试代码，从而也能够提高代码的可读性，减少编写脚本的工作量。Page Object Model的实现，就是通过分离测试对象和测试脚本的抽象来实现的。

**Part 3:**

**a.自动化测试框架都有哪些？**

1.模块化框架(test script modularity)

2.函数库结构框架(test library architecture)

3.关键字驱动测试框架(keyword-driven/table-driven testing)

4.数据驱动测试框架(data-driven testing)

5.混合型框架(hybrid test automation)



**b.id,name,class,xpath, css selector这些属性，你最偏爱哪一种，为什么？**

css 、xpath 几乎所有的元素都可以定位到 



**c.如何去定位页面上动态加载的元素？**

触发动态加载元素的事件，直至动态元素出现，进行定位



![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_ios/u1F4AA.0.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_ios/u1F4AA.0.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_ios/u1F4AA.0.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**d.如何去定位属性动态变化的元素？**

xpath或者css通过同级、父级、子级进行定位 



**e.提到Selenium和Sikuli有什么区别？**

Selenium：

它不能自动化视频播放器，音频播放器等闪存对象。

它有复杂的API

它没有视觉匹配

它只能自动化Web应用程序



**f.如何提高selenium脚本的执行速度？**

**Selenium脚本的执行速度受多方面因素的影响：**

- 网速；

- 操作步骤的繁琐程度；

- 页面加载的速度；

- 脚本中设置的等待时间；

- 运行脚本的线程数；

- 能稳定的视线回归测试是关键；





**提高速度的方法：**

减少操作步骤，如经过三四步才能打开我们要测试的页面的话，我们就可以直接通过网址来打开，减少不必要的操作。

中断页面加载，如果页面加载的内容过多，我们可以查看一下加载慢的原因，如果加载的内容不影响我们测试，就设置超时时间，中断页面加载。

在设置等待时间的时候，可以sleep固定的时间，也可以检测某个元素出现后中断等待也可以提高速度。

配置testNG实现多线程。在编写测试用例的时候，一定要实现松耦合，然后在服务器允许的情况下，尽量设置多线程运行，提高执行速度。



**g.做自动化测试是否需要连接数据库做数据校验？**

UI自动化测试不需要，接口自动化测试需要；

**Part 4:**

**一．如何提高selenium脚本的执行速度？**

1．少用sleep

2．多用显式等待方法

3．性能好的电脑执行



**二，用例在运行过程中经常会出现不稳定的情况，也就是这次可以通过，下次无法通过了？**

1．查找元素前先做判断：expected_conditions()里面的各种方法

2．显式等待：WebDriverWait()

注：这两者配合使用



**三，你的自动化用例的执行策略是什么？**

1．集成到jenkins一键执行，可以手动执行，也可以定时执行



![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**四．什么是持续集成？**

持续集成是一种软件开发实践，即团队开发成员经常集成他们的工作，通过每个成员每天至少集成一次，也就意味着每天可能会发生多次集成。每次集成都通过自动化的构建（包括编译，发布，自动化测试）来验证，从而尽早地发现集成错误。



**五．id，name，class，xpath，css selector这些属性，你最偏爱哪一种，为什么？**

css，css语法简洁，定位快（xpath语法长，定位慢，还不稳定）



**六．如何去定位页面上动态加载的元素？**

WebDriverWait()方法循环去查询是否元素加载出来了



![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**七．如何定位属性动态变化的元素？**

先去找该元素不变的属性，要是都变，那就找不变的父元素，同层级定位（以不变应万变）



**八．点击链接以后，selenium是否会自动等待该页面加载完毕？**

这个不会，没加载完也可以进行下一步操作



**九．如果有时候因网络较慢或其他原因导致定位不到其他元素，如何提高覆盖率？**

1．等待页面加载完成，隐式等待

2．显式等待



![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)![](https://res.wx.qq.com/mpres/htmledition/images/icon/common/emotion_panel/emoji_wx/2_14.png?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)



**十．显式等待和隐式等待的作用和区别**

显式等待：用于等待某个条件发生，然后再继续执行后续代码。显式等待是等元素加载

隐式等待：相当于设置全局的等待，在定位元素时，对所有元素设置超时时间。隐式等待是等页面加载，而不是元素加载（隐式等待就是针对页面的，显式等待是针对元素的）



