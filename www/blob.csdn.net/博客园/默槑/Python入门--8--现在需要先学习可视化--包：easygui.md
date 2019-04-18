# Python入门--8--现在需要先学习可视化--包：easygui - 默槑 - 博客园







# [Python入门--8--现在需要先学习可视化--包：easygui](https://www.cnblogs.com/modaidai/p/6842871.html)





一、安装、了解easygui

下载地址：http://bbs.fishc.com/forum.php?mod=viewthread&tid=46069&extra=page%3D1%26filter%3Dtypeid%26typeid%3D403



安装：下载后解压缩，cmd下进入压缩后的文件夹，并输入 python setup.py install

说明文档：也在下载地址里面

二、调用此包

import easygui as g

三、做一个合格的程序猿

写个简单的程序：

1、　　msgbox(普通的窗口)

import easygui as g

g.msgbox("Hellow World")　　　　　　#这是出来一个窗口，easybox中的函数基本每个都会产生一个窗口

g.msgbox("我一定要学会编程!", ok_button="加油!",title="")　　#把ok按钮的字改为加油,还可以设置标题



2、　　ccbox(确认窗口，比如确定是否删除)


下面产生一个确认的窗口

if ccbox(): 　　　　　　　　　　#在各类按钮组件里，默认的消息是“shall i continue”,所以可以不加参数调用他们

        pass         # user chose to continue　　
else: 
        return      # user chose to cancel　　#当选择cancel或关闭窗口时，会返回一个布尔类型的值



再举个栗子

if ccbox('要再来一次吗？', choices=('要啊要啊^_^', '算了吧T_T')):
        　　msgbox('不给玩了，再玩就玩坏了......')
else:
        　　sys.exit(0) 　　　　　　　　　　　　　　　　# 记得先 import sys 哈

############################################

ynbox(msg='Shall I continue?', title=' ', choices=('Yes', 'No'), image=None) 　　 #这也是一种选择窗口

############################################

3、　　chiocebox(多选项窗口)

说出你的梦想。。。。。请选择！

 choices = ['愿意', '不愿意', '有钱的时候愿意']　　　　　　　　#会有三个选项你可以选择
 reply = choicebox('你愿意购买Q币支持腾讯吗？', choices = choices)



4、　　buttonbox(简单一点的多选项)

举个栗子

buttonbox(msg='', title='你喜欢那个品牌 ', choices=('gangben', 'duleisi', 'piaorou'), image=None,root=None)

选项里面你可以自己调整写什么，当用户点击任意一个按钮的时候，buttonbox()返回按钮的文本内容，如果用户关闭窗口，那么会返回默认选项，也就是第一个选项

栗子：

buttonbox('大家说我长得帅吗?', image='565be4cdea107.gif', choices=('帅', '不帅', '!#@$'))

添加了图片，美观一些。但仅支持gif格式



5、　　indexbox(基本跟上面一样)

先举栗子

indexbox(msg='Shall I continue?', title=' ', choices=('Yes', 'No'), image=None )

跟上面的区别就是：选择第一个按钮的时候返回序号0，选择第二个的时候返回序号1



6、　　boolbox(难道就没有一点新鲜的吗？怎么都差不多！)

boolbox(msg='Shall I continue?', title=' ', choices=('Yes', 'No'), image=None)

如果第一个按钮被选中则返回 1,否则返回 0。



7、multchiocebox()

跟choice函数差不多，提供一个可选择的列表。但这个multchiocebox()与之不同的是，multchiocebox()支持用户选择0个，1个或者同时选择多个选项。

multchiocebox()函数也是使用序列（元祖或者列表）作为选项，这些选项显示前会按照不区分大小写的方式排好序。

multchoicebox(msg='Pick as many items as you like.', title=' ', choices=('123','456'))



8、　　passwordbox(让用户输入密码)

passwordbox(msg='Enter Your Passwaord',title=' ',image=None,root=None)

会出来一个窗口，输入啥都显示为：******



9、　　multpasswordbox(可以输入用户名 密码)

g.multpasswordbox(msg,title,("用户名","密码"))    #事先命名好msg、title

还可以这样用

msg = "请输入用户名和密码"
title = "用户登录接口"
user_info = []
user_info = g.multpasswordbox(msg,title,("用户名","密码"))
print(user_info)



10、　　textbox(显示文本)

