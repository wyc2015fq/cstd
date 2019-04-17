# Mysql安装过程中出现apply security settings错误的解决方法 - DoubleLi - 博客园






# 在学习Mysql的过程中，首先要安装Mysql。然而在第一遍安装过程中难免会出现安装错误的时候，当卸载后第二次安装（或者第三次甚至更多次）的时候，往往在安装最后一步会出现apply security settings错误。现在网上存在的普遍方法可能对绝大多数人的问题都不适用，究其原因是因为你没有将之前安装失败的残留文件彻底清除干净，最可能被你忽视的就是一个隐藏文件夹的配置你没有删除。    对于安装Mysql，这个网上都有教程，这个很简单。不同版本的安装步骤不同，这里就不详细列出。

#     出现apply security settings错误，必须手动清除所有相关文件！

#     我的电脑

#     下面的安装目录，可能不同系统文件目录的路径不一样。

# 第1步：删除Mysql的安装目录，默认为C:\Program Files目录下(如果你更改了安装路径，就到你所重新设置的路径下查找)。

# 第2步：删除Mysql的数据存放目录，一般在C:\ProgramData\MySQL目录下（需要注意这个文件夹默认是隐藏的，要通过查看->隐藏的项目，无论你的Mysql安装在哪一个盘下，C:\ProgramData\MySQL目录下都会有Mysql文件）。第3步：删除注册表数据，通过cmd窗口，输入regedit，删除以下几个文件：HKEY_LOCAL_MACHINE/SYSTEM/ControlSet001/Services/Eventlog/Applications/MySQLHKEY_LOCAL_MACHINE/SYSTEM/ControlSet002/Services/Eventlog/Applications/MySQLHKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Eventlog/Applications/MySQL

# 为了尽可能的删除干净你的注册列表，cmd---regedit---编辑---查找下一个---查找目标：mysql，仅将“查看”中的“项”打勾，删除所有mysql相关的记录。

# 第4步：重启电脑。

# 第5步：重新安装你的Mysql程序。安装完成后，你可以在系统的服务中查看，Mysql是否已经启动，如果没有启动，右击，点击启动（我的电脑->管理->服务与应用程序->服务），找到MySQL启动就行。









