# 五大最实用的Jupyter Notebook扩展插件 - fighting！！！ - CSDN博客
2019年02月24日 23:06:12[dujiahei](https://me.csdn.net/dujiahei)阅读数：80
转自：[https://mp.weixin.qq.com/s?__biz=MzI3ODkxODU3Mg%3D%3D&mid=2247485697&idx=2&sn=cfd494920bc59c62e192ee475bc01b9d&chksm=eb4eedc2dc3964d40a6df962cde5fdfc6ccd2cbb88c423b24d479754b992e5430cdeebea9da4](https://mp.weixin.qq.com/s?__biz=MzI3ODkxODU3Mg%3D%3D&mid=2247485697&idx=2&sn=cfd494920bc59c62e192ee475bc01b9d&chksm=eb4eedc2dc3964d40a6df962cde5fdfc6ccd2cbb88c423b24d479754b992e5430cdeebea9da4)
Notebook扩展是一些能帮助你轻松把想要的内容/功能添加进Jupyter Notebook的插件。安装扩展最好的方法是Jupyter NbExtensions Configurator，它提供了一系列标签，只需勾选相应插件就能自动载入。
![](https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsj1MaqgyS8Ntlj6iaT6WSZjTWia0dj2VxTWe7O3KUJmK8e6kjTMCA9JsQg/640?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
*NbExtensions Configurator截图*
安装
用conda安装：
- 
`conda install -c conda-forge jupyter_contrib_nbextensions`
- 
`conda install -c conda-forge jupyter_nbextensions_configurator`
或者用pip：
- 
`pip install jupyter_nbextensions_configurator jupyter_contrib_nbextensions`
- 
`jupyter contrib nbextension install --user`
- 
`jupyter nbextensions_configurator enable --user`
更多配置细节，请前往jupyternbextensionsconfigurator的github。
1.Collapsible headings
这个扩展在大型Notebook中非常有用，可折叠的标题能帮你收起/放下Notebook中的某些内容，使整个页面看起来更干净整洁。
![](https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsjrkBZ8zo9y2D8VQuGRibibyiaPTKrDEPiaCflW25yT6VSVcqrg4pzRPUnqA/640?tp=webp&wxfrom=5&wx_lazy=1)
*Collapsible headings*
2.Notify
这是Jupyter Notebook中的通知机制，有时如果需要跑一些耗时较久的任务，你会把它放在一边自己去做别的事，而Notify功能就能在任务处理完后及时向你发送通知。
![](https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsjbLIqWPG283zQ22AWIzorGDMjek16WojnnicRo6NbNbTfqz4uOjRes3Q/640?tp=webp&wxfrom=5&wx_lazy=1)
*Notify*
需要注意的是，如果你想启用通知功能，首先你要勾选Notify扩展，其次再点击按钮正式启动。你选择的时间是Jupyter Notebook运行的最短时间，到点后它会向你发送通知。另外，请确保这个过程中Jupyter Notebook在浏览器中处于被打开状态。
3.Code folding
在Jupyter Notebook中，你能折叠的不只有标题，还有代码。
![](https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsjaWWnOCCSsMrd8iaREZV1I42Za2H1m6IgUDHDOOZdgGMLHq91icUdHadQ/640?tp=webp&wxfrom=5&wx_lazy=1)
*Code folding*
4.tqdm_notebook
这事实上不是Notebook原有的插件。tqdm原本是一个Python模块，它可以为循环代码显示执行进度条，但有时它无法在Jupyter Notebook上工作。几天前，Jupyter Notebook新加入了tqdm_notebook扩展，从此你就无需再为进度条担忧了，
![](https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsjMQoNPDXHMYChLnRmJNfl6gQia9hD6wGrVwqQn9fM0ccdHzibjHjplTUQ/640?tp=webp&wxfrom=5&wx_lazy=1)
*tqdm_notebook*
5.%debug
这也不是Notebook原生的。它最初是IPython magic的一个命令，支持两种激活调试器的方式：一是在执行代码之前激活调试器，二是在验尸模式下激活调试器。简而言之，就是当代码出现异常后，输入`%debug`可以直接激活调试器跳到出现错误的地方，而且你还可以检查前后代码情况。它实现了即时调试+快速迭代，更多细节可以参考Radek Osmulski的推文。
![](https://mmbiz.qpic.cn/mmbiz_jpg/hq0PKaHicMTGtHTOP2HBaEhWLIGNN9wsjafysog3c6y7QsaWc8JjPQdqq2EgiazbfjZuKUo9zdRZYia2GJJvEzVUg/640?tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
6.一些小型扩展和特殊技巧
- 
**%lsmagic**：执行%lsmagic，它会列出所有可用的IPython magics。
- 
**Zen mode extension**：隐藏活动状态栏，方便你把注意力集中在代码上。
- 
**Execute time extension**：显示运行的时间。
- 
**autoreload**：无需退出Jupyter Notebook就能动态修改代码。它的具体操作是：
- 
`%load_ext autoreload`
- 
`%autoreload 2`
除此之外，你还知道哪些好用的扩展吗？欢迎在留言中指出。
原文地址：ndres.me/post/best-jupyter-notebook-extensions/
