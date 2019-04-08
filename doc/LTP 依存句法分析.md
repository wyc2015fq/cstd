# LTP 依存句法分析

2019年01月07日 17:25:33 [Waldenz](https://me.csdn.net/enter89) 阅读数：451



#  **依存句法**

**依存语法 (Dependency Parsing, DP) 通过分析语言单位内成分之间的依存关系揭示其句法结构。 直观来讲，依存句法分析识别句子中的“主谓宾”、“定状补”这些语法成分，并分析各成分之间的关系。**

![img](https://img-blog.csdnimg.cn/20190108123135474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

```python
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

​                  ![img](https://img-blog.csdnimg.cn/20190107170946195.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

#  

 