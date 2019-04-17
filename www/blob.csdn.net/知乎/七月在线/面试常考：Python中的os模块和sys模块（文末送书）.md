# 面试常考：Python中的os模块和sys模块（文末送书） - 知乎
# 



​

近些年随着Python语言越来越流行，越来越多的人选择Python语言作为自己的职业方向。如何在心仪公司的面试中获得好成绩，并最终成功获得offer是每一个Python开发者都要慎重对待的事情。今天，小编整理了Python程序员在面试中经常被问到os与sys模块问题。




首先，我们来看下，os模块与sys模块二者的简单区别：




**os与sys模块的官方解释如下：**




**os**: This module provides a portable way of using operating system dependent functionality.

这个模块提供了一种方便的使用操作系统函数的方法。





**sys**: This module provides access to some variables used or maintained by the interpreter and to functions that interact strongly with the interpreter.

这个模块可供访问由解释器使用或维护的变量和与解释器进行交互的函数。





**总结就是：**

os模块负责程序与操作系统的交互，提供了访问操作系统底层的接口;

sys模块负责程序与python解释器的交互，提供了一系列的函数和变量，用于操控python的运行时环境。







接下来，让我们一起看看，os与sys模块的常用方法有哪些？




**🔷   os模块 常用方法**




os.remove(‘path/filename’) 删除文件

os.rename(oldname, newname) 重命名文件

os.walk() 生成目录树下的所有文件名

os.chdir('dirname') 改变目录

os.mkdir/makedirs('dirname')创建目录/多层目录

os.rmdir/removedirs('dirname') 删除目录/多层目录

os.listdir('dirname') 列出指定目录的文件

os.getcwd() 取得当前工作目录

os.chmod() 改变目录权限

os.path.basename(‘path/filename’) 去掉目录路径，返回文件名

os.path.dirname(‘path/filename’) 去掉文件名，返回目录路径

os.path.join(path1[,path2[,...]]) 将分离的各部分组合成一个路径名

os.path.split('path') 返回( dirname(), basename())元组

os.path.splitext() 返回 (filename, extension) 元组

os.path.getatime\ctime\mtime 分别返回最近访问、创建、修改时间

os.path.getsize() 返回文件大小

os.path.exists() 是否存在

os.path.isabs() 是否为绝对路径

os.path.isdir() 是否为目录

os.path.isfile() 是否为文件







**🔷   sys模块 常用方法：**




sys.argv 命令行参数List，第一个元素是程序本身路径

sys.modules.keys() 返回所有已经导入的模块列表

sys.exc_info() 获取当前正在处理的异常类,exc_type、exc_value、exc_traceback当前处理的异常详细信

sys.exit(n) 退出程序，正常退出时exit(0)

sys.hexversion 获取Python解释程序的版本值，16进制格式如：0x020403F0

sys.version 获取Python解释程序的版本信息

sys.maxint 最大的Int值

sys.maxunicode 最大的Unicode值

sys.modules 返回系统导入的模块字段，key是模块名，value是模块

sys.path 返回模块的搜索路径，初始化时使用PYTHONPATH环境变量的值

sys.platform 返回操作系统平台名称

sys.stdout 标准输出

sys.stdin 标准输入

sys.stderr 错误输出

sys.exc_clear() 用来清除当前线程所出现的当前的或最近的错误信息

sys.exec_prefix 返回平台独立的python文件安装的位置

sys.byteorder 本地字节规则的指示器，bi-endian平台的值是'big',little-endian平台的值是'little'

sys.copyright 记录python版权相关的东西

sys.api_version 解释器的C的API版本

sys.version_info 元组则提供一个更简单的方法来使你的程序具备Python版本要求功能




以上就是小编整理出来的Python面试常用问题os模块和sys模块，供广大Python程序员参考，希望能提供一点助力。





9月份喽，各大企业狂招人的季节来呢，除三四月份之外还有“金九银十”的九月份和十月份喔，跳槽涨薪的好时机来喽！




希望每位Python程序员都能早日拿到  dream offer！







**今日推荐：**

【机器学习集训营第六期】，三个月挑战年薪四十万。我们集4邱童鞋更是拿到了45万offer，场场面试offer均拿下，人称“offer收割机”，点击查看：[45万offer面经](https://link.zhihu.com/?target=https%3A//ask.julyedu.com/question/85543)，查看他的学习经验和面试分享


早报名早优惠喔，报名即送《机器学习工程师 第八期》、《深度学习 第三期》，而且组团还能优惠喔，2人及2人以上组团报名，可各减500元，想组团者请加微信客服：julyedukefu_02，还在等什么，拿高薪从现在开始！！！ 点击立即报名：**[机器学习集训营第六期](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/train6)**




关注微信公众号：**七月在线实验室**，查看更多面试题和干货福利！




**福利来喽**：送Python畅销书，限时免费喔，想要的抓紧时间喽，点击查看**：[免费送书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/wtY0JHFvCzkVx2I1xs2dyw)**




