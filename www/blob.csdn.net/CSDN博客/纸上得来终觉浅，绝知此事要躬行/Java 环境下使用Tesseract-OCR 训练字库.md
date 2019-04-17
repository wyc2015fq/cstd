# Java 环境下使用Tesseract-OCR 训练字库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月01日 10:31:04[boonya](https://me.csdn.net/boonya)阅读数：415








从我测试的情况来看，使用jTessBoxEditor编辑box框识别文字来训练字体太过于繁琐了而且即使手动矫正了也未必能保证100%能识别正确。所以，还是需要继续深入研究之，暂时记录测试过程和相关参考文章！

`**Tesseract OCR Github:**[https://github.com/tesseract-ocr/tesseract/wiki/4.0-with-LSTM#400-alpha-for-windows](https://github.com/tesseract-ocr/tesseract/wiki/4.0-with-LSTM#400-alpha-for-windows)`

**jTessBoxEditor 工具：**[https://sourceforge.net/projects/vietocr/files/jTessBoxEditor/](https://sourceforge.net/projects/vietocr/files/jTessBoxEditor/)

工具将png等图片转成tif-----boonya.chinese.exp0.tif


**1、使用中文字库生成文字框makebox**
`tesseract boonya.chinese.exp0.png boonya.chinese.exp0 -l chi_sim batch.nochop makebox`

**2、生成训练字库**

新建 font_properties，写入 文件内容：chinese 0 0 0 0 0

执行以下命令：

```
tesseract  boonya.chinese.exp0.png boonya.chinese.exp0  nobatch box.train

unicharset_extractor boonya.chinese.exp0.box

shapeclustering -F font_properties -U unicharset boonya.chinese.exp0.tr

mftraining -F font_properties -U unicharset -O unicharset boonya.chinese.exp0.tr

cntraining boonya.chinese.exp0.tr
```

**3、合并和添加字库文件**

=============================执行完成后生成以下几个文件=============

unicharset、inttemp、pffmtable、shapetable、normproto 添加这几个文件的前缀为chinese

=============================合并生成的五个文件====================

`combine_tessdata chinese.`

最终得到：chinese.traineddata  将此文件复制到:C:\Program Files (x86)\Tesseract-OCR\tessdata

**4、测试训练字库**
`tesseract boonya.chinese.exp0.png result -l chinese`


**5、字库训练示例**

[https://blog.csdn.net/cylj102908/article/details/78818676](https://blog.csdn.net/cylj102908/article/details/78818676)

[https://blog.csdn.net/huobanjishijian/article/details/76212214](https://blog.csdn.net/huobanjishijian/article/details/76212214)

[https://blog.csdn.net/dcrmg/article/details/78233459?locationNum=7&fps=1](https://blog.csdn.net/dcrmg/article/details/78233459?locationNum=7&fps=1)

[https://www.cnblogs.com/wzben/p/5930538.html](https://www.cnblogs.com/wzben/p/5930538.html)

[https://www.cnblogs.com/cnlian/p/5765871.html](https://www.cnblogs.com/cnlian/p/5765871.html)

[https://baijiahao.baidu.com/s?id=1595469927762383585&wfr=spider&for=pc](https://baijiahao.baidu.com/s?id=1595469927762383585&wfr=spider&for=pc)

[https://blog.csdn.net/ProgramOfApe/article/details/78288622](https://blog.csdn.net/ProgramOfApe/article/details/78288622)

[https://blog.csdn.net/sdlypyzq/article/details/78583133](https://blog.csdn.net/sdlypyzq/article/details/78583133)

6、Java OCR相关项目

Tess4J：[https://sourceforge.net/projects/tess4j/](https://sourceforge.net/projects/tess4j/)

Java OCR（也适用于Android）：[https://sourceforge.net/projects/javaocr/](https://sourceforge.net/projects/javaocr/)



