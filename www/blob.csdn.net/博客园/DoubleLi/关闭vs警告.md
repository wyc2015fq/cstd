# 关闭vs警告 - DoubleLi - 博客园






禁用所有编译器警告

当“解决方案资源管理器”中有项目选中时，在“项目”菜单上单击“属性”。

单击“编译”选项卡。

选中“禁用所有警告”复选框。

禁用单个编译器警告

在“解决方案资源管理器”中选定一个项目，然后在“项目”菜单中单击“属性”。

单击“编译”选项卡。

在“默认编译器选项”表中，将该警告的“通知”值设置为“无”。

将所有编译器警告视为编译错误

在“解决方案资源管理器”中选定一个项目，然后在“项目”菜单中单击“属性”。

单击“编译”选项卡。

选中“将所有警告视为错误”复选框。

将单个编译器警告视为编译错误

在“解决方案资源管理器”中选定一个项目，然后在“项目”菜单中单击“属性”。

单击“编译”选项卡。

在“默认编译器选项”表中，将该警告的“通知”值设置为“错误”



警告等级




警告等级

含义

0 关闭所有警告消息的显示。

1 显示严重的警告消息。

2 显示等级 1 警告以及某些不太严重的警告，如关于隐藏类成员的警告。

3 显示等级 2 警告以及某些不太严重的警告，例如有关总是计算为 true 或 false 的表达式的警告。

4（默认）显示所有等级 3 警告以及信息性警告。







项目属性---C/++ --->预处理定义下-》配置 -》下面添加_CRT_SECURE_NO_DEPRECATE和_SCL_SECURE_NO_DEPRECATE两个宏

或调用VS鼓吹的那些带“_s”后缀的非标准函数，即在其后加上"_S"，如sprintf_s

或文件最开始： 

#define _CRT_SECURE_NO_DEPRECATE 

或者在工程属性中设置。





#pragma warning

#pragma warning( disable : 4507 34; once : 4385; error : 164 )

等价于：

#pragma warning(disable:4507 34) // 不显示4507和34号警告信息

#pragma warning(once:4385) // 4385号警告信息仅报告一次

#pragma warning(error:164) // 把164号警告信息作为一个错误。

同时这个pragma warning 也支持如下格式：

#pragma warning( push [ ,n ] )

#pragma warning( pop )

这里n代表一个警告等级(1---4)。

#pragma warning( push )保存所有警告信息的现有的警告状态。

#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。

#pragma warning( pop )向栈中弹出最后一个警告信息，

在入栈和出栈之间所作的一切改动取消。例如：

#pragma warning( push )

#pragma warning( disable : 4705 )

#pragma warning( disable : 4706 )

#pragma warning( disable : 4707 )

//.......

#pragma warning( pop )

在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。










