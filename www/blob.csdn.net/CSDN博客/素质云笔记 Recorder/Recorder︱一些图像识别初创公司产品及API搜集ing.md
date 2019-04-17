# Recorder︱一些图像识别初创公司产品及API搜集ing... - 素质云笔记/Recorder... - CSDN博客





2017年02月04日 17:05:49[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7544








> 
一些公司的产品路线可以很好地给我们启示，欢迎看客补充。

可参考：[从人脸识别到机器翻译：52个有用的机器学习和预测API](https://mp.weixin.qq.com/s?__biz=MzIwMjY2NDA5Mg==&mid=2247484096&idx=4&sn=c26199197e1a593d41ede3d29bd6abec&chksm=96da7333a1adfa2587e3f67d56677c759661047227341d3422abd2e8eb68ad9482b71d1cd359&mpshare=1&scene=1&srcid=0503dje2ETpyVdCmdoWmgaVz#rd)





### 文章目录
- [一、微软认知服务API](#API_11)
- [1、年龄、性别检测](#1_16)
- [2、物体分类、识别](#2_21)
- [3、识别名人](#3_26)
- [4、读取图片中的文字](#4_31)
- [5、情绪识别+视频图片情绪识别](#5_37)
- [6、人脸技术](#6_43)
- [7、视频中的图像技术](#7_48)
- [二、国内图像处理](#_59)
- [1、face++旷视（人脸识别）](#1face_62)
- [2、格灵深瞳（图像识别）](#2_67)
- [3、图普科技（图像识别）](#3_72)
- [4、Linkface 脸云科技（人脸识别）](#4Linkface__77)
- [5、腾讯优图（人脸识别）](#5_82)
- [6、SenseTime 商汤科技（图像识别）](#6SenseTime__93)
- [7、衣+（图像识别）](#7_98)
- [8、码隆科技（图像识别）](#8_103)
- [9、依图科技依图：与您一起构建计算机视觉的未来](#9_108)
- [10、tu Simple图森](#10tu_Simple_113)
- [11、飞搜](#11_118)
- [12、faceFinger 脸指](#12faceFinger__124)
- [13、颜鉴](#13_131)
- [14、Netposa 东方网力科技股份有限公司](#14Netposa__142)
- [15、网易易盾——广告图片识别](#15_149)
- [16、车伯乐拍照识车](#16_157)
- [17、百度研究院](#17_165)
- [18、荟萃-荟集人力之萃](#18_172)
- [19.深图智服](#19_197)
- [20、Linkfluence](#20Linkfluence_210)
- [21、易道博识](#21_217)
- [22 、我知图](#22__229)
- [23 慧科AI实验室  Wisers AI Lab](#23_AI__Wisers_AI_Lab_238)
- [24 ImageDT 图匠数据- 图片大数据的商业应用](#24_ImageDT___251)
- [25 朗镜科技 - AI品牌识别](#25___AI_262)
- [26 拍拍赚](#26__268)
- [27 trax - 货架识别新零售（国外）](#27_trax___275)
- [28 美图AI开放平台](#28_AI_285)
- [三、国外图像处理公司](#_298)
- [1、Enlitic 医疗诊断](#1Enlitic__301)
- [2、Affectiva 面部表情分析](#2Affectiva__310)
- [3、Deepomatic 计算机视觉](#3Deepomatic__316)
- [4、Clarifai 图像和视频识别](#4Clarifai__321)
- [5、Descartes Labs 图像识别、卫星与农业](#5Descartes_Labs__332)
- [6、MetaMind 自然语言处理和图像识别](#6MetaMind__337)
- [7、HyperVerge 计算机视觉与图像识别引擎](#7HyperVerge__347)
- [8、Tractable 计算机视觉](#8Tractable__352)
- [9、服装推荐Indico](#9Indico_360)
- [10、使用深度学习（Algorithmia API）为照片着色](#10Algorithmia_API_390)
- [11、AWS的人脸表情识别](#11AWS_399)
- [12、Hashley — 为你的照片自动生成诙谐的标签或评论](#12Hashley  _405)
- [13、Hotness.ai — 基于照片来分析你的性感程度](#13Hotnessai  _409)
- [14、Bitesnap — 基于照片的食物识别 AI，可用于控制热量摄入](#14Bitesnap  _AI_412)
- [15、Fify — 帮你买衣服](#15Fify  _417)
- [16、GoFind —用照片来为你在线搜衣服](#16GoFind _420)
- [17、Mode.ai — 帮你在线找衣服](#17Modeai  _423)
- [18、AI Weekly — 每周的人工智能与机器学习新闻、资源汇总](#18AI_Weekly  _427)
- [19、Approximately Correct —人工智能与机器学习博客](#19Approximately_Correct _430)
- [20、Axiomzen — AI 行业通讯半月刊](#20Axiomzen  AI__433)
- [21、Concerning.ai — AI 评论](#21Concerningai  AI__436)
- [22、Fast.ai —致力于普及深度学习](#22Fastai _439)
- [23、Machinelearning.ai — 专注于机器学习、人工智能新闻](#23Machinelearningai  _442)
- [24、Machine Learning Weekly — 一份手工的机器学习、深度学习周报](#24Machine_Learning_Weekly  _445)
- [25、PRAI —人工智能、机器学习、深度学习的一个论坛](#25PRAI _448)
- [26、Rekognition](#26Rekognition_451)
- [**参考文献：**](#_461)




# 一、微软认知服务API

![这里写图片描述](https://img-blog.csdn.net/20170204163832735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 1、年龄、性别检测

![这里写图片描述](https://img-blog.csdn.net/20170204164016735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 2、物体分类、识别

![这里写图片描述](https://img-blog.csdn.net/20170204164044145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3、识别名人

全新的名人识别模块可以识别20万来自全球各地涉及商界、政界、体育界以及娱乐界的名人。

## 4、读取图片中的文字

光学字符识别（OCR）可检测图片中的文字信息，并将提取出来的文字信息转化成为机器可读的字符串。通过分析图像来检测嵌入式文本，生成字符串并提供搜索功能。可以让直接拍摄文本的图片，无需手动转录文字，从而节省时间和精力。
![这里写图片描述](https://img-blog.csdn.net/20170204164154377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 5、情绪识别+视频图片情绪识别

可检测到的情感有愤怒、轻蔑、厌恶、恐惧、快乐、无表情、悲伤以及惊讶。这些情感具有特定的面部表情，被认为是可进行跨文化和普适交流的。
![这里写图片描述](https://img-blog.csdn.net/20170204164250658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 6、人脸技术

人脸识别、人脸关键点定位、人脸验证技术（核对两张脸是否属于同一个人。并给出置信度评分。 ）、人脸辨识、相似人脸搜索、人脸分组

## 7、视频中的图像技术

抖动的视频自动进行平滑和稳定处理、使用具有高精度的人脸位置检测和跟踪功能来分析视频，可在一个视频中最多检测 64 张人脸、测在静态背景的视频中发生运动的时间。此服务将分析检测到帧运动的输入视频并输出元数据，还可定义运动发生时所在的精确坐标、自动创建运动缩略图摘要，让人们快速预览你的视频

.

.
# 二、国内图像处理

## 1、face++旷视（人脸识别）

人脸识别、证件识别、图像识别

## 2、格灵深瞳（图像识别）

格灵深瞳是一家专注于计算机视觉以及人工智能的科技公司，投资人给这个团队的未来市场估值达几千亿。CTO赵勇是Google Glass团队核心成员之一。据说他们的成员都在来自海内外一级名校的学霸，要加入他们比进哈佛大学还要困难。2014年6月获得红杉资本的A轮投资。

## 3、图普科技（图像识别）

国内最大的图像识别云服务平台，每日处理数亿的图片及视频内容。创始人李明强是微信创始团队成员之一，曾带领团队打造出QQ邮箱。业界最专业的智能图片鉴黄师。2014年8月获A轮融资。

## 4、Linkface 脸云科技（人脸识别）

FDDB人脸检测公开测试世界第一，300-W Benchmark 准确率世界第一, LFW人脸识别准确率已达99.5%以上，即将推出中国银联和京东钱包的“人脸识别”。使用“高斯脸”算法首次击败了人眼识别率的陆超超也加入了Linkface。获得A轮融资，2015年底被SenseTime并购。

## 5、腾讯优图（人脸识别）

腾讯旗下专门研究机器学习、智能识别的团队，在另一项人脸识别测试LFW排名第一。相比与创业公司，腾讯优图有着得天独厚的大数据、研发投入、平台等多种优势。

腾讯AI实验室现在的接口变得更好了：

![这里写图片描述](https://img-blog.csdn.net/20180705173034303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

智能P图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214113802284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
## 6、SenseTime 商汤科技（图像识别）

在人脸识别测试LFW中准确率超过Facebook和Google。目前已开始对外提供精准的人脸识别技术，以及集成了人脸识别、危险品识别、行为检测、车辆检测等的安防监控系统。2014年11月获得IDG千万美元A轮融资。

## 7、衣+（图像识别）

衣+是计算机视觉搜索引擎，创造人和中意商品的连接。在ImageNet2015计算机视觉竞赛上获得五项世界第一 。团队成员来自于斯坦福、剑桥、IBM、Intel、阿里巴巴、百度等。创始人张默曾就职微软和IBM，任IBM开源联盟负责人、主机Linux中国区技术负责人，还做过平面模特。获1000万美元A轮融资。

## 8、码隆科技（图像识别）

主要产品StyleAI，一款通过服饰照片快速了解明星模特们或者其他用户如何搭配类似服饰的应用。团队成员来自微软、百度、腾讯等名企。入选微软创投加速器，已获得1200万元天使轮投资。

## 9、依图科技依图：与您一起构建计算机视觉的未来

大数据智慧平台

## 10、tu Simple图森

我们提供最佳的自动驾驶解决方案

## 11、飞搜

打造最好的在线人脸识别引擎，提供了人脸校验、人脸属性分析、目标场景识别、名人识别、色情识别、食品分析
[http://www.faceall.cn/](http://www.faceall.cn/)

## 12、faceFinger 脸指

一直致力于为社会各界提供以人脸、指掌纹等生物特征识别技术为核心的安全防范解决方案。
[http://www.facefinger.cn:8000/dashboard/demos/index](http://www.facefinger.cn:8000/dashboard/demos/index)

## 13、颜鉴

人脸检测、人脸比对
[http://colorreco.com/faceCompare](http://colorreco.com/faceCompare)

最精准的人脸识别、五官定位算法，不论是检出率、错检率、误检率、检测速度、支持旋转角度等标准均处于全球领先水平。
## 14、Netposa 东方网力科技股份有限公司

以视频解码、智能分析、智能检索、云计算等技术为核心，为公安用户提供一整套贴合业务流程的视频侦查装备和管理平台。

## 15、网易易盾——广告图片识别

[http://dun.163.com/trial/image/ad](http://dun.163.com/trial/image/ad)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114305652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

## 16、车伯乐拍照识车

依靠车辆语义分割和机器识别，分析比对图片信息与数据库信息，从而识别出照片中的车辆信息。目前车伯乐的拍照识车准确率已经超过96%。

## 17、百度研究院

[http://apistore.baidu.com/](http://apistore.baidu.com/)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114331787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

## 18、荟萃-荟集人力之萃

[http://huicui.me/?from=singlemessage&isappinstalled=0](http://huicui.me/?from=singlemessage&isappinstalled=0)

图片识别

可智能识别图片内容、属性、分类、是否涉黄等，支持单图多图多种形式。
语音转化

可替您将文字转成语音、文字转成方言（真人语音），识别语音、歌曲等。

视频识别

可以为您完成视频内容收集，字幕识别，视频内容鉴定等内容。

视频创作

为您拍摄或收集某一主题的视频，以小视频形式上传。

网页展示任务

可自定义任意网页在用户端展示时间，如新品推广、广告观看等类型。

自定义任务

抢票？秒杀？联系上下文？只要你脑洞够大，任意H5网页类任务皆可接入。

## 19.深图智服

[http://www.deepir.com/tech](http://www.deepir.com/tech)

智能鉴别色情、反动、暴力、恐怖、血腥、违规等图片与视频。

商标，人脸，检测数百种不同商标，电商以图搜图

语义分割，衣物，室外
更新了视频场景识别：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214113710261.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
## 20、Linkfluence

社交舆情，营销报告
[https://linkfluence.com/zh-hans/](https://linkfluence.com/zh-hans/)

.
## 21、易道博识

OCR是主打，人脸识别、图像识别
[http://www.exocr.com/coretech/TextReco.html#page7](http://www.exocr.com/coretech/TextReco.html#page7)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121411384863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214113900947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214113910263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214113944892.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

.
## 22 、我知图

[http://www.tudatech.com/](http://www.tudatech.com/)

主要还有视频内容 的解读。

![这里写图片描述](https://img-blog.csdn.net/20180801214823712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 23 慧科AI实验室  Wisers AI Lab

[https://www.wisers.ai/zh-cn/browse/logo-recognition/demo/](https://www.wisers.ai/zh-cn/browse/logo-recognition/demo/)

文本识别：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114422727.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
品牌识别：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114357142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

## 24 ImageDT 图匠数据- 图片大数据的商业应用

[https://www.imagedt.com/product/q](https://www.imagedt.com/product/q)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114512503.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114518937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114530203.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114549852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

## 25 朗镜科技 - AI品牌识别

[http://www.lenztechretail.com/Cn/Index/pageView/catid/4.html](http://www.lenztechretail.com/Cn/Index/pageView/catid/4.html)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114721354.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114709319.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
## 26 拍拍赚

[http://retailstore.ai/](http://retailstore.ai/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114821260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214114830828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

## 27 trax - 货架识别新零售（国外）

[https://traxretail.com/cn/technology/](https://traxretail.com/cn/technology/)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214115228597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214115300687.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214115337290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
## 28 美图AI开放平台

[https://ai.meitu.com/#/index](https://ai.meitu.com/#/index)

挺不错的

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120140531195.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019012014054744.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120140555332.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
# 三、国外图像处理公司

## 1、Enlitic 医疗诊断

Enlitic于2014年8月在三藩市成立，采用深度学习和图像分析帮助医生做出诊断并标记出医学图像中的异常，从而让医疗更加精确和更有效率。例如，Enlitic可以分析X光、核磁共振成像或CT扫描得到的医学图像，然后找出数据中的趋势或单个图像中的异常情况。

公司创始人Jeremy Howard（同时也是一名黑客和数据科学家）认为，数据科学是项非常性感的职业。但是，数据科学家现在做的大部分工作却是产品推荐或者广告投放等。他觉得这不够性感。为了给数据科学找到更好的应用，让深度学习做些更有意义的事情，他将注意力放在了医疗方面。

他的基本思考是，创造一个类似星际迷航三录仪一样的系统（不过可能无法便携），搜集有关特定病人的数据——从医疗图片、实验室检测结果到医生的便笺——让深度学习分析这些数据，做出诊断并给出治疗建议。这并非让机器替代医生，而是为其提供让诊断更加便利的工具。公司还将与诊所、医院以及其他医疗单位合作，分析算法，进一步精炼公司的技术。

## 2、Affectiva 面部表情分析

Affectiva的技术采用先进的计算机视觉算法来捕捉和识别视觉刺激所激发的情感反应。Affectiva的旗舰产品Affdex简单易用；只要一个摄像头，任何地方都可以，也不需要安装软件。另外Affdex也很简洁低调，没有凌乱的布线或电极。

## 3、Deepomatic 计算机视觉

Deepomatic正在打造一个能够将媒体图片中任何想要的产品与电子商务网站中同样或类似的产品链接起来的按钮。内容发行者可以将他们的图像经过算法的扫描，检测和识别出其中让人心动的产品（如：时尚产品）。通过将自动理解产品属性和图案颜色的比较相结合，Deepomatic将这些图像和电子商务网站上同样或类似的产品链接在一起。Deepomatic便由此可以和内容所有者分享这个新增的收入来源。

## 4、Clarifai 图像和视频识别

2013年，Clarifai的第一个图像识别系统在识别图像中的物体的ImageNet比赛中进入了前五强。自那以后，Clarifai的深度学习系统不断进化，提高了识别的速度、词典的大小和内存的占用，同时应用范围也超出了图像识别，可以从各种形式的数据中提取知识。Clarifai的技术中枢是高性能深度学习API，在这之上Clarifai正在构建新一代智能应用。这让Clarifai能够通过以全新的创新的方式向所有人提供高技术解决方案应对日常问题。

公司创始人Matthew Zeiler，纽约大学计算机科学PHD，曾和深度学习领域两个最牛人物一——Geoff Hinton和Yann LeCun一起工作过五年。他发现真正困难的地方是建立学习模式——处理所有视觉数据的关键算法——能够快速处理各种不同图片。Zeiler很清楚，「训练这些模型与其说是科学工作，不如说是种艺术」，「需要很多年的经验。」这正好是Clarifai的切入点。Zieler的想法是，只要用户将照片上传到Clarifai软件，这个软件就会分析出照片里的内容并提供与此类似的更多照片。与传统基于文本的图片搜索截然不同的是，这是真的以视觉为基础的搜索。

可以免费获得其API使用：

## 5、Descartes Labs 图像识别、卫星与农业

基于深度学习和先进的遥感算法，Descartes Labs正在教导计算机如何看世界以及世界如何随时间改变。他们的第一个应用是使用大量卫星图像（包括可见光段和不可见光段），更好了解全球农作物生产。

## 6、MetaMind 自然语言处理和图像识别

MetaMind想让人人都能使用深度学习。该公司正在打造一个用于自然语言处理、图像理解和知识基础的分析的人工智能平台。该公司提供了用于医疗成像、食物识别和解决方案定制的产品。

MetaMind希望提供包括自然语言处理在哪的更为广泛多样的工具。 深度学习有望帮助机器真正理解用户说的话，而这种技术的关键特点之一就是能够自我训练，这也是许多人相信它能有助于自然语言处理的根据所在。而这正是另一个MetaMind工具正在开发的领域，当你输入连个句子，这个工具能够告诉你两个句子的相似程度。这种技术能够被商家用来自动回复客户问题。Socher说，用户的询问方式多种多样，尽管绝大多数时候，意思差不都多。这个工具也能用来分析一些社交网络（比如推特）上用户对公司的评价。

MetaMind目前从事深度学习咨询的业务，也会提供自己的深度学习服务和软件。借由运行MetaMind的数以百计装备成千上万图像处理器的学习机器，这一在线服务省去了客户建立自己系统的麻烦和成本。但是，如果客户想要运营自己的深度学习系统，MetaMind会为它提供软件和专业服务，如果确有必要的话。

## 7、HyperVerge 计算机视觉与图像识别引擎

HyperVerge使用深度学习算法处理云中消费者的图片和视频。HyperVerge开发的用于图像处理的已获专利的专有图像技术模型包括：面部检测、面部识别、场景识别、差照片检测、重复照片检测、照片分类、相册总结、面部美化和照片美化。

## 8、Tractable 计算机视觉

Tractable正在开发专有的机器学习算法，重点是用于计算机视觉的深度学习。该公司的重点是让未标记的数据和监督学习一起工作。应用领域包括保险索赔、工业检测、远程监控等

## 9、服装推荐Indico

演示网址：[https://indico.io/demos/clothing-matching](https://indico.io/demos/clothing-matching)

推荐系统正在日益凸显它的价值。随着商品数目的增加，瞄准那些可能愿意购买某种产品的特定消费人群变得格外重要。在这一类应用的开发中，深度学习同样也能帮助到我们！

我不是个时尚爱好者，但是我知道人们会“浪费”很多时间在选择穿什么样的衣服上。如果我们能拥有一个知道我们的喜好并且能够向我们推荐完美的穿着的智能机器人该有多好！

幸运的是，在深度学习的帮助下，这成为了可能。

Indico 的官方文章（[https://indico.io/blog/fashion-matching-tutorial/）已经对它做了非常详细的描述。现在让我们弄清楚你要如何在你的终端上构建这个推荐系统。](https://indico.io/blog/fashion-matching-tutorial/%EF%BC%89%E5%B7%B2%E7%BB%8F%E5%AF%B9%E5%AE%83%E5%81%9A%E4%BA%86%E9%9D%9E%E5%B8%B8%E8%AF%A6%E7%BB%86%E7%9A%84%E6%8F%8F%E8%BF%B0%E3%80%82%E7%8E%B0%E5%9C%A8%E8%AE%A9%E6%88%91%E4%BB%AC%E5%BC%84%E6%B8%85%E6%A5%9A%E4%BD%A0%E8%A6%81%E5%A6%82%E4%BD%95%E5%9C%A8%E4%BD%A0%E7%9A%84%E7%BB%88%E7%AB%AF%E4%B8%8A%E6%9E%84%E5%BB%BA%E8%BF%99%E4%B8%AA%E6%8E%A8%E8%8D%90%E7%B3%BB%E7%BB%9F%E3%80%82)

9.1 服装推荐  Stitch Fix玩转时尚算法

来自文章：Stitch Fix玩转时尚算法，年销7.3亿 IT派

在进入正题前，我们需要了解Stitch Fix如何給客户服务。具体步骤如下：- 
第1步：顾客填写风格偏好问卷- 
第2步：选择订购周期:从两周一次到每季度一次不等- 
第3步：造型师挑款，收取造型费20元- 
第4步：顾客按时收到盒子（Fix），里面有5件衣服，和搭配方法- 
第5步：试穿，确定购买或退回。造型费可冲抵服装费用。

其中使用了非常多的算法：- 
评分排序：针对淘汰后留存的每一款服饰，机器会尝试评估这个客户喜欢这款商品的相关概率。经典的协同过滤问题：在基于之前所收集的不同客户对不同风格商品的回馈历史数据，他们填充一个稀疏矩阵- 
用户喜好问卷，可以选衣服，库存的每一款商品上提前计算出来向量与这些向量之间求它们的余弦相似度作为分数。- 
仓库取货路径问题，在确定好应该挑选的商品后，因为仓库比较大，还需要规划取货路径。这是一个经典的算法问题，是一个NP-hard旅行商问题，已有方法求得这类问题的次优解。10、使用深度学习（Algorithmia API）为照片着色

[](http://mp.weixin.qq.com/s?__biz=MjM5MTQzNzU2NA==&mid=2651648450&idx=2&sn=4252ecba83204f87c69fcc374ff1d82e&chksm=bd4dd6518a3a5f47393ff505af6a33f034c4cb1b70224971bbc4fb2055d3c101c6a7749644ab&mpshare=1&scene=1&srcid=0307lOH5spsiVkoKQ9TR8f6Z#rd)六招教你用Python构建好玩的深度学习应用

自动着色一直是计算机视觉社区中的热门话题。从一张黑白图片获得一张彩色照片似乎是件超现实的事。想象一个4岁的孩子拿着蜡笔全神贯注于涂鸦本的场景，我们是否能教会人工智能同样去做这件事情呢？11、AWS的人脸表情识别

[](https://aws.amazon.com/cn/rekognition/?nc2=h_a1)https://aws.amazon.com/cn/rekognition/?nc2=h_a1

检测图像中的对象、场景和面孔。您还可以搜索和比较面孔。借助 Rekognition 的 API，您可以快速为应用程序添加基于深度学习的复杂视觉搜索和图像分类功能。12、Hashley — 为你的照片自动生成诙谐的标签或评论

[](http://hash.ai/)http://hash.ai/
![这里写图片描述](https://img-blog.csdn.net/20170507210603416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)13、[](http://Hotness.ai)Hotness.ai — 基于照片来分析你的性感程度

[](http://hotness.ai/)http://hotness.ai/14、Bitesnap — 基于照片的食物识别 AI，可用于控制热量摄入

[](https://getbitesnap.com/)https://getbitesnap.com/

Entrupy — 自动识别衣服与配饰的真假
[](https://www.entrupy.com/)https://www.entrupy.com/15、Fify — 帮你买衣服

[](http://fify.ai/)http://fify.ai/16、GoFind —用照片来为你在线搜衣服

[](http://gofind.ai/)http://gofind.ai/17、[](http://Mode.ai)Mode.ai — 帮你在线找衣服

[](http://mode.ai/)http://mode.ai/18、AI Weekly — 每周的人工智能与机器学习新闻、资源汇总

[](http://aiweekly.co)http://aiweekly.co19、Approximately Correct —人工智能与机器学习博客

[](http://approximatelycorrect.com/)http://approximatelycorrect.com/20、Axiomzen — AI 行业通讯半月刊

[](http://ai.axiomzen.co/)http://ai.axiomzen.co/21、[](http://Concerning.ai)Concerning.ai — AI 评论

[](https://concerning.ai)https://concerning.ai22、[](http://Fast.ai)Fast.ai —致力于普及深度学习

[](http://www.fast.ai/)http://www.fast.ai/23、[](http://Machinelearning.ai)Machinelearning.ai — 专注于机器学习、人工智能新闻

[](http://machinelearning.ai)http://machinelearning.ai24、Machine Learning Weekly — 一份手工的机器学习、深度学习周报

[](http://mlweekly.com)http://mlweekly.com25、PRAI —人工智能、机器学习、深度学习的一个论坛

[](https://pr.ai/index.php)https://pr.ai/index.php26、Rekognition

为社交图片应用提供面部和场景的识别和优化。Rekognition API 可以利用眼睛、嘴、鼻子和面部的特征实现情绪识别和性别检测，可以用来确定性别、年龄和情绪。

链接：[](http://www.programmableweb.com/api/rekognition)http://www.programmableweb.com/api/rekognition参考文献：

1、机器之心：[](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=401970614&idx=1&sn=33be793f092a39b4f8481e956bb28bac&scene=1&srcid=0127Hs12Oyr5VifXlwUepHns#rd)业界｜最值得关注的18家深度学习创业公司

2、知乎，[](https://www.zhihu.com/question/30156262)国内有哪些人工智能领域的创业团队或创业公司？




















