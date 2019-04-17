# R+OCR︱借助tesseract包实现图片文本提取功能 - 素质云笔记/Recorder... - CSDN博客





2017年01月15日 10:39:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6158标签：[图像处理																[r语言																[图片](https://so.csdn.net/so/search/s.do?q=图片&t=blog)
个人分类：[图像︱相关技术跟踪与商业变现](https://blog.csdn.net/sinat_26917383/article/category/6818531)





> 
2016年11月，Jeroen Ooms在CRAN发布了tesseract包，实现了R语言对简单图片的文本提取、分析功能。 

  利用开源OCR引擎进行图片处理，目前可以识别超过100种语言，R语言可以借助tesseract调用OCR引擎进行相应操作。 

  从图像中提取文本时，需要提前安装训练数据（地址：[https://github.com/tesseract-ocr/tessdata](https://github.com/tesseract-ocr/tessdata)），系统默认为英语训练数据。在使用过程中，最好使用高对比度、低噪声、水平格式文本的图片。
转载于公众号R语言中文社区

# 一．核心函数介绍

```
ocr(image, engine = tesseract("eng"))
tesseract(language = NULL, datapath = NULL, options = NULL,cache = TRUE)
```

参数： 

image    图片文件路径，支持png、tiff、jpeg等格式 

engine   tesseract引擎，通过函数tesseract()来创建 

language 训练数据的语言字符简写，默认为英语（eng） 

datapath 训练数据的路径，模型为系统库 

options  tesseract引擎的相关参数，默认为NULL，可查看文档 

cache    可以使用训练数据的缓存版本，默认为TRUE
3.tesseract_download(lang, datapath = NULL, progress = TRUE) #下载训练数据 

4.tesseract_info() #查看训练数据路径、可使用数据的语言格式、当前版本

参数： 

lang 训练数据的语言格式简写，比如英语就是eng，可查看tessdata repository. 

datapath 训练数据下载路径地址 

progress  下载中，是否要输出下载进程，默认为输出
# 二．案例演示

## 1.环境配置

```
install.packages('tesseract')
library('tesseract')
setwd('e:/tess') # 设定工作路径
```

## 2.利用tesseract包提取英文文本

```bash
tesseract_info() #查看当前可用语言格式
```

![这里写图片描述](https://img-blog.csdn.net/20170115102919894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
text_1<-ocr('e:/tess/eng_1.jpg', engine = tesseract("eng"))
cat(text_1) #输出结果
```

![这里写图片描述](https://img-blog.csdn.net/20170115102952578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3. 利用tesseract包提取中文文本

```
tesseract_info() #先查看是否有中文训练数据，如果没有，需要下载安装
tesseract_download("chi_tra")
tesseract_download("chi_sim") #chi_sim和chi_tra均是中文训练数据
```

![这里写图片描述](https://img-blog.csdn.net/20170115103022114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170115103040005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
`text<-ocr('e:/tess/chi_1.jpg', engine = tesseract("chi_sim"))`
![这里写图片描述](https://img-blog.csdn.net/20170115103103688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 4. 批量提取图片文本内容

```
temp<-list.files(pattern='*.jpg')  #处理默认路径下jpg格式图片
text<-ocr(temp, engine = tesseract("chi_tra"))
cat(text)
```

注释：在使用图片批量处理方法时，需要图片格式一致！

# 三．文章小结

目前R软件通过tesseract包调用OCR引擎提取图片文本信息，对图片文本格式、噪声、对比度要求比较高，同时在多种语言（简体中文、英文等）混合时，提取准确度比较低，目前可以借助tesseract包实现简单图片的文本提取，同时结合jiebaR包、tm包进行文本分析与挖掘。 

Github：[https://github.com/tesseract-ocr](https://github.com/tesseract-ocr)

.

## 延伸一：实践过程中乱码较多
`[1] "L】 = ′,\n` /′ /` _ …翦\n\"\n酐 敬\n〖 》 ' ` …\n`/\n鳙 Z … 蓁L `\n仗 ′ `) 犍 E\n璧荸「翼 〈v胴 ′  量\n\" 麸 `  \n嵩支 ′′\n`=…荃′′熹立′\n_ 二鼻′婶 二: 冀三熹′ “ \"u\n~z(…毫屋喹墓莫重…了篡=`′′_-._′′ …` ′′_〉:、.” c, ) `z 『\n一.〈 脲 _ 『 .\n. _ 霆) 映忧愁郁渤瓜\nWe|bo.Com/u/6069986291\n\n"`
这是一张图的输出，乱码这么多？  这是R问题，还是算法的问题？ 

从笔者在python那边实践来看，还是一样的不好，需要自己训练自己词库才能更好的识别，所以这也只是简单的应用了，中文还是需要自己训练一些文件才能进行识别。](https://so.csdn.net/so/search/s.do?q=r语言&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)




