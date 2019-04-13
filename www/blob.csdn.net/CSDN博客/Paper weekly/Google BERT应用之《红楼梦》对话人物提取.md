
# Google BERT应用之《红楼梦》对话人物提取 - Paper weekly - CSDN博客


2019年01月24日 08:50:23[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：258


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨庞龙刚
学校丨UC Berkeley博士后
研究方向丨高能核物理、人工智能

之前看到过一篇文章，通过提取文章中对话的人物，分析人物之间的关系，很好奇如何通过编程的方式知道一句话是谁说的。但是遍搜网络没有发现类似的研究。

前段时间看到一个微信里的读书小程序，将人物对话都提取出来，将一本书的内容通过微信对话的方式表达出来，通过将对话的主角替换成读者的微信号以及用户头像，从而增加读者的代入感。试了之后非常佩服程序作者的巧思。这使得我写一个自然语言处理程序，提取书中对话，以及对话人物的念头更加强烈。

之前并没有多少 NLP 的经验，只零碎试过用 LSTM 训练写唐诗，用 jieba 做分词，用 Google 的 gensim 在 WikiPedia 中文语料上训练词向量。最近 Google 的 BERT 模型很火，运行了 BERT 的 SQuAD 阅读理解与问答系统，分类器以及特征提取例子之后，觉得这个任务可以用 BERT 微调来完成，在这里记录实验的粗略步骤，与君共勉。

**我把训练数据和准备数据的脚本开源**，放在 GitLab 上，开放下载。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmkjPznGVkNrzmAH9VJHrfkpA7M5iaprAOvicBWTMKuz1OUsfHQ5dKYR0pbmH1z0REVRHTQ6LtY4Pjw/640?wx_fmt=png)

**该目录包含以下内容：**

用于提取对话人物语境的脚本 conversation_extraction.ipynb；
辅助打标签的脚本 label_data_by_click_buttons.ipynb；
提取出的语境文件：honglou.py；
打过标签的训练数据：label_honglou.txt；
从打过标签的数据合成百万级别新数据的脚本：augment_data.py；
将训练数据转换为 BERT/SQUAD 可读的脚本：prepare_squad_data.py；
预测结果文件：res.txt（使用 36000 组数据训练后的预测结果）；
预测结果文件：res_1p2million.txt（使用 120万 组数据训练后的预测结果）。

对比之后发现使用更多的数据训练所提升的效果有限，比较大的提升是后者在没有答案时，输出是输入的完整拷贝。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmkjPznGVkNrzmAH9VJHrfkFfc9Xibkb1ic0iaOyHoahr4CouXv7DibyWKQeuyPPo5TyuLUE8kF2x4mOA/640?wx_fmt=png)

BERT/SQuAD预言的结果可以从 res.txt 里面找到。

# 准备训练数据

《红楼梦》中的对话很好提取，大部分对话都有特定的格式，即一段话从：*“开始，从”*结束。使用 Python 的正则表达式，可以很容易提取所有满足这样条件的对话。

如果假设说出这段话的人的名字出现在这段话的前面，那么可以用这段话前面的一段话作为包含说话人（speaker）的上下文（context）。如果说话人不存在这段上下文中，标签为空字符串。

下面是第一步提取出的数据示例：

```python
{
```
```python
'istart'
```
```python
:
```
```python
414
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
457
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'原来如此，下愚不知．但那宝玉既有如此的来历，又何以情迷至此，复又豁悟如此？还要请教。'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'雨村听了，虽不能全然明白，却也十知四五，便点头叹道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
463
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
526
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'此事说来，老先生未必尽解．太虚幻境即是真如福地．一番阅册，原始要终之道，历历生平，如何不悟？仙草归真，焉有通灵不复原之理呢！'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'士隐笑道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
552
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
588
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'宝玉之事既得闻命，但是敝族闺秀如此之多，何元妃以下算来结局俱属平常呢？'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'雨村听着，却不明白了．知仙机也不便更问，因又说道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
880
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
891
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'此系后事，未便预说。'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'士隐微微笑道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
19
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
45
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'老先生草庵暂歇，我还有一段俗缘未了，正当今日完结。'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'食毕，雨村还要问自己的终身，士隐便道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
52
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
68
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'仙长纯修若此，不知尚有何俗缘？'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'雨村惊讶道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
51
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
77
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'大士，真人，恭喜，贺喜！情缘完结，都交割清楚了么？'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'这士隐自去度脱了香菱，送到太虚幻境，交那警幻仙子对册，刚过牌坊，见那一僧一道，缥渺而来．士隐接着说道：'
```
```python
},
```

