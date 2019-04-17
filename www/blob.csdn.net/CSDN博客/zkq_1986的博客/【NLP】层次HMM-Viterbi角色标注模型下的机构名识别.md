# 【NLP】层次HMM-Viterbi角色标注模型下的机构名识别 - zkq_1986的博客 - CSDN博客





2017年12月08日 15:07:11[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：348








# [层次HMM-Viterbi角色标注模型下的机构名识别](http://www.hankcs.com/nlp/ner/place-name-recognition-model-of-the-stacked-hmm-viterbi-role-labeling.html)





## 原理


基本原理请参考《[实战HMM-Viterbi角色标注地名识别](http://www.hankcs.com/nlp/ner/place-names-to-identify-actual-hmm-viterbi-role-labeling.html)》，不再赘述。


与人名和地名识别稍有不同的是，在命名实体识别之前，需要先执行人名和地名识别，将粗分结果送入HMM模型求解，得出细分结果后才能进行，这是因为人名和地名也是机构名中的常见成分。这是与《[实战HMM-Viterbi角色标注地名识别](http://www.hankcs.com/nlp/ner/place-names-to-identify-actual-hmm-viterbi-role-labeling.html)》《[实战HMM-Viterbi角色标注中国人名识别](http://www.hankcs.com/nlp/chinese-name-recognition-in-actual-hmm-viterbi-role-labeling.html)》这两个姊妹篇最显著的不同。

## 训练

### 角色标注


参考《![](http://www.hankcs.com/wp-content/plugins/wp-ueditor/ueditor/dialogs/attachment/fileTypeImages/icon_doc.gif)[基于角色标注的中文机构名识别.doc](http://www.hankcs.com/wp-content/uploads/2014/11/%E5%9F%BA%E4%BA%8E%E8%A7%92%E8%89%B2%E6%A0%87%E6%B3%A8%E7%9A%84%E4%B8%AD%E6%96%87%E6%9C%BA%E6%9E%84%E5%90%8D%E8%AF%86%E5%88%AB.doc)》，采用如下角色——
|角色|意义|例子|
|----|----|----|
|A|上文|参与亚太经合组织的活动|
|B|下文|中央电视台报道|
|X|连接词|北京电视台和天津电视台|
|C|特征词的一般性前缀|北京电影学院|
|F|特征词的译名性前缀|美国摩托罗拉公司|
|G|特征词的地名性前缀|交通银行北京分行|
|H|特征词的机构名前缀|中共中央顾问委员会|
|I|特征词的特殊性前缀|中央电视台|
|J|特征词的简称性前缀|巴政府|
|D|机构名的特征词|国务院侨务办公室|
|Z|非机构名成份||

表格 1中文机构名称构成角色

这些角色远远不够，在HanLP中，还拓展了数词和方位词等等。

比如下列熟语料：

参与/v [北京/ns 电影/n 学院/nis]/nt 和/cc [美国/nsf 辛普森/nr 公司/nis]/nt 的/ude1 活动/vn ，/w 由/p [交通/n 银行/nis 北京/ns 分行/n]/nt 与/cc 麦当劳/nt 赞助/v ，/w [巴/b 政府/nis]/nt 和/cc [中共中央/nt 顾问/nnt 委员会/nis]/nt 指导/vn ，/w [中央/n 电视台/nis]/nt 报道/v

经过自动角色转换程序转化为：

原始语料 [参与/v, [北京/ns 电影/n 学院/nis]/nt, 和/cc, [美国/nsf 辛普森/nr 公司/nis]/nt, 的/ude1, 活动/vn, ，/w, 由/p, [交通/n 银行/nis 北京/ns 分行/n]/nt, 与/cc, 麦当劳/nt, 赞助/v, ，/w, [巴/b 政府/nis]/nt, 和/cc, [中共中央/nt 顾问/nnt 委员会/nis]/nt, 指导/vn, ，/w,
 [中央/n 电视台/nis]/nt, 报道/v]


添加首尾 [始##始/S, 参与/v, [北京/ns 电影/n 学院/nis]/nt, 和/cc, [美国/nsf 辛普森/nr 公司/nis]/nt, 的/ude1, 活动/vn, ，/w, 由/p, [交通/n 银行/nis 北京/ns 分行/n]/nt, 与/cc, 麦当劳/nt, 赞助/v, ，/w, [巴/b 政府/nis]/nt, 和/cc, [中共中央/nt 顾问/nnt 委员会/nis]/nt, 指导/vn, ，/w, [中央/n 电视台/nis]/nt, 报道/v, 末##末/Z]

标注上文 [始##始/S, 参与/A, [北京/ns 电影/n 学院/nis]/nt, 和/A, [美国/nsf 辛普森/nr 公司/nis]/nt, 的/ude1, 活动/vn, ，/w, 由/A, [交通/n 银行/nis 北京/ns 分行/n]/nt, 与/A, 麦当劳/nt, 赞助/v, ，/A, [巴/b 政府/nis]/nt, 和/A, [中共中央/nt 顾问/nnt 委员会/nis]/nt, 指导/vn, ，/A, [中央/n 电视台/nis]/nt, 报道/v, 末##末/Z]

标注下文 [始##始/S, 参与/A, [北京/ns 电影/n 学院/nis]/nt, 和/B, [美国/nsf 辛普森/nr 公司/nis]/nt, 的/B, 活动/vn, ，/w, 由/A, [交通/n 银行/nis 北京/ns 分行/n]/nt, 与/B, 麦当劳/nt, 赞助/B, ，/A, [巴/b 政府/nis]/nt, 和/B, [中共中央/nt 顾问/nnt 委员会/nis]/nt, 指导/B, ，/A, [中央/n 电视台/nis]/nt, 报道/B, 末##末/Z]

标注中间 [始##始/S, 参与/A, [北京/ns 电影/n 学院/nis]/nt, 和/X, [美国/nsf 辛普森/nr 公司/nis]/nt, 的/B, 活动/vn, ，/w, 由/A, [交通/n 银行/nis 北京/ns 分行/n]/nt, 与/X, 麦当劳/nt, 赞助/B, ，/A, [巴/b 政府/nis]/nt, 和/X, [中共中央/nt 顾问/nnt 委员会/nis]/nt, 指导/B, ，/A, [中央/n 电视台/nis]/nt, 报道/B, 末##末/Z]

处理整个 [始##始/S, 参与/A, 未##地/G, 电影/C, 学院/D, 和/X, 未##地/G, 未##人/F, 公司/D, 的/B, 活动/Z, ，/Z, 由/A, 交通/C, 银行/D, 未##地/G, 分行/D, 与/X, 麦当劳/K, 赞助/B, ，/A, 巴/J, 政府/D, 和/X, 未##团/K, 顾问/C, 委员会/D, 指导/B, ，/A, 中央/C, 电视台/D, 报道/B, 末##末/Z]





### 统计词频


在对所有熟语料句子执行自动标注后，即可统计每一个非Z词语的各角色词频，得到一个角色词典：


公司 D 4621 A 24 B 24

公司总部 B 1

公司治理 B 2

公司股票 B 1

公告 B 15 X 1

公告栏 B 2

公园 C 9

公安 C 728 A 19 B 5

公安分局 A 18

公安厅 D 226 A 4

公安处 D 183

公安学 B 4

公安局 D 2251 A 5

公安机关 B 10

……


统计转移矩阵


转移矩阵指的是从一个角色标签转移到另一个角色的频次，利用它和角色词频可以计算出HMM中的初始概率、转移概率、发射概率，进而完成求解。关于维特比算法和实现请参考《[通用维特比算法的Java实现](http://www.hankcs.com/nlp/general-java-implementation-of-the-viterbi-algorithm.html)》。


这里对人民日报2014切分语料训练出如下转移矩阵：

![](http://ww4.sinaimg.cn/large/6cbb8645jw1emfgxm47yoj210p0c9q6v.jpg)

## 识别


例子


以“济南杨铭宇餐饮管理有限公司是由杨先生创办的餐饮企业”为例，不进行机构名识别时，会得出下列输出：


[济南/ns, 杨铭宇/nr, 餐饮/n, 管理/vn, 有限公司/nis, 是/vshi, 由/p, 杨先生/nr, 创办/v, 的/ude1, 餐饮/n, 企业/n]



上例中该公司的各个成分被拆散，无法组成完整的机构名

### 角色标注

地名角色观察：[  S 1162194 ][济南 G 83472 B 1200 A 470 D 84 X 4 ][杨铭宇 F 4309 B 769 A 266 D 254 X 6 ][餐饮 C 58 B 12 ][管理 C 706 B 70 A 5 ][有限公司 D 2861 A 1 B 1 ][是 A 2340 B 353 X 20 P 2 ][由 A 1579 B 16 X 11 ][杨先生 F 4309 B 769 A 266 D 254 X 6 ][创办 A 20 B 5 ][的 B 7092
 A 4185 X 20 ][餐饮 C 58 B 12 ][企业 C 86 A 42 B 11 X 6 ][  B 710 ]

地名角色标注：[ /S ,济南/G ,杨铭宇/F ,餐饮/C ,管理/C ,有限公司/D ,是/B ,由/A ,杨先生/F ,创办/A ,的/B ,餐饮/C ,企业/C , /B]


### 模式匹配


利用[Aho-Corasick算法](http://www.hankcs.com/program/algorithm/implementation-and-analysis-of-aho-corasick-algorithm-in-java.html)模式匹配如下模式串：


CCCD

PPD

PPDCD

PPFCCD

PPFCD

……



事实上，全部的模式串非常多：![](http://www.hankcs.com/wp-content/plugins/wp-ueditor/ueditor/dialogs/attachment/fileTypeImages/icon_txt.gif)[nt.pattern.txt](http://www.hankcs.com/wp-content/uploads/2014/11/nt.pattern.txt)


得到如下机构名：


识别出机构名：济南杨铭宇餐饮管理有限公司 GFCCD


![](http://ww1.sinaimg.cn/large/6cbb8645jw1emfhsy7py5j20js05qaas.jpg)

### 第二层隐马模型细分


其实这应该算是第四层隐马模型，粗分用到一次，因为地名识别中也用到了一次HMM，细分又用到一次，细分的输出是这次的输入。这次细分之后得出最终的结果：


[济南杨铭宇餐饮管理有限公司/nt, 是/vshi, 由/p, 杨先生/nr, 创办/v, 的/ude1, 餐饮/n, 企业/n]


## 总结


多层HMM模型可以在消耗性能的前提下提高精准度，识别命名实体。


目前人民日报语料中有不少错误，并且主要集中在政府机构，为了补充公司等机构，我找到了一个公司名词典，切分后得到一个伪熟语料库：


[北京/ns 华安/ns 通达/v 科技/n 有限公司/nis]/ntc

[北京/ns 京文/nz 联发/nz 教育/vn 文化/n 有限公司/nis]/ntc

[北京东方/nz 商机/n 喷码/nz 技术/n 有限公司/nis]/ntc

[北京/ns 海信/nz 箱包/n 发展/vn 有限公司/nis]/ntc

[北京/ns 恒正/nr 金建/ns 仪器/n 有限公司/nis]/ntc

[北京/ns 海腾威/nr 商贸/n 中心/nis]/ntc

……



将前100条发布出来作为示例：![](http://www.hankcs.com/wp-content/plugins/wp-ueditor/ueditor/dialogs/attachment/fileTypeImages/icon_txt.gif)[公司名切分语料.txt](http://www.hankcs.com/wp-content/uploads/2014/11/%E5%85%AC%E5%8F%B8%E5%90%8D%E5%88%87%E5%88%86%E8%AF%AD%E6%96%99.txt)


然后执行上述自动标注，补充到此模型词典中，使得HanLP能够同时支持政商二用。





转载自：http://www.hankcs.com/nlp/ner/place-name-recognition-model-of-the-stacked-hmm-viterbi-role-labeling.html



