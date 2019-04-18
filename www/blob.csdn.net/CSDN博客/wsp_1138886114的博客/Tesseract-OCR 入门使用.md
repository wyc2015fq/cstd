# Tesseract-OCR 入门使用 - wsp_1138886114的博客 - CSDN博客





2018年11月14日 17:31:17[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：94
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)












### 文章目录
- [一、Tesseract-OCR 安装](#TesseractOCR__2)
- [二、测试](#_13)



以下只针对widows平台，linux下没有测试


### 一、Tesseract-OCR 安装

Tesseract-OCR遵循Apache 2.0 license开源协议。

下载地址：[https://digi.bib.uni-mannheim.de/tesseract/](https://digi.bib.uni-mannheim.de/tesseract/)

你也可以查看源码编译安装：[https://github.com/tesseract-ocr/tesseract/wiki/Downloads](https://github.com/tesseract-ocr/tesseract/wiki/Downloads)

或者非官方安装包：[https://github.com/UB-Mannheim/tesseract/wiki](https://github.com/UB-Mannheim/tesseract/wiki)
windows下安装一路next
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114165848843.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

这一步，我们需要选择添加语言 chinese simple

在进入安装目录，执行`.\tesseract`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114170320909.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
### 二、测试
- 
查看可用的 “语言” `-–list-langs` 执行：`tesseract --list-langs`

- 
执行 `tesseract D:\example_05.jpg D:\out` 默认使用英文识别，输出out.txt

- 
执行 `tesseract D:\example_05.jpg D:\out -l eng` 指定英文识别，输出out.txt

- 
执行 `tesseract D:\example_05.jpg D:\out -l eng pdf` 使用英文识别，输出out.pdf

- 
执行 `tesseract --print-parameters`  查看全部参数

- 
使用 -c 选项来设定单项参数的值:
`tesseract D:\example_05.jpg D:\out -l chi_sim -c language_model_ngram_on=1`

- 
使用多个 -c 选项来设置多个参数的值。

将多项参数设置写入文件，然后在识别时使用该文件:
`tesseract paper.png paper -l chi_sim tess.conf`- 
需要注意的是，如果配置文件有两个配置文件 tess_1.conf 和 tess_2.conf:
`tesseract paper.png paper -l chi_sim tess_1.conf tess_2.conf`

以上代码确实实现了输出：不过结果糟糕，可以试一下。![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114173035641.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
鸣谢
[https://blog.csdn.net/haluoluo211/article/details/53304900](https://blog.csdn.net/haluoluo211/article/details/53304900)









