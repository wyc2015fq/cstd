
# pyltp 完整Demo源码 - 追求卓越,做到专业 - CSDN博客


2019年01月08日 13:44:16[Waldenz](https://me.csdn.net/enter89)阅读数：92



```python
import sys, os
# ROOTDIR = os.path.join(os.path.dirname(__file__), os.pardir)
ROOTDIR = os.path.join(os.path.dirname(__file__))
sys.path = [os.path.join(ROOTDIR, "lib")] + sys.path
# Set your own model path
MODELDIR=os.path.join(ROOTDIR, "./ltp_data")
from pyltp import SentenceSplitter, Segmentor, Postagger, Parser, NamedEntityRecognizer, SementicRoleLabeller
paragraph = '东湖公安分局于22日凌晨对熊某某作出行政拘留10日的处罚。中国进出口银行与中国银行加强合作！王某某 涉嫌 诈骗 被拘留。国务院总理李克强调研上海外高桥时提出，支持上海积极探索新机制。 '
sentence = SentenceSplitter.split(paragraph)[3]
# 分词模型
segmentor = Segmentor()
segmentor.load(os.path.join(MODELDIR, "cws.model"))
words = segmentor.segment(sentence)
print("\t".join(words))
#词性标注模型
postagger = Postagger()
postagger.load(os.path.join(MODELDIR, "pos.model"))
postags = postagger.postag(words)
# list-of-string parameter is support in 0.1.5
# postags = postagger.postag(["中国","进出口","银行","与","中国银行","加强","合作"])
print("\t".join(postags))
#依存句法分析模型
parser = Parser()
parser.load(os.path.join(MODELDIR, "parser.model"))
arcs = parser.parse(words, postags)
print("\t".join("%d:%s" % (arc.head, arc.relation) for arc in arcs))
# 命名实体识别模型
recognizer = NamedEntityRecognizer()
recognizer.load(os.path.join(MODELDIR, "ner.model"))
netags = recognizer.recognize(words, postags)
print("\t".join(netags))
# 语义角色标注模型
labeller = SementicRoleLabeller()
labeller.load(os.path.join(MODELDIR, "pisrl_win.model"))
roles = labeller.label(words, postags, arcs)
for role in roles:
    print(role.index, "".join(
            ["%s:(%d,%d)" % (arg.name, arg.range.start, arg.range.end) for arg in role.arguments]))
segmentor.release()
postagger.release()
parser.release()
recognizer.release()
labeller.release()
```
运行结果：
![](https://img-blog.csdnimg.cn/2019010812043043.png)

