# Python中常用的机器学习模块——Scikit-Learn - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:28:18[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1280










scikit-learn是一个用于机器学习的 Python 模块，建立在SciPy基础之上，获得3-Clause BSD 开源许可证。这个项目是由 David Cournapeau 在 2007 年发起的一个 Google Summer of Code 项目，从那时起这个项目就已经拥有很多的贡献者了，而且该项目目前为止也是由一个志愿者团队在维护着。

![](https://img-blog.csdn.net/20140927163041697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**主要特点：**


- 操作简单、高效的数据挖掘和数据分析
- 无访问限制，在任何情况下可重新使用
- 建立在NumPy、SciPy 和 matplotlib基础上
- 使用商业开源协议——BSD许可证



**重要链接：**


- 官方源代码报告：[https://github.com/scikit-learn/scikit-learn](https://github.com/scikit-learn/scikit-learn)
- HTML文档（稳定本）：[http://scikit-learn.org](http://scikit-learn.org/)
- HTML文档（开发版本）：[http://scikit-learn.org/dev/](http://scikit-learn.org/dev/)
- 下载版本：[http://sourceforge.net/projects/scikit-learn/files/](http://sourceforge.net/projects/scikit-learn/files/)
- 问题跟踪：[https://github.com/scikit-learn/scikit-learn/issues](https://github.com/scikit-learn/scikit-learn/issues)
- 邮箱列表：[https://lists.sourceforge.net/lists/listinfo/scikit-learn-general](https://lists.sourceforge.net/lists/listinfo/scikit-learn-general)



**相关性**


scikit-learn 经过测试之后可以运行在 Python 2.6、Python 2.7 和 Python 3.4平台上。除此之外，它还要适应运行在Python 3.3平台上。

**安装**


这个工具包使用的是distutils，这是安装Python模块的一种默认方式。如果要安装在你的根目录下面的话，可以使用：


python setup.py install --user




任何Unix/Linux用户安装的话可以使用：


python setup.py build
sudo python setup.py install



**测试**


安装后，你可以从外部源目录启动测试包（需要事先安装nosetests）：


$ nosetests --exe sklearn




- 更多信息可以看这里：[http://scikit-learn.org](http://scikit-learn.org/stable/install.html#testing)
- 相关案例：[http://scikit-learn.org/examples/](http://scikit-learn.org/stable/auto_examples/index.html)
- 项目主页：[http://scikit-learn.org](http://scikit-learn.org/)
- Github托管地址：[https://github.com](https://github.com/scikit-learn/scikit-learn)




