
# LTP 语义角色标注 - 追求卓越,做到专业 - CSDN博客


2019年01月08日 12:27:56[Waldenz](https://me.csdn.net/enter89)阅读数：407


# 语义角色标注
语义角色标注 (Semantic Role Labeling, SRL) 是一种浅层的语义分析技术，标注句子中某些短语为给定谓词的论元 (语义角色) ，如施事、受事、时间和地点等。其能够对问答系统、信息抽取和机器翻译等应用产生推动作用。
![](https://img-blog.csdnimg.cn/20190108123013794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
```python
# 语义角色标注模型
labeller = SementicRoleLabeller()
labeller.load(os.path.join(MODELDIR, "pisrl_win.model"))
roles = labeller.label(words, postags, arcs)
for role in roles:
    print(role.index, "".join(
            ["%s:(%d,%d)" % (arg.name, arg.range.start, arg.range.end) for arg in role.arguments]))
```
完整源码参考：[https://blog.csdn.net/enter89/article/details/86066243](https://blog.csdn.net/enter89/article/details/86066243)
![](https://img-blog.csdnimg.cn/20190108115833471.png)
第一个词开始的索引一次是0、1、2....
返回结果 roles 是关于多个谓词的语义角色分析的结果。由于一句话中可能不含有语义角色，所以结果可能为空。
role.index 代表谓词的索引， role.arguments 代表关于该谓词的若干语义角色。
arg.name 表示语义角色类型，arg.range.start 表示该语义角色起始词位置的索引，arg.range.end 表示该语义角色结束词位置的索引。
上例子中的结果 为
7 TMP:(0,6)A1:(9,14)
9 A1:(10,10)
谓语索引7，即“提出”，这个谓语有两个语义角色，第一个为类型为TMP，即时间，范围为（0,6）;第二个语义角色类型为A1,即动作的影响，范围为(9,14)。
总结意思为： 在“**国务院总理李克强调研上海外高桥时**”这个时间“**提出**”了“**支持上海积极探索新机制**”。
核心的语义角色为 A0-5 六种，A0 通常表示动作的施事，A1通常表示动作的影响等，A2-5 根据谓语动词不同会有不同的语义含义。
|标记|说明|
|A0|通常表示动作的施事|
|A1|通常表示动作的影响等|
其余的15个语义角色为附加语义角色，如LOC 表示地点，TMP 表示时间等。附加语义角色列表如下：
|语义角色类型|说明|
|---|---|
|ADV|adverbial, default tag ( 附加的，默认标记 )|
|BNE|beneﬁciary ( 受益人 )|
|CND|condition ( 条件 )|
|DIR|direction ( 方向 )|
|DGR|degree ( 程度 )|
|EXT|extent ( 扩展 )|
|FRQ|frequency ( 频率 )|
|LOC|locative ( 地点 )|
|MNR|manner ( 方式 )|
|PRP|purpose or reason ( 目的或原因 )|
|TMP|temporal ( 时间 )|
|TPC|topic ( 主题 )|
|CRD|coordinated arguments ( 并列参数 )|
|PRD|predicate ( 谓语动词 )|
|PSR|possessor ( 持有者 )|
|PSE|possessee ( 被持有 )|
![](https://img-blog.csdnimg.cn/20190107172214290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

官方完整解释：[http://www.ltp-cloud.com/intro\#cws_how](http://www.ltp-cloud.com/intro#cws_how)

