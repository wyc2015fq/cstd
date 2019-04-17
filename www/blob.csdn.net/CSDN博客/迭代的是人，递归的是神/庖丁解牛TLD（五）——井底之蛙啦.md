# 庖丁解牛TLD（五）——井底之蛙啦~ - 迭代的是人，递归的是神 - CSDN博客





2011年12月21日 15:40:51[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：41558








随着和我交流TLD的朋友越来越多，我渐渐的知道的也多了，才发现我研究的结果只是沧海一粟。

这里先膜拜一下Alan Torres大神，他已经用c++把TLD重新写好了，而且代码很规范。他设计的理念有：

1. depends *only* on OpenCV (2.3) 
2. no Matlab! 
3. easy to compile and run (on linux, work in progress on OSX and windows) 
4. fast! (and more potential to be much faster) 
5. No Matlab! (did I say no matlab?) 


没有matlab，多平台，更快的速度。真好，就是我想做的，不过他现在这个程序，在我这电脑上实现速度还不行。他代码的下载地址为[https://github.com/alantrrs/OpenTLD](http://www.google.com/url?sa=D&q=https://github.com/alantrrs/OpenTLD&usg=AFQjCNFU8PdcI0cQ79FR5TF6Lk04D6GePw)，好像打不开，我是在这上面得到的[https://github.com/arthurv/OpenTLD](http://www.google.com/url?sa=D&q=https://github.com/arthurv/OpenTLD&usg=AFQjCNGDXLUFLgmCuf4Np-Tr6241h_G6fA)。不过是个Linux版本的。大家这么强，改改肯定就可以在xp下跑起来了，反正我是搞定了。

附上他软件的设计接口。真是赏心悦目啊，不得不说人家做的东西很规范，惭愧惭愧

![](http://hi.csdn.net/attachment/201112/21/0_1324453220t66t.gif)


看不清还是下载下来大家自己看咯，不好意思，我不知道怎么能传上去看得清晰

从他这个设计图也可以看出来我之前几讲分析的流程还是可以接受的，init部分和track部分是相对独立的。而比较复杂的是learning的部分和detect部分。下一步主攻这两部分了。学习的越深入，越是发现自己很挫，都没信心继续写下去了。硬着头皮装大葱吧~