```python
{
```
```python
'istart'
```
```python
:
```
```python
75
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
243
```
```python
,
```
```python
'talk'
```
```python
:
```
```python
'我从前见石兄这段奇文，原说可以闻世传奇，所以曾经抄录，但未见返本还原．不知何时复有此一佳话，方知石兄下凡一次，磨出光明，修成圆觉，也可谓无复遗憾了．只怕年深日久，字迹模糊，反有舛错，不如我再抄录一番，寻个世上无事的人，托他传遍，知道奇而不奇，俗而不俗，真而不真，假而不假．或者尘梦劳人，聊倩鸟呼归去，山灵好客，更从石化飞来，亦未可知。'
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'这一日空空道人又从青埂峰前经过，见那补天未用之石仍在那里，上面字迹依然如旧，又从头的细细看了一遍，见后面偈文后又历叙了多少收缘结果的话头，便点头叹道：'
```
```python
},
```

大部分数据的上下文都很简单，比如*'士隐笑道：'*等，但也有比较复杂的语境，比如*'这一日空空道人又从青埂峰前经过，见那补天未用之石仍在那里，上面字迹依然如旧，又从头的细细看了一遍，见后面偈文后又历叙了多少收缘结果的话头，便点头叹道：'*。

# 手动标记数据

为了训练机器，让它知道我想让它干什么，必须手动标记一些数据。我在 Jupyter notebook 下写了一个简单的 GUI 程序，将每段话变成按钮，只需要点击需要标记数据的句首和句尾，程序会自动计算标记数据在上下文中的位置，并将记录保存到文本中。

花了两个多小时，标记了大约 1500 多个数据，这些数据的最后几个例子如下：

```python
{
```
```python
'uid'
```
```python
:
```
```python
1552
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'黛玉又道：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'黛玉'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
0
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
2
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1553
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'因念云：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
None
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
-1
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
0
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1554
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'宝钗道：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'宝钗'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
0
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
2
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1555
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'五祖便将衣钵传他．今儿这偈语，亦同此意了．只是方才这句机锋，尚未完全了结，这便丢开手不成？"黛玉笑道：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'黛玉'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
46
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
48
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1556
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'宝玉自己以为觉悟，不想忽被黛玉一问，便不能答，宝钗又比出"语录"来，此皆素不见他们能者．自己想了一想：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'宝玉'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
0
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
2
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1557
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'想毕，便笑道：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
None
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
-1
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
0
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1558
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'说着，四人仍复如旧．忽然人报，娘娘差人送出一个灯谜儿，命你们大家去猜，猜着了每人也作一个进去．四人听说忙出去，至贾母上房．只见一个小太监，拿了一盏四角平头白纱灯，专为灯谜而制，上面已有一个，众人都争看乱猜．小太监又下谕道：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'小太监'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
103
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
106
```
```python
}
```

```python
{
```
```python
'uid'
```
```python
:
```
```python
1559
```
```python
,
```
```python
'context'
```
```python
:
```
```python
'太监去了，至晚出来传谕：'
```
```python
,
```
```python
'speaker'
```
```python
:
```
```python
'太监'
```
```python
,
```
```python
'istart'
```
```python
:
```
```python
0
```
```python
,
```
```python
'iend'
```
```python
:
```
```python
2
```
```python
}
```

1500 个数据太少了，为了增加数据量，我又做了 data augmentation，将 1500 多个 speaker 插入到 1500 多个语境中，凭空生成了 200多万对训练数据。所以在训练数据中，有一些非常搞笑的内容，比如：


> 说毕走来，只见
> 宝玉
> 拄着拐棍，在当地骂袭人：

这个训练例子中的**宝玉**，原文应该是**李嬷嬷**。


# 训练过程

简单构造 SQUAD 的中文训练和测试数据，训练并预测，结果输出在 predictions.json 中。

训练数据的 json 格式如下：

