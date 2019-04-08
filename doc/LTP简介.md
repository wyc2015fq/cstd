# LTP简介

2019年01月08日 14:07:11 [Waldenz](https://me.csdn.net/enter89) 阅读数：71



语言技术平台（Language Technology Platform，LTP）是哈工大社会计算与信息检索研究中心历时十年研制的一整套开放中文自然语言处理系统。提供了一整套自底向上的丰富、高效、高精度的中文自然语言处理模块 （包括词法、句法、语义等5项中文处理核心技术）

​                             ![img](https://img-blog.csdnimg.cn/20190108135556155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

官网地址： <http://ltp.ai/index.html>

GitHub:  ltp: <https://github.com/HIT-SCIR/ltp>
              pyltp: <https://github.com/HIT-SCIR/pyltp>
              使用pyltp: <https://github.com/HIT-SCIR/pyltp/blob/master/doc/api.rst>

## 词性标注

LTP使用863词性标注集

|Tag	|Description	| Example|
| ---- | ---- | ---- |
|a| adjective：形容词| 美丽|
|b| other noun-modifier：其他的修饰名词|大型, 西式|
|c| conjunction：连词|和, 虽然|
|d| adverb：副词|很|
|e| exclamation：感叹词|哎|
|g| morpheme|茨, 甥|
|h| prefix：前缀| 阿, 伪|
|i| idiom：成语|百花齐放|
|j| abbreviation：缩写| 公检法?|
|k| suffix：后缀| 界, 率|
|m| number：数字| 一, 第一|
|n| general noun：一般名词| 苹果|
|nd|direction noun：方向名词| 右侧|
|nh|person name：人名| 杜甫, 汤姆|
|ni|organization name：公司名| 保险公司，中国银行|
|nl|location noun：地点名词| 城郊|
|ns|geographical name：地理名词| 北京|
|nt|temporal noun：时间名词| 近日, 明代|
|nz|other proper noun：其他名词| 诺贝尔奖|
|o| onomatopoeia：拟声词|哗啦|
|p| preposition：介词|在, 把，与|
|q| quantity：量词| 个|
|r| pronoun：代词|我们|
|u| auxiliary：助词|的, 地|
|v| verb：动词| 跑, 学习|
|wp|punctuation：标点|，。！|
|ws|foreign words：国外词|CPU|
|x| non-lexeme：不构成词| 萄, 翱|
|z|descriptive words 描写，叙述的词| 瑟瑟，匆匆|

 

## 依存句法

**依存语法 (Dependency Parsing, DP) 通过分析语言单位内成分之间的依存关系揭示其句法结构。 直观来讲，依存句法分析识别句子中的“主谓宾”、“定状补”这些语法成分，并分析各成分之间的关系。**

![img](https://img-blog.csdnimg.cn/20190108123135474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

```
#依存句法分析模型
parser = Parser()
parser.load(os.path.join(MODELDIR, "parser.model"))
arcs = parser.parse(words, postags)
 
print("\t".join("%d:%s" % (arc.head, arc.relation) for arc in arcs))
```

完整源码参考：<https://blog.csdn.net/enter89/article/details/86066243>

![img](https://img-blog.csdnimg.cn/20190108133411502.png)

arc.head 表示依存弧的父节点词的索引。ROOT节点的索引是0，第一个词开始的索引依次为1、2、3…
arc.relation 表示依存弧的关系。

| 关系类型   | Tag  | Description               | Example                   |
| :--------- | :--- | :------------------------ | :------------------------ |
| 主谓关系   | SBV  | subject-verb              | 我送她一束花 (我 <– 送)   |
| 动宾关系   | VOB  | 直接宾语，verb-object     | 我送她一束花 (送 –> 花)   |
| 间宾关系   | IOB  | 间接宾语，indirect-object | 我送她一束花 (送 –> 她)   |
| 前置宾语   | FOB  | 前置宾语，fronting-object | 他什么书都读 (书 <– 读)   |
| 兼语       | DBL  | double                    | 他请我吃饭 (请 –> 我)     |
| 定中关系   | ATT  | attribute                 | 红苹果 (红 <– 苹果)       |
| 状中结构   | ADV  | adverbial                 | 非常美丽 (非常 <– 美丽)   |
| 动补结构   | CMP  | complement                | 做完了作业 (做 –> 完)     |
| 并列关系   | COO  | coordinate                | 大山和大海 (大山 –> 大海) |
| 介宾关系   | POB  | preposition-object        | 在贸易区内 (在 –> 内)     |
| 左附加关系 | LAD  | left adjunct              | 大山和大海 (和 <– 大海)   |
| 右附加关系 | RAD  | right adjunct             | 孩子们 (孩子 –> 们)       |
| 独立结构   | IS   | independent structure     | 两个单句在结构上彼此独立  |
| 核心关系   | HED  | head                      | 指整个句子的核心          |

##  ![img](https://img-blog.csdnimg.cn/20190107170127516.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

![img](https://img-blog.csdnimg.cn/20190107170946195.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

   





## 语义依存

语义依存分析 (Semantic Dependency Parsing, SDP)，分析句子各个语言单位之间的语义关联，并将语义关联以依存结构呈现。 使用语义依存刻画句子语义，好处在于不需要去抽象词汇本身，而是通过词汇所承受的语义框架来描述该词汇，而论元的数目相对词汇来说数量总是少了很多的。语义依存分析目标是跨越句子表层句法结构的束缚，直接获取深层的语义信息。 例如以下三个句子，用不同的表达方式表达了同一个语义信息，即张三实施了一个吃的动作，吃的动作是对苹果实施的。

​                                         ![img](https://img-blog.csdnimg.cn/20190108113055981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

语义依存分析不受句法结构的影响，将具有直接语义关联的语言单元直接连接依存弧并标记上相应的语义关系。这也是语义依存分析与句法依存分析的重要区别。

​                              ![img](https://img-blog.csdnimg.cn/20190108113122989.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

如上例对比了句法依存和语义分析的结果，可以看到两者存在两个显著差别。第一，句法依存某种程度上更重视非实词（如介词）在句子结构分析中的作用，而语义依存更倾向在具有直接语义关联的实词之间建立直接依存弧，非实词作为辅助标记存在。 第二，两者依存弧上标记的语义关系完全不同，语义依存关系是由论元关系引申归纳而来，可以用于回答问题，如我在哪里喝汤，我在用什么喝汤，谁在喝汤，我在喝什么。但是句法依存却没有这个能力。

语义依存与语义角色标注之间也存在关联，语义角色标注只关注句子主要谓词的论元及谓词与论元之间的关系，而语义依存不仅关注谓词与论元的关系，还关注谓词与谓词之间、论元与论元之间、论元内部的语义关系。语义依存对句子语义信息的刻画更加完整全面。

语义依存关系分为三类，分别是主要语义角色，每一种语义角色对应存在一个嵌套关系和反关系；事件关系，描述两个事件间的关系；语义依附标记，标记说话者语气等依附性信息。

| 关系类型     | Tag           | Description        | Example                                   |
| :----------- | :------------ | :----------------- | :---------------------------------------- |
| 施事关系     | Agt           | Agent              | 我送她一束花 (我 <-- 送)                  |
| 当事关系     | Exp           | Experiencer        | 我跑得快 (跑 --> 我)                      |
| 感事关系     | Aft           | Affection          | 我思念家乡 (思念 --> 我)                  |
| 领事关系     | Poss          | Possessor          | 他有一本好读 (他 <-- 有)                  |
| 受事关系     | Pat           | Patient            | 他打了小明 (打 --> 小明)                  |
| 客事关系     | Cont          | Content            | 他听到鞭炮声 (听 --> 鞭炮声)              |
| 成事关系     | Prod          | Product            | 他写了本小说 (写 --> 小说)                |
| 源事关系     | Orig          | Origin             | 我军缴获敌人四辆坦克 (缴获 --> 坦克)      |
| 涉事关系     | Datv          | Dative             | 他告诉我个秘密 ( 告诉 --> 我 )            |
| 比较角色     | Comp          | Comitative         | 他成绩比我好 (他 --> 我)                  |
| 属事角色     | Belg          | Belongings         | 老赵有俩女儿 (老赵 <-- 有)                |
| 类事角色     | Clas          | Classification     | 他是中学生 (是 --> 中学生)                |
| 依据角色     | Accd          | According          | 本庭依法宣判 (依法 <-- 宣判)              |
| 缘故角色     | Reas          | Reason             | 他在愁女儿婚事 (愁 --> 婚事)              |
| 意图角色     | Int           | Intention          | 为了金牌他拼命努力 (金牌 <-- 努力)        |
| 结局角色     | Cons          | Consequence        | 他跑了满头大汗 (跑 --> 满头大汗)          |
| 方式角色     | Mann          | Manner             | 球慢慢滚进空门 (慢慢 <-- 滚)              |
| 工具角色     | Tool          | Tool               | 她用砂锅熬粥 (砂锅 <-- 熬粥)              |
| 材料角色     | Malt          | Material           | 她用小米熬粥 (小米 <-- 熬粥)              |
| 时间角色     | Time          | Time               | 唐朝有个李白 (唐朝 <-- 有)                |
| 空间角色     | Loc           | Location           | 这房子朝南 (朝 --> 南)                    |
| 历程角色     | Proc          | Process            | 火车正在过长江大桥 (过 --> 大桥)          |
| 趋向角色     | Dir           | Direction          | 部队奔向南方 (奔 --> 南)                  |
| 范围角色     | Sco           | Scope              | 产品应该比质量 (比 --> 质量)              |
| 数量角色     | Quan          | Quantity           | 一年有365天 (有 --> 天)                   |
| 数量数组     | Qp            | Quantity-phrase    | 三本书 (三 --> 本)                        |
| 频率角色     | Freq          | Frequency          | 他每天看书 (每天 <-- 看)                  |
| 顺序角色     | Seq           | Sequence           | 他跑第一 (跑 --> 第一)                    |
| 描写角色     | Desc(Feat)    | Description        | 他长得胖 (长 --> 胖)                      |
| 宿主角色     | Host          | Host               | 住房面积 (住房 <-- 面积)                  |
| 名字修饰角色 | Nmod          | Name-modifier      | 果戈里大街 (果戈里 <-- 大街)              |
| 时间修饰角色 | Tmod          | Time-modifier      | 星期一上午 (星期一 <-- 上午)              |
| 反角色       | r + main role |                    | 打篮球的小姑娘 (打篮球 <-- 姑娘)          |
| 嵌套角色     | d + main role |                    | 爷爷看见孙子在跑 (看见 --> 跑)            |
| 并列关系     | eCoo          | event Coordination | 我喜欢唱歌和跳舞 (唱歌 --> 跳舞)          |
| 选择关系     | eSelt         | event Selection    | 您是喝茶还是喝咖啡 (茶 --> 咖啡)          |
| 等同关系     | eEqu          | event Equivalent   | 他们三个人一起走 (他们 --> 三个人)        |
| 先行关系     | ePrec         | event Precedent    | 首先，先                                  |
| 顺承关系     | eSucc         | event Successor    | 随后，然后                                |
| 递进关系     | eProg         | event Progression  | 况且，并且                                |
| 转折关系     | eAdvt         | event adversative  | 却，然而                                  |
| 原因关系     | eCau          | event Cause        | 因为，既然                                |
| 结果关系     | eResu         | event Result       | 因此，以致                                |
| 推论关系     | eInf          | event Inference    | 才，则                                    |
| 条件关系     | eCond         | event Condition    | 只要，除非                                |
| 假设关系     | eSupp         | event Supposition  | 如果，要是                                |
| 让步关系     | eConc         | event Concession   | 纵使，哪怕                                |
| 手段关系     | eMetd         | event Method       |                                           |
| 目的关系     | ePurp         | event Purpose      | 为了，以便                                |
| 割舍关系     | eAban         | event Abandonment  | 与其，也不                                |
| 选取关系     | ePref         | event Preference   | 不如，宁愿                                |
| 总括关系     | eSum          | event Summary      | 总而言之                                  |
| 分叙关系     | eRect         | event Recount      | 例如，比方说                              |
| 连词标记     | mConj         | Recount Marker     | 和，或                                    |
| 的字标记     | mAux          | Auxiliary          | 的，地，得                                |
| 介词标记     | mPrep         | Preposition        | 把，被                                    |
| 语气标记     | mTone         | Tone               | 吗，呢                                    |
| 时间标记     | mTime         | Time               | 才，曾经                                  |
| 范围标记     | mRang         | Range              | 都，到处                                  |
| 程度标记     | mDegr         | Degree             | 很，稍微                                  |
| 频率标记     | mFreq         | Frequency Marker   | 再，常常                                  |
| 趋向标记     | mDir          | Direction Marker   | 上去，下来                                |
| 插入语标记   | mPars         | Parenthesis Marker | 总的来说，众所周知                        |
| 否定标记     | mNeg          | Negation Marker    | 不，没，未                                |
| 情态标记     | mMod          | Modal Marker       | 幸亏，会，能                              |
| 标点标记     | mPunc         | Punctuation Marker | ，。！                                    |
| 重复标记     | mPept         | Repetition Marker  | 走啊走 (走 --> 走)                        |
| 多数标记     | mMaj          | Majority Marker    | 们，等                                    |
| 实词虚化标记 | mVain         | Vain Marker        |                                           |
| 离合标记     | mSepa         | Seperation Marker  | 吃了个饭 (吃 --> 饭) 洗了个澡 (洗 --> 澡) |
| 根节点       | Root          | Root               | 全句核心节点                              |

语义依存分析参考官网：<http://www.ltp-cloud.com/intro#sdp_how>









## 命名实体识别标注集

NE识别模块的标注结果采用O-S-B-I-E标注形式，其含义为

| 标记 | 含义                 |
| :--- | :------------------- |
| O    | 这个词不是NE         |
| S    | 这个词单独构成一个NE |
| B    | 这个词为一个NE的开始 |
| I    | 这个词为一个NE的中间 |
| E    | 这个词位一个NE的结尾 |

LTP中的NE 模块识别三种NE，分别如下：

| 标记 | 含义   |
| :--- | :----- |
| Nh   | 人名   |
| Ni   | 机构名 |
| Ns   | 地名   |

举例：中国进出口银行与中国银行加强合作

下图中第一行为分词结果，第二行为词性标注，第三行为依存句法，第四行为命名实体识别，最后一行为语义角色标注

![img](https://img-blog.csdnimg.cn/20190108110658304.png)

图解

![img](https://img-blog.csdnimg.cn/20190108111701675.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

 

 



## 语义角色标注



语义角色标注 (Semantic Role Labeling, SRL) 是一种浅层的语义分析技术，标注句子中某些短语为给定谓词的论元 (语义角色) ，如施事、受事、时间和地点等。其能够对问答系统、信息抽取和机器翻译等应用产生推动作用。

![img](https://img-blog.csdnimg.cn/20190108123013794.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

```python
# 语义角色标注模型
labeller = SementicRoleLabeller()
labeller.load(os.path.join(MODELDIR, "pisrl_win.model"))
roles = labeller.label(words, postags, arcs)
 
for role in roles:
    print(role.index, "".join(
            ["%s:(%d,%d)" % (arg.name, arg.range.start, arg.range.end) for arg in role.arguments]))
```

完整源码参考：<https://blog.csdn.net/enter89/article/details/86066243>

![img](https://img-blog.csdnimg.cn/20190108115833471.png)

第一个词开始的索引一次是0、1、2....
返回结果 roles 是关于多个谓词的语义角色分析的结果。由于一句话中可能不含有语义角色，所以结果可能为空。
role.index 代表谓词的索引， role.arguments 代表关于该谓词的若干语义角色。
arg.name 表示语义角色类型，arg.range.start 表示该语义角色起始词位置的索引，arg.range.end 表示该语义角色结束词位置的索引。

上例子中的结果 为
7 TMP:(0,6)A1:(9,14)
9 A1:(10,10)
谓语索引7，即“提出”，这个谓语有两个语义角色，第一个为类型为TMP，即时间，范围为（0,6）;第二个语义角色类型为A1,即动作的影响，范围为(9,14)。
总结意思为： 在“**国务院总理李克强调研上海外高桥时**”这个时间“**提出**”了“**支持上海积极探索新机制**”。

核心的语义角色为 A0-5 六种，A0 通常表示动作的施事，A1通常表示动作的影响等，A2-5 根据谓语动词不同会有不同的语义含义。

| 标记 | 说明                 |
| :--- | :------------------- |
| A0   | 通常表示动作的施事   |
| A1   | 通常表示动作的影响等 |

其余的15个语义角色为附加语义角色，如LOC 表示地点，TMP 表示时间等。附加语义角色列表如下：

| 语义角色类型 | 说明                                        |
| :----------- | :------------------------------------------ |
| ADV          | adverbial, default tag ( 附加的，默认标记 ) |
| BNE          | beneﬁciary ( 受益人 )                       |
| CND          | condition ( 条件 )                          |
| DIR          | direction ( 方向 )                          |
| DGR          | degree ( 程度 )                             |
| EXT          | extent ( 扩展 )                             |
| FRQ          | frequency ( 频率 )                          |
| LOC          | locative ( 地点 )                           |
| MNR          | manner ( 方式 )                             |
| PRP          | purpose or reason ( 目的或原因 )            |
| TMP          | temporal ( 时间 )                           |
| TPC          | topic ( 主题 )                              |
| CRD          | coordinated arguments ( 并列参数 )          |
| PRD          | predicate ( 谓语动词 )                      |
| PSR          | possessor ( 持有者 )                        |
| PSE          | possessee ( 被持有 )                        |

​                                         ![img](https://img-blog.csdnimg.cn/20190107172214290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

 

 

官方完整解释：<http://www.ltp-cloud.com/intro#cws_how>







pyltp 完整Demo源码：<https://blog.csdn.net/enter89/article/details/86071483>

 