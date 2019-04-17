# MATLAB的cd函数与cd命令 - 小灰笔记 - CSDN博客





2017年03月29日 20:20:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：15698
个人分类：[MATLAB](https://blog.csdn.net/grey_csdn/article/category/6639461)









       最初使用MATLAB的时候发现cd可以像Linux的shell命令一样使用，恰好cd又是我会的不多的几个shell命令之一。于是，用得到的时候就各种cd，如果不成功就简单修改摸索一下。那时候我确实是没有看过help，更没有去查比较详细的help文档。不过，后来，我发现很多MATLAB中的命令是命令的同时也是函数。

       慢慢地，使用函数的频率是要多余命令多一些。本来这个没有优劣之分，何况很多时候我甚至感觉不到他们的差异。不过，两者有差异是肯定的，不仅仅在于函数上面多了一个小括号。

       简单的探索摸索如下：

>> dir_str1= 'E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01';

>> dir_str2= 'E:\WorkSpace\01_编程语言\02_MATLAB';

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01



>>cd(dir_str2);

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB



       上述操作肯定是没有问题的，但是，如果把最后一个换成命令可以吗？其实，只要是稍微思索一下就可以得出否定的结论，因为这根切换到一个名称为dir_str2这种含义相冲突。简单测试如下：

>> cddir_str1

Error using cd

Cannot CD todir_str1 (Name is nonexistent or not a directory).



>> cddir_str2

Error using cd

Cannot CD todir_str2 (Name is nonexistent or not a directory).

       两次尝试都遇到了问题，最终失败。这种情况可以通过歧义来理解清楚了，那么换个角度，传入的参数如果是一个确切的目录字符串呢？

       测试1：

>> cdE:\WorkSpace\01_编程语言\02_MATLAB\matlab_01

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01



>> cd ../

>> cd'E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01'

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01

       如果使用命令，不管是目录还是字符串都能够识别并能够切换成功。

       测试2：

>> cd ../

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB



>>cd('E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01')

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01



>> cd ../

>> pwd



ans =



E:\WorkSpace\01_编程语言\02_MATLAB



>>cd(E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01)

 cd(E:\WorkSpace\01_编程语言\02_MATLAB\matlab_01)

      |

Error: UnexpectedMATLAB operator.

       最终，函数在非字符串属性的目录描述参数识别时候碰壁。通过这些尝试，总结一下cd函数与命令的使用方法如下：

       1，如果后面的目录表示形式是一个字符串常量，两种方法都可以用；

       2，如果后面的目录是一个不加引号的目录自然描述方式，只能够用命令；

       3，如果是一个存储了目录字符串的变量，只能够用函数。

       这里仅仅对cd的函数和命令进行了简单的尝试与总结，或许相应的结论可以拓展到MATLAB其他功能的理解，那些具有函数与命令“重载”属性的功能或许都有着类似的使用特点。



