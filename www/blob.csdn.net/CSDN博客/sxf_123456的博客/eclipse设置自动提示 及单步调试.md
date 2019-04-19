# eclipse设置自动提示 及单步调试 - sxf_123456的博客 - CSDN博客
2017年10月02日 13:28:04[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：133
个人分类：[java](https://blog.csdn.net/sxf_123456/article/category/6733911)
                
eclipse设置自动提示
1、eclipse安装成功后
2、点击Eclipse，上方工具栏中的Window,找到弹出的列表Preferences选项
3、依次找到Java->Editor->Content Assist
4、修改 Auto activation delay(ms):10
Auto activation triggers for Java:.AaBbCcEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz 
5、点击OK
Eclipse 单步调试
1、在程序左侧边框双击，设置断点
2、右击程序，选择Debug As 调试方式->Java Application,则可以进入调试模式
3、调试快捷键
F5 单步执行程序，遇到方法时进入
F6 单步执行程序，遇到方法时跳过
F7 单步执行程序，从当前方法跳出
F8 直接执行程序，遇到断点时暂停
4、修改变量的值
运行调试程序，跳出Variables窗口，选择某个变量，右键选择Change Value,修改变量的值
            
