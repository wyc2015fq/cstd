# 【中文分词】基于ICTCLAS的Python中文分词 - CD's Coding - CSDN博客





2015年07月20日 23:31:08[糖果天王](https://me.csdn.net/okcd00)阅读数：4842








## 前言

在ICT实习获得的第一个任务与中文分词有关，本想用Stanford中文分词器，苦于VirtualBox中的Ubuntu跑Stanford的中文分词程序的时候会出现OSError: Java Command Failed 的错误。负责OS这方面的队友在纠结不前之时，老师提醒了我：为什么不去试试别的分词器呢？

网上搜索了一下中文分词，一眼看到这个ICTCLAS，哈哈哈我正在这里实习为何不试试看呢？

CSDN上花了不少积分，PUDN上也花了不少积分，下载开发文档和看配置blog也花了不少时间……大家得出的结论似乎都是“难配”，“错误多”，“不知道什么问题”……

我也几乎把这些提及的问题都遇到了个遍……心灰意冷之时发现，官网的最新package下载好了，反正那么大呢，下载下来还是试试看吧

官网链接[传送门](http://ictclas.nlpir.org/downloads)来一个

里面的java包，C++包果然还是因为本人比较愚昧一个是一直报错一个是不知道怎么打开……

灵光一闪看到了Python示例，就试试吧，毕竟python是解释性语言，依葫芦画瓢试试看好了，随手敲了一些，上网找了个文件输入输出的例子照着做……然后，居然就成功了……




## 配置

文件树如下

2015/07/20  23:16    <DIR>          .

2015/07/20  23:16    <DIR>          ..

2015/07/20  19:19    <DIR>          Data

2015/07/20  20:31               511 infile.txt

2015/07/20  20:32    <DIR>          nlpir

2015/07/20  23:22             8,737 nlpir.py

2015/07/20  23:22               649 outfile.txt

2014/01/03  09:42               332 Readme.txt

2015/07/20  20:45    <DIR>          Support

2014/01/02  17:41                 0 __init__.py

               5 个文件         10,229 字节

               5 个目录  2,904,150,016 可用字节





[DIR>DATA]


2015/07/20  20:32    <DIR>          .

2015/07/20  20:32    <DIR>          ..

2014/12/25  14:58                92 20141225.err

2012/05/18  21:36           286,196 BIG2GBK.map

2012/05/18  21:33           468,456 BIG5.pdat

2012/05/18  21:33           158,695 BIG5.wordlist

2009/01/16  13:48         3,520,144 BiWord.big

2012/11/08  20:45            65,540 charset.type

2015/06/03  12:52             3,356 classifier.user

2015/06/03  12:53             3,356 cluster.user

2014/10/31  13:58             1,084 Configure.xml

2009/01/16  13:48         1,696,620 CoreDict.pdat

2009/01/16  13:48         1,786,424 CoreDict.pos

2009/01/16  13:48           478,168 CoreDict.unig

2015/06/03  12:53             3,356 DocExtractor.user

2015/07/20  20:32    <DIR>          English

2014/03/23  20:10           380,016 FieldDict.pdat

2014/03/23  20:10            27,257 FieldDict.pos

2014/03/23  14:55           546,720 FTU8.pdat

2014/03/23  14:55           190,687 FTU8.wordlist

2014/03/23  14:55           286,196 FTU82GBK.map

2012/05/18  21:34           549,204 GBK.pdat

2012/05/18  21:34           166,985 GBK.wordlist

2012/05/18  21:36           286,196 GBK2BIG.map

2014/03/23  14:55           286,196 GBK2FTU8.map

2012/05/18  21:37           286,196 GBK2GBKC.map

2012/05/18  21:37           286,196 GBK2UTF.map

2012/12/22  11:50           550,848 GBKA.pdat

2012/12/22  11:50           166,985 GBKA.wordlist

2012/12/22  11:50           286,196 GBKA2UTF.map

2012/05/18  21:34           550,848 GBKC.pdat

2012/05/18  21:34           166,985 GBKC.wordlist

2012/05/18  21:37           286,196 GBKC2GBK.map

2009/01/16  13:48         1,978,128 GranDict.pdat

2009/01/16  13:48         1,778,776 GranDict.pos

2014/01/03  10:39               422 ICTPOS.map

2015/06/03  12:52             3,356 keyExtract.user

2014/03/18  13:27             3,356 LJHtmlParser.user

2014/03/12  12:47            79,412 location.map

2014/03/12  12:46           416,748 location.pdat

2014/03/12  12:47           106,165 location.wordlist

2014/08/12  10:51             5,100 NewWord.lst

2009/01/16  13:48            37,253 NLPIR.ctx

2015/07/01  14:48             3,356 NLPIR.user

2009/01/16  13:48               288 NLPIR_First.map

2012/11/14  15:57             3,356 NLPIR_trial.user

2009/01/16  13:48             2,213 nr.ctx

2009/01/16  13:48             3,008 nr.fsa

2009/01/16  13:48         1,757,200 nr.role

2014/01/09  15:24               323 PKU.map

2014/01/09  15:24               300 PKU_First.map

2013/09/16  13:32           854,520 sentiment.pdat

2013/09/16  13:33            88,000 sentiment.ung

2015/06/03  12:53             3,356 summary.user

2014/08/12  10:30            33,620 UserDict.pdat

2014/03/23  14:02           286,196 UTF2GBK.map

2012/12/22  11:50           286,196 UTF2GBKA.map

2012/05/18  21:33           557,268 UTF8.pdat

2012/05/18  21:33           190,690 UTF8.wordlist

              56 个文件     22,250,000 字节

               3 个目录  2,904,150,016 可用字节





[DIR>NLPIR]

2015/07/20  20:32    <DIR>          .

2015/07/20  20:32    <DIR>          ..

2013/03/20  09:46         1,027,819 libNLPIR32.so

2013/12/20  13:32         2,014,184 libNLPIR64.so

2013/12/19  21:15            21,533 NLPIR.h

2013/12/19  20:51         1,684,992 NLPIR32.dll

2013/12/19  20:51            11,636 NLPIR32.lib

2014/01/01  21:07         2,301,952 NLPIR64.dll

2014/01/01  21:07            11,678 NLPIR64.lib

2013/11/08  16:48               156 Readme.txt

               8 个文件      7,073,950 字节

               2 个目录  2,904,150,016 可用字节





[DATA>SUPPORT]

2015/07/20  20:45    <DIR>          .

2015/07/20  20:45    <DIR>          ..

2015/07/17  16:11             2,737 Anti-Simply.cpp

2015/07/17  20:50               478 Count.cpp

2015/07/17  23:49               477 FileTest.cpp

2015/07/17  13:51               447 Multifile_Test.cpp

2015/07/20  20:44                87 Readme.md

2015/07/17  16:58             2,848 Simply.cpp

               6 个文件          7,074 字节

               2 个目录  2,904,150,016 可用字节





## 代码

Python修改后可运行代码如下



```python
# -*- coding: UTF-8 -*-
__author__ = 'Peter_Howe<haobibo@gmail.com>'

'''
Python Warpper for ICTCLAS2014
Loading functions from Dynamic Link Library  directly.
'''
from ctypes import *

#NLPIR2014 Lib File (NLPIR64, NLPIR32, libNLPIR64.so, libNLPIR32.so),
#Change this when you are not using a Win64 environment:
libFile = './nlpir/NLPIR64.dll'

dll =  CDLL(libFile)
def loadFun(exportName, restype, argtypes):
    global dll
    f = getattr(dll,exportName)
    f.restype = restype
    f.argtypes = argtypes
    return f

class ENCODING:
    GBK_CODE        =   0               #默认支持GBK编码
    UTF8_CODE       =   GBK_CODE+1      #UTF8编码
    BIG5_CODE       =   GBK_CODE+2      #BIG5编码
    GBK_FANTI_CODE  =   GBK_CODE+3      #GBK编码，里面包含繁体字

class POSMap:
    ICT_POS_MAP_SECOND  = 0 #计算所二级标注集
    ICT_POS_MAP_FIRST   = 1 #计算所一级标注集
    PKU_POS_MAP_SECOND  = 2 #北大二级标注集
    PKU_POS_MAP_FIRST   = 3	#北大一级标注集

POS = {
	"n": {  #1.	名词  (1个一类，7个二类，5个三类)
		"n":"名词",
		"nr":"人名",
		"nr1":"汉语姓氏",
		"nr2":"汉语名字",
		"nrj":"日语人名",
		"nrf":"音译人名",
		"ns":"地名",
		"nsf":"音译地名",
		"nt":"机构团体名",
		"nz":"其它专名",
		"nl":"名词性惯用语",
		"ng":"名词性语素"
	},
	"t": {  #2.	时间词(1个一类，1个二类)
		"t":"时间词",
		"tg":"时间词性语素"
	},
	"s": {  #3.	处所词(1个一类)
		"s":"处所词"
	},
	"f": {  #4.	方位词(1个一类)
		"f":"方位词"
	},
	"v": {  #5.	动词(1个一类，9个二类)
		"v":"动词",
		"vd":"副动词",
		"vn":"名动词",
		"vshi":"动词“是”",
		"vyou":"动词“有”",
		"vf":"趋向动词",
		"vx":"形式动词",
		"vi":"不及物动词（内动词）",
		"vl":"动词性惯用语",
		"vg":"动词性语素"
	},
	"a": {  #6.	形容词(1个一类，4个二类)
		"a":"形容词",
		"ad":"副形词",
		"an":"名形词",
		"ag":"形容词性语素",
		"al":"形容词性惯用语"
	},
	"b": {  #7.	区别词(1个一类，2个二类)
		"b":"区别词",
		"bl":"区别词性惯用语"
	},
	"z": {  #8.	状态词(1个一类)
		"z":"状态词"
	},
	"r": {  #9.	代词(1个一类，4个二类，6个三类)
		"r":"代词",
		"rr":"人称代词",
		"rz":"指示代词",
		"rzt":"时间指示代词",
		"rzs":"处所指示代词",
		"rzv":"谓词性指示代词",
		"ry":"疑问代词",
		"ryt":"时间疑问代词",
		"rys":"处所疑问代词",
		"ryv":"谓词性疑问代词",
		"rg":"代词性语素"
	},
	"m": {  #10.	数词(1个一类，1个二类)
		"m":"数词",
		"mq":"数量词"
	},
	"q": {  #11.	量词(1个一类，2个二类)
		"q":"量词",
		"qv":"动量词",
		"qt":"时量词"
	},
	"d": {  #12.	副词(1个一类)
		"d":"副词"
	},
	"p": {  #13.	介词(1个一类，2个二类)
		"p":"介词",
		"pba":"介词“把”",
		"pbei":"介词“被”"
	},
	"c": {  #14.	连词(1个一类，1个二类)
		"c":"连词",
		"cc":"并列连词"
	},
	"u": {  #15.	助词(1个一类，15个二类)
		"u":"助词",
		"uzhe":"着",
		"ule":"了 喽",
		"uguo":"过",
		"ude1":"的 底",
		"ude2":"地",
		"ude3":"得",
		"usuo":"所",
		"udeng":"等 等等 云云",
		"uyy":"一样 一般 似的 般",
		"udh":"的话",
		"uls":"来讲 来说 而言 说来",
		"uzhi":"之",
		"ulian":"连 " #（“连小学生都会”）
	},
	"e": {  #16.	叹词(1个一类)
		"e":"叹词"
	},
	"y": {  #17.	语气词(1个一类)
		"y":"语气词(delete yg)"
	},
	"o": {  #18.	拟声词(1个一类)
		"o":"拟声词"
	},
	"h": {  #19.	前缀(1个一类)
		"h":"前缀"
	},
	"k": {  #20.	后缀(1个一类)
		"k":"后缀"
	},
	"x": {  #21.	字符串(1个一类，2个二类)
		"x":"字符串",
		"xx":"非语素字",
		"xu":"网址URL"
	},
	"w":{   #22.	标点符号(1个一类，16个二类)
		"w":"标点符号",
		"wkz":"左括号", 	#（ 〔  ［  ｛  《 【  〖 〈   半角：( [ { <
		"wky":"右括号", 	#） 〕  ］ ｝ 》  】 〗 〉 半角： ) ] { >
		"wyz":"全角左引号", 	#“ ‘ 『
		"wyy":"全角右引号", 	#” ’ 』
		"wj":"全角句号",	#。
		"ww":"问号",	#全角：？ 半角：?
		"wt":"叹号",	#全角：！ 半角：!
		"wd":"逗号",	#全角：， 半角：,
		"wf":"分号",	#全角：； 半角： ;
		"wn":"顿号",	#全角：、
		"wm":"冒号",	#全角：： 半角： :
		"ws":"省略号",	#全角：……  …
		"wp":"破折号",	#全角：——   －－   ——－   半角：---  ----
		"wb":"百分号千分号",	#全角：％ ‰   半角：%
		"wh":"单位符号"	#全角：￥ ＄ ￡  °  ℃  半角：$
	}
}

class SegAtom(Structure):
    _fields_ = [("start", c_int32), ("length", c_int32),
        ("sPOS", c_char * 40),      ("iPOS", c_int32),
        ("word_ID", c_int32),       ("word_type", c_int32), ("weight", c_int32)
    ]

def translatePOS(sPOS):
    global POS
    if sPOS=='url': sPOS = 'xu'
    c = sPOS[0]
    return POS[c][sPOS]

Init = loadFun('NLPIR_Init',c_int, [c_char_p, c_int, c_char_p])
Exit = loadFun('NLPIR_Exit',c_bool, None)
ParagraphProcess = loadFun('NLPIR_ParagraphProcess',c_char_p, [c_char_p, c_int])
ParagraphProcessA = loadFun('NLPIR_ParagraphProcessA',POINTER(SegAtom), [c_char_p, c_void_p, c_bool])
#ParagraphProcessAW = loadFun('NLPIR_ParagraphProcessAW',None, [c_int, POINTER(SegAtom)])
FileProcess = loadFun('NLPIR_FileProcess',c_double, [c_char_p, c_char_p, c_int])
ImportUserDict = loadFun('NLPIR_ImportUserDict',c_uint, [c_char_p])
AddUserWord = loadFun('NLPIR_AddUserWord', c_int, [c_char_p])
SaveTheUsrDic = loadFun('NLPIR_SaveTheUsrDic', c_int, None)
DelUsrWord = loadFun('NLPIR_DelUsrWord',c_int, [c_char_p])
GetUniProb = loadFun('NLPIR_GetUniProb', c_double, [c_char_p])
IsWord = loadFun('NLPIR_IsWord',c_bool, [c_char_p])
GetKeyWords = loadFun('NLPIR_GetKeyWords',c_char_p, [c_char_p, c_int, c_bool])
GetFileKeyWords = loadFun('NLPIR_GetNewWords',c_char_p, [c_char_p, c_int, c_bool])
GetNewWords = loadFun('NLPIR_GetNewWords', c_char_p, [c_char_p, c_int, c_bool])
GetFileNewWords = loadFun('NLPIR_GetFileNewWords',c_char_p, [c_char_p, c_int, c_bool])
FingerPrint = loadFun('NLPIR_FingerPrint',c_ulong, [c_char_p])
SetPOSmap = loadFun('NLPIR_SetPOSmap',c_int, [c_int])
#New Word Identification
NWI_Start = loadFun('NLPIR_NWI_Start', c_bool, None)
NWI_AddFile = loadFun('NLPIR_NWI_AddFile',c_bool, [c_char_p])
NWI_AddMem = loadFun('NLPIR_NWI_AddMem',c_bool, [c_char_p])
NWI_Complete = loadFun('NLPIR_NWI_Complete', c_bool, None)
NWI_GetResult = loadFun('NLPIR_NWI_GetResult',c_char_p, [c_int])
NWI_Result2UserDict = loadFun('NLPIR_NWI_Result2UserDict',c_uint, None)

if not Init('',ENCODING.UTF8_CODE,''):
    print("Initialization failed!")
    exit(-111111)

'''
if not SetPOSmap(3): #POSMap.ICT_POS_MAP_SECOND
    print("Setting POS Map failed!")
    exit(-22222)
'''

def seg(paragraph):
    result = ParagraphProcess(paragraph, c_int(1))
    atoms = [i.strip().split('/') for i in result.split(' ') if len(i)>=1 and i[0]!=' ']
    atoms = [(a[0],a[1]) for a in atoms if len(a[0])>0]
    return atoms

def segment(paragraph):
    count = c_int32()
    result = ParagraphProcessA(paragraph, byref(count),c_bool(True))
    count = count.value
    atoms = cast(result, POINTER(SegAtom))
    return [atoms[i] for i in range(0,count)]

def Seg(paragraph):
    atoms = segment(paragraph)
    for a in atoms:
        if len(a.sPOS) < 1: continue
        i = paragraph[a.start: a.start + a.length]#.decode('utf-8')#.encode('ascii')
        yield (i, a.sPOS)

if __name__ == "__main__":
	#p = "Test Article: 中国科学院计算技术研究所在多年研究基础上，耗时一年研制出了基于多层隐码模型的汉语词法分析系统 ICTCLAS(Institute of Computing Technology, Chinese Lexical Analysis System)，该系统的功能有：中文分词；词性标注；未登录词识别。分词正确率高达97.58%(最近的973专家组评测结果)，基于角色标注的未登录词识别能取得高于90%召回率，其中中国人名的识别召回率接近98%，分词和词性标注处理速度为31.5KB/s。ICTCLAS 和计算所其他14项免费发布的成果被中外媒体广泛地报道，国内很多免费的中文分词模块都或多或少的参考过ICTCLAS的代码。"
	r = open('infile.txt','r')
	f = open('outfile.txt','w')
	for eachline in r:
		p = eachline
		for t in Seg(p):
			#s = '%s\t%s\t%s\n' % (t[0],t[1],translatePOS(t[1]))
			s = '%s\t' % (t[0])
			f.write(s)
		f.write('\n')
	f.close()
```





## 使用说明

需要改动的应该只有main函数了，被注释的部分中，t[0]是分词结果，t[1]是这个被分出来的词的词性，translatePOS(t[1])则是将其分离出来所用到的规则（理论依据）

示例：

infile.txt



```
中国科学院计算技术研究所在多年研究基础上
耗时一年研制出了基于多层隐码模型的汉语词法分析系统
ICTCLAS(Institute of Computing Technology, Chinese Lexical Analysis System)
该系统的功能有：中文分词；词性标注；未登录词识别。
分词正确率高达97.58%(最近的973专家组评测结果)，
基于角色标注的未登录词识别能取得高于90%召回率，
其中中国人名的识别召回率接近98%，
分词和词性标注处理速度为31.5KB/s。
ICTCLAS 和计算所其他14项免费发布的成果被中外媒体广泛地报道
```


outfile.txt





```
中	国	科学院	计算	技术	研	究	所在	多	年	研	究	基础	上	
耗时	一	年	研制	出	了	基于	多	层	隐	码	模型	的	汉语	词法	分析	系统	
ICTCLAS	(	Institute	of	Computing	Technology	,	Chinese	Lexical	Analysis	System	)	
该	系统	的	功能	有	：	中文	分词	；	词性	标注	；	未	登录	词	识别	。	
分词	正确	率	高	达	97.58%	(	最近	的	973	专家组	评测	结果	)	，	
基于	角色	标注	的	未	登录	词	识别	能	取得	高于	90%	召回	率	，	
其中	中	国	人名	的	识别	召回	率	接近	98%	，	
分词	和	词性	标注	处理	速度	为	31.5KB/s	。	
ICTCLAS	和	计算	所	其他	14	项	免费	发布	的	成果	被	中外	媒体	广泛	地	报道
```



After TranslatePOS(t[1])



```
中国科学院	nt	机构团体名
计算	v	动词
技术	n	名词
研究	vn	名动词
所	usuo	所
在	v	动词
多年	mq	数量词
研究	vn	名动词
基础	n	名词
上	f	方位词

耗时	vi	不及物动词（内动词）
一	m	数词
年	qt	时量词
研制	v	动词
出	vf	趋向动词
了	ule	了 喽
基于	p	介词
多	m	数词
层	qv	动量词
隐	v	动词
码	q	量词
模型	n	名词
的	ude1	的 底
汉语	nz	其它专名
词法	n	名词
分析	vn	名动词
系统	n	名词

ICTCLAS	x	字符串
(	wkz	左括号
Institute	x	字符串
of	x	字符串
Computing	x	字符串
Technology	x	字符串
,	wd	逗号
Chinese	x	字符串
Lexical	x	字符串
Analysis	x	字符串
System	x	字符串
)	wky	右括号

该	rz	指示代词
系统	n	名词
的	ude1	的 底
功能	n	名词
有	vyou	动词“有”
：	wp	破折号
中文	nz	其它专名
分词	v	动词
；	wf	分号
词性	n	名词
标注	v	动词
；	wf	分号
未	d	副词
登录	v	动词
词	n	名词
识别	vn	名动词
。	wj	全角句号

分词	v	动词
正确	ad	副形词
率	v	动词
高	ad	副形词
达	v	动词
97.58%	m	数词
(	wkz	左括号
最近	t	时间词
的	ude1	的 底
973	m	数词
专家组	n	名词
评测	vn	名动词
结果	n	名词
)	wky	右括号
，	wd	逗号

基于	p	介词
角色	n	名词
标注	v	动词
的	ude1	的 底
未	d	副词
登录	v	动词
词	n	名词
识别	v	动词
能	v	动词
取得	v	动词
高于	v	动词
90%	m	数词
召回	v	动词
率	v	动词
，	wd	逗号

其中	rz	指示代词
中国	ns	地名
人	n	名词
名	q	量词
的	ude1	的 底
识别	vn	名动词
召回	v	动词
率	k	后缀
接近	v	动词
98%	m	数词
，	wd	逗号

分词	v	动词
和	cc	并列连词
词性	n	名词
标注	v	动词
处理	v	动词
速度	n	名词
为	v	动词
31.5KB/s	url	网址URL
。	wj	全角句号

ICTCLAS	x	字符串
和	cc	并列连词
计算	vn	名动词
所	n	名词
其他	rzv	谓词性指示代词
14	m	数词
项	q	量词
免费	vd	副动词
发布	v	动词
的	ude1	的 底
成果	n	名词
被	pbei	介词“被”
中外	b	区别词
媒体	n	名词
广泛	a	形容词
地	ude2	地
报道	v	动词
```





## Source_Code

Python.Ver Demo

[Github](https://github.com/okcd00/Colorful_Projects/tree/master/Python_Projects/ICTCLAS_%E8%87%AA%E5%86%99%E4%B8%AD%E6%96%87%E5%88%86%E8%AF%8D%E5%99%A8), 向张华平博士致敬

Python.Ver Ofiicial Demo

[Github](https://github.com/tsroten/pynlpir). Tsroten/pynlpir

[Github](https://github.com/haobibo/ICTCLAS_Python_Wrapper). Haobio








