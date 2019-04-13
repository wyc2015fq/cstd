
# 哈工大LTP项目 - forever1dreamsxx--NLP - CSDN博客


2012年11月30日 11:28:22[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：4762


# 来自哈工大官网：
# [http://ir.hit.edu.cn/ltp/](http://ir.hit.edu.cn/ltp/)
# Ltp: Language Technology Platform (
# [下载源代码](http://ir.hit.edu.cn/ltp/#download)
# )
## Introduction
**语言技术平台（Language Technology Platform，LTP）**是**[哈工大社会计算与信息检索研究中心](http://ir.hit.edu.cn/)**历时十年开发的一整套中文语言处理系统。LTP制定了基于XML的语言处理结果表示，并在此基础上提供了一整套自底向上的丰富而且高效的中文语言处理模块（包括词法、句法、语义等6项中文处理核心技术），以及基于动态链接库（Dynamic Link Library,
 DLL）的应用程序接口，可视化工具，并且能够以网络服务（[Web Service](http://ir.hit.edu.cn/ltp/#links)）的形式进行使用。
从2006年9月5日开始该平台对外免费共享目标代码，截止目前，已经有国内外400多家研究单位共享了LTP，也有国内外多家商业公司购买了LTP，用于实际的商业项目中。2010年12月获得中国中文信息学会颁发的行业最高奖项：**“钱伟长中文信息处理科学技术奖”一等奖**。
2011年6月1日，为了与业界同行共同研究和开发中文信息处理核心技术，我中心正式将LTP开源。
## Table of contents
[系统框架](http://ir.hit.edu.cn/ltp/#structure)
[功能模块](http://ir.hit.edu.cn/ltp/#features)[分词(CRFWordSeg)](http://ir.hit.edu.cn/ltp/#crfwordseg)
[词性标注(PosTag)](http://ir.hit.edu.cn/ltp/#postag)
[命名实体识别(NER)](http://ir.hit.edu.cn/ltp/#ner)
[依存句法分析(GParser)](http://ir.hit.edu.cn/ltp/#gparser)
[词义消歧(wsd)](http://ir.hit.edu.cn/ltp/#wsd)
[语义角色标注(SRL)](http://ir.hit.edu.cn/ltp/#srl)

[下载源代码](http://ir.hit.edu.cn/ltp/#download)
[在线演示](http://ir.hit.edu.cn/ltp/#demo)
[安装](http://ir.hit.edu.cn/ltp/#install)[@Windows](http://ir.hit.edu.cn/ltp/#windows)
[@Linux](http://ir.hit.edu.cn/ltp/#Linux)
[使用说明](http://ir.hit.edu.cn/ltp/#usage)
[未来工作](http://ir.hit.edu.cn/ltp/#todo)
[相关链接](http://ir.hit.edu.cn/ltp/#links)
[更新历史](http://ir.hit.edu.cn/ltp/#news)
## 系统框架
LTP系统框架图如下所示：
![ltp](http://ir.hit.edu.cn/ltp/image/structure.png)
## 功能模块
### 分词(CRFWordSeg)
模型简介LTP的分词模块基于CRF模型进行训练和解码。CRF（Conditional Random Field，条件随机场）是Lafferty在2001年提出的一种典型的判别式模型。它在观测序列的基础上对目标序列进行建模,重点解决序列化标注的问题。
使用开源工具：CRF++([http://crfpp.sourceforge.net/](http://crfpp.sourceforge.net/))
使用资源数据来源：1998年1-6月人民日报
数据规模：1~6月数据作为训练集
性能及效率性能：F1=97.4%
效率：185KB/s

### 词性标注(PosTag)
模型简介词性标注模块基于SVM模型进行训练和解码。SVM（Support Vector Machine，支持向量机）是Cortes和Vapnik于1995年首先提出的，它在解决小样本、非线性及高维模式识别中表现出许多特有的优势，并能够推广应用到函数拟合等其他机器学习问题中。
使用开源工具：svmtools([http://www.lsi.upc.edu/~nlp/SVMTool/](http://www.lsi.upc.edu/~nlp/SVMTool/))
使用资源数据来源：1998年1-6月人民日报
数据规模：2-6月份数据进行训练（6百万词）、1月份数据进行测试
性能及效率性能：整体准确率为97.4%，未登录词准确率为85.48%
效率：43KB/s

### 命名实体识别(NER)
模型简介NER（Named Entity Recognition, 命名实体识别）把命名实体识别的过程看作一个多分类的问题，使用最大熵模型来完成这一任务。
使用开源工具：[Maximum Entropy Modeling Toolkit for Python and C++](http://homepages.inf.ed.ac.uk/lzhang10/maxent_toolkit.html)
使用资源数据来源：1998年1月份人民日报+6月份人民日报前10000句
数据规模：1月份数据进行训练、6月份前10000句测试
性能及效率性能：F1(overall)=92.25%
效率：11KB/s

### 依存句法分析(GParser)
模型简介GParser全称为Graph-Based Parser，即基于图模型的依存句法分析器。
基于图的依存分析方法由McDonald首先提出，他将依存分析问题归结为在一个有向图中寻找最大生成树（Maximum Spanning Tree）的问题。边权重使用Online Learning算法学习获得，解码算法使用的是Eisner算法。
使用资源数据来源：HIT-CDT
数据规模：前8000句训练，8001-9000开发，后1000句测试
性能及效率基于正确分词、自动词性标注(svmtool)
性能：LAS=73.91%; UAS=78.23%
效率：0.20KB/s

### 词义消歧(wsd)

### 语义角色标注(SRL)
模型简介SRL（Semantic Role Labeling, 语义角色标注）是一种浅层的语义分析技术，它标注句子中某些短语为给定谓词的论元（语义角色）。SRL可以分为两个子任务，一是识别，即判断一个短语是否语义角色；其次是分类，即判断该短语属于哪一种语义角色。本模块将这两个任务合并，看成一个多元分类问题（非语义角色也属于一类，NULL）。同时，系统增加了一个后处理过程。
使用开源工具：[Maximum Entropy Modeling Toolkit for Python and C++](http://homepages.inf.ed.ac.uk/lzhang10/maxent_toolkit.html)
使用资源数据来源：Chinese PropBank
数据规模：22000句训练， 1700句开发，2500句测试
性能及效率基于正确分词、自动词性标注、自动依存句法
性能：F-Score = 77.15%
效率：0.18KB/s

## 下载源代码
请先阅读[LTP开源协议](http://ir.hit.edu.cn/ltp/agreement.html)。
## 在线演示
[语言技术平台的在线演示](http://ir.hit.edu.cn/demo/ltp)
## 编译及安装
LTP可运行于Windows以及Linux平台(已在Ubuntu, CentOS, Redhat平台下编译通过)
### Windows
安装Visual Studio(建议2008)。
运行windows\win_project\vs2008\__ltp.sln打开工程，使用VS进行编译即可。
解压模型文件ltp_data.zip。
依赖boost的regex库(建议使用较旧版本的boost，如1.34.1, 1.37.1)
### Linux
Requirements:gcc/g++(建议使用4.1)
gfortran
boost的regex库(建议使用较旧版本的boost，如1.34.1, 1.37.1)
编译$ ./configure\#也可以使用--prefix选项指定安装路径$ make
$ su
\# make install
## 使用说明
LTP的使用以及详细API请参考LTP开源工具包中所附的[《LTP使用文档v2.0.1》](http://ir.hit.edu.cn/ltp/files/LTP-manual-v2.0.1.pdf)
执行LTP时，需要保证输入的文件编码为gbk/gb2312;
ltp_data.zip中存放的是所有模块的模型文件。
完整运行LTP需要机器配置较高（主要是内存消耗比较大），如果不具备充分的条件，建议使用[Web Service](http://code.google.com/p/ltp-service/)访问LTP服务。
## 发表论文
Wanxiang Che, Zhenghua Li, Ting Liu.[
LTP: A Chinese Language Technology Platform](http://www.aclweb.org/anthology/C/C10/C10-3.pdf#page=16). In Proceedings of the Coling 2010:Demonstrations. 2010.08, pp13-16, Beijing, China.
Che. Wanxiang, Zhenghua Li, Yongqiang Li, Yuhang Guo, Bing Qin, Ting Liu. 2009.[
Multilingual dependency-based syntactic and semantic parsing](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.165.1686&rep=rep1&type=pdf#page=61). In CoNLL 2009, pages 49-54, Boulder, Colorado, June.
Guo, Yuhang, Wanxiang Che, Yuxuan Hu, Wei Zhang, and Ting Liu. 2007.[
Hit-ir-wsd: A wsd system for english lexical sample task](http://acl.ldc.upenn.edu/W/W07/W07-2034.pdf). In SemEval-2007, pages 165–168.
Liu, Ting, Jinshan Ma, and Sheng Li. 2006.[
Building a dependency treebank for improving Chinese parser](http://ir.hit.edu.cn/phpwebsite/index.php?module=documents&JAS_DocumentManager_op=downloadFile&JAS_File_id=255#page=43). Journal of Chinese Language and Computing, 16(4):207–224.
Lijie Wang, Wanxiang Che, and Ting Liu. 2009. An SVMTool-based Chinese POS Tagger. Journal of Chinese Information Processing, 23(4):16–22.
## 未来工作
WSD(Word Sence Disambiguation, 词义消歧)模块开源
分词模块升级，将支持领域词典扩展
依存句法分析模块升级，效率将提升一倍左右
## 相关链接
[LTP Online Demo](http://ir.hit.edu.cn/demo/ltp)
[LTP Web Service](http://code.google.com/p/ltp-service/)
[HIT-SCIR](http://ir.hit.edu.cn/)
[新][LTP新浪微群](http://q.weibo.com/849045)(欢迎大家加入讨论)
## 更新历史
**01/06/2011**LTP正式开源!
**04/01/2010**ltp-service 1.0 发布
**19/06/2009**发布[HIT-CIR_LTP_Sharing_Package_Full_v2.0.1](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT-CIR_LTP_Sharing_Package_Full_v2.0.1.zip)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。在此版本中，添加了LTP程序包的使用文档，请看[LTP使用文档v2.0.1](http://ir.hit.edu.cn/demo/ltp/LTP-manual-v2.0.1.pdf)。
**19/06/2009**发布升级包[Update_v2.0.1_20090619.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v2.0.1_20090619.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。
**01/01/2009**发布[HIT-CIR_LTP_Sharing_Package_Full_v2.0](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v2.0.zip)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。在此版本中，添加了LTP程序包的使用文档，请看[LTP使用文档v2.0](http://ir.hit.edu.cn/demo/ltp/LTP%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3v2.0.pdf)。
**24/04/2008**发布升级包[Update_v1.4.3_20080424.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.4.3_20080424.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。
**11/01/2008**发布升级包[Update_v1.4.2_20080111.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.4.2_20080111.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。
**08/12/2007**发布升级包[Update_v1.4.1_20071208.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.4.1_20071208.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。
**03/12/2007**发布升级包[Update_v1.4.1_20071203.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.4.1_20071203.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。
**01/12/2007**发布[HIT-CIR_LTP_Sharing_Package_Full_v1.5.0](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.5.0.zip)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。在此版本中，添加了LTP程序包的使用文档，请看[LTP使用文档v1.5](http://ir.hit.edu.cn/demo/ltp/LTP%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3v1.5.pdf)。
**01/12/2007**发布升级包[Update_v1.4.0_20071201.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.4.0_20071201.rar)，同时发布[HIT-CIR_LTP_Sharing_Package_Full_v1.4.0.rar](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.4.0.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。在此版本中，添加了LTP程序包的使用文档，请看[LTP使用文档v1.4](http://ir.hit.edu.cn/demo/ltp/LTP%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3v1.4.pdf)。
**07/10/2007**发布升级包[Update_v1.3.3_20071007.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.3.3_20071007.rar)，同时发布[HIT-CIR_LTP_Sharing_Package_Full_v1.3.3.rar](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.3.3.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Updates.txt)。在此版本中，添加了LTP程序包的使用文档，请看[LTP使用文档v1.3](http://ir.hit.edu.cn/demo/ltp/LTP%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3v1.3.pdf)。
**08/05/2007**发布升级包[Update_v1.3.2_20070508.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.3.2_20070508.rar)，同时发布[HIT-CIR_LTP_Sharing_Package_Full_v1.3.2.rar](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.3.2.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.3.2_Note.txt)
**08/05/2007**发布升级包[Update_v1.2.1_20070508.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.2.1_20070508.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.2.1_Note.txt)
**14/04/2007**发布升级包[Update_v1.3.1_20070430.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.3.1_20070430.rar)，同时发布[HIT-CIR_LTP_Sharing_Package_Full_v1.3.1.rar](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.3.1.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.3.1_Note.txt)
**11/01/2007**发布升级包[Update_v1.2_20070111.rar](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.2_20070111.rar)，同时发布[HIT-CIR_LTP_Sharing_Package_Full_v1.2.rar](http://ir.hit.edu.cn/demo/ltp/SharingPackage/HIT_IR_Lab_LTP_Sharing_Package_Full_v1.2.rar)，[升级说明](http://ir.hit.edu.cn/demo/ltp/Update/Update_v1.2_Note.txt)
**05/09/2006**发布HIT_CIR_LTP_Sharing_Package_Full_v1.rar
Copyright © Research Center for Social Computing and Information Retrieval 2001 - 2011
P.O.Box 321, HIT, P.R.China 150001
webmaster at ir dot hit dot edu dot cn

