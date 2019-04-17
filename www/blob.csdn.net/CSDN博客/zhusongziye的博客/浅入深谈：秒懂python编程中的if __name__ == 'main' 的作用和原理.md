# 浅入深谈：秒懂python编程中的if __name__ == 'main' 的作用和原理 - zhusongziye的博客 - CSDN博客





2018年03月10日 10:47:57[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：30224










相信初学者在学习Python的过程中，不可避免的总会遇到

if __name__ == 'main'语句，虽然一直知道它的作用，但是可能一直比较模糊，今天菜鸟分析就与大家一起举例说说我的理解。

举个例子，我们在下面在test.py中写入如下代码：

```
print('恋习Python')

def main():
	print('恋习Python')
if __name__ == '__main__':
	main()
	print('跟着菜鸟分析，练习Python越练越恋')
运行代码，则输出结果为：


然后，在同一文件夹新建名称为import_test.py的脚本，只输入代码：
```
import test``运行代码，则输出结果为：
![](https://img-blog.csdn.net/20180310104719701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

与之前test.py代码运行结果比较，只有输出恋习Python，也就是if __name__=="__main__": 之前的语句被执行，之后的没有被执行。

这是为什么呢？别急，菜鸟分析继续给你举例子说明，绝对让你满意为止，最后有一种豁然开朗的感觉。

现在，我们在test.py脚本的if __name__=="__main__":之前加入print __name__，即将__name__打印出来，则最后运行结果如下：

![](https://img-blog.csdn.net/20180310104728645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看出，此时变量__name__的值为"__main__"。

再执行import_test.py，模块内容和执行结果如下：

![](https://img-blog.csdn.net/20180310104737337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时，import_test.py中的__name__变量值为test，不满足__name__=="__main__"的条件，因此，无法执行其后的代码。

哈哈，是不是一下明白啦，觉得不错，给菜鸟分析点个赞哦！

再仔细想想，其运行原理也就是：

由于每个python模块（python文件）都包含内置的变量__name__，当运行模块被执行的时候，__name__等于文件名（包含了后缀.py）。如果import到其他模块中，则__name__等于模块名称（不包含后缀.py）。而“__main__”等于当前执行文件的名称（包含了后缀.py）。所以当模块被直接执行时，__name__ == '__main__'结果为真；而当模块被import到其他模块中时，__name__ == '__main__'结果为假，就是不调用对应的方法。

简而言之就是：__name__ 是当前模块名，当模块被直接运行时模块名为 __main__ 。当模块被直接运行时，代码将被运行，当模块是被导入时，代码不被运行。


相信初学者在学习Python的过程中，不可避免的总会遇到

if __name__ == 'main'语句，虽然一直知道它的作用，但是可能一直比较模糊，今天菜鸟分析就与大家一起举例说说我的理解。

举个例子，我们在下面在test.py中写入如下代码：

```
print('恋习Python')

def main():
	print('恋习Python')
if __name__ == '__main__':
	main()
	print('跟着菜鸟分析，练习Python越练越恋')
运行代码，则输出结果为：


然后，在同一文件夹新建名称为import_test.py的脚本，只输入代码：
```
import test``运行代码，则输出结果为：
![](https://mmbiz.qpic.cn/mmbiz_jpg/Kad3LZzM7n4ttNkCUibYiaFIF7V6nw1L9FSkyxG0wfezq41IRECVDIP9am8Nkpuhnytb23L0NQeM9BFxEfGlREBg/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

与之前test.py代码运行结果比较，只有输出恋习Python，也就是if __name__=="__main__": 之前的语句被执行，之后的没有被执行。

这是为什么呢？别急，菜鸟分析继续给你举例子说明，绝对让你满意为止，最后有一种豁然开朗的感觉。

现在，我们在test.py脚本的if __name__=="__main__":之前加入print __name__，即将__name__打印出来，则最后运行结果如下：

![](https://mmbiz.qpic.cn/mmbiz_jpg/Kad3LZzM7n4ttNkCUibYiaFIF7V6nw1L9FwyoOQEvodNmmVVnXNXibVOZhl6vjX6IOynCsGBkk6PnJY5icRzicoCZlA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

可以看出，此时变量__name__的值为"__main__"。

再执行import_test.py，模块内容和执行结果如下：

![](https://mmbiz.qpic.cn/mmbiz_jpg/Kad3LZzM7n4ttNkCUibYiaFIF7V6nw1L9Fc5ar9ACFvicpiaaU06libmIOrfUpZ1ibmrZcWC9jlFrhcdaibiaItw10K0uw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

此时，import_test.py中的__name__变量值为test，不满足__name__=="__main__"的条件，因此，无法执行其后的代码。

哈哈，是不是一下明白啦，觉得不错，给菜鸟分析点个赞哦！

再仔细想想，其运行原理也就是：

由于每个python模块（python文件）都包含内置的变量__name__，当运行模块被执行的时候，__name__等于文件名（包含了后缀.py）。如果import到其他模块中，则__name__等于模块名称（不包含后缀.py）。而“__main__”等于当前执行文件的名称（包含了后缀.py）。所以当模块被直接执行时，__name__ == '__main__'结果为真；而当模块被import到其他模块中时，__name__ == '__main__'结果为假，就是不调用对应的方法。

简而言之就是：__name__ 是当前模块名，当模块被直接运行时模块名为 __main__ 。当模块被直接运行时，代码将被运行，当模块是被导入时，代码不被运行。




