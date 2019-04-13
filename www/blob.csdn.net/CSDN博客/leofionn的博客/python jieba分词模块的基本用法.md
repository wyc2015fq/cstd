
# python jieba分词模块的基本用法 - leofionn的博客 - CSDN博客


2018年05月22日 12:40:04[leofionn](https://me.csdn.net/qq_36142114)阅读数：114个人分类：[python																](https://blog.csdn.net/qq_36142114/article/category/7385154)



# 安装jieba
`pip install jieba`
# 简单用法
结巴分词分为三种模式：精确模式（默认）、全模式和搜索引擎模式，下面对这三种模式分别举例介绍：
### 精确模式
```python
import
```
```python
jieba
s
```
```python
=
```
```python
u'我想和女朋友一起去北京故宫博物院参观和闲逛。'
```

```python
cut
```
```python
=
```
```python
jieba.cut(s)
```
```python
print
```
```python
'【Output】'
```
```python
print
```
```python
cut
```
```python
print
```
```python
','
```
```python
.join(cut)
```

```python
【
```
```python
Output
```
```python
】
<generator
```
```python
object
```
```python
cut
```
```python
at
```
```python
0
```
```python
x7f8dbc0efc30>
```
```python
我,想,和,女朋友,一起,去,北京故宫博物院,参观,和,闲逛,。
```
可见分词结果返回的是一个生成器（这对大数据量数据的分词尤为重要）。
### 全模式
```python
print
```
```python
'【Output】'
```
```python
print
```
```python
','
```
```python
.join(jieba.cut(s,cut_all
```
```python
=
```
```python
True
```
```python
))
```

```python
【Output】
我,想,和,女朋友,朋友,一起,去,北京,北京故宫,北京故宫博物院,故宫,故宫博物院,博物,博物院,参观,和,闲逛,,
```
可见全模式就是把文本分成尽可能多的词。
### 搜索引擎模式
```python
print
```
```python
'【Output】'
```
```python
print
```
```python
','
```
```python
.join(jieba.cut_for_search(s))
```

```python
【Output】
我,想,和,朋友,女朋友,一起,去,北京,故宫,博物,博物院,北京故宫博物院,参观,和,闲逛,。
```
# 获取词性
每个词都有其词性，比如名词、动词、代词等，结巴分词的结果也可以带上每个词的词性，要用到jieba.posseg，举例如下：
```python
import
```
```python
jieba.posseg
```
```python
as
```
```python
psg
```
```python
print
```
```python
'【Output】'
```
```python
print
```
```python
[(x.word,x.flag)
```
```python
for
```
```python
x
```
```python
in
```
```python
psg.cut(s)]
```
```python
# 输出：
```
```python
'''
```
```python
[(u'我', u'r'), (u'想', u'v'), (u'和', u'c'), (u'女朋友', u'n'), (u'一起', u'm'),
```
```python
(u'去', u'v'), (u'北京故宫博物院', u'ns'), (u'参观', u'n'), (u'和', u'c'), (u'闲逛', u'v'), (u'。', u'x')]
```
```python
'''
```
可以看到成功获取到每个词的词性，这对于我们对分词结果做进一步处理很有帮助，比如只想获取分词结果列表中的名词，那么就可以这样过滤：
```python
print
```
```python
[(x.word,x.flag)
```
```python
for
```
```python
x
```
```python
in
```
```python
psg.cut(s)
```
```python
if
```
```python
x.flag.startswith(
```
```python
'n'
```
```python
)]
```
```python
# 输出：
```
```python
'''
```
```python
[(u'女朋友', u'n'), (u'北京故宫博物院', u'ns'), (u'参观', u'n')]
```
```python
'''
```
至于词性的每个字母分别表示什么词性，jieba分词的结果可能有哪些词性，就要去查阅词性对照表了，本文结尾附了一份从网上搜到的词性对照表，想了解更详细的词性分类信息，可以到网上搜索"结巴分词词性对照"。
# 并行分词
在文本数据量非常大的时候，为了提高分词效率，开启并行分词就很有必要了。jieba支持并行分词，基于python自带的multiprocessing模块，但要注意的是在Windows环境下不支持。
用法：
```python
# 开启并行分词模式，参数为并发执行的进程数
```
```python
jieba.enable_parallel(
```
```python
5
```
```python
)
```
```python
# 关闭并行分词模式
```
```python
jieba.disable_parallel()
```
举例：开启并行分词模式对三体全集文本进行分词
```python
santi_text
```
```python
=
```
```python
open
```
```python
(
```
```python
'./santi.txt'
```
```python
).read()
```
```python
print
```
```python
len
```
```python
(santi_text)
```

```python
2681968
```
可以看到三体全集的数据量还是非常大的，有260多万字节的长度。
```python
jieba.enable_parallel(
```
```python
100
```
```python
)
santi_words
```
```python
=
```
```python
[x
```
```python
for
```
```python
x
```
```python
in
```
```python
jieba.cut(santi_text)
```
```python
if
```
```python
len
```
```python
(x)
```
```python
>=
```
```python
2
```
```python
]
jieba.disable_parallel()
```

# 获取出现频率Top n的词
还是以上面的三体全集文本为例，假如想要获取分词结果中出现频率前20的词列表，可以这样获取：
```python
from
```
```python
collections
```
```python
import
```
```python
Counter
c
```
```python
=
```
```python
Counter(santi_words).most_common(
```
```python
20
```
```python
)
```
```python
print
```
```python
c
```
```python
# 输出：
```
```python
'''
```
```python
[(u'\r\n', 21805), (u'一个', 3057), (u'没有', 2128), (u'他们', 1690), (u'我们', 1550),
```
```python
(u'这个', 1357), (u'自己', 1347), (u'程心', 1320), (u'现在', 1273), (u'已经', 1259),
```
```python
(u'世界', 1243), (u'罗辑', 1189), (u'可能', 1177), (u'什么', 1176), (u'看到', 1114),
```
```python
(u'知道', 1094), (u'地球', 951), (u'人类', 935), (u'太空', 930), (u'三体', 883)]
```
```python
'''
```
可以看到结果中'\r\n'居然是出现频率最高的词，还有'一个'、'没有'、'这个'等这种我们并不想要的无实际意义的词，那么就可以根据前面说的词性来进行过滤，这个以后细讲。
# 使用用户字典提高分词准确性
不使用用户字典的分词结果：
```python
txt
```
```python
=
```
```python
u'欧阳建国是创新办主任也是欢聚时代公司云计算方面的专家'
```
```python
print
```
```python
','
```
```python
.join(jieba.cut(txt))
```

```python
欧阳,建国,是,创新,办,主任,也,是,欢聚,时代,公司,云,计算,方面,的,专家
```
使用用户字典的分词结果：
```python
jieba.load_userdict(
```
```python
'user_dict.txt'
```
```python
)
```
```python
print
```
```python
','
```
```python
.join(jieba.cut(txt))
```

```python
欧阳建国,是,创新办,主任,也,是,欢聚时代,公司,云计算,方面,的,专家
```
可以看出使用用户字典后分词准确性大大提高。
注：其中user_dict.txt的内容如下：
欧阳建国 5
创新办 5 i
欢聚时代 5
云计算 5
用户字典每行一个词，格式为：
词语 词频 词性
其中词频是一个数字，词性为自定义的词性，要注意的是词频数字和空格都要是半角的。
# 附：结巴分词词性对照表（按词性英文首字母排序）
### 形容词(1个一类，4个二类)
a 形容词
ad 副形词
an 名形词
ag 形容词性语素
al 形容词性惯用语
### 区别词(1个一类，2个二类)
b 区别词
bl 区别词性惯用语
### 连词(1个一类，1个二类)
c 连词
cc 并列连词
### 副词(1个一类)
d 副词
### 叹词(1个一类)
e 叹词
### 方位词(1个一类)
f 方位词
### 前缀(1个一类)
h 前缀
### 后缀(1个一类)
k 后缀
### 数词(1个一类，1个二类)
m 数词
mq 数量词
### 名词 (1个一类，7个二类，5个三类)
名词分为以下子类：
n 名词
nr 人名
nr1 汉语姓氏
nr2 汉语名字
nrj 日语人名
nrf 音译人名
ns 地名
nsf 音译地名
nt 机构团体名
nz 其它专名
nl 名词性惯用语
ng 名词性语素
### 拟声词(1个一类)
o 拟声词
### 介词(1个一类，2个二类)
p 介词
pba 介词“把”
pbei 介词“被”
### 量词(1个一类，2个二类)
q 量词
qv 动量词
qt 时量词
### 代词(1个一类，4个二类，6个三类)
r 代词
rr 人称代词
rz 指示代词
rzt 时间指示代词
rzs 处所指示代词
rzv 谓词性指示代词
ry 疑问代词
ryt 时间疑问代词
rys 处所疑问代词
ryv 谓词性疑问代词
rg 代词性语素
### 处所词(1个一类)
s 处所词
### 时间词(1个一类，1个二类)
t 时间词
tg 时间词性语素
### 助词(1个一类，15个二类)
u 助词
uzhe 着
ule 了 喽
uguo 过
ude1 的 底
ude2 地
ude3 得
usuo 所
udeng 等 等等 云云
uyy 一样 一般 似的 般
udh 的话
uls 来讲 来说 而言 说来
uzhi 之
ulian 连 （“连小学生都会”）
### 动词(1个一类，9个二类)
v 动词
vd 副动词
vn 名动词
vshi 动词“是”
vyou 动词“有”
vf 趋向动词
vx 形式动词
vi 不及物动词（内动词）
vl 动词性惯用语
vg 动词性语素
### 标点符号(1个一类，16个二类)
w 标点符号
wkz 左括号，全角：（ 〔 ［ ｛ 《 【 〖 〈 半角：( [ { <
wky 右括号，全角：） 〕 ］ ｝ 》 】 〗 〉 半角： ) ] { >
wyz 左引号，全角：“ ‘ 『
wyy 右引号，全角：” ’ 』
wj 句号，全角：。
ww 问号，全角：？ 半角：?
wt 叹号，全角：！ 半角：!
wd 逗号，全角：， 半角：,
wf 分号，全角：； 半角： ;
wn 顿号，全角：、
wm 冒号，全角：： 半角： :
ws 省略号，全角：…… …
wp 破折号，全角：—— －－ ——－ 半角：--- ----
wb 百分号千分号，全角：％ ‰ 半角：%
wh 单位符号，全角：￥ ＄ ￡ ° ℃ 半角：$
### 字符串(1个一类，2个二类)
x 字符串
xx 非语素字
xu 网址URL
### 语气词(1个一类)
y 语气词(delete yg)
### 状态词(1个一类)
z 状态词

