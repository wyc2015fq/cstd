# AlphaGo Zero代码迟迟不开源，TF等不及自己推了一个 - 知乎
# 



> Root 编译自GitHub
量子位 出品 | 公众号 QbitAI

TensorFlow官方在GitHub上推了一个AlphaGo Zero的开源代码！

这个叫做Minigo的围棋AI引擎，是一个使用Python语言、在TensorFlow框架实现的基于神经网络的围棋算法。

这个项目确实是受到DeepMind的AlphaGo算法的启发，但TensorFlow官方再三强调这个项目**不属于DeepMind，也不是官方正式的AlphaGo项目**。

## **不是行货！不是行货！不是行货！**

重要的事情说三遍！

DeepMind迟迟不开源AlphaGo代码，看来Google自己人也等不及了。

**以下内容搬运自TensorFlow的GitHub项目网页。**

Minigo其实基于Brian Lee此前发布的MuGo。

MuGo是依据首篇AlphaGo论文*Mastering the Game of Go with Deep Neural Networks and Tree Search*用Python复现的结果。
![](https://pic1.zhimg.com/v2-1c20a9b5096ab5363482eb07352a2e14_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='638' height='479'></svg>)
Minigo里所添加的参数，以及所做的架构调整是参考*AlphaGo Zero的论文Mastering the Game of Go without Human Knowledge*。

最近，这个经调整的架构已经扩展运用到了国际象棋和将棋的论文*Mastering Chess and Shogi by Self-Play with a General Reinforcement Learning Algorithm*中了。

这些论文的工作都会在Minigo中用AG(AlphaGo)，AGZ(AlphaGo Zero)，AZ(AlphaZero)缩写表示。

## **这个项目的目标**
- 提供一套清晰的学习样板，能够运用TensorFlow、Kubernetes及谷歌云平台来建立硬件加速器上的增强学习的流程。
- 尽可能还原重现原始DeepMind AlphaGo论文中的方法，通过开源的流程工具及开源的实现。
- 把我们的数据，结果，发现都公开出来，让围棋，机器学习，Kubernetes社区获益。

这个项目还有一个明确但不是目标的愿景，就是做出来一个具有竞争力的围棋项目，甚至能成为顶级的围棋算法。

我们致力于做一个可读的、可理解的操作手册，能够对圈子里的人有帮助，即使这么做意味着我们操作起来效率和速度上会受到影响。

尽管最后的结果是会做出来一个强大的模型，但是我们是更看重和享受其中的过程：）

我们希望这个项目，对于感兴趣的开发者来说，可以通过容易理解的python代码平台触及到强大的围棋模型，来扩展，适配更多的项目任务。

如果你想知道我们是怎么训练这个模型的，请看**RESULTS.md**。

也想加入我们看看能帮忙做些什么的话，请参考**CONTRIBUTING.md**。

## **发车！**

想做这个项目，你需要有以下的：

1.virtualenv / virtualenvwrapper

2.Python 3.5+

3.Docker

4.Cloud SDK

**搭便车指南(Hitchhiker’s guide)**是非常棒的python开发及虚拟环境使用的入门资料。往后的指导没有一个不是在非虚拟环境下测试的。

> pip3 install virtualenv
pip3 install virtualenvwrapper

**安装TensorFlow**

首先创建一个新的虚拟环境，进去之后，开始安装TensorFlow和依赖工具：

> pip3 install -r requirements-gpu.txt

当你要用GPU的时候，打开**requirements-gpu.txt**这个文件。一定要安装CUDA 8.0。具体看TensorFlow 那堆文档。

如果你不用GPU跑，或者你没有，那么你搞个较早的版本：

> pip3 uninstall tensorflow-gpu
pip3 install tensorflow

或者安装CPU要求：

> pip3 install -r requirements-cpu.txt

**创建一个环境**

你可能需要用到一个云来存资源，如果是酱安装：

> PROJECT=foo-project

然后，运行

> source cluster/common.sh

这样可以设置其他环境可变的默认参数。

**运行单元测试**

> BOARD_SIZE=9 python3 -m unittest discover tests

## **基础**

所有请求必须兼容谷歌云盘（作为远程文件系统），或者你本地的文件系统。这里举得例子是用谷歌云，你要用本地文件路径是一样OK哒。

用谷歌云的伐呢，就要设置**BUCKET_NAME**参数，然后验证**登录谷歌云**。不然的话，所有的请求都会被搁置。

比如说，设置一个桶，验证，然后查找一个最近的模型。

> export BUCKET_NAME=your_bucket;
gcloud auth application-default login
gsutil ls gs://minigo/models | tail -3

看起来是酱紫的：

> gs:/BUCKET_NAME/models/000193-trusty.index
gs://$BUCKET_NAME/models/000193-trusty.meta

这三个文件共同组成这个模型。而一个模型所需的指令需要一个路径传递给模型的basename。比如说，**gs://$BUCKET_NAME/models/000193-trusty**

你需要把这个复制到你的本地盘上。这一节（fragment）把最近的模型拷贝到目录下的MINIGO_MODELS。

> MINIGO_MODELS=$HOME/minigo-models
mkdir -p $MINIGO_MODELS
gsutil ls gs://minigo/models | tail -3 | xargs -I{} gsutil cp “{}” $MINIGO_MODELS

## **先让Minigo自己玩**

看Minigo开一局，你需要specify模型。下面是个用最新模型在你桶里运行的例子。

> python rl_loop.py selfplay —readouts=$READOUTS -v 2

READOUTS那部分指的是，每一步需要做几次搜索。每一步的时间信息和数据都会打印出来。把速度设定到3或更高，会把每一步都打印在板上。

## **再虐Minigo**

Minigo用的是GTP协议，你可以使用任何的符合GTP协议的项目。

> # Latest model should look like: /path/to/models/000123-something
LATEST_MODEL=$(ls -d $MINIGO_MODELS/* | tail -1 | cut -f 1 -d ‘.’)
BOARD_SIZE=19 python3 main.py gtp -l $LATEST_MODEL -r $READOUTS -v 3

（如果没有模型的话，那它会随机设个初始值）

在加载了几个信息之后，它会提示GTP引擎已经准备好，也就是说这一刻它可以接受指令。GTP的小抄(cheatsheet)：

> genmove [color] # Asks the engine to generate a move for a side
play [color] [coordinate] # Tells the engine that a move should be played for `color` at `coordinate`
showboard # Asks the engine to print the board.

拿GTP来玩的一个办法是用gogui-display（含兼容 GTP 的 UI）。

你可以在[http://gogui.sourceforge.net/](https://link.zhihu.com/?target=http%3A//gogui.sourceforge.net/)下载gogui工具。

看使用GTP的几个好玩的方法。

> gogui-twogtp -black ‘python3 main.py gtp -l gs://$BUCKET_NAME/models/000000-bootstrap’ -white ‘gogui-display’ -size 19 -komi 7.5 -verbose -auto

另外一个办法呢，就是看Minigo和GnuGo互殴。

> BLACK=”gnugo —mode gtp”
WHITE=”python3 main.py gtp -l path/to/model”
TWOGTP=”gogui-twogtp -black \”WHITE\” -games 10 \
-size 19 -alternate -sgffile gnugo”
gogui -size 19 -program “$TWOGTP” -computer-both -auto

## **训练Minigo**

**概述**

接下来呢，跟着这一串的指令做，你能得到增强学习在9x9围棋上的迭代。这些很基础的指令都是用来建前面提到的模型的。

这些指令是：
- bootstrap：初始化模型；
- 自我博弈：用最新版的模型自我对弈下棋，产生可用来训练的数据；
- 收集：把同一个模型产生的数据文件导入到训练数据中；
- 训练：用N代自我对弈产生的结果训练一个新模型。

**Bootstrap**

这一步的指令是创建模型，放在**gs://MODEL_NAME(.index|.meta|.data-00000-of-00001)**

> export MODEL_NAME=000000-bootstrap
python3 main.py bootstrap gs://MODEL_NAME

**自我对弈**

这步指令是输出原始对战数据，能够兼容TensorFlow格式以及目录下的SGF。

> gs://MODEL_NAME/local_worker/.tfrecord.zz
gs://MODEL_NAME/local_worker/.sgf
python3 main.py selfplay gs://MODEL_NAME \
—readouts 10 \
-v 3 \
—output-dir=gs:/MODEL_NAME/local_worker \
—output-sgf=gs://MODEL_NAME/local_worker

**搜集**

> python3 main.py gather

这指令是把（可能只有几KB大小的）tfrecord.zz文件打乱重组成不到100MB的tfrecord.zz文件。

根据模型的编号来搜集数据，这样同一个模型产生的数据就会放在一起。默认情况下，目录里的文件名，rl_loop.py之前后加个环境参数**BUCKET_NAME**的前缀。

> gs://MODEL_NAME-{chunk_number}.tfrecord.zz

**gs://$BUCKET_NAME/data/training_chunks/meta.txt** 文件是用来查看目前是在处理哪个游戏。

> python3 main.py gather \
—input-directory=gs:/BUCKET_NAME/data/training_chunks

**训练**

这步指令刨出最新的50个模型的训练，以及参考最新的模型权重来训练新的模型。

运行训练任务：

> python3 main.py train gs://BUCKET_NAME/models/000001-somename \
—load-file=gs://$BUCKET_NAME/models/000000-bootstrap \
—generation-num=1 \
—logdir=path/to/tensorboard/logs \

已经更新的模型权重会被保存下来。

（要做的事儿：搞几个本地的基于global_step的检查点儿，酱回头可以任性地恢复）

另外，你可以一步步跟着TensorBoard训练的指南。如果你给每次运行都不同的名字的话，比方说，**logs/my_training_run**, **logs/my_training_run2**，你可以覆盖掉之前的数据。

> tensorboard —logdir=path/to/tensorboard/logs/

## **在Kubernetes Cluster上运行Minigo啦**

自个儿去cluster/README.md看！

最后，附开源代码地址，
[https://github.com/tensorflow/minigo](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/minigo)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