```python
{
```
```python
"data"
```
```python
: [{
```
```python
"title"
```
```python
:
```
```python
"红楼梦"
```
```python
,
```
```python
"paragraphs"
```
```python
:[{context
```
```python
and
```
```python
qas item
```
```python
1
```
```python
}, {context
```
```python
and
```
```python
qas item
```
```python
2
```
```python
}, ... {context
```
```python
and
```
```python
qas item i}, ..., {context
```
```python
and
```
```python
qas item n}]},
```

```python
{
```
```python
"title"
```
```python
:
```
```python
"寻秦记"
```
```python
,
```
```python
"paragraphs"
```
```python
:[{}, {}, {}]},
```

```python
{
```
```python
"title"
```
```python
:
```
```python
"xxxxxx"
```
```python
,
```
```python
"paragraphs"
```
```python
:[{}, {}, {}]}],
```

```python
"version"
```
```python
:
```
```python
"speaker1.0"
```
```python
}
```

输入数据是个字典，包含 “data" 和 "version" 两个键值。data 是个数组，里面的每一项对应一本书，以及这本书中的的「语境，问题，答案」字典列表。

对于每个「语境，问题，答案」，其格式又如下：

```python
{context
```
```python
and
```
```python
qas item
```
```python
1
```
```python
} =
```

```python
{
```
```python
"context"
```
```python
:
```
```python
"正闹着，贾母遣人来叫他吃饭，方往前边来，胡乱吃了半碗，仍回自己房中．只见袭人睡在外头炕上，麝月在旁边抹骨牌．宝玉素知麝月与袭人亲厚，一并连麝月也不理，揭起软帘自往里间来．麝月只得跟进来．平儿便推他出去，说："
```
```python
,
```

```python
"qas"
```
```python
: [ {
```
```python
"answers"
```
```python
:[{
```
```python
"answer_start"
```
```python
:
```
```python
46
```
```python
,
```
```python
"text"
```
```python
:
```
```python
"平儿"
```
```python
}],
```

```python
"question"
```
```python
:
```
```python
"接下来一句话是谁说的"
```
```python
,
```

```python
"id"
```
```python
:
```
```python
"index"
```
```python
},
```

```python
{question answer pair
```
```python
2
```
```python
},
```

```python
..., {question answer pair n}]
```

```python
}
```

在这次尝试中，我只使用了经过 Data Augmentation 生成的 200 多万组数据中的 36000 组做训练。BERT 的 SQUAD 训练脚本 test_squad.sh 设置基本没改变，最大的改变是 max_seq_length=128，以及训练数据测试数据文件所在位置及内容。

```python
export BERT_BASE_DIR=
```
```python
"pathto/chinese_L-12_H-768_A-12"
```

```python
export SQUAD_DIR=
```
```python
"pathto/squad_data_chinese"
```

```python
python pathto/run_squad.py \
```

```python
--vocab_file=$BERT_BASE_DIR/vocab.txt \
```

```python
--bert_config_file=$BERT_BASE_DIR/bert_config.json \
```

```python
--init_checkpoint=$BERT_BASE_DIR/bert_model.ckpt \
```

```python
--do_train=
```
```python
True
```
```python
\
```

```python
--train_file=$SQUAD_DIR/chinese_speaker_squad.json \
```

```python
--do_predict=
```
```python
True
```
```python
\
```

```python
--predict_file=$SQUAD_DIR/chinese_speaker_squad_valid.json \
```

```python
--train_batch_size=
```
```python
12
```
```python
\
```

```python
--learning_rate=
```
```python
3e-5
```
```python
\
```

```python
--num_train_epochs=
```
```python
2.0
```
```python
\
```

```python
--max_seq_length=
```
```python
128
```
```python
\
```

```python
--doc_stride=
```
```python
128
```
```python
\
```

```python
--output_dir=pathto/squad_data_chinese
```

# 预测结果

因为 BERT 在维基百科的大量中文语料上做过训练，已经掌握了中文的基本规律。而少量的训练数据微调，即可让 BERT 知道它所需要处理的任务类型。

通过简单的阅读理解与问答训练，说话人提取的任务效果惊人，虽然还没有人工完全验证提取结果的正确性，但是从语境和答案对看来，大部分结果无差错。

总共数据是 10683 条，打了标签的训练数据是前面的 1500 多条。下面将预测的 10683 条中从后往前数的部分预测结果列出。

```python
想了一回，也觉解了好些．又想到袭人身上： |||
```
```python
袭人（此预测结果❌）
```

