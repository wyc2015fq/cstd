# ASR（语音识别）总结大全

2019年02月26日 09:34:53 [春雨里de太阳](https://me.csdn.net/qq_16633405) 阅读数：348



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_16633405/article/details/87920034



### 文章目录

- - [1、使用Pocket Sphinx进行英文语音识别](https://blog.csdn.net/qq_16633405/article/details/87920034#1Pocket_Sphinx_3)
  - [2、PocketSphinx语音识别系统语言模型的训练和声学模型的改进](https://blog.csdn.net/qq_16633405/article/details/87920034#2PocketSphinx_7)
  - [3、PocketSphinx语音识别系统的编译、安装和使用](https://blog.csdn.net/qq_16633405/article/details/87920034#3PocketSphinx_11)
  - [4、FS之play_and_detect_speech模块](https://blog.csdn.net/qq_16633405/article/details/87920034#4FSplay_and_detect_speech_15)
  - [5、一些开源的语音识别软件](https://blog.csdn.net/qq_16633405/article/details/87920034#5_18)
  - [6、某大神写的一系列干货](https://blog.csdn.net/qq_16633405/article/details/87920034#6_22)
  - [7、语音识别——基于深度学习的中文语音识别系统框架](https://blog.csdn.net/qq_16633405/article/details/87920034#7_26)
  - [8、kaldi上第一个免费的中文语音识别例子](https://blog.csdn.net/qq_16633405/article/details/87920034#8kaldi_29)
  - [9、几个常见的语音交互平台的简介和比较](https://blog.csdn.net/qq_16633405/article/details/87920034#9_32)
  - [10、CMU Sphinx下载地址](https://blog.csdn.net/qq_16633405/article/details/87920034#10CMU_Sphinx_35)
  - [11、FreeSwitch LUA API中文版](https://blog.csdn.net/qq_16633405/article/details/87920034#11FreeSwitch_LUA_API_37)
  - [12、DeepVoice3: 百度多人语音合成实践](https://blog.csdn.net/qq_16633405/article/details/87920034#12DeepVoice3__40)
  - [13、语音识别jsgf学习笔记](https://blog.csdn.net/qq_16633405/article/details/87920034#13jsgf_43)
  - [14、某大神语音识别相关的系列文章](https://blog.csdn.net/qq_16633405/article/details/87920034#14_48)
  - [15、语音识别开源项目](https://blog.csdn.net/qq_16633405/article/details/87920034#15_52)
  - [16、语音开源代码与平台比较](https://blog.csdn.net/qq_16633405/article/details/87920034#16_55)
  - [17、Sphinx武林秘籍](https://blog.csdn.net/qq_16633405/article/details/87920034#17Sphinx_58)
  - [18、构建简单的智能客服系统系统文章](https://blog.csdn.net/qq_16633405/article/details/87920034#18_61)
  - [19、汉子拼音转化工具](https://blog.csdn.net/qq_16633405/article/details/87920034#19_64)
  - [20、PocketSphinx语音识别系统声学模型的训练与使用](https://blog.csdn.net/qq_16633405/article/details/87920034#20PocketSphinx_68)
  - [21、语音识别技术学习CMU Sphinx文档](https://blog.csdn.net/qq_16633405/article/details/87920034#21CMU_Sphinx_71)
  - [22、FreeSWITCH通过mod_unimrcp集成第三方服务实现ASR和TTS](https://blog.csdn.net/qq_16633405/article/details/87920034#22FreeSWITCHmod_unimrcpASRTTS_74)
  - [23、语音识别中的声学模型（AM）和语言模型(LM)](https://blog.csdn.net/qq_16633405/article/details/87920034#23AMLM_77)
  - [24、CMU SPHINX介绍](https://blog.csdn.net/qq_16633405/article/details/87920034#24CMU_SPHINX_80)

在做智能外呼的时候要做智能交互的功能，所以就了解了些ASR相关的功能，这里分享下留下的一些资料。根据FS权威指南中Freeswitch使用的第三方模块进行试验，但是效果很不理想，所以如果你想要一个准确率比较好的效果，就需要自己训练声音模型。



## 1、使用Pocket Sphinx进行英文语音识别

参考：<https://wsonh.com/article/60.html>

## 2、PocketSphinx语音识别系统语言模型的训练和声学模型的改进

参考：<https://www.cnblogs.com/qiuhong/articles/3671991.html>

## 3、PocketSphinx语音识别系统的编译、安装和使用

参考：<https://blog.csdn.net/qiaocuiyu/article/details/52093509>

## 4、FS之play_and_detect_speech模块

参考：<https://freeswitch.org/confluence/display/FREESWITCH/mod_dptools:+play_and_detect_speech>

## 5、一些开源的语音识别软件

参考：<http://www.eguidedog.net/>

## 6、某大神写的一系列干货

参考：<https://blog.csdn.net/u012637501/article/category/2662045>

## 7、语音识别——基于深度学习的中文语音识别系统框架

参考：<https://blog.csdn.net/chinatelecom08/article/details/82557715?utm_source=blogkpcl12>

## 8、kaldi上第一个免费的中文语音识别例子

参考：<https://blog.csdn.net/amds123/article/details/70313756>

## 9、几个常见的语音交互平台的简介和比较

参考：<https://blog.csdn.net/improve100/article/details/51006376>

## 10、CMU Sphinx下载地址

参考：<https://osdn.net/projects/sfnet_cmusphinx/>

## 11、FreeSwitch LUA API中文版

参考：<https://blog.csdn.net/jiajiren11/article/details/79269009>

## 12、DeepVoice3: 百度多人语音合成实践

参考：<https://blog.csdn.net/sparkexpert/article/details/80099724>

## 13、语音识别jsgf学习笔记

参考：<https://wenku.baidu.com/view/c6a80e04580102020740be1e650e52ea5518cec0.html>
<https://www.w3.org/TR/jsgf/>

## 14、某大神语音识别相关的系列文章

参考：<https://blog.csdn.net/zouxy09/article/category/1218766>

## 15、语音识别开源项目

参考：<https://blog.csdn.net/github_19776427/article/details/52530066>

## 16、语音开源代码与平台比较

参考：<https://www.jianshu.com/p/532745af477d>

## 17、Sphinx武林秘籍

参考：<http://www.cnblogs.com/huanghuang/archive/2011/07/14/2106579.html>

## 18、构建简单的智能客服系统系统文章

参考：<https://cotin.tech/>

## 19、汉子拼音转化工具

参考：<https://github.com/mozillazg/python-pinyin>

## 20、PocketSphinx语音识别系统声学模型的训练与使用

参考：<https://blog.csdn.net/zouxy09/article/details/7962382>

## 21、语音识别技术学习CMU Sphinx文档

参考：<https://max.book118.com/html/2017/0613/114567570.shtm>

## 22、FreeSWITCH通过mod_unimrcp集成第三方服务实现ASR和TTS

参考：<https://wsonh.com/article/62.html>

## 23、语音识别中的声学模型（AM）和语言模型(LM)

参考：<https://blog.csdn.net/zjm750617105/article/details/51785526>

## 24、CMU SPHINX介绍

参考：<https://blog.csdn.net/jojozhangju/article/details/18720887?utm_source=blogxgwz6>

Freeswitch总结请参考：
<https://blog.csdn.net/qq_16633405/article/details/87918031>
智能外呼系统相关资料请参考：
<https://blog.csdn.net/qq_16633405/article/details/87934824>