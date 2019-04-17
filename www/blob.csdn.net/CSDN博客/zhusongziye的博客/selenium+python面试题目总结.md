# selenium+python面试题目总结 - zhusongziye的博客 - CSDN博客





2018年11月10日 22:11:41[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：322








**✎✎✎第一小节>>**



**1. WebDriver原理**

webDriver是按照client/server模式设计，client就是我们的测试代码，发送请求，server就是打开的浏览器来打开client发出的请求并做出响应。

具体的工作流程：

·webdriver打开浏览器并绑定到指定端口。启动的浏览器作为remote server.

·client通过commandExecuter发送httpRequest给remote server的的侦听端口（the webdriver wire control通信协议）

·Remote server根据原生的浏览器组件来转化为浏览器的native调用。

备注：启动浏览器的时候，使用的是http: hyper test transfer protocol



**2. Selenium 特点和组成**

Selenium是一个针对web应用的开源测试框架，它的测试用例可以用html table或者html 代码或者编程语言进行开发，而且他能在几乎所有的现在的浏览器上执行。Selenium可以被部署到windows.linux.mac平台上，支持的语言有java/python/ruby/.net/perl

特点：

·支持录制回放

·多种方式对页面元素进行定位并操作

·能够及进行debug和设置断点

·能够把测试脚本保存成html/ruby或者其他语言

·支持user-extensions.js形式的用户扩展

·能够进行页面的断言



1) Selenium IDE: 是firefox的插件，可以录制/回放/测试脚本

2) Selenium RC：支持程序语言（java/C#/python/ruby）编写测试用例; selenium和其他测试框架集成，比如java环境下将selenium和Junit集成，利用selenium来书写测试用例，用Junit来实现测试用例的自动化运行。

3) Selenium Core：是selenium的核心，是有js和html文件组成，它是selenium IDE和selenium RC的核心引擎。

4) Selenium Grid：是selenium的扩展，可以将一批selenium脚本分配到不同的测试机上同步运行。即selenium grid可以在不同的主机上建立主节点(hub)和分支节点(node), 可以使主节点上的测试用例在不同的分支节点上运行。对于不同的节点来说，可以搭建不同的测试环境（操作系统/浏览器）。



**3.如何提高自动化脚本稳定性**

找原因

1)网速原因，增加时间等待

2)函数原因，尽量少使用容易冲突的函数

3)配置testNG实现多线程，在编写测试用例的时候，一定要实现松耦合，在服务器允许的情况下尽量设置多线程运行，提高执行速度。



** 4.高质量自动化脚本特点**

业务和代码分离，封装型好。

自动化用例耦合性低，独立性强，易于扩展维护



** 5.自动化测试缺陷**

不稳定

可靠性不强

不易维护

成本与收益





** 6.自动化用例的执行策略**

根据自动化执行的目的来决定

（1）自动化测试用例执行用来监控，可以设置用例定时执行；

（2）用于回归测试，可以把测试用例设置成触发式执行；

（3）不需要经常执行的测试用例，可以由人工执行。



** 7. 提高selenium脚本的执行速度方式**

（1）减少操作步骤，减少不必要的操作；

（2）中断页面加载，如果页面加载内容过多并且加载的内容不影响我们测试，可以设置超时时间，中断页面加载；

（3）设置等待时间的时候，可以sleep()固定的时间，检测到元素出现后中断等待也可以提高速度；

（4） 配置testNG实现多线程，在编写测试用例的时候，一定要实现松耦合，在服务器允许的情况下尽量设置多线程运行，提高执行速度。



**8.元素定位**

1) Id定位  

find_element_by_id(‘kw’)   //通过id属性定位



2) Name定位   

find_element_by_name(‘wd’)    //通过名字进行定位，名字一般不唯一，一般不能通过其定位



3) ClassName定位    

find_element_by_class_name(‘cn’)   //通过类名进行定位



4) TagName定位       

