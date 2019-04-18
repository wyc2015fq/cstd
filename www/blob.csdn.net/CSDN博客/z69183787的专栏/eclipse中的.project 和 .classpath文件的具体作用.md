# eclipse中的.project 和 .classpath文件的具体作用 - z69183787的专栏 - CSDN博客
2014年09月12日 22:23:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：700
个人分类：[IDE-Eclipse](https://blog.csdn.net/z69183787/article/category/2176833)

.project是项目文件，项目的结构都在其中定义，比如lib的位置,src的位置,classes的位置
.classpath的位置定义了你这个项目在编译时所使用的$CLASSPATH
这些文件你用文本编辑器就能察看了
在一个项目中点刷新的目的是为了更新.project文件中的文件清单，让你把不通过eclipse提交到项目的文件显示出来
**.classpath 文件**
  从数据上我们容易看出，上面描述了工程的依赖文件：
- 源文件的具体位置（kind="src"）
- 运行的系统环境（kind="con"）
- 工程的library的具体位置信息(kind="lib")
- 在每个lib的xml子节点中，有关于它的其它配置信息（例如我配置的那个"javadoc_location"）
- 项目的输出目录(kind="output")
总体上说这个文件就是配置整个工程的运行环境。
**.project****文件**
 从数据上看，我们可以了解到这些配置主要是描述工程的基本信息：
- 工程名<name></name>
- 工程注释描述<comment></comment>
- 运行时需要的额外Eclipse插件<natures></natures>，及其具体加载方式信息<buildSpec></buildSpec>
如果你在开发过程中向工程里面加入了很多额外的插件，则必然会导致你的Eclipse启动速度变慢。在这种情况下，你可以到这个文件里面去掉一些插件，不过这样一来你在开启那些关联文件的时候会加载那些插件。
**  .settings 文件**
     里面存放各种插件的配置文件。
以上是简单介绍了Eclipse的Project相关的隐藏配置文件.