```python
那日薛姨妈并未回家，因恐宝钗痛哭，所以在宝钗房中解劝．那宝钗却是极明理，思前想后，宝玉原是一种奇异的人．夙世前因，自有一定，原无可怨天尤人．了．薛姨妈心里反倒安了，便到王夫人那里先把宝钗的话说了．王夫人点头叹道： ||| 王夫人
```

```python
说着，更又伤心起来．薛姨妈倒又劝了一会子，因又提起袭人来，说： ||| 薛姨妈
```

```python
王夫人道： ||| 王夫人
```

```python
薛姨妈道： ||| 薛姨妈
```

```python
王夫人听了道： ||| 王夫人
```

```python
薛姨妈听了点头道： ||| 薛姨妈
```

```python
看见袭人泪痕满面，薛姨妈便劝解譬喻了一会．W袭人本来老实，不是伶牙利齿的人，薛姨妈说一句，他应一句，回来说道： ||| 薛姨妈 （此结果从语境看不出是否正确）
```

```python
过了几日，贾政回家，众人迎接．贾政见贾赦贾珍已都回家，弟兄叔侄相见，大家历叙别来的景况．然后内眷们见了，不免想起宝玉来，又大家伤了一会子心．贾政喝住道： ||| 贾政
```

```python
次日贾政进内，请示大臣们，说是： ||| 贾政
```

```python
回到家中，贾琏贾珍接着，贾政将朝内的话述了一遍，众人喜欢．贾珍便回说： ||| 贾珍
```

```python
贾政并不言语，隔了半日，却吩咐了一番仰报天恩的话．贾琏也趁便回说： ||| 贾琏
```

```python
贾政昨晚也知巧姐的始末，便说： ||| 贾政
```

```python
贾琏答应了
```
```python
"是"
```
```python
，又说： ||| 贾琏
```

```python
贾政道： ||| 贾政
```

```python
贾政说毕进内．贾琏打发请了刘姥姥来，应了这件事．刘姥姥见了王夫人等，便说些将来怎样升官，怎样起家，怎样子孙昌盛．正说着，丫头回道： ||| 丫头
```

```python
王夫人问几句话，花自芳的女人将亲戚作媒，说的是城南蒋家的，现在有房有地，又有铺面，姑爷年纪略大了几岁，并没有娶过的，况且人物儿长的是百里挑一的．王夫人听了愿意，说道： ||| 王夫人
```

```python
王夫人又命人打听，都说是好．王夫人便告诉了宝钗，仍请了薛姨妈细细的告诉了袭人．袭人悲伤不已，又不敢违命的，心里想起宝玉那年到他家去，回来说的死也不回去的话，
```
```python
"如今太太硬作主张．若说我守着，又叫人说我不害臊，若是去了，实不是我的心愿"
```
```python
，便哭得咽哽难鸣，又被薛姨妈宝钗等苦劝，回过念头想道： |||
```
```python
薛姨妈宝钗（此预测结果❌）
```

```python
于是，袭人含悲叩辞了众人，那姐妹分手时自然更有一番不忍说．袭人怀着必死的心肠上车回去，见了哥哥嫂子，也是哭泣，但只说不出来．那花自芳悉把蒋家的娉礼送给他看，又把自己所办妆奁一一指给他瞧，说那是太太赏的，那是置办的．袭人此时更难开口，住了两天，细想起来： ||| 袭人
```

```python
不言袭人从此又是一番天地．且说那贾雨村犯了婪索的案件，审明定罪，今遇大赦，褫籍为民．雨村因叫家眷先行，自己带了一个小厮，一车行李，来到急流津觉迷渡口．只见一个道者从那渡头草棚里出来，执手相迎．雨村认得是甄士隐，也连忙打恭，士隐道： ||| 士隐
```

```python
雨村道： ||| 雨村
```

```python
甄士隐道： ||| 甄士隐
```

```python
雨村欣然领命，两人携手而行，小厮驱车随后，到了一座茅庵．士隐让进雨村坐下，小童献上茶来．雨村便请教仙长超尘的始末．士隐笑道： ||| 士隐
```

```python
雨村道： ||| 雨村
```

```python
士隐道： ||| 士隐
```

```python
雨村惊讶道： ||| 雨村
```

```python
士隐道： ||| 士隐
```

```python
雨村道： ||| 雨村
```

```python
士隐道： ||| 士隐
```

```python
雨村听了，虽不能全然明白，却也十知四五，便点头叹道： ||| 雨村
```

