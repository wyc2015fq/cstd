# 循序渐进学Python2变量与输入 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年04月20日 13:36:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：476
新建一个test.py文件，右键选择“Edit with IDLE”，编辑完成后，Ctrl+S保存，然后按下F5就可以执行代码了。
注：IDLE是Python官方提供的一个IDE工具。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5#)] 
- [1 注释](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5#.E6.B3.A8.E9.87.8A)
- [2 数字](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5#.E6.95.B0.E5.AD.97)
- [3 变量](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5#.E5.8F.98.E9.87.8F)
- [4 输入用法](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5#.E8.BE.93.E5.85.A5.E7.94.A8.E6.B3.95)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5&action=edit&section=1)]注释
*#This is a comment***print**"This will run."*#comment*
多行注释使用三个单引号(''')或三个双引号(""")。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5&action=edit&section=2)]数字
**print **25 + 30 / 6**print**"Is it true that 3+2 < 5-7?"**print **3+2<5-7
输出：
30
Is it **true** that 3+2**<**5-7?
False
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5&action=edit&section=3)]变量
cars =100**print**"There are", cars,"cars available."
输出：
There are 100 cars available.
my_name ="Lei Jun"
your_name ="Jobs"**print**"Let's talk about",my_name
**print**"Let's talk about %s and %s."%(my_name, your_name)
输出：
Let's talk about Lei Jun
Let's talk about Lei Jun and Jobs.
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E5%BE%AA%E5%BA%8F%E6%B8%90%E8%BF%9B%E5%AD%A6Python%EF%BC%88%E4%BA%8C%EF%BC%89%EF%BC%9A%E5%8F%98%E9%87%8F%E4%B8%8E%E8%BE%93%E5%85%A5&action=edit&section=4)]输入用法
**print**"How old are you?"
age =raw_input()**print**"You're %s old."% age
输出：
How old are you?
27
You're 27 old.
或
age =raw_input("How old are you?")**print**"You're %s old."% age
输出：
How old are you?38
You're 38 old.