find_element_by_tag_name(‘input’)  //标签，一般用于一类元素的定位



5) LinkText定位     

find_element_by_link_text(‘新闻’)  //文本链接定位



6) PartialLinkText定位   

find_element_by_partial_link_text(‘一个很长的’) //文本链接的部分文字



7) 绝对路径定位find_element_by_xpath(‘/html/body/div/div[2]/form/span/input[2]’) //绝对地址定位，利用层级定位

find_element_by_xpath(‘//input[@name=’n’ and id=’kw’]’) //当前页面某个目录下 name为n，id为kw的input标签，利用元素属性进行定位

find_element_by_xpath(‘//*[@id=kw]’) //其中*表示任何属性，表示id为kw的任何属性的元素

find_element_by_xpath(‘//span[@class=’c’]/input’) //层级和属性结合进行定位

find_element_by_xpath(‘//a[contains(href , ’test’)]’)



8) 相对路径定位

相对路径定位一般比绝对路径定位快。

find_element_by_css_selector(‘.class1’) // 类名定位 .class

find_element_by_css_selector(‘#id’) //id定位 #id

find_element_by_css_selector(‘input’) // 标签名定位

find_element_by_css_selector(‘[autocomplete=off]’) //属性定位

find_element_by_css_selector(‘span>input’) //标签名为span的input子元素





***✎✎✎第二小节***

9) 定位一组元素

Checkboxes = find_elements_by_tag_name(‘checkbox’)

for checkbox in Checkboxes:

checkbox.click()  # 通过for循环全选所有的checkbox

Checkboxes.pop().click()  #取消勾选一些checkbox，pop(-1)=pop()表示一组元素的最后一个；pop(0)      表示一组元素的第一个，pop(1)表示一组元素的第二个，以此类推



10) 父子/兄弟/相邻节点的定位

·父->子

Find_element_by_id(‘parent’).find_element_by_tag_name(‘input’) 串联寻找

Find_element_by_xpath(‘//input/div[@name=’n’]/span/input[1]’)  xpath的层级

Find_element_by_css_selector(‘div#b>input’) css selector的父子关系·子->父

Find_element_by_xpath(‘//div[@id=’c’]/../..’) 其中//div[@id=’c’]的父亲的父亲

·哥哥->弟弟

Find_element_by_xpath(‘//div[@id=’c’’/../div[1]) 找到弟弟后找父亲然后找哥哥

Find_element_by_css_selector(‘div#d+div’) 表示id=d的div后面紧跟着的div

Find_element_by_css_selector(‘div#d~div’) 表示id=d的div后面同一级所有div



11) 二次定位

Element1 = driver.find_element_by_id(‘ID1’)

Element2 = Element1.find_element_by_id(‘ID2’)



12) display: none 和hidden的区别

共同点：把网页中的某个元素隐藏起来；他们在selenium中都是定位不到的。

区别：display:none不为隐藏的对象保留其物理空间，该对象在这个页面上彻底

失，看不到/摸不到；hidden使对象在网页上不可见，但该对象在网页中依然占有空间，看不到/摸得到。

备注：isElementPresent()来判断元素是否存在。



** 9.常见控件使用**

1) link/button:

element.click()

注：ajax不能使用clickAndWait()函数，因为cickAndWait函数会在click之后等待页面重新加载完成，而ajax是部分刷新，所以这个页面不能重新加载完成。



2) Textbox:

element.send_keys(‘test’)



3) Upload

Driver.find_element_by_id(‘id’).send_keys(‘D\test.txt’)



4) Mouse Event:

e1=driver.find_element_by_id(‘kw’)

ActionChains(driver).double_click()/.context_click()/.drag_and_drop()/.move_to_eleme        nt()/.perform()   # 双击/右击/拖动/悬停/执行actionChains中的行为



 5) Dropdown:

·<Select>标签的下拉菜单

from selenium.webdriver.support.ui import Select

Select(driver.find_element_by_id(‘gender’)).select_by_index(1)

Select(driver.find_element_by_id(‘gender’)).select_by_value(“2”)

Select(driver.find_element_by_id(‘gender’)).select_by_visible_text(“Male”)

·非<Select>标签的下拉菜单

Dropdown1 = driver.find_element_by_id(‘id’) #先定位到dropdown

Dropdown1.find_element_by_id(“li2_input_2”) #再定位到dropdown中的值



 6) Alert:

