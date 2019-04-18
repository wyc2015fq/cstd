# 各种文件格式(CSV、CFG、INC、deb) - wydbyxr的博客 - CSDN博客
2018年12月21日 10:15:52[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：33
个人分类：[计算机基础和编程基础](https://blog.csdn.net/wydbyxr/article/category/7751906)
# CSV
  CSV是以逗号间隔的纯文本文件CSV文件格式	CSV是以逗号间隔的纯文本文件
# cfg/CFG文件格式
  大多数情况下，很多程序都要保存用户的设置，办法有很多：注册表，日志文件… 而很多程序都使用了一个专用的文件。为了方便起见，常常命名为*.cfg，有时甚至直接命名为Config.cfg。
  这只是一个为开发及使用方便而"“发明”“的一个后缀名。所以，这种文件没有固定的格式，其实也并不能算作是一种文件类型。用途也仅仅是保存用户的设置，平常没有必要打开。
  同样，为了方便起见，里面的内容大多数时候也确实是文本。可以用任何文本编辑器打开，比如QUAKE的config.cfg。不妨用记事本打开看看，如果出现乱码，则不要去动它。”	CFG：控制流图（Control flow graph）是用在编译器中的一个抽象数据结构。它是一个过程或程序的抽象表现，由编译器在内部维护。嵌入式编程者一般以cfg命名寄存器配置的头文件等相关文件。
# .inc文件
  .inc 文件顾名思义是include file的意思。
  实际上，文件的后缀对于文件包含是无所谓你可以包含一个asp文件，也可以包含txt文。
  一般我们使用inc作为后缀，是因为这样能体现该文件的作用。
  【why inc file】
  .inc文件的作用有点类似于C/C++内的.H .HPP头文件，使用inc文件可以使我们的程序，增加可读性，更易于开发和维护。
# Lincese
  也就是注册码，对应的就是.lic文件。
  例如：正版Matlab在安装时需要lincese号，即注册码，即Standalone_License.lic文件。
# .deb文件
  .deb文件，直接双击即可安装
  终端命令安装：sudo dpkg -i lantern-installer-beta-64-bit.deb
