# 数据科学领域，你该选 Python 还是 R ？ - 文章 - 伯乐在线
原文出处： [usejournal](https://blog.usejournal.com/python-vs-and-r-for-data-science-833b48ccc91d)   译文出处：[开源中国](https://www.oschina.net/translate/python-vs-and-r-for-data-science?print)
![](http://jbcdn2.b0.upaiyun.com/2018/07/dd000831704fa240c532ce5ef32a7094.png)
根据需求，为了那些希望知道在数据科学方面选择 Python 还是 R 编程语言的人，我发布了这篇指导文章。你可能在数据科学方面是个新手，或者你需要在一个项目中选出一个语言，这篇文章可能会帮助到你。
*非免责声明：在最大的数据科学家雇主之一（Deloitte）中，我是一个数据科学家的管理者。我也对 R 和 Python 有几十年的了解。我是个语言不可知论者，但是参与到 Python 社区已经有15年左右了。*
### 还会有第三种选择
![](http://jbcdn2.b0.upaiyun.com/2018/07/6e7676ad2eb0bdc3e9b8ed0f511ccff6.png)
[Hadley Wickham,](https://twitter.com/hadleywickham) RStudio 的首席数据科学家，已经给出了答复“使用‘and’替代‘vs’”。由此，同时使用Python/R 是我将提到的第三种选择。这个选项引起了我的好奇心，而且我会在本文末尾介绍这一点。
### 如何比较 R 和 Python
下面是这两种语言之间一些值得比较的因素，这并不是一个完全的列表。
- **历史**：R 和 Python 具有明显不同的历史，有时候会交叉。
- **社区**：通过实际调查发现的很多复杂的社会人类学因素。
- **性能**：详尽的比较以及为什么比较起来这么难。
- **第三方支持**：模块，代码库，可视化，存储库，组织和开发环境。
- **用例**：有些任务和工作类型适合其中一种或者另一种。
- **我们不能和睦相处吗？**Python 调用 R 和 R 调用 Python 。
- **预测 R 还是 Python**：吃你自家的狗粮的一个预测练习。
- **偏好**：最终答案。
### 历史
![](http://jbcdn2.b0.upaiyun.com/2018/07/512dfeaefeef4add39dd3ba2263cfb4e.png)
简短概要：
- ABC -> Python 发布（1989 Guido van Rossum）-> Python 2 (2000) -> Python 3 (2008)
- Fortan -> S(贝尔实验室)-> R 发布（1991 Ross Ihaka 和 Robert Gentleman）-> R 1.0.0 (2000) ->R  3.0.2 (2013)
# 社区
当比较Python和R的用户时，首先要记住的就是：
![](http://jbcdn2.b0.upaiyun.com/2018/07/4a5c7a85268a6f124e0a614d84623792.jpg)
> 
只有50%的Python用户与R重叠
那是假定所有R程序员会用“科学和数字（Scientific and Numeric）”来称呼他。我们也确定，无论程序员的等级如何，这个分布都是正确的。
要进一步了解Python“宣传”，请阅读关于Python宣传调查结果：[https://www.linkedin.com/pulse/python-hype-survey-results-experience-any-drastic-decline-brian-ray/](https://www.linkedin.com/pulse/python-hype-survey-results-experience-any-drastic-decline-brian-ray/)
如果我们只看科学和数字社区，这就会把我们带到第二类社区，哪个社区？在所有的科学和数字社区中有一些子社区。尽管也许还会有一些重叠，因为你会怀疑他们与大一些的R/Python社区之间的交互方式确实不同。
一些使用Python/R的子社区的例子：
- 深度学习
- 机器学习
- 高级分析
- 预测分析
- 统计
- 探索和数据分析
- 学术可惜研究
- 几乎无穷无尽的[计算领域研究](https://en.wikipedia.org/wiki/Category:Computational_fields_of_study)
然而每个领域看起来都只致力于一个专门社区，你会发现R在如统计和探索之类的领域中更加流行。不久前，你可能会使用R进行构建运行或者做一些非常有意义的探索，而使用的时间比安装Python或者用它来做相同的探索的时候短得多。
> 
这一切都被颠覆性的技术改变了，他们是Jupyter notebook和Anaconda。
> 
注：Jupyter Notebokks：在浏览器中可以编辑Python/R代码；Anaconda：可以为Python和R简单的安装和打包
既然你可以在一个方便提供报告和现成的分析的环境启动运行，就已经排除了一个横在那些想要完成这些任务的人和他们喜爱的语言之间的障碍。Python现在可以使用独立于平台的方式打包，而且可以更快的提供快速、低成本的分析比。
在社区中影响了语言选择的另一个区别就是“开源”思想。不仅是开源库，还有致力于开源的协作社区的影响。讽刺的是，开源许可软件，像Tensorflow这样的软件到GNU Scientific Library（各自为Apache和GPL），他们看起来都有Python和R绑定。尽管有R的公共版权，还是有更多人纯粹的支持Python社区。另一方面，看起来有更多的企业支持R，特别是那些有统计方面历史的。
最后，考虑到社区和协作，在Github上Python的支持更多。如果我要看[最新Python包趋势](https://github.com/trending/python?since=monthly)，我会看到有超过3.5万个关注的Tensorflow之类的项目。相反，如果我看[R包的最新趋势](https://github.com/trending/r?since=monthly)，像Shiny，Stan…之类的包，他们都少于2千个关注。
### **性能**
性能提升很困难，因为有太多的指标和情况需要测试了，也很难基于特定的硬件来测试。一些操作在某个语言里已经做了优化，但其它语言里却还没有实现。确实，你可能会失去一些东西，比如：一些人会抱怨，一些人会离开，整个分析报告也可能会被丢弃。无论如何，生活还是要继续… …
### **循环**
在继续之前，让我们先看一下 Python 和 R 是怎么样使用的。在 R 中，你是如何做循环迭代的呢？R 语言有稍微的不同。
![](http://jbcdn2.b0.upaiyun.com/2018/07/3355ee45243cae7bedfce5d2e44d9c23.png)
![](http://jbcdn2.b0.upaiyun.com/2018/07/6c4072518b68eb0cbbd75c175bf2205e.png)
> 
通过一个快速的完整性检查, 包括加载时间和命令行运行时间: R 耗时 0m0.238s, Python 耗时是0m0.147s. 再次，这不是一个严谨的测试。
一个快速的测试显示 Python 代码会快很多，通常，这并不是太重要。
既然速度不是重点，那数据科学家更关心哪些东西呢？从这两门语言最新的趋势发现，它们被用作命令式语言的能力是一个重要的因素。比如，大多数 Python 程序员严重依赖 Pandas 来工作。这又引出了下一个主题：两种语言都有哪些模块和库，它们又是如何实现的？这是一个更有意义的比较。
### **第三方支持**
**包管理工具**
Python 使用 PyPi ，R 使用 CRAN ，Anaconda 同时支持二者。
CRAN 使用它内部的“install.packages”命令做分发管理。截至目前为止，CRAN 上有大约 12000 个有效的软件包。浏览一下你就会发现，大约二分之一的包是关于数据科学的，占了大约 6000 个还不止。
PyPi 上有超过 CRAN 十倍数量的包，大约 141000 个左右。其中有大约 3700 个包被标识为科学工程相关的。当然还有大量的包实际是科学相关的，但并没有被正确标识出来。
这两种语言好像并没有受到大量的重复劳动的影响。确实，当我在 PyPi 上搜索“随机森林”时，我搜到了 170 个项目，可是，这些包之间又有些许的不同。
尽管 Python 包的数量超过 R 十倍之多，但做数据科学计算的包的数量却差不多，也许 Python 更少一些。
大量有效的第三方库是非常重要的，所有东西都要从头写是非常痛苦的。同样地，我也希望你做一些工作来回馈社区。
**速度很重要**
DataFrames vs Pandas可能是一个更有意义和更重要的比较。
我们进行一个实验：在进行复制的时候进行一个复杂的遍历，比较两者的执行时间。下面是结果：
![](http://jbcdn2.b0.upaiyun.com/2018/07/72423d54950ec71a90fc32b568ea872a.png)
源代码： [http://nbviewer.jupyter.org/gist/brianray/4ce15234e6ac2975b335c8d90a4b6882](http://nbviewer.jupyter.org/gist/brianray/4ce15234e6ac2975b335c8d90a4b6882)
正如我们看到的结果，Python+Pandas要比原生的R DataFrames快很多。请注意这并不意味着Python要比R快。Pandas是基于C语言写的Numpy库的。
**想象一下这个！**
![](http://jbcdn2.b0.upaiyun.com/2018/07/b2ae77f0d63aed7a3a940ff313a73aba.png)
我真正想说的是ggplot2 vs matplotlib。声明：matplotlib是Python社区里我最看重的一个人写的，他教会了我Python，他就是 John D. Hunter。
Matplotlib是一个强大而且可个性化定制的库，虽然不太容易学但是扩展性非常好。ggplot不但不易个性化定制而且可以说更加困难。
如果你喜欢漂亮的绘图图案，而且并不需要自定义绘图，R是我的选择。如果你需要做更多的事情选择Matplotlib，他甚至可以帮助与bokeh进行交互。同样，你可能在寻找的ShinnyR对R而言也会增加其交互性。
### 难道我们不能同时使用两种语言吗？
有些人可能要问：你为什么不能同时使用两种语言呢？
有一些情况你可以同时使用这两个。比如当：
- 你的项目组或组织允许的时候。
- 你能比较容易地同时维护这两种环境。
- 你的代码不需要迁移到另一个系统。
- 你不会给别人制造一些混乱。
一些可以使两者同时工作的方法：
- Python 对 R 的包装器，比如：rpy2，pyRserve，Rpython，… (rpy2 扩展在 Jupyter 中有使用)
- R 也有一些包，比如：rPython，PythonInR，[reticulate](https://cran.r-project.org/web/packages/reticulate/index.html)，rJython，SnakeCharmR，XRPython
- 在 Jupyter 里，混合这两种语言，举例如下：
![](http://jbcdn2.b0.upaiyun.com/2018/07/eb609017fc373624a9477f7565f8e594.png)
然后，我们可以传递 pandas 数据帧，它会通过 rpy2 被自动转换为 R 数据帧，传递时加上 “-i df”开关。
![](http://jbcdn2.b0.upaiyun.com/2018/07/7456500daa401f5ea458c33adf684076.png)
代码源: [http://nbviewer.jupyter.org/gist/brianray/734bd54f468d9a6db9171b2cfc98405a](http://nbviewer.jupyter.org/gist/brianray/734bd54f468d9a6db9171b2cfc98405a)
### R 与 Python 预测
[Kaggle 上的一个用户](https://www.kaggle.com/nanomathias)写了一个关于[预测开发人员使用 R 还是 Python](https://www.kaggle.com/nanomathias/predicting-r-vs-python) 的内核。他根据这些数据得出了一些有趣的观察结果:
![](http://jbcdn2.b0.upaiyun.com/2018/07/fa7922d8d111cdfab58c1ca9efec415d.png)
- 如果你希望明年转向 Linux ，你更有可能是一个 Python 用户。
- 如果你研究统计学，你更有可能是 R 用户。如果你研究计算机科学，你可能是 Python 用户。
- 如果你年轻（18-24岁），你更可能是 Python 用户。
- 如果你进行代码竞赛，你更可能是 Python 用户。
- 如果你明年想使用安卓，你更可能是 Python 用户。
- 如果你明年想学习 SQL ，你更可能是 R 用户。
- 如果你使用 MS office ，你更可能是 R 用户。
- 如果你明年想使用 Rasperry Pi ，你更可能是一个 Python 用户。
- 如果你是全日制学生，你更可能是 Python 用户。
- 如果你使用敏捷方法，你更可能是 Python 用户。
- 如果你对 AI 的看法是担忧而不是兴奋，你更可能是 R 用户。
## 偏好
当我与Alex Martelli, Googler 和 Stack Overflow的统治者通信时，他向我解释为什么Google开始使用他们官方支持的一些语言。即使在像Google这样的自由精神创新空间，似乎有一些制度。这是在这里能起作用的偏好，公司偏好。
除了企业偏好，有些人在组织里经常创造第一。我知道在Deloitte第一个使用R语言的是谁。他仍然在公司，他是数据学家的领军人。重点是，在所有事情上我通常会建议，遵循你的爱。爱你所追随的，引领潮流，爱你所做的。
一个合格的声明，虽然我从未成为工具的第一思考着，但如果你正在做一写重要的事情，那可能不是做实验的最佳时机。错误是可能的。然而，每个精心的设计数据科学项目都给数据学家留下了一定的空间。使用其中的一部分来学习和实验。保持开源心态，拥抱多样性。
