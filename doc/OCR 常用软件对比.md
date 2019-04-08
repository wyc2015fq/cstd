# OCR 常用软件对比

2018年02月01日 10:21:41 [iamdll](https://me.csdn.net/iamdll) 阅读数：8121



国内主要OCR厂商产品的介绍

1、文通（http://www.wintone.com.cn/a/prods/netocr/）

北京文通科技有限公司是享誉国内外的OCR（光学字符识别）技术生产商、文档影像技术和应用解决方案提供商。经过多年的创新与发展，文通科技现已成长为国内知名的高新技术企业，在全国十余个城市建立了分支机构，公司产品涉及多个领域。

演示地址：http://cloud.wintone.com.cn/

 

2、百度（http://apistore.baidu.com/apiworks/servicedetail/969.html）

演示地址：http://apistore.baidu.com/astore/toolshttpproxy?apiId=usu9zg&isAworks=1

 

3、灵云（http://www.hcicloud.com/）

灵云作为第一个开放的人工智能云服务平台，将智能语音交互、图像识别、语义理解、机器翻译、生物特征识别等人工智能技术完美整合。用户可通过移动互 联网、互联网，使用任何设备，在任何时间、任何地点，随时随地享受灵云提供的“听、说、读、写、意……”等全方位的人工智能技术服务。目前，灵云以“云 + 端”的方式，通过灵云开发者社区为开发者提供语音合成（TTS）、语音识别（ASR）、手写识别（HWR）、光学字符识别（OCR）、机器翻译（MT）、 语义理解（NLU）、声纹识别（VPR）等多项人工智能核心技术能力服务。

演示地址：http://www.hcicloud.com/products/show/forward/ocr

 

4、微软

微软的牛津计划中的一部分，对中文支持不怎么好。

其中的人脸检测，人脸识别，语音识别，目标检测，场景理解，OCR（特别是英文）都做得很不错。

演示地址：https://www.projectoxford.ai/demo/visions#Ocr

 

5、云脉科技（http://www.yunmai.cn/）

厦门云脉技术有限公司（原厦门思根科技有限公司），我司成立于2002年6月，总部位于环境优美，宜人宜居的厦门，是厦门最大的一家手机软件研发公 司。云脉技术在手机拍照文字识别、名片识别、手机信息网络/PC同步及管理技术方面处于领先地位，与100多家手机厂商、电信运营商、PC外接设备制造 商、行业项目系统集成商及各大互联网公司建立了深厚的合作关系。公司凭借先进的技术实力与良好的客户关系，今后将以OCR识别引擎SDK合作、为各行业提 供移动办公信息化管理及移动电子商务解决方案作为公司下一阶段研发和运营的战略重点。

这家公司有手机版的车牌识别

演示地址：http://www.yunmai.cn/?viewnews-165.html

 

6、还有老毛子的公司ABBYYFineReader。号称OCR中的奔驰，现在不知道如何了，是否被其他公司超越了呢。

 

7、汉王（http://www.hw99.com/）

国内的老牌子了，以前和文通都是OCR的领头羊，现在很多新公司在慢慢的超越他们了。汉王也提供了云服务，名片，身份证，人脸都包含进去了

演示地址：http://developer.hanvon.com/card/toCard.do

 

这里有一个帖子，对国内外产品进行了讨论比较：

http://www.zhihu.com/question/19593313

真正能把中文OCR做得比较专业的，一共也没几家，国内2家，国外2家。国内是文通和汉王，国外是ABBYY和IRIS（台湾原来有2家丹青和蒙恬，这两年没什么动静了）。像大家提到的紫光OCR、CAJViewer、MS Office、清华OCR、包括慧视小灵鼠，这些都是文通的产品或者使用文通的识别引擎，尚书则是汉王的产品，和中晶扫描仪捆绑销售的。这两家的中文识别率都是非常不错的。而国外的2家，主要特点是西方语言的识别率很好，而且支持多种西欧语言，产品化程度也很高，不过中文方面速度和识别率还是有差距的，当然这两年人家也是在不断进步。Google的开源项目(Tesseract )，至少在中文方面，和这些家相比，各项性能指标水平差距还蛮大的呢



## 光学字符识别（OCR）软件比较

| **名称**                                                     | **支持平台**          | **编程语言**                               | **字体语言** | **输出格式**                                                 | **备注**                                                     |
| ------------------------------------------------------------ | --------------------- | ------------------------------------------ | ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Tesseract                                                    | Windows/MacOSX /Linux | C++, C                                     | 40+          | 文本,hOCR,其他                                               | 开源，支持中文                                               |
| ExperVisionTypeReader                                        | Windows/MacOSX /Linux | C/C++                                      | 21           | hOCR                                                         | 多次获奖；不支持中文                                         |
| [ABBYY FineReader](http://www.evget.com/zh-CN/product/1374/feature.aspx) | Windows/MacOSX /Linux | C/C++                                      | 198          | DOC, DOCX, XLS, XLSX, PPTX, RTF, PDF, HTML, CSV, TXT, ODT, DjVu, EPUB, FB2 | 世界排名第一；识别精度达99%；                                |
| [LEADTOOLS](http://www.evget.com/zh-CN/product/2413/feature.aspx) | Windows/MacOSX /Linux | C/C++, .NET, Objective-C, Java, JavaScript | 56           | PDF, PDF/A, DOC, DOCX, XLS, XPS, RTF, HTML, ANSI Text, Unicode Text, CSV | 支持拉丁语， 亚洲语言，阿拉伯语，MICR字符集；支持整页或者部分区域的OCR识别； |
| CuneiForm/OpenOCR                                            | Windows/MacOSX /Linux | C/C++                                      | 28           | **?**                                                        | 企业级系统，可以保存文本格式，并识别结构的复杂表格           |
| Image to OCR Converter                                       | Windows               | C/C++, VB , .NET                           | 40           | PDF, Word, HTML, Text                                        | 读取的图像格式和PDF文件，并可以从照相机扫描图像              |
| [Dynamsoft OCR SDK](http://www.evget.com/zh-CN/product/1313/feature.aspx) | Windows               | C/C++                                      | 40+          | PDF, TXT                                                     | -                                                            |
| Puma.NET                                                     | Windows               | C#                                         | 28           | **?**                                                        | -                                                            |

