# 28 天自制你的 AlphaGo (3) : 训练策略网络，真正与之对弈 - 知乎
# 



知乎对于人机大战的关注度很高，所以写这个系列，希望能让大家对于人工智能和围棋有更多的了解。如果有收获，请记得点一下赞。

这篇我们安装 TensorFlow，真正训练一下 AlphaGo v13 的 policy network，并且你还可以与它真正对弈，因为前几天已经有网友在做可以运行的 AlphaGo v13 的简化版：[brilee/MuGo](https://link.zhihu.com/?target=https%3A//github.com/brilee/MuGo)。所以这个过程真的已经很傻瓜化，毫不夸张地说，人人都可以拥有一只小狗了，只要你把棋谱喂给它，它就能学到棋谱的棋风。（如果不会围棋，可以看本系列的第一篇入门：[https://zhuanlan.zhihu.com/p/24801451](https://zhuanlan.zhihu.com/p/24801451) ）（**本文是给大家快速找到感觉，后续我们会从头写代码，因为这位网友的代码真的很多 bug**）

***本系列已更新多篇，其它几篇的传送门：***
- ***(1) : 围棋 AI 基础 [知乎专栏](https://zhuanlan.zhihu.com/p/24801451)***

- ***(2) : 安装 MXNet 搭建深度学习环境 [知乎专栏](https://zhuanlan.zhihu.com/p/24879716)***

- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***

- ***(4.5)：后文预告（Or 为什么你应该试试 Batch Normalization 和 ResNet）**[知乎专栏](https://zhuanlan.zhihu.com/p/25051435)***


## **一、安装 TensorFlow**

如果还没有装 CUDA 等等，请看前一篇（[知乎专栏](https://zhuanlan.zhihu.com/p/24879716)）装好，记得把 cudnn 解压到 CUDA 的目录。TensorFlow 最近开始原生支持 Windows，安装很方便。下面看 Windows 的安装。

1. 在上一篇我们装了 Anaconda Python 2.7，而 TensorFlow 需要 Python 3.5，不过两者在 Windows 下也可以共存，具体见：[Windows下Anaconda2(Python2)和Anaconda3(Python3)的共存 - Infin1te的博客 - 博客频道 - CSDN.NET](https://link.zhihu.com/?target=http%3A//blog.csdn.net/infin1te/article/details/50445217)

2. 按上文切换到 Python 3，然后一个命令就可以装好： [https://www.tensorflow.org/versions/master/get_started/os_setup.html#pip-installation-on-windows](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/versions/master/get_started/os_setup.html%23pip-installation-on-windows) 。例如 GPU 版本目前是：pip install --upgrade [https://storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-0.12.0rc1-cp35-cp35m-win_amd64.whl](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/tensorflow/windows/gpu/tensorflow_gpu-0.12.0rc1-cp35-cp35m-win_amd64.whl) 。但是，你很可能会遇到 404 错误，那么可以把这个文件用迅雷下载下来，然后 pip install 文件名 即可。

3. 装完后检验一下。首先进 python 然后 import tensorflow 然后 hello = tf.constant('Hello, TensorFlow!') 然后 sess = tf.Session() 然后 print(sess.run(hello))。

4. 然后 python -m tensorflow.models.image.mnist.convolutional 测试训练 MNIST 模型（一开始也会下载数据文件，比较慢）。

另外再看看 Mac 的安装。我是 OSX 10.11。

1.首先 sudo easy_install pip 然后 sudo easy_install --upgrade six 然后 pip install tensorflow-gpu 即可。

2. 可能需要再执行 sudo ln -s /usr/local/cuda/lib/libcuda.dylib /usr/local/cuda/lib/libcuda.1.dylib ，否则 import tensorflow 会报错。

3. 用一样的办法检验一下安装是否成功。

## **二、训练策略网络**

激动人心的时刻到了，我们开始真正训练 policy network。下面都以 Windows 下面的操作为例。


1. 把网友做好的 AlphaGo v13 简化版下载下来：[brilee/MuGo](https://link.zhihu.com/?target=https%3A//github.com/brilee/MuGo) 。然后 pip install 了 argh 和 sgf。注意 gtp 要额外装：[jtauber/gtp](https://link.zhihu.com/?target=https%3A//github.com/jtauber/gtp) （下载下来后用 easy_install . 装）。

2. 然后下载一些用于学习的棋谱。围棋棋谱的通用格式是 SGF。比如，就下载 KGS 的对局棋谱吧：[u-go.net](https://link.zhihu.com/?target=https%3A//u-go.net/gamerecords/) 。我没有统计过，不过上面大概有十万局吧。从原理上说，棋谱越多，训练的质量就越有保证；但是，棋谱中对弈者的水平参差不齐，如何控制这一变量，做过深度学习的朋友心中会有答案。本篇先不谈技术细节，我们先继续走。

3. 下面提取棋谱的特征，并将其分类为训练集和测试集。先建立 data 子目录，把 SGF 拷贝进去。例如下载 2014 年的 13029 局棋谱，解压到 data 下面是 kgs-19-2014，那么就执行

python main.py preprocess data/kgs-19-2014

截至今天为止，你会发现它写的 chunk 比预想少很多，所以生成的训练数据很少。具体来说，棋谱的让子大于 4 会崩溃，棋谱没有写明贴目会崩溃，有时候棋谱里面的 [tt] 其实也代表 PASS，等等。做为作业，请自己一个个修复吧！ SGF 格式的说明在此：[SGF file format FF[4]](https://link.zhihu.com/?target=http%3A//www.red-bean.com/sgf/)


4. 再建立一个 tmp 目录，然后开始训练 1 个周期：

python main.py train processed_data --save-file=tmp\savedmodel --epochs=1 --logdir=logs\my_training_run

5. 你会发现策略网络的预测准确率开始从 0 慢慢上升了！ 然后可以随时继续训练，比如说继续训练 10 个周期就是：

python main.py train processed_data --read-file=tmp\savedmodel --save-file=tmp\savedmodel --epochs=10 --logdir=logs\my_training_run
![](https://pic2.zhimg.com/v2-56ed744ce74bd2abe46762a75f709bb5_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='777' height='526'></svg>)
6. 训练的时候，可以再开一个命令行，激活 python3，然后 tensorboard --logdir=logs 然后在浏览器打开 [http://127.0.0.1:6006/](https://link.zhihu.com/?target=http%3A//127.0.0.1%3A6006/) 可以看到训练的过程：
![](https://pic1.zhimg.com/v2-8648ae7be1b4c50ac1d3b5218e598350_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1539' height='638'></svg>)
7. 一直训练到准确率增加缓慢为止，应该可以到接近 60%。

## **三、与自制小狗对弈**

8. 测试一下走棋（如果你会 GTP 协议）：python main.py gtp policy --read-file=tmp\savedmodel 这是纯网络输出。如果把 policy 改成 mcts 就是加入了蒙特卡洛树搜索的，棋力更高（但是截至今天为止，你执行会发现立刻退出，这位网友的程序 bug 真是太多了，我们以后再重写）。

9. 如果不会 GTP，还是下载 GoGui 围棋图形界面吧： [Download GoGui from SourceForge.net](https://link.zhihu.com/?target=https%3A//sourceforge.net/projects/gogui/files/gogui/1.4.9/gogui-1.4.9-install.exe/download) 。然后执行："C:\Program Files (x86)\GoGui\gogui-twogtp.exe" -black "python main.py gtp policy --read-file=tmp\savedmodel" -white "C:\Program Files (x86)\GoGui\gogui-display" -size 19 -komi 7.5 -verbose -auto，但是截至今天为止，你会发现它很快就报错退出......


10. 这是因为截至今天为止，代码有个 bug 是 strategies.py 的第 95 行的函数要改成 is_move_reasonable(position, move) 。然后......你亲手制造的小狗就可以运行了！ 黑棋是小狗：
![](https://pic4.zhimg.com/v2-29bcfc9f1d57df0e1c4840cdcedc86ef_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='888' height='686'></svg>)
Behold，小狗已经学会了守角！ 不过现在他还特别弱，因为没有搜索，容易死活出问题。

11. 如果不会下围棋，让 GnuGo 来玩玩吧，下载链接：[http://gnugo.baduk.org/gnugo2/gnugo-3.8.zip](https://link.zhihu.com/?target=http%3A//gnugo.baduk.org/gnugo2/gnugo-3.8.zip) ，比如解压到 C:\gnugo\gnugo.exe ，然后执行："C:\Program Files (x86)\GoGui\gogui.exe" -size 19 -computer-both -auto -program "C:\Program Files (x86)\GoGui\gogui-twogtp.exe -black ""C:\gnugo\gnugo.exe --level 1 --mode gtp"" -white ""python main.py gtp policy --read-file=tmp\savedmodel"" -games 1 -size 19 -alternate -sgffile gnugo -verbose" 即可。你会发现下到后面也会崩溃，如果打开 GTP Shell 看看，是因为小狗还无法理解对方的 PASS，哈哈。


于是，这篇就到此吧，我们下一篇见。

***本系列已更新多篇，其它几篇的传送门：***

- ***(1) : 围棋 AI 基础 [知乎专栏](https://zhuanlan.zhihu.com/p/24801451)***
- ***(2) : 安装 MXNet 搭建深度学习环境 [知乎专栏](https://zhuanlan.zhihu.com/p/24879716)***
- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***

> 如需转载本系列，请先与本人联系，谢谢。小广告：晚上工作学习是否觉得光线不够舒服，精神不够集中，眼睛容易疲劳？不妨点击看看我们的自然全光谱灯系列：[Blink Sunshine护眼LED灯泡 高显指97显色指数无频闪学习台灯床头](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056) 如果需要好用的耳机或钱包，我们也有 :-)


