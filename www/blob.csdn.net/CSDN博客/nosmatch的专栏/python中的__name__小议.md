# python中的__name__小议 - nosmatch的专栏 - CSDN博客
2011年08月17日 10:07:03[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：517标签：[python																[测试																[import																[扩展																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本](https://blog.csdn.net/HDUTigerkin/article/category/845589)
                
 所有的 Python 模块都是对象并且有几个有用的属性。 您可以使用这些属性方便地测试您所书写的模块。 下面是一个使用
if__name__ 的技巧。
if __name__ == "__main__":
在继续学习新东西之前, 有几点重要的观察结果。 首先, if 表达式无需使用圆括号括起来。 其次, 
if 语句以冒号结束, 随后跟随的是 [缩进代码](http://www.woodpecker.org.cn/diveintopython/getting_to_know_python/indenting_code.html)。
与 C 一样, Python 使用
== 做比较, 使用 = 做赋值。 与 C 不一样, Python 不支持行内赋值, 所以不会出现想要进行比较却意外地出现赋值的情况。
那么为什么说这个特殊的 if 语句是一个技巧呢？模块是对象, 并且所有的模块都有一个内置属性 
__name__。一个模块的 __name__ 的值要看您如何应用模块。如果 
import 模块, 那么 __name__ 的值通常为模块的文件名, 不带路径或者文件扩展名。但是您也可以像一个标准的程序一样直接运行模块, 在这种情况下
__name__ 的值将是一个特别的缺省值, __main__。
>>> import odbchelper>>> odbchelper.__name__'odbchelper'
一旦了解到这一点, 您可以在模块内部为您的模块设计一个测试套件, 在其中加入这个 if 语句。当您直接运行模块,
__name__ 的值是 __main__, 所以测试套件执行。当您导入模块,
__name__ 的值就是别的东西了, 所以测试套件被忽略。这样使得在将新的模块集成到一个大程序之前开发和调试容易多了。
在 
MacPython 上, 需要一个额外的步聚来使得 if
__name__ 技巧有效。 点击窗口右上角的黑色三角, 弹出模块的属性菜单, 确认 Run as __main__ 被选
