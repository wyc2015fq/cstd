# APP测试面试题 - zhusongziye的博客 - CSDN博客





2018年02月27日 19:55:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：4024标签：[APP测试面试题](https://so.csdn.net/so/search/s.do?q=APP测试面试题&t=blog)
个人分类：[软件测试面试宝典](https://blog.csdn.net/zhusongziye/article/category/7131189)









年后肯定有不少小伙伴要换工作的额，希望对大家有所帮助。




一、开场问题：（自由发挥）
　　1、请自我介绍一下；
　　2、为什么离开上一个公司呢？
　　3、做测试多久了？以前做过哪些项目？你们以前测试的流程是怎样的？用过哪些测试工具？
　　4、你觉得为什么要在一个团队中开展软件测试工作，测试的价值提现在哪里？


二、技术方面：
1、代码能力：
　　1.1、请用自己最擅长的编程语言，将一个字符串反转并输出？
　　参考答案：
　　（1）python实现：
　　方法1、使用[::-1]：
　　s = 'python'
　　print s[::-1]
　　方法2、使用reverse()方法：
　　l = list(s)
　　l.reverse()
　　print ''.join(l)
（2）java实现：
　　方法1、使用StringBuffer，一点技巧也没有：
　　public String reverse(String s){
　　return new StringBuffer(s).reverse().toString();
　　}
　　方法2、灵活运用了字符串功能：
　　public String reverse(String s){
　　char[] letters=s.toCharArray();
　　char temp;
　　int len=letters.length;
　　for(int i=len/2-1;i>=0;i--){
　　temp=letters[i];
　　letters[i]=letters[len-1-i];
　　letters[len-1-i]=temp;
　　}
　　return new String(letters);
　　}
　　1.2、针对数十个安卓应用市场渠道包，请问如何进行大体的功能验证，简单叙述一下思路？
　　参考答案：可以自动化遍历某个目录下面的所有渠道包apk，然后循环：安装——》登录——》**操作——》退出——卸载
　2、测试思路：
　　2.1、给你一个模块，比如注册模块，你会怎么样设计与执行测试？
　　参考答案：数据——》从哪里来（入口）——》到哪里去（出口）——》数据库（检验数据的正确性）
　　2.2、请说一下你以前公司功能测试通过的标准？（自由发挥）
　　2.3、你认为app测试过程中，相对于web，要更多注意哪些测试点？或者说app测试和web测试有哪些不同之处？
　　参考答案：
　　1、“点击加载更多”的分页处理技术，是否有重复的数据，数据显示是否完整，到达最后一页后是否还有数据进行显示；
　　2、数据的排序方式；
　　2、界面跳转是否正确；
　　3、出现异常情况是否有提示，是否跳转到已经设定好的默认页面，如断网情况下，显示网络未连接，数据加载失败，或者如果此页面没有数据显示，显示友好提示信息；
　　4、图片处理的地方，是否容易出现程序崩溃现象，主要是图片压缩机制；
　　5、前台展示的数据，后台进行变动（增、删、改），是否是实时更新还是app一开始运行再进行加载；
　　6、前台主动发出请求，后台数据库中是否存在相应的数据同时包括数据的关联性（商家的会员进行下订单，数据库中生成一条订单的记录的同时，生成一条积分记录，该会员的积分进行相应的变化）；
　　7、手机app网络环境测试重点：主要是针对2G、3G、4G、wifi三种网络环境进行测试 ；
　　8、手机app兼容性测试：主要是针对android各个系统版本进行测试，及测试屏幕分辨率进行测试；
3、测试技术：
　　3.1、说说会用哪些抓包工具？怎么用的？
　　参考答案：比如fiddler，主要用来做app抓包使用，先在fiddler客户端做好各项配置，端口设置为8888，然后在手机上设置代理，就可以抓包了，主要看的是服务器返回的值、还能够修改传入参数、传出的参数、模拟网络延时，去构造不同场景。
　　3.2、什么是alpha测试和beta测试？
　　alpha测试：在公司场地，由员工主导的测试行为；beta测试：在客户场地，由客户主导进行的测试。
　　3.3、请写出一个SQL来查询分数前五5的所有人？
　　mysql：select * from 分数表 order by 分数 desc limit 5;
　　oracle：select top 5 * from 分数表 order by 分数 desc;
　　3.4、linux常用命令有哪些？
　　cd命令：切换到某个目录
　　ls命令：列出当前目录的所有文件、文件夹
　　pwd命令：列出当前目录的路径
　　cp命令：复制
　　mv命令：剪切
　　grep命令：管道
　　find命令：查找
　　rm命令：删除
　　ps命令：查看进程
　　kill命令：杀掉某个进程
　　cat命令：查看某文件
　　tar命令：打包
　　chmod命令：赋权限
　　chown命令：改变文件的所有者
　　vim命令：文本编辑
　　3.5、请说明Android手机和IOS手机，系统有什么区别？
　　1、两者运行机制不同：IOS采用的是沙盒运行机制，安卓采用的是虚拟机运行机制。
　　2、两者后台制度不同：IOS中任何第三方程序都不能在后台运行；安卓中任何程序都能在后台运行，直到没有内存才会关闭。
　　3、IOS中用于UI指令权限最高，安卓中数据处理指令权限最高。
　　3.6、请简要介绍一下安卓系统四层架构？
　　从上到下，依次是：应用程序层——》应用程序框架层——》系统运行库层——》Linux核心层
　　3.7、简单介绍一下Android SDK中自带的几个工具/命令的功能？
　　参考答案：
　　ddms：Dalvik Debug Monitor Service，是 Android 开发环境中的Dalvik[虚拟机]调试监控服务。
　　monkey：Android中的一个命令行工具，可以运行在模拟器里或实际设备中。它向系统发送伪随机的用户事件流(如按键输入、触摸屏输入、手势输入等)，实现对正在开发的应用程序进行压力测试。
　　uiautomator：UIAutomator是Eclipse自带的用于UI自动化测试工具，可仿真APP上的单击、滑动、输入文本等操作。
　　monitor：同uiautomator
　　adb：ADB的全称为Android Debug Bridge，就是起到调试桥的作用。通过ADB我们可以在Eclipse中方面通过DDMS来调试Android程序，就是debug工具。
　　3.8、常用的adb命令有哪些？请随便写一个monkey脚本，并解释其中参数的含义？
　　参考答案：
　　（1）adb devices、adb install、adb uninstall、adb shell pm 、adb shell am
　　（2）adb shell monkey -p com.xiaoniu.finance -s 123 --throttle 500 --ignore-crashes --ignore-timeouts --ignore-security-exceptions -v -v -v 2000 > d:\xnonline-monkey-test1.txt
　　-p：app包名、
　　--throttle：每个操作的间隔时间，单位ms
　　--ignore-crashes：忽略崩溃
　　--ignore-timeouts：忽略超时
　　--ignore-security-exceptions：忽略安全异常
　　-v -v -v：日志详细等级，3个v代表最详细等级的日志
　　d:\xnonline-monkey-test1.txt：代表将产生的日志，放到本地PC的D盘，并命名为：xnonline-monkey-test1.txt
　　3.9、测试过程中遇到app出现crash或者ANR，你会怎么处理？
　　参考答案：可以先把日志过滤出来： adb logcat | findstr xxxxx(过滤日志信息) ，然后再搜索其中的关键字，比如：exception、crash，看看是那些方法或者异常导致了问题的发送，初步定位问题原因后，可以交给开发人员去具体查找深层原因并修复。
　　3.10、请说一下以往工作中都曾具体做过哪些工作？其中最擅长哪部分工作（功能，自动化（UI和接口），性能）？这部分你是怎么做的？（自由发挥）
　　3.11、请简单介绍一下实用过的安卓UI自动化测试工具？
　　参考答案：
　　appium：是一个移动端的自动化框架，可用于测试原生应用，移动网页应用和混合型应用，且是跨平台的。
　　robotium：是一款国外的Android自动化测试框架，主要针对Android平台的应用进行黑盒自动化测试，它提供了模拟各种手势操作（点击、长按、滑动等）、查找和断言机制的API，能够对各种控件进行操作。
　　3.12、你觉得app的性能测试，即专项测试，需要重点关注那些方面？
　　参考答案：内存、cpu占用、耗电量、流量、流畅度等