```python
士隐笑道： ||| 士隐
```

```python
雨村听着，却不明白了．知仙机也不便更问，因又说道： |||
```
```python
雨村听着，却不明白了．知仙机（此预测结果❌）
```

```python
士隐叹息道： ||| 士隐
```

```python
雨村听到这里，不觉拈须长叹，因又问道： ||| 雨村
```

```python
士隐道： ||| 士隐
```

```python
雨村低了半日头，忽然笑道： ||| 雨村
```

```python
士隐微微笑道： ||| 士隐
```

```python
食毕，雨村还要问自己的终身，士隐便道： ||| 士隐
```

```python
雨村惊讶道： ||| 雨村
```

```python
士隐道： ||| 士隐
```

```python
这士隐自去度脱了香菱，送到太虚幻境，交那警幻仙子对册，刚过牌坊，见那一僧一道，缥渺而来．士隐接着说道： ||| 士隐
```

```python
那僧说： ||| 那僧
```

```python
这一日空空道人又从青埂峰前经过，见那补天未用之石仍在那里，上面字迹依然如旧，又从头的细细看了一遍，见后面偈文后又历叙了多少收缘结果的话头，便点头叹道： ||| 空空道人
```

```python
想毕，便又抄了，仍袖至那繁华昌盛的地方，遍寻了一番，不是建功立业之人，即系饶口谋衣之辈，那有闲情更去和石头饶舌．直寻到急流津觉迷度口，草庵中睡着一个人，因想他必是闲人，便要将这抄录的《石头记》给他看看．那知那人再叫不醒．空空道人复又使劲拉他，才慢慢的开眼坐起，便草草一看，仍旧掷下道： ||| 空空道人
```

```python
空空道人忙问何人，那人道： ||| 那人
```

```python
那空空道人牢牢记着此言，又不知过了几世几劫，果然有个悼红轩，见那曹雪芹先生正在那里翻阅历来的古史．空空道人便将贾雨村言了，方把这《石头记》示看．那雪芹先生笑道： ||| 雪芹先生
```

```python
空空道人便问： ||| 空空道人
```

```python
曹雪芹先生笑道： ||| 曹雪芹先生
```

```python
那空空道人听了，仰天大笑，掷下抄本，飘然而去．一面走着，口中说道： ||| 空空道人
```

结果分析：大部分简单的语境，BERT 都可以正确的预测谁是说话的那个人，但是有些复杂一点的，就会出错，比如上面这些例子中的：

```python
想了一回，也觉解了好些．又想到袭人身上： |||
```
```python
袭人（此预测结果❌）
```

```python
王夫人又命人打听，都说是好．王夫人便告诉了宝钗，仍请了薛姨妈细细的告诉了袭人．袭人悲伤不已，又不敢违命的，心里想起宝玉那年到他家去，回来说的死也不回去的话，
```
```python
"如今太太硬作主张．若说我守着，又叫人说我不害臊，若是去了，实不是我的心愿"
```
```python
，便哭得咽哽难鸣，又被薛姨妈宝钗等苦劝，回过念头想道： |||
```
```python
薛姨妈宝钗（此预测结果❌）
```

```python
雨村听着，却不明白了．知仙机也不便更问，因又说道： |||
```
```python
雨村听着，却不明白了．知仙机（此预测结果❌）
```

第三个错误最是搞笑，好像机器还没有明白**“雨村听着，却不明白了．知仙机”**并不是一个人的名字。

下面我再从其他预言的结果中挑选了一些看起来不容易预测，但是机器正确理解并预测的例子：

```python
10575
```
```python
贾兰那里肯走．尤氏等苦劝不止．众人中只有
```
```python
惜春
```
```python
心里却明白了，只不好说出来，便问宝钗道： ||| 惜春
```

```python
10183
```
```python
王夫人
```
```python
已到宝钗那里，见宝玉神魂失所，心下着忙，便说袭人道： ||| 王夫人
```

```python
王仁
```
```python
便叫了他外甥女儿巧姐过来说： ||| 王仁
```
```python
（下面一句话算谁说的？我也很懵）
```

```python
9490
```
```python
正推让着，
```
```python
宝玉
```
```python
也来请薛姨妈李婶娘的安．听见宝钗自己推让，他心里本早打算过宝钗生日，因家中闹得七颠八倒，也不敢在贾母处提起，今见湘云等众人要拜寿，便喜欢道： ||| 宝玉
```