driver.switch_to.alert.accept()//接受

driver.switch_to.alert.dismiss() //取消

Message=driver.switch_to.alert.text  //获取弹窗的文本消息

driver.switch_to.alert.send_keys(‘hello’) //输入值,这个在alert和confirm中不可用



 7) Window

Driver.refresh() 刷新

Driver.back() 后退

Driver.forward() 前进

Driver.maximize_window() 最大化

Driver.set_window_size(100,200) 设置窗口大小

Driver.switch_to.window(searchwindow)



 8) Frame

Driver.switch_to.frame(ReferenceFrame)

Driver.switch_to.parent_frame()  # frame需要一级一级切

Driver.switch_to.default_content()



***✎✎✎第三小节***



**10.等待**

1) 显式等待

等待某个条件成立时继续执行，每隔一段时间进行检测，超出最大时间    则抛出异常（程序每隔X秒看一眼，如果条件成立，执行下一步，否则等待，超出最大    时间，抛出异常）

From selenium.webdriver.common.by import By

From selelnium.webdirver.support.ui import WebDriverWait

From selenium.webdriver.support import expected_conditions as EC

E = WebDriverWait(driver, 5,0.5).until(EC.presence_of_element_located( (By.ID, ‘kw’) ))



2) 隐式等待

隐式等待中的时间并非一个固定的等待时间，它并不影响脚本的执行速度。比如进行某元素的定位时，如果元素可以定位就继续执行，如果目前定位不到就以轮询的方式持续判断该元素是否被定位到，如果超过规定的时间还没定位到就抛出异常。（甲约乙去吃饭，甲在这里等着，等了10分钟乙到达，则从10分钟之后开始下一步，假如等了20分钟，则20分钟后开始下一步，如果到了最大时间30分钟还没到就抛异常）

driver.implicitly_wait(20)



3) 强制等待

From time import sleep

Sleep(2)



**11.截图**

driver.get_screenshot_as_file('E:\sc.jpg')



**12.线性测试**

开发维护成本高



**13.模块化驱动测试**

将重复的操作独立为公共模块，尽量消除重复，提高测试用例的可维护性。



**14.数据驱动测试**

数据参数化

1) 读取txt文件

File= open(‘E:\wtest.txt’, ‘r’)

Lines = File.readlines()

For line in lines:

User = line.split(‘,’)[0]

Passw=line.split(‘,’)[1]

Print(User, Passw)



2) 读取csv文件

UnicodeDecodeError （未找到解决方案）



3) 读取xml文件



** 15.关键字驱动测试**

脚本/数据/业务 分离





** 16. page object设计模式**

是将page对象封装成一个HTML页面，通过提供的应用程序特定的API来操作页面元素，而不是在html中来搜寻对象，即提供一个易于编程的接口并隐藏窗口中底层的部件，对界面的交互细节进行封装。

优点是：减少了代码的重复/提高测试用例的可读性/提高测试用例的可维护性（特别是UI频繁变化的项目）

page object设置模式中，不需要在page里定位的方法中加上断言（why）



**17.Assert断言**

-断言的英文是assertion，断言检查的英文是assertion checking。

-断言是指定一个程序必须已经存在的状态的一个逻辑表达式，或者一组程序变量在程序执行期间的某个点上必须满足的条件。

Assert判断如果出现错误，则会中断整个测试；使用verify进行判断如果出现错误仍会继续执行直到结束。



