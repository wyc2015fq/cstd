
# Python快速从注释生成文档的方法 - jiahaowanhao的博客 - CSDN博客


2018年06月01日 22:03:13[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：788


[Python快速从注释生成文档的方法](http://cda.pinggu.org/view/25702.html)
作为一个标准的程序猿，为程序编写说明文档是一步必不可少的工作，如何才能写的又好又快呢，下面我们就来详细探讨下吧。
今天将告诉大家一个简单平时只要注意的小细节，就可以轻松生成注释文档，也可以检查我们写的类方法引用名称是否重复有问题等。
一看别人专业的大牛们写的文档多牛多羡慕，不用担心我们可以让python为我们生成基本满足的说明文档，一来可以提高代码整体阅读性，二来可以将代码的整体结构看着也更清晰，这样在交接的时候可以省很多麻烦，其它同事在接手你工作的时候也不会一行行去问你这是什么那是什么的，因为注释已经很直观的表述了，在整合的时候可当说明文档给客户简单说明(主要是给你BOSS看的).
那么进入正题怎么实现看下面我简单的代码格式，注意注释的地方和函数名类名都是与__all__内相关联的
\#!/usr/bin/env python
\# -*-coding:utf-8 -*-
'''

文档快速生成注释的方法介绍,首先我们要用到__all__属性
在Py中使用为导出__all__中的所有类、函数、变量成员等
在模块使用__all__属性可避免相互引用时命名冲突
'''
__all__=['Login','check','Shop','upDateIt','findIt','deleteIt','createIt']
classLogin:
'''
测试注释一可以写上此类的作用说明等
例如此方法用来写登录
'''
def__init__(self):
'''
初始化你要的参数说明
那么登录可能要用到
用户名username
密码password
'''
pass
defcheck(self):
'''
协商你要实现的功能说明
功能也有很多例如验证
判断语句，验证码之类的
'''
pass
classShop:
'''
商品类所包含的属性及方法
update改/更新
find查找
delete删除
create添加
'''
def__init__(self):
'''
初始化商品的价格、日期、分类等
'''
pass
defupDateIt(self):
'''
用来更新商品信息
'''
pass
deffindIt(self):
'''
查找商品信息
'''
pass
defdeleteIt(self):
'''
删除过期下架商品信息
'''
pass
defcreateIt(self):
'''
创建新商品及上架信息
'''
pass
if__name__=="__main__":
importpythonzhushi
printhelp(pythonzhushi)
这里要强调说明下py的名字要和你当前工作的py名字一样(我这里创建的py名字就是pythonzhushi,最后导入也是这名字)，因为下面的测试中import的导入的是自身所以,im port 名称 和Help(名称)以及你的文件名称要一致才可以。为了更加直观请看下面这图：
![](http://cda.pinggu.org/uploadfile/image/20180601/20180601063329_93890.png)
当然还可以从其它文件引用的方式导入这里用到python常用的导入方式from从哪个目录import导入什么文件pythonzhushi
当然我这里还是用图片来展示以下：

![](http://cda.pinggu.org/uploadfile/image/20180601/20180601063336_19279.png)

是不是一目了然，这样也可以导入我们需要做的注释文档，help帮助方法就是帮我们查看到此文件中所包含的类，函数,的一个简单帮助文档，类似于readme文档的一个说明。最后来看看掩饰效果吧：

![](http://cda.pinggu.org/uploadfile/image/20180601/20180601063342_33437.png)

![](http://cda.pinggu.org/uploadfile/image/20180601/20180601063348_21962.png)
是不是简单的打印出我们程序的说明文档，后面步骤直接可以建一个文本把这个粘贴过去，就完成一个readme的文档说明，经过测试有几点注意是，想要写\#这种注释不要写到def放发内要写到它的上面位置和装饰器类似，相反三引号'''不要写到def外面,这样也是没显示效果的。大家赶紧去试试吧。感谢观看。。