# 人物关系分析

按照相邻的两个说话者极有可能是对话者统计出红楼梦中人物关系如下，宝玉与袭人之间对话最多（178+175），宝玉与黛玉之间对话次之（177+174），宝玉与宝钗之间对话（65+61），仅从对话次数来看，袭人与黛玉在宝玉心目中的占地差不多，宝钗（65+61）占地只相当于黛玉的三分之一，略高于晴雯（46+41）。

通过这个例子，深深感觉 Google 的 BERT 预训练+微调的自然语言处理模型之强大。很多 NLP 的问题可以转换成 “阅读理解 + 问答”（SQuAD）的问题。在此写下假期 3 天做的一个有趣的尝试，希望看到更多使用 BERT 开发出更多好玩的应用。

```python
[(
```
```python
'宝玉-袭人'
```
```python
,
```
```python
178
```
```python
),
```

```python
(
```
```python
'黛玉-宝玉'
```
```python
,
```
```python
177
```
```python
),
```

```python
(
```
```python
'袭人-宝玉'
```
```python
,
```
```python
175
```
```python
),
```

```python
(
```
```python
'宝玉-黛玉'
```
```python
,
```
```python
174
```
```python
),
```

```python
(
```
```python
'宝玉-宝玉'
```
```python
,
```
```python
137
```
```python
),
```

```python
(
```
```python
'贾母-贾母'
```
```python
,
```
```python
115
```
```python
),
```

```python
(
```
```python
'宝玉-宝钗'
```
```python
,
```
```python
65
```
```python
),
```

```python
(
```
```python
'凤姐-凤姐'
```
```python
,
```
```python
64
```
```python
),
```

```python
(
```
```python
'宝钗-宝玉'
```
```python
,
```
```python
61
```
```python
),
```

```python
(
```
```python
'黛玉-黛玉'
```
```python
,
```
```python
59
```
```python
),
```

```python
(
```
```python
'贾母-凤姐'
```
```python
,
```
```python
57
```
```python
),
```

```python
(
```
```python
'贾政-贾政'
```
```python
,
```
```python
54
```
```python
),
```

```python
(
```
```python
'袭人-袭人'
```
```python
,
```
```python
48
```
```python
),
```

```python
(
```
```python
'宝玉-晴雯'
```
```python
,
```
```python
46
```
```python
),
```

```python
(
```
```python
'贾琏-凤姐'
```
```python
,
```
```python
46
```
```python
),
```

```python
(
```
```python
'宝钗-黛玉'
```
```python
,
```
```python
45
```
```python
),
```

```python
(
```
```python
'凤姐-贾母'
```
```python
,
```
```python
44
```
```python
),
```

```python
(
```
```python
'黛玉-宝钗'
```
```python
,
```
```python
42
```
```python
),
```

```python
(
```
```python
'凤姐-贾琏'
```
```python
,
```
```python
42
```
```python
),
```

```python
(
```
```python
'王夫人-贾母'
```
```python
,
```
```python
41
```
```python
),
```

```python
(
```
```python
'宝玉-贾母'
```
```python
,
```
```python
41
```
```python
),
```

```python
(
```
```python
'晴雯-宝玉'
```
```python
,
```
```python
41
```
```python
),
```

```python
(
```
```python
'王夫人-宝玉'
```
```python
,
```
```python
41
```
```python
),
```

```python
(
```
```python
'贾母-宝玉'
```
```python
,
```
```python
40
```
```python
),
```

```python
(
```
```python
'宝玉-贾政'
```
```python
,
```
```python
39
```
```python
),
```

```python
(
```
```python
'黛玉-紫鹃'
```
```python
,
```
```python
39
```
```python
),
```

```python
(
```
```python
'黛玉-湘云'
```
```python
,
```
```python
38
```
```python
),
```

```python
(
```
```python
'紫鹃-黛玉'
```
```python
,
```
```python
37
```
```python
),
```

```python
(
```
```python
'凤姐儿-贾母'
```
```python
,
```
```python
35
```
```python
),
```

```python
(
```
```python
'众人-贾政'
```
```python
,
```
```python
35
```
```python
)]
```

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
AI Challenger 2018 机器翻译参赛总结
Airbnb实时搜索排序中的Embedding技巧
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐


