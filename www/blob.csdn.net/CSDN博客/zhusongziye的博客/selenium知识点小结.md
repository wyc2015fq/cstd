# selenium知识点小结 - zhusongziye的博客 - CSDN博客





2017年09月17日 17:49:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：443







**环境：**

Python：3.6.1

selenium：3.4.3

Firefox：54.0.1

geckodriver:0.15.0

chrome：59.0.3

chromedriver：v3.1



**自动化测试基础**

**1、软件测试分类**

根据项目流程：单元测试（编码）、集成测试（设计）、系统测试（需求）、验收测试

根据代码是否可见：白盒测试、黑盒测试、灰盒测试

功能测试：逻辑功能测试、界面测试、易用性测试、安装测试、兼容性测试

性能测试：负载测试、压力测试

手工测试：手工执行测试用例

自动化测试：性能自动化和功能自动化

冒烟测试：在对系统进行正式测试之前，先验证主要功能是否实现，是否具备可测性

回归测试：修改代码之后确保没有引入新的错误，或导致其他代码产生错误

随机测试：为了模拟用户的操作，测试中输入的数据都是随机的，以便发现一些边缘性的错误

探索性测试：强调主观能动性，碰到问题时主动改变策略

安全测试：验证产品是否符合安全需求定义和产品质量标准

**2、什么项目适合自动化**

      1）项目需求变动不频繁（回归测试，每日构建后的测试验证）

      2）项目周期长

      3）自动化测试脚本可复用

**3、自动化测试分类和工具**

      1）分类：UI自动化、接口自动化、单元测试自动化

      2）工具：

             QTP：企业级自动化测试工具，支持B/S，C/S架构

             robot framework：Python语言编写，支持关键字驱动，分布式测试

             Watir：基于web模式的功能自动化测试工具

             selenium：支持多语言、多平台、多浏览器


**4、selenium工具**

       1）特点：多语言---Java、Python、PHP、ruby

                         多平台---Windows、Linux、Mac

                         多浏览器----Firefox、chrome、IE

                         开源、免费

                         简单、灵活


        2）selenium1.0

selenium IDE：实现浏览器操作的简单录制和回放，支持多种语言

selenium Grid：自动化测试辅助工具

                              并行执行

                              通过一个主机控制用例在不同环境、不同浏览器下执行

selenium RC：selenium的核心，支持多种编程语言编写脚本，通过selenium服务器作为代理服务器去访问应用，从而实现测试的目的。

selenium RC：client libraries---编写测试脚本，控制selenium server的库

                        selenium server---控制浏览器行为

selenium server：selenium core---一堆js函数，嵌入到浏览器中，通过这些函数控制浏览器操作

                            launcher---启动浏览器，并将selenium core加载到浏览器中，将selenium server的http proxy设置为浏览器的代理服务器



         3）selenium2.0

webdriver替代了selenium RC，为了向下兼容性，selenium RC仍然可以使用

selenium RC通过嵌入到浏览器中的js函数操作

webdriver针对各浏览器开发，通过原生浏览器支持或者浏览器扩展控制浏览器

简单自动化测试脚本


**Python webdriver API**


1、浏览器操作

     1）浏览器最大化

            driver.maximize_window()

      2）设置浏览器宽和高

             driver.set_window_size(400,800)

      3）控制浏览器前进、后退

            driver.forward()

            driver.back()



2、简单对象的定位

find_element_by_id()----唯一

find_element_by_name()----唯一

find_element_by_linx_text()----操作对象是文字超链接

find_element_by_partial_link_text()----操作对象是文字超链接

find_element_by_class_name()

find_element_by_tag_name()

find_element_by_xpath()

暂时可以通过firepath定位xpath路径，以后再研究

find_element_by_css_selector()

这个真的不懂，firebug里面可以定位



3、操作测试对象

clear()---------------清除内容，如默认用户名和密码

click()--------------  模拟鼠标点击操作

send_keys()------向输入框输入

submit()------------提交表单



4、WebElement接口常用方法

size--------------------返回元素尺寸

text----------------------获取元素文本信息

get_attribute(name)-------------获取元素某个属性值

is_displayde-----------------------该元素用户是否可见



5、鼠标事件

ActionChains类操作鼠标事件



ActionChains的使用

from selenium.webdriver.common.actionchains import ActionChains



ActionChains的执行原理

调用ActionChains方法的时候，用户行为不会立刻执行，而是将所有操作放在一个队列中，当执行perform()方法时，按照放入队列的顺序先进先出执行



ActionChains方法的书写格式

ActionChains(driver).context_click(操作对象).perform()



鼠标点击

context_click()------------------鼠标右击

double_click()------------------鼠标双击（应用场景：查看图片，双击选定文字）

click()-----------------------------鼠标点击

click_and_hold()---------------按住鼠标左键不懂



鼠标移动

move_to_element(目标元素)------------------------移动到某个元素

move_by_offset(xoffset,yoffset)--------------------移动到某个坐标



鼠标拖曳

drag_and_drop(source,target)------将元素从起点source移动到终点target

drag_and_drop_by_offset(source,xoffset,yoffset)-------按照坐标移动



6、键盘事件

Keys类操作键盘事件



Keys类的使用

from selenium.webdriver.common.keys import Keys



常用的组合键

send_keys(Keys.CONTROL,'a')----------------全选

send_keys(Keys.CONTROL,'c')----------------复制

send_keys(Keys.CONTROL,'v')----------------粘贴

send_keys(Keys.CONTROL,'x')----------------剪切



常用的非组合键

send_keys(Keys.ENTER)--------------------------------回车键

send_keys(Keys.BACK_SPACE)----------------------删除键

send_keys(Keys.SPACE)--------------------------------空格键

send_keys(Keys.TAB)------------------------------------制表键

send_keys(Keys.ESCAPE)------------------------------回退键

send_keys(Keys.F5)---------------------------------------刷新键



7、获得页面URL和title

1）获得当前页面title，判断页面跳转是否符合预期

title = driver.title

2）获得当前URL，一般用来测试重定向

url = driver.current_url



8、设置等待时间

sleep()：设置固定休眠时间。Python的time包提供sleep方法

implicitly_wait()：webdriver提供的一个隐性等待的时间，在一个时间段内只能的等待，超时则抛出异常

WebDriverWait()：webdriver提供的另一个方法，在设置时间内，默认每隔一段时间去检测页面元素是否存在，如果超出设置时间检测不到则抛出异常。


WebDriverWait(driver, timeout, poll_frequency=0.5, ignored_exceptions=None)

driver - WebDriver 的驱动程序(Ie, Firefox, Chrome 或远程)

timeout - 最长超时时间，默认以秒为单位

poll_frequency - 休眠时间的间隔（步长）时间，默认为0.5 秒

ignored_exceptions - 超时后的异常信息，默认情况下抛NoSuchElementException 异常。



9、定位一组对象

find_elements返回的是一个list

定位一组对象，一般应用于下列场景：

           批量操作对象，比如将页面上的CheckBox都选上

1）find_elements_by_tag_name()


2）find_element_by_css_selector()



10、定位frame中的对象

针对frame嵌套的情况，使用switch_to.frame(id/name/xpath)切换到被嵌套的frame中



11、多窗口处理

要想在多个窗口之间切换，首先要获得每一个窗口的唯一标识符号（句柄）。通过获得的句柄来区别分不同的窗口，从而对不同窗口上的元素进行操作

driver.current_window_handle----------------------获得当前窗口的句柄

driver.window_handles--------------------------------获得所有窗口的句柄

driver.switch_to_handle("句柄")---------------------切换回句柄所属的窗口

driver.close()---------------------------------------------关闭当前窗口

driver.quit()-----------------------------------------------关闭所有窗口



12、下拉菜单处理

1）传统下拉菜单

先定位到下拉菜单，再点击选项

2）下拉菜单需点击才能显示选项

有两次点击动作，第一次点击下拉菜单，第二次点击选项

3）下拉菜单不需点击，鼠标放上去就会显示选项，则可以使用move_to_element()方法定位

4）针对下拉菜单标签是select的



导入Select类：from selenium.webdriver.support.select import Select

使用方法：Select(driver.find_element_by_name("xxx")).select_by_index()

选择列表：

select_by_index(index)---------------------------根据index属性定位选项，index从0开始

select_by_value(value)---------------------------根据value属性定位

select_by_visible_text(text)----------------------根据选项文本值来定位

first_selected_option()----------------------------选择第一个选项



清除列表

deselect_by_index(index)---------------------------根据index属性清除选定的选项，index从0开始

deselect_by_value(value)---------------------------根据value属性

deselect_by_visible_text(text)----------------------根据选项文本值

deselect_all()--------------------------------------------清除所有选项



13、上传文件

上传过程一般是打开一个系统的window窗口，从窗口选择文件添加，一般会卡在如何操作Window窗口。其实，没那么复杂，只要定位上传按钮，通过send_keys()添加文件路径就可以了。

上传控件标签为input



上传控件标签为非input

可以借助第三方工具：Autolt



14、下载文件

webdriver允许设置默认下载路径，从而可以跳过下载弹窗提示

1）chrome下载

思路：实例化一个option对象

           设置配置，并加载到option中

           启动浏览器，点击下载链接

优势：使用谷歌浏览器下载，不需要针对各种文件类型进行配置



2）Firefox下载

对于Firefox，需要我们设置其Profile：

browser.download.dir：指定下载路径

browser.download.folderList：设置成2表示使用自定义下载路径；设置成0表示下载到桌面；设置成1表示下载到默认路径



browser.download.manager.showWhenStarting：在开始下载时是否显示下载管理器

browser.helperApps.neverAsk.saveToDisk：对所给出文件类型不再弹出框进行询问

Firefox需要针对每种文件类型进行设置，这里需要我们查询对应文件的MIME类型，可以用以下链接进行查询：MIME 参考手册



15、调用js和控制浏览器滚动条

1）webdriver提供了execute_script(script,*args)方法调用js

2）控制浏览器滚动条

应用场景：判断法律文件是否阅读完，滚动条拉倒最下面，表示用户已看完

                   要操作的元素不在视觉范围内，需要拖动滚动条

火狐浏览器



谷歌浏览器



16、处理cookie

driver.get_cookies()-------------------------------获得所有cookie

driver.get_cookie(name)-------------------------获得name属性的cookie

driver.add_cookie(cookie_dic)-----------------添加cookie（cookie格式为字典，）

driver.delete_cookie(name)---------------------删除特定cookie

driver.delete_all_cookies()----------------------删除所有cookie



17、验证码问题

跳过验证码的方法：

1）去掉验证码

2）设置万能码

3）通过cookie跳过验证码登录



18、webdriver原理

1）WebDriver 启动目标浏览器，并绑定到指定端口。该启动的浏览器实例，做为web driver 的remote server。

2）Client 端通过CommandExcuter 发送HTTPRequest 给remote server 的侦听端口（通信协议： the webriver wire protocol）

3）Remote server 需要依赖原生的浏览器组件（如：IEDriverServer.exe、chromedriver.exe），来转化转化浏览器的native 调用。



**自动化测试模型**

1、自动化测试模型介绍

1）模块化与类库

将脚本中重复可复用的部分拿出来写成一个公共的模块，需要的时候就调用它，可以大大提高测试人员编写脚本的效率。如将登录和退出模块化

模块化的优点：提高效率，不用编写重复脚本

                         需要修改代码时，只需要修改模块代码，调用模块的代码不需要修改



2）数据驱动

数据驱动可理解成参数化，输入数据的不同从而导致输出结果的不同

将数据与脚本分离



3）关键字驱动

通过关键字的改变引起结果的改变

QTP、robot framework 等都是以关键字驱动为主的自动化工具



2、登录模块化



3、数据驱动（参数化）

参数化方式：读取TXT文件和CSV文件、函数、字典 

1）读取TXT文件



分别打开两个文件，将用户名和密码赋值给变量，传递给输入框



缺点：用户名和密码在不同的文件中，修改较麻烦

           文件中只能保存一个用户名和密码，不能循环读取



2）登录参数化（函数）

user_info.py



登录模块中，通过2个变量来接受函数返回的值（用户名、密码）



3）登录参数化（字典）

user_info.py



登录模块通过字典的键值对取值



4）表单参数化

通过WPS 或excel 创建表格，文件另存为选择CSV 格式



csv.reader()用于读取CSV 文件，user[0] 表示表格中第一行第一列的数据（用户名），user[1]表示表格中第一行第二列的数据（邮箱），后面类推。通过CSV 读取文件比较灵活，可以循环读取每一条数据，从而又不局限每次所读取数据的个数。


**自动化测试用例设计**



1、手工测试用例和自动化测试用例



手工测试用例

 较好的异常处理能力，能通过人为的逻辑判断校验当前步骤的功能实现正确与否。

 人工执行用例具有一定的步骤跳跃性。

 人工测试步步跟踪，能够细致的定位问题。

 主要用来发现功能缺陷



自动化测试用例

 执行对象是脚本，任何一个判断都需要编码定义。

 用例步骤之间关联性强。

 主要用来保证产品主体功能正确完整和让测试人员从繁琐重复的工作中解脱出来。

 目前自动化测试阶段定位在冒烟测试和回归测试。



2、测试类型

1）测试静态内容

用于验证静态性的、不变化的UI元素的存在性

如，页面底部备案信息，以及页面顶部的图片



2）测试链接

如果页面链接经常变化或文件不时被重定向，可以尝试自动化测试



3）功能测试

功能测试通常是需要自动化测试的最复杂的测试类型,但也通常是最重要的。典型的测试是登录,注册网站账户,用户帐户操作,帐户设置变化,复杂的数据检索操作等等。功能测试通常对应着您的应用程序的描述应用特性或设计的使用场景。



4）测试动态元素



5）ajax测试

Ajax 是一种支持动态改变用户界面元素的技术。页面元素可以动态更改,但不需要浏览器重新载入页面,如动画,RSS 源,其他实时数据更新等等。



6）断言assert和验证verify



断言的优点和缺点：

优点:你可以直截了当地看到检查是否通过。

缺点:当检查失败,后续的检查不会被执行,无法收集那些检查的结果状态



验证的优点和缺点：

优点：遇到失败时，测试不会终止

缺点：不能得到测试失败的相关反馈



及时得到反馈会更合适,因此断言通常比验证更常使用。



3、Python异常断言 

1）异常的抛出机制：

1> 如果在运行时发生异常，解释器会查找相应的处理语句（称为handler）.



2> 要是在当前函数里没有找到的话，它会将异常传递给上层的调用函数，看看那里能不能处理。



3> 如果在最外层（全局“main”）还是没有找到的话，解释器就会退出，同时打印出traceback 以便让用户找到错误产生的原因。



注意：虽然大多数错误会导致异常，但一个异常不一定代表错误，有时候它们只是一个警告，有时候它们可能是一个终止信号，比如退出循环等。



2）捕捉异常

try....except...

输出报错信息


try...finally...

finally语句必须要要执行，比如：文件关闭，把数据库连接返回给连接池

如图，按Ctrl+C中断程序

finally中的语句仍然被执行

3）raise抛出异常




4、webdriver截图

Webdriver 提供错误截图函数get_screenshot_as_file()，可以帮助我们跟踪bug，在脚本无法继续执行时候， get_screenshot_as_file()函数将截取当前页面的截图保存到指定的位置



5、编写自动化测试实例


**编写自动化测试用例的原则：**

1、一个脚本是一个完整的场景，从用户登陆操作到用户退出系统关闭浏览器。


2、一个脚本脚本只验证一个功能点，不要试图用户登陆系统后把所有的功能都进行验证再退出系统


3、尽量只做功能中正向逻辑的验证，不要考虑太多逆向逻辑的验证，逆向逻辑的情况很多（例如手号输错有很多种情况），验证一方面比较复杂，需要编写大量的脚本，另一方面自动化脚本本身比较脆弱，很多非正常的逻辑的验证能力不强。（我们尽量遵循用户正常使用原则编写脚本即可）


4、脚本之间不要产生关联性，也就是说编写的每一个脚本都是独立的，不能依赖或影响其他脚本。


5、如果对数据进行了修改，需要对数据进行还原。


6、在整个脚本中只对验证点进行验证，不要对整个脚本每一步都做验证。


**unittest单元测试框架**

1、引入单元测试框架

测试类：

setup()方法要在每个方法执行前执行一次

teardown()方法在每个方法执行后执行一次
            


