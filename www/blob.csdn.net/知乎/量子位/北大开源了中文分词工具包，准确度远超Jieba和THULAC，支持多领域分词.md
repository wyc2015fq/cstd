# 北大开源了中文分词工具包，准确度远超Jieba和THULAC，支持多领域分词 - 知乎
# 



> 车栗子 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-faf2e82e4ecf33546b5690f314fb1ac2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
“土地，快告诉俺老孙，俺的金箍棒在哪？”
“大圣，您的金箍，棒就棒在特别适合您的发型。”

中文分词，是一门高深莫测的技术。不论对于人类，还是对于AI。

最近，北大开源了一个**中文分词工具包**，名为**PKUSeg**，基于Python。

工具包的分词**准确率**，远远超过**THULAC**和**结巴分词**这两位重要选手。
![](https://pic2.zhimg.com/v2-9e3c52a989a33d9ff9f8b308ff0616e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='540'></svg>)△ 我们 [中出] 了个叛徒
除此之外，PKUSeg支持**多领域分词**，也支持用全新的标注数据来训练模型。

## **准确度远超对手**

这次比赛，PKUSeg的对手有两位：

一位是来自清华的**THULAC**，一位是要“做最好的中文分词组件”的**结巴分词**。它们都是目前主流的分词工具。

测试环境是Linux，测试数据集是**MSRA** (新闻数据) 和**CTB8** (混合型文本) 。

结果如下：
![](https://pic1.zhimg.com/v2-b439b68fdc033da32db168b0601feda8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='668'></svg>)
比赛用的评判标准，是第二届国际汉语分词评测比赛提供的分词评价脚本。

在**F分数**和**错误率**两项指标上，PKUSeg都明显优于另外两位对手。

## **食用方法**

**预训练模型**

PKUSeg提供了三个预训练模型，分别是在不同类型的数据集上训练的。

> 一是用MSRA (新闻语料) 训练出的模型：
[https://pan.baidu.com/s/1twci0QVBeWXUg06dK47tiA](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1twci0QVBeWXUg06dK47tiA)

二是用CTB8 (新闻文本及网络文本的混合型语料) 训练出的模型：
[https://pan.baidu.com/s/1DCjDOxB0HD2NmP9w1jm8MA](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1DCjDOxB0HD2NmP9w1jm8MA)

三是在微博 (网络文本语料) 上训练的模型：
[https://pan.baidu.com/s/1QHoK2ahpZnNmX6X7Y9iCgQ](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1QHoK2ahpZnNmX6X7Y9iCgQ)
![](https://pic2.zhimg.com/v2-400c9ec058412c3518abdc6a627115d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='151'></svg>)△ 微博语料举栗
大家可以按照自己的需要，选择加载不同的模型。

除此之外，也可以用全新的标注数据，来训练新的模型。




**代码示例**


```python
代码示例        使用默认模型及默认词典分词
import pkuseg
seg = pkuseg.pkuseg()                #以默认配置加载模型
text = seg.cut('我爱北京天安门')    #进行分词
print(text)

代码示例        设置用户自定义词典
import pkuseg
lexicon = ['北京大学', '北京天安门']    #希望分词时用户词典中的词固定不分开
seg = pkuseg.pkuseg(user_dict=lexicon)    #加载模型，给定用户词典
text = seg.cut('我爱北京天安门')        #进行分词
print(text)

代码示例
import pkuseg
seg = pkuseg.pkuseg(model_name='./ctb8')    #假设用户已经下载好了ctb8的模型并放在了'./ctb8'目录下，通过设置model_name加载该模型
text = seg.cut('我爱北京天安门')            #进行分词
print(text)
```


如果想自己训练一个新模型的话：


```python
代码示例
import pkuseg
pkuseg.train('msr_training.utf8', 'msr_test_gold.utf8', './models', nthread=)    #训练文件为'msr_training.utf8'，测试文件为'msr_test_gold.utf8'，模型存到'./models'目录下，开20个进程训练模型
```


欲知更详细的用法，可前往文底传送门。

## **快去试一下**

PKUSeg的作者有三位，Ruixuan Luo (罗睿轩)，Jingjing Xu (许晶晶) ，以及Xu Sun (孙栩) 。

工具包的诞生，也是基于其中两位参与的**ACL论文**。

准确率又那么高，还不去试试？




GitHub传送门：
[https://github.com/lancopku/PKUSeg-python](https://link.zhihu.com/?target=https%3A//github.com/lancopku/PKUSeg-python)




论文传送门：
![](https://pic3.zhimg.com/v2-8c9cb9b0df45c78051aa6fd1fba03a3e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='316'></svg>)



[http://www.aclweb.org/anthology/P12-1027](https://link.zhihu.com/?target=http%3A//www.aclweb.org/anthology/P12-1027)



![](https://pic4.zhimg.com/v2-7d75e32518254ad095430b96e0ed6beb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1062' height='388'></svg>)



[http://aclweb.org/anthology/P16-2092](https://link.zhihu.com/?target=http%3A//aclweb.org/anthology/P16-2092)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