textbox(msg='Pick as many items as you like', title=' ', text='sadsad', codebox=0)

空白处显示：msg　　　　　　文本框显示：text



11、　　codebox(也是显示文本)

codebox(msg='', title=' ', text='')



12、　　diropenbox(可以选择路径)

diropenbox(msg='123', title=None, default=None)

此函数用于提供一个对话框,返回用户选择的目录名(带完整路径哦),如果用户选择"Cancel"则返回 None。



13、　　fileopenbox(也是用于打开文件)

fileopenbox(msg=None, title=None, default='*', filetypes=None)

关于 default 参数的设置方法:

　　　　　　　　　　　　 default 参数指定一个默认路径,通常包含一个或多个通配符。

　　　　　　　　　　　　如果设置了 default 参数,fileopenbox() 显示默认的文件路径和格式。

　　　　　　　　　　　　default 默认的参数是'*',即匹配所有格式的文件。

例如：

　　1、default="c:/fishc/*.py" 即显示 C:\fishc 文件夹下所有的 Python 文件。

　   2、default="c:/fishc/test*.py" 即显示 C:\fishc 文件夹下所有的名字以 test 开头的python文件

关于 filetypes 参数的设置方法:

　　1、可以是包含文件掩码的字符串列表,例如:filetypes = ["*.txt"]

　   2、可以是字符串列表,列表的最后一项字符串是文件类型的描述,例如:filetypes = ["*.css", ["*.htm", "*.html", "HTML files"]]



14、　　filesavebox()

提供一个对话框,让用户选择文件需要保存的路径(带完整路径哦),如果用户选择"Cancel"则返回 None。

filesavebox(msg=None, title=None, default='', filetypes=None)

　　　　default 参数应该包含一个文件名(例如当前需要保存的文件名),当然你也可以设置为空的,或者包含一个文件格式掩码的通配符。

　　　　filetypes 参数的设置方法请参考上边。



15、　　EgStore()

　　　　GUI 编程中一个常见的场景就是要求用户设置一下参数，然后保存下来，以便下次用户使用你的程序的时候可以记住他的设置。

　　　　为了实现对用户的设置进行存储和恢复这一过程，EasyGui 提供了一个叫做 EgStore 的类。为了记住某些设置，你的应用程序必须定义一个类（暂时称之为"设置"类，尽管你随意地使用你想要的名称设置它）继承自 EgStore 类。

　　　　然后你的应用程序必须创建一个该类的对象（暂时称之为"设置"对象）。

　　　　设置类的构造函数（__init__ 方法）必须初始化所有的你想要它所记住的那些值。

　　　　一旦你这样做了，你就可以在"设置"对象中通过设定值去实例化变量，从而很简单地记住设置。之后使用 settings.store() 方法在硬盘上持久化设置对象。



下面是创建一个"设置"类的例子：
- #----------------------------------------------------------------------
- # create "settings", a persistent Settings object
- # Note that the "filename" argument is required.
- # The directory for the persistent file must already exist.
- #-----------------------------------------------------------------------
- settingsFilename = os.path.join("C:", "FishCApp", "settings.txt")  # Windows example
- settings = Settings(settingsFilename)

下面是使用"设置"对象的例子：
- # we initialize the "user" and "server" variables
- # In a real application, we'd probably have the user enter them via enterbox
- user    = "奥巴马"
- server  = "白宫"
- # we save the variables as attributes of the "settings" object
- settings.userId = user
- settings.targetServer = server
- settings.store()    # persist the settings
- # run code that gets a new value for userId
- # then persist the settings with the new value
- user    = "小甲鱼"
- settings.userId = user
- settings.store()



16、　　exceptionbox()

使用 EasyGui 编写 GUI 程序，有时候难免会产生异常。当然这取决于你如何运行你的应用程序，当你的应用程序崩溃的时候，堆栈追踪可能会被抛出，或者被写入到 stdout 标准输出函数中。

EasyGui 通过 exceptionbox() 函数提供了更好的方式去处理异常，异常出现的时候，exceptionbox() 会显示堆栈追踪在一个 codebox() 中并且允许你做进一步的处理。

exceptionbox() 很容易使用，下面是一个例子：

try:
        　　print('I Love FishC.com!')
        　　int('FISHC') # 这里会产生异常
except:
        　　exceptionbox()














