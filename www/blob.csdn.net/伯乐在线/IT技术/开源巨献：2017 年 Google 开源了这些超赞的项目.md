# 开源巨献：2017 年 Google 开源了这些超赞的项目 - 文章 - 伯乐在线
原文出处： [开源中国编辑部](https://my.oschina.net/editorial-story/blog/1622423)
春节小长假，走亲访友之余，还可以利用一些零碎的时间收集学习资料，给自己充充电，好在 2018 年成为更优秀的自己。我们知道在开源和贡献开源方面，Google 一直是行业的典范。2017 年它开源了哪些优秀的软件，有哪些值得我们借鉴学习呢？让我们一起来看下：
### **[1、硬件加速的机器学习 JavaScript 库 DeepLearn.js](https://www.oschina.net/p/deeplearn-js)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/5fdd5ddc8d33cc51492da2ce36abbb8a.gif)
DeepLearn.js 是 Google 推出的一个可用于机器智能并加速 WebGL 的开源 JavaScript 库，完全在浏览器中运行，不需要安装，不需要后端处理。
### **[2、大型微服务系统管理工具 Istio](https://www.oschina.net/p/istio)**
Istio 是一个由谷歌、IBM 与 Lyft 共同开发的开源项目，旨在提供一种统一化的微服务连接、安全保障、管理与监控方式。Istio 项目能够为微服务架构提供流量管理机制，同时亦为其它增值功能（包括安全性、监控、路由、连接管理与策略等）创造了基础。这款软件利用久经考验的 Lyft Envoy 代理进行构建，可在无需对应用程序代码作出任何发动的前提下实现可视性与控制能力。Istio 项目是一款强大的工具，可帮助 CTO/CIO 们立足企业内部实施整体性安全、政策与合规性要求。
### **[3、可自动生成命令行接口 Python Fire](https://www.oschina.net/p/python-fire)**
Python Fire 是 Google 开源的一个可从任何 Python 代码自动生成命令行接口（CLI）的库。
**简易示例：**


```
import fire
class Calculator(object):
  """A simple calculator class."""
  def double(self, number):
    return 2 * number
if __name__ == '__main__':
  fire.Fire(Calculator)
```
然后，你可以运行：


```
python calculator.py double 10  # 20
python calculator.py double --number=15  # 30
```
### **[4、](http://www.oschina.net/p/fold)[谷歌深度学习库](https://www.oschina.net/p/fold)[](http://www.oschina.net/p/fold)[TensorFlow Fold](https://www.oschina.net/p/fold)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/ef09d1435cf9037de0d3f937e7d037a3.gif)
TensorFlow Fold 是用于创建使用结构化数据的 TensorFlow 模型库，其中计算图的结构取决于输入数据的结构。
TensorFlow Fold 使得处理不同数据尺寸和结构的深度学习模型更容易实现。Fold 实现动态批处理，变换任意形状的计算图形的批次以产生静态计算图形。 该图具有相同的结构，而不管其接收到什么输入，并且可以通过 TensorFlow 有效地执行。
### **[5、Go 运行 Python：Grumpy](https://www.oschina.net/p/grumpy)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/6730624316ecb500a0dfd2ea065bc979.png)
Grumpy 是一个 Python to Go 源代码翻译编译器和运行时，旨在替代 CPython 2.7。
关键的区别是它将 Python 源代码编译为 Go 源代码，然后将其编译为本机代码，而不是字节码。这意味着 Grumpy 没有 VM。编译的 Go 源代码是对 Grumpy 运行时的一系列调用，Go 库服务与 Python C API 类似的目的（尽管不直接支持 C API）。
### **[6、基于 TensorFlow 的神经网络库  Sonnet](https://www.oschina.net/p/sonnet)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/028d77e27841c02dd3be45d476057482.png)
Sonnet 库使用面向对象的方法，允许创建定义一些前向传导计算的模块。模块用一些输入 Tensor 调用，添加操作到图里并返回输出 Tensor。其中一种设计选择是通过在随后调用相同的模块时自动重用变量来确保变量分享被透明化处理。
### **[7、控制 headless Chrome 的 Node.js API ：Puppeteer](https://www.oschina.net/p/puppeteer-nodejs)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/c2a148cc9e3e24a3a7460fbe2747903c.png)
Puppeteer 是一个控制 headless Chrome 的 Node.js API 。它是一个 Node.js 库，通过 [DevTools 协议](https://chromedevtools.github.io/devtools-protocol/)提供了一个高级的 API 来控制 [headless](https://developers.google.com/web/updates/2017/04/headless-chrome) Chrome。它还可以配置为使用完整的（非 headless）Chrome。
### **[8、图像算法工具 Guetzli](https://www.oschina.net/p/guetzli)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/436a615913f05be56a8cff238f7b433f.png)
![](http://jbcdn2.b0.upaiyun.com/2018/02/af1999686e33df99092c5a91b18484dd.png)
Guetzli，在瑞士德语中是“cookie（曲奇）”的意思，是一个针对数码图像和网页图像的 JPEG 编码器，能够通过产生更小的 JPEG 文件来达到更快的在线体验，并且同时保持与当前浏览器，图像处理应用和 JPEG 标准的兼容性。Google 称 Guetzli 创建高质量的 JPEG 图像文件的大小比当前的压缩方法要再小 35％。
### **[9、Google 开源机器学习数据集可视化工具 Facets](https://www.oschina.net/p/facets)**
![](http://jbcdn2.b0.upaiyun.com/2018/02/fe7a07d28b364469f3eabd3b99c112cc.png)
Facets 是 Google 开源的一款可视化工具，帮助理解、分析和调试 ML 数据集。
Facets 包含两个部分 —— Facets Overview 和 Facets Dive ，允许用户以不同的粒度查看其数据的整体图像。Facets Overview 可用于可视化数据的每一个特征，Facets Dive 用来探索个别的数据观察集。
### **[10、机器学习工具库 Kubeflow](https://www.oschina.net/p/kubeflow)**
Kubeflow 是谷歌发布的一个机器学习工具库，Kubeflow 项目旨在使 Kubernetes 上的机器学习变的轻松、便捷、可扩展，其目标不是重建其他服务，而是提供一种简便的方式找到最好的 OSS 解决方案。该库包含包含的清单用于创建：
- 用于创建和管理交互式 Jupyter notebook 的 JupyterHub
- 可配置为使用 CPU 或 GPU，并通过单一设置调整至单个集群大小的 TensorFlow 训练控制器（Tensorflow Training Controller）
- TF 服务容器（TF Serving container）
### **[11、游戏状态自动管理框架 BOARDGAME.IO](https://www.oschina.net/p/boardgame-io)**
BOARDGAME.IO 是 Google 开源的一个游戏框架，旨在允许游戏作者将游戏规则从本质上转化为一系列简单的函数，这些函数用于描述当一个指定动作发生时游戏的状态变化，框架负责处理表述性状态传递。 无需再手动编写任何网络或后端代码。
### **[12、开源量子软件 OpenFermion](https://www.oschina.net/p/openfermion)**
OpenFermion 是一个开源的软件，用于编译和分析量子算法来模拟费米子系统，包括量子化学，在其他功能中，当前版本的特征是用数据结构和工具获取和操纵费米子和量子位哈密顿算子的表示。
更多 Google 开源软件查看请点击：
- [http://www.oschina.net/project/google](http://www.oschina.net/project/google)
