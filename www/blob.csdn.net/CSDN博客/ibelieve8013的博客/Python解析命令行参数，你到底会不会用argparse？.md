# Python解析命令行参数，你到底会不会用argparse？ - ibelieve8013的博客 - CSDN博客





2019年04月14日 16:32:20[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：22标签：[Python																[命令行解析																[argparse](https://so.csdn.net/so/search/s.do?q=argparse&t=blog)
个人分类：[python](https://blog.csdn.net/ibelieve8013/article/category/7648976)





为了提升效率，加上提高代码的可复用性，大型的任务中，我们一般不会直接把参数硬编码在程序中，而是通过命令行传入相应的参数，这样的方式更加灵活，且效率更高；甚至，我们都不会从命令行去传入参数，而是通过sh文件，shell脚本编程来传入相应的参数，当然这个不是本文的重点。无论如何，都需要解析命令行参数，那么python中解析命令行参数的模块主要就是argparse了（之前还有个optparse,不过模老珠黄， 一般我们直接用argparse就能满足日常的需要啦！）

首先，用Python的小伙伴肯定都知道一个模块sys吧，这个模块可以解析十分简单的命令行，怎么做到呢？

![](https://img-blog.csdnimg.cn/20190414163003216.png)

![](https://img-blog.csdnimg.cn/20190414163003236.png)

比如这段程序，就很简单地从命令行中读取了输入的参数，其中sys.argv[0]指的就是运行的文件名，其实也很容易理解，我们在命令行中输入的时候，如【python run.py 1 2 3 4 5】第一个参数不就是文件名吗。一句话，命令行传入的参数以列表的形式保存在sys.argv这个变量中！

理想总是丰满，现实总是骨感，当涉及到很多的参数，有些参数可以不用传入，有些参数是有一些固定的取值，有些参数是多个，有些参数有数据类型要求，你一个sys.argv岂能胜任？

接下来，隆重推出argparse模块

我们一般用argparse，最主要就用里面的ArgumentParser解析器。毋庸置疑，这是个类，我们需要先实例化。

![](https://img-blog.csdnimg.cn/20190414163003236.png)

然而，上面这个实例化，我们没有规定任何限制，其实它是有很多的参数的。

class ArgumentParser(prog=None, usage=None, description=None, epilog=None, parents=[], formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, argument_default=None, conflict_handler='error', add_help=True)

部分参数的解释：

**prog**：文件名，默认为sys.argv[0]，用来在help信息中描述程序的名称。

**usage**：描述程序用途的字符串

**description**：help信息前显示的信息

**epilog**：help信息之后显示的信息

**parents**：由ArgumentParser对象组成的列表，它们的arguments选项会被包含到新ArgumentParser对象中

**formatter_class**：help信息输出的格式，为了美观…

**prefix_chars**：参数前缀，默认为’-‘(最好不要修改)

**fromfile_prefix_chars**：前缀字符，放在文件名之前。这个东西是在参数实在太多，需要放在如一个TXT文件中时，那么用这个前缀字符来解析这个TXT文件，把内容作为参数传进来。比如可以用@符号，来作为文件的前缀名。

好了，我们挑重点的来说：

添加参数add_argument

参数的区别很重要，分为可选参数和位置参数。举例：

![](https://img-blog.csdnimg.cn/20190414163003235.png)

这样，第一个参数就是位置参数，给值的时候是按顺序来给的。

第二个参数是按标识‘-’来给的，比如 python run.py –age 17 Jamie 这样一条命令，17就给了age这个参数，而Jamie就给了name。

简而言之，先用短横线来认证可选参数，剩下的按顺序喂给位置参数。

【tip: 这里也可以用parse.parse_arg([‘-age’ 17 ‘Jamie’])这种方式来给parse传入参数，本质是一样的】另外，如果缺少位置参数，解析时是会报错的。

然后就是add_argument()中，有action选项，可以控制值存储的方式

**比如：**
- **store_const****：**值存放在const中：

parser.add_argument('--foo', action='store_const', const=42)
- **store_true****和store_false：**值存为True或False

![](https://img-blog.csdnimg.cn/20190414163003256.png)
- **append****：**存为列表，可以有多个参数

![](https://img-blog.csdnimg.cn/20190414163003304.png)
- 
**count****：**统计参数出现的次数


![](https://img-blog.csdnimg.cn/20190414163003303.png)
- 
**nargs****：**参数的数量


值可以为整数N(N个)，`*`(任意多个，可以为0个)，`+`(一个或更多)，有点像正则表达式

![](https://img-blog.csdnimg.cn/20190414163003369.png)

值为`?`时，首先从命令行获得参数，如果有`-y`后面没加参数，则从const中取值，如果没有`-y`，则从default中取值

![](https://img-blog.csdnimg.cn/20190414163003370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)
- 
**choices ****：设置参数值的范围，如果choices中的类型不是字符串，记得指定type**


parser.add_argument('x', type=int, choices=range(1, 4))
- 
**required**：该选项是否必选，默认为True

- 
dest ：参数名。


不清楚这里为什么还要搞个参数名，--后面的那个不就是参数名。

最后再举个例子，不想自己写了，例子来源[https://blog.csdn.net/lis_12/article/details/54618868](https://blog.csdn.net/lis_12/article/details/54618868)，

```python
#!/usr/bin/python

# -*- coding: utf-8 -*-

import sys

import argparse



def cmd():

    args = argparse.ArgumentParser(description = 'Personal Information ',epilog = 'Information end ')

    #必写属性,第一位

    args.add_argument("name",         type = str,                  help = "Your name")

    #必写属性,第二位

    args.add_argument("birth",        type = str,                  help = "birthday")

    #可选属性,默认为None

    args.add_argument("-r",'--race',  type = str, dest = "race",   help = u"民族")

    #可选属性,默认为0,范围必须在0~150

    args.add_argument("-a", "--age",  type = int, dest = "age",    help = "Your age",         default = 0,      choices=range(150))

    #可选属性,默认为male

    args.add_argument('-g',"--gender",   type = str, dest = "gender",    help = 'Your gender',         default = 'male', choices=['male', 'female'])

    #可选属性,默认为None,-p后可接多个参数

    args.add_argument("-p","--parent",type = str, dest = 'parent', help = "Your parent",      default = "None", nargs = '*')

    #可选属性,默认为None,-o后可接多个参数

    args.add_argument("-o","--other", type = str, dest = 'other',  help = "other Information",required = False,nargs = '*')



    args = args.parse_args()#返回一个命名空间,如果想要使用变量,可用args.attr

    print "argparse.args=",args,type(args)

    print 'name = %s'%args.name

    d = args.__dict__

    for key,value in d.iteritems():

        print '%s = %s'%(key,value)



if __name__=="__main__":

cmd()
```

命令行示例：

python argv_argparse.py xiaoming 1991.11.11 -p xiaohong xiaohei -a 25 -r han -g female -o 1 2 3 4 5 6

最后再总结一下，要用这个模块很简单，几条命令掌握就OK了，

parser = argparse.ArgumentParser() #实例化

然后只管添加参数就行了：

parser.add_argument('--age', defalut = 0)

然后传入可以用

args = parser.parse_args(['--age', 18])

当然一般是从命令行中传入，args= parser.parse_args(),再用args.__dict__取出所有参数，或者用args.age也可以取出特定参数。

精华就是实例化、添加参数、返回命名空间这三条命令，已经足够你去装逼了。。。](https://so.csdn.net/so/search/s.do?q=命令行解析&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




