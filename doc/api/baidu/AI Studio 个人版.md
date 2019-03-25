#  AI Studio 个人版 

# 基础介绍

## 平台说明

Hi，您好，欢迎使用百度AI Studio深度学习平台. 百度AI Studio是针对AI学习者的在线一体化开发实训平台. 本平台集合了AI教程, 深度学习样例工程, 各领域的经典数据集, 云端的运算及存储资源, 以及比赛平台和社区.

从而解决学习者在AI学习过程中的一系列难题, 例如教程水平不一, 教程和样例代码难以衔接, 高质量的数据集不易获得,以及本地难以使用大体量数据集进行模型训练.

本平台支持Chrome/Firefox/Safari等浏览器. 在IE/360等其他浏览器上不能保证正常运行.

![img](https://ai.bdstatic.com/file/368A84A29E0F4CAAABB88711C3A6E8BC)

本文将引导用户对该平台的基础功能进行一个快速浏览, 以便用户了解该平台的主要功能.

百度AI Studio平台已经为使用者预置了Python语言环境, 以及百度PaddlePaddle深度学习开发框架. 同时用户可以在其中自行加载Scikit-Learn等机器学习库.

## 平台内容

![img](https://ai.bdstatic.com/file/721F3F82B26C48518E90E446D4A1BCAA)

平台分为项目大厅、数据集、比赛三部分

- 项目大厅
- 数据集
- 比赛







# 项目大厅-创建项目

## 用户界面

![img](https://ai.bdstatic.com/file/3E6BB0491935478D938871DB62DE419A)

1. 本页亦即项目大厅页, 用于创建, 管理, 查看各种项目.
2. 正中则是创建项目的入口.
3. 再下是新手引导说明, AI教程, 百度AI新闻中心.
4. 再下是项目管理区, 其中分为三部分内容: a. 百度官方AI学习项目; b. 其他学习者共享的AI项目; c. 用户自己创建的个人项目.

我们先从AI基础教程入口, 开始AI学习之旅.

## AI基础教程

点击”AI基础教程”, 则进入如下页面:

![img](https://ai.bdstatic.com/file/C32B20BECB4D407EB7E33871F856BF4A)

该课程涵盖了AI基础入门知识, 核心技术以及PaddlePaddle实践. 循序渐进.

其中”PaddlePaddle实战”中讲解的项目, 部分代码已经在”百度AI学习项目”中进行共享. 视频结合真实可在线运行的项目, 可以加深理解, 提高学习效率.

![img](https://ai.bdstatic.com/file/B7F83E5EC4EE495DAFB01D6AF8CE938F)

## 创建并运行一个项目

### 项目创建

回到项目大厅页, 点击居中的【创建项目】:

![img](https://ai.bdstatic.com/file/95AAED9C32964F76B32FE81AF70F1725)

则出现创建项目的窗口: ![img](https://ai.bdstatic.com/file/C733FD71E7D94D4994CBEA0266283E6E)

- 项目环境: 语言基础环境, 包括Python 2.7(默认), Python3.5.
- 项目框架: 深度学习开发框架, 已支持PaddlePaddle最新版, 未来我们也将集成更多的开发框架.
- 配置资源: 程序部署运行环境, 包括单机, 远程集群. 单机资源基于Notebook, 交互更加友好. 远程集群资源提供大规模机器支持，训练速度更快
- 项目名称/项目描述: 用来标识项目, 便于日后进行查找和管理, 创建后支持修改.

### 添加数据集

如果项目涉及到数据集, 可以考虑直接使用系统预置的数据集, 点击添加数据集按钮

![img](https://ai.bdstatic.com/file/DF6B115500164EBDBB79EE1876CDDBC2)

然后: ![img](https://ai.bdstatic.com/file/ED1794029DAC4BE3A98390593A0E2A92)

每个项目**最多可以引入两个**数据集, 便于模型比较在不同数据集下的准确率和召回率. 如无合适的数据集, 用户也可以自行上传创建新数据集.

最后点击【创建】并在弹出框中选择【查看】进入项目详情页. ![img](https://ai.bdstatic.com/file/01EE59762A884B6998BD00B69204F3FF)

之后在项目详情页, 对项目进行编辑, 可以对数据集进行变更.





# 项目大厅-单机项目

## 页面概览

在项目详情页中, 用户可以浏览刚自己创建的项目, 并且编辑项目名称及数据集等信息.

![img](https://ai.bdstatic.com/file/00FD734EA75349779E9E73FDAEC85A71)

- 版本内容: 展示当前Notebook最新内容.
- Fork记录: 项目被其他人Fork的记录.

点击右方【运行项目】进行项目环境初始化

![img](https://ai.bdstatic.com/file/1725A6AFFA7C4CC5A7FD632612EC72A8)点击【进入】跳转到项目代码在线编辑Notebook环境。Notebook使用说明详见下一节《项目大厅-Notebook使用说明》

## Fork项目

如果不熟悉操作, 则可以直接Fork百度AI学习项目, 或者其他开发者共享的项目, 加快学习速度.

![img](https://ai.bdstatic.com/file/B306D4FAB7164EC6A9563FAE6ED3D12A)

## Visual DL工具

VisualDL是一个面向深度学习任务设计的可视化工具，包含了scalar、参数分布、模型结构、图像可视化等功能. AI Studio单机项目已经集成Visual DL工具, Notebook代码编写请参考文档[如何在PaddlePaddle中使用VisualDL](http://visualdl.paddlepaddle.org/documentation/visualdl/zh/develop/getting_started/demo/paddle/TUTORIAL_CN.html).

- **Step1** 训练代码中增加 Loggers 来记录不同种类的数据. 注意我们的`logdir = "./log"`, 即需要把log目录放到`/home/aistudio/log`.

```
# create VisualDL logger and directory
logdir = "./log"
logwriter = LogWriter(logdir, sync_cycle=10)

# create 'train' run
with logwriter.mode("train") as writer:
    # create 'loss' scalar tag to keep track of loss function
    loss_scalar = writer.scalar("loss")
```

- **Step2** 使用PaddlePaddle API创建训练训练模型.

```
def vgg16_bn_drop(input):
	pass
```

- **Step3** 开始训练并且同时用 VisualDL 来采集相关数据

```
# add record for loss and accuracy to scalar
        loss_scalar.add_record(step, loss)
```

- **Step4** 在Web浏览器中输入URL访问. URL生成规则: 将项目地址中的`notebooks及之后部分`替换为`visualdl`.

```
#notebooks项目访问URL
url_notebook = 'http://aistudio.baidu.com/user/30799/33852/notebooks/33852.ipynb?redirects=1'
#替换后visualdl访问URL
url_visualdl = 'http://aistudio.baidu.com/user/30799/33852/visualdl'
```













# 项目大厅-Notebook使用说明

## 页面概览

![img](https://ai.bdstatic.com/file/09338C44CF2D4F03942E7722126AB1B4)当前Notebook编辑界面有如下几个部分组成

- 操作区
- Notebook内容编辑区
- 数据集
- 工具栏，提供了保存、导出、重载notebook, 以及重启内核等选项

以下对每个区域的操作分别说明.

## 操作区

![img](https://ai.bdstatic.com/file/D6933508483D4962AB932CA433DF8D51)

### 新建块

点击![img](https://ai.bdstatic.com/file/D9F5901A8A6A45339D473820AD1D36A9) 可以分别插入代码块或文字块.

- 代码块, 代码可以运行, 点击【运行】, 在下方输出结果. ![img](https://ai.bdstatic.com/file/0ADACB11CC304991AEF27E282A8389D7)
- 文字块, 支持Markdown格式, 点击【预览】则出现渲染后的效果(但下方不会出现运行结果) ![img](https://ai.bdstatic.com/file/DFFA546A694A4347ABFB3D30AF6CF597)![img](https://ai.bdstatic.com/file/E2DAFD8953BC4A6E883520F7C657C64C)

选中某个块, 然后点击 ![img](https://ai.bdstatic.com/file/76A2A582322B4044AB5BE70EC279473D) , 则可以使其在代码/文字之间进行切换.

### 操作块

点击 ![img](https://ai.bdstatic.com/file/EEBBA85491DD410A90F79E4682E00E0B)中的【运行】, 如果是代码块, 则自动执行该块内容, 同时激活下一个块. 如果连续点击,则顺次执行.

块执行时, 左侧的In[ ]会变成In[*], 以示当前该块正在执行中.

如果发现代码并不尽如人意, 可以点击【中断】, 打断所有代码块的执行.不过通常需要耗时数十秒才能完全停止.

最后如果需要重置整个项目环境, 清除中间变量, 则可以点击【重启】按钮.

## Notebook内容编辑区

### 命令/编辑模式

notebook内容编辑区由基本的块(cell)组成. `绿色`代表块内容可编辑状态-编辑模式（比如输入代码），`蓝色`代表块可操作状态-命令模式（比如删除块，必须回到蓝色），与linux编辑器vi/vim类似，编辑模式和命令模式之间可以用Esc和Enter来切换。

- 编辑模式 ![img](https://ai.bdstatic.com/file/560934AA121147CAAF1A3C703DC85423)
- 命令模式 ![img](https://ai.bdstatic.com/file/AE57BA80A4D94D309B91997747188539)

### 鼠标操作

![img](https://ai.bdstatic.com/file/3A049925BC8C4735AECD5DD2A64005BE)

### 快捷键操作

- 常用操作列表

| 模式                   | 内容           | 快捷键(Windows) | 快捷键(Mac) |
| :--------------------- | :------------- | :-------------- | :---------- |
| 命令模式 (`Esc`切换)   | 运行块         | Shift-Enter     | Shift-Enter |
| 命令模式               | 在下方插入块   | B               | B           |
| 命令模式               | 在上方插入块   | A               | A           |
| 命令模式               | 删除块         | d-d             | d-d         |
| 命令模式               | 切换到编辑模式 | Enter           | Enter       |
| 编辑模式 (`Enter`切换) | 运行块         | Shift-Enter     | Shift-Enter |
| 编辑模式               | 缩进           | Clrl-]          | Command-]   |
| 编辑模式               | 取消缩进       | Ctrl-[          | Command-[   |
| 编辑模式               | 注释           | Ctrl-/          | Command-/   |
| 编辑模式               | 函数内省       | Tab             | Tab         |

- [全部操作说明](http://aistudio.baidu.com/aistudio/#/projectdetail/20397)

### 代码块说明

- In提示符

| 提示符  | 含义         |
| :------ | :----------- |
| In[ ]   | 程序未运行   |
| In[num] | 程序运行后   |
| In[*]   | 程序正在运行 |

- Linux命令

  运行Linux命令的方式是在Linux命令前加一个`!`，就可以在块里运行 ![img](https://ai.bdstatic.com/file/30B11ACCAC43481483444EEDE28E3BEE)

- Tab键查看提示信息或者补全命令 ![img](https://ai.bdstatic.com/file/4B608FECE377469697E7BB7A5DB304E0)

- 在一个库、方法或变量前加上 ?，就可以获得它的一个快速语法说明 ![img](https://ai.bdstatic.com/file/24587AA416D1457192EC110B3A053D6C)

- Magic关键字

  Magic关键字是可以运行特殊的命令. Magic 命令的前面带有一个或两个百分号(% 或 %%), 分别代表`行Magic命令`和`块Magic命令`. `行Magic命令`仅应用于编写Magic命令时所在的行, 而`块 Magic命令`应用于整个块.

| Magic关键字        | 含义                                                         |
| :----------------- | :----------------------------------------------------------- |
| %timeit            | 测试单行语句的执行时间                                       |
| %%timeit           | 测试整个块中代码的执行时间                                   |
| %matplotlib inline | 显示 matplotlib 包生成的图形                                 |
| %run               | 调用外部python脚本                                           |
| %pdb               | 调试程序                                                     |
| %pwd               | 查看当前工作目录                                             |
| %ls                | 查看目录文件列表                                             |
| %reset             | 清除全部变量                                                 |
| %who               | 查看所有全局变量的名称，若给定类型参数，只返回该类型的变量列表 |
| %whos              | 显示所有的全局变量名称、类型、值/信息                        |
| %xmode Plain       | 设置为当异常发生时只展示简单的异常信息                       |
| %xmode Verbose     | 设置为当异常发生时展示详细的异常信息                         |
| %debug             | bug调试，输入quit退出调试                                    |
| %bug               | 调试，输入quit退出调试                                       |
| %env               | 列出全部环境变量                                             |

示例1: 使用 %%timeit测算整个块的运行时间. ![img](https://ai.bdstatic.com/file/150734EE12D342D1A64D502A0803B73F)示例2: 块可集成Matplotlib,从而进行绘图, 但需要注意绘图前需要输入%Matplotlib inline并运行, 否则即使运行终端可用的绘图代码段, cell也只会返回一个文件说明, 如下图所示 ![img](https://ai.bdstatic.com/file/354B8F9D3F3D4F149338B6F9ED60BAC9)示例3: 查看所有支持的Magic关键字. ![img](https://ai.bdstatic.com/file/713BA9FA3E6B4D2CBE47D393E5BAF700)

- 查看当前环境中的Python版本和Paddle版本 ![img](https://ai.bdstatic.com/file/C52017A386A04457A96DCDA63A05B231)

### 文字块说明

文字块除文本外还可嵌入公式、表格、图片、音乐、视频、网页等.

相关Markdown用法可以参考官网 <http://www.markdown.cn/>.

- 插入图片方法说明

  点击【编辑】菜单中的【插入图片】. ![img](https://ai.bdstatic.com/file/38A540460D8040F792C3C5CB622E2E46)上传图片. ![img](https://ai.bdstatic.com/file/8C520E4F5BEB4305AD0BF487F463AA15)图片插入成功. ![img](https://ai.bdstatic.com/file/2CB364E7EBAA4CCFA3D5085590FD3F6E)

## 侧边栏

![img](https://ai.bdstatic.com/file/7FFF13DC835841D4B5682204D55DF5A9)

### 文件夹

按照树形结构展示/home/aistudio路径下的文件夹和文件。可以在该目录下进行如下操作：

- 文件夹操作: 创建新的文件夹. 鼠标悬浮在文件夹条目上, 会出现操作按钮, 包括删除文件夹、重命名文件夹、路径复制.
- 文件操作: 创建上传文件(上传的单个文件最大20M). 鼠标悬浮在文件条目上, 会出现操作按钮, 包括下载文件、重命名文件、路径复制.
- 更新操作：如果在代码运行过程中磁盘里的文件更新了，可以手动刷新, 在侧边栏查看文件更新的状态.

![img](https://ai.bdstatic.com/file/A5D6F8DF28EE49C792BD64E25513CF7C)

### 数据集

在数据集栏中, 可以复制数据集文件的路径, 并置于代码中. 复制数据集路径成功则出现:![img](https://ai.bdstatic.com/file/22BEA363A2B94577B9BD0F43CCCC8F8D)

![img](https://ai.bdstatic.com/file/7E0276BBDA004C39865989A614287963)

## 工具栏

顶部工具栏有大量的功能, 由于名称足以表意, 因此不一一介绍.

![img](https://ai.bdstatic.com/file/C384FA0EF55145C49C8D4214E44FFFD6)







# 项目大厅-在线部署及预测

## 功能说明

在线部署与预测为开发者提供训练模型向应用化API转换的功能. 开发者在AI Studio平台通过单机项目NoteBook页面完成模型训练后, 通过创建一个在线服务, 应用模型生成在线API, 使用该API可以直接检验模型效果或实际应用到开发者的私有项目中.目前, 该功能暂时**仅对单机项目**开放。

![img](https://ai.bdstatic.com/file/9F8B65CBE0F248389DAE7CF637C4FFC8)

## 通过训练任务生成模型文件

- 在训练任务过程中, 通过调用[paddle.fluid.io.save_inference_model](http://paddlepaddle.org/documentation/docs/zh/1.2/api_cn/io_cn.html#permalink-5-save_inference_model)`**实现模型的保存**，保存后的目录需要可以被在线服务使用. 我们以房价预测的线性回归任务为例, 具体代码如下

```
import paddle
import paddle.fluid as fluid
import numpy
import math
import sys
from __future__ import print_function
BATCH_SIZE = 20
train_reader = paddle.batch(
    paddle.reader.shuffle(
        paddle.dataset.uci_housing.train(), buf_size=500),
        batch_size=BATCH_SIZE)
test_reader = paddle.batch(
    paddle.reader.shuffle(
        paddle.dataset.uci_housing.test(), buf_size=500),
        batch_size=BATCH_SIZE)
params_dirname = "model2"
x = fluid.layers.data(name='x', shape=[13], dtype='float32')
y = fluid.layers.data(name='y', shape=[1], dtype='float32')
y_predict = fluid.layers.fc(input=x, size=1, act=None)
main_program = fluid.default_main_program()
startup_program = fluid.default_startup_program()
cost = fluid.layers.square_error_cost(input=y_predict, label=y)
avg_loss = fluid.layers.mean(cost)
sgd_optimizer = fluid.optimizer.SGD(learning_rate=0.001)
sgd_optimizer.minimize(avg_loss)
#clone a test_program
test_program = main_program.clone(for_test=True)
use_cuda = False
place = fluid.CUDAPlace(0) if use_cuda else fluid.CPUPlace()
exe = fluid.Executor(place)
num_epochs = 100
# For training test cost
def train_test(executor, program, reader, feeder, fetch_list):
    accumulated = 1 * [0]
    count = 0
    for data_test in reader():
        outs = executor.run(program=program,
                            feed=feeder.feed(data_test),
                            fetch_list=fetch_list)
        accumulated = [x_c[0] + x_c[1][0] for x_c in zip(accumulated, outs)]
        count += 1
    return [x_d / count for x_d in accumulated]
params_dirname = "fit_a_line.inference.model"
feeder = fluid.DataFeeder(place=place, feed_list=[x, y])
naive_exe = fluid.Executor(place)
naive_exe.run(startup_program)
step = 0
exe_test = fluid.Executor(place)
# main train loop.
for pass_id in range(num_epochs):
    for data_train in train_reader():
        avg_loss_value, = exe.run(main_program,
                                  feed=feeder.feed(data_train),
                                  fetch_list=[avg_loss])
        if step % 10 == 0:  # record a train cost every 10 batches
            print (step, avg_loss_value[0])
        if step % 100 == 0:  # record a test cost every 100 batches
            test_metics = train_test(executor=exe_test,
                                     program=test_program,
                                     reader=test_reader,
                                     fetch_list=[avg_loss.name],
                                     feeder=feeder)
            print (step, test_metics[0])
            # If the accuracy is good enough, we can stop the training.
            if test_metics[0] < 10.0:
                break
        step += 1
        if math.isnan(float(avg_loss_value[0])):
            sys.exit("got NaN loss, training failed.")
    if params_dirname is not None:
        # We can save the trained parameters for the inferences later
        fluid.io.save_inference_model(params_dirname, ['x'],
                                      [y_predict], exe)
```

- 使用已有模型, 可以通过`!wget`在Notebook中传输模型文件到环境目录。以房价预测的线性回归模型为例, 通过`!wget https://ai.baidu.com/file/4E1D1FCC670E4A5E8441634201658107 -O fit_a_line.inference.model`传输文件, 解压后直接被在线服务使用.

![img](https://ai.bdstatic.com/file/883EC8AA04DC4BE796E9285883CBB1CD)

## 创建一个在线服务

完成模型训练后, 在单机项目页面点击【创建预测服务】

![img](https://ai.bdstatic.com/file/BB301B6F45084ACFB3426420A537A4DD)

### 第一步 选择模型文件

- 勾选模型文件 ![img](https://ai.bdstatic.com/file/18EA54AA63E2438EBF1E2E8BE25DA42B)![img](https://ai.bdstatic.com/file/57E6C27F2C0941568555F1B2BE9AB024)
- 设置主程序, 主程序为`paddle.fluid.io.save_inference_model`中参数`main_program`配置的程序, 在房价预测的示例中，我们使用默认参数调用`save_inference_model`, 因此将`__model__`文件设置为主程序. ![img](https://ai.bdstatic.com/file/D985B5ABB69E4765AF465089F733ADE0)![img](https://ai.bdstatic.com/file/55883C55A42F4062AC7D2F1F03BF250C)

### 第二步 确认输入输出

填写模型的输入输出参数. 以房价预测的线性回归模型为例([参数参考](http://paddlepaddle.org/documentation/docs/zh/1.2/beginners_guide/quick_start/fit_a_line/README.cn.html)), 添加参数如下图所示.

![img](https://ai.bdstatic.com/file/AC4A159F5E99486D9AA8C59F3D2425DD)![img](https://ai.bdstatic.com/file/FC199544D48B4D868C41E5951775EACE)

### 第三步 制作参数转换器

参数转换器帮助用户转化合法输入并完成数据预处理.

- 方式一:自定义转换器(Python2.7)(推荐).

输入参数转换器方法

```
def reader_infer(data_args):
  	"""
  	reader_infer 输入参数转换器方法
  	:param data_args: 接口传入的数据，以k-v形式
  	:return [[]], feeder
  	"""
  	#构造内容
  	pass
```

输出参数转换器方法

```
def output(results, data_args):
  	"""
  	output 输出参数转换器方法
  	:param results 模型预测结果
  	:param data_args: 接口传入的数据，以k-v形式
 	 	:return array 需要能被json_encode的数据格式
 	 	"""
  	#构造内容
  	pass
```

![img](https://ai.bdstatic.com/file/AAFCC99F632A492C84E3E7C46E65B96B)

转换器代码示例, 以房价预测为例.

输入参数转换器:

```
import os
import sys
sys.path.append("..")
from PIL import Image
import numpy as np
import paddle.fluid as fluid
from home.utility import base64_to_image
def reader_infer(data_args):
   """
   reader_infer 输入参数转换器方法
   :param data_args: 接口传入的数据，以k-v形式
   :return [[]], feeder
   """
   def reader():
       """
       reader
       :return:
       """
       x = fluid.layers.data(name='x', shape=[13], dtype='float32')
       # y = fluid.layers.data(name='y', shape=[1], dtype='float32')
       feeder = fluid.DataFeeder(place=fluid.CPUPlace(), feed_list=[x])
       CRIM = float(data_args["CRIM"])
       ZN = float(data_args["ZN"])
       INDUS =  float(data_args["INDUS"])
       CHAS = float(data_args["CHAS"])
       NOX = float(data_args["NOX"])
       RM = float(data_args["RM"])
       AGE = float(data_args["AGE"])
       DIS = float(data_args["DIS"])
       RAD =  float(data_args["RAD"])
       TAX = float(data_args["TAX"])
       PTRATIO = float(data_args["PTRATIO"])
       B =  float(data_args["B"])
       LSTAT = float(data_args["LSTAT"])
       return [[[CRIM, ZN, INDUS, CHAS, NOX, RM, AGE, DIS, RAD, TAX, PTRATIO, B, LSTAT]]], feeder
   return reader
```

输出参数转换器:

```
def output(results, data_args):
    """
    output 输出参数转换器方法
    :param results 模型预测结果
    :param data_args: 接口传入的数据，以k-v形式
    :return array 需要能被json_encode的数据格式
    """
   lines = []
   for dt in results:
       y = dt.tolist()
       lines.append({"predict": y})
   return lines
```

- 方式二: 默认参数, 不设置转换器.

  用户的API参数直接传递给模型. ![img](https://ai.bdstatic.com/file/61BCE455CD7E4065AB10AC2C3FBB9443)

### 第四步 沙盒部署

用户可以同时部署之多五个沙盒服务, 用来对比模型优化结果.

录入名称点击【生成沙盒】或者点击【暂存】将沙盒保存到草稿箱.

![img](https://ai.bdstatic.com/file/F6A38195FC764E2D97EB0C23745D6B6C)

## 测试沙盒服务

对沙盒列表中的沙盒服务进行测试，验证是否配置正确。

### 第一步 点击【测试】打开测试页面

![img](https://ai.bdstatic.com/file/80923AC01F504AE3B54B693AC0C2CA22)

### 第二步 填写json格式请求参数

![img](https://ai.bdstatic.com/file/37167C83A424458E90DA75D519C1F565)

### 第三步 点击【发送】检验返回结果

![img](https://ai.bdstatic.com/file/467D5CFE1C5A4D58B50A63124960FC07)

## 部署在线服务

点击【正式部署】部署线上API.

- 一个项目可以创建**五个**沙盒服务, 并选择其中**一个**沙盒服务部署为线上服务.
- 沙盒服务如果连续超过**24小时**无调用将自动调整为暂停状态.
- 线上服务如果连续超过**14天**无调用将自动调整为暂停状态.

![img](https://ai.bdstatic.com/file/1305946D707443068C1167BD8AEA286F)![img](https://ai.bdstatic.com/file/BBDC930F1D7E4AB69377E808508D2300)

## 调用在线服务

依据API key、服务地址和用户自定义参数, 实现对服务的调用.

### 请求方式

- HTTP请求URL: [服务地址] [?] [apiKey=xxx]
- HTTP请求方法: POST
- HTTP Body: 用户自定义参数

![img](https://ai.bdstatic.com/file/62B2838DE1DD4FE49E2643AA364A8D48)

### 调用示例

以房价预测项目为例.

- CURL

```
curl -H "Content-Type: application/json" -X POST -d '{"CRIM":0.01887747, "ZN":-0.11363636, "INDUS":0.25525005, "CHAS":-0.06916996,  "NOX":0.29898136, "RM": -0.04476612, "AGE": 0.14340987, "DIS":-0.14797285,  "RAD":0.62828665, "TAX":0.49191383, "PTRATIO":0.18558153, "B":0.05473289, "LSTAT":0.16851371}' "https://aistudio.baidu.com/serving/online/xxx?apiKey=xxxxxxxxxx"
```

![img](https://ai.bdstatic.com/file/9B8C3480C5B54DD59B24334FE480DC1C)

- Python

```
import json
import traceback
import urllib
import urllib2

formdata = {
    "CRIM":0.01887747, 
    "ZN":-0.11363636, 
    "INDUS":0.25525005, 
    "CHAS":-0.06916996,  
    "NOX":0.29898136, 
    "RM": -0.04476612, 
    "AGE": 0.14340987, 
    "DIS":-0.14797285,  
    "RAD":0.62828665, 
    "TAX":0.49191383, 
    "PTRATIO":0.18558153, 
    "B":0.05473289, 
    "LSTAT":0.16851371
}
header = {"Content-Type": "application/json; charset=utf-8"}
url = "https://aistudio.baidu.com/serving/online/xxx?apiKey=a280cf48-6d0c-4baf-bd39xxxxxxcxxxxx"
data = json.dumps(formdata)
try:
    request = urllib2.Request(url, data, header)
    response = urllib2.urlopen(request)
    response_str = response.read()
    response.close()
    print(response_str)
except urllib2.HTTPError as e:
    print("The server couldn't fulfill the request")
    print(e.code)
    print(e.read())
except urllib2.URLError as e:
    print("Failed to reach the server")
    print(e.reason)
except:
    traceback.print_exc()
```

![img](https://ai.bdstatic.com/file/AD1048C5763548C785F13E32A2D61C3C)





# 项目大厅-集群项目

## 集群项目说明

集群项目的任务执行由**GPU集群**作为支撑, 具有实时高速的并行计算和浮点计算能力, 有效解放深度学习训练中的计算压力, 提高处理效率.

用户可以先在**单机项目**中, 利用在线的Notebook功能, 完成代码的编写与调试, 之后在**集群项目**中运行, 从而提高模型训练速度.

## 创建集群项目

![img](https://ai.bdstatic.com/file/B4DB2169224345D481C3E9F6B3FDC474)![img](https://ai.bdstatic.com/file/80C1391FF1EC4448B439E664FD343114)

## 页面概览

在集群项目详情页中, 用户可以浏览自己创建的项目内容, 编辑项目名称及数据集等信息, 查看集群历史任务信息等.

![img](https://ai.bdstatic.com/file/75BB2E7057864CF0AC2B00502F418AAE)

- 版本内容: 默认展示当前Notebook最新内容. 初始化状态为集群项目示例代码. 用户可以手动选择提交任务时对应的历史版本.
- 数据集: 项目所引用的数据集信息.
- 历史任务: 每一次执行任务的记录.

## 代码编辑

![img](https://ai.bdstatic.com/file/9C3FFF2BC0054908AA259E876E06AC7E)

代码编辑界面主要分为左侧: 文件管理及数据集, 和右侧: 代码编辑区及提交任务

### 左侧文件管理和数据集

#### 文件管理

- 用户可以手动创建文件/文件夹, 对文件/文件夹进行重命名或删除.
- 其中用户可以选择指定文件, 并设置为主文件. 用作整个项目运行的入口.
- 用户也可以手动上传文件(体积上限为20MB, 体积巨大的文件请通告数据集上传).
- 用户可以双击文件, 在右侧将新建一个tab. 用户可以进一步查看或编辑该文件的内容. (目前仅支持.py文件和.txt文件; 同时预览文件的体积上限为1MB)

#### 数据集管理

- 用户可以查看数据集文件, 并复制该文件的相对路径. 最后拼合模板内置绝对路径, 即可使用. 下方有详细介绍.

![img](https://ai.bdstatic.com/file/B8B7F2F5B97D45D7ACF1A8E9497EBAF0)

### 右侧文件预览编辑和提交任务

- 当多个文件被打开时, 用户可以将它们逐一关闭, 当至最后一个文件时即不可关闭
- 选中文件对应的tab即可对文件内容进行预览和编辑, 但当前仅支持.py和.txt格式的文件
- 点击保存按钮, 会将所有文件的改动信息全部保存, 如用户不提交任务, 直接退出, 则自动保存为一个"未提交"版本
- 提交任务前, 建议写一个备注名称, 方便未来进行不同版本代码/参数的效果比较
- 

![img](https://ai.bdstatic.com/file/578685C3D9354ACFB7DC644EF532A20F)

## PaddlePaddle集群训练说明

PaddlePaddle基于集群的分布式训练任务与单机训练任务调用方法**不同**。基于pserver-trainer架构的的分布式训练任务分为两种角色: **parameter server(pserver)和trainer**.

在Fluid 中, 用户只需配置单机训练所需要的网络配置, `DistributeTranspiler`模块会自动地根据 当前训练节点的角色将用户配置的单机网路配置改写成pserver和trainer需要运行的网络配置:

```
t = fluid.DistributeTranspiler()
t.transpile(
    trainer_id = trainer_id,
    pservers = pserver_endpoints,
    trainers = trainers)
if PADDLE_TRAINING_ROLE == "TRAINER":
    # 获取pserver程序并执行它
    trainer_prog = t.get_trainer_program()
    ...

elif PADDLE_TRAINER_ROLE == "PSERVER":
    # 获取trainer程序并执行它
    pserver_prog = t.get_pserver_program(current_endpoint)
    ...
```

- 目前集群项目中提供的默认环境`PADDLE_TRAINERS=1`(`PADDLE_TRAINERS`是分布式训练任务中 trainer 节点的数量)
- 非PaddlePaddle代码请放到`if PADDLE_TRAINING_ROLE == "TRAINER":`分支下执行, 例如数据集解压任务

更多集群项目说明请参考[PaddlePaddle官方文档](http://www.paddlepaddle.org/documentation/docs/zh/1.1/user_guides/howto/training/cluster_quick_start.html)

## 数据集与输出文件路径说明

- 集成项目中添加的数据集统一放到**绝对路径**`./datasets`

```
# 数据集文件会被自动拷贝到./datasets目录下
CLUSTER_DATASET_DIR = '/root/paddlejob/workspace/train_data/datasets/'
```

- 集成项目数据集文件路径的获取

在页面左侧数据集中点击复制数据集文件路径, 得到文件的**相对路径**, 例如点击后复制到剪切板的路径为`data65/train-labels-idx1-ubyte.gz`.

![img](https://ai.bdstatic.com/file/B27FD883CF654E70BE6854BD22223041)

```
# 数据集文件相对路径
file_path = 'data65/train-labels-idx1-ubyte.gz'
```

真正使用的时候需要将两者拼合 train_datasets = datasets_prefix + file_path

- 集成项目输出文件路径为`./output`

```
# 需要下载的文件可以输出到'/root/paddlejob/workspace/output'目录
CLUSTER_OUTPUT_DIR = '/root/paddlejob/workspace/output'
```

## 提交任务

点击【运行项目】后进入到任务编辑页面.

![img](https://ai.bdstatic.com/file/56E86C6695BE40F6BA4F60114E450992)

- 提交: 点击提交会发起本次本次任务的执行. 并将代码自动保存为一个版本.
- 任务备注: 任务自定义标识, 用于区分项目内每次执行的任务.

## 历史任务

历史任务页面如下所示.

![img](https://ai.bdstatic.com/file/3F69E41D7C2C4C1E83723539776BD456)

任务操作:

- 下载输出: 下载任务输出文件, 文件格式`xxx(任务编号)_output.tar.gz` .
- 查看/下载日志: 在任务运行过程中, 点击"查看日志", 可以查看实时日志, 掌握运行进度. 运行结束后, 按钮转为"下载日志". 下载任务执行日志, 日志格式`xxx(任务编号)_log.tar.gz` .
- 终止任务: 在任务执行过程中, 可以点击终止任务.
- 编辑: 编辑任务对应的代码版本内容.

## 空间说明

- 集群项目空间安装包列表

| Package                       | Version  |
| :---------------------------- | :------- |
| backports.functools-lru-cache | 1.5      |
| cycler                        | 0.10.0   |
| graphviz                      | 0.10.1   |
| kiwisolver                    | 1.0.1    |
| matplotlib                    | 2.2.3    |
| nltk                          | 3.4      |
| numpy                         | 1.15.4   |
| opencv-python                 | 3.4.4.19 |
| paddlepaddle-gpu              | 1.3.1    |
| Pillow                        | 5.3.0    |
| pip                           | 18.1     |
| protobuf                      | 3.1.0    |
| pyparsing                     | 2.3.0    |
| rarfile                       | 3.0      |
| recordio                      | 0.1.5    |
| requests                      | 2.9.2    |
| scipy                         | 1.1.0    |
| setuptools                    | 40.6.2   |
| six                           | 1.12.0   |
| subprocess32                  | 3.5.3    |
| wheel                         | 0.32.3   |

## 问题反馈

如在使用中遇到问题, 可以邮件至 aistudio-all@baidu.com







# 数据集

## 页面概览

![img](https://ai.bdstatic.com/file/0706D160DF354220929873553DF7AFDD)

## 创建数据集

如果当前预置的数据集不能满足使用, 可以自行上传数据集. 上传数据集有多个入口.

### 数据集大厅入口

![img](https://ai.bdstatic.com/file/4D0CA47E921042EEB7C96A459A3EE5B5)

点击【创建数据集】按钮, 则出现创建数据集的窗口.

![img](https://ai.bdstatic.com/file/4FDECE661B2D4F1E9684F1A36B358E4C)

按照提示上传数据文件, 点击【下一步】. ![img](https://ai.bdstatic.com/file/96492EDE14A0455CA0EDB117C29387C6)

在这个文本编辑框中可以对数据集背景、内容、来源、使用说明等信息进一步说明, 最后点击【创建】.

### 在创建项目时创建数据集入口

在创建项目时可以直接创建数据集并添加.

![img](https://ai.bdstatic.com/file/6502376F50F44703A85BE9EB49D300B9)

## 收藏数据集

![img](https://ai.bdstatic.com/file/8FBD8734FA0A417991192205B1E7F291)

## 数据集预览

支持`csv`格式数据文件预览功能.



![img](https://ai.bdstatic.com/file/EAE2754EEEA443BBB02091D911890B11)





# 比赛

## 概况

- 比赛内容: 参赛者将使用指定数据集, 分析数据, 然后运用机器学习、深度学习、数据挖掘等知识, 建立算法模型, 解决问题得出结果, 最后将结果提交. 如果提交的结果符合指标要求(常规赛)或者在参赛者中取得很好的排名(技术大赛),将获得比赛丰厚的奖金.
- 比赛目标：参赛者通过比赛检测AI技能、知识掌握的水平, 在解决问题中发挥自己的聪明才智, 同时获取奖励.
- 比赛分类: 常规赛, 技术大赛.

![img](https://ai.bdstatic.com/file/30CE94F98EC4435AAA6FCED3C5089769)

## 常规赛

- 赛制: 单轮, 设定通过分数.
- 参赛对象: 面向全社会开放，不限年龄、身份、国籍，相关领域的个人、高等院校、科研机构、企业单位、初创团队等人员均可报名参赛. 注意仅限个人参赛.
- 奖品: 通过即可获得奖品
- 举办时间: 每月一次

## 技术大赛

- 参赛对象: 面向全社会开放，相关领域的个人、科研机构、高等院校、企业单位等人员均可报名参加。团队报名或个人报名均可.
- 赛制: 多轮.
- 奖品: 前三名按比例分配奖池
- 举办时间: 不定期





